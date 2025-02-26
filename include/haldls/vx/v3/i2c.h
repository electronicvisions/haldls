#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/i2c.h"

namespace haldls::vx { namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using INA219Config GENPYBIND(visible) = haldls::vx::INA219Config;
using INA219Status GENPYBIND(visible) = haldls::vx::INA219Status;
using TCA9554Inputs GENPYBIND(visible) = haldls::vx::TCA9554Inputs;
using TCA9554Config GENPYBIND(visible) = haldls::vx::TCA9554Config;
using AD5252ChannelConfig GENPYBIND(visible) = haldls::vx::AD5252ChannelConfig;
using AD5252ChannelConfigPersistent GENPYBIND(visible) = haldls::vx::AD5252ChannelConfigPersistent;
using DAC6573ChannelConfig GENPYBIND(visible) = haldls::vx::DAC6573ChannelConfig;

} // namespace v3
} // namespace haldls::vx
