#pragma once
#include <stdint.h>
#include "hate/visibility.h"

namespace halco::hicann_dls::vx {
struct SynapseQuadColumnOnDLS;
} // namespace halco::hicann_dls::vx

namespace haldls::vx::detail {

uint32_t to_synram_quad_address_offset(halco::hicann_dls::vx::SynapseQuadColumnOnDLS const& column)
    SYMBOL_VISIBLE;

} // namespace haldls::vx::detail
