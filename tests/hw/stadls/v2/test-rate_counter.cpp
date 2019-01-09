#include <cstdlib>
#include <gtest/gtest.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/playback.h"
#include "stadls/v2/experiment.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::v2;
using namespace stadls::v2;

void rate_counter_test_rw(NeuronOnDLS const neuron)
{
	Board board;
	Chip chip;

	auto synapse_driver = SynapseDriverOnDLS(0);

	SynapseBlock::Synapse::Weight const weight{63};
	SynapseBlock::Synapse::Address const address{42};

	// Set connectivity
	SynapseOnDLS const synapse(neuron.toSynapseColumnOnDLS(), synapse_driver.toSynapseRowOnDLS());
	auto synapse_config = chip.get_synapse(synapse);
	synapse_config.set_weight(weight);
	synapse_config.set_address(address);
	chip.set_synapse(synapse, synapse_config);

	// Set current switches at the synaptic input
	auto const current_switch = neuron.toColumnCurrentSwitchOnDLS();
	auto switch_config = chip.get_column_current_switch(current_switch);
	switch_config.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);
	chip.set_column_current_switch(current_switch, switch_config);

	// Set synapse driver
	auto syndrv_config = chip.get_synapse_drivers();
	syndrv_config.set_mode(synapse_driver, SynapseDriverBlock::Mode::excitatory);
	syndrv_config.set_pulse_length(SynapseDriverBlock::PulseLength(8));
	chip.set_synapse_drivers(syndrv_config);

	auto common_neuron_config = chip.get_common_neuron_config();
	common_neuron_config.set_enable_digital_out(true);
	chip.set_common_neuron_config(common_neuron_config);

	// Set neuron
	auto neuron_config = chip.get_neuron_digital_config(neuron);
	neuron_config.set_enable_synapse_input_excitatory(true);
	neuron_config.set_fire_out_mode(NeuronDigitalConfig::FireOutMode::bypass_exc);
	chip.set_neuron_digital_config(neuron, neuron_config);

	auto rate_counter_config = chip.get_rate_counter_config();
	rate_counter_config.set_enable_neuron(neuron, true);
	chip.set_rate_counter_config(rate_counter_config);

	{
		// test rate counter read write
		PlaybackProgramBuilder builder;
		builder.write(RateCounterOnDLS(), RateCounter());
		auto counter_ticket = builder.read<RateCounter>(RateCounterOnDLS());
		builder.halt();
		auto program = builder.done();

		{
			ExperimentControl ctrl;
			ctrl.run_experiment(board, chip, program);
		}

		auto res = program.get(counter_ticket);
		EXPECT_EQ(RateCounter(), res);
	}

	{
		RateCounter rate_counter;
		{
			// test rate counter counting number of spikes
			PlaybackProgramBuilder builder;
			size_t num_spikes = rand() % RateCounter::Count::size;
			for (size_t i = 0; i < num_spikes; ++i) {
				builder.fire(synapse_driver, address);
				builder.wait_for(100);
			}
			auto counter_ticket = builder.read<RateCounter>(RateCounterOnDLS());
			builder.halt();
			auto program = builder.done();

			{
				ExperimentControl ctrl;
				ctrl.run_experiment(board, chip, program);
			}

			rate_counter = program.get(counter_ticket);
			EXPECT_EQ(rate_counter.get_count(neuron), num_spikes);
		}

		{
			// test rate counter write resets count to 0
			PlaybackProgramBuilder builder;
			builder.fire(synapse_driver, address);
			builder.wait_for(100);
			builder.write(RateCounterOnDLS(), rate_counter);
			builder.wait_for(100);
			// check that counting actually continues at 0 after reset
			builder.fire(synapse_driver, address);
			builder.wait_for(100);
			auto counter_ticket = builder.read<RateCounter>(RateCounterOnDLS());
			builder.halt();
			auto program = builder.done();

			{
				ExperimentControl ctrl;
				ctrl.run_experiment(board, chip, program);
			}

			auto const rate_counter_2 = program.get(counter_ticket);
			EXPECT_EQ(rate_counter_2.get_count(neuron), 1); // would expect 2, if not reset and num_spikes + 1 if set instead of reset.
		}
	}

	{
		// test rate counter clear_on_read = false
		PlaybackProgramBuilder builder;
		builder.fire(synapse_driver, address);
		builder.wait_for(100);
		auto counter_ticket = builder.read<RateCounter>(RateCounterOnDLS());
		auto counter_ticket_2 = builder.read<RateCounter>(RateCounterOnDLS());
		builder.halt();
		auto program = builder.done();

		{
			ExperimentControl ctrl;
			ctrl.run_experiment(board, chip, program);
		}

		auto const rate_counter = program.get(counter_ticket);
		auto const rate_counter_2 = program.get(counter_ticket_2);
		EXPECT_EQ(rate_counter, rate_counter_2);
	}

	{
		Chip chip_local = chip;

		// test rate counter clear_on_read = true
		auto rate_counter_config = chip_local.get_rate_counter_config();
		rate_counter_config.set_enable_clear_on_read(true);
		chip_local.set_rate_counter_config(rate_counter_config);

		PlaybackProgramBuilder builder;
		builder.fire(synapse_driver, address);
		builder.wait_for(100);
		auto counter_ticket = builder.read<RateCounter>(RateCounterOnDLS());
		auto counter_ticket_2 = builder.read<RateCounter>(RateCounterOnDLS());
		builder.halt();
		auto program = builder.done();

		{
			ExperimentControl ctrl;
			ctrl.run_experiment(board, chip_local, program);
		}

		auto const rate_counter = program.get(counter_ticket);
		auto const rate_counter_2 = program.get(counter_ticket_2);
		EXPECT_EQ(rate_counter.get_count(neuron), 1);
		EXPECT_EQ(rate_counter_2.get_count(neuron), 0);
	}
}

/**
 * For all neurons test rate counter read write and spike count behavior.
 */
TEST(RateCounter, RW) {
	for (auto neuron: iter_all<NeuronOnDLS>()) {
		rate_counter_test_rw(neuron);
	}
}
