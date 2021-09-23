#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/i2c.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using INA219Config GENPYBIND(visible) = haldls::vx::INA219Config;
using INA219Status GENPYBIND(visible) = haldls::vx::INA219Status;
using TCA9554Inputs GENPYBIND(visible) = haldls::vx::TCA9554Inputs;
using TCA9554Config GENPYBIND(visible) = haldls::vx::TCA9554Config;

} // namespace haldls::vx::v3
