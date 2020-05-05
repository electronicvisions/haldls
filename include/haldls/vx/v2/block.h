#pragma once
#include "haldls/vx/block.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using PollingOmnibusBlockConfig GENPYBIND(visible) = haldls::vx::PollingOmnibusBlockConfig;
using PollingOmnibusBlock GENPYBIND(visible) = haldls::vx::PollingOmnibusBlock;

} // namespace haldls::vx::v2
