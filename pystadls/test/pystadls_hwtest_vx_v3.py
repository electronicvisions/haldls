#!/usr/bin/env python

import unittest
import numpy

from pyhalco_common import iter_all
import pyhalco_hicann_dls_vx_v3 as halco
import pyhaldls_vx_v3 as haldls
import pystadls_vx_v3 as stadls
import pyfisch_vx as fisch
import pyhxcomm_vx as hxcomm


class HwTestPystadlsVxV3(unittest.TestCase):
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
        Samples are recorded continuously, in order to test more
        samples than configurable in the MADC config.
        """

        builder, _ = stadls.DigitalInit().generate()

        madc_config = haldls.MADCConfig()
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
        madc_control.enable_power_down_after_sampling = False
        madc_control.enable_continuous_sampling = True
        builder.write(halco.MADCControlOnDLS(), madc_control)

        # wait for samples
        sampling_time = 40000  # us
        builder.block_until(halco.TimerOnDLS(), haldls.Timer.Value(
            (initial_wait + sampling_time)
            * int(haldls.Timer.Value.fpga_clock_cycles_per_us)))

        # turn off MADC
        madc_control.start_recording = False
        madc_control.stop_recording = True
        madc_control.enable_power_down_after_sampling = True
        builder.write(halco.MADCControlOnDLS(), madc_control)

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

        # assert number of received samples is as expected,
        # assuming a sampling rate of 30 MHz
        sampling_rate = 30  # MHz
        expected_number_of_samples = \
            sampling_time * sampling_rate  # us * MHz cancels

        self.assertEqual(samples.ndim, 1)
        self.assertGreater(
            len(samples), int(expected_number_of_samples * 0.95),
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
        diff = numpy.diff(samples["value"].astype(int))
        self.assertAlmostEqual(
            numpy.mean(diff[diff > 0]), 1, places=1,
            msg="MADC sample values don't increase by 1"
            + "on average in sawtooth pattern.")
        self.assertLess(
            numpy.mean(diff[diff < 0]), -1000,
            "MADC samples don't show proper falling edge in sawtooth pattern.")

    def test_madc_sample_rate(self):
        """
        Observe the MADC sampling rate in dependency of the MADC configuration,
        then compare it to a calculated sample rate based on the configuration.
        """
        # define (arbitrary) test values for the MADC config
        # defaults are tested via Nones
        madc_clock_scale_values = [None, 10, 50, 100, 200, 4095]
        sample_duration_adjusts = [0, None, 7]
        enable_madc_clock_scalings = [True, False]

        # sweep over configuration space and sample dumpy data
        for madc_clock_scale_value in madc_clock_scale_values:
            for sample_duration_adjust in sample_duration_adjusts:
                for enable_madc_clock_scaling in enable_madc_clock_scalings:
                    builder, _ = stadls.DigitalInit().generate()

                    # configure MADC
                    madc_config = haldls.MADCConfig()
                    madc_config.number_of_samples = 20000
                    madc_config.enable_dummy_data = True
                    madc_config.enable_madc_clock_scaling = \
                        enable_madc_clock_scaling
                    if sample_duration_adjust is not None:
                        madc_config.sample_duration_adjust = \
                            sample_duration_adjust
                    if madc_clock_scale_value is not None:
                        madc_config.madc_clock_scale_value = \
                            madc_clock_scale_value
                    builder.write(halco.MADCConfigOnDLS(), madc_config)

                    # calculate expected sample rate from MADC config (Hz)
                    adpll = haldls.ADPLL()
                    output = adpll.Output(adpll.Output.dco)  # dco
                    madc_base_frequ = adpll.calculate_output_frequency(output)
                    calc_rate = madc_config.calculate_sample_rate(
                        madc_base_frequ) / 1e6  # MHz

                    # wake up MADC
                    madc_control = haldls.MADCControl()
                    madc_control.enable_power_down_after_sampling = False
                    madc_control.start_recording = False
                    madc_control.wake_up = True
                    builder.write(halco.MADCControlOnDLS(), madc_control)

                    # initial wait, systime sync
                    initial_wait = 100  # us
                    builder.write(halco.TimerOnDLS(), haldls.Timer())
                    builder.write(halco.SystimeSyncOnFPGA(),
                                  haldls.SystimeSync())
                    builder.block_until(halco.TimerOnDLS(), haldls.Timer.Value(
                        initial_wait
                        * int(haldls.Timer.Value.fpga_clock_cycles_per_us)))

                    # trigger MADC sampling, power MADC down once done
                    madc_control.wake_up = False
                    madc_control.start_recording = True
                    madc_control.enable_power_down_after_sampling = True
                    builder.write(halco.MADCControlOnDLS(), madc_control)

                    # wait for samples and additional 3 ms
                    sampling_time = float(madc_config.number_of_samples) / \
                        calc_rate + 3000  # us
                    builder.block_until(halco.TimerOnDLS(), haldls.Timer.Value(
                        (initial_wait + int(sampling_time))
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
                    samples = numpy.sort(samples,
                                         order=["chip_time", "fpga_time"])
                    # convert to SI units (us)
                    sample_times = samples["chip_time"] / \
                        (float(haldls.Timer.Value.fpga_clock_cycles_per_us))
                    # get time between two samples
                    time_between_samples = numpy.diff(sample_times)
                    # calculate average sample rate from measured samples (MHz)
                    average_rate = 1 / numpy.mean(time_between_samples)
                    # compare measurement with prediction and check that the
                    # error is less than 1%
                    self.assertAlmostEqual(
                        average_rate, calc_rate, delta=calc_rate * 0.01,
                        msg="Recorded rate deviates by more than 1% from "
                            "the calculated one.")


if __name__ == "__main__":
    unittest.main()
