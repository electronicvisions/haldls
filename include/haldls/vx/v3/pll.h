#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/pll.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using ADPLL GENPYBIND(visible) = haldls::vx::ADPLL;
using PLLClockOutputBlock GENPYBIND(visible) = haldls::vx::PLLClockOutputBlock;
using PLLSelfTest GENPYBIND(visible) = haldls::vx::PLLSelfTest;
using PLLSelfTestStatus GENPYBIND(visible) = haldls::vx::PLLSelfTestStatus;

} // namespace haldls::vx::v3
