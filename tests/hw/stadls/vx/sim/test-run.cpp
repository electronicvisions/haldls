#include <gtest/gtest.h>
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/run.h"

#include "connection.h"

using namespace stadls::vx;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	PlaybackProgramBuilder sim_builder(ExecutorBackend::simulation);
	auto program_simulation_restricted = sim_builder.done();

	auto connection = generate_test_connection();

	EXPECT_NO_THROW(run(connection, program_unrestricted));
	EXPECT_NO_THROW(run(connection, program_simulation_restricted));

	PlaybackProgramBuilder hw_builder(ExecutorBackend::hardware);
	auto program_hardware_restricted = hw_builder.done();
	EXPECT_THROW(run(connection, program_hardware_restricted), std::runtime_error);
}
