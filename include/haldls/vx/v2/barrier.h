#pragma once
#include "haldls/vx/barrier.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using Barrier GENPYBIND(visible) = haldls::vx::Barrier;

} // namespace haldls::vx::v2
