#include "lola/vx/v3/chip.h"

#include "halco/common/iter_all.h"
#include "haldls/vx/container.tcc"
#include "hate/indent.h"
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

	hate::IndentingOstream ios(os);
	ios << "Chip(\n";
	ios << hate::Indentation("\t");
	ios << "memory_timing:\n";
	ios << hate::Indentation("\t\t");
	ios << config.memory_timing;
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "crossbar:\n";
	ios << hate::Indentation("\t\t");
	ios << config.crossbar;
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "background_spike_sources:\n";
	ios << hate::Indentation("\t\t");
	for (auto const c : iter_all<BackgroundSpikeSourceOnDLS>()) {
		ios << c << ": " << config.background_spike_sources[c];
	}
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "capmem:\n";
	ios << hate::Indentation("\t\t");
	ios << config.capmem;
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "synapse_driver_blocks:\n";
	ios << hate::Indentation("\t\t");
	for (auto const c : iter_all<SynapseDriverBlockOnDLS>()) {
		ios << c << ": " << config.synapse_driver_blocks[c];
	}
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "synapse_blocks:\n";
	ios << hate::Indentation("\t\t");
	for (auto const c : iter_all<SynapseBlockOnDLS>()) {
		ios << c << ": " << config.synapse_blocks[c];
	}
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "neuron_block:\n";
	ios << hate::Indentation("\t\t");
	ios << config.neuron_block;
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "cadc_readout_chains:\n";
	ios << hate::Indentation("\t\t");
	for (auto const c : iter_all<CADCOnDLS>()) {
		ios << config.cadc_readout_chains[c];
	}
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "readout_chain:\n";
	ios << hate::Indentation("\t\t");
	ios << config.readout_chain;
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "ppu_memory:\n";
	for (auto const c : iter_all<PPUMemoryOnDLS>()) {
		ios << c << ": " << config.ppu_memory[c];
	}
	ios << hate::Indentation("\t");
	ios << "\n";
	ios << "external_ppu_memory:\n";
	ios << hate::Indentation("\t\t");
	ios << config.external_ppu_memory;
	ios << hate::Indentation();
	ios << "\n";
	ios << ")";
	return os;
}

Chip const Chip::default_neuron_bypass = []() {
	Chip config;

	using namespace halco::common;
	using namespace halco::hicann_dls::vx::v3;

	for (auto const neuron : iter_all<AtomicNeuronOnDLS>()) {
		config.neuron_block.atomic_neurons[neuron].threshold.enable = false;
		config.neuron_block.atomic_neurons[neuron].event_routing.enable_bypass_excitatory = true;
		config.neuron_block.atomic_neurons[neuron].event_routing.enable_bypass_inhibitory = true;
	}

	for (auto const block : iter_all<SynapseDriverBlockOnDLS>()) {
		auto value = config.synapse_driver_blocks[block].padi_bus.get_dacen_pulse_extension();
		value.fill(haldls::vx::v3::CommonPADIBusConfig::DacenPulseExtension(
		    haldls::vx::v3::CommonPADIBusConfig::DacenPulseExtension::max));
		config.synapse_driver_blocks[block].padi_bus.set_dacen_pulse_extension(value);
	}

	return config;
}();

} // namespace lola::vx::v3

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::v3::Chip)
