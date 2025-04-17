#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/synapse_driver.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

bool SynapseDriverBlock::operator==(SynapseDriverBlock const& other) const
{
	return equal(*this, other);
}

bool SynapseDriverBlock::operator!=(SynapseDriverBlock const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseDriverBlock const& config)
{
	return print(os, config);
}

} // namespace lola::vx::CHIP_REVISION_STR

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::SynapseDriverBlock)
