#include "lola/vx/v4/neuron.h"

#include "haldls/vx/container.tcc"
#include "hate/indent.h"
#include "lola/vx/hana.h"

namespace lola::vx::v4 {

AtomicNeuron::SynapticInput::SynapticInput() :
    enable(false),
    i_bias_tau(),
    i_bias_coba(),
    i_shift_reference(),
    i_bias_gm(),
    enable_small_capacitance(true),
    enable_high_resistance(false),
    enable_coba_mode(false)
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
	ss << "SynapticInput(\n"
	   << "\tenable:                   " << std::boolalpha << config.enable << "\n"
	   << "\ti_bias_tau:               " << config.i_bias_tau << "\n"
	   << "\ti_shift_reference:        " << config.i_shift_reference << "\n"
	   << "\ti_bias_gm:                " << config.i_bias_gm << "\n"
	   << "\ti_bias_coba:              " << config.i_bias_coba << "\n"
	   << "\tenable_small_capacitance: " << config.enable_small_capacitance << "\n"
	   << "\tenable_high_resistance:   " << config.enable_high_resistance << "\n"
	   << "\tenable_coba_mode:         " << config.enable_coba_mode << "\n)";
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
	ss << "Leak(\n"
	   << "\tv_leak:                " << config.v_leak << "\n"
	   << "\ti_bias:                " << config.i_bias << "\n"
	   << "\tenable_degeneration:   " << std::boolalpha << config.enable_degeneration << "\n"
	   << "\tenable_division:       " << config.enable_division << "\n"
	   << "\tenable_multiplication: " << config.enable_multiplication << "\n)";
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
	ss << "Reset(\n"
	   << "\tv_reset:               " << config.v_reset << "\n"
	   << "\ti_bias:                " << config.i_bias << "\n"
	   << "\tenable_degeneration:   " << std::boolalpha << config.enable_degeneration << "\n"
	   << "\tenable_division:       " << config.enable_division << "\n"
	   << "\tenable_multiplication: " << config.enable_multiplication << "\n)";
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
	ss << "Threshold(\n"
	   << "\tenable:      " << std::boolalpha << config.enable << "\n"
	   << "\tv_threshold: " << config.v_threshold << "\n)";
	os << ss.str();
	return os;
}


AtomicNeuron::ConstantCurrent::ConstantCurrent() :
    enable(false), i_offset(), type(lola::vx::v4::AtomicNeuron::ConstantCurrent::Type::source)
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
	ss << "ConstantCurrent(\n"
	   << "\tenable:   " << std::boolalpha << config.enable << "\n"
	   << "\ti_offset: " << config.i_offset << "\n"
	   << "\ttype:     "
	   << (config.type == lola::vx::v4::AtomicNeuron::ConstantCurrent::Type::sink ? "sink"
	                                                                              : "source")
	   << "\n)";
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
	ss << "Multicompartment(\n"
	   << std::boolalpha << "\tconnect_soma:                      " << config.connect_soma << "\n"
	   << "\tconnect_soma_right:                " << config.connect_soma_right << "\n"
	   << "\tconnect_right:                     " << config.connect_right << "\n"
	   << "\tconnect_vertical:                  " << config.connect_vertical << "\n"
	   << "\tenable_conductance:                " << config.enable_conductance << "\n"
	   << "\tenable_conductance_division:       " << config.enable_conductance_division << "\n"
	   << "\tenable_conductance_multiplication: " << config.enable_conductance_multiplication
	   << "\n"
	   << "\ti_bias_nmda:                       " << config.i_bias_nmda << "\n)";
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
	ss << "MembraneCapacitance(\n"
	   << std::boolalpha << "\tcapacitance: " << config.capacitance << "\n)";
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
	ss << "Adaptation(\n"
	   << "\tenable:       " << std::boolalpha << config.enable << "\n"
	   << "\tenable_pulse: " << config.enable_pulse << "\n"
	   << "\tinvert_a:     " << config.invert_a << "\n"
	   << "\tinvert_b:     " << config.invert_b << "\n"
	   << "\tv_ref:        " << config.v_ref << "\n"
	   << "\ti_bias_tau:   " << config.i_bias_tau << "\n"
	   << "\ti_bias_a:     " << config.i_bias_a << "\n"
	   << "\ti_bias_b:     " << config.i_bias_b << "\n"
	   << "\tv_leak:       " << config.v_leak << "\n)";
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
	ss << "Exponential(\n"
	   << "\tenable: " << std::boolalpha << config.enable << "\n"
	   << "\tv_exp:  " << config.v_exp << "\n"
	   << "\ti_bias: " << config.i_bias << "\n)";
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
	ss << "Readout(\n"
	   << "\tenable_buffered_access:   " << std::boolalpha << config.enable_buffered_access << "\n"
	   << "\tenable_amplifier:         " << config.enable_amplifier << "\n"
	   << "\tenable_unbuffered_access: " << config.enable_unbuffered_access << "\n"
	   << "\tsource:                   " << config.source;
	ss << "\n)";
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
	ss << "EventRouting(\n"
	   << "\tanalog_output:            "
	   << (config.analog_output == AtomicNeuron::EventRouting::AnalogOutputMode::off
	           ? "off"
	           : (config.analog_output == AtomicNeuron::EventRouting::AnalogOutputMode::normal
	                  ? "normal"
	                  : "strong"))
	   << std::boolalpha << "\n"
	   << "\tenable_digital:           " << config.enable_digital << "\n"
	   << "\taddress:                  " << config.address << "\n"
	   << "\tenable_bypass_excitatory: " << config.enable_bypass_excitatory << "\n"
	   << "\tenable_bypass_inhibitory: " << config.enable_bypass_inhibitory << "\n"
	   << "\tenable_post_overwrite:    " << config.enable_post_overwrite << "\n)";
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
	ss << "RefractoryPeriod(\n"
	   << "\tinput_clock:     " << config.input_clock << "\n"
	   << "\tenable_pause:    " << config.enable_pause << "\n"
	   << "\trefractory_time: " << config.refractory_time << "\n"
	   << "\treset_holdoff:   " << config.reset_holdoff << "\n)";
	os << ss.str();
	return os;
}


