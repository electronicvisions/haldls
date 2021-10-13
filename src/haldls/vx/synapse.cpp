#include "haldls/vx/synapse.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"

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
    std::array<halco::hicann_dls::vx::OmnibusAddress, CommonSynramConfig::config_size_in_words>
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
	    [](uint32_t const& w) { return static_cast<WordT>(typename WordT::Value(w)); });
	return data;
}

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, CommonSynramConfig::config_size_in_words>
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
    std::array<fisch::vx::Omnibus, CommonSynramConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void CommonSynramConfig::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, CommonSynramConfig::config_size_in_words> const&
        data);

std::ostream& operator<<(std::ostream& os, CommonSynramConfig const& config)
{
	os << "CommonSynramConfig(west: " << config.m_pc_conf_west << ", " << config.m_w_conf_west
	   << ", east: " << config.m_pc_conf_east << ", " << config.m_w_conf_east << ", "
	   << config.m_wait_ctr_clear << ")";
	return os;
}

template <class Archive>
void CommonSynramConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_pc_conf_west));
	ar(CEREAL_NVP(m_pc_conf_east));
	ar(CEREAL_NVP(m_w_conf_west));
	ar(CEREAL_NVP(m_w_conf_east));
	ar(CEREAL_NVP(m_wait_ctr_clear));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CommonSynramConfig)


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
	std::stringstream ss;
	ss << "SynapseBiasSelection(\n" << std::boolalpha;
	ss << "enable_internal_dac_bias:    \t[";
	hate::join(ss, config.m_int_dac_bias.begin(), config.m_int_dac_bias.end(), ", ");
	ss << "]\n";
	ss << "enable_internal_ramp_bias:   \t[";
	hate::join(ss, config.m_int_ramp_bias.begin(), config.m_int_ramp_bias.end(), ", ");
	ss << "]\n";
	ss << "enable_internal_store_bias:  \t[";
	hate::join(ss, config.m_int_store_bias.begin(), config.m_int_store_bias.end(), ", ");
	ss << "]\n";
	ss << "enable_internal_output_bias: \t[";
	hate::join(ss, config.m_int_output_bias.begin(), config.m_int_output_bias.end(), ", ");
	ss << "]\n)";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, SynapseBiasSelection::write_config_size_in_words>
SynapseBiasSelection::write_addresses(coordinate_type const& /*coord*/)
{
	return {AddressT(anncore_center_base_address)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusAddress, SynapseBiasSelection::write_config_size_in_words>
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

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusAddress, SynapseBiasSelection::read_config_size_in_words>
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

	return {WordT(typename WordT::Value(bitfield.u.raw))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::Omnibus, SynapseBiasSelection::write_config_size_in_words>
    SynapseBiasSelection::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseBiasSelection::write_config_size_in_words>
    SynapseBiasSelection::encode() const;

template <typename WordT>
void SynapseBiasSelection::decode(
    std::array<WordT, SynapseBiasSelection::read_config_size_in_words> const& /*words*/)
{}

template SYMBOL_VISIBLE void SynapseBiasSelection::decode(
    std::array<fisch::vx::Omnibus, SynapseBiasSelection::read_config_size_in_words> const& words);
template SYMBOL_VISIBLE void SynapseBiasSelection::decode(
    std::array<
        fisch::vx::OmnibusChipOverJTAG,
        SynapseBiasSelection::read_config_size_in_words> const& words);

template <class Archive>
void SynapseBiasSelection::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_int_dac_bias));
	ar(CEREAL_NVP(m_int_ramp_bias));
	ar(CEREAL_NVP(m_int_store_bias));
	ar(CEREAL_NVP(m_int_output_bias));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseBiasSelection)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::CommonSynramConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::SynapseBiasSelection, 0)
