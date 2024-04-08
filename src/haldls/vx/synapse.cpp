#include "haldls/vx/synapse.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/quad.h"
#include "halco/hicann-dls/vx/synram.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/indent.h"
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
#define WORD \
			(uint32_t pc_conf_west       :  4;) \
			(uint32_t pc_conf_east       :  4;) \
			(uint32_t                    : 24;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t w_conf_west        :  8;) \
			(uint32_t w_conf_east        :  8;) \
			(uint32_t                    : 16;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t wait_ctr_clear     :  3;) \
			(uint32_t                    : 29;)
			EXPAND_WORD(WORD)
#undef WORD
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
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CommonSynramConfig::config_size_in_words>
    CommonSynramConfig::encode() const;
template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CommonSynramConfig::config_size_in_words>
CommonSynramConfig::encode() const;

template <typename WordT>
void CommonSynramConfig::decode(
    std::array<WordT, CommonSynramConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });

	CommonSynramConfigBitfield bitfield(raw_data);
	m_pc_conf_west = PCConf(bitfield.u.m.pc_conf_west);
	m_pc_conf_east = PCConf(bitfield.u.m.pc_conf_east);
	m_w_conf_west = WConf(bitfield.u.m.w_conf_west);
	m_w_conf_east = WConf(bitfield.u.m.w_conf_east);
	m_wait_ctr_clear = WaitCtrClear(bitfield.u.m.wait_ctr_clear);
}

template SYMBOL_VISIBLE void CommonSynramConfig::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CommonSynramConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void CommonSynramConfig::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CommonSynramConfig::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, CommonSynramConfig const& config)
{
	os << "CommonSynramConfig(west: " << config.m_pc_conf_west << ", " << config.m_w_conf_west
	   << ", east: " << config.m_pc_conf_east << ", " << config.m_w_conf_east << ", "
	   << config.m_wait_ctr_clear << ")";
	return os;
}


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
	ss << hate::join(config.m_int_dac_bias.begin(), config.m_int_dac_bias.end(), ", ");
	ss << "]\n";
	ss << "enable_internal_ramp_bias:   \t[";
	ss << hate::join(config.m_int_ramp_bias.begin(), config.m_int_ramp_bias.end(), ", ");
	ss << "]\n";
	ss << "enable_internal_store_bias:  \t[";
	ss << hate::join(config.m_int_store_bias.begin(), config.m_int_store_bias.end(), ", ");
	ss << "]\n";
	ss << "enable_internal_output_bias: \t[";
	ss << hate::join(config.m_int_output_bias.begin(), config.m_int_output_bias.end(), ", ");
	ss << "]\n)";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, SynapseBiasSelection::config_size_in_words> SynapseBiasSelection::addresses(
    coordinate_type const& /*coord*/)
{
	return {AddressT(anncore_center_base_address)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SynapseBiasSelection::config_size_in_words>
    SynapseBiasSelection::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SynapseBiasSelection::config_size_in_words>
SynapseBiasSelection::addresses(coordinate_type const& coord);

namespace {

struct SynapseBiasSelectionBitfield
{
	union
	{
		uint32_t raw;

		// clang-format off
		struct {
#define WORD \
			(uint32_t int_output_q1 : 1;) \
			(uint32_t int_store_q1  : 1;) \
			(uint32_t int_ramp_q1   : 1;) \
			(uint32_t int_dac_q1    : 1;) \
			(uint32_t int_output_q0 : 1;) \
			(uint32_t int_store_q0  : 1;) \
			(uint32_t int_ramp_q0   : 1;) \
			(uint32_t int_dac_q0    : 1;) \
			(uint32_t int_output_q3 : 1;) \
			(uint32_t int_store_q3  : 1;) \
			(uint32_t int_ramp_q3   : 1;) \
			(uint32_t int_dac_q3    : 1;) \
			(uint32_t int_output_q2 : 1;) \
			(uint32_t int_store_q2  : 1;) \
			(uint32_t int_ramp_q2   : 1;) \
			(uint32_t int_dac_q2    : 1;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on

		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseBiasSelectionBitfield() { u.raw = 0ul; }

	SynapseBiasSelectionBitfield(uint32_t data) { u.raw = data; }
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, SynapseBiasSelection::config_size_in_words> SynapseBiasSelection::encode() const
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

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, SynapseBiasSelection::config_size_in_words>
    SynapseBiasSelection::encode() const;
template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    SynapseBiasSelection::config_size_in_words>
SynapseBiasSelection::encode() const;

template <typename WordT>
void SynapseBiasSelection::decode(
    std::array<WordT, SynapseBiasSelection::config_size_in_words> const& words)
{
	SynapseBiasSelectionBitfield bitfield(words.at(0));

	m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] = bitfield.u.m.int_dac_q0;
	m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] = bitfield.u.m.int_dac_q1;
	m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] = bitfield.u.m.int_dac_q2;
	m_int_dac_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] = bitfield.u.m.int_dac_q3;

	m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] = bitfield.u.m.int_ramp_q0;
	m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] = bitfield.u.m.int_ramp_q1;
	m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] = bitfield.u.m.int_ramp_q2;
	m_int_ramp_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] = bitfield.u.m.int_ramp_q3;

	m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] = bitfield.u.m.int_store_q0;
	m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] = bitfield.u.m.int_store_q1;
	m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] = bitfield.u.m.int_store_q2;
	m_int_store_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] = bitfield.u.m.int_store_q3;

	m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] = bitfield.u.m.int_output_q0;
	m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] = bitfield.u.m.int_output_q1;
	m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] = bitfield.u.m.int_output_q2;
	m_int_output_bias[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] = bitfield.u.m.int_output_q3;
}

