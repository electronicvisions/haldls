#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/capmem.h"

#include "halco/common/iter_all.h"
#include "haldls/vx/container.tcc"
#include "hate/indent.h"
#include "hate/join.h"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#elif CHIP_REVISION == 4
#define CHIP_REVISION_STR v4
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
	os << "CapMem(\n";
	for (auto block : halco::common::iter_all<
	         halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockConfigOnDLS>()) {
		os << block << ":\n";
		{
			hate::IndentingOstream ios(os);
			ios << hate::Indentation("\t");
			ios << config.blocks[block];
		}
		os << "\n";
	}
	{
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t");
		ios << config.synapse_bias_selection << "\n";
		ios << config.reference_generator;
	}
	os << "\n";
	os << "unused_cells:\n";
	{
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t");
		ios << hate::join(config.unused_cells, "\n");
	}
	os << "\n)";
	return os;
}

}

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::CapMem)
