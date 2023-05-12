#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/synapse_block.h"

#include "haldls/cerealization.tcc"
#include "hate/indent.h"
#include "hate/join.h"
#include "lola/vx/cerealization.tcc"
#include "lola/vx/hana.h"
#include <boost/hana/adapt_struct.hpp>

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

bool SynapseBlock::operator==(SynapseBlock const& other) const
{
	return equal(*this, other);
}

bool SynapseBlock::operator!=(SynapseBlock const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseBlock const& config)
{
	os << "SynapseBlock(\n";
	{
		std::stringstream ss;
		ss << config.matrix;
		os << hate::indent(ss.str(), "\t") << "\n";
	}
	os << "\ti_bias_dac: "
	   << "[left: " << config.i_bias_dac[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << ", right: " << config.i_bias_dac[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n)";
	return os;
}

} // namespace lola::vx::vY

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::SynapseBlock)
