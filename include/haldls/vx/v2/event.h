#pragma once
#include "haldls/vx/event.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

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

} // namespace haldls::vx::v2
