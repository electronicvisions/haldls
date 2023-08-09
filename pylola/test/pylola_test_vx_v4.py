#!/usr/bin/env python
import pickle
import os
import unittest
from random import randrange
from copy import deepcopy
import numpy as np
import pyhalco_hicann_dls_vx_v4 as halco
import pylola_vx_v4 as lola


def random_ranged(type_param):
    return randrange(type_param.min, type_param.max + 1)


# pylint: disable=too-many-public-methods
class TestPylolaVXV4(unittest.TestCase):
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
        program_internal_size = 42
        self.assertEqual(elf_file.read_program().internal.size(),
                         program_internal_size)

        mem = elf_file.read_program()
        ext_mem = mem.external
        if mem.external is not None:
            self.assertEqual(ext_mem, mem.external)

        mem = elf_file.read_program()
        old_ext_mem = mem.external
        self.assertIsNotNone(mem.external)
        mem.external = None
        ext_mem = mem.external
        self.assertEqual(ext_mem, mem.external)
        self.assertNotEqual(ext_mem, old_ext_mem)
        # TODO (Issue #3988): mem.external = None leads to "free" of
        # old_ext_mem
        # self.assertEqual(old_ext_mem, elf_file.read_program().external)

        symbols = elf_file.read_symbols()
        # get these numbers by powerpc-ppu-nm <program>
        symbol_a_position = halco.PPUMemoryBlockOnPPU(
            halco.PPUMemoryWordOnPPU(38), halco.PPUMemoryWordOnPPU(38))
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

        self.assertEqual(neuron.excitatory_input.i_bias_coba, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.excitatory_input.i_bias_coba = value
        self.assertEqual(neuron.excitatory_input.i_bias_coba, value)

        self.assertEqual(neuron.excitatory_input.i_shift_reference, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.excitatory_input.i_shift_reference = value
        self.assertEqual(neuron.excitatory_input.i_shift_reference, value)

        self.assertEqual(neuron.excitatory_input.i_bias_gm, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.excitatory_input.i_bias_gm = value
        self.assertEqual(neuron.excitatory_input.i_bias_gm, value)

        self.assertEqual(
            neuron.excitatory_input.enable_small_capacitance, True)
        neuron.excitatory_input.enable_small_capacitance = False
        self.assertEqual(
            neuron.excitatory_input.enable_small_capacitance, False)

        self.assertEqual(neuron.excitatory_input.enable_high_resistance, False)
        neuron.excitatory_input.enable_high_resistance = True
        self.assertEqual(neuron.excitatory_input.enable_high_resistance, True)

        self.assertEqual(neuron.excitatory_input.enable_coba_mode, False)
        neuron.excitatory_input.enable_coba_mode = True
        self.assertEqual(neuron.excitatory_input.enable_coba_mode, True)

    def test_neuron_synaptic_input_inh(self):
        neuron = lola.AtomicNeuron()

        self.assertEqual(neuron.inhibitory_input.enable, False)
        neuron.inhibitory_input.enable = True
        self.assertEqual(neuron.inhibitory_input.enable, True)

        self.assertEqual(neuron.inhibitory_input.i_bias_tau, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_bias_tau = value
        self.assertEqual(neuron.inhibitory_input.i_bias_tau, value)

        self.assertEqual(neuron.inhibitory_input.i_bias_coba, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_bias_coba = value
        self.assertEqual(neuron.inhibitory_input.i_bias_coba, value)

        self.assertEqual(neuron.inhibitory_input.i_shift_reference, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_shift_reference = value
        self.assertEqual(neuron.inhibitory_input.i_shift_reference, value)

        self.assertEqual(neuron.inhibitory_input.i_bias_gm, 0)
        value = random_ranged(lola.AtomicNeuron.AnalogValue)
        neuron.inhibitory_input.i_bias_gm = value
        self.assertEqual(neuron.inhibitory_input.i_bias_gm, value)

        self.assertEqual(
            neuron.inhibitory_input.enable_small_capacitance, True)
        neuron.inhibitory_input.enable_small_capacitance = False
        self.assertEqual(
            neuron.inhibitory_input.enable_small_capacitance, False)

        self.assertEqual(neuron.inhibitory_input.enable_high_resistance, False)
        neuron.inhibitory_input.enable_high_resistance = True
        self.assertEqual(neuron.inhibitory_input.enable_high_resistance, True)

        self.assertEqual(neuron.inhibitory_input.enable_coba_mode, False)
        neuron.inhibitory_input.enable_coba_mode = True
        self.assertEqual(neuron.inhibitory_input.enable_coba_mode, True)

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

        self.assertEqual(neuron.bayesian.to_post_pulse, True)
        neuron.bayesian.to_post_pulse = False
        self.assertEqual(neuron.bayesian.to_post_pulse, False)

        self.assertEqual(neuron.bayesian.operation,
                         neuron.bayesian.Operation.local)
        neuron.bayesian.operation = neuron.bayesian.Operation.AND
        self.assertEqual(neuron.bayesian.operation,
                         neuron.bayesian.Operation.AND)
        neuron.bayesian.operation = neuron.bayesian.Operation.XNOR
        self.assertEqual(neuron.bayesian.operation,
                         neuron.bayesian.Operation.XNOR)
        neuron.bayesian.operation = neuron.bayesian.Operation.MULLER_C
        self.assertEqual(neuron.bayesian.operation,
                         neuron.bayesian.Operation.MULLER_C)

        self.assertEqual(neuron.bayesian.connect_fire_vertical, False)
        neuron.bayesian.connect_fire_vertical = True
        self.assertEqual(neuron.bayesian.connect_fire_vertical, True)

        self.assertEqual(neuron.bayesian.connect_fire_to_right, False)
        neuron.bayesian.connect_fire_to_right = True
        self.assertEqual(neuron.bayesian.connect_fire_to_right, True)

        self.assertEqual(neuron.bayesian.connect_fire_from_right, False)
        neuron.bayesian.connect_fire_from_right = True
        self.assertEqual(neuron.bayesian.connect_fire_from_right, True)

        self.assertEqual(neuron.bayesian.enable_master, False)
        neuron.bayesian.enable_master = True
        self.assertEqual(neuron.bayesian.enable_master, True)

        self.assertEqual(neuron.bayesian.enable_slave, False)
        neuron.bayesian.enable_slave = True
        self.assertEqual(neuron.bayesian.enable_slave, True)

    def test_morphology_builder(self):
        builder = lola.Morphology()
        moco = halco.AtomicNeuronOnLogicalNeuron
        row = halco.NeuronRowOnLogicalNeuron
        col = halco.NeuronColumnOnLogicalNeuron

        # test all member functions of the builder class Morphology
        builder.connect_to_soma(moco(0, 0))
        self.assertEqual(builder[moco(0, 0)].connect_soma, True)
        self.assertEqual(builder[moco(0, 0)].enable_conductance, False)

        builder.connect_resistor_to_soma(moco(0, 1))
        self.assertEqual(builder[moco(0, 1)].connect_soma, False)
        self.assertEqual(builder[moco(0, 1)].enable_conductance, True)

        builder.create_compartment([moco(0, 0), moco(0, 1)])
        self.assertEqual(builder[moco(0, 0)].connect_vertical, True)
        self.assertEqual(builder[moco(0, 1)].connect_vertical, True)

        builder.connect_soma_line(col.min, col.max, row(0))
        # to conntect 128 compartments we need 127 interconnections
        for column_idx in range(127):
            self.assertEqual(builder[moco(column_idx, 0)].connect_soma_right,
                             True)
        self.assertEqual(builder[moco(127, 0)].connect_soma_right, False)

    def test_morphology_connect_comp(self):
        builder = lola.Morphology()
        moco = halco.AtomicNeuronOnLogicalNeuron

        # morph_chain: ▀▀█▄█▀
        horizontal_chain_row1_seg1 = [moco(0, 0), moco(1, 0), moco(2, 0)]
        horizontal_chain_row1_seg2 = [moco(4, 0), moco(5, 0)]
        horizontal_chain_row2_seg1 = [moco(2, 1), moco(3, 1), moco(4, 1)]
        segments = horizontal_chain_row1_seg1 + horizontal_chain_row2_seg1 + \
            horizontal_chain_row1_seg2
        vertical_chain_segs = [moco(2, 0), moco(2, 1), moco(4, 0), moco(4, 1)]

        _ = builder.create_compartment(segments)

        # check that connections are set
        for idx in range(len(horizontal_chain_row1_seg1) - 1):
            self.assertEqual(
                builder[horizontal_chain_row1_seg1[idx]].connect_right, True)
        for idx in range(len(horizontal_chain_row1_seg2) - 1):
            self.assertEqual(
                builder[horizontal_chain_row1_seg2[idx]].connect_right, True)
        for idx in range(len(horizontal_chain_row2_seg1) - 1):
            self.assertEqual(
                builder[horizontal_chain_row2_seg1[idx]].connect_right, True)
        for idx in range(len(vertical_chain_segs) - 1):
            self.assertEqual(
                builder[vertical_chain_segs[idx]].connect_vertical, True)

        # test: violating contiguousness (by deleting elements within the
        # chain created above)
        for segment in segments[1:-1]:
            incorrect_builder = lola.Morphology()
            tmp_segments = deepcopy(segments)
            tmp_segments.remove(segment)
            self.assertRaises(RuntimeError, incorrect_builder.
                              create_compartment, tmp_segments)

    def test_logical_neuron(self):
        moco = halco.AtomicNeuronOnLogicalNeuron
        row = halco.NeuronRowOnLogicalNeuron
        col = halco.NeuronColumnOnLogicalNeuron

        # test the done routine for incomplete morphologies
        builder = lola.Morphology()
        builder.create_compartment([moco(0, 0), moco(0, 1)])
        builder.connect_to_soma(moco(2, 0))
        # until now builder is not describing a contiguous morphology
        self.assertRaises(RuntimeError, builder.done)
        builder.connect_soma_line(col(0), col(2), row(0))
        self.assertRaises(RuntimeError, builder.done)
        # now it is contiguous
        builder.connect_resistor_to_soma(moco(0, 0))
        _, _ = builder.done()

        # another test for done routine with a neuron whose connect_soma_right
        # is set but is not a bridge neuron
        builder2 = lola.Morphology()
        builder2.create_compartment([moco(0, 0), moco(0, 1)])
        # 'random' bridge neuron -> should not be allowed config
        builder2.connect_soma_line(col(20), col(21), row(0))
        self.assertRaises(RuntimeError, builder2.done)

    def test_illegal_construction(self):
        moco = halco.AtomicNeuronOnLogicalNeuron
        row = halco.NeuronRowOnLogicalNeuron
        col = halco.NeuronColumnOnLogicalNeuron

        # follwing morphology is tested
        # +-+-+-+  <- somatic line
        # R |   |  <- connection to somatic line
        # | |   |
        # N-N N-N  <- neuron
        # 0 0 ? ?  <- Compartment membership
        builder1 = lola.Morphology()
        builder1.create_compartment([moco(0, 0), moco(1, 0)])
        builder1.create_compartment([moco(2, 0), moco(3, 0)])
        builder1.create_compartment([moco(5, 0)])
        builder1.connect_to_soma(moco(3, 0))
        builder1.connect_resistor_to_soma(moco(0, 0))
        builder1.connect_soma_line(col(0), col(3), row(0))
        builder1.connect_to_soma(moco(1, 0))

        self.assertRaises(RuntimeError, builder1.done)

        # follwing morphology is tested
        #   +-+  <- somatic line
        #   | |  <- connection to somatic line
        #   | |
        # N-N N  <- neuron
        # 0 0 ?  <- Compartment membership
        builder2 = lola.Morphology()
        builder2.create_compartment([moco(0, 0), moco(1, 0)])
        builder2.connect_to_soma(moco(1, 0))
        builder2.connect_to_soma(moco(2, 0))
        builder2.connect_soma_line(col(1), col(2), row(0))

        self.assertRaises(RuntimeError, builder2.done)

        # follwing morphology is tested
        # +-+-+-+  <- somatic line
        # |     R  <- connection to somatic line
        # |     |
        # N   N-N  <- neuron
        # 0   1 1  <- Compartment membership
        builder3 = lola.Morphology()
        builder3.create_compartment([moco(2, 0), moco(3, 0)])
        builder3.connect_resistor_to_soma(moco(3, 0))
        builder3.connect_to_soma(moco(0, 0))
        builder3.connect_soma_line(col(0), col(3), row(0))

        builder3.done()


if __name__ == "__main__":
    unittest.main()
