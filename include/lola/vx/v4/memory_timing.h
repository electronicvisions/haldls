#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/memory_timing.h"

namespace lola::vx { namespace v4 GENPYBIND_TAG_LOLA_VX_V4 {

using MemoryTiming GENPYBIND(visible) = lola::vx::MemoryTiming;

} // namespace v4
} // namespace lola::vx
