#include "haldls/vx/synapse.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"
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
    coordinate_type const& coord) const
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
    CommonSynramConfig::addresses(coordinate_type const& coord) const;

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CommonSynramConfig::config_size_in_words>
CommonSynramConfig::addresses(coordinate_type const& coord) const;

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

} // namespace haldls::vx
