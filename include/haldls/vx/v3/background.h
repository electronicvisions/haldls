#pragma once
#include "haldls/vx/background.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using BackgroundSpikeSource GENPYBIND(visible) = haldls::vx::BackgroundSpikeSource;

} // namespace v3
} // namespace haldls::vx
