#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/readout.h"

namespace haldls::vx::v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using PadMultiplexerConfig GENPYBIND(visible) = haldls::vx::PadMultiplexerConfig;
using ReadoutSourceSelection GENPYBIND(visible) = haldls::vx::ReadoutSourceSelection;

} // namespace haldls::vx::v4
