#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/routing_crossbar.h"

namespace lola::vx::v3 GENPYBIND_TAG_LOLA_VX_V3 {

using Crossbar GENPYBIND(visible) = lola::vx::Crossbar;

} // namespace lola::vx::v3
