#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/memory_timing.h"

namespace lola::vx::v3 GENPYBIND_TAG_LOLA_VX_V3 {

using MemoryTiming GENPYBIND(visible) = lola::vx::MemoryTiming;

} // namespace lola::vx::v3
