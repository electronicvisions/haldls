#include "lola/vx/v1/neuron.h"

#include "haldls/cerealization.tcc"
#include "lola/vx/cerealization.tcc"
#include "lola/vx/hana.h"

namespace lola::vx::v1 {

AtomicNeuron::SynapticInput::SynapticInput() : enable(false), i_bias_res(), i_bias_gm(), v_syn() {}

bool AtomicNeuron::SynapticInput::operator==(SynapticInput const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::SynapticInput::operator!=(SynapticInput const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::SynapticInput const& config)
{
	std::stringstream ss;
	ss << "SynapticInput(enable: " << std::boolalpha << config.enable << ", v_syn: " << config.v_syn
	   << ", i_bias_res: " << config.i_bias_res << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::LeakReset::Leak::Leak() :
    v_leak(),
    i_bias(),
    enable_degeneration(false),
    enable_division(false),
    enable_multiplication(false)
{}

bool AtomicNeuron::LeakReset::Leak::operator==(Leak const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::LeakReset::Leak::operator!=(Leak const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::LeakReset::Leak const& config)
{
	std::stringstream ss;
	ss << "Leak(v_leak: " << config.v_leak << ", i_bias: " << config.i_bias
	   << ", enable_degeneration: " << std::boolalpha << config.enable_degeneration
	   << ", enable_division: " << config.enable_division
	   << ", enable_multiplication: " << config.enable_multiplication << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::LeakReset::Reset::Reset() :
    v_reset(),
    i_bias(),
    enable_degeneration(false),
    enable_division(false),
    enable_multiplication(false)
{}

bool AtomicNeuron::LeakReset::Reset::operator==(Reset const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::LeakReset::Reset::operator!=(Reset const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::LeakReset::Reset const& config)
{
	std::stringstream ss;
	ss << "Reset(v_reset: " << config.v_reset << ", i_bias: " << config.i_bias
	   << ", enable_degeneration: " << std::boolalpha << config.enable_degeneration
	   << ", enable_division: " << config.enable_division
	   << ", enable_multiplication: " << config.enable_multiplication << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::LeakReset::LeakReset() : leak(), reset(), i_bias_source_follower() {}

bool AtomicNeuron::LeakReset::operator==(LeakReset const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::LeakReset::operator!=(LeakReset const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::LeakReset const& config)
{
	std::stringstream ss;
	ss << "LeakReset(" << std::endl
	   << config.leak << std::endl
	   << config.reset << std::endl
	   << "i_bias_source_follower: " << config.i_bias_source_follower << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Threshold::Threshold() : enable(false), v_threshold() {}

bool AtomicNeuron::Threshold::operator==(Threshold const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Threshold::operator!=(Threshold const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Threshold const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "Threshold(enable: " << std::boolalpha << config.enable
	   << ", v_threshold: " << config.v_threshold << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::ConstantCurrent::ConstantCurrent() : enable(false), i_offset() {}

bool AtomicNeuron::ConstantCurrent::operator==(ConstantCurrent const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::ConstantCurrent::operator!=(ConstantCurrent const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::ConstantCurrent const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "ConstantCurrent(enable: " << std::boolalpha << config.enable
	   << ", i_offset: " << config.i_offset << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Multicompartment::Multicompartment() :
    connect_soma(false),
    connect_soma_right(false),
    connect_right(false),
    connect_vertical(false),
    enable_conductance(false),
    enable_conductance_division(false),
    enable_conductance_multiplication(false)
{}

bool AtomicNeuron::Multicompartment::operator==(Multicompartment const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Multicompartment::operator!=(Multicompartment const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Multicompartment const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "Multicompartment(" << std::boolalpha << "connect_soma: " << config.connect_soma << ", "
	   << "connect_soma_right: " << config.connect_soma_right << ", "
	   << "connect_right: " << config.connect_right << ", "
	   << "connect_vertical: " << config.connect_vertical << ", "
	   << "enable_conductance: " << config.enable_conductance << ", "
	   << "enable_conductance_division: " << config.enable_conductance_division << ", "
	   << "enable_conductance_multiplication: " << config.enable_conductance_multiplication << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::MembraneCapacitance::MembraneCapacitance() : enable_merge(false), capacitance() {}

bool AtomicNeuron::MembraneCapacitance::operator==(MembraneCapacitance const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::MembraneCapacitance::operator!=(MembraneCapacitance const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::MembraneCapacitance const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "MembraneCapacitance(" << std::boolalpha << "enable_merge: " << config.enable_merge
	   << ", capacitance: " << config.capacitance << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Adaptation::Adaptation() :
    enable(false),
    enable_pulse(false),
    enable_capacitor(false),
    invert_a(false),
    invert_b(false),
    i_bias(),
    i_bias_amp(),
    i_bias_sd(),
    i_bias_res(),
    i_w(),
    v_leak()
{}

bool AtomicNeuron::Adaptation::operator==(Adaptation const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Adaptation::operator!=(Adaptation const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Adaptation const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "Adaptation(enable: " << std::boolalpha << config.enable
	   << ", enable_capacitor: " << config.enable_capacitor
	   << ", enable_pulse: " << config.enable_pulse << ", invert_a: " << config.invert_a
	   << ", invert_b: " << config.invert_b << ", i_bias: " << config.i_bias
	   << ", i_bias_amp: " << config.i_bias_amp << ", i_bias_sd: " << config.i_bias_sd
	   << ", i_bias_res: " << config.i_bias_res << ", i_w: " << config.i_w
	   << ", v_leak: " << config.v_leak << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Exponential::Exponential() : enable(false), strength() {}

bool AtomicNeuron::Exponential::operator==(Exponential const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Exponential::operator!=(Exponential const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Exponential const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "Exponential(enable: " << std::boolalpha << config.enable
	   << ", strength: " << config.strength << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Readout::Readout() :
    enable_buffered_access(false),
    enable_amplifier(false),
    enable_unbuffered_access(false),
    source(),
    i_bias()
{}

bool AtomicNeuron::Readout::operator==(Readout const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Readout::operator!=(Readout const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Readout const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "Readout(enable_buffered_access: " << std::boolalpha << config.enable_buffered_access
	   << ", enable_amplifier: " << config.enable_amplifier
	   << ", enable_unbuffered_access: " << config.enable_unbuffered_access
	   << ", source: " << config.source << ", i_bias: " << config.i_bias << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::EventRouting::EventRouting() :
    enable_analog(false),
    enable_digital(false),
    enable_bypass_excitatory(false),
    enable_bypass_inhibitory(false),
    address(),
    enable_post_overwrite(false)
{}

AtomicNeuron::EventRouting::EventRouting(
    bool const enable_analog,
    bool const enable_digital,
    bool const enable_bypass_excitatory,
    bool const enable_bypass_inhibitory,
    Address const address,
    bool const enable_post_overwrite) :
    enable_analog(enable_analog),
    enable_digital(enable_digital),
    enable_bypass_excitatory(enable_bypass_excitatory),
    enable_bypass_inhibitory(enable_bypass_inhibitory),
    address(address),
    enable_post_overwrite(enable_post_overwrite)
{}

bool AtomicNeuron::EventRouting::operator==(EventRouting const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::EventRouting::operator!=(EventRouting const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::EventRouting const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "EventRouting(enable_analog: " << std::boolalpha << config.enable_analog
	   << ", enable_digital: " << config.enable_digital << ", address: " << config.address
	   << ", enable_bypass_excitatory: " << config.enable_bypass_excitatory
	   << ", enable_bypass_inhibitory: " << config.enable_bypass_inhibitory
	   << ", enable_post_overwrite: " << config.enable_post_overwrite << ")";
	os << ss.str();
	return os;
}

AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::disabled = AtomicNeuron::EventRouting(
    false, false, false, false, AtomicNeuron::EventRouting::Address(), false);
AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::enabled = AtomicNeuron::EventRouting(
    true, true, false, false, AtomicNeuron::EventRouting::Address(), false);
AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::bypass_exc =
    AtomicNeuron::EventRouting(
        false, false, true, false, AtomicNeuron::EventRouting::Address(), false);
AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::bypass_inh =
    AtomicNeuron::EventRouting(
        false, false, false, true, AtomicNeuron::EventRouting::Address(), false);


AtomicNeuron::RefractoryPeriod::RefractoryPeriod() :
    input_clock(), reset_holdoff(), refractory_time()
{}

bool AtomicNeuron::RefractoryPeriod::operator==(RefractoryPeriod const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::RefractoryPeriod::operator!=(RefractoryPeriod const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::RefractoryPeriod const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "RefractoryPeriod(input_clock: " << config.input_clock
	   << ", refractory_time: " << config.refractory_time
	   << ", reset_holdoff: " << config.reset_holdoff << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Bayesian::Bayesian() :
    enable(false),
    connect_fire_vertical(false),
    connect_fire_to_right(false),
    connect_fire_from_right(false),
    enable_master(true),
    enable_slave(false),
    enable_0(false),
    enable_1(false)
{}

bool AtomicNeuron::Bayesian::operator==(Bayesian const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Bayesian::operator!=(Bayesian const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Bayesian const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "Bayesian(" << std::boolalpha << "enable: " << config.enable << ", "
	   << "connect_fire_vertical: " << config.connect_fire_vertical << ", "
	   << "connect_fire_to_right: " << config.connect_fire_to_right << ", "
	   << "connect_fire_from_right: " << config.connect_fire_from_right << ", "
	   << "enable_master: " << config.enable_master << ", "
	   << "enable_slave: " << config.enable_slave << ", "
	   << "enable_0: " << config.enable_0 << ", "
	   << "enable_1: " << config.enable_1 << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::AtomicNeuron() :
    excitatory_input(),
    inhibitory_input(),
    leak_reset(),
    threshold(),
    multicompartment(),
    constant_current(),
    membrane_capacitance(),
    adaptation(),
    exponential(),
    event_routing(),
    readout(),
    refractory_period(),
    bayesian()
{}

bool AtomicNeuron::operator==(AtomicNeuron const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::operator!=(AtomicNeuron const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "AtomicNeuron(\n";
	ss << "\texcitatory_input: " << config.excitatory_input << "\n";
	ss << "\tinhibitory_input: " << config.inhibitory_input << "\n";
	ss << "\t" << config.leak_reset << "\n";
	ss << "\t" << config.threshold << "\n";
	ss << "\t" << config.multicompartment << "\n";
	ss << "\t" << config.constant_current << "\n";
	ss << "\t" << config.membrane_capacitance << "\n";
	ss << "\t" << config.adaptation << "\n";
	ss << "\t" << config.exponential << "\n";
	ss << "\t" << config.event_routing << "\n";
	ss << "\t" << config.readout << "\n";
	ss << "\t" << config.refractory_period << "\n";
	ss << "\t" << config.bayesian << "\n";
	ss << ")";
	os << ss.str();
	return os;
}

} // namespace lola::vx::v1

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::SynapticInput)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::LeakReset::Leak)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::LeakReset::Reset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::LeakReset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Threshold)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Multicompartment)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::ConstantCurrent)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::MembraneCapacitance)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Adaptation)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Exponential)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Readout)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::EventRouting)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::RefractoryPeriod)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron::Bayesian)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v1::AtomicNeuron)
