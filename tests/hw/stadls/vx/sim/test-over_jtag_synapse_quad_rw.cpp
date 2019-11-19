#include <gtest/gtest.h>

#include "haldls/vx/jtag.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

extern std::optional<size_t> const max_words_per_reduced_test;

/**
 * Enable over JTAG omnibus connection and write and read all Synapse blocks for verification.
 */
TEST(SynapseQuad, WROverJTAG)
{
	auto sequence = DigitalInit();
	sequence.enable_highspeed_link = false;
	auto [builder, _] = generate(sequence);

	std::map<SynapseQuadOnDLS, SynapseQuad> quads;
	std::map<SynapseQuadOnDLS, PlaybackProgram::ContainerTicket<SynapseQuad>> quad_tickets;

	PlaybackProgramBuilder read_builder;
	for (auto const quad : iter_sparse<SynapseQuadOnDLS>(max_words_per_reduced_test)) {
		SynapseQuad config;
		for (auto syn : iter_all<EntryOnQuad>()) {
			auto synapse = config.get_synapse(syn);
			synapse.set_weight(draw_ranged_non_default_value<decltype(synapse.get_weight())>(
			    synapse.get_weight()));
			synapse.set_address(draw_ranged_non_default_value<decltype(synapse.get_address())>(
			    synapse.get_address()));
			synapse.set_time_calib(
			    draw_ranged_non_default_value<decltype(synapse.get_time_calib())>(
			        synapse.get_time_calib()));
			synapse.set_amp_calib(draw_ranged_non_default_value<decltype(synapse.get_amp_calib())>(
			    synapse.get_amp_calib()));
			config.set_synapse(syn, synapse);
		}
		quads.insert(std::make_pair(quad, config));
		builder.write(quad, quads.at(quad), Backend::OmnibusChipOverJTAG);
		quad_tickets.emplace(
		    std::make_pair(quad, read_builder.read(quad, Backend::OmnibusChipOverJTAG)));
	}
	builder.merge_back(read_builder);

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(40000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto const [quad, ticket] : quad_tickets) {
		EXPECT_TRUE(ticket.valid());
		EXPECT_EQ(ticket.get(), quads.at(quad)) << quad;
	}
}
