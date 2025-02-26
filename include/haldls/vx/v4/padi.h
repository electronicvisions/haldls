#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/padi.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using PADIEvent GENPYBIND(visible) = haldls::vx::PADIEvent;
using CommonPADIBusConfig GENPYBIND(visible) = haldls::vx::CommonPADIBusConfig;
using CommonSTPConfig GENPYBIND(visible) = haldls::vx::CommonSTPConfig;

} // namespace v4
} // namespace haldls::vx
