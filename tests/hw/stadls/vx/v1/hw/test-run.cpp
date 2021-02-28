#include "halco/hicann-dls/vx/v1/routing_crossbar.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/playback_program_builder.h"
#include "stadls/vx/v1/run.h"
#include <gtest/gtest.h>

using namespace halco::hicann_dls::vx::v1;
using namespace stadls::vx::v1;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	builder.read(CrossbarNodeOnDLS());
	auto program_simulation_restricted = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();

	EXPECT_NO_THROW(run(connection, program_unrestricted));
	EXPECT_THROW(run(connection, program_simulation_restricted), std::runtime_error);
}
