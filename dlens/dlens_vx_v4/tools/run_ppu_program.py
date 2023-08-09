import ctypes
import argparse

from dlens_vx_v4 import halco
from dlens_vx_v4.sta import PlaybackProgramBuilder, DigitalInit, \
    generate, run
from dlens_vx_v4.hal import PPUControlRegister, Timer, Barrier
from dlens_vx_v4.lola import PPUElfFile
from dlens_vx_v4.hxcomm import ConnectionHandle, ManagedConnection

_DEFAULT_PPU = halco.PPUOnDLS(0)


class PPUTimeoutError(Exception):
    pass


def load_and_start_program(connection: ConnectionHandle,
                           binary_path: str,
                           ppu: halco.PPUOnDLS = _DEFAULT_PPU) -> None:
    """
    Load a PPU program and trigger its execution.

    :param connection: Connected conection to be used for loading and starting
                       the program.
    :param binary_path: Path to the unstripped (*.bin) program to be loaded
    :param ppu: PPU the program is started on.
    """
    builder = PlaybackProgramBuilder()

    ppu_control_reg_run = PPUControlRegister()
    ppu_control_reg_run.inhibit_reset = True

    ppu_control_reg_reset = PPUControlRegister()
    ppu_control_reg_reset.inhibit_reset = False

    program_file = PPUElfFile(binary_path)
    program = program_file.read_program()
    program_on_ppu = halco.PPUMemoryBlockOnPPU(
        halco.PPUMemoryWordOnPPU(0),
        halco.PPUMemoryWordOnPPU(program.internal.size() - 1)
    )
    program_on_fpga = None
    if program.external:
        program_on_fpga = halco.ExternalPPUMemoryBlockOnFPGA(
            halco.ExternalPPUMemoryByteOnFPGA(0),
            halco.ExternalPPUMemoryByteOnFPGA(program.external.size() - 1)
        )

    program_on_dls = halco.PPUMemoryBlockOnDLS(program_on_ppu, ppu)

    # Ensure PPU is in reset state
    builder.write(ppu.toPPUControlRegisterOnDLS(), ppu_control_reg_reset)

    # Write PPU program
    builder.write(program_on_dls, program.internal)
    if program_on_fpga:
        builder.write(program_on_fpga, program.external)

    # Set PPU to run state, start execution
    builder.write(ppu.toPPUControlRegisterOnDLS(), ppu_control_reg_run)
    run(connection, builder.done())


def stop_program(connection: ConnectionHandle,
                 print_mailbox: bool = True,
                 ppu: halco.PPUOnDLS = _DEFAULT_PPU) -> int:
    """
    Stop the PPU and evaluate the exit code. Optionally, read back the mailbox
    and print it.

    :param connection: Connection to be used for stopping the program.
    :param print_mailbox: Read back and print the mailbox as string to stdout.
    :param ppu: PPU the program is stopped on.
    :return Exit code of the program
    """
    ppu_control_reg_end = PPUControlRegister()
    ppu_control_reg_end.inhibit_reset = False

    builder = PlaybackProgramBuilder()
    builder.write(ppu.toPPUControlRegisterOnDLS(), ppu_control_reg_end)
    return_handle = builder.read(
        halco.PPUMemoryWordOnDLS(halco.PPUMemoryWordOnPPU.return_code, ppu))

    if print_mailbox:
        mailbox_handle = builder.read(
            halco.PPUMemoryBlockOnDLS(halco.PPUMemoryBlockOnPPU.mailbox, ppu))
    else:
        mailbox_handle = None

    # Wait for all read responses to arrive
    builder.block_until(halco.BarrierOnFPGA(), Barrier.omnibus)

    # Run builder
    run(connection, builder.done())

    # Print Mailbox
    if print_mailbox:
        print(mailbox_handle.get().to_string())

    # Return the exit code
    return ctypes.c_int32(int(return_handle.get().value)).value


def wait_until_ppu_finished(connection: ConnectionHandle,
                            timeout: int = None,
                            ppu: halco.PPUOnDLS = _DEFAULT_PPU) -> None:
    """
    Poll the PPU status register until program has finished. An optional
    timeout may be specified.

    :param connection: Connection to be used for polling the ppu status
                       register
    :param timeout: Timeout for the PPU program execution. Any number of
                    FPGA cycles may be given, which correspond to the
                    number of cycles the FPGA is actively waiting.
                    The time needed for communication with the FPGA is
                    neglected.
    :param ppu: PPU whose completion is awaited.
    :raises PPUTimeoutError: PPU did not finish within max_polls
    """
    per_poll_wait = 1000
    max_num_polls = timeout // per_poll_wait if timeout else None

    poll_builder = PlaybackProgramBuilder()
    status_handle = poll_builder.read(ppu.toPPUStatusRegisterOnDLS())
    poll_builder.write(halco.TimerOnDLS(), Timer(0))
    poll_builder.block_until(
        halco.TimerOnDLS(), Timer.Value(per_poll_wait))
    poll_builder.block_until(halco.BarrierOnFPGA(), Barrier.omnibus)
    poll_program = poll_builder.done()

    run(connection, poll_program)
    num_polls = 1
    while status_handle.get().sleep is not True:
        run(connection, poll_program)

        if timeout is not None and num_polls > max_num_polls:
            raise PPUTimeoutError("PPU execution did not finish in time.")

        num_polls += 1


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Execute PPU programs")
    parser.add_argument('program', type=str)
    parser.add_argument(
        '--wait', type=int, default=int(1E7),
        help="Timeout for the PPU program execution. Any number of FPGA "
             "cycles may be given, which correspond to the number of cycles "
             "the FPGA is actively waiting. The time needed for communication "
             "with the FPGA is neglected.")
    parser.add_argument('--ppu_id', type=int, default=_DEFAULT_PPU.toEnum(),
                        help="PPU ID used for program execution.")
    args = parser.parse_args()

    with ManagedConnection() as conn:
        init_builder, _ = generate(DigitalInit())
        run(conn, init_builder.done())

        load_and_start_program(conn, args.program,
                               halco.PPUOnDLS(args.ppu_id))
        wait_until_ppu_finished(conn, args.wait,
                                halco.PPUOnDLS(args.ppu_id))
        exit_code = stop_program(conn, ppu=halco.PPUOnDLS(args.ppu_id))

    if exit_code != 0:
        raise RuntimeError(f"PPU Program exited with exit code {exit_code}")
