import ctypes
import argparse
from dlens_vx import halco
from dlens_vx.sta import PlaybackProgramBuilder, PlaybackProgramExecutor, \
    AutoConnection
from pyhaldls_vx import PPUMemory, PPUControlRegister, Timer  # Issue 3429


class PPUTimeoutError(Exception):
    pass


def load_and_start_program(executor: PlaybackProgramExecutor,
                           binary_path: str) -> None:
    """
    Load a PPU program and trigger its execution.

    :param executor: Connected executor to be used for loading and starting the
                     program.
    :param binary_path: Path to the program to be loaded
    """
    program = PPUMemory()
    program.load_from_file(binary_path)

    ppu_control_reg_start = PPUControlRegister()
    ppu_control_reg_start.inhibit_reset = True

    ppu_control_reg_end = PPUControlRegister()
    ppu_control_reg_end.inhibit_reset = False

    builder = PlaybackProgramBuilder()
    builder.write(halco.PPUMemoryOnDLS(), program)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_start)

    executor.run(builder.done())


def stop_program(executor: PlaybackProgramExecutor) -> int:
    """
    Stop the PPU, print the memory contents and evaluate the exit code.

    :param executor: Connected executor to be used for stopping the program.
    :return Exit code of the program
    """
    ppu_control_reg_end = PPUControlRegister()
    ppu_control_reg_end.inhibit_reset = False

    builder = PlaybackProgramBuilder()
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    return_handle = builder.read(
        halco.PPUMemoryWordOnDLS(halco.PPUMemoryWordOnPPU.return_code,
                                 halco.PPUOnDLS()))
    mailbox_handle = builder.read(
        halco.PPUMemoryBlockOnDLS(halco.PPUMemoryBlockOnPPU.mailbox))

    # Wait for all read responses to arrive
    builder.write(halco.TimerOnDLS(), Timer(0))
    builder.wait_until(halco.TimerOnDLS(), 1000)

    # Run builder
    executor.run(builder.done())

    # Print Mailbox
    print(mailbox_handle.get().to_string())

    # Return the exit code
    return ctypes.c_int32(int(return_handle.get().value)).value


def wait_until_ppu_finished(executor: PlaybackProgramExecutor,
                            timeout: int = None) -> None:
    """
    Poll the PPU status register until program has finished. An optional
    timeout may be specified.

    :param executor: Connected executor to be used for polling the ppu status
                     register
    :param timeout: Timeout for the PPU program execution. Any number of
                    FPGA cycles may be given, which correspond to the
                    number of cycles the FPGA is actively waiting.
                    The time needed for communication with the FPGA is
                    neglected.
    :raises PPUTimeoutError: PPU did not finish within max_polls
    """
    per_poll_wait = 1000
    max_num_polls = timeout // per_poll_wait if timeout else None

    poll_builder = PlaybackProgramBuilder()
    status_handle = poll_builder.read(halco.PPUStatusRegisterOnDLS())
    poll_builder.write(halco.TimerOnDLS(), Timer(0))
    poll_builder.wait_until(halco.TimerOnDLS(), per_poll_wait)
    poll_program = poll_builder.done()

    executor.run(poll_program)
    num_polls = 1
    while status_handle.get().sleep is not True:
        executor.run(poll_program)

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
    args = parser.parse_args()

    with AutoConnection() as connection:
        load_and_start_program(connection, args.program)
        wait_until_ppu_finished(connection, args.wait)
        exit_code = stop_program(connection)

    if exit_code != 0:
        raise RuntimeError(f"PPU Program exited with exit code {exit_code}")
