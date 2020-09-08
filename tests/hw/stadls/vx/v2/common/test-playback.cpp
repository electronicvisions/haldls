#include <gtest/gtest.h>

#include "connection.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/run.h"

using namespace stadls::vx::v2;


TEST(PlaybackProgram, DefaultConstructor)
{
	PlaybackProgram program;

	auto connection = generate_test_connection();
	run(connection, program);
}
