#!/usr/bin/env python

import unittest
import pyhalco_common as Co
import pyhalco_hicann_dls_v2 as C
import pyhaldls_v2 as Ct


class TestPyhaldlsV2Chip(unittest.TestCase):
    def setUp(self):
        self.chip = Ct.Chip()

    def test_neuron(self):
        neuron = Ct.NeuronDigitalConfig()
        neuron.enable_leak = True
        self.chip.set_neuron_digital_config(5, neuron)
        self.assertEqual(self.chip.get_neuron_digital_config(5), neuron)

    def test_synapses(self):
        # test getter/setter: synapse
        synapse = Ct.SynapseBlock.Synapse()
        synapse.weight = 4
        synapse.address = 5
        self.chip.set_synapse(C.SynapseOnDLS(Co.Enum(3)), synapse)
        self.assertEqual(self.chip.get_synapse(C.SynapseOnDLS(Co.Enum(3))),
                         synapse)
        self.assertNotEqual(self.chip.get_synapse(C.SynapseOnDLS(Co.Enum(0))),
                            synapse)

        # test getter/setter: synapse_block
        synapse_block = Ct.SynapseBlock()
        synapse_block.set_synapse(3, synapse)
        self.chip.set_synapse_block(C.SynapseBlockOnDLS(Co.X(0), Co.Y(1)),
                                    synapse_block)
        synapse_block_eq_1 = self.chip.get_synapse_block(
            C.SynapseBlockOnDLS(Co.X(0), Co.Y(1)))
        self.assertEqual(synapse_block_eq_1, synapse_block)
        # test if single synapse setter modifies correct block
        synapse_block_eq_2 = self.chip.get_synapse_block(
            C.SynapseBlockOnDLS(Co.X(0), Co.Y(0)))
        self.assertEqual(synapse_block_eq_2, synapse_block)
        # test if first synapse block in the third row are not equal
        synapse_block_ne = self.chip.get_synapse_block(
            C.SynapseBlockOnDLS(Co.X(0), Co.Y(2)))
        self.assertNotEqual(synapse_block_ne, synapse_block)

    def test_correlation_block(self):
        corr_switch = Ct.ColumnCorrelationBlock.ColumnCorrelationSwitch()
        corr_switch.causal_config = \
            Ct.ColumnCorrelationBlock.ColumnCorrelationSwitch.Config.internal
        self.chip.set_column_correlation_switch(3, corr_switch)
        self.assertEqual(self.chip.get_column_correlation_switch(3),
                         corr_switch)

        corr_block = Ct.ColumnCorrelationBlock()
        corr_block.set_switch(3, corr_switch)
        self.chip.set_column_correlation_block(7, corr_block)
        self.assertEqual(self.chip.get_column_correlation_block(7), corr_block)
        # test if single correlation switch setter modifies correct block
        self.assertEqual(self.chip.get_column_correlation_block(0), corr_block)

        curr_switch = Ct.ColumnCurrentBlock.ColumnCurrentSwitch()
        curr_switch.exc_config = \
            Ct.ColumnCurrentBlock.ColumnCurrentSwitch.Config.internal
        self.chip.set_column_current_switch(2, curr_switch)
        self.assertEqual(self.chip.get_column_current_switch(2), curr_switch)

        curr_block = Ct.ColumnCurrentBlock()
        curr_block.set_switch(2, curr_switch)
        self.chip.set_column_current_block(6, curr_block)
        self.assertEqual(self.chip.get_column_current_block(6), curr_block)
        # test if single current switch setter modifies correct block
        self.assertEqual(self.chip.get_column_current_block(0), curr_block)

    def test_capmem(self):
        capmem = Ct.CapMem()
        capmem.set(C.CapMemCellOnDLS(Co.Enum(4)), 123)
        self.chip.capmem = capmem
        self.assertEqual(self.chip.capmem, capmem)

    def test_ppu_memory(self):
        ppu_memory = Ct.PPUMemory()
        test_mem = []
        for word in C.iter_all(C.PPUMemoryWordOnDLS):
            test_mem.append(Ct.PPUMemoryWord(word.value()))
        ppu_memory.words = test_mem
        self.chip.ppu_memory = ppu_memory
        self.assertEqual(self.chip.ppu_memory, ppu_memory)

    def test_ppu_control(self):
        control_register = Ct.PPUControlRegister()
        control_register.inhibit_reset = True
        self.chip.ppu_control_register = control_register
        self.assertEqual(self.chip.ppu_control_register, control_register)

    def test_rate_counter(self):
        rate = Ct.RateCounterConfig()
        rate.set_enable_neuron(4, True)
        self.chip.rate_counter_config = rate
        self.assertEqual(self.chip.rate_counter_config, rate)

    def test_syndriver_config(self):
        syndriver_config = Ct.SynapseDriverBlock()
        syndriver_config.pulse_length = 22
        self.chip.synapse_drivers = syndriver_config
        self.assertEqual(self.chip.synapse_drivers, syndriver_config)

    def test_synram_config(self):
        synram_config = Ct.CommonSynramConfig()
        synram_config.pc_conf = 14
        self.chip.common_synram_config = synram_config
        self.assertEqual(self.chip.common_synram_config, synram_config)

    def test_capmem_config(self):
        capmem_config = Ct.CapMemConfig()
        capmem_config.sub_counter = 1234
        self.chip.capmem_config = capmem_config
        self.assertEqual(self.chip.capmem_config, capmem_config)

    def test_neuron_config(self):
        neuron_config = Ct.CommonNeuronConfig()
        neuron_config.post_correlation_signal_length = 12
        self.chip.common_neuron_config = neuron_config
        self.assertEqual(self.chip.common_neuron_config, neuron_config)

    def test_correlation_config(self):
        correlation_config = Ct.CorrelationConfig()
        correlation_config.sense_delay = 12
        self.chip.correlation_config = correlation_config
        self.assertEqual(self.chip.correlation_config, correlation_config)

    def test_chip_modification(self):
        chip2 = Ct.Chip(self.chip)
        chip3 = Ct.Chip()
        capmem_config = Ct.CapMemConfig()
        self.assertNotEqual(capmem_config.sub_counter, 4321)
        capmem_config.sub_counter = 4321
        chip3.capmem_config = capmem_config

        self.assertEqual(self.chip, chip2)
        self.assertFalse(self.chip == chip3)

        self.assertNotEqual(self.chip, chip3)
        self.assertFalse(self.chip != chip2)

    def test_neuron_io(self):
        test_neuron_coord = 14
        test_neuron_coord2 = 10

        self.assertFalse(bool(self.chip.buffered_readout_neuron))
        self.chip.enable_buffered_readout(test_neuron_coord)
        self.assertTrue(bool(self.chip.buffered_readout_neuron))

        self.assertEqual(self.chip.buffered_readout_neuron, test_neuron_coord)

        for neuron in C.iter_all(C.NeuronOnDLS):
            config = self.chip.get_neuron_digital_config(neuron)
            self.assertFalse(config.enable_buffered_readout)

        self.chip.enable_buffered_readout(test_neuron_coord2)
        self.assertEqual(self.chip.buffered_readout_neuron, test_neuron_coord2)

        for neuron in C.iter_all(C.NeuronOnDLS):
            config = self.chip.get_neuron_digital_config(neuron)
            self.assertFalse(config.enable_buffered_readout)

        self.assertEqual(self.chip.buffered_readout_neuron, test_neuron_coord2)

        config2 = self.chip.get_neuron_digital_config(test_neuron_coord2)
        self.assertFalse(config2.enable_buffered_readout)

        self.chip.set_neuron_digital_config(test_neuron_coord, config2)
        config = self.chip.get_neuron_digital_config(test_neuron_coord)
        self.assertFalse(config.enable_buffered_readout)
        self.assertEqual(config2, config)
        self.assertEqual(self.chip.buffered_readout_neuron, test_neuron_coord2)

        self.chip.disable_buffered_readout()
        self.assertFalse(self.chip.buffered_readout_neuron)

    def test_chain_assign_synram_conf(self):
        """
        Test that writing new values to common_synram_config.wait_ctr_clear
        actually changes the common_synram_config object.

        This has failed before because common_synram_config was returning a
        copy of wait_ctr_clear that would be modified but never set.
        """
        for val in Co.iter_all(self.chip.common_synram_config.WaitCtrClear):
            self.chip.common_synram_config.wait_ctr_clear = val

            self.assertIsInstance(
                self.chip.common_synram_config.wait_ctr_clear,
                self.chip.common_synram_config.WaitCtrClear)

            self.assertEqual(val,
                             self.chip.common_synram_config.wait_ctr_clear)


if __name__ == "__main__":
    unittest.main()
