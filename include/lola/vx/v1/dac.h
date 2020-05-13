#pragma once
#include "lola/vx/dac.h"
#include "lola/vx/genpybind.h"

namespace lola::vx::v1 GENPYBIND_TAG_LOLA_VX_V1 {

using DACChannelBlock GENPYBIND(visible) = lola::vx::DACChannelBlock;
using DACControlBlock GENPYBIND(visible) = lola::vx::DACControlBlock;

} // namespace lola::vx::v1
