#include "lola/vx/memory_timing.h"

#include "haldls/vx/container.tcc"
#include "hate/indent.h"
#include "hate/join.h"
#include "lola/vx/hana.h"
#include <sstream>

namespace lola::vx {

bool MemoryTiming::operator==(MemoryTiming const& other) const
{
	return (cadc_offset == other.cadc_offset) && (synapse_driver == other.synapse_driver) &&
	       (neuron == other.neuron) && (neuron_backend == other.neuron_backend) &&
	       (synram == other.synram);
}

bool MemoryTiming::operator!=(MemoryTiming const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, MemoryTiming const& config)
{
	os << "MemoryTiming(\n";
	hate::IndentingOstream ios(os);
	boost::hana::for_each(boost::hana::keys(config), [&](auto const key) {
		ios << hate::Indentation("\t");
		ios << key.c_str() << ": [\n"
		    << hate::Indentation("\t\t") << hate::join(boost::hana::at_key(config, key), ",\n")
		    << hate::Indentation("\t") << "]\n";
	});
	ios << hate::Indentation() << ")";
	return os;
}

} // namespace lola::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::MemoryTiming)
