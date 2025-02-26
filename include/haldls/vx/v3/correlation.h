#pragma once
#include "haldls/vx/correlation.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using CommonCorrelationConfig GENPYBIND(visible) = haldls::vx::CommonCorrelationConfig;

} // namespace v3
} // namespace haldls::vx