template SYMBOL_VISIBLE void SynapseBiasSelection::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        SynapseBiasSelection::config_size_in_words> const& words);
template SYMBOL_VISIBLE void SynapseBiasSelection::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        SynapseBiasSelection::config_size_in_words> const& words);


SynapseWeightQuad::SynapseWeightQuad() : m_values() {}

SynapseWeightQuad::values_type const& SynapseWeightQuad::get_values() const
{
	return m_values;
}

void SynapseWeightQuad::set_values(values_type const& value)
{
	m_values = value;
}

bool SynapseWeightQuad::operator==(SynapseWeightQuad const& other) const
{
	return m_values == other.m_values;
}

bool SynapseWeightQuad::operator!=(SynapseWeightQuad const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, SynapseWeightQuad::config_size_in_words> SynapseWeightQuad::addresses(
    SynapseWeightQuad::coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx;
	uint32_t base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base = synram_synapse_weight_bottom_base_address;
	} else {
		base = synram_synapse_weight_top_base_address;
	}
	uint32_t const address_offset =
	    (block.y() * SynapseQuadColumnOnDLS::size * config_size_in_words) + block.x();
	return {AddressT(base + address_offset)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SynapseWeightQuad::config_size_in_words>
    SynapseWeightQuad::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SynapseWeightQuad::config_size_in_words>
SynapseWeightQuad::addresses(coordinate_type const& coord);

namespace {

struct SynapseWeightQuadBitfield
{
	union
	{
		std::array<uint32_t, SynapseWeightQuad::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t value_3 : 6;) \
			(uint32_t         : 2;) \
			                        \
			(uint32_t value_2 : 6;) \
			(uint32_t         : 2;) \
			                        \
			(uint32_t value_1 : 6;) \
			(uint32_t         : 2;) \
			                        \
			(uint32_t value_0 : 6;) \
			(uint32_t         : 2;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseWeightQuadBitfield()
	{
		u.raw = {{0}};
	}
	SynapseWeightQuadBitfield(
	    std::array<uint32_t, SynapseWeightQuad::config_size_in_words> const& data)
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
std::array<WordT, SynapseWeightQuad::config_size_in_words> SynapseWeightQuad::encode() const
{
	using namespace halco::hicann_dls::vx;
	SynapseWeightQuadBitfield bitfield;

	bitfield.u.m.value_0 = weight_permutation(m_values.at(EntryOnQuad(0)));
	bitfield.u.m.value_1 = weight_permutation(m_values.at(EntryOnQuad(1)));
	bitfield.u.m.value_2 = weight_permutation(m_values.at(EntryOnQuad(2)));
	bitfield.u.m.value_3 = weight_permutation(m_values.at(EntryOnQuad(3)));

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, SynapseWeightQuad::config_size_in_words>
    SynapseWeightQuad::encode() const;
template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, SynapseWeightQuad::config_size_in_words>
    SynapseWeightQuad::encode() const;

template <typename WordT>
void SynapseWeightQuad::decode(
    std::array<WordT, SynapseWeightQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });

	SynapseWeightQuadBitfield bitfield(raw_data);

	m_values.at(EntryOnQuad(0)) = Value(weight_permutation(bitfield.u.m.value_0));
	m_values.at(EntryOnQuad(1)) = Value(weight_permutation(bitfield.u.m.value_1));
	m_values.at(EntryOnQuad(2)) = Value(weight_permutation(bitfield.u.m.value_2));
	m_values.at(EntryOnQuad(3)) = Value(weight_permutation(bitfield.u.m.value_3));
}

template SYMBOL_VISIBLE void SynapseWeightQuad::decode(
    std::array<fisch::vx::word_access_type::Omnibus, SynapseWeightQuad::config_size_in_words> const&
        data);
template SYMBOL_VISIBLE void SynapseWeightQuad::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        SynapseWeightQuad::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, SynapseWeightQuad const& config)
{
	os << "SynapseWeightQuad(" << hate::join(config.m_values, ", ") << ")";
	return os;
}


SynapseLabelQuad::SynapseLabelQuad() : m_values() {}

SynapseLabelQuad::values_type const& SynapseLabelQuad::get_values() const
{
	return m_values;
}

void SynapseLabelQuad::set_values(values_type const& value)
{
	m_values = value;
}

bool SynapseLabelQuad::operator==(SynapseLabelQuad const& other) const
{
	return m_values == other.m_values;
}

bool SynapseLabelQuad::operator!=(SynapseLabelQuad const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, SynapseLabelQuad::config_size_in_words> SynapseLabelQuad::addresses(
    SynapseLabelQuad::coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx;
	uint32_t base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base = synram_synapse_label_bottom_base_address;
	} else {
		base = synram_synapse_label_top_base_address;
	}
	uint32_t const address_offset =
	    (block.y() * SynapseQuadColumnOnDLS::size * config_size_in_words) + block.x();
	return {AddressT(base + address_offset)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SynapseLabelQuad::config_size_in_words>
    SynapseLabelQuad::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, SynapseLabelQuad::config_size_in_words>
    SynapseLabelQuad::addresses(coordinate_type const& coord);

namespace {

struct SynapseLabelQuadBitfield
{
	union
	{
		std::array<uint32_t, SynapseLabelQuad::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t value_3 : 6;) \
			(uint32_t         : 2;) \
			                        \
			(uint32_t value_2 : 6;) \
			(uint32_t         : 2;) \
			                        \
			(uint32_t value_1 : 6;) \
			(uint32_t         : 2;) \
			                        \
			(uint32_t value_0 : 6;) \
			(uint32_t         : 2;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseLabelQuadBitfield()
	{
		u.raw = {{0}};
	}
	SynapseLabelQuadBitfield(
	    std::array<uint32_t, SynapseLabelQuad::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, SynapseLabelQuad::config_size_in_words> SynapseLabelQuad::encode() const
{
	using namespace halco::hicann_dls::vx;
	SynapseLabelQuadBitfield bitfield;

	bitfield.u.m.value_0 = m_values.at(EntryOnQuad(0));
	bitfield.u.m.value_1 = m_values.at(EntryOnQuad(1));
	bitfield.u.m.value_2 = m_values.at(EntryOnQuad(2));
	bitfield.u.m.value_3 = m_values.at(EntryOnQuad(3));

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, SynapseLabelQuad::config_size_in_words>
    SynapseLabelQuad::encode() const;
template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, SynapseLabelQuad::config_size_in_words>
    SynapseLabelQuad::encode() const;

template <typename WordT>
void SynapseLabelQuad::decode(std::array<WordT, SynapseLabelQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });

	SynapseLabelQuadBitfield bitfield(raw_data);

	m_values.at(EntryOnQuad(0)) = Value(bitfield.u.m.value_0);
	m_values.at(EntryOnQuad(1)) = Value(bitfield.u.m.value_1);
	m_values.at(EntryOnQuad(2)) = Value(bitfield.u.m.value_2);
	m_values.at(EntryOnQuad(3)) = Value(bitfield.u.m.value_3);
}

template SYMBOL_VISIBLE void SynapseLabelQuad::decode(
    std::array<fisch::vx::word_access_type::Omnibus, SynapseLabelQuad::config_size_in_words> const&
        data);
template SYMBOL_VISIBLE void SynapseLabelQuad::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        SynapseLabelQuad::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, SynapseLabelQuad const& config)
{
	os << "SynapseLabelQuad(" << hate::join(config.m_values, ", ") << ")";
	return os;
}


SynapseCorrelationCalibQuad::SynapseCorrelationCalibQuad() : m_time_calibs(), m_amp_calibs() {}

SynapseCorrelationCalibQuad::time_calibs_type const& SynapseCorrelationCalibQuad::get_time_calibs()
    const
{
	return m_time_calibs;
}

void SynapseCorrelationCalibQuad::set_time_calibs(time_calibs_type const& value)
{
	m_time_calibs = value;
}

SynapseCorrelationCalibQuad::amp_calibs_type const& SynapseCorrelationCalibQuad::get_amp_calibs()
    const
{
	return m_amp_calibs;
}

void SynapseCorrelationCalibQuad::set_amp_calibs(amp_calibs_type const& value)
{
	m_amp_calibs = value;
}

bool SynapseCorrelationCalibQuad::operator==(SynapseCorrelationCalibQuad const& other) const
{
	return m_time_calibs == other.m_time_calibs && m_amp_calibs == other.m_amp_calibs;
}

bool SynapseCorrelationCalibQuad::operator!=(SynapseCorrelationCalibQuad const& other) const
{
	return !(*this == other);
}

template <typename AddressT>
std::array<AddressT, SynapseCorrelationCalibQuad::config_size_in_words>
SynapseCorrelationCalibQuad::addresses(SynapseCorrelationCalibQuad::coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx;
	uint32_t base_even;
	uint32_t base_odd;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base_even = synram_synapse_even_2msb_bottom_base_address;
		base_odd = synram_synapse_odd_2msb_bottom_base_address;
	} else {
		base_even = synram_synapse_even_2msb_top_base_address;
		base_odd = synram_synapse_odd_2msb_top_base_address;
	}
	uint32_t const address_offset = (block.y() * SynapseQuadColumnOnDLS::size) + block.x();
	return {AddressT(base_even + address_offset), AddressT(base_odd + address_offset)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusAddress, SynapseCorrelationCalibQuad::config_size_in_words>
    SynapseCorrelationCalibQuad::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SynapseCorrelationCalibQuad::config_size_in_words>
SynapseCorrelationCalibQuad::addresses(coordinate_type const& coord);

namespace {

struct SynapseCorrelationCalibQuadBitfield
{
	union
	{
		std::array<uint32_t, SynapseCorrelationCalibQuad::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t time_calib_3 : 2;) \
			(uint32_t              : 6;) \
			                             \
			(uint32_t time_calib_2 : 2;) \
			(uint32_t              : 6;) \
			                             \
			(uint32_t time_calib_1 : 2;) \
			(uint32_t              : 6;) \
			                             \
			(uint32_t time_calib_0 : 2;) \
			(uint32_t              : 6;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t amp_calib_3  : 2;) \
			(uint32_t              : 6;) \
			                             \
			(uint32_t amp_calib_2  : 2;) \
			(uint32_t              : 6;) \
			                             \
			(uint32_t amp_calib_1  : 2;) \
			(uint32_t              : 6;) \
			                             \
			(uint32_t amp_calib_0  : 2;) \
			(uint32_t              : 6;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseCorrelationCalibQuadBitfield()
	{
		u.raw = {{0}};
	}
	SynapseCorrelationCalibQuadBitfield(
	    std::array<uint32_t, SynapseCorrelationCalibQuad::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, SynapseCorrelationCalibQuad::config_size_in_words>
SynapseCorrelationCalibQuad::encode() const
{
	using namespace halco::hicann_dls::vx;
	SynapseCorrelationCalibQuadBitfield bitfield;

#define SYNAPSE_ENCODE(index)                                                                      \
	{                                                                                              \
		bitfield.u.m.time_calib_##index = m_time_calibs.at(EntryOnQuad(index));                    \
		bitfield.u.m.amp_calib_##index = m_amp_calibs.at(EntryOnQuad(index));                      \
	}

	SYNAPSE_ENCODE(0)
	SYNAPSE_ENCODE(1)
	SYNAPSE_ENCODE(2)
	SYNAPSE_ENCODE(3)
#undef SYNAPSE_ENCODE

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::Omnibus, SynapseCorrelationCalibQuad::config_size_in_words>
    SynapseCorrelationCalibQuad::encode() const;
template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    SynapseCorrelationCalibQuad::config_size_in_words>
SynapseCorrelationCalibQuad::encode() const;

template <typename WordT>
void SynapseCorrelationCalibQuad::decode(
    std::array<WordT, SynapseCorrelationCalibQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });

	SynapseCorrelationCalibQuadBitfield bitfield(raw_data);

#define SYNAPSE_DECODE(index)                                                                      \
	{                                                                                              \
		m_time_calibs.at(EntryOnQuad(index)) = TimeCalib(bitfield.u.m.time_calib_##index);         \
		m_amp_calibs.at(EntryOnQuad(index)) = AmpCalib(bitfield.u.m.amp_calib_##index);            \
	}

	SYNAPSE_DECODE(0)
	SYNAPSE_DECODE(1)
	SYNAPSE_DECODE(2)
	SYNAPSE_DECODE(3)
#undef SYNAPSE_DECODE
}

template SYMBOL_VISIBLE void SynapseCorrelationCalibQuad::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        SynapseCorrelationCalibQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void SynapseCorrelationCalibQuad::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        SynapseCorrelationCalibQuad::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, SynapseCorrelationCalibQuad const& config)
{
	os << "SynapseCorrelationCalibQuad(\n"
	   << "\t" << hate::join(config.m_time_calibs, ", ") << "\n"
	   << "\t" << hate::join(config.m_amp_calibs, ", ") << "\n)";
	return os;
}


SynapseQuad::SynapseQuad() : m_weights(), m_labels(), m_time_calibs(), m_amp_calibs() {}

SynapseQuad::weights_type const& SynapseQuad::get_weights() const
{
	return m_weights;
}

void SynapseQuad::set_weights(weights_type const& value)
{
	m_weights = value;
}

SynapseQuad::labels_type const& SynapseQuad::get_labels() const
{
	return m_labels;
}

void SynapseQuad::set_labels(labels_type const& value)
{
	m_labels = value;
}

SynapseQuad::time_calibs_type const& SynapseQuad::get_time_calibs() const
{
	return m_time_calibs;
}

void SynapseQuad::set_time_calibs(time_calibs_type const& value)
{
	m_time_calibs = value;
}

SynapseQuad::amp_calibs_type const& SynapseQuad::get_amp_calibs() const
{
	return m_amp_calibs;
}

void SynapseQuad::set_amp_calibs(amp_calibs_type const& value)
{
	m_amp_calibs = value;
}

bool SynapseQuad::operator==(SynapseQuad const& other) const
{
	return m_weights == other.m_weights && m_labels == other.m_labels &&
	       m_time_calibs == other.m_time_calibs && m_amp_calibs == other.m_amp_calibs;
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
	    (block.y() * SynapseQuadColumnOnDLS::size * config_size_in_words) + block.x();
	return {
	    {AddressT(base + address_offset),
	     AddressT(base + address_offset + SynapseQuadColumnOnDLS::size)}};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SynapseQuad::config_size_in_words>
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
#define WORD \
			(uint32_t weight_3         : 6;) \
			(uint32_t time_calib_3     : 2;) \
			                                 \
			(uint32_t weight_2         : 6;) \
			(uint32_t time_calib_2     : 2;) \
			                                 \
			(uint32_t weight_1         : 6;) \
			(uint32_t time_calib_1     : 2;) \
			                                 \
			(uint32_t weight_0         : 6;) \
			(uint32_t time_calib_0     : 2;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t label_3          : 6;) \
			(uint32_t amp_calib_3      : 2;) \
			                                 \
			(uint32_t label_2          : 6;) \
			(uint32_t amp_calib_2      : 2;) \
			                                 \
			(uint32_t label_1          : 6;) \
			(uint32_t amp_calib_1      : 2;) \
			                                 \
			(uint32_t label_0          : 6;) \
			(uint32_t amp_calib_0      : 2;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SynapseQuadBitfield()
	{
		u.raw = {{0}};
	}
	SynapseQuadBitfield(std::array<uint32_t, SynapseQuad::config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, SynapseQuad::config_size_in_words> SynapseQuad::encode() const
{
	using namespace halco::hicann_dls::vx;
	SynapseQuadBitfield bitfield;

#define SYNAPSE_ENCODE(index)                                                                      \
	{                                                                                              \
		EntryOnQuad const entry(index);                                                            \
		bitfield.u.m.time_calib_##index = m_time_calibs.at(entry);                                 \
		bitfield.u.m.weight_##index = weight_permutation(m_weights.at(entry));                     \
		bitfield.u.m.amp_calib_##index = m_amp_calibs.at(entry);                                   \
		bitfield.u.m.label_##index = m_labels.at(entry);                                           \
	}

	SYNAPSE_ENCODE(0)
	SYNAPSE_ENCODE(1)
	SYNAPSE_ENCODE(2)
	SYNAPSE_ENCODE(3)
#undef SYNAPSE_ENCODE

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, SynapseQuad::config_size_in_words>
    SynapseQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, SynapseQuad::config_size_in_words>
    SynapseQuad::encode() const;

template <typename WordT>
void SynapseQuad::decode(std::array<WordT, SynapseQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });

	SynapseQuadBitfield bitfield(raw_data);

#define SYNAPSE_DECODE(index)                                                                      \
	{                                                                                              \
		EntryOnQuad const entry(index);                                                            \
		m_time_calibs.at(entry) = TimeCalib(bitfield.u.m.time_calib_##index);                      \
		m_weights.at(entry) = Weight(weight_permutation(bitfield.u.m.weight_##index));             \
		m_amp_calibs.at(entry) = AmpCalib(bitfield.u.m.amp_calib_##index);                         \
		m_labels.at(entry) = Label(bitfield.u.m.label_##index);                                    \
	}

	SYNAPSE_DECODE(0)
	SYNAPSE_DECODE(1)
	SYNAPSE_DECODE(2)
	SYNAPSE_DECODE(3)
#undef SYNAPSE_DECODE
}

template SYMBOL_VISIBLE void SynapseQuad::decode(
    std::array<fisch::vx::word_access_type::Omnibus, SynapseQuad::config_size_in_words> const&
        data);
template SYMBOL_VISIBLE void SynapseQuad::decode(std::array<
                                                 fisch::vx::word_access_type::OmnibusChipOverJTAG,
                                                 SynapseQuad::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, SynapseQuad const& config)
{
	os << "SynapseQuad(\n"
	   << "\t" << hate::join(config.m_weights, ", ") << "\n"
	   << "\t" << hate::join(config.m_labels, ", ") << "\n"
	   << "\t" << hate::join(config.m_time_calibs, ", ") << "\n"
	   << "\t" << hate::join(config.m_amp_calibs, ", ") << "\n)";
	return os;
}


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

std::ostream& operator<<(std::ostream& os, ColumnCurrentQuad::ColumnCurrentSwitch const& config)
{
	std::stringstream ss;
	ss << "ColumnCurrentSwitch(\n" << std::boolalpha;
	ss << "\tenable_synaptic_current_excitatory: \t" << config.m_enable_synaptic_current_excitatory
	   << "\n";
	ss << "\tenable_synaptic_current_excitatory: \t" << config.m_enable_synaptic_current_excitatory
	   << "\n";
	ss << "\tenable_debug_excitatory:            \t" << config.m_enable_debug_excitatory << "\n";
	ss << "\tenable_debug_excitatory:            \t" << config.m_enable_debug_excitatory << "\n)";
	return (os << ss.str());
}

std::ostream& operator<<(std::ostream& os, ColumnCurrentQuad const& config)
{
	hate::IndentingOstream ios(os);
	ios << "ColumnCurrentQuad(\n"
	    << hate::Indentation("\t") << hate::join(config.m_switches, "\n") << "\n"
	    << hate::Indentation() << ")";
	return os;
}

bool ColumnCorrelationQuad::operator==(ColumnCorrelationQuad const& other) const
{
	return m_switches == other.m_switches;
}

bool ColumnCorrelationQuad::operator!=(ColumnCorrelationQuad const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(
    std::ostream& os, ColumnCorrelationQuad::ColumnCorrelationSwitch const& config)
{
	std::stringstream ss;
	ss << "ColumnCorrelationSwitch(\n" << std::boolalpha;
	ss << "\tenable_internal_causal:             \t" << config.m_enable_internal_causal << "\n";
	ss << "\tenable_internal_acausal:            \t" << config.m_enable_internal_acausal << "\n";
	ss << "\tenable_debug_causal:                \t" << config.m_enable_debug_causal << "\n";
	ss << "\tenable_debug_acausal:               \t" << config.m_enable_debug_acausal << "\n";
	ss << "\tenable_cadc_neuron_readout_causal:  \t" << config.m_enable_cadc_neuron_readout_causal
	   << "\n";
	ss << "\tenable_cadc_neuron_readout_acausal: \t" << config.m_enable_cadc_neuron_readout_acausal
	   << "\n)";
	return (os << ss.str());
}

std::ostream& operator<<(std::ostream& os, ColumnCorrelationQuad const& config)
{
	hate::IndentingOstream ios(os);
	ios << "ColumnCorrelationQuad(\n"
	    << hate::Indentation("\t") << hate::join(config.m_switches, "\n") << "\n"
	    << hate::Indentation() << ")";
	return os;
}

template <typename AddressT>
std::array<AddressT, ColumnCorrelationQuad::config_size_in_words> ColumnCorrelationQuad::addresses(
    coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx;
	uint32_t raw_base;
	uint32_t lsb_weight_base;
	uint32_t lsb_label_base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		raw_base = synram_synapse_bottom_base_address;
		lsb_weight_base = synram_synapse_weight_bottom_base_address;
		lsb_label_base = synram_synapse_label_bottom_base_address;
	} else {
		raw_base = synram_synapse_top_base_address;
		lsb_weight_base = synram_synapse_weight_top_base_address;
		lsb_label_base = synram_synapse_label_top_base_address;
	}
	auto const address_0 =
	    raw_base + block.toSynapseQuadColumnOnDLS() +
	    SynapseQuadOnSynram::size * SynapseQuad::config_size_in_words +
	    ColumnCurrentQuadOnSynram::size * ColumnCurrentQuad::config_size_in_words;
	auto const address_1 =
	    lsb_weight_base + block.toSynapseQuadColumnOnDLS() + SynapseQuadOnSynram::size;
	auto const address_2 =
	    lsb_label_base + block.toSynapseQuadColumnOnDLS() + SynapseQuadOnSynram::size;
	return {
	    {AddressT(address_0), AddressT((address_0) + ColumnCorrelationQuadOnSynram::size),
	     AddressT(address_1), AddressT(address_2)}};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, ColumnCorrelationQuad::config_size_in_words>
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
#define WORD \
			(uint32_t                                      : 6;) \
			(uint32_t enable_internal_causal_3             : 1;) \
			(uint32_t enable_internal_acausal_3            : 1;) \
			                                                     \
			(uint32_t                                      : 6;) \
			(uint32_t enable_internal_causal_2             : 1;) \
			(uint32_t enable_internal_acausal_2            : 1;) \
			                                                     \
			(uint32_t                                      : 6;) \
			(uint32_t enable_internal_causal_1             : 1;) \
			(uint32_t enable_internal_acausal_1            : 1;) \
			                                                     \
			(uint32_t                                      : 6;) \
			(uint32_t enable_internal_causal_0             : 1;) \
			(uint32_t enable_internal_acausal_0            : 1;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t                                      : 6;) \
			(uint32_t enable_debug_causal_3                : 1;) \
			(uint32_t enable_debug_acausal_3               : 1;) \
			                                                     \
			(uint32_t                                      : 6;) \
			(uint32_t enable_debug_causal_2                : 1;) \
			(uint32_t enable_debug_acausal_2               : 1;) \
			                                                     \
			(uint32_t                                      : 6;) \
			(uint32_t enable_debug_causal_1                : 1;) \
			(uint32_t enable_debug_acausal_1               : 1;) \
			                                                     \
			(uint32_t                                      : 6;) \
			(uint32_t enable_debug_causal_0                : 1;) \
			(uint32_t enable_debug_acausal_0               : 1;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_causal_3  : 1;) \
			(uint32_t                                      : 2;) \
			                                                     \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_causal_2  : 1;) \
			(uint32_t                                      : 2;) \
			                                                     \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_causal_1  : 1;) \
			(uint32_t                                      : 2;) \
			                                                     \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_causal_0  : 1;) \
			(uint32_t                                      : 2;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_acausal_3 : 1;) \
			(uint32_t                                      : 2;) \
			                                                     \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_acausal_2 : 1;) \
			(uint32_t                                      : 2;) \
			                                                     \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_acausal_1 : 1;) \
			(uint32_t                                      : 2;) \
			                                                     \
			(uint32_t                                      : 5;) \
			(uint32_t enable_cadc_neuron_readout_acausal_0 : 1;) \
			(uint32_t                                      : 2;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ColumnCorrelationQuadBitfield()
	{
		u.raw = {{0}};
	}
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

	return {
	    WordT(bitfield.u.raw[0]), WordT(bitfield.u.raw[1]), WordT(bitfield.u.raw[2]),
	    WordT(bitfield.u.raw[3])};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, ColumnCorrelationQuad::config_size_in_words>
    ColumnCorrelationQuad::encode() const;
template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    ColumnCorrelationQuad::config_size_in_words>
ColumnCorrelationQuad::encode() const;

template <typename WordT>
void ColumnCorrelationQuad::decode(
    std::array<WordT, ColumnCorrelationQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	ColumnCorrelationQuadBitfield bitfield({data[0], data[1], data[2], data[3]});

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
    std::array<
        fisch::vx::word_access_type::Omnibus,
        ColumnCorrelationQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void ColumnCorrelationQuad::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        ColumnCorrelationQuad::config_size_in_words> const& data);


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
	uint32_t base_even;
	uint32_t base_odd;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base_even = synram_synapse_even_2msb_bottom_base_address;
		base_odd = synram_synapse_odd_2msb_bottom_base_address;
	} else {
		base_even = synram_synapse_even_2msb_top_base_address;
		base_odd = synram_synapse_odd_2msb_top_base_address;
	}
	auto const offset = block.toSynapseQuadColumnOnDLS() + SynapseQuadOnSynram::size;
	return {{AddressT(base_even + offset), AddressT(base_odd + offset)}};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, ColumnCurrentQuad::config_size_in_words>
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
#define WORD \
			(uint32_t enable_synaptic_current_excitatory_3 : 1;) \
			(uint32_t enable_synaptic_current_inhibitory_3 : 1;) \
			(uint32_t                                      : 6;) \
			                                                     \
			(uint32_t enable_synaptic_current_excitatory_2 : 1;) \
			(uint32_t enable_synaptic_current_inhibitory_2 : 1;) \
			(uint32_t                                      : 6;) \
			                                                     \
			(uint32_t enable_synaptic_current_excitatory_1 : 1;) \
			(uint32_t enable_synaptic_current_inhibitory_1 : 1;) \
			(uint32_t                                      : 6;) \
			                                                     \
			(uint32_t enable_synaptic_current_excitatory_0 : 1;) \
			(uint32_t enable_synaptic_current_inhibitory_0 : 1;) \
			(uint32_t                                      : 6;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t enable_debug_excitatory_3            : 1;) \
			(uint32_t enable_debug_inhibitory_3            : 1;) \
			(uint32_t                                      : 6;) \
			                                                     \
			(uint32_t enable_debug_excitatory_2            : 1;) \
			(uint32_t enable_debug_inhibitory_2            : 1;) \
			(uint32_t                                      : 6;) \
			                                                     \
			(uint32_t enable_debug_excitatory_1            : 1;) \
			(uint32_t enable_debug_inhibitory_1            : 1;) \
			(uint32_t                                      : 6;) \
			                                                     \
			(uint32_t enable_debug_excitatory_0            : 1;) \
			(uint32_t enable_debug_inhibitory_0            : 1;) \
			(uint32_t                                      : 6;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ColumnCurrentQuadBitfield()
	{
		u.raw = {{0}};
	}
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

	return {WordT(bitfield.u.raw[0]), WordT(bitfield.u.raw[1])};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, ColumnCurrentQuad::config_size_in_words>
    ColumnCurrentQuad::encode() const;
template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, ColumnCurrentQuad::config_size_in_words>
    ColumnCurrentQuad::encode() const;

template <typename WordT>
void ColumnCurrentQuad::decode(
    std::array<WordT, ColumnCurrentQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	ColumnCurrentQuadBitfield bitfield({data[0], data[1]});

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
    std::array<fisch::vx::word_access_type::Omnibus, ColumnCurrentQuad::config_size_in_words> const&
        data);
template SYMBOL_VISIBLE void ColumnCurrentQuad::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        ColumnCurrentQuad::config_size_in_words> const& data);


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

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CorrelationReset::read_config_size_in_words>
    CorrelationReset::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename AddressT>
std::array<AddressT, CorrelationReset::write_config_size_in_words>
CorrelationReset::write_addresses(CorrelationReset::coordinate_type const& coord)
{
	auto const base_address = correlation_reset_base_addresses.at(coord.toSynramOnDLS());
	int row_offset =
	    coord.toSynapseRowOnSynram().toEnum() * halco::hicann_dls::vx::SynapseQuadColumnOnDLS::size;
	int quad_offset = coord.toSynapseQuadColumnOnDLS();

	return {AddressT(base_address + row_offset + quad_offset)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CorrelationReset::write_config_size_in_words>
CorrelationReset::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CorrelationReset::write_config_size_in_words>
    CorrelationReset::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename WordT>
std::array<WordT, CorrelationReset::write_config_size_in_words> CorrelationReset::encode() const
{
	// Value does not matter
	return {WordT()};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CorrelationReset::write_config_size_in_words>
CorrelationReset::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CorrelationReset::write_config_size_in_words>
    CorrelationReset::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CorrelationReset::decode(std::array<WordT, CorrelationReset::read_config_size_in_words> const&)
{}

template SYMBOL_VISIBLE void
CorrelationReset::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CorrelationReset::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void CorrelationReset::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CorrelationReset::read_config_size_in_words> const& data);

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

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CommonSynramConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SynapseWeightQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SynapseLabelQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SynapseCorrelationCalibQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SynapseQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ColumnCorrelationQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ColumnCurrentQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CorrelationReset)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SynapseBiasSelection)
