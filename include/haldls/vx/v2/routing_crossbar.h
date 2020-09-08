#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/routing_crossbar.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using CrossbarOutputConfig GENPYBIND(visible) = haldls::vx::CrossbarOutputConfig;
using CrossbarInputDropCounter GENPYBIND(visible) = haldls::vx::CrossbarInputDropCounter;
using CrossbarOutputEventCounter GENPYBIND(visible) = haldls::vx::CrossbarOutputEventCounter;
using CrossbarNode GENPYBIND(visible) = haldls::vx::CrossbarNode;

} // namespace haldls::vx::v2
