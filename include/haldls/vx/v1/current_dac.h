#pragma once
#include "haldls/vx/current_dac.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using CurrentDAC GENPYBIND(visible) = haldls::vx::CurrentDAC;

} // namespace haldls::vx::v1
