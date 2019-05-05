#include <string>

#include <boost/program_options.hpp>
#include <gtest/gtest.h>

#include "executor.h"

stadls::vx::PlaybackProgramExecutor generate_playback_program_test_executor()
{
	stadls::vx::PlaybackProgramExecutor executor;
	executor.connect();
	return std::move(executor);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
