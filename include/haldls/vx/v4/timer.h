#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/timer.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using Timer GENPYBIND(visible) = haldls::vx::Timer;

} // namespace v4
} // namespace haldls::vx
