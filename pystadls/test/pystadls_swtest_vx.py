#!/usr/bin/env python

import unittest

from pyhalco_common import iter_all
import pyhalco_hicann_dls_vx as halco
import pyhaldls_vx as haldls
import pystadls_vx as stadls
import pyfisch_vx as fisch


class TestPystadlsVx(unittest.TestCase):
    def test_pb_program_builder_dumper(self):
        builder = stadls.PlaybackProgramBuilderDumper()

        # reset chip
        builder.write(halco.ResetChipOnDLS(), haldls.ResetChip(True))
        builder.write(halco.TimerOnDLS(), haldls.Timer())
        builder.block_until(halco.TimerOnDLS(), 10)
        tmp = (halco.ResetChipOnDLS(), haldls.ResetChip(False))
        builder.write(*tmp)

        # write shiftreg container
        shiftreg = haldls.ShiftRegister()
        builder.write(halco.ShiftRegisterOnBoard(), shiftreg)

        all_leds = list(iter_all(halco.LEDOnBoard))
        for led in all_leds + list(reversed(all_leds)):
            builder.write(halco.TimerOnDLS(), haldls.Timer())
            shiftreg.set_enable_led(led, True)
            builder.write(halco.ShiftRegisterOnBoard(), shiftreg)
            builder.block_until(halco.TimerOnDLS(),
                                int(fisch.fpga_clock_cycles_per_us * 1e6 / 8))
            shiftreg.set_enable_led(led, False)
        builder.write(halco.ShiftRegisterOnBoard(), shiftreg)

        builder.block_until(halco.BarrierOnFPGA(), haldls.Barrier.omnibus)

        # pop playback program
        program = builder.done()

        self.assertEqual(len(program), 5 + len(all_leds) * 2 * 3 + 2,
                         "Wrong number of coordinate/container pairs")

        self.assertNotEqual(program.tolist()[0], tmp)
        self.assertNotEqual(program.tolist()[1], tmp)
        self.assertNotEqual(program.tolist()[2], tmp)
        self.assertEqual(program.tolist()[3], tmp)
        self.assertNotEqual(program.tolist()[4], tmp)

        builder = stadls.PlaybackProgramBuilder()
        for cor, con in program.tolist():
            if isinstance(cor, (halco.TimerOnDLS, halco.BarrierOnFPGA)) and \
               isinstance(con, (haldls.Timer.Value, haldls.Barrier)):
                builder.block_until(cor, con)
            else:
                builder.write(cor, con)


if __name__ == "__main__":
    unittest.main()
