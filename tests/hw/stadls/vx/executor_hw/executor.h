#pragma once
#include "fisch/vx/playback_executor.h"

#define STADLS_TEST_ARQ_EXECUTOR
typedef fisch::vx::PlaybackProgramARQExecutor PlaybackProgramTestExecutor;

PlaybackProgramTestExecutor generate_playback_program_test_executor();
