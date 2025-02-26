#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/jtag.h"

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using JTAGClockScaler GENPYBIND(visible) = haldls::vx::JTAGClockScaler;
using ResetJTAGTap GENPYBIND(visible) = haldls::vx::ResetJTAGTap;
using JTAGIdCode GENPYBIND(visible) = haldls::vx::JTAGIdCode;

} // namespace v3
} // namespace haldls::vx
