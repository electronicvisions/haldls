#!/usr/bin/env python

import unittest
import pyhalco_common as Co
import pyhalco_hicann_dls_v2 as C
import pyhaldls_v2 as Ct
import pystadls_v2 as IO

import pylogging as logger


OFFSET = 1000 # ~10 us
ISI = 2000    # ~20 us
NUM_SPIKES = 50


class TestPyhaldlsIOV2Hardware(unittest.TestCase):
    def test_playback(self):

        capmem_config = Ct.CapMem()
        for cell in C.iter_all(C.CapMemCellOnDLS):
            capmem_config.set(cell, Ct.CapMemCell.Value(cell.toEnum().value()))

        cell = C.CapMemCellOnDLS(Co.Enum(2))
        capmemvalue = Ct.CapMemCell.Value(334)
        capmem_config.set(
            C.CapMemCellOnDLS(Co.Enum(0)), Ct.CapMemCell.Value(123))
        capmem_config.set(
            C.CapMemCellOnDLS(Co.Enum(1)), Ct.CapMemCell.Value(321))
        capmem_config.set(cell, capmemvalue)

        builder = Ct.PlaybackProgramBuilder()
        builder.write(C.CapMemOnDLS(), capmem_config)
        builder.wait_until(100)
        capmem_ticket = builder.read(C.CapMemOnDLS(), capmem_config)
        capmemcell_ticket = builder.read(cell, Ct.CapMemCell())
        builder.halt()

        program = builder.done()

        # Builder is reset (starts a new program) every time .done() is called
        empty_new_program = builder.done()
        self.assertNotEqual(program.serial_number(), empty_new_program.serial_number())

        # No data available yet
        with self.assertRaises(RuntimeError):
            capmem_copy = program.get(capmem_ticket)
        with self.assertRaises(RuntimeError):
            capmemcell_copy = program.get(capmemcell_ticket)

        capmem_ticket_ = builder.read(C.CapMemOnDLS(), capmem_config)
        program_ = builder.done()

        # Using Ticket issued for a different program
        with self.assertRaises(ValueError):
            capmem_copy = program.get(capmem_ticket_)
        with self.assertRaises(ValueError):
            capmem_copy = program_.get(capmem_ticket)

        ctrl = IO.ExperimentControl()
        ctrl.run_experiment(Ct.Board(), Ct.Chip(), program)

        with self.assertRaises(ValueError):
            capmem_copy = program.get(capmem_ticket_)

        capmem_copy = program.get(capmem_ticket)
        capmemcell_copy = program.get(capmemcell_ticket)

        self.assertEqual(capmem_config, capmem_copy)
        self.assertEqual(capmemvalue, capmemcell_copy.get_value())


