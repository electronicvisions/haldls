#include "haldls/vx/v1/address_transformation.h"

#include "halco/hicann-dls/vx/v1/synapse.h"

namespace haldls::vx::v1::detail {

uint32_t to_synram_quad_address_offset(
    halco::hicann_dls::vx::v1::SynapseQuadColumnOnDLS const& column)
{
	using namespace halco::hicann_dls::vx::v1;
	uint32_t quad_on_horizontal_half = column.toEnum() % (SynapseQuadColumnOnDLS::size / 2);
	uint32_t quad_quad = quad_on_horizontal_half / 4;
	uint32_t quad_in_quad = quad_on_horizontal_half % 4;
	bool is_east = column.toEnum() >= (SynapseQuadColumnOnDLS::size / 2);
	uint32_t offset = 16 * is_east + 32 * (quad_quad % 2) + 4 * (quad_quad / 2) + 3 - quad_in_quad;
	assert(offset < SynapseQuadColumnOnDLS::size);
	return offset;
}

} // namespace haldls::vx::detail::v1
