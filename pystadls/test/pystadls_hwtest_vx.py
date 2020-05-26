#!/usr/bin/env python

import unittest
import numpy

from pyhalco_common import iter_all
import pyhalco_hicann_dls_vx as halco
import pyhaldls_vx as haldls
import pystadls_vx as stadls
import pyfisch_vx as fisch


class HwTestPystadlsVx(unittest.TestCase):
    @classmethod
    def test_board_led_chain(cls):
        all_leds = list(iter_all(halco.LEDOnBoard))

        builder = stadls.PlaybackProgramBuilder()

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
        executor = stadls.PlaybackProgramExecutor()
        executor.connect()
        executor.run(program)

    # pylint: disable=R0914
    def test_event(self):
        # ported from C++ test-event
        sequence = stadls.DigitalInit()
        builder, _ = sequence.generate()

        num_spikes = 1000

        spike_pack_types = [
            [1, halco.SpikePack1ToChipOnDLS, haldls.SpikePack1ToChip],
            [2, halco.SpikePack2ToChipOnDLS, haldls.SpikePack2ToChip],
            [3, halco.SpikePack3ToChipOnDLS, haldls.SpikePack3ToChip]]
        to_fpga_spike_labels = []
        # TODO: introspection would be nicer than hard-coding the types
        for spike_type_count, spike_type_coord, spike_type in spike_pack_types:
            for i in range(num_spikes):
                # TODO: random numbers would be nicer here
                labels = [haldls.SpikeLabel(i)] * spike_type_count
                spike = spike_type(labels)
                builder.write(spike_type_coord(), spike)
                builder.write(halco.TimerOnDLS(), haldls.Timer())
                builder.wait_until(halco.TimerOnDLS(), haldls.Timer.Value(10))
                to_fpga_spike_labels.extend(labels)

        builder.write(halco.TimerOnDLS(), haldls.Timer())
        builder.wait_until(halco.TimerOnDLS(), haldls.Timer.Value(1000))
        program = builder.done()

        executor = stadls.PlaybackProgramExecutor()
        executor.connect()
        executor.run(program)

        spikes = program.spikes

        # assert 98 % of sent spikes are received again
        total_spikes_sent = sum(numpy.array(spike_pack_types)[:, 0]) \
            * num_spikes
        self.assertGreater(len(spikes), total_spikes_sent * 0.98)

        spikes = program.spikes.to_numpy()
        self.assertEqual(spikes.shape, (total_spikes_sent,))
        for spike in spikes:
            # otherwise it will raise
            to_fpga_spike_labels.index(haldls.SpikeLabel((spike["label"])))

        self.assertEqual(spikes.ndim, 1,
                         "Expected numpy-wrapped spikes to be 1D")
        dtype_tmp = numpy.dtype([
            ('label', numpy.uint16),
            ('fpga_time', numpy.uint64),
            ('chip_time', numpy.uint64)
        ])
        self.assertEqual(spikes.dtype, dtype_tmp,
                         "Expect spikes.dtype to be {}".format(str(dtype_tmp)))
        self.assertGreater(spikes.size, total_spikes_sent * 0.98,
                           "Expected at least {} received spikes".format(
                               total_spikes_sent * 0.98))


if __name__ == "__main__":
    unittest.main()