AtomicNeuron::Bayesian::Bayesian() :
    to_post_pulse(true),
    operation(AtomicNeuron::Bayesian::Operation::local),
    connect_fire_vertical(false),
    connect_fire_to_right(false),
    connect_fire_from_right(false),
    enable_master(false),
    enable_slave(false)
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

	std::stringstream operation;
	if (config.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::local)
		operation << "local";
	else if (config.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::AND)
		operation << "AND";
	else if (config.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::XNOR)
		operation << "XNOR";
	else if (config.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::MULLER_C)
		operation << "MULLER_C";

	std::stringstream ss;
	ss << "Bayesian(\n"
	   << std::boolalpha << "\tto_post_pulse:           " << config.to_post_pulse << "\n"
	   << "\tconnect_fire_vertical:   " << config.connect_fire_vertical << "\n"
	   << "\tconnect_fire_to_right:   " << config.connect_fire_to_right << "\n"
	   << "\tconnect_fire_from_right: " << config.connect_fire_from_right << "\n"
	   << "\tenable_master:           " << config.enable_master << "\n"
	   << "\tenable_slave:            " << config.enable_slave << "\n"
	   << "\toperation:               " << operation.str() << "\n)";

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
	os << "AtomicNeuron(\n";
	hate::IndentingOstream ios(os);
	ios << hate::Indentation("\t");
	ios << "excitatory_input: " << config.excitatory_input << "\n";
	ios << "inhibitory_input: " << config.inhibitory_input << "\n";
	ios << config.leak << "\n";
	ios << config.reset << "\n";
	ios << config.threshold << "\n";
	ios << config.multicompartment << "\n";
	ios << config.constant_current << "\n";
	ios << config.membrane_capacitance << "\n";
	ios << config.adaptation << "\n";
	ios << config.exponential << "\n";
	ios << config.event_routing << "\n";
	ios << config.readout << "\n";
	ios << config.refractory_period << "\n";
	ios << config.bayesian;
	os << "\n)";
	return os;
}

