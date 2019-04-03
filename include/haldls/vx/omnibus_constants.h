#pragma once
#include "haldls/vx/common.h"

namespace haldls::vx {

constexpr omnibus_address_type top_ppu_base_address{0x02800000};
constexpr omnibus_address_type bottom_ppu_base_address{0x03800000};

constexpr omnibus_address_type spi_over_omnibus_mask{0x01000000};
constexpr omnibus_word_type spi_over_omnibus_stop_bit{0x80000000};

} // namespace haldls::vx
