#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/perftest.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using PerfTest GENPYBIND(visible) = haldls::vx::PerfTest;
using PerfTestStatus GENPYBIND(visible) = haldls::vx::PerfTestStatus;

} // namespace v3
} // namespace haldls::vx
