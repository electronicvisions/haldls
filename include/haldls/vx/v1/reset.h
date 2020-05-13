#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/reset.h"

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using ResetChip GENPYBIND(visible) = haldls::vx::ResetChip;

} // namespace haldls::vx::v1
