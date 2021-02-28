#include "halco/hicann-dls/vx/v2/routing_crossbar.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/playback_program_builder.h"
#include "stadls/vx/v2/run.h"
#include <gtest/gtest.h>

using namespace stadls::vx::v2;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	builder.read(halco::hicann_dls::vx::v2::CrossbarNodeOnDLS());
	auto program_simulation_restricted = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();

	EXPECT_NO_THROW(run(connection, program_unrestricted));
	EXPECT_THROW(run(connection, program_simulation_restricted), std::runtime_error);
}
