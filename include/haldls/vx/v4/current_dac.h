#pragma once
#include "haldls/vx/current_dac.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using CurrentDAC GENPYBIND(visible) = haldls::vx::CurrentDAC;

} // namespace v4
} // namespace haldls::vx
