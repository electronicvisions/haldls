#include <gtest/gtest.h>

#include "haldls/vx/background.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

void test_background_spike_source_regular(
    BackgroundSpikeSource::Period period, Timer::Value running_period, size_t spike_count_deviation)
{
	auto sequence = DigitalInit();
	auto [builder, _] = generate(sequence);

	// reset time to wait for defined period
	builder.write(TimerOnDLS(), Timer());

	size_t expected_count =
	    running_period * 2 /* f(FPGA) = 0.5 * f(BackgroundSpikeSource) */ / period;

	typed_array<SpikeLabel, BackgroundSpikeSourceOnDLS> expected_labels;

	// enable background spike sources with unique configuration
	for (auto source_coord : iter_all<BackgroundSpikeSourceOnDLS>()) {
		NeuronLabel neuron_label(source_coord.toEnum());
		SPL1Address spl1_address(source_coord % SPL1Address::size);
		SpikeLabel label;
		label.set_neuron_label(neuron_label);
		label.set_spl1_address(spl1_address);
		expected_labels[source_coord] = label;

		BackgroundSpikeSource source_config;
		source_config.set_period(period);
		source_config.set_enable(true);
		source_config.set_enable_random(false);
		source_config.set_neuron_label(neuron_label);
		builder.write(source_coord, source_config);
	}

	// wait until enabled period is due
	builder.wait_until(TimerOnDLS(), running_period);

	// disable background spike sources
	for (auto source_coord : iter_all<BackgroundSpikeSourceOnDLS>()) {
		BackgroundSpikeSource source_config;
		builder.write(source_coord, source_config);
	}

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	auto spikes = program.get_spikes();

	typed_array<size_t, BackgroundSpikeSourceOnDLS> expected_labels_count;
	expected_labels_count.fill(0);
	for (auto spike : spikes) {
		for (auto source_coord : iter_all<BackgroundSpikeSourceOnDLS>()) {
			if (expected_labels[source_coord] == spike.get_label()) {
				expected_labels_count[source_coord] += 1;
			}
		}
	}

	// for every source check approx. equality in number of spikes expected
	for (auto source_coord : iter_all<BackgroundSpikeSourceOnDLS>()) {
		EXPECT_TRUE(
		    (expected_labels_count[source_coord] <= (expected_count + spike_count_deviation)) &&
		    (expected_labels_count[source_coord] >= (expected_count - spike_count_deviation)))
		    << "expected: " << expected_count << " actual: " << expected_labels_count[source_coord];
	}
}

/**
 * Enable background sources and generate regular spike-trains.
 *
 * This test has been disabled for simulation due to its long runtime (issue #3458).
 */
TEST(BackgroundSpikeSource, Regular)
{
#if SIMULATION_TEST
	test_background_spike_source_regular(
	    BackgroundSpikeSource::Period(100), Timer::Value(10000), 10);
#else
	// 5% allowed deviation in spike count
	test_background_spike_source_regular(
	    BackgroundSpikeSource::Period(1000), Timer::Value(10000000), 1000);
	test_background_spike_source_regular(
	    BackgroundSpikeSource::Period(10000), Timer::Value(100000000), 1000);
#endif
}
