#pragma once
#include <stdint.h>
#include "halco/hicann-dls/vx/v1/synapse.h"
#include "hate/visibility.h"

namespace haldls::vx::v1::detail {

uint32_t to_synram_quad_address_offset(
    halco::hicann_dls::vx::v1::SynapseQuadColumnOnDLS const& column) SYMBOL_VISIBLE;

} // namespace haldls::vx::detail