class TestHelloWorldHardware(unittest.TestCase):
    def configure(self):
        self.weight = Ct.SynapseBlock.Synapse.Weight(63)
        self.address = Ct.SynapseBlock.Synapse.Address(42)

        self.neuron = C.NeuronOnDLS(0)
        self.synapse_driver = C.SynapseDriverOnDLS(0)

        self.chip = Ct.Chip()
        self.board = Ct.Board()

        # Set the dac values
        self.board.set_parameter(Ct.Board.Parameter.cadc_ramp_bias, Ct.DAC.Value(2730))
        self.board.set_parameter(Ct.Board.Parameter.cadc_ramp_01, Ct.DAC.Value(1365))
        self.board.set_parameter(Ct.Board.Parameter.cadc_ramp_slope, Ct.DAC.Value(1350))
        self.board.set_parameter(Ct.Board.Parameter.cadc_v_bias, Ct.DAC.Value(2389))
        self.board.set_parameter(Ct.Board.Parameter.syn_v_dd_res_meas, Ct.DAC.Value(4095))
        self.board.set_parameter(Ct.Board.Parameter.syn_v_store, Ct.DAC.Value(1200))
        self.board.set_parameter(Ct.Board.Parameter.syn_v_ramp, Ct.DAC.Value(1200))
        self.board.set_parameter(Ct.Board.Parameter.syn_v_bias, Ct.DAC.Value(1500))
        self.board.set_parameter(Ct.Board.Parameter.capmem_i_offset, Ct.DAC.Value(0))
        self.board.set_parameter(Ct.Board.Parameter.general_purpose_0, Ct.DAC.Value(0))
        self.board.set_parameter(Ct.Board.Parameter.general_purpose_1, Ct.DAC.Value(0))
        self.board.set_parameter(Ct.Board.Parameter.syn_v_reset, Ct.DAC.Value(3700))
        self.board.set_parameter(Ct.Board.Parameter.syn_corout_bias, Ct.DAC.Value(573))
        self.board.set_parameter(Ct.Board.Parameter.capmem_i_buf_bias, Ct.DAC.Value(3000))
        self.board.set_parameter(Ct.Board.Parameter.capmem_i_ref, Ct.DAC.Value(3906))

        capmem_config = self.chip.get_capmem_config()
        capmem_config.set_enable_capmem(True)
        self.chip.set_capmem_config(capmem_config)

        # Set the cap mem
        capmem_config = self.chip.get_capmem()
        capmem_config.set(self.neuron, C.NeuronParameter.v_leak, Ct.CapMemCell.Value(400))
        capmem_config.set(self.neuron, C.NeuronParameter.v_treshold, Ct.CapMemCell.Value(600))
        capmem_config.set(self.neuron, C.NeuronParameter.v_exc_syn_input_reference, Ct.CapMemCell.Value(670))
        capmem_config.set(self.neuron, C.NeuronParameter.v_inh_syn_input_reference, Ct.CapMemCell.Value(690))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_spike_comparator, Ct.CapMemCell.Value(650))
        capmem_config.set(self.neuron, C.NeuronParameter.i_spike_comparator_delay, Ct.CapMemCell.Value(130))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_leak_main, Ct.CapMemCell.Value(200))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_leak_sd, Ct.CapMemCell.Value(500))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_readout_buffer, Ct.CapMemCell.Value(1022))
        capmem_config.set(self.neuron, C.NeuronParameter.i_refractory_time, Ct.CapMemCell.Value(300))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_exc_syn_input_main, Ct.CapMemCell.Value(1022))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_exc_syn_input_sd, Ct.CapMemCell.Value(1022))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_exc_syn_input_resistor, Ct.CapMemCell.Value(200))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_exc_syn_input_offset, Ct.CapMemCell.Value(650))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_inh_syn_input_resistor, Ct.CapMemCell.Value(200))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_inh_syn_input_main, Ct.CapMemCell.Value(1022))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_inh_syn_input_sd, Ct.CapMemCell.Value(1022))
        capmem_config.set(self.neuron, C.NeuronParameter.i_bias_inh_syn_input_offset, Ct.CapMemCell.Value(400))
        capmem_config.set(C.CommonNeuronParameter.e_reset, Ct.CapMemCell.Value(300))
        self.chip.set_capmem(capmem_config)

        # Set connectivity
        synapse = C.SynapseOnDLS(self.neuron, self.synapse_driver)
        synapse_config = self.chip.get_synapse(synapse)
        synapse_config.set_weight(self.weight)
        synapse_config.set_address(self.address)
        self.chip.set_synapse(synapse, synapse_config)
        common_synram_config = self.chip.get_common_synram_config()
        common_synram_config.set_pc_conf(Ct.CommonSynramConfig.PCConf(1))
        common_synram_config.set_w_conf(Ct.CommonSynramConfig.WConf(1))
        common_synram_config.set_wait_ctr_clear(Ct.CommonSynramConfig.WaitCtrClear(1))
        self.chip.set_common_synram_config(common_synram_config)

        # Set current switches at the synaptic input
        current_switch = self.neuron.toColumnCurrentSwitchOnDLS()
        switch_config = self.chip.get_column_current_switch(current_switch)
        switch_config.set_inh_config(
                Ct.ColumnCurrentBlock.ColumnCurrentSwitch.Config.disabled)
        switch_config.set_exc_config(
                Ct.ColumnCurrentBlock.ColumnCurrentSwitch.Config.internal)
        self.chip.set_column_current_switch(current_switch, switch_config)

        # Set synapse driver
        syndrv_config = self.chip.get_synapse_drivers()
        syndrv_config.set_state(self.synapse_driver, Ct.SynapseDriverBlock.State.excitatory)
        syndrv_config.set_pulse_length(Ct.SynapseDriverBlock.PulseLength(8))
        self.chip.set_synapse_drivers(syndrv_config)

        common_neuron_config = self.chip.get_common_neuron_config()
        common_neuron_config.set_enable_digital_out(True)
        self.chip.set_common_neuron_config(common_neuron_config)

        # Set neuron's debug output and disable spike output
        neuron_config = self.chip.get_neuron_digital_config(self.neuron)
        neuron_config.set_fire_out_mode(Ct.NeuronDigitalConfig.FireOutMode.disabled)
        neuron_config.set_mux_readout_mode(Ct.NeuronDigitalConfig.MuxReadoutMode.v_mem)
        self.chip.set_neuron_digital_config(self.neuron, neuron_config)
        self.chip.enable_buffered_readout(self.neuron)

    def run_program(self, program):
        ctrl = IO.ExperimentControl()
        ctrl.run_experiment(self.board, self.chip, program)

    def test_silence(self):
        builder = Ct.PlaybackProgramBuilder()
        builder.wait_for(10000)
        builder.halt()
        program = builder.done()
        self.configure()
        self.run_program(program)
        spikes = program.get_spikes()
        self.assertEqual(len(spikes), 0)

    def configure_spiking(self):
        self.configure()

        neuron_config = self.chip.get_neuron_digital_config(self.neuron)
        neuron_config.set_enable_synapse_input_excitatory(True)
        neuron_config.set_enable_synapse_input_inhibitory(True)
        self.chip.set_neuron_digital_config(self.neuron, neuron_config)

        # Create a playback program (all times are in FPGA cycles / 96MHz)
        builder = Ct.PlaybackProgramBuilder()
        builder.set_time(0)
        for idx in range(NUM_SPIKES):
            builder.wait_until(OFFSET + idx * ISI) # Absolute wait
            builder.fire(self.synapse_driver, self.address)
        builder.wait_for(OFFSET) # Relative wait
        builder.halt()
        program = builder.done()

        return program

    def check_spikes(self, spikes):
        self.assertEqual(len(spikes), NUM_SPIKES)

        last_time = 0
        for spike in spikes:
            time = spike.get_time()
            actual_neuron = spike.get_neuron()
            self.assertEqual(actual_neuron, self.neuron)
            if last_time > 0:
                self.assertAlmostEqual(time - last_time, ISI, delta=10)
            last_time = time

    def test_bypass(self):
        program = self.configure_spiking()

        neuron_config = self.chip.get_neuron_digital_config(self.neuron)
        neuron_config.set_fire_out_mode(Ct.NeuronDigitalConfig.FireOutMode.bypass_exc)
        self.chip.set_neuron_digital_config(self.neuron, neuron_config)

        self.run_program(program)
        self.check_spikes(program.get_spikes())


if __name__ == "__main__":
    logger.reset()
    logger.default_config(level=logger.LogLevel.DEBUG)
    unittest.main()
