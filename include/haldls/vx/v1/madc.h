#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/madc.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using MADCControl GENPYBIND(visible) = haldls::vx::MADCControl;
using MADCConfig GENPYBIND(visible) = haldls::vx::MADCConfig;

} // namespace haldls::vx::v1
