#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/timer.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using Timer GENPYBIND(visible) = haldls::vx::Timer;

} // namespace v3
} // namespace haldls::vx
