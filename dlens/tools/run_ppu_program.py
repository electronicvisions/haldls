import ctypes
import argparse
from dlens_v2 import hal, halco, sta


class PPUTimeoutError(Exception):
    pass


def run_program(
        program_path: str, as_string: bool = False,
        wait: int = int(1E7), board_id: str = None) -> int:
    """
    :param program_path: PPU program binary
    :param board_id: Optional FlySPI ID to connect to
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

    # Playback memory program
    builder = hal.PlaybackProgramBuilder()
    builder.write(halco.PPUMemoryOnDLS(), program)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_start)
    builder.set_time(0)
    builder.wait_until(wait)
    status_handle = builder.read(halco.PPUStatusRegisterOnDLS())
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    program_handle = builder.read(halco.PPUMemoryOnDLS())
    mailbox_handle = builder.read(halco.PPUMemoryBlockOnDLS.mailbox)
    builder.halt()

    # Connect
    if not board_id:
        executor = sta.ExperimentControl()
    else:
        executor = sta.LocalBoardControl(board_id)

    # Run
    executor.run_experiment(hal.Board(), hal.Chip(), builder.done())

    # Read mailbox
    if as_string:
        print(mailbox_handle.get().to_string())
    else:
        print(mailbox_handle.get())

    # Check status register
    status_reg_result = status_handle.get()
    if status_reg_result.sleep is not True:
        raise PPUTimeoutError("PPU Program did not finish")

    # Last word before mailbox is the exit code
    raw_code = int(program_handle.get().words[
        halco.PPUMemoryWordOnDLS.return_code].value)
    return ctypes.c_int32(raw_code).value


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Execute PPU programs")
    parser.add_argument('program', type=str)
    parser.add_argument('--board_id', type=str, default=None)
    parser.add_argument('--as_string', action='store_true')
    parser.add_argument(
        '--wait',
        type=int,
        default=int(1E7),
        help="Number of FPGA cycles after which the PPU is stopped")
    args = parser.parse_args()

    code = run_program(args.program, args.as_string, args.wait, args.board_id)
    if code != 0:
        raise RuntimeError("PPU Program exited with exit code {}".format(code))
