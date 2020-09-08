#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/i2c.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using INA219Config GENPYBIND(visible) = haldls::vx::INA219Config;
using INA219Status GENPYBIND(visible) = haldls::vx::INA219Status;

} // namespace haldls::vx::v2
