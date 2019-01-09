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
        neuron.set_enable_leak(True)
        chip.set_neuron_digital_config(C.NeuronOnDLS(5), neuron)
        self.assertEqual(chip.get_neuron_digital_config(C.NeuronOnDLS(5)), neuron)

        # test getter/setter: synapse
        synapse = Ct.SynapseBlock.Synapse()
        synapse.set_weight(Ct.SynapseBlock.Synapse.Weight(4))
        synapse.set_address(Ct.SynapseBlock.Synapse.Address(5))
        chip.set_synapse(C.SynapseOnDLS(Co.Enum(3)), synapse)
        self.assertEqual(chip.get_synapse(C.SynapseOnDLS(Co.Enum(3))), synapse)
        self.assertNotEqual(chip.get_synapse(C.SynapseOnDLS(Co.Enum(0))), synapse)

        # test getter/setter: synapse_block
        synapse_block = Ct.SynapseBlock()
        synapse_block.set_synapse(C.SynapseOnSynapseBlock(3), synapse)
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
        corr_switch.set_causal_config(Ct.ColumnCorrelationBlock.ColumnCorrelationSwitch.Config.internal)
        chip.set_column_correlation_switch(C.ColumnCorrelationSwitchOnDLS(3), corr_switch)
        self.assertEqual(chip.get_column_correlation_switch(C.ColumnCorrelationSwitchOnDLS(3)), corr_switch)

        corr_block = Ct.ColumnCorrelationBlock()
        corr_block.set_switch(C.ColumnCorrelationSwitchOnColumnBlock(3), corr_switch)
        chip.set_column_correlation_block(C.ColumnBlockOnDLS(7), corr_block)
        self.assertEqual(chip.get_column_correlation_block(C.ColumnBlockOnDLS(7)), corr_block)
        # test if single correlation switch setter modifies correct block
        self.assertEqual(chip.get_column_correlation_block(C.ColumnBlockOnDLS(0)), corr_block)

        curr_switch = Ct.ColumnCurrentBlock.ColumnCurrentSwitch()
        curr_switch.set_exc_config(Ct.ColumnCurrentBlock.ColumnCurrentSwitch.Config.internal)
        chip.set_column_current_switch(C.ColumnCurrentSwitchOnDLS(2), curr_switch)
        self.assertEqual(chip.get_column_current_switch(C.ColumnCurrentSwitchOnDLS(2)), curr_switch)

        curr_block = Ct.ColumnCurrentBlock()
        curr_block.set_switch(C.ColumnCurrentSwitchOnColumnBlock(2), curr_switch)
        chip.set_column_current_block(C.ColumnBlockOnDLS(6), curr_block)
        self.assertEqual(chip.get_column_current_block(C.ColumnBlockOnDLS(6)), curr_block)
        # test if single current switch setter modifies correct block
        self.assertEqual(chip.get_column_current_block(C.ColumnBlockOnDLS(0)), curr_block)

        capmem = Ct.CapMem()
        capmem.set(C.CapMemCellOnDLS(Co.Enum(4)), Ct.CapMemCell.Value(123))
        chip.set_capmem(capmem)
        self.assertEqual(chip.get_capmem(), capmem)

        ppu_memory = Ct.PPUMemory()
        test_mem = []
        for word in C.iter_all(C.PPUMemoryWordOnDLS):
            test_mem.append(Ct.PPUMemoryWord(Ct.PPUMemoryWord.Value(word.value())))
        ppu_memory.set_words(test_mem)
        chip.set_ppu_memory(ppu_memory)
        self.assertEqual(chip.get_ppu_memory(), ppu_memory)

        control_register = Ct.PPUControlRegister()
        control_register.set_inhibit_reset(True)
        chip.set_ppu_control_register(control_register)
        self.assertEqual(chip.get_ppu_control_register(), control_register)

        rate = Ct.RateCounterConfig()
        rate.set_enable_neuron(C.NeuronOnDLS(4), True)
        chip.set_rate_counter_config(rate)
        self.assertEqual(chip.get_rate_counter_config(), rate)

        syndriver_config = Ct.SynapseDriverBlock()
        syndriver_config.set_pulse_length(Ct.SynapseDriverBlock.PulseLength(22))
        chip.set_synapse_drivers(syndriver_config)
        self.assertEqual(chip.get_synapse_drivers(), syndriver_config)

        synram_config = Ct.CommonSynramConfig()
        synram_config.set_pc_conf(Ct.CommonSynramConfig.PCConf(14))
        chip.set_common_synram_config(synram_config)
        self.assertEqual(chip.get_common_synram_config(), synram_config)

        capmem_config = Ct.CapMemConfig()
        capmem_config.set_sub_counter(Ct.CapMemConfig.SubCounter(1234))
        chip.set_capmem_config(capmem_config)
        self.assertEqual(chip.get_capmem_config(), capmem_config)

        neuron_config = Ct.CommonNeuronConfig()
        neuron_config.set_post_correlation_signal_length(
            Ct.CommonNeuronConfig.PostCorrelationSignalLength(12))
        chip.set_common_neuron_config(neuron_config)
        self.assertEqual(chip.get_common_neuron_config(), neuron_config)

        correlation_config = Ct.CorrelationConfig()
        correlation_config.set_sense_delay(Ct.CorrelationConfig.Delay(12))
        chip.set_correlation_config(correlation_config)
        self.assertEqual(chip.get_correlation_config(), correlation_config)

        chip2 = Ct.Chip(chip)
        chip3 = Ct.Chip()
        capmem_config.set_sub_counter(Ct.CapMemConfig.SubCounter(4321))
        chip3.set_capmem_config(capmem_config)

        self.assertEqual(chip, chip2)
        self.assertFalse(chip == chip3)

        self.assertNotEqual(chip, chip3)
        self.assertFalse(chip != chip2)

        # check neuron current input and voltage output setter
        test_neuron_coord = C.NeuronOnDLS(14)
        test_neuron_coord2 = C.NeuronOnDLS(10)

        self.assertFalse(bool(chip.get_buffered_readout_neuron()))
        chip.enable_buffered_readout(test_neuron_coord)
        self.assertTrue(bool(chip.get_buffered_readout_neuron()))

        self.assertEqual(chip.get_buffered_readout_neuron(), test_neuron_coord)

        for neuron in C.iter_all(C.NeuronOnDLS):
            config = chip.get_neuron_digital_config(neuron)
            self.assertFalse(config.get_enable_buffered_readout())

        chip.enable_buffered_readout(test_neuron_coord2)
        self.assertEqual(chip.get_buffered_readout_neuron(), test_neuron_coord2)

        for neuron in C.iter_all(C.NeuronOnDLS):
            config = chip.get_neuron_digital_config(neuron)
            self.assertFalse(config.get_enable_buffered_readout())

        self.assertEqual(chip.get_buffered_readout_neuron(), test_neuron_coord2)

        config2 = chip.get_neuron_digital_config(test_neuron_coord2)
        self.assertFalse(config2.get_enable_buffered_readout())

        chip.set_neuron_digital_config(test_neuron_coord, config2)
        config = chip.get_neuron_digital_config(test_neuron_coord)
        self.assertFalse(config.get_enable_buffered_readout())
        self.assertEqual(config2, config)
        self.assertEqual(chip.get_buffered_readout_neuron(), test_neuron_coord2)

        chip.disable_buffered_readout()
        self.assertFalse(chip.get_buffered_readout_neuron())


if __name__ == "__main__":
    unittest.main()
