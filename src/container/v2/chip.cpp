#include "haldls/container/v2/chip.h"

#include <utility>

#include "halco/common/iter_all.h"

namespace haldls {
namespace container {
namespace v2 {

Chip::Chip()
	: m_neuron_digital_configs(),
	  m_synapse_blocks(),
	  m_correlation_blocks(),
	  m_current_blocks(),
	  m_capmem(),
	  m_ppu_memory(),
	  m_ppu_control_register(),
	  m_ppu_status_register(),
	  m_rate_counter(),
	  m_synapse_drivers(),
	  m_synram_config(),
	  m_capmem_config(),
	  m_neuron_config(),
	  m_correlation_config()
{}

void Chip::enable_external_current_input(halco::hicann_dls::v2::NeuronOnDLS const& neuron)
{
	disable_external_current_input();
	m_neuron_digital_configs.at(neuron).set_enable_external_current_input(true, {});
}

void Chip::disable_external_current_input()
{
	for (auto neuron : halco::common::iter_all<halco::hicann_dls::v2::NeuronOnDLS>()) {
		m_neuron_digital_configs.at(neuron).set_enable_external_current_input(false, {});
	}
}

common::optional<halco::hicann_dls::v2::NeuronOnDLS>
Chip::get_external_current_input_neuron() const
{
	for (auto neuron : halco::common::iter_all<halco::hicann_dls::v2::NeuronOnDLS>()) {
		if (m_neuron_digital_configs.at(neuron).get_enable_external_current_input()) {
			return neuron;
		}
	}
	return {};
}

void Chip::enable_external_voltage_output(halco::hicann_dls::v2::NeuronOnDLS const& neuron)
{
	disable_external_voltage_output();
	m_neuron_digital_configs.at(neuron).set_enable_external_voltage_output(true, {});
}

void Chip::disable_external_voltage_output()
{
	for (auto neuron : halco::common::iter_all<halco::hicann_dls::v2::NeuronOnDLS>()) {
		m_neuron_digital_configs.at(neuron).set_enable_external_voltage_output(false, {});
	}
}

common::optional<halco::hicann_dls::v2::NeuronOnDLS>
Chip::get_external_voltage_output_neuron() const
{
	for (auto neuron : halco::common::iter_all<halco::hicann_dls::v2::NeuronOnDLS>()) {
		if (m_neuron_digital_configs.at(neuron).get_enable_external_voltage_output()) {
			return neuron;
		}
	}
	return {};
}

NeuronDigitalConfig Chip::get_neuron_digital_config(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron) const
{
	NeuronDigitalConfig config = m_neuron_digital_configs.at(neuron);
	config.set_enable_external_current_input(false, {});
	config.set_enable_external_voltage_output(false, {});
	return config;
}

void Chip::set_neuron_digital_config(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron, NeuronDigitalConfig value)
{
	NeuronDigitalConfig& config = m_neuron_digital_configs.at(neuron);
	value.set_enable_external_current_input(config.get_enable_external_current_input(), {});
	value.set_enable_external_voltage_output(config.get_enable_external_voltage_output(), {});
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
	halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block) const
{
	return m_correlation_blocks.at(column_block);
}

void Chip::set_column_correlation_block(
	halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block,
	ColumnCorrelationBlock const& value)
{
	m_correlation_blocks.at(column_block) = value;
}

ColumnCorrelationBlock::ColumnCorrelationSwitch Chip::get_column_correlation_switch(
	halco::hicann_dls::v2::ColumnCorrelationSwitchOnDLS const& correlation_switch) const
{
	auto block = correlation_switch.toColumnBlockOnDLS();
	auto switch_on_block = correlation_switch.toColumnCorrelationSwitchOnColumnBlock();
	return m_correlation_blocks.at(block).get_switch(switch_on_block);
}

void Chip::set_column_correlation_switch(
	halco::hicann_dls::v2::ColumnCorrelationSwitchOnDLS const& correlation_switch,
	ColumnCorrelationBlock::ColumnCorrelationSwitch const& value)
{
	auto block = correlation_switch.toColumnBlockOnDLS();
	auto switch_on_block = correlation_switch.toColumnCorrelationSwitchOnColumnBlock();
	m_correlation_blocks.at(block).set_switch(switch_on_block, value);
}

ColumnCurrentBlock Chip::get_column_current_block(
	halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block) const
{
	return m_current_blocks.at(column_block);
}

void Chip::set_column_current_block(
	halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block,
	ColumnCurrentBlock const& value)
{
	m_current_blocks.at(column_block) = value;
}

ColumnCurrentBlock::ColumnCurrentSwitch Chip::get_column_current_switch(
	halco::hicann_dls::v2::ColumnCurrentSwitchOnDLS const& current_switch) const
{
	auto block = current_switch.toColumnBlockOnDLS();
	auto switch_on_block = current_switch.toColumnCurrentSwitchOnColumnBlock();
	return m_current_blocks.at(block).get_switch(switch_on_block);
}

void Chip::set_column_current_switch(
	halco::hicann_dls::v2::ColumnCurrentSwitchOnDLS const& current_switch,
	ColumnCurrentBlock::ColumnCurrentSwitch const& value)
{
	auto block = current_switch.toColumnBlockOnDLS();
	auto switch_on_block = current_switch.toColumnCurrentSwitchOnColumnBlock();
	m_current_blocks.at(block).set_switch(switch_on_block, value);
}

CapMem Chip::get_capmem() const
{
	return m_capmem;
}

void Chip::set_capmem(CapMem const& value)
{
	m_capmem = value;
}

PPUMemory Chip::get_ppu_memory() const
{
	return m_ppu_memory;
}

void Chip::set_ppu_memory(PPUMemory const& value)
{
	m_ppu_memory = value;
}

PPUControlRegister Chip::get_ppu_control_register() const
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

void Chip::set_rate_counter(RateCounter const& value)
{
	m_rate_counter = value;
}

SynapseDrivers Chip::get_synapse_drivers() const
{
	return m_synapse_drivers;
}

void Chip::set_synapse_drivers(SynapseDrivers const& value)
{
	m_synapse_drivers = value;
}

CommonSynramConfig Chip::get_common_synram_config() const
{
	return m_synram_config;
}

void Chip::set_common_synram_config(CommonSynramConfig const& value)
{
	m_synram_config = value;
}

CommonCapMemConfig Chip::get_common_capmem_config() const
{
	return m_capmem_config;
}

void Chip::set_common_capmem_config(CommonCapMemConfig const& value)
{
	m_capmem_config = value;
}

CommonNeuronConfig Chip::get_common_neuron_config() const
{
	return m_neuron_config;
}

void Chip::set_common_neuron_config(CommonNeuronConfig const& value)
{
	m_neuron_config = value;
}

CorrelationConfig Chip::get_correlation_config() const
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

} // namespace v2
} // namespace container
} // namespace haldls
