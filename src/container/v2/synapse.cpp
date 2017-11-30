#include "haldls/container/v2/synapse.h"

#include "halco/common/iter_all.h"

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

std::array<hardware_address_type, CommonSynramConfig::config_size_in_words>
CommonSynramConfig::addresses(CommonSynramConfig::coordinate_type const& /*coord*/) const
{
	hardware_address_type const pc_conf_addr = 0x08000000;
	hardware_address_type const w_conf_addr = pc_conf_addr + 1;
	hardware_address_type const wait_ctr_clear_addr = pc_conf_addr + 2;
	return {{pc_conf_addr, w_conf_addr, wait_ctr_clear_addr}};
}

std::array<hardware_address_type, CommonSynramConfig::config_size_in_words>
CommonSynramConfig::encode() const
{
	return {{static_cast<hardware_word_type>(m_pc_conf), static_cast<hardware_word_type>(m_w_conf),
	         static_cast<hardware_word_type>(m_wait_ctr_clear)}};
}

void CommonSynramConfig::decode(
	std::array<hardware_word_type, CommonSynramConfig::config_size_in_words> const& data)
{
	m_pc_conf = PCConf(data.at(0));
	m_w_conf = WConf(data.at(1));
	m_wait_ctr_clear = WaitCtrClear(data.at(2));
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

std::array<hardware_address_type, SynapseBlock::config_size_in_words> SynapseBlock::addresses(
	SynapseBlock::coordinate_type const& coord) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001f000;
	hardware_address_type const address_offset =
		(coord.y() * NeuronOnDLS::size * config_size_in_words / SynapseOnSynapseBlock::size) +
		coord.x();
	return {
		{base_address + address_offset,
		 base_address + address_offset +
			 static_cast<hardware_address_type>(NeuronOnDLS::size / SynapseOnSynapseBlock::size)}};
}

namespace {

struct SynapseBlockBitfield
{
	union
	{
		std::array<hardware_word_type, SynapseBlock::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type weight_3         : 6;
			hardware_word_type time_calib_3     : 2;

			hardware_word_type weight_2         : 6;
			hardware_word_type time_calib_2     : 2;

			hardware_word_type weight_1         : 6;
			hardware_word_type time_calib_1     : 2;

			hardware_word_type weight_0         : 6;
			hardware_word_type time_calib_0     : 2;

			hardware_word_type address_3        : 6;
			hardware_word_type amp_calib_3      : 2;

			hardware_word_type address_2        : 6;
			hardware_word_type amp_calib_2      : 2;

			hardware_word_type address_1        : 6;
			hardware_word_type amp_calib_1      : 2;

