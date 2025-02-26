#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/madc.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using MADCControl GENPYBIND(visible) = haldls::vx::MADCControl;
using MADCConfig GENPYBIND(visible) = haldls::vx::MADCConfig;

} // namespace v3
} // namespace haldls::vx