AtomicNeuron::operator haldls::vx::v4::NeuronConfig() const
{
	haldls::vx::v4::NeuronConfig neuron_config;

	neuron_config.set_enable_synaptic_input_excitatory(excitatory_input.enable);
	neuron_config.set_enable_synaptic_input_inhibitory(inhibitory_input.enable);

	neuron_config.set_enable_synaptic_input_excitatory_small_capacitance(
	    excitatory_input.enable_small_capacitance);
	neuron_config.set_enable_synaptic_input_inhibitory_small_capacitance(
	    inhibitory_input.enable_small_capacitance);

	neuron_config.set_enable_synaptic_input_excitatory_high_resistance(
	    excitatory_input.enable_high_resistance);
	neuron_config.set_enable_synaptic_input_inhibitory_high_resistance(
	    inhibitory_input.enable_high_resistance);

	neuron_config.set_enable_synaptic_input_excitatory_coba_mode(excitatory_input.enable_coba_mode);
	neuron_config.set_enable_synaptic_input_inhibitory_coba_mode(inhibitory_input.enable_coba_mode);

	neuron_config.set_enable_leak_degeneration(leak.enable_degeneration);
	neuron_config.set_enable_leak_division(leak.enable_division);
	neuron_config.set_enable_leak_multiplication(leak.enable_multiplication);

	neuron_config.set_enable_reset_degeneration(reset.enable_degeneration);
	neuron_config.set_enable_reset_division(reset.enable_division);
	neuron_config.set_enable_reset_multiplication(reset.enable_multiplication);

	neuron_config.set_enable_threshold_comparator(threshold.enable);

	neuron_config.set_enable_membrane_offset(constant_current.enable);
	neuron_config.set_invert_membrane_offset(
	    constant_current.type == lola::vx::v4::AtomicNeuron::ConstantCurrent::Type::sink);

	neuron_config.set_connect_soma(multicompartment.connect_soma);
	neuron_config.set_connect_soma_right(multicompartment.connect_soma_right);
	neuron_config.set_connect_membrane_right(multicompartment.connect_right);
	neuron_config.set_connect_bottom(multicompartment.connect_vertical);
	neuron_config.set_enable_multicomp_conductance(multicompartment.enable_conductance);
	neuron_config.set_enable_divide_multicomp_conductance_bias(
	    multicompartment.enable_conductance_division);
	neuron_config.set_enable_multiply_multicomp_conductance_bias(
	    multicompartment.enable_conductance_multiplication);

	neuron_config.set_membrane_capacitor_size(membrane_capacitance.capacitance);

	neuron_config.set_enable_adaptation(adaptation.enable);
	neuron_config.set_invert_adaptation_a(adaptation.invert_a);
	neuron_config.set_invert_adaptation_b(adaptation.invert_b);

	neuron_config.set_enable_exponential(exponential.enable);

	if (event_routing.analog_output !=
	    lola::vx::v4::AtomicNeuron::EventRouting::AnalogOutputMode::off) {
		neuron_config.set_enable_fire(true);
		if (event_routing.analog_output ==
		    lola::vx::v4::AtomicNeuron::EventRouting::AnalogOutputMode::strong) {
			neuron_config.set_enable_strong_fire(true);
		}
	}
	neuron_config.set_enable_bypass_excitatory(event_routing.enable_bypass_excitatory);
	neuron_config.set_enable_bypass_inhibitory(event_routing.enable_bypass_inhibitory);

	neuron_config.set_enable_unbuffered_access(readout.enable_unbuffered_access);
	neuron_config.set_enable_readout_amplifier(readout.enable_amplifier);
	neuron_config.set_readout_source(readout.source);
	neuron_config.set_enable_readout(readout.enable_buffered_access);

	neuron_config.set_enable_pause(refractory_period.enable_pause);

	return neuron_config;
}

