#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/timer.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using Timer GENPYBIND(visible) = haldls::vx::Timer;

} // namespace haldls::vx::v3
