#include <gtest/gtest.h>

#include "haldls/vx/event.h"
#include "haldls/vx/systime.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

#define TEST_SPIKE(Num)                                                                            \
	TEST(SpikePack##Num##ToChip, Loopback)                                                         \
	{                                                                                              \
		auto sequence = DigitalInit();                                                             \
		auto [builder, _] = generate(sequence);                                                    \
                                                                                                   \
		constexpr size_t num_spikes = 1000;                                                        \
                                                                                                   \
		std::vector<SpikeLabel> to_fpga_spike_labels;                                              \
		for (size_t i = 0; i < num_spikes; ++i) {                                                  \
			auto label = draw_non_default_value<SpikeLabel>(SpikeLabel(0));                        \
			SpikePack##Num##ToChip::labels_type labels;                                            \
			labels.fill(fisch::vx::SpikeLabel(label));                                             \
			SpikePack##Num##ToChip spike(labels);                                                  \
			builder.write(SpikePack##Num##ToChipOnDLS(), spike);                                   \
			builder.write(TimerOnDLS(), Timer());                                                  \
			builder.wait_until(TimerOnDLS(), Timer::Value(10));                                    \
			to_fpga_spike_labels.push_back(label);                                                 \
		}                                                                                          \
                                                                                                   \
		builder.write(TimerOnDLS(), Timer());                                                      \
		builder.wait_until(TimerOnDLS(), Timer::Value(1000));                                      \
		auto program = builder.done();                                                             \
                                                                                                   \
		auto executor = generate_playback_program_test_executor();                                 \
		executor.run(program);                                                                     \
                                                                                                   \
		auto spikes = program.get_spikes();                                                        \
                                                                                                   \
		EXPECT_LE(spikes.size(), num_spikes* Num);                                                 \
		EXPECT_GT(spikes.size(), 0);                                                               \
                                                                                                   \
		for (auto spike : spikes) {                                                                \
			auto it = std::find(                                                                   \
			    to_fpga_spike_labels.cbegin(), to_fpga_spike_labels.cend(), spike.get_label());    \
			EXPECT_TRUE(it != to_fpga_spike_labels.cend()) << "Received spike not sent.";          \
		}                                                                                          \
	}

// clang-format off
TEST_SPIKE(1)
TEST_SPIKE(2)
TEST_SPIKE(3)
// clang-format on

#undef TEST_SPIKE