void AtomicNeuron::set_from(haldls::vx::v4::NeuronConfig const& neuron_config)
{
	excitatory_input.enable = neuron_config.get_enable_synaptic_input_excitatory();
	inhibitory_input.enable = neuron_config.get_enable_synaptic_input_inhibitory();

	excitatory_input.enable_small_capacitance =
	    neuron_config.get_enable_synaptic_input_excitatory_small_capacitance();
	inhibitory_input.enable_small_capacitance =
	    neuron_config.get_enable_synaptic_input_inhibitory_small_capacitance();

	excitatory_input.enable_high_resistance =
	    neuron_config.get_enable_synaptic_input_excitatory_high_resistance();
	inhibitory_input.enable_high_resistance =
	    neuron_config.get_enable_synaptic_input_inhibitory_high_resistance();

	excitatory_input.enable_coba_mode =
	    neuron_config.get_enable_synaptic_input_excitatory_coba_mode();
	inhibitory_input.enable_coba_mode =
	    neuron_config.get_enable_synaptic_input_inhibitory_coba_mode();

	leak.enable_degeneration = neuron_config.get_enable_leak_degeneration();
	leak.enable_division = neuron_config.get_enable_leak_division();
	leak.enable_multiplication = neuron_config.get_enable_leak_multiplication();

	reset.enable_degeneration = neuron_config.get_enable_reset_degeneration();
	reset.enable_division = neuron_config.get_enable_reset_division();
	reset.enable_multiplication = neuron_config.get_enable_reset_multiplication();

	threshold.enable = neuron_config.get_enable_threshold_comparator();

	constant_current.enable = neuron_config.get_enable_membrane_offset();
	constant_current.type = neuron_config.get_invert_membrane_offset()
	                            ? lola::vx::v4::AtomicNeuron::ConstantCurrent::Type::sink
	                            : lola::vx::v4::AtomicNeuron::ConstantCurrent::Type::source;

	multicompartment.connect_soma = neuron_config.get_connect_soma();
	multicompartment.connect_soma_right = neuron_config.get_connect_soma_right();
	multicompartment.connect_right = neuron_config.get_connect_membrane_right();
	multicompartment.connect_vertical = neuron_config.get_connect_bottom();
	multicompartment.enable_conductance = neuron_config.get_enable_multicomp_conductance();
	multicompartment.enable_conductance_division =
	    neuron_config.get_enable_divide_multicomp_conductance_bias();
	multicompartment.enable_conductance_multiplication =
	    neuron_config.get_enable_multiply_multicomp_conductance_bias();

	membrane_capacitance.capacitance = neuron_config.get_membrane_capacitor_size();

	adaptation.enable = neuron_config.get_enable_adaptation();
	adaptation.invert_a = neuron_config.get_invert_adaptation_a();
	adaptation.invert_b = neuron_config.get_invert_adaptation_b();
	refractory_period.enable_pause = neuron_config.get_enable_pause();

	exponential.enable = neuron_config.get_enable_exponential();

	readout.enable_unbuffered_access = neuron_config.get_enable_unbuffered_access();
	readout.enable_amplifier = neuron_config.get_enable_readout_amplifier();
	readout.source = neuron_config.get_readout_source();
	readout.enable_buffered_access = neuron_config.get_enable_readout();

	if (neuron_config.get_enable_fire()) {
		if (neuron_config.get_enable_strong_fire()) {
			event_routing.analog_output =
			    lola::vx::v4::AtomicNeuron::EventRouting::AnalogOutputMode::strong;
		} else {
			event_routing.analog_output =
			    lola::vx::v4::AtomicNeuron::EventRouting::AnalogOutputMode::normal;
		}
	} else {
		event_routing.analog_output =
		    lola::vx::v4::AtomicNeuron::EventRouting::AnalogOutputMode::off;
	}

	event_routing.enable_bypass_excitatory = neuron_config.get_enable_bypass_excitatory();
	event_routing.enable_bypass_inhibitory = neuron_config.get_enable_bypass_inhibitory();
}

AtomicNeuron::operator haldls::vx::v4::NeuronBackendConfig() const
{
	haldls::vx::v4::NeuronBackendConfig neuron_backend_config;

	neuron_backend_config.set_enable_spike_out(event_routing.enable_digital);
	neuron_backend_config.set_enable_bayesian_extension(bayesian.to_post_pulse);

	if (bayesian.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::local) {
		neuron_backend_config.set_enable_bayesian_0(false);
		neuron_backend_config.set_enable_bayesian_1(false);
	} else if (bayesian.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::AND) {
		neuron_backend_config.set_enable_bayesian_0(true);
		neuron_backend_config.set_enable_bayesian_1(false);
	} else if (bayesian.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::XNOR) {
		neuron_backend_config.set_enable_bayesian_0(false);
		neuron_backend_config.set_enable_bayesian_1(true);
	} else if (bayesian.operation == lola::vx::v4::AtomicNeuron::Bayesian::Operation::MULLER_C) {
		neuron_backend_config.set_enable_bayesian_0(true);
		neuron_backend_config.set_enable_bayesian_1(true);
	}

	neuron_backend_config.set_enable_neuron_master(bayesian.enable_master);
	neuron_backend_config.set_enable_neuron_slave(bayesian.enable_slave);
	neuron_backend_config.set_connect_fire_bottom(bayesian.connect_fire_vertical);
	neuron_backend_config.set_connect_fire_to_right(bayesian.connect_fire_to_right);
	neuron_backend_config.set_connect_fire_from_right(bayesian.connect_fire_from_right);
	neuron_backend_config.set_address_out(event_routing.address);
	neuron_backend_config.set_reset_holdoff(refractory_period.reset_holdoff);
	neuron_backend_config.set_refractory_time(refractory_period.refractory_time);
	neuron_backend_config.set_select_input_clock(refractory_period.input_clock);
	neuron_backend_config.set_enable_adaptation_pulse(adaptation.enable_pulse);
	neuron_backend_config.set_post_overwrite(event_routing.enable_post_overwrite);

	return neuron_backend_config;
}

