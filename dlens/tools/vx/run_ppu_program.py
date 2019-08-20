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

    # Playback program
    builder = sta.PlaybackProgramBuilder()

    # enable reset chip pin
    builder.write(halco.ResetChipOnDLS(), True)
    builder.write(halco.TimerOnDLS(), 0)
    builder.wait_until(halco.TimerOnDLS(), 10)

    # disable reset chip pin
    builder.write(halco.ResetChipOnDLS(), False)
    builder.write(halco.TimerOnDLS(), 0)
    builder.wait_until(halco.TimerOnDLS(), 100)

    # write JTAG clock scaler, reset JTAG TAP
    builder.write(halco.JTAGClockScalerOnDLS(), 3)
    builder.write(halco.ResetJTAGTapOnDLS(), hal.ResetJTAGTap())

    # write ADPLL config via JTAG
    for adpll_coord in halco.iter_all(halco.ADPLLOnDLS):
        builder.write(adpll_coord, hal.ADPLL())

    # write PLLClockOutputBlock config via JTAG
    builder.write(
        halco.PLLClockOutputBlockOnDLS(), hal.PLLClockOutputBlock())
    builder.write(halco.TimerOnDLS(), 0)
    builder.wait_until(halco.TimerOnDLS(),
                       hal.Timer.Value.fpga_clock_cycles_per_us * 100)

    # write, execute PPU program and read back results
    builder.write(halco.PPUMemoryOnDLS(), program)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_start)
    builder.write(halco.TimerOnDLS(), 0)
    builder.wait_until(halco.TimerOnDLS(), wait)
    status_handle = builder.read(halco.PPUStatusRegisterOnDLS())
    builder.write(halco.PPUControlRegisterOnDLS(), ppu_control_reg_end)
    program_handle = builder.read(halco.PPUMemoryOnDLS())
    mailbox_handle = builder.read(
        halco.PPUMemoryBlockOnDLS(halco.PPUMemoryBlockOnPPU.mailbox))

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
        halco.PPUMemoryWordOnPPU.return_code].value)
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
