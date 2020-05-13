#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/perftest.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using PerfTest GENPYBIND(visible) = haldls::vx::PerfTest;
using PerfTestStatus GENPYBIND(visible) = haldls::vx::PerfTestStatus;

} // namespace haldls::vx::v1
