#!/usr/bin/env python

import unittest
import pyhalco_common as Co
import pyhalco_hicann_dls_v2 as C
import pyhaldls_v2 as Ct


class TestPyhaldlsV2(unittest.TestCase):
    def test_chip(self):
        chip = Ct.Chip()

        # test getter/setter: neuron
        neuron = Ct.NeuronDigitalConfig()
        neuron.enable_leak = True
        chip.set_neuron_digital_config(5, neuron)
        self.assertEqual(chip.get_neuron_digital_config(5), neuron)

        # test getter/setter: synapse
        synapse = Ct.SynapseBlock.Synapse()
        synapse.weight = 4
        synapse.address = 5
        chip.set_synapse(C.SynapseOnDLS(Co.Enum(3)), synapse)
        self.assertEqual(chip.get_synapse(C.SynapseOnDLS(Co.Enum(3))), synapse)
        self.assertNotEqual(chip.get_synapse(C.SynapseOnDLS(Co.Enum(0))), synapse)

        # test getter/setter: synapse_block
        synapse_block = Ct.SynapseBlock()
        synapse_block.set_synapse(3, synapse)
        chip.set_synapse_block(C.SynapseBlockOnDLS(Co.X(0), Co.Y(1)), synapse_block)
        synapse_block_eq_1 = chip.get_synapse_block(C.SynapseBlockOnDLS(Co.X(0), Co.Y(1)))
        self.assertEqual(synapse_block_eq_1, synapse_block)
        # test if single synapse setter in line 26 modifies correct block
        synapse_block_eq_2 = chip.get_synapse_block(C.SynapseBlockOnDLS(Co.X(0), Co.Y(0)))
        self.assertEqual(synapse_block_eq_2, synapse_block)
        # test if first synapse block in the third row are not equal
        synapse_block_ne = chip.get_synapse_block(C.SynapseBlockOnDLS(Co.X(0), Co.Y(2)))
        self.assertNotEqual(synapse_block_ne, synapse_block)

        corr_switch = Ct.ColumnCorrelationBlock.ColumnCorrelationSwitch()
        corr_switch.causal_config = Ct.ColumnCorrelationBlock.ColumnCorrelationSwitch.Config.internal
        chip.set_column_correlation_switch(3, corr_switch)
        self.assertEqual(chip.get_column_correlation_switch(3), corr_switch)

        corr_block = Ct.ColumnCorrelationBlock()
        corr_block.set_switch(3, corr_switch)
        chip.set_column_correlation_block(7, corr_block)
        self.assertEqual(chip.get_column_correlation_block(7), corr_block)
        # test if single correlation switch setter modifies correct block
        self.assertEqual(chip.get_column_correlation_block(0), corr_block)

        curr_switch = Ct.ColumnCurrentBlock.ColumnCurrentSwitch()
        curr_switch.exc_config = Ct.ColumnCurrentBlock.ColumnCurrentSwitch.Config.internal
        chip.set_column_current_switch(2, curr_switch)
        self.assertEqual(chip.get_column_current_switch(2), curr_switch)

        curr_block = Ct.ColumnCurrentBlock()
        curr_block.set_switch(2, curr_switch)
        chip.set_column_current_block(6, curr_block)
        self.assertEqual(chip.get_column_current_block(6), curr_block)
        # test if single current switch setter modifies correct block
        self.assertEqual(chip.get_column_current_block(0), curr_block)

        capmem = Ct.CapMem()
        capmem.set(C.CapMemCellOnDLS(Co.Enum(4)), 123)
        chip.capmem = capmem
        self.assertEqual(chip.capmem, capmem)

        ppu_memory = Ct.PPUMemory()
        test_mem = []
        for word in C.iter_all(C.PPUMemoryWordOnDLS):
            test_mem.append(Ct.PPUMemoryWord(word.value()))
        ppu_memory.words = test_mem
        chip.ppu_memory = ppu_memory
        self.assertEqual(chip.ppu_memory, ppu_memory)

        control_register = Ct.PPUControlRegister()
        control_register.inhibit_reset = True
        chip.ppu_control_register = control_register
        self.assertEqual(chip.ppu_control_register, control_register)

        rate = Ct.RateCounterConfig()
        rate.set_enable_neuron(4, True)
        chip.rate_counter_config = rate
        self.assertEqual(chip.rate_counter_config, rate)

        syndriver_config = Ct.SynapseDriverBlock()
        syndriver_config.pulse_length = 22
        chip.synapse_drivers = syndriver_config
        self.assertEqual(chip.synapse_drivers, syndriver_config)

        synram_config = Ct.CommonSynramConfig()
        synram_config.pc_conf = 14
        chip.common_synram_config = synram_config
        self.assertEqual(chip.common_synram_config, synram_config)

        capmem_config = Ct.CapMemConfig()
        capmem_config.sub_counter = 1234
        chip.capmem_config = capmem_config
        self.assertEqual(chip.capmem_config, capmem_config)

        neuron_config = Ct.CommonNeuronConfig()
        neuron_config.post_correlation_signal_length = 12
        chip.common_neuron_config = neuron_config
        self.assertEqual(chip.common_neuron_config, neuron_config)

        correlation_config = Ct.CorrelationConfig()
        correlation_config.sense_delay = 12
        chip.correlation_config = correlation_config
        self.assertEqual(chip.correlation_config, correlation_config)

        chip2 = Ct.Chip(chip)
        chip3 = Ct.Chip()
        capmem_config.sub_counter = 4321
        chip3.capmem_config = capmem_config

        self.assertEqual(chip, chip2)
        self.assertFalse(chip == chip3)

        self.assertNotEqual(chip, chip3)
        self.assertFalse(chip != chip2)

        # check neuron current input and voltage output setter
        test_neuron_coord = 14
        test_neuron_coord2 = 10

        self.assertFalse(bool(chip.buffered_readout_neuron))
        chip.enable_buffered_readout(test_neuron_coord)
        self.assertTrue(bool(chip.buffered_readout_neuron))

        self.assertEqual(chip.buffered_readout_neuron, test_neuron_coord)

        for neuron in C.iter_all(C.NeuronOnDLS):
            config = chip.get_neuron_digital_config(neuron)
            self.assertFalse(config.enable_buffered_readout)

        chip.enable_buffered_readout(test_neuron_coord2)
        self.assertEqual(chip.buffered_readout_neuron, test_neuron_coord2)

        for neuron in C.iter_all(C.NeuronOnDLS):
            config = chip.get_neuron_digital_config(neuron)
            self.assertFalse(config.enable_buffered_readout)

        self.assertEqual(chip.buffered_readout_neuron, test_neuron_coord2)

        config2 = chip.get_neuron_digital_config(test_neuron_coord2)
        self.assertFalse(config2.enable_buffered_readout)

        chip.set_neuron_digital_config(test_neuron_coord, config2)
        config = chip.get_neuron_digital_config(test_neuron_coord)
        self.assertFalse(config.enable_buffered_readout)
        self.assertEqual(config2, config)
        self.assertEqual(chip.buffered_readout_neuron, test_neuron_coord2)

        chip.disable_buffered_readout()
        self.assertFalse(chip.buffered_readout_neuron)

    def test_chained_assignment_common_synram_config(self):
        """
        Test that writing new values to common_synram_config.wait_ctr_clear
        actually changes the common_synram_config object.

        This has failed before because common_synram_config was returning a
        copy of wait_ctr_clear that would be modified but never set.
        """
        chip = Ct.Chip()

        for val in Co.iter_all(chip.common_synram_config.WaitCtrClear):
            chip.common_synram_config.wait_ctr_clear = val

            self.assertIsInstance(chip.common_synram_config.wait_ctr_clear,
                                  chip.common_synram_config.WaitCtrClear)

            self.assertEqual(val, chip.common_synram_config.wait_ctr_clear)


if __name__ == "__main__":
    unittest.main()
