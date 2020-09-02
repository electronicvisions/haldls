#include "haldls/vx/v1/capmem.h"

#include "haldls/vx/capmem.tcc"

#include "halco/hicann-dls/vx/v1/coordinates.h"

namespace haldls::vx {

CAPMEM_CELL_UNROLL(halco::hicann_dls::vx::v1::Coordinates)
CAPMEM_BLOCK_UNROLL(halco::hicann_dls::vx::v1::Coordinates)
CAPMEM_BLOCK_CONFIG_UNROLL(halco::hicann_dls::vx::v1::Coordinates)

} // namespace haldls::vx

CAPMEM_CEREAL_VERSION(halco::hicann_dls::vx::v1::Coordinates)
