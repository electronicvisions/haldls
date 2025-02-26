#pragma once
#include "halco/common/typed_array.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using CADCConfig GENPYBIND(visible) = haldls::vx::CADCConfig;
using CADCChannelConfig GENPYBIND(visible) = haldls::vx::CADCChannelConfig;
using CADCOffsetSRAMTimingConfig GENPYBIND(visible) = haldls::vx::CADCOffsetSRAMTimingConfig;
using CADCSampleQuad GENPYBIND(visible) = haldls::vx::CADCSampleQuad;

} // namespace v3
} // namespace haldls::vx
