#include "lola/vx/v4/chip.h"

#include "halco/common/iter_all.h"
#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

namespace lola::vx::v4 {

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
	return print(os, config);
}

Chip const Chip::default_neuron_bypass = []() {
	Chip config;

	using namespace halco::common;
	using namespace halco::hicann_dls::vx::v4;

	for (auto const neuron : iter_all<AtomicNeuronOnDLS>()) {
		config.neuron_block.atomic_neurons[neuron].threshold.enable = false;
		config.neuron_block.atomic_neurons[neuron].event_routing.enable_bypass_excitatory = true;
		config.neuron_block.atomic_neurons[neuron].event_routing.enable_bypass_inhibitory = true;
	}

	for (auto const block : iter_all<SynapseDriverBlockOnDLS>()) {
		auto value = config.synapse_driver_blocks[block].padi_bus.get_dacen_pulse_extension();
		value.fill(haldls::vx::v4::CommonPADIBusConfig::DacenPulseExtension(
		    haldls::vx::v4::CommonPADIBusConfig::DacenPulseExtension::max));
		config.synapse_driver_blocks[block].padi_bus.set_dacen_pulse_extension(value);
	}

	return config;
}();

} // namespace lola::vx::v4

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::v4::Chip)
