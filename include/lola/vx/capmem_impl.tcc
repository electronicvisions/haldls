#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/capmem.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

bool CapMem::operator==(CapMem const& other) const
{
	return equal(*this, other);
}

bool CapMem::operator!=(CapMem const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, CapMem const& config)
{
	return print(os, config);
}

}

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::CapMem)
