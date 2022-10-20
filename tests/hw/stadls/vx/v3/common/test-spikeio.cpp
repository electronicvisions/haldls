#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/fpga.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/run.h"
#include "test-helper.h"
#include <map>
#include <set>
#include <gtest/gtest.h>

using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

/**
 * We send spikes that will follow the following route:
 *
 *  (0) FPGA (executor)
 *  (1) Chip (loopback)
 *  (2) FPGA (SpikeIO TX, trace)
 *  (3) loopback wire
 *  (4) FPGA (SpikeIO RX)
 *  (5) Chip (loopback)
 *  (6) FPGA (trace)
 *
 * If both, SpikeIO RX and TX are enabled, we expect to receive all spikes twice (2, 6), modified
 * by address transformations on the way. Else, we only expect to see all spikes once (2).
 */
void test_spikeio_nodrop(SpikeIOConfig const config)
{
	auto sequence = DigitalInit();
	auto [config_builder, _] = generate(sequence);

	config_builder.write(SpikeIOConfigOnFPGA(), config);

	std::map<SpikeLabel, SpikeIOAddress> output_routing{
	    {SpikeLabel{0}, SpikeIOAddress{3}},
	    {SpikeLabel{1}, SpikeIOAddress{4}},
	    {SpikeLabel{2}, SpikeIOAddress{5}}};

	const std::map<SpikeIOAddress, SpikeLabel> input_routing{
	    {SpikeIOAddress{3}, SpikeLabel{6}},
	    {SpikeIOAddress{4}, SpikeLabel{7}},
	    {SpikeIOAddress{5}, SpikeLabel{8}}};

	// Prevent loops: input targets should not map to serial output
	for (auto const& [source, target] : input_routing) {
		output_routing.insert({target, SpikeIOOutputRoute::SILENT});
	}

	const bool spikeio_loopback_enabled = (config.get_enable_tx() && config.get_enable_rx());
	const size_t num_expected_trace_events = 3 * (1 + spikeio_loopback_enabled);
	std::set<SpikeLabel> expected_trace_events;

	for (auto const& [source, target] : output_routing) {
		config_builder.write(source.toSpikeIOOutputRouteOnFPGA(), SpikeIOOutputRoute(target));
		expected_trace_events.insert(source);
	}

	for (auto const& [source, target] : input_routing) {
		config_builder.write(source.toSpikeIOInputRouteOnFPGA(), SpikeIOInputRoute(target));

		if (spikeio_loopback_enabled) {
			expected_trace_events.insert(target);
		}
	}

	config_builder.block_until(BarrierOnFPGA(), Barrier::omnibus);

	// event input
	PlaybackProgramBuilder stim_builder;
	for (auto const& [source, target] : output_routing) {
		if (target == SpikeIOOutputRoute::SILENT) {
			continue;
		}
		stim_builder.write(SpikePack1ToChipOnDLS(), SpikePack1ToChip({source}));
		stim_builder.write(TimerOnDLS(), Timer());
		stim_builder.block_until(
		    TimerOnDLS(), Timer::Value(2 * Timer::Value::fpga_clock_cycles_per_us));
	}

	// wait for loopback events
	stim_builder.write(TimerOnDLS(), Timer());
	stim_builder.block_until(
	    TimerOnDLS(), Timer::Value(100 * Timer::Value::fpga_clock_cycles_per_us));

	auto config_program = config_builder.done();
	auto stim_program = stim_builder.done();

	{
		auto connection = hxcomm::vx::get_connection_from_env();
		run(connection, config_program);
		run(connection, stim_program);
	}

	auto spikes = stim_program.get_spikes();
	EXPECT_EQ(spikes.size(), num_expected_trace_events) << "Unexpected number of events received.";

	for (const auto& spike : spikes) {
		EXPECT_TRUE(expected_trace_events.contains(spike.label))
		    << "Unexpected event received: " << spike;
	}
}

TEST(SpikeIO, NoDrop)
{
	SpikeIOConfig config{};
	config.set_enable_internal_loopback(true);
	config.set_enable_tx(true);
	config.set_enable_rx(true);
	test_spikeio_nodrop(config);
}

TEST(SpikeIO, NoTX)
{
	SpikeIOConfig config{};
	config.set_enable_internal_loopback(true);
	config.set_enable_rx(true);
	test_spikeio_nodrop(config);
}

TEST(SpikeIO, NoRX)
{
	SpikeIOConfig config{};
	config.set_enable_internal_loopback(true);
	config.set_enable_tx(true);
	test_spikeio_nodrop(config);
}

TEST(SpikeIO, NoTXNoRX)
{
	SpikeIOConfig config{};
	config.set_enable_internal_loopback(true);
	test_spikeio_nodrop(config);
}
