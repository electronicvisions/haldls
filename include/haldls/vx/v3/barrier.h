#pragma once
#include "haldls/vx/barrier.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using Barrier GENPYBIND(visible) = haldls::vx::Barrier;

} // namespace haldls::vx::v3
