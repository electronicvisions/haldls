#pragma once
#include "haldls/vx/event.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using SpikeLabel GENPYBIND(visible) = haldls::vx::SpikeLabel;
using SpikePack1ToChip GENPYBIND(visible) = haldls::vx::SpikePack1ToChip;
using SpikePack2ToChip GENPYBIND(visible) = haldls::vx::SpikePack2ToChip;
using SpikePack3ToChip GENPYBIND(visible) = haldls::vx::SpikePack3ToChip;
using FPGATime GENPYBIND(visible) = haldls::vx::FPGATime;
using ChipTime GENPYBIND(visible) = haldls::vx::ChipTime;
using SpikeFromChip GENPYBIND(visible) = haldls::vx::SpikeFromChip;
using MADCSampleFromChip GENPYBIND(visible) = haldls::vx::MADCSampleFromChip;
using HighspeedLinkNotification GENPYBIND(visible) = haldls::vx::HighspeedLinkNotification;

} // namespace haldls::vx::v3
