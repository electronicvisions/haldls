#include <gtest/gtest.h>

#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/neuron.h"
#include "stadls/vx/v3/container_ticket.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"

#include "hxcomm/vx/connection_from_env.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

/**
 * Test the Neuron Backend containing the NeuronReset and the SpikeCounter.
 * First the Counters are resettet, following three Neuron Resets which should give a counter
 * value of 3. After this is verified the counters are resetted and now the values of
 * every counter should be 0.
 *
 * Disabled for simulation due to #3475.
 */
TEST(NeuronBackend, ResetCounterTest)
{
	auto sequence = DigitalInit();
	sequence.chip.highspeed_link.enable_systime = false;
	auto [builder, _] = generate(sequence);

	// Set the Neuron Configuration
	NeuronConfig nrn_config;
	nrn_config.set_enable_fire(true);
	for (auto coord : iter_all<NeuronConfigOnDLS>()) {
		builder.write(coord, nrn_config);
	}

	// Set Neuron Backend Configuration
	NeuronBackendConfig nrn_backend_config;
	nrn_backend_config.set_enable_spike_out(true);
	for (auto coord : iter_all<NeuronBackendConfigOnDLS>()) {
		builder.write(coord, nrn_backend_config);
	}

	// Set Common Neuron Backend
	CommonNeuronBackendConfig common_nrn_backend;
	common_nrn_backend.set_enable_event_registers(true);
	common_nrn_backend.set_enable_clocks(true);
	for (auto coord : iter_all<EventOutputOnNeuronBackendBlock>()) {
		common_nrn_backend.set_sample_positive_edge(coord, true);
	}
	for (auto coord : iter_all<CommonNeuronBackendConfigOnDLS>()) {
		builder.write(coord, common_nrn_backend);
	}

	// Reset all Neuron Spike Counters
	for (auto cnt_reset : iter_all<SpikeCounterResetOnDLS>()) {
		builder.write(cnt_reset, SpikeCounterReset());
	}

	// Reset all Neurons three times resulting in a Counter value of three
	for (size_t i = 0; i < 3; i++) {
		for (auto nrn_reset : iter_all<NeuronResetOnDLS>()) {
			builder.write(nrn_reset, NeuronReset());
		}
	}

	// Readout Neuron Spike Counters
	std::vector<ContainerTicket> cnt_value_read;
	for (auto nrn_read : iter_all<SpikeCounterReadOnDLS>()) {
		cnt_value_read.push_back(builder.read(nrn_read));
	}

	// With 256 spikes detected the Spike Counter sets the overflow bit.
	// The 8 bit counter still runs between 0 and 255 with no influence on
	// the overflow bit.
	// Just a Spike Counter Reset will set the overflow bit back to zero.
	// So resetting all Neurons 512 times (Two times the size of the Counter)
	// should enable the overflow bit. The value of the counter however
	// should be the same as before, namely three.
	for (size_t i = 0; i < SpikeCounterRead::Count::size * 2; i++) {
		for (auto nrn_reset : iter_all<NeuronResetOnDLS>()) {
			builder.write(nrn_reset, NeuronReset());
		}
	}

	// Readout Neuron Spike Counters
	std::vector<ContainerTicket> cnt_value_ovrflw;
	for (auto nrn_read : iter_all<SpikeCounterReadOnDLS>()) {
		cnt_value_ovrflw.push_back(builder.read(nrn_read));
	}

	// Reset Neuron Spike Counters again
	for (auto cnt_reset : iter_all<SpikeCounterResetOnDLS>()) {
		builder.write(cnt_reset, SpikeCounterReset());
	}

	// Readout Neuron Spike Counters again
	std::vector<ContainerTicket> cnt_value_reset;
	for (auto nrn_read : iter_all<SpikeCounterReadOnDLS>()) {
		cnt_value_reset.push_back(builder.read(nrn_read));
	}

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	for (auto cnt_value_ticket : cnt_value_read) {
		auto const& cnt_value = dynamic_cast<SpikeCounterRead const&>(cnt_value_ticket.get());
		EXPECT_EQ(cnt_value.get_count(), SpikeCounterRead::Count(3));
		EXPECT_EQ(cnt_value.get_overflow(), false);
	}

	for (auto cnt_value_ticket : cnt_value_ovrflw) {
		auto const& cnt_value = dynamic_cast<SpikeCounterRead const&>(cnt_value_ticket.get());
		EXPECT_EQ(cnt_value.get_count(), SpikeCounterRead::Count(3));
		EXPECT_EQ(cnt_value.get_overflow(), true);
	}

	for (auto cnt_value_ticket : cnt_value_reset) {
		auto const& cnt_value = dynamic_cast<SpikeCounterRead const&>(cnt_value_ticket.get());
		EXPECT_EQ(cnt_value.get_count(), SpikeCounterRead::Count(0));
		EXPECT_EQ(cnt_value.get_overflow(), false);
	}
}
