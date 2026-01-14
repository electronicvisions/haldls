#include "halco/hicann-dls/vx/v3/barrier.h"
#include "halco/hicann-dls/vx/v3/neuron.h"
#include "halco/hicann-dls/vx/v3/routing_crossbar.h"
#include "haldls/vx/v3/barrier.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;

	auto connection = hxcomm::vx::get_connection_from_env(1);

	auto sim_builder =
	    generate(SystemInit(std::visit(
	                 [](auto const& connection) { return connection.get_hwdb_entry().at(0); },
	                 connection)))
	        .builder;
	sim_builder.read(CrossbarNodeOnDLS());
	sim_builder.block_until(BarrierOnFPGA(), Barrier::omnibus);

	auto program = builder.done();
	EXPECT_NO_THROW(run(connection, {program}));
	auto sim_program = sim_builder.done();
	EXPECT_NO_THROW(run(connection, {sim_program}));

	auto hw_builder =
	    generate(SystemInit(std::visit(
	                 [](auto const& connection) { return connection.get_hwdb_entry().at(0); },
	                 connection)))
	        .builder;
	hw_builder.read(NeuronConfigOnDLS());
	hw_builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto hw_program = hw_builder.done();
	EXPECT_THROW(run(connection, {hw_program}), std::runtime_error);
}
