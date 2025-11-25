#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/event.h"
#include "haldls/vx/v3/systime.h"
#include "haldls/vx/v3/timer.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include "test-helper.h"
#include <gtest/gtest.h>
#include <log4cxx/logger.h>

using namespace halco::common;
using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

#define TEST_SPIKE(Num)                                                                            \
	TEST(SpikePack##Num##ToChip, Loopback)                                                         \
	{                                                                                              \
		auto connection = hxcomm::vx::get_connection_from_env();                                   \
		auto sequence = DigitalInit(std::visit(                                                    \
		    [](auto const& connection) { return connection.get_hwdb_entry().at(0); },              \
		    connection));                                                                          \
		auto [builder, _] = generate(sequence);                                                    \
                                                                                                   \
		constexpr size_t num_spikes = 1000;                                                        \
                                                                                                   \
		std::vector<SpikeLabel> to_fpga_spike_labels;                                              \
		for (size_t i = 0; i < num_spikes; ++i) {                                                  \
			auto label = draw_non_default_value<SpikeLabel>(SpikeLabel(0));                        \
			SpikePack##Num##ToChip::labels_type labels;                                            \
			labels.fill(SpikeLabel(label));                                                        \
			SpikePack##Num##ToChip spike(labels);                                                  \
			builder.write(SpikePack##Num##ToChipOnDLS(), spike);                                   \
			builder.write(TimerOnDLS(), Timer());                                                  \
			builder.block_until(TimerOnDLS(), Timer::Value(10));                                   \
			to_fpga_spike_labels.push_back(label);                                                 \
		}                                                                                          \
                                                                                                   \
		builder.write(TimerOnDLS(), Timer());                                                      \
		builder.block_until(TimerOnDLS(), Timer::Value(1000));                                     \
		auto program = builder.done();                                                             \
                                                                                                   \
		run(connection, program);                                                                  \
                                                                                                   \
		auto spikes = program.get_spikes();                                                        \
                                                                                                   \
		auto logger = log4cxx::Logger::getLogger(                                                  \
		    "stadls.SpikePack" + std::to_string(Num) + "ToChip.Loopback");                         \
		if (spikes.size() < Num * num_spikes) {                                                    \
			LOG4CXX_TRACE(                                                                         \
			    logger,                                                                            \
			    ("WARNING: " + std::to_string(Num * num_spikes - spikes.size()) +                  \
			     " spikes were lost from a total of " + std::to_string(Num * num_spikes) + "!"));  \
		}                                                                                          \
		EXPECT_LE(spikes.size(), num_spikes* Num);                                                 \
		EXPECT_GT(spikes.size(), 0.9 * num_spikes * Num);                                          \
                                                                                                   \
		for (auto spike : spikes) {                                                                \
			auto it = std::find(                                                                   \
			    to_fpga_spike_labels.cbegin(), to_fpga_spike_labels.cend(), spike.label);          \
			EXPECT_TRUE(it != to_fpga_spike_labels.cend()) << "Received spike not sent.";          \
		}                                                                                          \
	}

// clang-format off
TEST_SPIKE(1)
TEST_SPIKE(2)
TEST_SPIKE(3)
// clang-format on

#undef TEST_SPIKE
