#pragma once

#include <array>
#include "haldls/vx/common.h"

namespace haldls::vx {

constexpr uint32_t top_ppu_base_address{0x0280'0000};
constexpr uint32_t bottom_ppu_base_address{0x0380'0000};

constexpr uint32_t phy_on_chip_base_address{0x0004'0000};
constexpr uint32_t pll_base_address{0x0008'0000};

constexpr uint32_t capmem_nw_sram_base_address{0x140000};
constexpr uint32_t capmem_ne_sram_base_address{0x148000};
constexpr uint32_t capmem_sw_sram_base_address{0x150000};
constexpr uint32_t capmem_se_sram_base_address{0x158000};
constexpr std::array<uint32_t, 4> capmem_sram_base_addresses = {
    capmem_nw_sram_base_address, capmem_ne_sram_base_address, capmem_sw_sram_base_address,
    capmem_se_sram_base_address};


constexpr uint32_t executor_omnibus_mask{0x80000000};
constexpr uint32_t ut_omnibus_mask{0x04000000 | executor_omnibus_mask};
constexpr uint32_t phy_omnibus_mask{0x02000000 | ut_omnibus_mask};

} // namespace haldls::vx
