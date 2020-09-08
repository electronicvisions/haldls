#include <gtest/gtest.h>
#include "halco/hicann-dls/vx/v2/routing_crossbar.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/playback_program_builder.h"
#include "stadls/vx/v2/run.h"

#include "connection.h"

using namespace stadls::vx::v2;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	builder.read(halco::hicann_dls::vx::v2::CrossbarNodeOnDLS());
	auto program_simulation_restricted = builder.done();

	auto connection = generate_test_connection();

	EXPECT_NO_THROW(run(connection, program_unrestricted));
	EXPECT_THROW(run(connection, program_simulation_restricted), std::runtime_error);
}
