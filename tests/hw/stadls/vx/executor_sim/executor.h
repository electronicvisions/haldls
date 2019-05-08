#pragma once
#include "fisch/vx/playback_executor.h"

typedef fisch::vx::PlaybackProgramExecutor<hxcomm::vx::SimConnection> PlaybackProgramTestExecutor;

PlaybackProgramTestExecutor generate_playback_program_test_executor();
