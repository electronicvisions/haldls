#pragma once
#include "haldls/vx/current_dac.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using CurrentDAC GENPYBIND(visible) = haldls::vx::CurrentDAC;

} // namespace v3
} // namespace haldls::vx
