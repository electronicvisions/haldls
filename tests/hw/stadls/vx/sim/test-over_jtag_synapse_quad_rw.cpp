#include <gtest/gtest.h>

#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/jtag.h"
#include "haldls/vx/v1/pll.h"
#include "haldls/vx/v1/reset.h"
#include "haldls/vx/v1/synapse.h"
#include "haldls/vx/v1/traits.h"
#include "stadls/vx/v1/init_generator.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/run.h"

#include "connection.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx::v1;
using namespace stadls::vx::v1;

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
		auto weights = config.get_weights();
		auto labels = config.get_labels();
		auto time_calibs = config.get_time_calibs();
		auto amp_calibs = config.get_amp_calibs();
		for (auto syn : iter_all<EntryOnQuad>()) {
			weights.at(syn) = draw_ranged_non_default_value<SynapseQuad::Weight>(weights.at(syn));
			labels.at(syn) = draw_ranged_non_default_value<SynapseQuad::Label>(labels.at(syn));
			time_calibs.at(syn) =
			    draw_ranged_non_default_value<SynapseQuad::TimeCalib>(time_calibs.at(syn));
			amp_calibs.at(syn) =
			    draw_ranged_non_default_value<SynapseQuad::AmpCalib>(amp_calibs.at(syn));
		}
		config.set_weights(weights);
		config.set_labels(labels);
		config.set_time_calibs(time_calibs);
		config.set_amp_calibs(amp_calibs);
		quads.insert(std::make_pair(quad, config));
		builder.write(quad, quads.at(quad), Backend::OmnibusChipOverJTAG);
		quad_tickets.emplace(
		    std::make_pair(quad, read_builder.read(quad, Backend::OmnibusChipOverJTAG)));
	}
	builder.merge_back(read_builder);

	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	for (auto const [quad, ticket] : quad_tickets) {
		EXPECT_TRUE(ticket.valid());
		EXPECT_EQ(ticket.get(), quads.at(quad)) << quad;
	}
}
