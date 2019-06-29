#include "haldls/vx/synapse.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
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
    halco::hicann_dls::vx::SynapseOnSynapseQuad const& synapse) const
{
	return m_synapses.at(synapse);
}

void SynapseQuad::set_synapse(
    halco::hicann_dls::vx::SynapseOnSynapseQuad const& synapse, SynapseQuad::Synapse const& value)
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
    SynapseQuad::coordinate_type const& block) const
{
	using namespace halco::hicann_dls::vx;
	uint32_t base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base = synram_synapse_bottom_base_address;
	} else {
		base = synram_synapse_top_base_address;
	}
	uint32_t const address_offset =
	    (block.y() * SynapseQuadColumnOnDLS::size * config_size_in_words) + block.x();
	return {{AddressT(base + address_offset),
	         AddressT(base + address_offset + SynapseQuadColumnOnDLS::size)}};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, SynapseQuad::config_size_in_words>
    SynapseQuad::addresses(coordinate_type const& coord) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, SynapseQuad::config_size_in_words>
    SynapseQuad::addresses(coordinate_type const& coord) const;

namespace {

struct SynapseQuadBitfield
{
	union
	{
		std::array<uint32_t, SynapseQuad::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t weight_3         : 6;
			uint32_t time_calib_3     : 2;

			uint32_t weight_2         : 6;
			uint32_t time_calib_2     : 2;

			uint32_t weight_1         : 6;
			uint32_t time_calib_1     : 2;

			uint32_t weight_0         : 6;
			uint32_t time_calib_0     : 2;

			uint32_t address_3        : 6;
			uint32_t amp_calib_3      : 2;

			uint32_t address_2        : 6;
			uint32_t amp_calib_2      : 2;

			uint32_t address_1        : 6;
			uint32_t amp_calib_1      : 2;

			uint32_t address_0        : 6;
			uint32_t amp_calib_0      : 2;

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
		SynapseQuad::Synapse const& config = m_synapses.at(SynapseOnSynapseQuad(index));           \
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
		m_synapses.at(SynapseOnSynapseQuad(index)) = config;                                       \
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

} // namespace haldls::vx
