#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/systime.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using SystimeSyncBase GENPYBIND(visible) = haldls::vx::SystimeSyncBase;
using SystimeSync GENPYBIND(visible) = haldls::vx::SystimeSync;

} // namespace v4
} // namespace haldls::vx
