#include "haldls/vx/v1/synapse.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/print.tcc"
#include "haldls/vx/v1/address_transformation.h"
#include "haldls/vx/v1/omnibus_constants.h"

namespace haldls::vx::v1 {

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
	using namespace halco::hicann_dls::vx::v1;
	uint32_t base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base = synram_synapse_weight_bottom_base_address;
	} else {
		base = synram_synapse_weight_top_base_address;
	}
	uint32_t const address_offset =
	    (block.y() * SynapseQuadColumnOnDLS::size * config_size_in_words) +
	    detail::to_synram_quad_address_offset(block.x());
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
			uint32_t value_0 : 6;
			uint32_t         : 2;

			uint32_t value_1 : 6;
			uint32_t         : 2;

			uint32_t value_2 : 6;
			uint32_t         : 2;

			uint32_t value_3 : 6;
			uint32_t         : 2;
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
	using namespace halco::hicann_dls::vx::v1;
	SynapseWeightQuadBitfield bitfield;

	bitfield.u.m.value_0 = weight_permutation(m_values.at(EntryOnQuad(0)));
	bitfield.u.m.value_1 = weight_permutation(m_values.at(EntryOnQuad(1)));
	bitfield.u.m.value_2 = weight_permutation(m_values.at(EntryOnQuad(2)));
	bitfield.u.m.value_3 = weight_permutation(m_values.at(EntryOnQuad(3)));

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, SynapseWeightQuad::config_size_in_words>
SynapseWeightQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseWeightQuad::config_size_in_words>
    SynapseWeightQuad::encode() const;

template <typename WordT>
void SynapseWeightQuad::decode(
    std::array<WordT, SynapseWeightQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx::v1;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });

	SynapseWeightQuadBitfield bitfield(raw_data);

	m_values.at(EntryOnQuad(0)) = Value(weight_permutation(bitfield.u.m.value_0));
	m_values.at(EntryOnQuad(1)) = Value(weight_permutation(bitfield.u.m.value_1));
	m_values.at(EntryOnQuad(2)) = Value(weight_permutation(bitfield.u.m.value_2));
	m_values.at(EntryOnQuad(3)) = Value(weight_permutation(bitfield.u.m.value_3));
}

template SYMBOL_VISIBLE void SynapseWeightQuad::decode(
    std::array<fisch::vx::Omnibus, SynapseWeightQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void SynapseWeightQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseWeightQuad::config_size_in_words> const&
        data);

HALDLS_VX_DEFAULT_OSTREAM_OP(SynapseWeightQuad)

template <class Archive>
void SynapseWeightQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_values));
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
	using namespace halco::hicann_dls::vx::v1;
	uint32_t base;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base = synram_synapse_label_bottom_base_address;
	} else {
		base = synram_synapse_label_top_base_address;
	}
	uint32_t const address_offset =
	    (block.y() * SynapseQuadColumnOnDLS::size * config_size_in_words) +
	    detail::to_synram_quad_address_offset(block.x());
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
			uint32_t value_0 : 6;
			uint32_t         : 2;

			uint32_t value_1 : 6;
			uint32_t         : 2;

			uint32_t value_2 : 6;
			uint32_t         : 2;

			uint32_t value_3 : 6;
			uint32_t         : 2;
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
	using namespace halco::hicann_dls::vx::v1;
	SynapseLabelQuadBitfield bitfield;

	bitfield.u.m.value_0 = m_values.at(EntryOnQuad(0));
	bitfield.u.m.value_1 = m_values.at(EntryOnQuad(1));
	bitfield.u.m.value_2 = m_values.at(EntryOnQuad(2));
	bitfield.u.m.value_3 = m_values.at(EntryOnQuad(3));

	std::array<WordT, config_size_in_words> data;
	std::transform(
	    bitfield.u.raw.begin(), bitfield.u.raw.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, SynapseLabelQuad::config_size_in_words>
SynapseLabelQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseLabelQuad::config_size_in_words>
    SynapseLabelQuad::encode() const;

template <typename WordT>
void SynapseLabelQuad::decode(std::array<WordT, SynapseLabelQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx::v1;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });

	SynapseLabelQuadBitfield bitfield(raw_data);

	m_values.at(EntryOnQuad(0)) = Value(bitfield.u.m.value_0);
	m_values.at(EntryOnQuad(1)) = Value(bitfield.u.m.value_1);
	m_values.at(EntryOnQuad(2)) = Value(bitfield.u.m.value_2);
	m_values.at(EntryOnQuad(3)) = Value(bitfield.u.m.value_3);
}

template SYMBOL_VISIBLE void SynapseLabelQuad::decode(
    std::array<fisch::vx::Omnibus, SynapseLabelQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void SynapseLabelQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseLabelQuad::config_size_in_words> const& data);

HALDLS_VX_DEFAULT_OSTREAM_OP(SynapseLabelQuad)

