#pragma once
#include "lola/vx/dac.h"
#include "lola/vx/genpybind.h"

namespace lola::vx::v2 GENPYBIND_TAG_LOLA_VX_V2 {

using DACChannelBlock GENPYBIND(visible) = lola::vx::DACChannelBlock;
using DACControlBlock GENPYBIND(visible) = lola::vx::DACControlBlock;

} // namespace lola::vx::v2
