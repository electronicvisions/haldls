#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/perftest.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using PerfTest GENPYBIND(visible) = haldls::vx::PerfTest;
using PerfTestStatus GENPYBIND(visible) = haldls::vx::PerfTestStatus;

} // namespace v4
} // namespace haldls::vx
