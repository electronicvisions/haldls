#include "haldls/vx/cadc.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/address_transformation.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"
#include "hate/join.h"


namespace haldls {
namespace vx {

CADCConfig::CADCConfig() : m_enable(false), m_reset_wait(), m_dead_time() {}

bool CADCConfig::get_enable() const
{
	return m_enable;
}

void CADCConfig::set_enable(bool const value)
{
	m_enable = value;
}

typename CADCConfig::ResetWait CADCConfig::get_reset_wait() const
{
	return m_reset_wait;
}

void CADCConfig::set_reset_wait(ResetWait const value)
{
	m_reset_wait = value;
}

typename CADCConfig::DeadTime CADCConfig::get_dead_time() const
{
	return m_dead_time;
}

void CADCConfig::set_dead_time(DeadTime const value)
{
	m_dead_time = value;
}

bool CADCConfig::operator==(CADCConfig const& other) const
{
	return m_enable == other.m_enable && m_reset_wait == other.m_reset_wait &&
	       m_dead_time == other.m_dead_time;
}

bool CADCConfig::operator!=(CADCConfig const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void CADCConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable));
	ar(CEREAL_NVP(m_reset_wait));
	ar(CEREAL_NVP(m_dead_time));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CADCConfig)

namespace {

struct CADCConfigBitfield
{
	union
	{
		detail::raw_omnibus_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			detail::raw_omnibus_type enable       :  1;
			detail::raw_omnibus_type /* unused */ :  3;
			detail::raw_omnibus_type reset_wait   :  8;
			detail::raw_omnibus_type dead_time    :  8;
			detail::raw_omnibus_type /* unused */ : 12;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CADCConfigBitfield() { u.raw = 0u; }

	CADCConfigBitfield(detail::raw_omnibus_type data) { u.raw = data; }
};

} // namespace

HALDLS_VX_DEFAULT_OSTREAM_OP(CADCConfig)

template <typename AddressT>
std::array<AddressT, CADCConfig::config_size_in_words> CADCConfig::addresses(
    coordinate_type const& coord)
{
	return {AddressT(cadc_busreg_addresses.at(coord.toEnum()))};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CADCConfig::config_size_in_words>
    CADCConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CADCConfig::config_size_in_words>
    CADCConfig::addresses(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, CADCConfig::config_size_in_words> CADCConfig::encode() const
{
	CADCConfigBitfield bitfield;
	bitfield.u.m.enable = m_enable;
	bitfield.u.m.reset_wait = m_reset_wait;
	bitfield.u.m.dead_time = m_dead_time;

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
}

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChipOverJTAG, CADCConfig::config_size_in_words>
CADCConfig::encode() const;
template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, CADCConfig::config_size_in_words>
CADCConfig::encode() const;

template <typename WordT>
void CADCConfig::decode(std::array<WordT, CADCConfig::config_size_in_words> const& data)
{
	CADCConfigBitfield bitfield(data[0].get());
	m_enable = bitfield.u.m.enable;
	m_reset_wait = ResetWait(bitfield.u.m.reset_wait);
	m_dead_time = DeadTime(bitfield.u.m.dead_time);
}

template SYMBOL_VISIBLE void CADCConfig::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, CADCConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void CADCConfig::decode(
    std::array<fisch::vx::Omnibus, CADCConfig::config_size_in_words> const& data);


CADCChannelConfig::CADCChannelConfig() : m_offset() {}

typename CADCChannelConfig::Offset CADCChannelConfig::get_offset() const
{
	return m_offset;
}

void CADCChannelConfig::set_offset(Offset const value)
{
	m_offset = value;
}

bool CADCChannelConfig::operator==(CADCChannelConfig const& other) const
{
	return m_offset == other.m_offset;
}

bool CADCChannelConfig::operator!=(CADCChannelConfig const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void CADCChannelConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_offset));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CADCChannelConfig)

namespace {

struct CADCChannelConfigBitfield
{
	union
	{
		detail::raw_omnibus_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			detail::raw_omnibus_type offset       :  8;
			detail::raw_omnibus_type /* unused */ : 24;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CADCChannelConfigBitfield() { u.raw = 0u; }

