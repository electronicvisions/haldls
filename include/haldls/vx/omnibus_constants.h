#pragma once
#include "haldls/vx/common.h"

namespace haldls::vx {

constexpr omnibus_address_type top_ppu_base_address{0x02800000};
constexpr omnibus_address_type bottom_ppu_base_address{0x03800000};

constexpr omnibus_address_type pll_base_address{0x0008'0000};

} // namespace haldls::vx