template <class Archive>
void SynapseLabelQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_values));
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
	using namespace halco::hicann_dls::vx::v1;
	uint32_t base_even;
	uint32_t base_odd;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base_even = synram_synapse_even_2msb_bottom_base_address;
		base_odd = synram_synapse_odd_2msb_bottom_base_address;
	} else {
		base_even = synram_synapse_even_2msb_top_base_address;
		base_odd = synram_synapse_odd_2msb_top_base_address;
	}
	uint32_t const address_offset = (block.y() * SynapseQuadColumnOnDLS::size) +
	                                detail::to_synram_quad_address_offset(block.x());
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
			uint32_t time_calib_0     : 2;
			uint32_t                  : 6;

			uint32_t time_calib_1     : 2;
			uint32_t                  : 6;

			uint32_t time_calib_2     : 2;
			uint32_t                  : 6;

			uint32_t time_calib_3     : 2;
			uint32_t                  : 6;

			uint32_t amp_calib_0      : 2;
			uint32_t                  : 6;

			uint32_t amp_calib_1      : 2;
			uint32_t                  : 6;

			uint32_t amp_calib_2      : 2;
			uint32_t                  : 6;

			uint32_t amp_calib_3      : 2;
			uint32_t                  : 6;
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
	using namespace halco::hicann_dls::vx::v1;
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
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::Omnibus, SynapseCorrelationCalibQuad::config_size_in_words>
    SynapseCorrelationCalibQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseCorrelationCalibQuad::config_size_in_words>
    SynapseCorrelationCalibQuad::encode() const;

template <typename WordT>
void SynapseCorrelationCalibQuad::decode(
    std::array<WordT, SynapseCorrelationCalibQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx::v1;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });

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
    std::array<fisch::vx::Omnibus, SynapseCorrelationCalibQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void SynapseCorrelationCalibQuad::decode(
    std::array<
        fisch::vx::OmnibusChipOverJTAG,
        SynapseCorrelationCalibQuad::config_size_in_words> const& data);

HALDLS_VX_DEFAULT_OSTREAM_OP(SynapseCorrelationCalibQuad)

template <class Archive>
void SynapseCorrelationCalibQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_time_calibs));
	ar(CEREAL_NVP(m_amp_calibs));
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
	using namespace halco::hicann_dls::vx::v1;
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
			uint32_t weight_0         : 6;
			uint32_t time_calib_0     : 2;

			uint32_t weight_1         : 6;
			uint32_t time_calib_1     : 2;

			uint32_t weight_2         : 6;
			uint32_t time_calib_2     : 2;

			uint32_t weight_3         : 6;
			uint32_t time_calib_3     : 2;

			uint32_t label_0        : 6;
			uint32_t amp_calib_0      : 2;

			uint32_t label_1        : 6;
			uint32_t amp_calib_1      : 2;

			uint32_t label_2        : 6;
			uint32_t amp_calib_2      : 2;

			uint32_t label_3        : 6;
			uint32_t amp_calib_3      : 2;

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
	using namespace halco::hicann_dls::vx::v1;
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
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, SynapseQuad::config_size_in_words>
SynapseQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseQuad::config_size_in_words>
    SynapseQuad::encode() const;

template <typename WordT>
void SynapseQuad::decode(std::array<WordT, SynapseQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx::v1;
	std::array<uint32_t, config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });

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
    std::array<fisch::vx::Omnibus, SynapseQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void SynapseQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseQuad::config_size_in_words> const& data);

HALDLS_VX_DEFAULT_OSTREAM_OP(SynapseQuad)

template <class Archive>
void SynapseQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_weights));
	ar(CEREAL_NVP(m_labels));
	ar(CEREAL_NVP(m_time_calibs));
	ar(CEREAL_NVP(m_amp_calibs));
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

template <class Archive>
void ColumnCorrelationQuad::ColumnCorrelationSwitch::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_internal_causal));
	ar(CEREAL_NVP(m_enable_internal_acausal));
	ar(CEREAL_NVP(m_enable_debug_causal));
	ar(CEREAL_NVP(m_enable_debug_acausal));
	ar(CEREAL_NVP(m_enable_cadc_neuron_readout_causal));
	ar(CEREAL_NVP(m_enable_cadc_neuron_readout_acausal));
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

HALDLS_VX_DEFAULT_OSTREAM_OP(ColumnCurrentQuad)

bool ColumnCorrelationQuad::operator==(ColumnCorrelationQuad const& other) const
{
	return m_switches == other.m_switches;
}

