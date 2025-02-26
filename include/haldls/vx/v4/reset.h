#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/reset.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using ResetChip GENPYBIND(visible) = haldls::vx::ResetChip;

} // namespace v4
} // namespace haldls::vx
