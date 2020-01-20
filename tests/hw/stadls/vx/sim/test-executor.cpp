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

	PlaybackProgramBuilder sim_builder(ExecutorBackend::simulation);
	auto program_simulation_restricted = sim_builder.done();

	auto executor = generate_playback_program_test_executor();

	EXPECT_NO_THROW(executor.run(program_unrestricted));
	EXPECT_NO_THROW(executor.run(program_simulation_restricted));

	PlaybackProgramBuilder hw_builder(ExecutorBackend::hardware);
	auto program_hardware_restricted = hw_builder.done();
	EXPECT_THROW(executor.run(program_hardware_restricted), std::runtime_error);
}
