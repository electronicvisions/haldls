#!/usr/bin/env python

import argparse
import unittest
from pyhalco_common import iter_all
import pyhalco_hicann_dls_vx as halco
import pyhaldls_vx as haldls
import pyfisch_vx as fisch

fpga_ip=""

class HwTestPyhaldlsVx(unittest.TestCase):
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
            builder.wait_until(halco.TimerOnDLS(), int(fisch.fpga_clock_cycles_per_us * 1e6 / 8))
            shiftreg.set_enable_led(led, False)
        builder.write(halco.ShiftRegisterOnBoard(), shiftreg)
        builder.halt()

        # pop playback program
        program = builder.done()

        # execute playback program
        executor = fisch.PlaybackProgramARQExecutor(fpga_ip)
        executor.transfer(program.impl())
        executor.execute()
        executor.fetch(program.impl())


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--fpga_ip", type=str)
    args, unknownargs = parser.parse_known_args()

    if (len(unknownargs)):
        unittest.main(argv=unknownargs)
    else:
        unittest.main(argv=[""])
