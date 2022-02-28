#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/capmem.h"

#include "halco/common/iter_all.h"
#include "haldls/cerealization.tcc"
#include "hate/indent.h"
#include "hate/join.h"
#include "lola/vx/cerealization.tcc"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 1
#define CHIP_REVISION_STR v1
#elif CHIP_REVISION == 2
#define CHIP_REVISION_STR v2
#elif CHIP_REVISION == 3
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
	os << "CapMem(\n";
	for (auto block : halco::common::iter_all<
	         halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockConfigOnDLS>()) {
		os << block << ":\n";
		std::stringstream ss;
		ss << config.blocks[block];
		os << hate::indent(ss.str(), "\t") << "\n";
	}
	std::stringstream ss;
	ss << config.synapse_bias_selection << "\n";
	ss << config.reference_generator;
	os << hate::indent(ss.str(), "\t") << "\n";
	os << "unused_cells:\n"
	   << hate::indent(
	          hate::join_string(config.unused_cells.begin(), config.unused_cells.end(), "\n"), "\t")
	   << "\n";
	os << ")";
	return os;
}

}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::CapMem)
