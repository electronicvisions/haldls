#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/spi.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using ShiftRegister GENPYBIND(visible) = haldls::vx::ShiftRegister;
using DACChannel GENPYBIND(visible) = haldls::vx::DACChannel;
using DACControl GENPYBIND(visible) = haldls::vx::DACControl;

} // namespace haldls::vx::v1