	CADCChannelConfigBitfield(detail::raw_omnibus_type data) { u.raw = data; }
};

} // namespace

HALDLS_VX_DEFAULT_OSTREAM_OP(CADCChannelConfig)

template <typename AddressT>
std::array<AddressT, CADCChannelConfig::config_size_in_words> CADCChannelConfig::addresses(
    coordinate_type const& coord)
{
	bool const is_east = coord.toCADCChannelColumnOnSynram() >=
	                     (halco::hicann_dls::vx::CADCChannelColumnOnSynram::size / 2);
	uint32_t const column_offset =
	    coord.toCADCChannelColumnOnSynram() -
	    (is_east ? (halco::hicann_dls::vx::CADCChannelColumnOnSynram::size / 2) : 0);
	uint32_t const base = cadc_sram_base_addresses.at(coord.toSynramOnDLS().toEnum() * 2 + is_east);
	return {AddressT(
	    base + (2 * column_offset) + coord.toCADCChannelConfigOnSynram().toCADCChannelType())};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CADCChannelConfig::config_size_in_words>
CADCChannelConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CADCChannelConfig::config_size_in_words>
    CADCChannelConfig::addresses(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, CADCChannelConfig::config_size_in_words> CADCChannelConfig::encode() const
{
	CADCChannelConfigBitfield bitfield;
	bitfield.u.m.offset = static_cast<int32_t>(m_offset) + 128;

	return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CADCChannelConfig::config_size_in_words>
    CADCChannelConfig::encode() const;
template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, CADCChannelConfig::config_size_in_words>
CADCChannelConfig::encode() const;

template <typename WordT>
void CADCChannelConfig::decode(
    std::array<WordT, CADCChannelConfig::config_size_in_words> const& data)
{
	CADCChannelConfigBitfield bitfield(data[0].get());
	m_offset = Offset(static_cast<int32_t>(bitfield.u.m.offset) - 128);
}

template SYMBOL_VISIBLE void CADCChannelConfig::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, CADCChannelConfig::config_size_in_words> const&
        data);
template SYMBOL_VISIBLE void CADCChannelConfig::decode(
    std::array<fisch::vx::Omnibus, CADCChannelConfig::config_size_in_words> const& data);


CADCSampleQuad::CADCSampleQuad() : m_samples() {}

typename CADCSampleQuad::Value CADCSampleQuad::get_sample(
    halco::hicann_dls::vx::EntryOnQuad const& coord) const
{
	return m_samples.at(coord);
}

void CADCSampleQuad::set_sample(halco::hicann_dls::vx::EntryOnQuad const& coord, Value const sample)
{
	m_samples.at(coord) = sample;
}

bool CADCSampleQuad::operator==(CADCSampleQuad const& other) const
{
	return m_samples == other.m_samples;
}

bool CADCSampleQuad::operator!=(CADCSampleQuad const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void CADCSampleQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_samples));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CADCSampleQuad)

namespace {

struct CADCSampleQuadBitfield
{
	union
	{
		detail::raw_omnibus_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			detail::raw_omnibus_type sample_3 : 8;
			detail::raw_omnibus_type sample_2 : 8;
			detail::raw_omnibus_type sample_1 : 8;
			detail::raw_omnibus_type sample_0 : 8;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CADCSampleQuadBitfield() { u.raw = 0u; }

	CADCSampleQuadBitfield(detail::raw_omnibus_type data) { u.raw = data; }
};

} // namespace

std::ostream& operator<<(std::ostream& os, CADCSampleQuad const& config)
{
	os << "CADCSampleQuad(" << hate::join_string(config.m_samples, ", ") << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, CADCSampleQuad::read_config_size_in_words>
CADCSampleQuad::read_addresses(coordinate_type const& coord)
{
	uint32_t const base = synram_cadc_base_addresses.at(coord.toSynramOnDLS().toEnum())
	                          .at(coord.toCADCChannelType().toEnum()) |
	                      (coord.toCADCReadoutType() ? cadc_buffer_enable_mask : 0ul);

	auto const quad = coord.toSynapseQuadOnSynram();
	uint32_t const address_offset = quad.y() * halco::hicann_dls::vx::SynapseQuadColumnOnDLS::size +
	                                halco::hicann_dls::vx::SynapseQuadColumnOnDLS::max -
	                                detail::to_synram_quad_address_offset(quad.x());
	return {halco::hicann_dls::vx::OmnibusAddress(base + address_offset)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, CADCSampleQuad::write_config_size_in_words>
CADCSampleQuad::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

std::array<fisch::vx::Omnibus, CADCSampleQuad::write_config_size_in_words> CADCSampleQuad::encode()
    const
{
	return {};
}

namespace {

uint8_t reverse_byte(uint8_t b)
{
	b = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4);
	b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2);
	b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1);
	return b;
}

} // namespace

void CADCSampleQuad::decode(
    std::array<fisch::vx::Omnibus, CADCSampleQuad::read_config_size_in_words> const& data)
{
	CADCSampleQuadBitfield bitfield(data[0].get());
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(0)) =
	    Value(reverse_byte(bitfield.u.m.sample_0));
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(1)) =
	    Value(reverse_byte(bitfield.u.m.sample_1));
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(2)) =
	    Value(reverse_byte(bitfield.u.m.sample_2));
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(3)) =
	    Value(reverse_byte(bitfield.u.m.sample_3));
}


bool CADCOffsetSRAMTimingConfig::operator==(CADCOffsetSRAMTimingConfig const& other) const
{
	return static_cast<SRAMTimingConfig>(*this) == static_cast<SRAMTimingConfig>(other);
}

bool CADCOffsetSRAMTimingConfig::operator!=(CADCOffsetSRAMTimingConfig const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(CADCOffsetSRAMTimingConfig)

template <typename AddressT>
std::array<AddressT, CADCOffsetSRAMTimingConfig::config_size_in_words>
CADCOffsetSRAMTimingConfig::addresses(coordinate_type const& coord) const
{
	return {AddressT(cadc_offset_sram_timing_config_base_addresses.at(coord.toEnum())),
	        AddressT(cadc_offset_sram_timing_config_base_addresses.at(coord.toEnum()) + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CADCOffsetSRAMTimingConfig::config_size_in_words>
CADCOffsetSRAMTimingConfig::addresses(coordinate_type const& coord) const;
template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusAddress, CADCOffsetSRAMTimingConfig::config_size_in_words>
    CADCOffsetSRAMTimingConfig::addresses(coordinate_type const& coord) const;

template <typename Archive>
void CADCOffsetSRAMTimingConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(cereal::base_class<detail::SRAMTimingConfig>(this));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CADCOffsetSRAMTimingConfig)

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::CADCSampleQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::CADCChannelConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::CADCConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::CADCOffsetSRAMTimingConfig, 0)
