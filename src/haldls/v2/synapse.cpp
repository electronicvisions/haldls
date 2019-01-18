#include "haldls/v2/synapse.h"

#include "halco/common/iter_all.h"

namespace haldls {
namespace v2 {

CommonSynramConfig::CommonSynramConfig()
    : m_pc_conf(2),
      m_w_conf(2),
      m_wait_ctr_clear(2),
      m_use_internal_i_bias_correlation_output(false),
      m_use_internal_i_bias_vstore(false),
      m_use_internal_i_bias_vramp(false),
      m_use_internal_i_bias_vdac(false)
{
}

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

bool CommonSynramConfig::get_use_internal_i_bias_correlation_output() const
{
	return m_use_internal_i_bias_correlation_output;
}

void CommonSynramConfig::set_use_internal_i_bias_correlation_output(bool const value)
{
	m_use_internal_i_bias_correlation_output = value;
}

bool CommonSynramConfig::get_use_internal_i_bias_vstore() const
{
	return m_use_internal_i_bias_vstore;
}

void CommonSynramConfig::set_use_internal_i_bias_vstore(bool const value)
{
	m_use_internal_i_bias_vstore = value;
}

bool CommonSynramConfig::get_use_internal_i_bias_vramp() const
{
	return m_use_internal_i_bias_vramp;
}

void CommonSynramConfig::set_use_internal_i_bias_vramp(bool const value)
{
	m_use_internal_i_bias_vramp = value;
}

bool CommonSynramConfig::get_use_internal_i_bias_vdac() const
{
	return m_use_internal_i_bias_vdac;
}

void CommonSynramConfig::set_use_internal_i_bias_vdac(bool const value)
{
	m_use_internal_i_bias_vdac = value;
}

bool CommonSynramConfig::operator==(CommonSynramConfig const& other) const
{
	return m_pc_conf == other.get_pc_conf() && m_w_conf == other.get_w_conf() &&
	       m_wait_ctr_clear == other.get_wait_ctr_clear() &&
	       m_use_internal_i_bias_correlation_output ==
	           other.get_use_internal_i_bias_correlation_output() &&
	       m_use_internal_i_bias_vstore == other.get_use_internal_i_bias_vstore() &&
	       m_use_internal_i_bias_vramp == other.get_use_internal_i_bias_vramp() &&
	       m_use_internal_i_bias_vdac == other.get_use_internal_i_bias_vdac();
}

bool CommonSynramConfig::operator!=(CommonSynramConfig const& other) const
{
	return !(*this == other);
}

std::array<hardware_address_type, CommonSynramConfig::config_size_in_words>
CommonSynramConfig::addresses(CommonSynramConfig::coordinate_type const& /*unique*/) const
{
	hardware_address_type const base_addr = 0x08000000;
	std::array<hardware_address_type, config_size_in_words> addr;
	for (size_t counter = 0; counter < config_size_in_words; counter++) {
		addr.at(counter) = base_addr + counter;
	}
	return addr;
}

namespace {

struct CommonSynramConfigBitfield
{
	union
	{
		std::array<hardware_word_type, CommonSynramConfig::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type pc_conf            :  4;
			hardware_word_type                    : 28;
			hardware_word_type w_conf             :  8;
			hardware_word_type                    : 24;
			hardware_word_type wait_ctr_clear     :  3;
			hardware_word_type                    : 29;
			hardware_word_type use_coroutbias     :  1;
			hardware_word_type use_vstore         :  1;
			hardware_word_type use_vramp          :  1;
			hardware_word_type use_vdac           :  1;
			hardware_word_type                    : 28;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CommonSynramConfigBitfield() { u.raw = {{0}}; }
	CommonSynramConfigBitfield(
	    std::array<hardware_word_type, CommonSynramConfig::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // anonymous namespace

std::array<hardware_address_type, CommonSynramConfig::config_size_in_words>
CommonSynramConfig::encode() const
{
	CommonSynramConfigBitfield bitfield;
	bitfield.u.m.pc_conf = m_pc_conf.value();
	bitfield.u.m.w_conf = m_w_conf.value();
	bitfield.u.m.wait_ctr_clear = m_wait_ctr_clear.value();
	bitfield.u.m.use_coroutbias = m_use_internal_i_bias_correlation_output;
	bitfield.u.m.use_vstore = m_use_internal_i_bias_vstore;
	bitfield.u.m.use_vramp = m_use_internal_i_bias_vramp;
	bitfield.u.m.use_vdac = m_use_internal_i_bias_vdac;
	return bitfield.u.raw;
}

void CommonSynramConfig::decode(
	std::array<hardware_word_type, CommonSynramConfig::config_size_in_words> const& data)
{
	CommonSynramConfigBitfield bitfield(data);
	m_pc_conf = PCConf(bitfield.u.m.pc_conf);
	m_w_conf = WConf(bitfield.u.m.w_conf);
	m_wait_ctr_clear = WaitCtrClear(bitfield.u.m.wait_ctr_clear);
	m_use_internal_i_bias_correlation_output = bitfield.u.m.use_coroutbias;
	m_use_internal_i_bias_vstore = bitfield.u.m.use_vstore;
	m_use_internal_i_bias_vramp = bitfield.u.m.use_vramp;
	m_use_internal_i_bias_vdac = bitfield.u.m.use_vdac;
}

template <class Archive>
void CommonSynramConfig::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_pc_conf));
	ar(CEREAL_NVP(m_w_conf));
	ar(CEREAL_NVP(m_wait_ctr_clear));
	ar(CEREAL_NVP(m_use_internal_i_bias_correlation_output));
	ar(CEREAL_NVP(m_use_internal_i_bias_vstore));
	ar(CEREAL_NVP(m_use_internal_i_bias_vramp));
	ar(CEREAL_NVP(m_use_internal_i_bias_vdac));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CommonSynramConfig)

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

template <class Archive>
void SynapseBlock::Synapse::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_weight));
	ar(CEREAL_NVP(m_address));
	ar(CEREAL_NVP(m_time_calib));
	ar(CEREAL_NVP(m_amp_calib));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseBlock::Synapse)

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
	SynapseBlock::coordinate_type const& block) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001f000;
	hardware_address_type const address_offset =
		(block.y() * NeuronOnDLS::size * config_size_in_words / SynapseOnSynapseBlock::size) +
		block.x();
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

template <class Archive>
void SynapseBlock::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_synapses));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseBlock)

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

template <class Archive>
void ColumnCorrelationBlock::ColumnCorrelationSwitch::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_causal));
	ar(CEREAL_NVP(m_acausal));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCorrelationBlock::ColumnCorrelationSwitch)

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
ColumnCorrelationBlock::addresses(coordinate_type const& block) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001f200 + 16;
	auto const address = base_address + static_cast<hardware_address_type>(block);
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

template <class Archive>
void ColumnCorrelationBlock::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_switches));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCorrelationBlock)

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

template <class Archive>
void ColumnCurrentBlock::ColumnCurrentSwitch::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_exc));
	ar(CEREAL_NVP(m_inh));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCurrentBlock::ColumnCurrentSwitch)

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
ColumnCurrentBlock::addresses(coordinate_type const& block) const
{
	using namespace halco::hicann_dls::v2;
	hardware_address_type const base_address = 0x0001f200;
	auto const address = base_address + static_cast<hardware_address_type>(block);
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

template <class Archive>
void ColumnCurrentBlock::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_switches));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCurrentBlock)

} // namespace v2
} // namespace haldls
