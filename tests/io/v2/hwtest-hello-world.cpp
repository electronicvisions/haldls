#include <gtest/gtest.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/container/v2/board.h"
#include "haldls/container/v2/chip.h"
#include "haldls/container/v2/capmem.h"
#include "haldls/io/v2/experiment.h"
#include "haldls/io/v2/playback.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::container::v2;
using namespace haldls::io::v2;

class IntegrationHelloWorld : public ::testing::Test
{
protected:
	SynapseBlock::Synapse::Weight const weight{63};
	SynapseBlock::Synapse::Address const address{42};

	NeuronOnDLS neuron{0};
	SynapseDriverOnDLS synapse_driver{0};

	IntegrationHelloWorld()
	{}

	IntegrationHelloWorld(NeuronOnDLS neuron_, SynapseDriverOnDLS synapse_driver_)
		: neuron(neuron_), synapse_driver(synapse_driver_)
	{}

	void configure()
	{
		// Set the dac values
		board.set_parameter(Board::Parameter::cadc_ramp_bias, DAC::Value(2730));
		board.set_parameter(Board::Parameter::cadc_ramp_01, DAC::Value(1365));
		board.set_parameter(Board::Parameter::cadc_ramp_slope, DAC::Value(1350));
		board.set_parameter(Board::Parameter::cadc_v_bias, DAC::Value(2389));
		board.set_parameter(Board::Parameter::syn_v_dd_res_meas, DAC::Value(4095));
		board.set_parameter(Board::Parameter::syn_v_store, DAC::Value(1200));
		board.set_parameter(Board::Parameter::syn_v_ramp, DAC::Value(1200));
		board.set_parameter(Board::Parameter::syn_v_bias, DAC::Value(1500));
		board.set_parameter(Board::Parameter::capmem_i_offset, DAC::Value(0));
		board.set_parameter(Board::Parameter::general_purpose_0, DAC::Value(0));
		board.set_parameter(Board::Parameter::general_purpose_1, DAC::Value(0));
		board.set_parameter(Board::Parameter::syn_v_reset, DAC::Value(3700));
		board.set_parameter(Board::Parameter::syn_corout_bias, DAC::Value(573));
		board.set_parameter(Board::Parameter::capmem_i_buf_bias, DAC::Value(3000));
		board.set_parameter(Board::Parameter::capmem_i_ref, DAC::Value(3906));

		auto common_capmem_config = chip.get_common_capmem_config();
		common_capmem_config.set_enable_capmem(true);
		chip.set_common_capmem_config(common_capmem_config);

		// Set the cap mem
		auto capmem_config = chip.get_capmem();
		capmem_config.set(neuron, NeuronParameter::v_leak, CapMemCell::Value(400));
		capmem_config.set(neuron, NeuronParameter::v_treshold, CapMemCell::Value(600));
		capmem_config.set(neuron, NeuronParameter::v_exc_syn_input_reference, CapMemCell::Value(670));
		capmem_config.set(neuron, NeuronParameter::v_inh_syn_input_reference, CapMemCell::Value(690));
		capmem_config.set(neuron, NeuronParameter::i_bias_spike_comparator, CapMemCell::Value(650));
		capmem_config.set(neuron, NeuronParameter::i_spike_comparator_delay, CapMemCell::Value(130));
		capmem_config.set(neuron, NeuronParameter::i_bias_leak_main, CapMemCell::Value(200));
		capmem_config.set(neuron, NeuronParameter::i_bias_leak_sd, CapMemCell::Value(500));
		capmem_config.set(neuron, NeuronParameter::i_bias_readout_buffer, CapMemCell::Value(1022));
		capmem_config.set(neuron, NeuronParameter::i_refractory_time, CapMemCell::Value(300));
		capmem_config.set(neuron, NeuronParameter::i_bias_exc_syn_input_main, CapMemCell::Value(1022));
		capmem_config.set(neuron, NeuronParameter::i_bias_exc_syn_input_sd, CapMemCell::Value(1022));
		capmem_config.set(neuron, NeuronParameter::i_bias_exc_syn_input_resistor, CapMemCell::Value(200));
		capmem_config.set(neuron, NeuronParameter::i_bias_exc_syn_input_offset, CapMemCell::Value(650));
		capmem_config.set(neuron, NeuronParameter::i_bias_inh_syn_input_resistor, CapMemCell::Value(200));
		capmem_config.set(neuron, NeuronParameter::i_bias_inh_syn_input_main, CapMemCell::Value(1022));
		capmem_config.set(neuron, NeuronParameter::i_bias_inh_syn_input_sd, CapMemCell::Value(1022));
		capmem_config.set(neuron, NeuronParameter::i_bias_inh_syn_input_offset, CapMemCell::Value(400));
		capmem_config.set(CommonNeuronParameter::e_reset, CapMemCell::Value(300));
		chip.set_capmem(capmem_config);

		// Set connectivity
		SynapseOnDLS const synapse(neuron, synapse_driver);
		auto synapse_config = chip.get_synapse(synapse);
		synapse_config.set_weight(weight);
		synapse_config.set_address(address);
		chip.set_synapse(synapse, synapse_config);

		auto common_synram_config = chip.get_common_synram_config();
		common_synram_config.set_pc_conf(CommonSynramConfig::PCConf(1));
		common_synram_config.set_w_conf(CommonSynramConfig::WConf(1));
		common_synram_config.set_wait_ctr_clear(CommonSynramConfig::WaitCtrClear(1));
		chip.set_common_synram_config(common_synram_config);

		// Set current switches at the synaptic input
		auto const current_switch = neuron.toColumnCurrentSwitchOnDLS();
		auto switch_config = chip.get_column_current_switch(current_switch);
		switch_config.set_inh_config(
			ColumnCurrentBlock::ColumnCurrentSwitch::Config::disabled);
		switch_config.set_exc_config(
			ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);
		chip.set_column_current_switch(current_switch, switch_config);

		// Set synapse driver
		auto syndrv_config = chip.get_synapse_drivers();
		syndrv_config.set_state(synapse_driver, SynapseDrivers::State::excitatory);
		syndrv_config.set_pulse_length(SynapseDrivers::PulseLength(8));
		chip.set_synapse_drivers(syndrv_config);

		auto common_neuron_config = chip.get_common_neuron_config();
		common_neuron_config.set_enable_digital_out(true);
		chip.set_common_neuron_config(common_neuron_config);

		// Set neuron's debug output and disable spike output
		auto neuron_config = chip.get_neuron_digital_config(neuron);
		neuron_config.set_fire_out_mode(NeuronDigitalConfig::FireOutMode::disabled);
		neuron_config.set_mux_readout_mode(NeuronDigitalConfig::MuxReadoutMode::v_mem);
		chip.set_neuron_digital_config(neuron, neuron_config);
		chip.enable_buffered_readout(neuron);
	}

