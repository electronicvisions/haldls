#include "halco/hicann-dls/vx/v3/routing_crossbar.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

using namespace stadls::vx::v3;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	builder.read(halco::hicann_dls::vx::v3::CrossbarNodeOnDLS());
	auto program_simulation_restricted = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env(1);

	std::vector<std::reference_wrapper<PlaybackProgram>> programs_unrestricted{
	    program_unrestricted};
	EXPECT_NO_THROW(run(connection, programs_unrestricted));

	std::vector<std::reference_wrapper<PlaybackProgram>> programs_simulation_restricted{
	    program_simulation_restricted};
	EXPECT_THROW(run(connection, programs_simulation_restricted), std::runtime_error);
}
