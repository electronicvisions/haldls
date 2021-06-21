#include "haldls/vx/cadc.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"
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

std::ostream& operator<<(std::ostream& os, CADCConfig const& config)
{
	std::stringstream ss;
	ss << "CADCConfig(enable: " << std::boolalpha << config.m_enable << ", " << config.m_reset_wait
	   << ", " << config.m_dead_time << ")";
	return (os << ss.str());
}

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


bool CADCOffsetSRAMTimingConfig::operator==(CADCOffsetSRAMTimingConfig const& other) const
{
	return static_cast<SRAMTimingConfig>(*this) == static_cast<SRAMTimingConfig>(other);
}

bool CADCOffsetSRAMTimingConfig::operator!=(CADCOffsetSRAMTimingConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CADCOffsetSRAMTimingConfig const& config)
{
	os << "CADCOffsetSRAMTimingConfig(" << config.get_read_delay() << ", "
	   << config.get_address_setup_time() << ", " << config.get_enable_width() << ")";
	return os;
}

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

CEREAL_CLASS_VERSION(haldls::vx::CADCConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::CADCOffsetSRAMTimingConfig, 0)
