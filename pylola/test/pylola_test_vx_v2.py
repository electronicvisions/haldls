#!/usr/bin/env python
import pickle
import os
import unittest
from random import randrange
import numpy as np
import pyhalco_hicann_dls_vx_v2 as halco
import pylola_vx_v2 as lola


def random_ranged(type_param):
    return randrange(type_param.min, type_param.max + 1)


class TestPylolaVXV2(unittest.TestCase):
    def test_import(self):
        self.assertIsNotNone(lola)

    # pylint warnings disabled below because attributes for nested
    # classes are loaded dynamically upon instantiation and therefore
    # not visible for pylint (which is an error).
    # pylint: disable=E1101,E1136,E1137
    def test_synapse_row(self):
        row = lola.SynapseRow()

        row.weights[13] = 27
        self.assertEqual(row.weights[13], 27)
        np_weights = row.weights.to_numpy()
        self.assertEqual(np_weights[13], 27)
        self.assertEqual(np_weights.shape,
                         (halco.SynapseOnSynapseRow.size,))
        np_weights[16] = 64
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            row.weights.from_numpy(np_weights)
        np_weights[16] = 4
        row.weights.from_numpy(np_weights)
        self.assertEqual(row.weights[16], 4)

        row.labels[14] = 28
        self.assertEqual(row.labels[14], 28)
        np_labels = row.labels.to_numpy()
        self.assertEqual(np_labels[14], 28)
        self.assertEqual(np_labels.shape,
                         (halco.SynapseOnSynapseRow.size,))
        np_labels[16] = 64
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            row.labels.from_numpy(np_labels)
        np_labels[16] = 4
        row.labels.from_numpy(np_labels)
        self.assertEqual(row.labels[16], 4)

        row.time_calibs[15] = 2
        self.assertEqual(row.time_calibs[15], 2)
        np_time_calibs = row.time_calibs.to_numpy()
        self.assertEqual(np_time_calibs[15], 2)
        self.assertEqual(np_time_calibs.shape,
                         (halco.SynapseOnSynapseRow.size,))
        np_time_calibs[16] = 4
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            row.time_calibs.from_numpy(np_time_calibs)
        np_time_calibs[16] = 3
        row.time_calibs.from_numpy(np_time_calibs)
        self.assertEqual(row.time_calibs[16], 3)

        row.amp_calibs[16] = 3
        self.assertEqual(row.amp_calibs[16], 3)
        np_amp_calibs = row.amp_calibs.to_numpy()
        self.assertEqual(np_amp_calibs[16], 3)
        self.assertEqual(np_amp_calibs.shape,
                         (halco.SynapseOnSynapseRow.size,))
        np_amp_calibs[16] = 4
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            row.amp_calibs.from_numpy(np_amp_calibs)
        np_amp_calibs[16] = 3
        row.amp_calibs.from_numpy(np_amp_calibs)
        self.assertEqual(row.amp_calibs[16], 3)

    def test_synapse_matrix(self):
        matrix = lola.SynapseMatrix()

        fill_weights = np.zeros((halco.SynapseRowOnSynram.size,
                                 halco.SynapseOnSynapseRow.size)) + 30
        matrix.weights.from_numpy(fill_weights)
        self.assertEqual(matrix.weights[8][15], 30)

        matrix.weights[7][1] = 9
        self.assertEqual(matrix.weights[7][1], 9)
        np_weights = matrix.weights.to_numpy()
        self.assertEqual(np_weights[7][1], 9)
        self.assertEqual(np_weights.shape,
                         (halco.SynapseRowOnSynram.size,
                          halco.SynapseOnSynapseRow.size))
        np_weights[13][37] = 64
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            matrix.weights.from_numpy(np_weights)
        np_weights[13][37] = 5
        matrix.weights.from_numpy(np_weights)
        self.assertEqual(matrix.weights[13][37], 5)

        matrix.labels[14][14] = 28
        self.assertEqual(matrix.labels[14][14], 28)
        np_labels = matrix.labels.to_numpy()
        self.assertEqual(np_labels[14][14], 28)
        self.assertEqual(np_labels.shape,
                         (halco.SynapseRowOnSynram.size,
                          halco.SynapseOnSynapseRow.size))
        np_labels[16][16] = 64
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            matrix.labels.from_numpy(np_labels)
        np_labels[16][16] = 4
        matrix.labels.from_numpy(np_labels)
        self.assertEqual(matrix.labels[16][16], 4)

        matrix.time_calibs[15][15] = 2
        self.assertEqual(matrix.time_calibs[15][15], 2)
        np_time_calibs = matrix.time_calibs.to_numpy()
        self.assertEqual(np_time_calibs[15][15], 2)
        self.assertEqual(np_time_calibs.shape,
                         (halco.SynapseRowOnSynram.size,
                          halco.SynapseOnSynapseRow.size))
        np_time_calibs[16][16] = 4
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            matrix.time_calibs.from_numpy(np_time_calibs)
        np_time_calibs[16][16] = 3
        matrix.time_calibs.from_numpy(np_time_calibs)
        self.assertEqual(matrix.time_calibs[16][16], 3)

        matrix.amp_calibs[16][16] = 3
        self.assertEqual(matrix.amp_calibs[16][16], 3)
        np_amp_calibs = matrix.amp_calibs.to_numpy()
        self.assertEqual(np_amp_calibs[16][16], 3)
        self.assertEqual(np_amp_calibs.shape,
                         (halco.SynapseRowOnSynram.size,
                          halco.SynapseOnSynapseRow.size))
        np_amp_calibs[16][16] = 4
        with self.assertRaisesRegex((RuntimeError, OverflowError),
                                    r"range overflow: (\d+) > max\((\d+)\)"):
            matrix.amp_calibs.from_numpy(np_amp_calibs)
        np_amp_calibs[16][16] = 3
        matrix.amp_calibs.from_numpy(np_amp_calibs)
        self.assertEqual(matrix.amp_calibs[16][16], 3)

    def test_ppu_elf_file(self):
        this_dir = os.path.dirname(os.path.realpath(__file__))
        elf_file = lola.PPUElfFile(
            os.environ.get('TEST_PPU_PROGRAM',
                           os.path.join(this_dir,
                                        os.pardir,
                                        os.pardir,
                                        os.pardir,
                                        'build',
                                        'haldls',
                                        'lola_ppu_test_elf_file.bin')))
        # all numbers below might and will change on change in build-profile,
        # compiler or runtime
        program_size = 113
        self.assertEqual(elf_file.read_program().size(), program_size)

        symbols = elf_file.read_symbols()
        # get these numbers by powerpc-ppu-nm <program>
        symbol_a_position = halco.PPUMemoryBlockOnPPU(
            halco.PPUMemoryWordOnPPU(113), halco.PPUMemoryWordOnPPU(113))
        self.assertEqual(symbols["a"], lola.PPUProgram.Symbol(
            lola.PPUProgram.Symbol.Type.object, symbol_a_position))

    def test_pickle(self):
        for container in lola.containers:
            obj = container()
            dump = pickle.dumps(obj)
            self.assertEqual(obj, pickle.loads(dump))

    def test_neuron_synaptic_input_exc(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.excitatory_input.enable, False)
        neuron.excitatory_input.enable = True
        self.assertEqual(neuron.excitatory_input.enable, True)

        self.assertEqual(neuron.excitatory_input.i_bias_tau, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.excitatory_input.i_bias_tau = value
        self.assertEqual(neuron.excitatory_input.i_bias_tau, value)

        self.assertEqual(neuron.excitatory_input.i_drop_input, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.excitatory_input.i_drop_input = value
        self.assertEqual(neuron.excitatory_input.i_drop_input, value)

        self.assertEqual(neuron.excitatory_input.i_shift_reference, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.excitatory_input.i_shift_reference = value
        self.assertEqual(neuron.excitatory_input.i_shift_reference, value)

        self.assertEqual(neuron.excitatory_input.i_bias_gm, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.excitatory_input.i_bias_gm = value
        self.assertEqual(neuron.excitatory_input.i_bias_gm, value)

        self.assertEqual(neuron.excitatory_input.enable_small_capacitor, True)
        neuron.excitatory_input.enable_small_capacitor = False
        self.assertEqual(neuron.excitatory_input.enable_small_capacitor, False)

        self.assertEqual(neuron.excitatory_input.enable_high_resistance, False)
        neuron.excitatory_input.enable_high_resistance = True
        self.assertEqual(neuron.excitatory_input.enable_high_resistance, True)

    def test_neuron_synaptic_input_inh(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.inhibitory_input.enable, False)
        neuron.inhibitory_input.enable = True
        self.assertEqual(neuron.inhibitory_input.enable, True)

        self.assertEqual(neuron.inhibitory_input.i_bias_tau, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_bias_tau = value
        self.assertEqual(neuron.inhibitory_input.i_bias_tau, value)

        self.assertEqual(neuron.inhibitory_input.i_drop_input, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_drop_input = value
        self.assertEqual(neuron.inhibitory_input.i_drop_input, value)

        self.assertEqual(neuron.inhibitory_input.i_shift_reference, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_shift_reference = value
        self.assertEqual(neuron.inhibitory_input.i_shift_reference, value)

        self.assertEqual(neuron.inhibitory_input.i_bias_gm, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_bias_gm = value
        self.assertEqual(neuron.inhibitory_input.i_bias_gm, value)

        self.assertEqual(neuron.inhibitory_input.enable_small_capacitor, True)
        neuron.inhibitory_input.enable_small_capacitor = False
        self.assertEqual(neuron.inhibitory_input.enable_small_capacitor, False)

        self.assertEqual(neuron.inhibitory_input.enable_high_resistance, False)
        neuron.inhibitory_input.enable_high_resistance = True
        self.assertEqual(neuron.inhibitory_input.enable_high_resistance, True)

    def test_neuron_leak(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.leak.v_leak, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.leak.v_leak = value
        self.assertEqual(neuron.leak.v_leak, value)

        self.assertEqual(neuron.leak.i_bias, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.leak.i_bias = value
        self.assertEqual(neuron.leak.i_bias, value)

        self.assertEqual(neuron.leak.enable_degeneration, False)
        neuron.leak.enable_degeneration = True
        self.assertEqual(neuron.leak.enable_degeneration, True)

        self.assertEqual(neuron.leak.enable_division, False)
        neuron.leak.enable_division = True
        self.assertEqual(neuron.leak.enable_division, True)

        self.assertEqual(neuron.leak.enable_multiplication, False)
        neuron.leak.enable_multiplication = True
        self.assertEqual(neuron.leak.enable_multiplication, True)

    def test_neuron_reset(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.reset.v_reset, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.reset.v_reset = value
        self.assertEqual(neuron.reset.v_reset, value)

        self.assertEqual(neuron.reset.i_bias, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.reset.i_bias = value
        self.assertEqual(neuron.reset.i_bias, value)

        self.assertEqual(neuron.reset.enable_degeneration, False)
        neuron.reset.enable_degeneration = True
        self.assertEqual(neuron.reset.enable_degeneration, True)

        self.assertEqual(neuron.reset.enable_division, False)
        neuron.reset.enable_division = True
        self.assertEqual(neuron.reset.enable_division, True)

        self.assertEqual(neuron.reset.enable_multiplication, False)
        neuron.reset.enable_multiplication = True
        self.assertEqual(neuron.reset.enable_multiplication, True)

    def test_neuron_threshold(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.threshold.v_threshold, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.threshold.v_threshold = value
        self.assertEqual(neuron.threshold.v_threshold, value)

        self.assertEqual(neuron.threshold.enable, False)
        neuron.threshold.enable = True
        self.assertEqual(neuron.threshold.enable, True)

    def test_neuron_constant(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.constant_current.i_offset, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.constant_current.i_offset = value
        self.assertEqual(neuron.constant_current.i_offset, value)

        self.assertEqual(neuron.constant_current.enable, False)
        neuron.constant_current.enable = True
        self.assertEqual(neuron.constant_current.enable, True)

        self.assertEqual(
            neuron.constant_current.type, neuron.constant_current.Type.source
        )
        neuron.constant_current.type = neuron.constant_current.Type.sink
        self.assertEqual(
            neuron.constant_current.type, neuron.constant_current.Type.sink
        )

    def test_neuron_multicompartment(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.multicompartment.connect_soma, False)
        neuron.multicompartment.connect_soma = True
        self.assertEqual(neuron.multicompartment.connect_soma, True)

        self.assertEqual(neuron.multicompartment.connect_soma_right, False)
        neuron.multicompartment.connect_soma_right = True
        self.assertEqual(neuron.multicompartment.connect_soma_right, True)

        self.assertEqual(neuron.multicompartment.connect_right, False)
        neuron.multicompartment.connect_right = True
        self.assertEqual(neuron.multicompartment.connect_right, True)

        self.assertEqual(neuron.multicompartment.connect_vertical,
                         False)
        neuron.multicompartment.connect_vertical = True
        self.assertEqual(neuron.multicompartment.connect_vertical,
                         True)

        self.assertEqual(neuron.multicompartment.enable_conductance_division,
                         False)
        neuron.multicompartment.enable_conductance_division = True
        self.assertEqual(neuron.multicompartment.enable_conductance_division,
                         True)

        self.assertEqual(
            neuron.multicompartment.enable_conductance_multiplication,
            False)
        neuron.multicompartment.enable_conductance_multiplication = True
        self.assertEqual(
            neuron.multicompartment.enable_conductance_multiplication,
            True)

    def test_neuron_membrane(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.membrane_capacitance.capacitance, 0)
        value = random_ranged(
            lola.AtomicNeuron.MembraneCapacitance.CapacitorSize)
        neuron.membrane_capacitance.capacitance = value
        self.assertEqual(neuron.membrane_capacitance.capacitance, value)

    def test_neuron_adaptation(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.adaptation.enable, False)
        neuron.adaptation.enable = True
        self.assertEqual(neuron.adaptation.enable, True)

        self.assertEqual(neuron.adaptation.invert_a, False)
        neuron.adaptation.invert_a = True
        self.assertEqual(neuron.adaptation.invert_a, True)

        self.assertEqual(neuron.adaptation.invert_b, False)
        neuron.adaptation.invert_b = True
        self.assertEqual(neuron.adaptation.invert_b, True)

        self.assertEqual(neuron.adaptation.v_ref, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.adaptation.v_ref = value
        self.assertEqual(neuron.adaptation.v_ref, value)

        self.assertEqual(neuron.adaptation.v_leak, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.adaptation.v_leak = value
        self.assertEqual(neuron.adaptation.v_leak, value)

        self.assertEqual(neuron.adaptation.i_bias_tau, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.adaptation.i_bias_tau = value
        self.assertEqual(neuron.adaptation.i_bias_tau, value)

        self.assertEqual(neuron.adaptation.i_bias_a, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.adaptation.i_bias_a = value
        self.assertEqual(neuron.adaptation.i_bias_a, value)

        self.assertEqual(neuron.adaptation.i_bias_b, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.adaptation.i_bias_b = value
        self.assertEqual(neuron.adaptation.i_bias_b, value)

    def test_neuron_exponential(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.exponential.enable, False)
        neuron.exponential.enable = True
        self.assertEqual(neuron.exponential.enable, True)

        self.assertEqual(neuron.exponential.v_exp, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.exponential.v_exp = value
        self.assertEqual(neuron.exponential.v_exp, value)

        self.assertEqual(neuron.exponential.i_bias, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.exponential.i_bias = value
        self.assertEqual(neuron.exponential.i_bias, value)

    def test_neuron_event_routing(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(
            neuron.event_routing.analog_output,
            neuron.event_routing.AnalogOutputMode.off
        )
        neuron.event_routing.analog_output = \
            neuron.event_routing.AnalogOutputMode.normal
        self.assertEqual(
            neuron.event_routing.analog_output,
            neuron.event_routing.AnalogOutputMode.normal
        )
        neuron.event_routing.analog_output = \
            neuron.event_routing.AnalogOutputMode.strong
        self.assertEqual(
            neuron.event_routing.analog_output,
            neuron.event_routing.AnalogOutputMode.strong
        )

        self.assertEqual(neuron.event_routing.enable_digital, False)
        neuron.event_routing.enable_digital = True
        self.assertEqual(neuron.event_routing.enable_digital, True)

        self.assertEqual(neuron.event_routing.enable_bypass_excitatory, False)
        neuron.event_routing.enable_bypass_excitatory = True
        self.assertEqual(neuron.event_routing.enable_bypass_excitatory, True)

        self.assertEqual(neuron.event_routing.enable_bypass_inhibitory, False)
        neuron.event_routing.enable_bypass_inhibitory = True
        self.assertEqual(neuron.event_routing.enable_bypass_inhibitory, True)

        self.assertEqual(neuron.event_routing.enable_post_overwrite, False)
        neuron.event_routing.enable_post_overwrite = True
        self.assertEqual(neuron.event_routing.enable_post_overwrite, True)

        self.assertEqual(neuron.event_routing.address, 0)
        value = random_ranged(lola.AtomicNeuron.EventRouting.Address)
        neuron.event_routing.address = value
        self.assertEqual(neuron.event_routing.address, value)

    def test_neuron_readout(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.readout.enable_buffered_access, False)
        neuron.readout.enable_buffered_access = True
        self.assertEqual(neuron.readout.enable_buffered_access, True)

        self.assertEqual(neuron.readout.enable_amplifier, False)
        neuron.readout.enable_amplifier = True
        self.assertEqual(neuron.readout.enable_amplifier, True)

        self.assertEqual(neuron.readout.enable_unbuffered_access, False)
        neuron.readout.enable_unbuffered_access = True
        self.assertEqual(neuron.readout.enable_unbuffered_access, True)

        self.assertEqual(neuron.readout.source, neuron.Readout.Source.membrane)
        value = lola.AtomicNeuron.Readout.Source.adaptation
        neuron.readout.source = value
        self.assertEqual(neuron.readout.source, value)

    def test_neuron_refractory_period(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.refractory_period.refractory_time,
                         neuron.RefractoryPeriod.RefractoryTime())
        value = random_ranged(
            lola.AtomicNeuron.RefractoryPeriod.RefractoryTime)
        neuron.refractory_period.refractory_time = value
        self.assertEqual(neuron.refractory_period.refractory_time, value)

        self.assertEqual(neuron.refractory_period.reset_holdoff,
                         neuron.RefractoryPeriod.ResetHoldoff())
        value = random_ranged(
            lola.AtomicNeuron.RefractoryPeriod.ResetHoldoff)
        neuron.refractory_period.reset_holdoff = value
        self.assertEqual(neuron.refractory_period.reset_holdoff, value)

        self.assertEqual(neuron.refractory_period.input_clock,
                         lola.AtomicNeuron.RefractoryPeriod.InputClock())
        value = random_ranged(
            lola.AtomicNeuron.RefractoryPeriod.InputClock)
        neuron.refractory_period.input_clock = value
        self.assertEqual(neuron.refractory_period.input_clock, value)

        self.assertEqual(neuron.refractory_period.enable_pause, False)
        neuron.refractory_period.enable_pause = True
        self.assertEqual(neuron.refractory_period.enable_pause, True)

    def test_neuron_bayesian(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.bayesian.enable, False)
        neuron.bayesian.enable = True
        self.assertEqual(neuron.bayesian.enable, True)

        self.assertEqual(neuron.bayesian.connect_fire_vertical, False)
        neuron.bayesian.connect_fire_vertical = True
        self.assertEqual(neuron.bayesian.connect_fire_vertical, True)

        self.assertEqual(neuron.bayesian.connect_fire_to_right, False)
        neuron.bayesian.connect_fire_to_right = True
        self.assertEqual(neuron.bayesian.connect_fire_to_right, True)

        self.assertEqual(neuron.bayesian.connect_fire_from_right, False)
        neuron.bayesian.connect_fire_from_right = True
        self.assertEqual(neuron.bayesian.connect_fire_from_right, True)

        self.assertEqual(neuron.bayesian.enable_master, True)
        neuron.bayesian.enable_master = False
        self.assertEqual(neuron.bayesian.enable_master, False)

        self.assertEqual(neuron.bayesian.enable_slave, False)
        neuron.bayesian.enable_slave = True
        self.assertEqual(neuron.bayesian.enable_slave, True)

        self.assertEqual(neuron.bayesian.enable_0, False)
        neuron.bayesian.enable_0 = True
        self.assertEqual(neuron.bayesian.enable_0, True)

        self.assertEqual(neuron.bayesian.enable_1, False)
        neuron.bayesian.enable_1 = True
        self.assertEqual(neuron.bayesian.enable_1, True)


if __name__ == "__main__":
    unittest.main()
