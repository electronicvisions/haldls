import ctypes
import argparse
from dlens_vx import hal, halco, sta


class PPUTimeoutError(Exception):
    pass


def run_program(program_path: str,
                as_string: bool = False,
                wait: int = int(1E7)) -> int:
    """
    :param program_path: Path to the PPU program binary
    :param as_string: Interpret mailbox content as string
    :param wait: Wait for n cycles before aborting the run
    :return: Exit code from program execution
    """
    # Load the data
    program = hal.PPUMemory()
    program.load_from_file(program_path)

    # PPU control register
    ppu_control_reg_start = hal.PPUControlRegister()
    ppu_control_reg_start.inhibit_reset = True

    ppu_control_reg_end = hal.PPUControlRegister()
    ppu_control_reg_end.inhibit_reset = False

    # Chip initialization
    builder, _ = sta.generate(sta.InitGenerator())

    # write, execute PPU program and read back results
    builder.write(halco.PPUMemoryOnDLS(), program)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_start)
    builder.write(halco.TimerOnDLS(), hal.Timer(0))
    builder.wait_until(halco.TimerOnDLS(), wait)
    status_handle = builder.read(halco.PPUStatusRegisterOnDLS())
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    program_handle = builder.read(halco.PPUMemoryOnDLS())
    mailbox_handle = builder.read(
        halco.PPUMemoryBlockOnDLS(halco.PPUMemoryBlockOnPPU.mailbox))

    # Wait for all read responses to arrive
    builder.write(halco.TimerOnDLS(), hal.Timer(0))
    builder.wait_until(halco.TimerOnDLS(), 1000)

    # Connect and execute
    with sta.AutoConnection() as connection:
        connection.run(builder.done())

    # Read mailbox
    if as_string:
        print(mailbox_handle.get().to_string())
    else:
        print(mailbox_handle.get())

    # Check status register
    if status_handle.get().sleep is not True:
        raise PPUTimeoutError("PPU Program did not finish")

    # Extract exit code
    raw_code = int(program_handle.get().words[
        int(halco.PPUMemoryWordOnPPU.return_code)].value)
    return ctypes.c_int32(raw_code).value


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Execute PPU programs")
    parser.add_argument('program', type=str)
    parser.add_argument('--as_string', action='store_true')
    parser.add_argument(
        '--wait', type=int, default=int(1E7),
        help="Number of FPGA cycles after which the PPU is stopped")
    args = parser.parse_args()

    code = run_program(args.program, args.as_string, args.wait)
    if code != 0:
        raise RuntimeError("PPU Program exited with exit code {}".format(code))
