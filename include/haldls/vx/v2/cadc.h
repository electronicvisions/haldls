#pragma once
#include "halco/common/typed_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using CADCConfig GENPYBIND(visible) = haldls::vx::CADCConfig;
using CADCChannelConfig GENPYBIND(opaque) = haldls::vx::CADCChannelConfig;
using CADCOffsetSRAMTimingConfig GENPYBIND(visible) = haldls::vx::CADCOffsetSRAMTimingConfig;
using CADCSampleQuad GENPYBIND(visible) = haldls::vx::CADCSampleQuad;

} // namespace haldls::vx::v2
