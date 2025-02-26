#pragma once
#include "haldls/vx/barrier.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using Barrier GENPYBIND(visible) = haldls::vx::Barrier;

} // namespace v4
} // namespace haldls::vx
