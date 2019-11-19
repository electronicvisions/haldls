#include <gtest/gtest.h>

#include "executor.h"
#include "stadls/vx/playback_program.h"

using namespace stadls::vx;


TEST(PlaybackProgram, DefaultConstructor)
{
	PlaybackProgram program;

	auto executor = generate_playback_program_test_executor();
	executor.run(program);
}