void AtomicNeuron::set_from(haldls::vx::v4::NeuronBackendConfig const& neuron_backend_config)
{
	adaptation.enable_pulse = neuron_backend_config.get_enable_adaptation_pulse();

	refractory_period.reset_holdoff = neuron_backend_config.get_reset_holdoff();
	refractory_period.refractory_time = neuron_backend_config.get_refractory_time();
	refractory_period.input_clock = neuron_backend_config.get_select_input_clock();

	event_routing.enable_digital = neuron_backend_config.get_enable_spike_out();
	event_routing.enable_post_overwrite = neuron_backend_config.get_post_overwrite();
	event_routing.address = neuron_backend_config.get_address_out();

	bayesian.enable_master = neuron_backend_config.get_enable_neuron_master();
	bayesian.enable_slave = neuron_backend_config.get_enable_neuron_slave();
	bayesian.connect_fire_vertical = neuron_backend_config.get_connect_fire_bottom();
	bayesian.connect_fire_to_right = neuron_backend_config.get_connect_fire_to_right();
	bayesian.connect_fire_from_right = neuron_backend_config.get_connect_fire_from_right();
	bayesian.to_post_pulse = neuron_backend_config.get_enable_bayesian_extension();

	if (!neuron_backend_config.get_enable_bayesian_0() &&
	    !neuron_backend_config.get_enable_bayesian_1()) {
		bayesian.operation = lola::vx::v4::AtomicNeuron::Bayesian::Operation::local;
	} else if (
	    neuron_backend_config.get_enable_bayesian_0() &&
	    !neuron_backend_config.get_enable_bayesian_1()) {
		bayesian.operation = lola::vx::v4::AtomicNeuron::Bayesian::Operation::AND;
	} else if (
	    !neuron_backend_config.get_enable_bayesian_0() &&
	    neuron_backend_config.get_enable_bayesian_1()) {
		bayesian.operation = lola::vx::v4::AtomicNeuron::Bayesian::Operation::XNOR;
	} else if (
	    neuron_backend_config.get_enable_bayesian_0() &&
	    neuron_backend_config.get_enable_bayesian_1()) {
		bayesian.operation = lola::vx::v4::AtomicNeuron::Bayesian::Operation::MULLER_C;
	}
}


bool NeuronBlock::operator==(NeuronBlock const& other) const
{
	return equal(*this, other);
}

bool NeuronBlock::operator!=(NeuronBlock const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, NeuronBlock const& config)
{
	using namespace halco::hicann_dls::vx::v4;
	using namespace halco::common;
	os << "NeuronBlock(\n";
	for (auto const an : iter_all<AtomicNeuronOnDLS>()) {
		os << an << ":\n";
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t");
		ios << config.atomic_neurons[an];
		os << "\n";
	}
	for (auto const block : iter_all<CommonNeuronBackendConfigOnDLS>()) {
		os << block << ":\n";
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t");
		ios << config.backends[block];
		os << "\n";
	}
	for (auto const row : iter_all<ColumnCurrentRowOnDLS>()) {
		os << row << ":\n";
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t");
		ios << config.current_rows[row];
		os << "\n";
	}
	for (auto const block : iter_all<CapMemBlockOnDLS>()) {
		os << block << ":\n";
		os << "\tv_bias_casc_n: " << config.v_bias_casc_n[block] << "\n";
		os << "\ti_bias_readout_amp: " << config.i_bias_readout_amp[block] << "\n";
		os << "\ti_bias_leak_source_follower: " << config.i_bias_leak_source_follower[block]
		   << "\n";
		os << "\ti_bias_threshold_comparator: " << config.i_bias_threshold_comparator[block]
		   << "\n";
	}
	os << ")";
	return os;
}

} // namespace lola::vx::v4

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::v4::AtomicNeuron)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::v4::NeuronBlock)
