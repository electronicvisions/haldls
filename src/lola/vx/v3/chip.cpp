#include "lola/vx/v3/chip.h"

#include "halco/common/iter_all.h"
#include "haldls/cerealization.tcc"
#include "hate/indent.h"
#include "lola/vx/cerealization.tcc"
#include "lola/vx/hana.h"

namespace lola::vx::v3 {

bool Chip::operator==(Chip const& other) const
{
	return equal(*this, other);
}

bool Chip::operator!=(Chip const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, Chip const& config)
{
	using namespace halco::hicann_dls::vx::v3;
	using namespace halco::common;

	os << "Chip(\n";

	{
		os << "\tmemory_timing:\n";
		std::stringstream ss;
		ss << config.memory_timing;
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tcrossbar:\n";
		std::stringstream ss;
		ss << config.crossbar;
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tbackground_spike_sources:\n";
		std::stringstream ss;
		for (auto const c : iter_all<BackgroundSpikeSourceOnDLS>()) {
			ss << c << ": " << config.background_spike_sources[c];
		}
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tcapmem:\n";
		std::stringstream ss;
		ss << config.capmem;
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tsynapse_driver_blocks:\n";
		std::stringstream ss;
		for (auto const c : iter_all<SynapseDriverBlockOnDLS>()) {
			ss << c << ": " << config.synapse_driver_blocks[c];
		}
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tsynapse_blocks:\n";
		std::stringstream ss;
		for (auto const c : iter_all<SynapseBlockOnDLS>()) {
			ss << c << ": " << config.synapse_blocks[c];
		}
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tneuron_block:\n";
		std::stringstream ss;
		ss << config.neuron_block;
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tcadc_readout_chains:\n";
		std::stringstream ss;
		for (auto const c : iter_all<CADCOnDLS>()) {
			ss << config.cadc_readout_chains[c];
		}
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\treadout_chain:\n";
		std::stringstream ss;
		ss << config.readout_chain;
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\tppu_memory:\n";
		std::stringstream ss;
		for (auto const c : iter_all<PPUMemoryOnDLS>()) {
			ss << c << ": " << config.ppu_memory[c];
		}
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	{
		os << "\texternal_ppu_memory:\n";
		std::stringstream ss;
		ss << config.external_ppu_memory;
		os << hate::indent(ss.str(), "\t\t") << "\n";
	}
	os << ")";
	return os;
}

} // namespace lola::vx::v3

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::Chip)
