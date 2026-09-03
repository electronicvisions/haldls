#include "lola/vx/v3/chip.h"

#include "halco/common/iter_all.h"
#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"
#include "lola/vx/v3/neuron.h"

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
	return print(os, config);
}

Chip const Chip::default_neuron_bypass = []() {
	Chip config;

	using namespace halco::common;
	using namespace halco::hicann_dls::vx::v3;

	for (auto const neuron : iter_all<AtomicNeuronOnDLS>()) {
		auto& n_config = config.neuron_block.atomic_neurons[neuron];
		n_config.threshold.enable = false;
		n_config.event_routing.enable_bypass_excitatory = true;
		n_config.event_routing.enable_bypass_inhibitory = true;
		n_config.event_routing.enable_digital = true;
		n_config.event_routing.analog_output = AtomicNeuron::EventRouting::AnalogOutputMode::normal;
	}

	for (auto const drv : iter_all<SynapseDriverOnDLS>()) {
		auto& s_config = config.synapse_driver_blocks[drv.toSynapseDriverBlockOnDLS()]
		                     .synapse_drivers[drv.toSynapseDriverOnSynapseDriverBlock()];
		s_config.set_enable_receiver(true);
		s_config.set_enable_address_out(true);
	}

	for (auto const s : iter_all<SynapseOnSynapseRow>()) {
		for (auto& current_row : config.neuron_block.current_rows) {
			auto& sw = current_row.values[s];
			sw.set_enable_synaptic_current_excitatory(true);
			sw.set_enable_synaptic_current_inhibitory(true);
		}
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
