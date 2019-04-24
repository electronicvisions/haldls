#include "haldls/v2/chip.h"

#include <utility>

#include "halco/common/iter_all.h"

namespace haldls {
namespace v2 {

Chip::Chip()
    : m_neuron_digital_configs(),
      m_synapse_blocks(),
      m_correlation_blocks(),
      m_current_blocks(),
      m_causal_correlation_blocks(),
      m_acausal_correlation_blocks(),
      m_capmem(),
      m_ppu_memory(),
      m_ppu_control_register(),
      m_ppu_status_register(),
      m_rate_counter(),
      m_rate_counter_config(),
      m_synapse_drivers(),
      m_synram_config(),
      m_capmem_config(),
      m_neuron_config(),
      m_correlation_config()
{}

void Chip::enable_buffered_readout(halco::hicann_dls::v2::NeuronOnDLS const& neuron)
{
	disable_buffered_readout();
	m_neuron_digital_configs.at(neuron).set_enable_buffered_readout(true, {});
}

void Chip::disable_buffered_readout()
{
	for (auto neuron : halco::common::iter_all<halco::hicann_dls::v2::NeuronOnDLS>()) {
		m_neuron_digital_configs.at(neuron).set_enable_buffered_readout(false, {});
	}
}

hate::optional<halco::hicann_dls::v2::NeuronOnDLS>
Chip::get_buffered_readout_neuron() const
{
	for (auto neuron : halco::common::iter_all<halco::hicann_dls::v2::NeuronOnDLS>()) {
		if (m_neuron_digital_configs.at(neuron).get_enable_buffered_readout()) {
			return neuron;
		}
	}
	return {};
}

NeuronDigitalConfig Chip::get_neuron_digital_config(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron) const
{
	NeuronDigitalConfig config = m_neuron_digital_configs.at(neuron);
	config.set_enable_buffered_readout(false, {});
	return config;
}

void Chip::set_neuron_digital_config(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron, NeuronDigitalConfig value)
{
	NeuronDigitalConfig& config = m_neuron_digital_configs.at(neuron);
	value.set_enable_buffered_readout(config.get_enable_buffered_readout(), {});
	config = value;
}

SynapseBlock Chip::get_synapse_block(
	halco::hicann_dls::v2::SynapseBlockOnDLS const& synapse_block) const
{
	return m_synapse_blocks.at(synapse_block);
}

void Chip::set_synapse_block(
	halco::hicann_dls::v2::SynapseBlockOnDLS const& synapse_block, SynapseBlock const& value)
{
	m_synapse_blocks.at(synapse_block) = value;
}

SynapseBlock::Synapse Chip::get_synapse(halco::hicann_dls::v2::SynapseOnDLS const& synapse) const
{
	return m_synapse_blocks.at(synapse.toSynapseBlockOnDLS())
		.get_synapse(synapse.toSynapseOnSynapseBlock());
}

void Chip::set_synapse(
	halco::hicann_dls::v2::SynapseOnDLS const& synapse, SynapseBlock::Synapse const& value)
{
	return m_synapse_blocks.at(synapse.toSynapseBlockOnDLS())
		.set_synapse(synapse.toSynapseOnSynapseBlock(), value);
}

ColumnCorrelationBlock Chip::get_column_correlation_block(
    halco::hicann_dls::v2::ColumnCorrelationBlockOnDLS const& column_block) const
{
	return m_correlation_blocks.at(column_block);
}

void Chip::set_column_correlation_block(
    halco::hicann_dls::v2::ColumnCorrelationBlockOnDLS const& column_block,
    ColumnCorrelationBlock const& value)
{
	m_correlation_blocks.at(column_block) = value;
}

ColumnCorrelationBlock::ColumnCorrelationSwitch Chip::get_column_correlation_switch(
	halco::hicann_dls::v2::ColumnCorrelationSwitchOnDLS const& correlation_switch) const
{
	auto block = correlation_switch.toColumnCorrelationBlockOnDLS();
	auto switch_on_block = correlation_switch.toColumnCorrelationSwitchOnColumnCorrelationBlock();
	return m_correlation_blocks.at(block).get_switch(switch_on_block);
}

void Chip::set_column_correlation_switch(
	halco::hicann_dls::v2::ColumnCorrelationSwitchOnDLS const& correlation_switch,
	ColumnCorrelationBlock::ColumnCorrelationSwitch const& value)
{
	auto block = correlation_switch.toColumnCorrelationBlockOnDLS();
	auto switch_on_block = correlation_switch.toColumnCorrelationSwitchOnColumnCorrelationBlock();
	m_correlation_blocks.at(block).set_switch(switch_on_block, value);
}

ColumnCurrentBlock Chip::get_column_current_block(
    halco::hicann_dls::v2::ColumnCurrentBlockOnDLS const& column_block) const
{
	return m_current_blocks.at(column_block);
}

void Chip::set_column_current_block(
    halco::hicann_dls::v2::ColumnCurrentBlockOnDLS const& column_block,
    ColumnCurrentBlock const& value)
{
	m_current_blocks.at(column_block) = value;
}

ColumnCurrentBlock::ColumnCurrentSwitch Chip::get_column_current_switch(
	halco::hicann_dls::v2::ColumnCurrentSwitchOnDLS const& current_switch) const
{
	auto block = current_switch.toColumnCurrentBlockOnDLS();
	auto switch_on_block = current_switch.toColumnCurrentSwitchOnColumnCurrentBlock();
	return m_current_blocks.at(block).get_switch(switch_on_block);
}

void Chip::set_column_current_switch(
	halco::hicann_dls::v2::ColumnCurrentSwitchOnDLS const& current_switch,
	ColumnCurrentBlock::ColumnCurrentSwitch const& value)
{
	auto block = current_switch.toColumnCurrentBlockOnDLS();
	auto switch_on_block = current_switch.toColumnCurrentSwitchOnColumnCurrentBlock();
	m_current_blocks.at(block).set_switch(switch_on_block, value);
}

CausalCorrelationBlock Chip::get_causal_correlation_block(
    halco::hicann_dls::v2::CausalCorrelationBlockOnDLS const& block) const
{
	return m_causal_correlation_blocks.at(block);
}

CausalCorrelationBlock::Correlation Chip::get_causal_correlation(
    halco::hicann_dls::v2::CausalCorrelationOnDLS const& correlation) const
{
	return m_causal_correlation_blocks.at(correlation.toCausalCorrelationBlockOnDLS())
	    .get_correlation(correlation.toCausalCorrelationOnCausalCorrelationBlock());
}

AcausalCorrelationBlock Chip::get_acausal_correlation_block(
    halco::hicann_dls::v2::AcausalCorrelationBlockOnDLS const& block) const
{
	return m_acausal_correlation_blocks.at(block);
}

AcausalCorrelationBlock::Correlation Chip::get_acausal_correlation(
    halco::hicann_dls::v2::AcausalCorrelationOnDLS const& correlation) const
{
	return m_acausal_correlation_blocks.at(correlation.toAcausalCorrelationBlockOnDLS())
	    .get_correlation(correlation.toAcausalCorrelationOnAcausalCorrelationBlock());
}

CapMem const& Chip::get_capmem() const
{
	return m_capmem;
}

void Chip::set_capmem(CapMem const& value)
{
	m_capmem = value;
}

PPUMemory const& Chip::get_ppu_memory() const
{
	return m_ppu_memory;
}

void Chip::set_ppu_memory(PPUMemory const& value)
{
	m_ppu_memory = value;
}

PPUControlRegister const& Chip::get_ppu_control_register() const
{
	return m_ppu_control_register;
}

void Chip::set_ppu_control_register(PPUControlRegister const& value)
{
	m_ppu_control_register = value;
}

PPUStatusRegister Chip::get_ppu_status_register() const
{
	return m_ppu_status_register;
}

RateCounter Chip::get_rate_counter() const
{
	return m_rate_counter;
}

RateCounterConfig const& Chip::get_rate_counter_config() const
{
	return m_rate_counter_config;
}

void Chip::set_rate_counter_config(RateCounterConfig const& value)
{
	m_rate_counter_config = value;
}

SynapseDriverBlock const& Chip::get_synapse_drivers() const
{
	return m_synapse_drivers;
}

void Chip::set_synapse_drivers(SynapseDriverBlock const& value)
{
	m_synapse_drivers = value;
}

CommonSynramConfig const& Chip::get_common_synram_config() const
{
	return m_synram_config;
}

void Chip::set_common_synram_config(CommonSynramConfig const& value)
{
	m_synram_config = value;
}

CapMemConfig const& Chip::get_capmem_config() const
{
	return m_capmem_config;
}

void Chip::set_capmem_config(CapMemConfig const& value)
{
	m_capmem_config = value;
}

CommonNeuronConfig const& Chip::get_common_neuron_config() const
{
	return m_neuron_config;
}

void Chip::set_common_neuron_config(CommonNeuronConfig const& value)
{
	m_neuron_config = value;
}

CorrelationConfig const& Chip::get_correlation_config() const
{
	return m_correlation_config;
}

void Chip::set_correlation_config(CorrelationConfig const& value)
{
	m_correlation_config = value;
}

bool Chip::operator==(Chip const& other) const
{
	return (
		m_neuron_digital_configs == other.m_neuron_digital_configs &&
		m_synapse_blocks == other.m_synapse_blocks &&
		m_correlation_blocks == other.m_correlation_blocks &&
		m_current_blocks == other.m_current_blocks &&
		m_capmem == other.m_capmem &&
		m_ppu_memory == other.m_ppu_memory &&
		m_ppu_control_register == other.m_ppu_control_register &&
		m_ppu_status_register == other.m_ppu_status_register &&
		m_rate_counter == other.m_rate_counter &&
		m_rate_counter_config == other.m_rate_counter_config &&
		m_synapse_drivers == other.m_synapse_drivers &&
		m_synram_config == other.m_synram_config &&
		m_capmem_config == other.m_capmem_config &&
		m_neuron_config == other.m_neuron_config &&
		m_correlation_config == other.m_correlation_config);
}

bool Chip::operator!=(Chip const& other) const
{
	return !(*this == other);
}

template <class Archive>
void Chip::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_neuron_digital_configs));
	ar(CEREAL_NVP(m_synapse_blocks));
	ar(CEREAL_NVP(m_causal_correlation_blocks));
	ar(CEREAL_NVP(m_acausal_correlation_blocks));
	ar(CEREAL_NVP(m_correlation_blocks));
	ar(CEREAL_NVP(m_current_blocks));
	ar(CEREAL_NVP(m_capmem));
	ar(CEREAL_NVP(m_ppu_memory));
	ar(CEREAL_NVP(m_ppu_control_register));
	ar(CEREAL_NVP(m_ppu_status_register));
	ar(CEREAL_NVP(m_rate_counter));
	ar(CEREAL_NVP(m_rate_counter_config));
	ar(CEREAL_NVP(m_synapse_drivers));
	ar(CEREAL_NVP(m_synram_config));
	ar(CEREAL_NVP(m_capmem_config));
	ar(CEREAL_NVP(m_neuron_config));
	ar(CEREAL_NVP(m_correlation_config));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(Chip)

} // namespace v2
} // namespace haldls
