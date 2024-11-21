#include "lola/vx/memory_timing.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

namespace lola::vx {

bool MemoryTiming::operator==(MemoryTiming const& other) const
{
	return (capmem == other.capmem) && (cadc_offset == other.cadc_offset) &&
	       (synapse_driver == other.synapse_driver) && (neuron == other.neuron) &&
	       (neuron_backend == other.neuron_backend) && (synram == other.synram);
}

bool MemoryTiming::operator!=(MemoryTiming const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, MemoryTiming const& config)
{
	return print(os, config);
}

} // namespace lola::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::MemoryTiming)
