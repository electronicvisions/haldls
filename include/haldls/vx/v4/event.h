#pragma once
#include "haldls/vx/event.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx { namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

using SpikePack1ToChip GENPYBIND(visible) = haldls::vx::SpikePack1ToChip;
using SpikePack2ToChip GENPYBIND(visible) = haldls::vx::SpikePack2ToChip;
using SpikePack3ToChip GENPYBIND(visible) = haldls::vx::SpikePack3ToChip;
#ifndef __ppu__
using FPGATime GENPYBIND(visible) = haldls::vx::FPGATime;
using ChipTime GENPYBIND(visible) = haldls::vx::ChipTime;
using SpikeFromChip GENPYBIND(visible) = haldls::vx::SpikeFromChip;
using MADCSampleFromChip GENPYBIND(visible) = haldls::vx::MADCSampleFromChip;
using HighspeedLinkNotification GENPYBIND(visible) = haldls::vx::HighspeedLinkNotification;
#endif

} // namespace v4
} // namespace haldls::vx
