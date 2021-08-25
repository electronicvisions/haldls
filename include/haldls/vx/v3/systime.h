#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/systime.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using SystimeSyncBase GENPYBIND(visible) = haldls::vx::SystimeSyncBase;
using SystimeSync GENPYBIND(visible) = haldls::vx::SystimeSync;

} // namespace haldls::vx::v3
