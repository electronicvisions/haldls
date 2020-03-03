#include "haldls/vx/synapse.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/address_transformation.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

namespace haldls::vx {

CommonSynramConfig::CommonSynramConfig() :
    m_pc_conf_west(),
    m_pc_conf_east(),
    m_w_conf_west(),
    m_w_conf_east(),
    m_wait_ctr_clear()
{}

CommonSynramConfig::PCConf CommonSynramConfig::get_pc_conf_west() const
{
	return m_pc_conf_west;
}

void CommonSynramConfig::set_pc_conf_west(CommonSynramConfig::PCConf const& value)
{
	m_pc_conf_west = value;
}

CommonSynramConfig::PCConf CommonSynramConfig::get_pc_conf_east() const
{
	return m_pc_conf_east;
}

void CommonSynramConfig::set_pc_conf_east(CommonSynramConfig::PCConf const& value)
{
	m_pc_conf_east = value;
}

CommonSynramConfig::WConf CommonSynramConfig::get_w_conf_west() const
{
	return m_w_conf_west;
}

void CommonSynramConfig::set_w_conf_west(CommonSynramConfig::WConf const& value)
{
	m_w_conf_west = value;
}

CommonSynramConfig::WConf CommonSynramConfig::get_w_conf_east() const
{
	return m_w_conf_east;
}

void CommonSynramConfig::set_w_conf_east(CommonSynramConfig::WConf const& value)
{
	m_w_conf_east = value;
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
	return (m_pc_conf_west == other.m_pc_conf_west) && (m_pc_conf_east == other.m_pc_conf_east) &&
	       (m_w_conf_west == other.m_w_conf_west) && (m_w_conf_east == other.m_w_conf_east) &&
	       (m_wait_ctr_clear == other.m_wait_ctr_clear);
}

bool CommonSynramConfig::operator!=(CommonSynramConfig const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, CommonSynramConfig::config_size_in_words> CommonSynramConfig::addresses(
    coordinate_type const& coord)
{
	size_t base;
	if (coord == CommonSynramConfig::coordinate_type::bottom) {
		base = synram_ctrl_bottom_base_address;
	} else {
		base = synram_ctrl_top_base_address;
	}
	return {AddressT(base), AddressT(base + 1), AddressT(base + 2)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, CommonSynramConfig::config_size_in_words>
    CommonSynramConfig::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CommonSynramConfig::config_size_in_words>
CommonSynramConfig::addresses(coordinate_type const& coord);

namespace {

struct CommonSynramConfigBitfield
{
	union
	{
		std::array<uint32_t, CommonSynramConfig::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t pc_conf_west       :  4;
			uint32_t pc_conf_east       :  4;
			uint32_t                    : 24;
			uint32_t w_conf_west        :  8;
			uint32_t w_conf_east        :  8;
			uint32_t                    : 16;
			uint32_t wait_ctr_clear     :  3;
			uint32_t                    : 29;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CommonSynramConfigBitfield() { u.raw = {{0}}; }
	CommonSynramConfigBitfield(
	    std::array<uint32_t, CommonSynramConfig::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, CommonSynramConfig::config_size_in_words> CommonSynramConfig::encode() const
{
	CommonSynramConfigBitfield bitfield;
	bitfield.u.m.pc_conf_west = m_pc_conf_west.value();
	bitfield.u.m.pc_conf_east = m_pc_conf_east.value();
	bitfield.u.m.w_conf_west = m_w_conf_west.value();
	bitfield.u.m.w_conf_east = m_w_conf_east.value();
	bitfield.u.m.wait_ctr_clear = m_wait_ctr_clear.value();

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, CommonSynramConfig::config_size_in_words>
CommonSynramConfig::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CommonSynramConfig::config_size_in_words>
    CommonSynramConfig::encode() const;

template <typename WordT>
void CommonSynramConfig::decode(
    std::array<WordT, CommonSynramConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });

	CommonSynramConfigBitfield bitfield(raw_data);
	m_pc_conf_west = PCConf(bitfield.u.m.pc_conf_west);
	m_pc_conf_east = PCConf(bitfield.u.m.pc_conf_east);
	m_w_conf_west = WConf(bitfield.u.m.w_conf_west);
	m_w_conf_east = WConf(bitfield.u.m.w_conf_east);
	m_wait_ctr_clear = WaitCtrClear(bitfield.u.m.wait_ctr_clear);
}

template SYMBOL_VISIBLE void CommonSynramConfig::decode(
    std::array<fisch::vx::OmnibusChip, CommonSynramConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void CommonSynramConfig::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, CommonSynramConfig::config_size_in_words> const&
        data);

HALDLS_VX_DEFAULT_OSTREAM_OP(CommonSynramConfig)

template <class Archive>
void CommonSynramConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_pc_conf_west));
	ar(CEREAL_NVP(m_pc_conf_east));
	ar(CEREAL_NVP(m_w_conf_west));
	ar(CEREAL_NVP(m_w_conf_east));
	ar(CEREAL_NVP(m_wait_ctr_clear));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CommonSynramConfig)


SynapseQuad::Synapse::Synapse() : m_weight(0), m_address(0), m_time_calib(0), m_amp_calib(0) {}

SynapseQuad::Synapse::Weight SynapseQuad::Synapse::get_weight() const
{
	return m_weight;
}

void SynapseQuad::Synapse::set_weight(SynapseQuad::Synapse::Weight const& value)
{
	m_weight = value;
}

SynapseQuad::Synapse::Address SynapseQuad::Synapse::get_address() const
{
	return m_address;
}

void SynapseQuad::Synapse::set_address(SynapseQuad::Synapse::Address const& value)
{
	m_address = value;
}

SynapseQuad::Synapse::TimeCalib SynapseQuad::Synapse::get_time_calib() const
{
	return m_time_calib;
}

void SynapseQuad::Synapse::set_time_calib(SynapseQuad::Synapse::TimeCalib const& value)
{
	m_time_calib = value;
}

SynapseQuad::Synapse::AmpCalib SynapseQuad::Synapse::get_amp_calib() const
{
	return m_amp_calib;
}

void SynapseQuad::Synapse::set_amp_calib(SynapseQuad::Synapse::AmpCalib const& value)
{
	m_amp_calib = value;
}

bool SynapseQuad::Synapse::operator==(SynapseQuad::Synapse const& other) const
{
	return m_weight == other.get_weight() && m_address == other.get_address() &&
	       m_time_calib == other.get_time_calib() && m_amp_calib == other.get_amp_calib();
}

bool SynapseQuad::Synapse::operator!=(SynapseQuad::Synapse const& other) const
{
	return !(*this == other);
}

template <class Archive>
void SynapseQuad::Synapse::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_weight));
	ar(CEREAL_NVP(m_address));
	ar(CEREAL_NVP(m_time_calib));
	ar(CEREAL_NVP(m_amp_calib));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseQuad::Synapse)


SynapseQuad::SynapseQuad() : m_synapses() {}

SynapseQuad::Synapse SynapseQuad::get_synapse(
    halco::hicann_dls::vx::EntryOnQuad const& synapse) const
{
	return m_synapses.at(synapse);
}

void SynapseQuad::set_synapse(
    halco::hicann_dls::vx::EntryOnQuad const& synapse, SynapseQuad::Synapse const& value)
{
	m_synapses.at(synapse) = value;
}

bool SynapseQuad::operator==(SynapseQuad const& other) const
{
	return m_synapses == other.m_synapses;
}

bool SynapseQuad::operator!=(SynapseQuad const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, SynapseQuad::config_size_in_words> SynapseQuad::addresses(
    SynapseQuad::coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx;
	uint32_t base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base = synram_synapse_bottom_base_address;
	} else {
		base = synram_synapse_top_base_address;
	}
	uint32_t const address_offset =
	    (block.y() * SynapseQuadColumnOnDLS::size * config_size_in_words) +
	    detail::to_synram_quad_address_offset(block.x());
	return {{AddressT(base + address_offset),
	         AddressT(base + address_offset + SynapseQuadColumnOnDLS::size)}};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, SynapseQuad::config_size_in_words>
    SynapseQuad::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, SynapseQuad::config_size_in_words>
    SynapseQuad::addresses(coordinate_type const& coord);

namespace {

struct SynapseQuadBitfield
{
	union
	{
		std::array<uint32_t, SynapseQuad::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t weight_0         : 6;
			uint32_t time_calib_0     : 2;

			uint32_t weight_1         : 6;
			uint32_t time_calib_1     : 2;

			uint32_t weight_2         : 6;
			uint32_t time_calib_2     : 2;

			uint32_t weight_3         : 6;
			uint32_t time_calib_3     : 2;

			uint32_t address_0        : 6;
			uint32_t amp_calib_0      : 2;

			uint32_t address_1        : 6;
			uint32_t amp_calib_1      : 2;

			uint32_t address_2        : 6;
			uint32_t amp_calib_2      : 2;

			uint32_t address_3        : 6;
			uint32_t amp_calib_3      : 2;

		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseQuadBitfield() { u.raw = {{0}}; }
	SynapseQuadBitfield(std::array<uint32_t, SynapseQuad::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

// synapse ram cells are permuted connected to the DAC
uint32_t weight_permutation(uint32_t const& weight)
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

template <typename WordT>
std::array<WordT, SynapseQuad::config_size_in_words> SynapseQuad::encode() const
{
	using namespace halco::hicann_dls::vx;
	SynapseQuadBitfield bitfield;

#define SYNAPSE_ENCODE(index)                                                                      \
	{                                                                                              \
		SynapseQuad::Synapse const& config = m_synapses.at(EntryOnQuad(index));                    \
		bitfield.u.m.time_calib_##index = config.m_time_calib;                                     \
		bitfield.u.m.weight_##index = weight_permutation(config.m_weight);                         \
		bitfield.u.m.amp_calib_##index = config.m_amp_calib;                                       \
		bitfield.u.m.address_##index = config.m_address;                                           \
	}

	SYNAPSE_ENCODE(0)
	SYNAPSE_ENCODE(1)
	SYNAPSE_ENCODE(2)
	SYNAPSE_ENCODE(3)
#undef SYNAPSE_ENCODE

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, SynapseQuad::config_size_in_words>
SynapseQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseQuad::config_size_in_words>
    SynapseQuad::encode() const;

template <typename WordT>
void SynapseQuad::decode(std::array<WordT, SynapseQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });

	SynapseQuadBitfield bitfield(raw_data);

#define SYNAPSE_DECODE(index)                                                                      \
	{                                                                                              \
		SynapseQuad::Synapse config;                                                               \
		config.set_time_calib(SynapseQuad::Synapse::TimeCalib(bitfield.u.m.time_calib_##index));   \
		config.set_weight(                                                                         \
		    SynapseQuad::Synapse::Weight(weight_permutation(bitfield.u.m.weight_##index)));        \
		config.set_amp_calib(SynapseQuad::Synapse::AmpCalib(bitfield.u.m.amp_calib_##index));      \
		config.set_address(SynapseQuad::Synapse::Address(bitfield.u.m.address_##index));           \
		m_synapses.at(EntryOnQuad(index)) = config;                                                \
	}

	SYNAPSE_DECODE(0)
	SYNAPSE_DECODE(1)
	SYNAPSE_DECODE(2)
	SYNAPSE_DECODE(3)
#undef SYNAPSE_DECODE
}

template SYMBOL_VISIBLE void SynapseQuad::decode(
    std::array<fisch::vx::OmnibusChip, SynapseQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void SynapseQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseQuad::config_size_in_words> const& data);

HALDLS_VX_DEFAULT_OSTREAM_OP(SynapseQuad)

template <class Archive>
void SynapseQuad::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_synapses));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseQuad)


ColumnCorrelationQuad::ColumnCorrelationSwitch::ColumnCorrelationSwitch() :
    m_enable_internal_causal(false),
    m_enable_internal_acausal(false),
    m_enable_debug_causal(false),
    m_enable_debug_acausal(false),
    m_enable_cadc_neuron_readout_causal(false),
    m_enable_cadc_neuron_readout_acausal(false)
{}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::get_enable_internal_causal() const
{
	return m_enable_internal_causal;
}

void ColumnCorrelationQuad::ColumnCorrelationSwitch::set_enable_internal_causal(bool const value)
{
	m_enable_internal_causal = value;
}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::get_enable_internal_acausal() const
{
	return m_enable_internal_acausal;
}

void ColumnCorrelationQuad::ColumnCorrelationSwitch::set_enable_internal_acausal(bool const value)
{
	m_enable_internal_acausal = value;
}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::get_enable_debug_causal() const
{
	return m_enable_debug_causal;
}

void ColumnCorrelationQuad::ColumnCorrelationSwitch::set_enable_debug_causal(bool const value)
{
	m_enable_debug_causal = value;
}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::get_enable_debug_acausal() const
{
	return m_enable_debug_acausal;
}

void ColumnCorrelationQuad::ColumnCorrelationSwitch::set_enable_debug_acausal(bool const value)
{
	m_enable_debug_acausal = value;
}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::get_enable_cadc_neuron_readout_causal() const
{
	return m_enable_cadc_neuron_readout_causal;
}

void ColumnCorrelationQuad::ColumnCorrelationSwitch::set_enable_cadc_neuron_readout_causal(
    bool const value)
{
	m_enable_cadc_neuron_readout_causal = value;
}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::get_enable_cadc_neuron_readout_acausal() const
{
	return m_enable_cadc_neuron_readout_acausal;
}

void ColumnCorrelationQuad::ColumnCorrelationSwitch::set_enable_cadc_neuron_readout_acausal(
    bool const value)
{
	m_enable_cadc_neuron_readout_acausal = value;
}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::operator==(
    ColumnCorrelationQuad::ColumnCorrelationSwitch const& other) const
{
	return m_enable_internal_causal == other.m_enable_internal_causal &&
	       m_enable_internal_acausal == other.m_enable_internal_acausal &&
	       m_enable_debug_causal == other.m_enable_debug_causal &&
	       m_enable_debug_acausal == other.m_enable_debug_acausal &&
	       m_enable_cadc_neuron_readout_causal == other.m_enable_cadc_neuron_readout_causal &&
	       m_enable_cadc_neuron_readout_acausal == other.m_enable_cadc_neuron_readout_acausal;
}

bool ColumnCorrelationQuad::ColumnCorrelationSwitch::operator!=(
    ColumnCorrelationQuad::ColumnCorrelationSwitch const& other) const
{
	return !(*this == other);
}

template <class Archive>
void ColumnCorrelationQuad::ColumnCorrelationSwitch::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_internal_causal));
	ar(CEREAL_NVP(m_enable_internal_acausal));
	ar(CEREAL_NVP(m_enable_debug_causal));
	ar(CEREAL_NVP(m_enable_debug_acausal));
	ar(CEREAL_NVP(m_enable_cadc_neuron_readout_causal));
	ar(CEREAL_NVP(m_enable_cadc_neuron_readout_acausal));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCorrelationQuad::ColumnCorrelationSwitch)

ColumnCorrelationQuad::ColumnCorrelationQuad() : m_switches() {}

ColumnCorrelationQuad::ColumnCorrelationSwitch ColumnCorrelationQuad::get_switch(
    halco::hicann_dls::vx::EntryOnQuad const& correlation_switch) const
{
	return m_switches.at(correlation_switch);
}

void ColumnCorrelationQuad::set_switch(
    halco::hicann_dls::vx::EntryOnQuad const& correlation_switch,
    ColumnCorrelationQuad::ColumnCorrelationSwitch const& value)
{
	m_switches.at(correlation_switch) = value;
}

bool ColumnCorrelationQuad::operator==(ColumnCorrelationQuad const& other) const
{
	return m_switches == other.m_switches;
}

bool ColumnCorrelationQuad::operator!=(ColumnCorrelationQuad const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, ColumnCorrelationQuad::config_size_in_words> ColumnCorrelationQuad::addresses(
    coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx;
	uint32_t raw_base;
	uint32_t lsb_base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		raw_base = synram_synapse_bottom_base_address;
		lsb_base = synram_synapse_6lsb_bottom_base_address;
	} else {
		raw_base = synram_synapse_top_base_address;
		lsb_base = synram_synapse_6lsb_top_base_address;
	}
	auto const address_0 =
	    raw_base + detail::to_synram_quad_address_offset(block.toSynapseQuadColumnOnDLS()) +
	    SynapseQuadOnSynram::size * SynapseQuad::config_size_in_words +
	    ColumnCurrentQuadOnSynram::size * ColumnCurrentQuad::config_size_in_words;
	auto const address_1 = lsb_base +
	                       detail::to_synram_quad_address_offset(block.toSynapseQuadColumnOnDLS()) +
	                       SynapseQuadOnSynram::size * SynapseQuad::config_size_in_words;
	return {{AddressT(address_0), AddressT((address_0) + ColumnCorrelationQuadOnSynram::size),
	         AddressT(address_1), AddressT(address_1 + ColumnCorrelationQuadOnSynram::size)}};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipAddress, ColumnCorrelationQuad::config_size_in_words>
    ColumnCorrelationQuad::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    ColumnCorrelationQuad::config_size_in_words>
ColumnCorrelationQuad::addresses(coordinate_type const& coord);

namespace {

struct ColumnCorrelationQuadBitfield
{
	union
	{
		std::array<uint32_t, ColumnCorrelationQuad::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t                             : 6;
			uint32_t enable_internal_causal_0    : 1;
			uint32_t enable_internal_acausal_0   : 1;

			uint32_t                             : 6;
			uint32_t enable_internal_causal_1    : 1;
			uint32_t enable_internal_acausal_1   : 1;

			uint32_t                             : 6;
			uint32_t enable_internal_causal_2    : 1;
			uint32_t enable_internal_acausal_2   : 1;

			uint32_t                             : 6;
			uint32_t enable_internal_causal_3    : 1;
			uint32_t enable_internal_acausal_3   : 1;

			uint32_t                             : 6;
			uint32_t enable_debug_causal_0       : 1;
			uint32_t enable_debug_acausal_0      : 1;

			uint32_t                             : 6;
			uint32_t enable_debug_causal_1       : 1;
			uint32_t enable_debug_acausal_1      : 1;

			uint32_t                             : 6;
			uint32_t enable_debug_causal_2       : 1;
			uint32_t enable_debug_acausal_2      : 1;

			uint32_t                             : 6;
			uint32_t enable_debug_causal_3       : 1;
			uint32_t enable_debug_acausal_3      : 1;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_causal_0  : 1;
			uint32_t                                      : 2;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_causal_1  : 1;
			uint32_t                                      : 2;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_causal_2  : 1;
			uint32_t                                      : 2;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_causal_3  : 1;
			uint32_t                                      : 2;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_acausal_0 : 1;
			uint32_t                                      : 2;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_acausal_1 : 1;
			uint32_t                                      : 2;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_acausal_2 : 1;
			uint32_t                                      : 2;

			uint32_t                                      : 5;
			uint32_t enable_cadc_neuron_readout_acausal_3 : 1;
			uint32_t                                      : 2;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ColumnCorrelationQuadBitfield() { u.raw = {{0}}; }
	ColumnCorrelationQuadBitfield(
	    std::array<uint32_t, ColumnCorrelationQuad::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, ColumnCorrelationQuad::config_size_in_words> ColumnCorrelationQuad::encode() const
{
	using namespace halco::hicann_dls::vx;
	ColumnCorrelationQuadBitfield bitfield;

#define CORRELATION_ENCODE(index)                                                                  \
	{                                                                                              \
		ColumnCorrelationSwitch const& config = m_switches.at(EntryOnQuad(index));                 \
		bitfield.u.m.enable_internal_causal_##index = config.get_enable_internal_causal();         \
		bitfield.u.m.enable_internal_acausal_##index = config.get_enable_internal_acausal();       \
		bitfield.u.m.enable_debug_causal_##index = config.get_enable_debug_causal();               \
		bitfield.u.m.enable_debug_acausal_##index = config.get_enable_debug_acausal();             \
		bitfield.u.m.enable_cadc_neuron_readout_causal_##index =                                   \
		    config.get_enable_cadc_neuron_readout_causal();                                        \
		bitfield.u.m.enable_cadc_neuron_readout_acausal_##index =                                  \
		    config.get_enable_cadc_neuron_readout_acausal();                                       \
	}

	CORRELATION_ENCODE(0);
	CORRELATION_ENCODE(1);
	CORRELATION_ENCODE(2);
	CORRELATION_ENCODE(3);
#undef CORRELATION_ENCODE

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw[0])),
	        WordT(fisch::vx::OmnibusData(bitfield.u.raw[1])),
	        WordT(fisch::vx::OmnibusData(bitfield.u.raw[2])),
	        WordT(fisch::vx::OmnibusData(bitfield.u.raw[3]))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, ColumnCorrelationQuad::config_size_in_words>
    ColumnCorrelationQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCorrelationQuad::config_size_in_words>
    ColumnCorrelationQuad::encode() const;

template <typename WordT>
void ColumnCorrelationQuad::decode(
    std::array<WordT, ColumnCorrelationQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	ColumnCorrelationQuadBitfield bitfield(
	    {data[0].get(), data[1].get(), data[2].get(), data[3].get()});

#define CORRELATION_DECODE(index)                                                                  \
	{                                                                                              \
		ColumnCorrelationSwitch config;                                                            \
		config.set_enable_internal_causal(bitfield.u.m.enable_internal_causal_##index);            \
		config.set_enable_internal_acausal(bitfield.u.m.enable_internal_acausal_##index);          \
		config.set_enable_debug_causal(bitfield.u.m.enable_debug_causal_##index);                  \
		config.set_enable_debug_acausal(bitfield.u.m.enable_debug_acausal_##index);                \
		config.set_enable_cadc_neuron_readout_causal(                                              \
		    bitfield.u.m.enable_cadc_neuron_readout_causal_##index);                               \
		config.set_enable_cadc_neuron_readout_acausal(                                             \
		    bitfield.u.m.enable_cadc_neuron_readout_acausal_##index);                              \
		m_switches.at(EntryOnQuad(index)) = config;                                                \
	}

	CORRELATION_DECODE(0)
	CORRELATION_DECODE(1)
	CORRELATION_DECODE(2)
	CORRELATION_DECODE(3)
#undef CORRELATION_DECODE
}

template SYMBOL_VISIBLE void ColumnCorrelationQuad::decode(
    std::array<fisch::vx::OmnibusChip, ColumnCorrelationQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void ColumnCorrelationQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCorrelationQuad::config_size_in_words> const&
        data);

template <class Archive>
void ColumnCorrelationQuad::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_switches));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCorrelationQuad)


ColumnCurrentQuad::ColumnCurrentSwitch::ColumnCurrentSwitch() :
    m_enable_synaptic_current_excitatory(false),
    m_enable_synaptic_current_inhibitory(false),
    m_enable_debug_excitatory(false),
    m_enable_debug_inhibitory(false)
{}

bool ColumnCurrentQuad::ColumnCurrentSwitch::get_enable_synaptic_current_excitatory() const
{
	return m_enable_synaptic_current_excitatory;
}

void ColumnCurrentQuad::ColumnCurrentSwitch::set_enable_synaptic_current_excitatory(
    bool const value)
{
	m_enable_synaptic_current_excitatory = value;
}

bool ColumnCurrentQuad::ColumnCurrentSwitch::get_enable_synaptic_current_inhibitory() const
{
	return m_enable_synaptic_current_inhibitory;
}

void ColumnCurrentQuad::ColumnCurrentSwitch::set_enable_synaptic_current_inhibitory(
    bool const value)
{
	m_enable_synaptic_current_inhibitory = value;
}

bool ColumnCurrentQuad::ColumnCurrentSwitch::get_enable_debug_excitatory() const
{
	return m_enable_debug_excitatory;
}

void ColumnCurrentQuad::ColumnCurrentSwitch::set_enable_debug_excitatory(bool const value)
{
	m_enable_debug_excitatory = value;
}

bool ColumnCurrentQuad::ColumnCurrentSwitch::get_enable_debug_inhibitory() const
{
	return m_enable_debug_inhibitory;
}

void ColumnCurrentQuad::ColumnCurrentSwitch::set_enable_debug_inhibitory(bool const value)
{
	m_enable_debug_inhibitory = value;
}

bool ColumnCurrentQuad::ColumnCurrentSwitch::operator==(
    ColumnCurrentQuad::ColumnCurrentSwitch const& other) const
{
	return m_enable_synaptic_current_excitatory == other.m_enable_synaptic_current_excitatory &&
	       m_enable_synaptic_current_inhibitory == other.m_enable_synaptic_current_inhibitory &&
	       m_enable_debug_excitatory == other.m_enable_debug_excitatory &&
	       m_enable_debug_inhibitory == other.m_enable_debug_inhibitory;
}

bool ColumnCurrentQuad::ColumnCurrentSwitch::operator!=(
    ColumnCurrentQuad::ColumnCurrentSwitch const& other) const
{
	return !(*this == other);
}

template <class Archive>
void ColumnCurrentQuad::ColumnCurrentSwitch::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_synaptic_current_excitatory));
	ar(CEREAL_NVP(m_enable_synaptic_current_inhibitory));
	ar(CEREAL_NVP(m_enable_debug_excitatory));
	ar(CEREAL_NVP(m_enable_debug_inhibitory));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCurrentQuad::ColumnCurrentSwitch)

ColumnCurrentQuad::ColumnCurrentQuad() : m_switches() {}

ColumnCurrentQuad::ColumnCurrentSwitch ColumnCurrentQuad::get_switch(
    halco::hicann_dls::vx::EntryOnQuad const& current_switch) const
{
	return m_switches.at(current_switch);
}

void ColumnCurrentQuad::set_switch(
    halco::hicann_dls::vx::EntryOnQuad const& current_switch,
    ColumnCurrentQuad::ColumnCurrentSwitch const& value)
{
	m_switches.at(current_switch) = value;
}

bool ColumnCurrentQuad::operator==(ColumnCurrentQuad const& other) const
{
	return m_switches == other.m_switches;
}

bool ColumnCurrentQuad::operator!=(ColumnCurrentQuad const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, ColumnCurrentQuad::config_size_in_words> ColumnCurrentQuad::addresses(
    coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx;
	uint32_t base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base = synram_synapse_2msb_bottom_base_address;
	} else {
		base = synram_synapse_2msb_top_base_address;
	}
	auto const address = base +
	                     detail::to_synram_quad_address_offset(block.toSynapseQuadColumnOnDLS()) +
	                     SynapseQuadOnSynram::size * SynapseQuad::config_size_in_words;
	return {{AddressT(address), AddressT((address) + ColumnCurrentQuadOnSynram::size)}};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, ColumnCurrentQuad::config_size_in_words>
    ColumnCurrentQuad::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    ColumnCurrentQuad::config_size_in_words>
ColumnCurrentQuad::addresses(coordinate_type const& coord);

namespace {

struct ColumnCurrentQuadBitfield
{
	union
	{
		std::array<uint32_t, ColumnCurrentQuad::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t                                      : 6;
			uint32_t enable_synaptic_current_excitatory_0 : 1;
			uint32_t enable_synaptic_current_inhibitory_0 : 1;

			uint32_t                                      : 6;
			uint32_t enable_synaptic_current_excitatory_1 : 1;
			uint32_t enable_synaptic_current_inhibitory_1 : 1;

			uint32_t                                      : 6;
			uint32_t enable_synaptic_current_excitatory_2 : 1;
			uint32_t enable_synaptic_current_inhibitory_2 : 1;

			uint32_t                                      : 6;
			uint32_t enable_synaptic_current_excitatory_3 : 1;
			uint32_t enable_synaptic_current_inhibitory_3 : 1;

			uint32_t                                      : 6;
			uint32_t enable_debug_excitatory_0            : 1;
			uint32_t enable_debug_inhibitory_0            : 1;

			uint32_t                                      : 6;
			uint32_t enable_debug_excitatory_1            : 1;
			uint32_t enable_debug_inhibitory_1            : 1;

			uint32_t                                      : 6;
			uint32_t enable_debug_excitatory_2            : 1;
			uint32_t enable_debug_inhibitory_2            : 1;

			uint32_t                                      : 6;
			uint32_t enable_debug_excitatory_3            : 1;
			uint32_t enable_debug_inhibitory_3            : 1;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ColumnCurrentQuadBitfield() { u.raw = {{0}}; }
	ColumnCurrentQuadBitfield(
	    std::array<uint32_t, ColumnCurrentQuad::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, ColumnCurrentQuad::config_size_in_words> ColumnCurrentQuad::encode() const
{
	using namespace halco::hicann_dls::vx;
	ColumnCurrentQuadBitfield bitfield;

#define CURRENT_ENCODE(index)                                                                      \
	{                                                                                              \
		ColumnCurrentSwitch const& config = m_switches.at(EntryOnQuad(index));                     \
		bitfield.u.m.enable_synaptic_current_excitatory_##index =                                  \
		    config.get_enable_synaptic_current_excitatory();                                       \
		bitfield.u.m.enable_synaptic_current_inhibitory_##index =                                  \
		    config.get_enable_synaptic_current_inhibitory();                                       \
		bitfield.u.m.enable_debug_excitatory_##index = config.get_enable_debug_excitatory();       \
		bitfield.u.m.enable_debug_inhibitory_##index = config.get_enable_debug_inhibitory();       \
	}

	CURRENT_ENCODE(0);
	CURRENT_ENCODE(1);
	CURRENT_ENCODE(2);
	CURRENT_ENCODE(3);
#undef CURRENT_ENCODE

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw[0])),
	        WordT(fisch::vx::OmnibusData(bitfield.u.raw[1]))};
}

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, ColumnCurrentQuad::config_size_in_words>
ColumnCurrentQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCurrentQuad::config_size_in_words>
    ColumnCurrentQuad::encode() const;

template <typename WordT>
void ColumnCurrentQuad::decode(
    std::array<WordT, ColumnCurrentQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	ColumnCurrentQuadBitfield bitfield({data[0].get(), data[1].get()});

#define CURRENT_DECODE(index)                                                                      \
	{                                                                                              \
		ColumnCurrentSwitch config;                                                                \
		config.set_enable_synaptic_current_excitatory(                                             \
		    bitfield.u.m.enable_synaptic_current_excitatory_##index);                              \
		config.set_enable_synaptic_current_inhibitory(                                             \
		    bitfield.u.m.enable_synaptic_current_inhibitory_##index);                              \
		config.set_enable_debug_excitatory(bitfield.u.m.enable_debug_excitatory_##index);          \
		config.set_enable_debug_inhibitory(bitfield.u.m.enable_debug_inhibitory_##index);          \
		m_switches.at(EntryOnQuad(index)) = config;                                                \
	}

	CURRENT_DECODE(0)
	CURRENT_DECODE(1)
	CURRENT_DECODE(2)
	CURRENT_DECODE(3)
#undef CURRENT_DECODE
}

template SYMBOL_VISIBLE void ColumnCurrentQuad::decode(
    std::array<fisch::vx::OmnibusChip, ColumnCurrentQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void ColumnCurrentQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCurrentQuad::config_size_in_words> const&
        data);

template <class Archive>
void ColumnCurrentQuad::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_switches));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ColumnCurrentQuad)

SynapseBiasSelection::SynapseBiasSelection() :
    m_int_dac_bias(),
    m_int_ramp_bias(),
    m_int_store_bias(),
    m_int_output_bias()
{
	for (auto coord : ::halco::common::iter_all<::halco::hicann_dls::vx::CapMemBlockOnDLS>()) {
		m_int_dac_bias[coord] = true;
		m_int_ramp_bias[coord] = true;
		m_int_store_bias[coord] = true;
		m_int_output_bias[coord] = true;
	}
}

SynapseBiasSelection::bias_selection_type const&
SynapseBiasSelection::get_enable_internal_dac_bias() const
{
	return m_int_dac_bias;
}

void SynapseBiasSelection::set_enable_internal_dac_bias(bias_selection_type const& value)
{
	m_int_dac_bias = value;
}

SynapseBiasSelection::bias_selection_type const&
SynapseBiasSelection::get_enable_internal_ramp_bias() const
{
	return m_int_ramp_bias;
}

void SynapseBiasSelection::set_enable_internal_ramp_bias(bias_selection_type const& value)
{
	m_int_ramp_bias = value;
}

SynapseBiasSelection::bias_selection_type const&
SynapseBiasSelection::get_enable_internal_store_bias() const
{
	return m_int_store_bias;
}

void SynapseBiasSelection::set_enable_internal_store_bias(bias_selection_type const& value)
{
	m_int_store_bias = value;
}

SynapseBiasSelection::bias_selection_type const&
SynapseBiasSelection::get_enable_internal_output_bias() const
{
	return m_int_output_bias;
}

void SynapseBiasSelection::set_enable_internal_output_bias(bias_selection_type const& value)
{
	m_int_output_bias = value;
}

bool SynapseBiasSelection::operator==(SynapseBiasSelection const& other) const
{
	return (
	    m_int_dac_bias == other.m_int_dac_bias && m_int_ramp_bias == other.m_int_ramp_bias &&
	    m_int_store_bias == other.m_int_store_bias && m_int_output_bias == other.m_int_output_bias);
}

bool SynapseBiasSelection::operator!=(SynapseBiasSelection const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SynapseBiasSelection const& config)
{
	for (auto coord : ::halco::common::iter_all<::halco::hicann_dls::vx::CapMemBlockOnDLS>()) {
		os << "Quadrant " << coord.toEnum() << ":" << std::endl;
		os << "Enable internal synapse DAC bias:               \t" << config.m_int_dac_bias[coord]
		   << std::endl;
		os << "Enable internal synapse ramp bias:              \t" << config.m_int_ramp_bias[coord]
		   << std::endl;
		os << "Enable internal synapse store bias:             \t" << config.m_int_store_bias[coord]
		   << std::endl;
		os << "Enable internal synapse correlation output bias:\t"
		   << config.m_int_output_bias[coord] << std::endl;
	}
	return print_words_for_each_backend(os, config);
}

template <typename AddressT>
std::array<AddressT, SynapseBiasSelection::write_config_size_in_words>
SynapseBiasSelection::write_addresses(coordinate_type const& /*coord*/)
{
	return {AddressT(anncore_center_base_address)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipAddress,
    SynapseBiasSelection::write_config_size_in_words>
SynapseBiasSelection::write_addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SynapseBiasSelection::write_config_size_in_words>
SynapseBiasSelection::write_addresses(coordinate_type const& coord);

template <typename AddressT>
std::array<AddressT, SynapseBiasSelection::read_config_size_in_words>
SynapseBiasSelection::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipAddress,
    SynapseBiasSelection::read_config_size_in_words>
SynapseBiasSelection::read_addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SynapseBiasSelection::read_config_size_in_words>
SynapseBiasSelection::read_addresses(coordinate_type const& coord);
namespace {

struct SynapseBiasSelectionBitfield
{
	union
	{
		uint32_t raw;

		// clang-format off
		struct {
			uint32_t int_output_q1 : 1;
			uint32_t int_store_q1  : 1;
			uint32_t int_ramp_q1   : 1;
			uint32_t int_dac_q1    : 1;
			uint32_t int_output_q0 : 1;
			uint32_t int_store_q0  : 1;
			uint32_t int_ramp_q0   : 1;
			uint32_t int_dac_q0    : 1;
			uint32_t int_output_q3 : 1;
			uint32_t int_store_q3  : 1;
			uint32_t int_ramp_q3   : 1;
			uint32_t int_dac_q3    : 1;
			uint32_t int_output_q2 : 1;
			uint32_t int_store_q2  : 1;
			uint32_t int_ramp_q2   : 1;
			uint32_t int_dac_q2    : 1;
		} m;
		// clang-format on

		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseBiasSelectionBitfield() { u.raw = 0ul; }

	SynapseBiasSelectionBitfield(uint32_t data) { u.raw = data; }
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, SynapseBiasSelection::write_config_size_in_words> SynapseBiasSelection::encode()
    const
{
	SynapseBiasSelectionBitfield bitfield;

	bitfield.u.m.int_dac_q0 = m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.int_dac_q1 = m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.int_dac_q2 = m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.int_dac_q3 = m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];

	bitfield.u.m.int_ramp_q0 = m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.int_ramp_q1 = m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.int_ramp_q2 = m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.int_ramp_q3 = m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];

	bitfield.u.m.int_store_q0 = m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.int_store_q1 = m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.int_store_q2 = m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.int_store_q3 = m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];

	bitfield.u.m.int_output_q0 = m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.int_output_q1 = m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.int_output_q2 = m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.int_output_q3 = m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, SynapseBiasSelection::write_config_size_in_words>
    SynapseBiasSelection::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseBiasSelection::write_config_size_in_words>
    SynapseBiasSelection::encode() const;

template <typename WordT>
void SynapseBiasSelection::decode(
    std::array<WordT, SynapseBiasSelection::read_config_size_in_words> const& /*words*/)
{}

template SYMBOL_VISIBLE void SynapseBiasSelection::decode(
    std::array<fisch::vx::OmnibusChip, SynapseBiasSelection::read_config_size_in_words> const&
        words);
template SYMBOL_VISIBLE void SynapseBiasSelection::decode(
    std::array<
        fisch::vx::OmnibusChipOverJTAG,
        SynapseBiasSelection::read_config_size_in_words> const& words);

template <class Archive>
void SynapseBiasSelection::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_int_dac_bias));
	ar(CEREAL_NVP(m_int_ramp_bias));
	ar(CEREAL_NVP(m_int_store_bias));
	ar(CEREAL_NVP(m_int_output_bias));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseBiasSelection)

CorrelationReset::CorrelationReset() {}

template <typename AddressT>
std::array<AddressT, CorrelationReset::read_config_size_in_words> CorrelationReset::read_addresses(
    CorrelationReset::coordinate_type const& /* neuron */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CorrelationReset::read_config_size_in_words>
CorrelationReset::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipAddress, CorrelationReset::read_config_size_in_words>
    CorrelationReset::read_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& cell);

template <typename AddressT>
std::array<AddressT, CorrelationReset::write_config_size_in_words>
CorrelationReset::write_addresses(CorrelationReset::coordinate_type const& coord)
{
	auto const base_address = correlation_reset_base_addresses.at(coord.toSynramOnDLS());
	int row_offset =
	    coord.toSynapseRowOnSynram().toEnum() * halco::hicann_dls::vx::SynapseQuadColumnOnDLS::size;
	int quad_offset = detail::to_synram_quad_address_offset(coord.toSynapseQuadColumnOnDLS());

	return {AddressT(base_address + row_offset + quad_offset)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CorrelationReset::write_config_size_in_words>
CorrelationReset::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipAddress, CorrelationReset::write_config_size_in_words>
    CorrelationReset::write_addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& cell);

template <typename WordT>
std::array<WordT, CorrelationReset::write_config_size_in_words> CorrelationReset::encode() const
{
	// Value does not matter
	return {WordT()};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CorrelationReset::write_config_size_in_words>
    CorrelationReset::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, CorrelationReset::write_config_size_in_words>
    CorrelationReset::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void CorrelationReset::decode(std::array<WordT, CorrelationReset::read_config_size_in_words> const&)
{}

template SYMBOL_VISIBLE void CorrelationReset::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, CorrelationReset::read_config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void CorrelationReset::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, CorrelationReset::read_config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, CorrelationReset const&)
{
	os << "CorrelationReset()";
	return os;
}

bool CorrelationReset::operator==(CorrelationReset const&) const
{
	return true;
}

bool CorrelationReset::operator!=(CorrelationReset const& other) const
{
	return !(*this == other);
}

template <class Archive>
void CorrelationReset::serialize(Archive&)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CorrelationReset)

} // namespace haldls::vx
