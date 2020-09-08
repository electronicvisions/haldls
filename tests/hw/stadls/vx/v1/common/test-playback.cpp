#include <gtest/gtest.h>

#include "connection.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/run.h"

using namespace stadls::vx::v1;


TEST(PlaybackProgram, DefaultConstructor)
{
	PlaybackProgram program;

	auto connection = generate_test_connection();
	run(connection, program);
}
