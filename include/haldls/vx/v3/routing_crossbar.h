#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/routing_crossbar.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using CrossbarOutputConfig GENPYBIND(visible) = haldls::vx::CrossbarOutputConfig;
using CrossbarInputDropCounter GENPYBIND(visible) = haldls::vx::CrossbarInputDropCounter;
using CrossbarOutputEventCounter GENPYBIND(visible) = haldls::vx::CrossbarOutputEventCounter;
using CrossbarNode GENPYBIND(visible) = haldls::vx::CrossbarNode;

} // namespace v3
} // namespace haldls::vx
