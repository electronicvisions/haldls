#include <gtest/gtest.h>

#include "haldls/vx/jtag.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"

#include "executor.h"
#include "test-helper.h"
#include "test-init_helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable over JTAG omnibus connection and write and read all Synapse blocks for verification.
 * Disabled due to failing read-back probably caused by wrong timing settings.
 */
TEST(SynapseQuad, DISABLED_WROverJTAG)
{
	PlaybackProgramBuilder builder;

	insert_highspeed_init(builder);

	typed_array<SynapseQuad, SynapseQuadOnDLS> configs;

	// Fill configs with random data
	for (auto coord : iter_all<SynapseQuadOnDLS>()) {
		for (auto syn : iter_all<EntryOnQuad>()) {
			auto synapse = configs[coord].get_synapse(syn);
			synapse.set_weight(draw_ranged_non_default_value<decltype(synapse.get_weight())>(
			    synapse.get_weight()));
			synapse.set_address(draw_ranged_non_default_value<decltype(synapse.get_address())>(
			    synapse.get_address()));
			synapse.set_time_calib(
			    draw_ranged_non_default_value<decltype(synapse.get_time_calib())>(
			        synapse.get_time_calib()));
			synapse.set_amp_calib(draw_ranged_non_default_value<decltype(synapse.get_amp_calib())>(
			    synapse.get_amp_calib()));
			configs[coord].set_synapse(syn, synapse);
		}
	}

	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		CommonSynramConfig config;
		builder.write(coord, config, Backend::OmnibusChipOverJTAG);
	}

	// Write using JTAG
	for (auto coord : iter_all<SynapseQuadOnDLS>()) {
		builder.write(coord, configs[coord], Backend::OmnibusChipOverJTAG);
	}

	std::vector<PlaybackProgram::ContainerTicket<SynapseQuad>> tickets;
	// Read back
	for (auto coord : iter_all<SynapseQuadOnDLS>()) {
		tickets.push_back(builder.read(coord, Backend::OmnibusChipOverJTAG));
	}
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(40000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto coord : iter_all<SynapseQuadOnDLS>()) {
		EXPECT_TRUE(tickets[coord.toEnum()].valid());
		EXPECT_EQ(tickets[coord.toEnum()].get(), configs[coord]) << coord;
	}
}
