#!/usr/bin/env python

import argparse
import unittest
from pyhalco_common import iter_all
import pyhalco_hicann_dls_vx as halco
import pyhaldls_vx as haldls
import pyfisch_vx as fisch


class HwTestPyhaldlsVx(unittest.TestCase):
    fpga_ip = "192.168.4.4"

    def test_board_led_chain(self):
        all_leds = [led for led in iter_all(halco.LEDOnBoard)]

        builder = haldls.PlaybackProgramBuilder()

        # reset chip
        builder.write(halco.ResetChipOnDLS(), haldls.ResetChip(True))
        builder.write(halco.TimerOnDLS(), haldls.Timer())
        builder.wait_until(halco.TimerOnDLS(), 10)
        builder.write(halco.ResetChipOnDLS(), haldls.ResetChip(False))

        # write shiftreg container
        shiftreg = haldls.ShiftRegister()
        builder.write(halco.ShiftRegisterOnBoard(), shiftreg)
        for led in all_leds + list(reversed(all_leds)):
            builder.write(halco.TimerOnDLS(), haldls.Timer())
            shiftreg.set_enable_led(led, True)
            builder.write(halco.ShiftRegisterOnBoard(), shiftreg)
            builder.wait_until(halco.TimerOnDLS(),
                               int(fisch.fpga_clock_cycles_per_us * 1e6 / 8))
            shiftreg.set_enable_led(led, False)
        builder.write(halco.ShiftRegisterOnBoard(), shiftreg)

        # pop playback program
        program = builder.done()

        # execute playback program
        executor = fisch.PlaybackProgramARQExecutor(self.fpga_ip)
        executor.run(program.impl())


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--fpga_ip", type=str,
                        default=HwTestPyhaldlsVx.fpga_ip)
    args, unknownargs = parser.parse_known_args()

    HwTestPyhaldlsVx.fpga_ip = args.fpga_ip
    if unknownargs:
        unittest.main(argv=unknownargs)
    else:
        unittest.main(argv=[""])
