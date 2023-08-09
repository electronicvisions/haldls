#pragma once
#include "lola/vx/genpybind.h"
#include "lola/vx/routing_crossbar.h"

namespace lola::vx::v4 GENPYBIND_TAG_LOLA_VX_V4 {

using Crossbar GENPYBIND(visible) = lola::vx::Crossbar;

} // namespace lola::vx::v4
