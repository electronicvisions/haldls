#pragma once
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_program.h"

namespace stadls::vx {
namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

using PlaybackProgram GENPYBIND(visible) = stadls::vx::PlaybackProgram;

} // namespace v3
} // namespace stadls::vx
