#pragma once

#include <array>
#include "haldls/vx/common.h"

namespace haldls::vx {

constexpr uint32_t ppu_top_subtree_address{0x0200'0000};
constexpr uint32_t ppu_bottom_subtree_address{0x0300'0000};
constexpr uint32_t top_ppu_base_address{0x0080'0000 | ppu_top_subtree_address};
constexpr uint32_t bottom_ppu_base_address{0x0080'0000 | ppu_bottom_subtree_address};
constexpr uint32_t ppu_control_register_address_mask{0x0020'0000};
constexpr uint32_t ppu_status_register_address_mask{0x0020'0001};

constexpr uint32_t phy_on_chip_base_address{0x0004'0000};
constexpr uint32_t pll_base_address{0x0008'0000};
constexpr uint32_t systime_base_address{0x0};

constexpr uint32_t capmem_nw_sram_base_address{0x140000};
constexpr uint32_t capmem_ne_sram_base_address{0x148000};
constexpr uint32_t capmem_sw_sram_base_address{0x150000};
constexpr uint32_t capmem_se_sram_base_address{0x158000};
constexpr std::array<uint32_t, 4> capmem_sram_base_addresses = {
    capmem_nw_sram_base_address, capmem_ne_sram_base_address, capmem_sw_sram_base_address,
    capmem_se_sram_base_address};

constexpr uint32_t neuron_nw_sram_base_address{0x14'0000};
constexpr uint32_t neuron_ne_sram_base_address{0x14'8000};
constexpr uint32_t neuron_sw_sram_base_address{0x15'0000};
constexpr uint32_t neuron_se_sram_base_address{0x15'8000};
constexpr std::array<uint32_t, 4> neuron_sram_base_addresses = {
    neuron_nw_sram_base_address, neuron_ne_sram_base_address, neuron_sw_sram_base_address,
    neuron_se_sram_base_address};

constexpr uint32_t ut_omnibus_mask{0x0400'0000};
constexpr uint32_t phy_omnibus_mask{0x0200'0000 | ut_omnibus_mask};
constexpr uint32_t perftest_omnibus_mask{0x0800'0000};

} // namespace haldls::vx