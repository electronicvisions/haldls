#!/usr/bin/env python

import unittest
import numpy

from pyhalco_common import iter_all
import pyhalco_hicann_dls_vx as halco
import pyhaldls_vx_v1 as haldls
import pystadls_vx_v1 as stadls
import pyfisch_vx as fisch
import pyhxcomm_vx as hxcomm


class HwTestPystadlsVxV1(unittest.TestCase):
    @classmethod
    def test_board_led_chain(cls):
        all_leds = list(iter_all(halco.LEDOnBoard))

        builder = stadls.PlaybackProgramBuilder()

        # reset chip
        builder.write(halco.ResetChipOnDLS(), haldls.ResetChip(True))
        builder.write(halco.TimerOnDLS(), haldls.Timer())
        builder.block_until(halco.TimerOnDLS(), 10)
        builder.write(halco.ResetChipOnDLS(), haldls.ResetChip(False))

        # write shiftreg container
        shiftreg = haldls.ShiftRegister()
        builder.write(halco.ShiftRegisterOnBoard(), shiftreg)
        for led in all_leds + list(reversed(all_leds)):
            builder.write(halco.TimerOnDLS(), haldls.Timer())
            shiftreg.set_enable_led(led, True)
            builder.write(halco.ShiftRegisterOnBoard(), shiftreg)
            builder.block_until(halco.TimerOnDLS(),
                                int(fisch.fpga_clock_cycles_per_us * 1e6 / 8))
            shiftreg.set_enable_led(led, False)
        builder.write(halco.ShiftRegisterOnBoard(), shiftreg)

        # pop playback program
        program = builder.done()

        # execute playback program
        with hxcomm.ManagedConnection() as conn:
            stadls.run(conn, program)

    # pylint: disable=R0914
    @unittest.skip("Issue #3578")
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
                builder.block_until(halco.TimerOnDLS(), haldls.Timer.Value(10))
                to_fpga_spike_labels.extend(labels)

        builder.write(halco.TimerOnDLS(), haldls.Timer())
        builder.block_until(halco.TimerOnDLS(), haldls.Timer.Value(1000))
        program = builder.done()

        with hxcomm.ManagedConnection() as conn:
            stadls.run(conn, program)

        spikes = program.spikes

        # assert 98 % of sent spikes are received again
        total_spikes_sent = sum(numpy.array(spike_pack_types)[:, 0]) \
            * num_spikes
        self.assertGreater(len(spikes), total_spikes_sent * 0.98)

        spikes = program.spikes.to_numpy()
        self.assertGreater(spikes.shape[0], total_spikes_sent * 0.98)

        # count incorrectly received spikes (unknown label)
        incorrect_spikes = numpy.sum(
            [haldls.SpikeLabel(spike["label"])
             not in to_fpga_spike_labels for spike in spikes])
        self.assertLess(incorrect_spikes, 100,
                        "Too many random spikes were received.")

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

    def test_madc_samples(self):
        """
        Observe an MADC test pattern and assert it is accessible
        through the madc_samples.to_numpy() interface.
        """

        builder, _ = stadls.DigitalInit().generate()

        madc_config = haldls.MADCConfig()
        madc_config.number_of_samples = 10000
        madc_config.enable_dummy_data = True
        builder.write(halco.MADCConfigOnDLS(), madc_config)

        # wake up MADC
        madc_control = haldls.MADCControl()
        madc_control.enable_power_down_after_sampling = False
        madc_control.start_recording = False
        madc_control.wake_up = True
        builder.write(halco.MADCControlOnDLS(), madc_control)

        # initial wait, systime sync
        initial_wait = 100  # us
        builder.write(halco.TimerOnDLS(), haldls.Timer())
        builder.write(halco.SystimeSyncOnFPGA(), haldls.SystimeSync())
        builder.block_until(halco.TimerOnDLS(), haldls.Timer.Value(
            initial_wait * int(haldls.Timer.Value.fpga_clock_cycles_per_us)))

        # trigger MADC sampling, power MADC down once done
        madc_control.wake_up = False
        madc_control.start_recording = True
        madc_control.enable_power_down_after_sampling = True
        builder.write(halco.MADCControlOnDLS(), madc_control)

        # wait for samples
        builder.block_until(halco.TimerOnDLS(), haldls.Timer.Value(
            (initial_wait + 500)
            * int(haldls.Timer.Value.fpga_clock_cycles_per_us)))

        # run program
        program = builder.done()
        with hxcomm.ManagedConnection() as conn:
            stadls.run(conn, program)

        # inspect MADC samples
        samples = program.madc_samples.to_numpy()

        # drop incorrectly received samples (value 0, issue 3545)
        # then sort by chip time
        samples = samples[samples["value"] != 0]
        samples = numpy.sort(samples, order=["chip_time", "fpga_time"])

        self.assertEqual(samples.ndim, 1)
        self.assertGreater(
            len(samples), int(madc_config.number_of_samples) * 0.98,
            "Too few MADC samples recorded.")

        expected_dtype = numpy.dtype([
            ('value', numpy.uint64),
            ('fpga_time', numpy.uint64),
            ('chip_time', numpy.uint64)
        ])
        self.assertEqual(
            samples.dtype, expected_dtype,
            "Expected samples.dtype to be {}".format(str(expected_dtype)))

        # assert sawtooth test pattern is visible in samples
        diff = numpy.diff(samples["value"].astype(numpy.int))
        self.assertAlmostEqual(
            numpy.mean(diff[diff > 0]), 1, places=1,
            msg="MADC sample values don't increase by 1"
            + "on average in sawtooth pattern.")
        self.assertLess(
            numpy.mean(diff[diff < 0]), -1000,
            "MADC samples don't show proper falling edge in sawtooth pattern.")


if __name__ == "__main__":
    unittest.main()
