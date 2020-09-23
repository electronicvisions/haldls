#include "lola/vx/v2/neuron.h"

#include "haldls/cerealization.tcc"
#include "lola/vx/cerealization.tcc"
#include "lola/vx/hana.h"

namespace lola::vx::v2 {

AtomicNeuron::SynapticInput::SynapticInput() :
    enable(false),
    i_bias_tau(),
    i_drop_input(),
    i_shift_reference(),
    i_bias_gm(),
    enable_small_capacitor(false),
    enable_high_resistance(false)
{}

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
	ss << "SynapticInput(enable: " << std::boolalpha << config.enable
	   << ", i_bias_tau: " << config.i_bias_tau << ", i_drop_input: " << config.i_drop_input
	   << ", i_shift_reference: " << config.i_shift_reference << ", i_bias_gm: " << config.i_bias_gm
	   << ", enable_small_capacitor: " << config.enable_small_capacitor
	   << ", enable_high_resistance: " << config.enable_high_resistance << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Leak::Leak() :
    v_leak(),
    i_bias(),
    enable_degeneration(false),
    enable_division(false),
    enable_multiplication(false)
{}

bool AtomicNeuron::Leak::operator==(Leak const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Leak::operator!=(Leak const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Leak const& config)
{
	std::stringstream ss;
	ss << "Leak(v_leak: " << config.v_leak << ", i_bias: " << config.i_bias
	   << ", enable_degeneration: " << std::boolalpha << config.enable_degeneration
	   << ", enable_division: " << config.enable_division
	   << ", enable_multiplication: " << config.enable_multiplication << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Reset::Reset() :
    v_reset(),
    i_bias(),
    enable_degeneration(false),
    enable_division(false),
    enable_multiplication(false)
{}

bool AtomicNeuron::Reset::operator==(Reset const& other) const
{
	return equal(*this, other);
}

bool AtomicNeuron::Reset::operator!=(Reset const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, AtomicNeuron::Reset const& config)
{
	std::stringstream ss;
	ss << "Reset(v_reset: " << config.v_reset << ", i_bias: " << config.i_bias
	   << ", enable_degeneration: " << std::boolalpha << config.enable_degeneration
	   << ", enable_division: " << config.enable_division
	   << ", enable_multiplication: " << config.enable_multiplication << ")";
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


AtomicNeuron::ConstantCurrent::ConstantCurrent() :
    enable(false), i_offset(), type(lola::vx::v2::AtomicNeuron::ConstantCurrent::Type::source)
{}

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
	   << ", i_offset: " << config.i_offset << ", type: "
	   << (config.type == lola::vx::v2::AtomicNeuron::ConstantCurrent::Type::sink ? "sink"
	                                                                              : "source")
	   << ")";
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


AtomicNeuron::MembraneCapacitance::MembraneCapacitance() : capacitance() {}

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
	ss << "MembraneCapacitance(" << std::boolalpha << ", capacitance: " << config.capacitance
	   << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Adaptation::Adaptation() :
    enable(false),
    enable_pulse(false),
    invert_a(false),
    invert_b(false),
    v_ref(),
    i_bias_tau(),
    i_bias_a(),
    i_bias_b(),
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
	   << ", enable_pulse: " << config.enable_pulse << ", invert_a: " << config.invert_a
	   << ", invert_b: " << config.invert_b << ", v_ref: " << config.v_ref
	   << ", i_bias_tau: " << config.i_bias_tau << ", i_bias_a: " << config.i_bias_a
	   << ", i_bias_b: " << config.i_bias_b << ", v_leak: " << config.v_leak << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Exponential::Exponential() : enable(false), v_exp(), i_bias() {}

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
	ss << "Exponential(enable: " << std::boolalpha << config.enable << ", v_exp: " << config.v_exp
	   << ", i_bias: " << config.i_bias << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::Readout::Readout() :
    enable_buffered_access(false),
    enable_amplifier(false),
    enable_unbuffered_access(false),
    source()
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
	   << ", enable_unbuffered_access: " << config.enable_unbuffered_access << ", source: ";
	switch (config.source) {
		case haldls::vx::v2::NeuronConfig::ReadoutSource::membrane:
			ss << "membrane";
			break;
		case haldls::vx::v2::NeuronConfig::ReadoutSource::exc_synin:
			ss << "exc_synin";
			break;
		case haldls::vx::v2::NeuronConfig::ReadoutSource::inh_synin:
			ss << "inh_synin";
			break;
		case haldls::vx::v2::NeuronConfig::ReadoutSource::adaptation:
			ss << "adaptation";
			break;
		default:
			break; // never reached
	}
	ss << ")";
	os << ss.str();
	return os;
}


AtomicNeuron::EventRouting::EventRouting() :
    analog_output(AtomicNeuron::EventRouting::AnalogOutputMode::off),
    enable_digital(false),
    enable_bypass_excitatory(false),
    enable_bypass_inhibitory(false),
    address(),
    enable_post_overwrite(false)
{}

AtomicNeuron::EventRouting::EventRouting(
    AtomicNeuron::EventRouting::AnalogOutputMode const analog_output,
    bool const enable_digital,
    bool const enable_bypass_excitatory,
    bool const enable_bypass_inhibitory,
    Address const address,
    bool const enable_post_overwrite) :
    analog_output(analog_output),
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
	ss << "EventRouting(analog_output: "
	   << (config.analog_output == AtomicNeuron::EventRouting::AnalogOutputMode::off
	           ? "off"
	           : (config.analog_output == AtomicNeuron::EventRouting::AnalogOutputMode::normal
	                  ? "normal"
	                  : "strong"))
	   << std::boolalpha << ", enable_digital: " << config.enable_digital
	   << ", address: " << config.address
	   << ", enable_bypass_excitatory: " << config.enable_bypass_excitatory
	   << ", enable_bypass_inhibitory: " << config.enable_bypass_inhibitory
	   << ", enable_post_overwrite: " << config.enable_post_overwrite << ")";
	os << ss.str();
	return os;
}

AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::disabled = AtomicNeuron::EventRouting(
    AtomicNeuron::EventRouting::AnalogOutputMode::off,
    false,
    false,
    false,
    AtomicNeuron::EventRouting::Address(),
    false);
AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::enabled = AtomicNeuron::EventRouting(
    AtomicNeuron::EventRouting::AnalogOutputMode::normal,
    true,
    false,
    false,
    AtomicNeuron::EventRouting::Address(),
    false);
AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::bypass_exc =
    AtomicNeuron::EventRouting(
        AtomicNeuron::EventRouting::AnalogOutputMode::off,
        false,
        true,
        false,
        AtomicNeuron::EventRouting::Address(),
        false);
AtomicNeuron::EventRouting const AtomicNeuron::EventRouting::bypass_inh =
    AtomicNeuron::EventRouting(
        AtomicNeuron::EventRouting::AnalogOutputMode::off,
        false,
        false,
        true,
        AtomicNeuron::EventRouting::Address(),
        false);


AtomicNeuron::RefractoryPeriod::RefractoryPeriod() :
    input_clock(), reset_holdoff(), refractory_time(), enable_pause(false)
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
	   << ", enable_pause: " << config.enable_pause
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
    leak(),
    reset(),
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
	ss << "excitatory_input: " << config.excitatory_input << std::endl;
	ss << "inhibitory_input: " << config.inhibitory_input << std::endl;
	ss << config.leak << std::endl;
	ss << config.reset << std::endl;
	ss << config.threshold << std::endl;
	ss << config.multicompartment << std::endl;
	ss << config.constant_current << std::endl;
	ss << config.membrane_capacitance << std::endl;
	ss << config.adaptation << std::endl;
	ss << config.exponential << std::endl;
	ss << config.event_routing;
	ss << config.readout;
	ss << config.refractory_period;
	ss << config.bayesian;
	os << ss.str();
	return os;
}

} // namespace lola::vx::v2

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::SynapticInput)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Leak)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Reset)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Threshold)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Multicompartment)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::ConstantCurrent)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::MembraneCapacitance)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Adaptation)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Exponential)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Readout)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::EventRouting)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::RefractoryPeriod)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron::Bayesian)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::AtomicNeuron)
