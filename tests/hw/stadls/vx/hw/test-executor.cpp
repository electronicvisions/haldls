#include <gtest/gtest.h>
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/playback_program_executor.h"

#include "executor.h"

using namespace stadls::vx;

TEST(PlaybackProgramExecutor, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	builder.read(halco::hicann_dls::vx::CrossbarNodeOnDLS());
	auto program_simulation_restricted = builder.done();

	auto executor = generate_playback_program_test_executor();

	EXPECT_NO_THROW(executor.run(program_unrestricted));
	EXPECT_THROW(executor.run(program_simulation_restricted), std::runtime_error);
}