			hardware_word_type address_0        : 6;
			hardware_word_type amp_calib_0      : 2;

		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseBlockBitfield() { u.raw = {{0}}; }
	SynapseBlockBitfield(
		std::array<hardware_word_type, SynapseBlock::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

// synapse ram cells are permuted connected to the DAC
hardware_word_type weight_permutation(hardware_word_type const& weight)
{
	std::bitset<6> before(weight);
	std::bitset<6> after;
	after[5] = before[5];
	after[4] = before[0];
	after[3] = before[1];
	after[2] = before[2];
	after[1] = before[3];
	after[0] = before[4];
	return after.to_ulong();
}

} // namespace


std::array<hardware_word_type, SynapseBlock::config_size_in_words> SynapseBlock::encode() const
{
	using namespace halco::hicann_dls::v2;
	SynapseBlockBitfield bitfield;

#define SYNAPSE_ENCODE(index)                                                                      \
	{                                                                                              \
		SynapseBlock::Synapse const& config = m_synapses.at(SynapseOnSynapseBlock(index));         \
		bitfield.u.m.time_calib_##index = config.get_time_calib();                                 \
		bitfield.u.m.weight_##index = weight_permutation(config.get_weight());                     \
		bitfield.u.m.amp_calib_##index = config.get_amp_calib();                                   \
		bitfield.u.m.address_##index = config.get_address();                                       \
	}

	SYNAPSE_ENCODE(0)
	SYNAPSE_ENCODE(1)
	SYNAPSE_ENCODE(2)
	SYNAPSE_ENCODE(3)
#undef SYNAPSE_ENCODE

	return bitfield.u.raw;
}


void SynapseBlock::decode(
	std::array<hardware_word_type, SynapseBlock::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::v2;
	SynapseBlockBitfield bitfield(data);

#define SYNAPSE_DECODE(index)                                                                      \
	{                                                                                              \
		SynapseBlock::Synapse config;                                                              \
		config.set_time_calib(SynapseBlock::Synapse::TimeCalib(bitfield.u.m.time_calib_##index));  \
		config.set_weight(                                                                         \
			SynapseBlock::Synapse::Weight(weight_permutation(bitfield.u.m.weight_##index)));       \
		config.set_amp_calib(SynapseBlock::Synapse::AmpCalib(bitfield.u.m.amp_calib_##index));     \
		config.set_address(SynapseBlock::Synapse::Address(bitfield.u.m.address_##index));          \
		m_synapses.at(SynapseOnSynapseBlock(index)) = config;                                      \
	}

	SYNAPSE_DECODE(0)
	SYNAPSE_DECODE(1)
	SYNAPSE_DECODE(2)
	SYNAPSE_DECODE(3)
#undef SYNAPSE_DECODE
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

std::array<hardware_address_type, ColumnCorrelationBlock::config_size_in_words>
ColumnCorrelationBlock::addresses(coordinate_type const& coord) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001f200 + 16;
	auto const address = base_address + static_cast<hardware_address_type>(coord);
	auto const offset =
		static_cast<hardware_address_type>(NeuronOnDLS::size / SynapseOnSynapseBlock::size);
	return {{address, address + offset}};
}

namespace {

struct ColumnCorrelationBlockBitfield
{
	union
	{
		std::array<hardware_word_type, ColumnCorrelationBlock::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type                  : 6;
			hardware_word_type int_config_3     : 2;

			hardware_word_type                  : 6;
			hardware_word_type int_config_2     : 2;

			hardware_word_type                  : 6;
			hardware_word_type int_config_1     : 2;

			hardware_word_type                  : 6;
			hardware_word_type int_config_0     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_3     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_2     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_1     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_0     : 2;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ColumnCorrelationBlockBitfield() { u.raw = {{0}}; }
	ColumnCorrelationBlockBitfield(
		std::array<hardware_word_type, ColumnCorrelationBlock::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // namespace


std::array<hardware_word_type, ColumnCorrelationBlock::config_size_in_words>
ColumnCorrelationBlock::encode() const
{
	using namespace halco::hicann_dls::v2;
	ColumnCorrelationBlockBitfield bitfield;

#define CORRELATION_ENCODE(index)                                                                  \
	{                                                                                              \
		ColumnCorrelationSwitch const& config =                                                    \
			m_switches.at(ColumnCorrelationSwitchOnColumnBlock(index));                            \
		if ((config.get_acausal_config() == ColumnCorrelationSwitch::Config::internal) ||          \
			(config.get_acausal_config() == ColumnCorrelationSwitch::Config::readout))             \
			bitfield.u.m.int_config_##index |= 0b10;                                               \
		if ((config.get_acausal_config() == ColumnCorrelationSwitch::Config::external) ||          \
			(config.get_acausal_config() == ColumnCorrelationSwitch::Config::readout))             \
			bitfield.u.m.ext_config_##index |= 0b10;                                               \
		if ((config.get_causal_config() == ColumnCorrelationSwitch::Config::internal) ||           \
			(config.get_causal_config() == ColumnCorrelationSwitch::Config::readout))              \
			bitfield.u.m.int_config_##index |= 0b01;                                               \
		if ((config.get_causal_config() == ColumnCorrelationSwitch::Config::external) ||           \
			(config.get_causal_config() == ColumnCorrelationSwitch::Config::readout))              \
			bitfield.u.m.ext_config_##index |= 0b01;                                               \
	}

	CORRELATION_ENCODE(0);
	CORRELATION_ENCODE(1);
	CORRELATION_ENCODE(2);
	CORRELATION_ENCODE(3);
#undef CORRELATION_ENCODE

	return bitfield.u.raw;
}


void ColumnCorrelationBlock::decode(
	std::array<hardware_word_type, ColumnCorrelationBlock::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::v2;
	ColumnCorrelationBlockBitfield bitfield(data);

#define CORRELATION_DECODE(index)                                                                  \
	{                                                                                              \
		ColumnCorrelationSwitch config;                                                            \
		hardware_word_type acausal =                                                               \
			((bitfield.u.m.int_config_##index & 0b10) |                                            \
			 ((bitfield.u.m.ext_config_##index & 0b10) >> 1));                                     \
		hardware_word_type causal =                                                                \
			(((bitfield.u.m.int_config_##index & 0b01) << 1) |                                     \
			 (bitfield.u.m.ext_config_##index & 0b01));                                            \
		config.set_acausal_config(ColumnCorrelationSwitch::Config(acausal));                       \
		config.set_causal_config(ColumnCorrelationSwitch::Config(causal));                         \
		m_switches.at(ColumnCorrelationSwitchOnColumnBlock(index)) = config;                       \
	}

	CORRELATION_DECODE(0)
	CORRELATION_DECODE(1)
	CORRELATION_DECODE(2)
	CORRELATION_DECODE(3)
#undef CORRELATION_DECODE
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

std::array<hardware_address_type, ColumnCurrentBlock::config_size_in_words>
ColumnCurrentBlock::addresses(coordinate_type const& coord) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001f200;
	auto const address = base_address + static_cast<hardware_address_type>(coord);
	auto const offset =
		static_cast<hardware_address_type>(NeuronOnDLS::size / SynapseOnSynapseBlock::size);
	return {{address, address + offset}};
}

namespace {

struct ColumnCurrentBlockBitfield
{
	union
	{
		std::array<hardware_word_type, ColumnCurrentBlock::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type                  : 6;
			hardware_word_type int_config_3     : 2;

			hardware_word_type                  : 6;
			hardware_word_type int_config_2     : 2;

			hardware_word_type                  : 6;
			hardware_word_type int_config_1     : 2;

			hardware_word_type                  : 6;
			hardware_word_type int_config_0     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_3     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_2     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_1     : 2;

			hardware_word_type                  : 6;
			hardware_word_type ext_config_0     : 2;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ColumnCurrentBlockBitfield() { u.raw = {{0}}; }
	ColumnCurrentBlockBitfield(
		std::array<hardware_word_type, ColumnCurrentBlock::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // namespace


std::array<hardware_word_type, ColumnCurrentBlock::config_size_in_words>
ColumnCurrentBlock::encode() const
{
	using namespace halco::hicann_dls::v2;
	ColumnCurrentBlockBitfield bitfield;

#define CURRENT_ENCODE(index)                                                                      \
	{                                                                                              \
		ColumnCurrentSwitch const& config =                                                        \
			m_switches.at(ColumnCurrentSwitchOnColumnBlock(index));                                \
		if ((config.get_inh_config() == ColumnCurrentSwitch::Config::internal) ||                  \
			(config.get_inh_config() == ColumnCurrentSwitch::Config::readout))                     \
			bitfield.u.m.int_config_##index |= 0b10;                                               \
		if ((config.get_inh_config() == ColumnCurrentSwitch::Config::external) ||                  \
			(config.get_inh_config() == ColumnCurrentSwitch::Config::readout))                     \
			bitfield.u.m.ext_config_##index |= 0b10;                                               \
		if ((config.get_exc_config() == ColumnCurrentSwitch::Config::internal) ||                  \
			(config.get_exc_config() == ColumnCurrentSwitch::Config::readout))                     \
			bitfield.u.m.int_config_##index |= 0b01;                                               \
		if ((config.get_exc_config() == ColumnCurrentSwitch::Config::external) ||                  \
			(config.get_exc_config() == ColumnCurrentSwitch::Config::readout))                     \
			bitfield.u.m.ext_config_##index |= 0b01;                                               \
	}

	CURRENT_ENCODE(0);
	CURRENT_ENCODE(1);
	CURRENT_ENCODE(2);
	CURRENT_ENCODE(3);
#undef CURRENT_ENCODE

	return bitfield.u.raw;
}


void ColumnCurrentBlock::decode(
	std::array<hardware_word_type, ColumnCurrentBlock::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::v2;
	ColumnCurrentBlockBitfield bitfield(data);

#define CURRENT_DECODE(index)                                                                      \
	{                                                                                              \
		ColumnCurrentSwitch config;                                                                \
		hardware_word_type inh =                                                                   \
			((bitfield.u.m.int_config_##index & 0b10) |                                            \
			 ((bitfield.u.m.ext_config_##index & 0b10) >> 1));                                     \
		hardware_word_type exc =                                                                   \
			(((bitfield.u.m.int_config_##index & 0b01) << 1) |                                     \
			 (bitfield.u.m.ext_config_##index & 0b01));                                            \
		config.set_inh_config(ColumnCurrentSwitch::Config(inh));                                   \
		config.set_exc_config(ColumnCurrentSwitch::Config(exc));                                   \
		m_switches.at(ColumnCurrentSwitchOnColumnBlock(index)) = config;                           \
	}

	CURRENT_DECODE(0)
	CURRENT_DECODE(1)
	CURRENT_DECODE(2)
	CURRENT_DECODE(3)
#undef CURRENT_DECODE
}

} // namespace v2
} // namespace container
} // namespace haldls
