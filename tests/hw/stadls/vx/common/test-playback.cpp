#include <gtest/gtest.h>

#include "connection.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/run.h"

using namespace stadls::vx;


TEST(PlaybackProgram, DefaultConstructor)
{
	PlaybackProgram program;

	auto connection = generate_test_connection();
	run(connection, program);
}
