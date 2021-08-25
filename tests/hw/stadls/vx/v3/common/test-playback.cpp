#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

using namespace stadls::vx::v3;


TEST(PlaybackProgram, DefaultConstructor)
{
	PlaybackProgram program;

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);
}
