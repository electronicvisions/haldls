#pragma once
#include "haldls/vx/cadc.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using CADCConfig GENPYBIND(visible) = haldls::vx::CADCConfig;
using CADCChannelConfig GENPYBIND(visible) = haldls::vx::CADCChannelConfig;
using CADCSampleQuad GENPYBIND(visible) = haldls::vx::CADCSampleQuad;
using CADCOffsetSRAMTimingConfig GENPYBIND(visible) = haldls::vx::CADCOffsetSRAMTimingConfig;

} // namespace haldls::vx::v2
