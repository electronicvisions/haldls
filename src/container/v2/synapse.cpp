#include "haldls/container/v2/synapse.h"

namespace haldls {
namespace container {
namespace v2 {

CommonSynramConfig::CommonSynramConfig() : m_pc_conf(0), m_w_conf(0), m_wait_ctr_clear(0) {}

CommonSynramConfig::PCConf CommonSynramConfig::get_pc_conf() const
{
	return m_pc_conf;
}

void CommonSynramConfig::set_pc_conf(CommonSynramConfig::PCConf const& value)
{
	m_pc_conf = value;
}

CommonSynramConfig::WConf CommonSynramConfig::get_w_conf() const
{
	return m_w_conf;
}

void CommonSynramConfig::set_w_conf(CommonSynramConfig::WConf const& value)
{
	m_w_conf = value;
}

CommonSynramConfig::WaitCtrClear CommonSynramConfig::get_wait_ctr_clear() const
{
	return m_wait_ctr_clear;
}

void CommonSynramConfig::set_wait_ctr_clear(CommonSynramConfig::WaitCtrClear const& value)
{
	m_wait_ctr_clear = value;
}

bool CommonSynramConfig::operator==(CommonSynramConfig const& other) const
{
	return m_pc_conf == other.get_pc_conf() && m_w_conf == other.get_w_conf() &&
		   m_wait_ctr_clear == other.get_wait_ctr_clear();
}

bool CommonSynramConfig::operator!=(CommonSynramConfig const& other) const
{
	return !(*this == other);
}


SynapseBlock::Synapse::Synapse() : m_weight(0), m_address(0), m_time_calib(0), m_amp_calib(0) {}

SynapseBlock::Synapse::Weight SynapseBlock::Synapse::get_weight() const
{
	return m_weight;
}

void SynapseBlock::Synapse::set_weight(SynapseBlock::Synapse::Weight const& value)
{
	m_weight = value;
}

SynapseBlock::Synapse::Address SynapseBlock::Synapse::get_address() const
{
	return m_address;
}

void SynapseBlock::Synapse::set_address(SynapseBlock::Synapse::Address const& value)
{
	m_address = value;
}

SynapseBlock::Synapse::TimeCalib SynapseBlock::Synapse::get_time_calib() const
{
	return m_time_calib;
}

void SynapseBlock::Synapse::set_time_calib(SynapseBlock::Synapse::TimeCalib const& value)
{
	m_time_calib = value;
}

SynapseBlock::Synapse::AmpCalib SynapseBlock::Synapse::get_amp_calib() const
{
	return m_amp_calib;
}

void SynapseBlock::Synapse::set_amp_calib(SynapseBlock::Synapse::AmpCalib const& value)
{
	m_amp_calib = value;
}

bool SynapseBlock::Synapse::operator==(SynapseBlock::Synapse const& other) const
{
	return m_weight == other.get_weight() && m_address == other.get_address() &&
		   m_time_calib == other.get_time_calib() && m_amp_calib == other.get_amp_calib();
}

bool SynapseBlock::Synapse::operator!=(SynapseBlock::Synapse const& other) const
{
	return !(*this == other);
}

SynapseBlock::SynapseBlock() : m_synapses() {}

SynapseBlock::Synapse SynapseBlock::get_synapse(
	halco::hicann_dls::v2::SynapseOnSynapseBlock const& synapse) const
{
	return m_synapses.at(synapse);
}

void SynapseBlock::set_synapse(
	halco::hicann_dls::v2::SynapseOnSynapseBlock const& synapse, SynapseBlock::Synapse const& value)
{
	m_synapses.at(synapse) = value;
}

bool SynapseBlock::operator==(SynapseBlock const& other) const
{
	return m_synapses == other.m_synapses;
}

bool SynapseBlock::operator!=(SynapseBlock const& other) const
{
	return !(*this == other);
}

ColumnCorrelationBlock::ColumnCorrelationSwitch::ColumnCorrelationSwitch()
	: m_causal(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::disabled),
	  m_acausal(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::disabled)
{}

ColumnCorrelationBlock::ColumnCorrelationSwitch::Config
ColumnCorrelationBlock::ColumnCorrelationSwitch::get_causal_config() const
{
	return m_causal;
}

void ColumnCorrelationBlock::ColumnCorrelationSwitch::set_causal_config(
	ColumnCorrelationBlock::ColumnCorrelationSwitch::Config const value)
{
	m_causal = value;
}

ColumnCorrelationBlock::ColumnCorrelationSwitch::Config
ColumnCorrelationBlock::ColumnCorrelationSwitch::get_acausal_config() const
{
	return m_acausal;
}

void ColumnCorrelationBlock::ColumnCorrelationSwitch::set_acausal_config(
	ColumnCorrelationBlock::ColumnCorrelationSwitch::Config const value)
{
	m_acausal = value;
}

bool ColumnCorrelationBlock::ColumnCorrelationSwitch::operator==(
	ColumnCorrelationBlock::ColumnCorrelationSwitch const& other) const
{
	return m_causal == other.get_causal_config() && m_acausal == other.get_acausal_config();
}

bool ColumnCorrelationBlock::ColumnCorrelationSwitch::operator!=(
	ColumnCorrelationBlock::ColumnCorrelationSwitch const& other) const
{
	return !(*this == other);
}

ColumnCorrelationBlock::ColumnCorrelationBlock() : m_switches() {}

ColumnCorrelationBlock::ColumnCorrelationSwitch ColumnCorrelationBlock::get_switch(
	halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnBlock const& correlation_switch) const
{
	return m_switches.at(correlation_switch);
}

void ColumnCorrelationBlock::set_switch(
	halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnBlock const& correlation_switch,
	ColumnCorrelationBlock::ColumnCorrelationSwitch const& value)
{
	m_switches.at(correlation_switch) = value;
}

bool ColumnCorrelationBlock::operator==(ColumnCorrelationBlock const& other) const
{
	return m_switches == other.m_switches;
}

bool ColumnCorrelationBlock::operator!=(ColumnCorrelationBlock const& other) const
{
	return !(*this == other);
}

ColumnCurrentBlock::ColumnCurrentSwitch::ColumnCurrentSwitch()
	: m_exc(ColumnCurrentBlock::ColumnCurrentSwitch::Config::disabled),
	  m_inh(ColumnCurrentBlock::ColumnCurrentSwitch::Config::disabled)
{}

ColumnCurrentBlock::ColumnCurrentSwitch::Config
ColumnCurrentBlock::ColumnCurrentSwitch::get_exc_config() const
{
	return m_exc;
}

void ColumnCurrentBlock::ColumnCurrentSwitch::set_exc_config(
	ColumnCurrentBlock::ColumnCurrentSwitch::Config const value)
{
	m_exc = value;
}

ColumnCurrentBlock::ColumnCurrentSwitch::Config
ColumnCurrentBlock::ColumnCurrentSwitch::get_inh_config() const
{
	return m_inh;
}

void ColumnCurrentBlock::ColumnCurrentSwitch::set_inh_config(
	ColumnCurrentBlock::ColumnCurrentSwitch::Config const value)
{
	m_inh = value;
}

bool ColumnCurrentBlock::ColumnCurrentSwitch::operator==(
	ColumnCurrentBlock::ColumnCurrentSwitch const& other) const
{
	return m_exc == other.get_exc_config() && m_inh == other.get_inh_config();
}

bool ColumnCurrentBlock::ColumnCurrentSwitch::operator!=(
	ColumnCurrentBlock::ColumnCurrentSwitch const& other) const
{
	return !(*this == other);
}

ColumnCurrentBlock::ColumnCurrentBlock() : m_switches() {}

ColumnCurrentBlock::ColumnCurrentSwitch ColumnCurrentBlock::get_switch(
	halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnBlock const& current_switch) const
{
	return m_switches.at(current_switch);
}

void ColumnCurrentBlock::set_switch(
	halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnBlock const& current_switch,
	ColumnCurrentBlock::ColumnCurrentSwitch const& value)
{
	m_switches.at(current_switch) = value;
}

bool ColumnCurrentBlock::operator==(ColumnCurrentBlock const& other) const
{
	return m_switches == other.m_switches;
}

bool ColumnCurrentBlock::operator!=(ColumnCurrentBlock const& other) const
{
	return !(*this == other);
}

SynapseDrivers::SynapseDrivers() : m_pulse_length(0), m_states() {}

auto SynapseDrivers::get_states() const -> states_type
{
	return m_states;
}

void SynapseDrivers::set_states(states_type const& values)
{
	m_states = values;
}

SynapseDrivers::State SynapseDrivers::get_state(
	halco::hicann_dls::v2::SynapseDriverOnDLS const& syndriver) const
{
	return m_states.at(syndriver.value());
}

void SynapseDrivers::set_state(
	halco::hicann_dls::v2::SynapseDriverOnDLS const& syndriver, SynapseDrivers::State value)
{
	m_states.at(syndriver.value()) = value;
}

SynapseDrivers::PulseLength SynapseDrivers::get_pulse_length() const
{
	return m_pulse_length;
}

void SynapseDrivers::set_pulse_length(SynapseDrivers::PulseLength const& value)
{
	m_pulse_length = value;
}

bool SynapseDrivers::operator==(SynapseDrivers const& other) const
{
	return m_pulse_length == other.get_pulse_length() && m_states == other.get_states();
}

bool SynapseDrivers::operator!=(SynapseDrivers const& other) const
{
	return !(*this == other);
}

} // namespace v2
} // namespace container
} // namespace haldls
