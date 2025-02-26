#pragma once
#include "haldls/vx/block.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
namespace v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using PollingOmnibusBlockConfig GENPYBIND(visible) = haldls::vx::PollingOmnibusBlockConfig;
using PollingOmnibusBlock GENPYBIND(visible) = haldls::vx::PollingOmnibusBlock;

} // namespace v3
} // namespace haldls::vx
