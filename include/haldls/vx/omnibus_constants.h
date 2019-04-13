#pragma once
#include "haldls/vx/common.h"

namespace haldls::vx {

constexpr omnibus_address_type top_ppu_base_address{0x02800000};
constexpr omnibus_address_type bottom_ppu_base_address{0x03800000};

constexpr omnibus_address_type phy_on_chip_base_address{0x0004'0000};
constexpr omnibus_address_type pll_base_address{0x0008'0000};

constexpr omnibus_address_type executor_omnibus_mask{0x80000000};
constexpr omnibus_address_type ut_omnibus_mask{0x04000000 | executor_omnibus_mask};
constexpr omnibus_address_type phy_omnibus_mask{0x02000000 | ut_omnibus_mask};

} // namespace haldls::vx
