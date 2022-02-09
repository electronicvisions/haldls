#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/memory_timing.h"

namespace lola::vx::v1 GENPYBIND_TAG_LOLA_VX_V1 {

using MemoryTiming GENPYBIND(visible) = lola::vx::MemoryTiming;

} // namespace lola::vx::v1
