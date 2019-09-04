#include <cctype>
#include <iomanip>
#include <utility>

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/common.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"


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
void CADCConfig::serialize(Archive& ar)
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
    coordinate_type const& coord) const
{
	return {AddressT(cadc_busreg_addresses.at(coord.toEnum()))};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CADCConfig::config_size_in_words>
    CADCConfig::addresses(coordinate_type const& coord) const;
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, CADCConfig::config_size_in_words>
    CADCConfig::addresses(coordinate_type const& coord) const;

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
template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, CADCConfig::config_size_in_words>
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
    std::array<fisch::vx::OmnibusChip, CADCConfig::config_size_in_words> const& data);


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
void CADCChannelConfig::serialize(Archive& ar)
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
    coordinate_type const& coord) const
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
CADCChannelConfig::addresses(coordinate_type const& coord) const;
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, CADCChannelConfig::config_size_in_words>
    CADCChannelConfig::addresses(coordinate_type const& coord) const;

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
template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, CADCChannelConfig::config_size_in_words>
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
    std::array<fisch::vx::OmnibusChip, CADCChannelConfig::config_size_in_words> const& data);

} // namespace vx
} // namespace haldls