bool ColumnCorrelationQuad::operator!=(ColumnCorrelationQuad const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(ColumnCorrelationQuad)

template <typename AddressT>
std::array<AddressT, ColumnCorrelationQuad::config_size_in_words> ColumnCorrelationQuad::addresses(
    coordinate_type const& block)
{
	using namespace halco::hicann_dls::vx::v1;
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
	    raw_base + detail::to_synram_quad_address_offset(block.toSynapseQuadColumnOnDLS()) +
	    SynapseQuadOnSynram::size * SynapseQuad::config_size_in_words +
	    ColumnCurrentQuadOnSynram::size * ColumnCurrentQuad::config_size_in_words;
	auto const address_1 = lsb_weight_base +
	                       detail::to_synram_quad_address_offset(block.toSynapseQuadColumnOnDLS()) +
	                       SynapseQuadOnSynram::size;
	auto const address_2 = lsb_label_base +
	                       detail::to_synram_quad_address_offset(block.toSynapseQuadColumnOnDLS()) +
	                       SynapseQuadOnSynram::size;
	return {{AddressT(address_0), AddressT((address_0) + ColumnCorrelationQuadOnSynram::size),
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
	using namespace halco::hicann_dls::vx::v1;
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

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, ColumnCorrelationQuad::config_size_in_words>
ColumnCorrelationQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCorrelationQuad::config_size_in_words>
    ColumnCorrelationQuad::encode() const;

template <typename WordT>
void ColumnCorrelationQuad::decode(
    std::array<WordT, ColumnCorrelationQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx::v1;
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
    std::array<fisch::vx::Omnibus, ColumnCorrelationQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void ColumnCorrelationQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCorrelationQuad::config_size_in_words> const&
        data);

template <class Archive>
void ColumnCorrelationQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_switches));
}


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
void ColumnCurrentQuad::ColumnCurrentSwitch::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_synaptic_current_excitatory));
	ar(CEREAL_NVP(m_enable_synaptic_current_inhibitory));
	ar(CEREAL_NVP(m_enable_debug_excitatory));
	ar(CEREAL_NVP(m_enable_debug_inhibitory));
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
	using namespace halco::hicann_dls::vx::v1;
	uint32_t base_even;
	uint32_t base_odd;
	if (block.toSynramOnDLS() == SynramOnDLS::bottom) {
		base_even = synram_synapse_even_2msb_bottom_base_address;
		base_odd = synram_synapse_odd_2msb_bottom_base_address;
	} else {
		base_even = synram_synapse_even_2msb_top_base_address;
		base_odd = synram_synapse_odd_2msb_top_base_address;
	}
	auto const offset = detail::to_synram_quad_address_offset(block.toSynapseQuadColumnOnDLS()) +
	                    SynapseQuadOnSynram::size;
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
			uint32_t                                      : 6;
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
	using namespace halco::hicann_dls::vx::v1;
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

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, ColumnCurrentQuad::config_size_in_words>
ColumnCurrentQuad::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCurrentQuad::config_size_in_words>
    ColumnCurrentQuad::encode() const;

template <typename WordT>
void ColumnCurrentQuad::decode(
    std::array<WordT, ColumnCurrentQuad::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx::v1;
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
    std::array<fisch::vx::Omnibus, ColumnCurrentQuad::config_size_in_words> const& data);
template SYMBOL_VISIBLE void ColumnCurrentQuad::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, ColumnCurrentQuad::config_size_in_words> const&
        data);

template <class Archive>
void ColumnCurrentQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_switches));
}


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
	int row_offset = coord.toSynapseRowOnSynram().toEnum() *
	                 halco::hicann_dls::vx::v1::SynapseQuadColumnOnDLS::size;
	int quad_offset = detail::to_synram_quad_address_offset(coord.toSynapseQuadColumnOnDLS());

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

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CorrelationReset::write_config_size_in_words>
    CorrelationReset::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, CorrelationReset::write_config_size_in_words>
CorrelationReset::encode<fisch::vx::Omnibus>() const;

template <typename WordT>
void CorrelationReset::decode(std::array<WordT, CorrelationReset::read_config_size_in_words> const&)
{}

template SYMBOL_VISIBLE void CorrelationReset::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, CorrelationReset::read_config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void CorrelationReset::decode<fisch::vx::Omnibus>(
    std::array<fisch::vx::Omnibus, CorrelationReset::read_config_size_in_words> const& data);

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
void CorrelationReset::serialize(Archive&, std::uint32_t const)
{}

} // namespace haldls::vx::v1

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::SynapseWeightQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::SynapseLabelQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::SynapseCorrelationCalibQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::SynapseQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::ColumnCorrelationQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(
    haldls::vx::v1::ColumnCorrelationQuad::ColumnCorrelationSwitch)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::ColumnCurrentQuad::ColumnCurrentSwitch)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::ColumnCurrentQuad)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::CorrelationReset)

CEREAL_CLASS_VERSION(haldls::vx::v1::SynapseWeightQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::SynapseLabelQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::SynapseCorrelationCalibQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::SynapseQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::ColumnCorrelationQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::ColumnCorrelationQuad::ColumnCorrelationSwitch, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::ColumnCurrentQuad::ColumnCurrentSwitch, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::ColumnCurrentQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::CorrelationReset, 0)