	void test_run_program(PlaybackProgram& program)
	{
		// get the board id
		std::vector<std::string> board_ids = available_board_usb_serial_numbers();
		ASSERT_EQ(1, board_ids.size()) << "number of allocated boards is not one";

		ExperimentControl ctrl = connect(board_ids.front());
		ctrl.configure_static(board, chip);
		ctrl.run(program);
	}

	Board board;
	Chip chip;
}; // IntegrationHelloWorld

TEST_F(IntegrationHelloWorld, Silence)
{
	PlaybackProgramBuilder builder;
	builder.wait_for(10000);
	builder.halt();

	auto program = builder.done();

	configure();
	test_run_program(program);
	auto const& spikes = program.get_spikes();
	EXPECT_TRUE(spikes.empty());
}

class IntegrationBypassHelloWorld
	: public IntegrationHelloWorld
	, public ::testing::WithParamInterface<size_t>
{
protected:
	size_t const offset = 1000; // ~10 us
	size_t const isi = 2000;    // ~20 us
	size_t const num_spikes = 50;

	IntegrationBypassHelloWorld()
		: IntegrationHelloWorld(
			  NeuronOnDLS(GetParam()),
			  SynapseDriverOnDLS((GetParam() + 3) % SynapseDriverOnDLS::size))
	{}

	void configure()
	{
		IntegrationHelloWorld::configure();

		auto neuron_config = chip.get_neuron_digital_config(neuron);
		neuron_config.set_enable_synapse_input_excitatory(true);
		neuron_config.set_enable_synapse_input_inhibitory(true);
		chip.set_neuron_digital_config(neuron, neuron_config);

		// Create a playback program (all times are in FPGA cycles / 96MHz)

		PlaybackProgramBuilder builder;
		builder.set_time(0);
		for (size_t ii = 0; ii < num_spikes; ++ii) {
			builder.wait_until(offset + ii * isi); // Absolute wait
			builder.fire(synapse_driver, address);
		}
		builder.wait_for(offset); // Relative wait
		builder.halt();

		program = builder.done();
	}

	void test_run_program()
	{
		IntegrationHelloWorld::test_run_program(program);
	}

	void check_spikes()
	{
		auto const& spikes = program.get_spikes();
		EXPECT_EQ(num_spikes, spikes.size());

		size_t last_time = 0;
		for (RecordedSpike const& spike : spikes) {
			size_t time = spike.get_time();
			auto actual_neuron = spike.get_neuron();
			EXPECT_EQ(neuron, actual_neuron) << actual_neuron;
			if (last_time > 0) {
				EXPECT_NEAR(isi, time - last_time, 10);
			}
			last_time = time;
		}
	}

	PlaybackProgram program;
}; // IntegrationBypassHelloWorld

INSTANTIATE_TEST_CASE_P(
	IntegrationBypassHelloWorldOffDiagonal,
	IntegrationBypassHelloWorld,
	::testing::Range(NeuronOnDLS::begin, NeuronOnDLS::end));

TEST_P(IntegrationBypassHelloWorld, Bypass)
{
	configure();

	auto neuron_config = chip.get_neuron_digital_config(neuron);
	neuron_config.set_fire_out_mode(NeuronDigitalConfig::FireOutMode::bypass_exc);
	chip.set_neuron_digital_config(neuron, neuron_config);

	test_run_program();
	check_spikes();
}

class IntegrationSpikingHelloWorld : public IntegrationBypassHelloWorld
{}; // IntegrationSpikingHelloWorld

INSTANTIATE_TEST_CASE_P(
	IntegrationSpikingHelloWorldOffDiagonal,
	IntegrationSpikingHelloWorld,
	// Working neurons depend on hardware, as we have no actual calibration data at the moment.
	// For chip 07 the following neurons seem to work:
	::testing::Values(
		0, 1, 2, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 17, 19, 21, 22, 23, 25, 28, 29, 31));

TEST_P(IntegrationSpikingHelloWorld, Spiking)
{
	configure();

	auto neuron_config = chip.get_neuron_digital_config(neuron);
	neuron_config.set_fire_out_mode(NeuronDigitalConfig::FireOutMode::enabled);
	chip.set_neuron_digital_config(neuron, neuron_config);

	test_run_program();
	check_spikes();
}
