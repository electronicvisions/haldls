#include "haldls/container/v2/neuron.h"

namespace haldls {
namespace container {
namespace v2 {

CommonNeuronConfig::CommonNeuronConfig()
	: m_digital_out(false),
	  m_post_correlation_signal_length(0),
	  m_external_correlation_signal(false),
	  m_inhibit_spike_comparator(false)
{}

bool CommonNeuronConfig::get_enable_digital_out() const
{
	return m_digital_out;
}

void CommonNeuronConfig::set_enable_digital_out(bool const value)
{
	m_digital_out = value;
}

CommonNeuronConfig::PostCorrelationSignalLength
CommonNeuronConfig::get_post_correlation_signal_length() const
{
	return m_post_correlation_signal_length;
}

void CommonNeuronConfig::set_post_correlation_signal_length(
	CommonNeuronConfig::PostCorrelationSignalLength const& value)
{
	m_post_correlation_signal_length = value;
}

bool CommonNeuronConfig::get_enable_external_post_correlation_signal() const
{
	return m_external_correlation_signal;
}

void CommonNeuronConfig::set_enable_external_post_correlation_signal(bool const value)
{
	m_external_correlation_signal = value;
}

bool CommonNeuronConfig::get_inhibit_spike_comparator() const
{
	return m_inhibit_spike_comparator;
}

void CommonNeuronConfig::set_inhibit_spike_comparator(bool const value)
{
	m_inhibit_spike_comparator = value;
}

bool CommonNeuronConfig::operator==(CommonNeuronConfig const& b) const
{
	return m_digital_out == b.get_enable_digital_out() &&
		   m_post_correlation_signal_length == b.get_post_correlation_signal_length() &&
		   m_external_correlation_signal == b.get_enable_external_post_correlation_signal() &&
		   m_inhibit_spike_comparator == b.get_inhibit_spike_comparator();
}

bool CommonNeuronConfig::operator!=(CommonNeuronConfig const& b) const
{
	return !(*this == b);
}

NeuronDigitalConfig::NeuronDigitalConfig()
	: m_synapse_input_exc(false),
	  m_synapse_input_inh(false),
	  m_leak_high_conductance(false),
	  m_leak(true),
	  m_bigcap(true),
	  m_smallcap(true),
	  m_fire_out_mode(FireOutMode::disabled),
	  m_mux_readout_mode(MuxReadoutMode::v_mem),
	  m_external_current_input(false),
	  m_external_voltage_output(false)
{}

bool NeuronDigitalConfig::get_enable_synapse_input_excitatory() const
{
	return m_synapse_input_exc;
}

void NeuronDigitalConfig::set_enable_synapse_input_excitatory(bool const value)
{
	m_synapse_input_exc = value;
}

bool NeuronDigitalConfig::get_enable_synapse_input_inhibitory() const
{
	return m_synapse_input_inh;
}

void NeuronDigitalConfig::set_enable_synapse_input_inhibitory(bool const value)
{
	m_synapse_input_inh = value;
}

bool NeuronDigitalConfig::get_enable_high_conductance_leak() const
{
	return m_leak_high_conductance;
}

void NeuronDigitalConfig::set_enable_high_conductance_leak(bool const value)
{
	m_leak_high_conductance = value;
}

bool NeuronDigitalConfig::get_enable_leak() const
{
	return m_leak;
}

void NeuronDigitalConfig::set_enable_leak(bool const value)
{
	m_leak = value;
}

bool NeuronDigitalConfig::get_enable_bigcap() const
{
	return m_bigcap;
}

void NeuronDigitalConfig::set_enable_bigcap(bool const value)
{
	m_bigcap = value;
}

bool NeuronDigitalConfig::get_enable_smallcap() const
{
	return m_smallcap;
}

void NeuronDigitalConfig::set_enable_smallcap(bool const value)
{
	m_smallcap = value;
}

NeuronDigitalConfig::FireOutMode NeuronDigitalConfig::get_fire_out_mode() const
{
	return m_fire_out_mode;
}

void NeuronDigitalConfig::set_fire_out_mode(NeuronDigitalConfig::FireOutMode const value)
{
	m_fire_out_mode = value;
}

NeuronDigitalConfig::MuxReadoutMode NeuronDigitalConfig::get_mux_readout_mode() const
{
	return m_mux_readout_mode;
}

void NeuronDigitalConfig::set_mux_readout_mode(
	NeuronDigitalConfig::MuxReadoutMode const value)
{
	m_mux_readout_mode = value;
}

bool NeuronDigitalConfig::get_enable_external_current_input() const
{
	return m_external_current_input;
}

void NeuronDigitalConfig::set_enable_external_current_input(bool value, const common::Passkey<Chip>& /*passkey*/)
{
	m_external_current_input = value;
}

bool NeuronDigitalConfig::get_enable_external_voltage_output() const
{
	return m_external_voltage_output;
}

void NeuronDigitalConfig::set_enable_external_voltage_output(bool value, const common::Passkey<Chip>& /*passkey*/)
{
	m_external_voltage_output = value;
}

bool NeuronDigitalConfig::operator==(NeuronDigitalConfig const& other) const
{
	// clang-format off
	return (
		m_synapse_input_exc == other.get_enable_synapse_input_excitatory() &&
		m_synapse_input_inh == other.get_enable_synapse_input_inhibitory() &&
		m_leak_high_conductance == other.get_enable_high_conductance_leak() &&
		m_leak == other.get_enable_leak() &&
		m_bigcap == other.get_enable_bigcap() &&
		m_smallcap == other.get_enable_smallcap() &&
		m_fire_out_mode == other.get_fire_out_mode() &&
		m_mux_readout_mode == other.get_mux_readout_mode() &&
		m_external_current_input == other.get_enable_external_current_input() &&
		m_external_voltage_output == other.get_enable_external_voltage_output());
	// clang-format on
}

bool NeuronDigitalConfig::operator!=(NeuronDigitalConfig const& other) const
{
	return !(*this == other);
}

} // namespace v2
} // namespace container
} // namespace haldls
