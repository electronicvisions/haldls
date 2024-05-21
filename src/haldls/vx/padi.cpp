#include "haldls/vx/padi.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"

namespace haldls {
namespace vx {

PADIEvent::PADIEvent() : m_fire_bus(), m_event_address(), m_row_select_address() {}

struct PADIEvent::PADIEventBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t fire_bus_0         :  1; /*       0 */ ) \
			(uint32_t fire_bus_1         :  1; /*       1 */ ) \
			(uint32_t fire_bus_2         :  1; /*       2 */ ) \
			(uint32_t fire_bus_3         :  1; /*       3 */ ) \
			(uint32_t                    : 12; /* 15 -  4 */ ) \
			(uint32_t event_address      :  6; /* 21 - 16 */ ) \
			(uint32_t row_select_address :  5; /* 26 - 22 */ ) \
			(uint32_t                    :  5; /* 31 - 27 */ )
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "Sizes of union types should match.");
	} u;

	PADIEventBitfield() { u.raw = 0u; }

	PADIEventBitfield(uint32_t data) { u.raw = data & 0b0000'0111'1111'1111'0000'0000'0000'1111u; }
};

typename PADIEvent::fire_bus_type const& PADIEvent::get_fire_bus() const
{
	return m_fire_bus;
}

void PADIEvent::set_fire_bus(fire_bus_type const& value)
{
	m_fire_bus = value;
}

PADIEvent::EventAddress PADIEvent::get_event_address() const
{
	return m_event_address;
}

void PADIEvent::set_event_address(PADIEvent::EventAddress const value)
{
	m_event_address = value;
}

PADIEvent::HagenAddress PADIEvent::get_hagen_address() const
{
	return HagenAddress(m_event_address >> 5);
}

void PADIEvent::set_hagen_address(PADIEvent::HagenAddress const value)
{
	m_event_address = EventAddress(value << 5 | (m_event_address & 0x1f));
}

PADIEvent::HagenActivation PADIEvent::get_hagen_activation() const
{
	return HagenActivation(m_event_address & 0x1f);
}

void PADIEvent::set_hagen_activation(PADIEvent::HagenActivation const value)
{
	m_event_address = EventAddress((value & 0x1f) | (m_event_address & 0x20));
}

PADIEvent::RowSelectAddress PADIEvent::get_row_select_address() const
{
	return m_row_select_address;
}

void PADIEvent::set_row_select_address(RowSelectAddress const value)
{
	m_row_select_address = value;
}

template <typename AddressT>
std::array<AddressT, PADIEvent::write_config_size_in_words> PADIEvent::write_addresses(
	coordinate_type const& coord)
{
	auto const base_address = padi_base_addresses.at(coord);
	return {AddressT(base_address + 0)};
}

template SYMBOL_VISIBLE std::array<
	halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	PADIEvent::write_config_size_in_words>
PADIEvent::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
	coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, PADIEvent::write_config_size_in_words>
    PADIEvent::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& coord);


template <typename AddressT>
std::array<AddressT, PADIEvent::read_config_size_in_words> PADIEvent::read_addresses(
	coordinate_type const& /* coord */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
	halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
	PADIEvent::read_config_size_in_words>
PADIEvent::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
	coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, PADIEvent::read_config_size_in_words>
    PADIEvent::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, PADIEvent::write_config_size_in_words> PADIEvent::encode() const
{
	PADIEventBitfield bitfield;

	bitfield.u.m.fire_bus_0 = m_fire_bus[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)];
	bitfield.u.m.fire_bus_1 = m_fire_bus[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)];
	bitfield.u.m.fire_bus_2 = m_fire_bus[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)];
	bitfield.u.m.fire_bus_3 = m_fire_bus[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)];
	bitfield.u.m.event_address = m_event_address;
	bitfield.u.m.row_select_address = m_row_select_address;

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, PADIEvent::write_config_size_in_words>
    PADIEvent::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, PADIEvent::write_config_size_in_words>
    PADIEvent::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void PADIEvent::decode(std::array<WordT, PADIEvent::read_config_size_in_words> const& /* data */)
{}

template SYMBOL_VISIBLE void PADIEvent::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        PADIEvent::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void PADIEvent::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, PADIEvent::read_config_size_in_words> const&
        data);

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, PADIEvent const& config)
{
	std::stringstream ss;
	ss << "PADIEvent(\n" << std::boolalpha;
	// clang-format off
	ss << "\tfire_bus:           \t[";
	ss << hate::join(config.m_fire_bus.begin(), config.m_fire_bus.end(), ", ");
	ss << "]\n"
	   << "\tevent_address:      \t" << config.m_event_address << "\n"
	   << "\trow_select_address: \t" << config.m_row_select_address << "\n)";
	// clang-format on
	return (os << ss.str());
}
#endif

bool PADIEvent::operator==(PADIEvent const& other) const
{
	return (
	    m_fire_bus == other.get_fire_bus() && m_event_address == other.get_event_address() &&
	    m_row_select_address == other.get_row_select_address());
}

bool PADIEvent::operator!=(PADIEvent const& other) const
{
	return !(*this == other);
}


CommonPADIBusConfig::CommonPADIBusConfig() :
    m_enable_spl1(),
    m_enable_extended_timing(),
    m_dacen_pulse_extension()
{}

struct CommonPADIBusConfig::CommonPADIBusConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t enable_spl1_0            : 1; /*       0 */ ) \
			(uint32_t enable_spl1_1            : 1; /*       1 */ ) \
			(uint32_t enable_spl1_2            : 1; /*       2 */ ) \
			(uint32_t enable_spl1_3            : 1; /*       3 */ ) \
			(uint32_t enable_extended_timing_0 : 1; /*       4 */ ) \
			(uint32_t enable_extended_timing_1 : 1; /*       5 */ ) \
			(uint32_t enable_extended_timing_2 : 1; /*       6 */ ) \
			(uint32_t enable_extended_timing_3 : 1; /*       7 */ ) \
			(uint32_t                          : 8; /* 15 -  8 */ ) \
			(uint32_t dacen_pulse_extension_0  : 4; /* 19 - 16 */ ) \
			(uint32_t dacen_pulse_extension_1  : 4; /* 23 - 20 */ ) \
			(uint32_t dacen_pulse_extension_2  : 4; /* 27 - 24 */ ) \
			(uint32_t dacen_pulse_extension_3  : 4; /* 31 - 28 */ )
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "Sizes of union types should match.");
	} u;

	CommonPADIBusConfigBitfield() { u.raw = 0u; }

	CommonPADIBusConfigBitfield(uint32_t data) { u.raw = data; }
};

typename CommonPADIBusConfig::enable_spl1_type const& CommonPADIBusConfig::get_enable_spl1() const
{
	return m_enable_spl1;
}

void CommonPADIBusConfig::set_enable_spl1(enable_spl1_type const& value)
{
	m_enable_spl1 = value;
}

CommonPADIBusConfig::enable_extended_timing_type const&
CommonPADIBusConfig::get_enable_extended_timing() const
{
	return m_enable_extended_timing;
}

void CommonPADIBusConfig::set_enable_extended_timing(
    CommonPADIBusConfig::enable_extended_timing_type const& value)
{
	m_enable_extended_timing = value;
}

CommonPADIBusConfig::dacen_pulse_extension_type const&
CommonPADIBusConfig::get_dacen_pulse_extension() const
{
	return m_dacen_pulse_extension;
}

void CommonPADIBusConfig::set_dacen_pulse_extension(
    CommonPADIBusConfig::dacen_pulse_extension_type const& value)
{
	m_dacen_pulse_extension = value;
}

template <typename AddressT>
std::array<AddressT, CommonPADIBusConfig::config_size_in_words> CommonPADIBusConfig::addresses(
    CommonPADIBusConfig::coordinate_type const& coord)
{
	auto const base_address = padi_base_addresses.at(coord);
	return {AddressT(base_address + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CommonPADIBusConfig::config_size_in_words>
CommonPADIBusConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CommonPADIBusConfig::config_size_in_words>
    CommonPADIBusConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);


template <typename WordT>
std::array<WordT, CommonPADIBusConfig::config_size_in_words> CommonPADIBusConfig::encode() const
{
	CommonPADIBusConfigBitfield bitfield;

	bitfield.u.m.enable_spl1_0 = m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)];
	bitfield.u.m.enable_spl1_1 = m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)];
	bitfield.u.m.enable_spl1_2 = m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)];
	bitfield.u.m.enable_spl1_3 = m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)];

	bitfield.u.m.enable_extended_timing_0 =
	    m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)];
	bitfield.u.m.enable_extended_timing_1 =
	    m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)];
	bitfield.u.m.enable_extended_timing_2 =
	    m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)];
	bitfield.u.m.enable_extended_timing_3 =
	    m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)];

	bitfield.u.m.dacen_pulse_extension_0 =
	    m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)];
	bitfield.u.m.dacen_pulse_extension_1 =
	    m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)];
	bitfield.u.m.dacen_pulse_extension_2 =
	    m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)];
	bitfield.u.m.dacen_pulse_extension_3 =
	    m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)];

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CommonPADIBusConfig::config_size_in_words>
CommonPADIBusConfig::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CommonPADIBusConfig::config_size_in_words>
    CommonPADIBusConfig::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CommonPADIBusConfig::decode(
    std::array<WordT, CommonPADIBusConfig::config_size_in_words> const& data)
{
	CommonPADIBusConfigBitfield bitfield;
	bitfield.u.raw = data.at(0);

	m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)] = bitfield.u.m.enable_spl1_0;
	m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] = bitfield.u.m.enable_spl1_1;
	m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] = bitfield.u.m.enable_spl1_2;
	m_enable_spl1[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)] = bitfield.u.m.enable_spl1_3;

	m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)] =
	    bitfield.u.m.enable_extended_timing_0;
	m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] =
	    bitfield.u.m.enable_extended_timing_1;
	m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] =
	    bitfield.u.m.enable_extended_timing_2;
	m_enable_extended_timing[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)] =
	    bitfield.u.m.enable_extended_timing_3;

	m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(0)] =
	    DacenPulseExtension(bitfield.u.m.dacen_pulse_extension_0);
	m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(1)] =
	    DacenPulseExtension(bitfield.u.m.dacen_pulse_extension_1);
	m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(2)] =
	    DacenPulseExtension(bitfield.u.m.dacen_pulse_extension_2);
	m_dacen_pulse_extension[halco::hicann_dls::vx::PADIBusOnPADIBusBlock(3)] =
	    DacenPulseExtension(bitfield.u.m.dacen_pulse_extension_3);
}

template SYMBOL_VISIBLE void
CommonPADIBusConfig::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CommonPADIBusConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void CommonPADIBusConfig::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CommonPADIBusConfig::config_size_in_words> const& data);

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CommonPADIBusConfig const& config)
{
	std::stringstream ss;
	ss << "CommonPADIBusConfig(\n" << std::boolalpha;
	// clang-format off
	ss << "\tenable_spl1:            \t[";
	ss << hate::join(config.m_enable_spl1.begin(), config.m_enable_spl1.end(), ", ");
	ss << "]\n";
	ss << "\tenable_extended_timing: \t[";
	ss << hate::join(config.m_enable_extended_timing.begin(), config.m_enable_extended_timing.end(), ", ");
	ss << "]\n";
	ss << "\tdacen_pulse_extension:  \t[";
	ss << hate::join(config.m_dacen_pulse_extension.begin(), config.m_dacen_pulse_extension.end(), ", ");
	ss << "]\n)";
	// clang-format on
	return (os << ss.str());
}
#endif

bool CommonPADIBusConfig::operator==(CommonPADIBusConfig const& other) const
{
	// clang-format off
	return (
			m_enable_spl1 == other.get_enable_spl1() &&
			m_enable_extended_timing == other.get_enable_extended_timing() &&
			m_dacen_pulse_extension == other.get_dacen_pulse_extension()
	       );
	// clang-format on
}

bool CommonPADIBusConfig::operator!=(CommonPADIBusConfig const& other) const
{
	return !(*this == other);
}


CommonSTPConfig::CommonSTPConfig() : m_enable_recovery_clock(false), m_recovery_clock_speed() {}

struct CommonSTPConfig::CommonSTPConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t recovery_clock_speed  :  4; /*  3 -  0 */ ) \
			(uint32_t enable_recovery_clock :  1; /*       4 */ ) \
			(uint32_t                       : 27; /* 31 -  5 */ )
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "Sizes of union types should match.");
	} u;

	CommonSTPConfigBitfield() { u.raw = 0u; }

	CommonSTPConfigBitfield(uint32_t data) { u.raw = data; }
};

bool CommonSTPConfig::get_enable_recovery_clock() const
{
	return m_enable_recovery_clock;
}

void CommonSTPConfig::set_enable_recovery_clock(bool const value)
{
	m_enable_recovery_clock = value;
}

CommonSTPConfig::RecoveryClockSpeed CommonSTPConfig::get_recovery_clock_speed() const
{
	return m_recovery_clock_speed;
}

void CommonSTPConfig::set_recovery_clock_speed(CommonSTPConfig::RecoveryClockSpeed const value)
{
	m_recovery_clock_speed = value;
}

template <typename AddressT>
std::array<AddressT, CommonSTPConfig::config_size_in_words> CommonSTPConfig::addresses(
    coordinate_type const& coord)
{
	auto const base_address = padi_base_addresses.at(coord);
	return {AddressT(base_address + 2)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CommonSTPConfig::config_size_in_words>
    CommonSTPConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CommonSTPConfig::config_size_in_words>
    CommonSTPConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, CommonSTPConfig::config_size_in_words> CommonSTPConfig::encode() const
{
	CommonSTPConfigBitfield bitfield;

	bitfield.u.m.enable_recovery_clock = m_enable_recovery_clock;
	bitfield.u.m.recovery_clock_speed = m_recovery_clock_speed;

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, CommonSTPConfig::config_size_in_words>
    CommonSTPConfig::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CommonSTPConfig::config_size_in_words>
    CommonSTPConfig::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CommonSTPConfig::decode(std::array<WordT, CommonSTPConfig::config_size_in_words> const& data)
{
	CommonSTPConfigBitfield bitfield(data.at(0));

	m_enable_recovery_clock = bitfield.u.m.enable_recovery_clock;
	m_recovery_clock_speed = RecoveryClockSpeed(bitfield.u.m.recovery_clock_speed);
}

template SYMBOL_VISIBLE void
CommonSTPConfig::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CommonSTPConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void CommonSTPConfig::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, CommonSTPConfig::config_size_in_words> const&
        data);

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CommonSTPConfig const& config)
{
	std::stringstream ss;
	ss << "CommonSTPConfig(enable_recovery_clock: " << std::boolalpha
	   << config.m_enable_recovery_clock << ", " << config.m_recovery_clock_speed << ")";
	return (os << ss.str());
}
#endif

bool CommonSTPConfig::operator==(CommonSTPConfig const& other) const
{
	return (
	    m_enable_recovery_clock == other.get_enable_recovery_clock() &&
	    m_recovery_clock_speed == other.get_recovery_clock_speed());
}

bool CommonSTPConfig::operator!=(CommonSTPConfig const& other) const
{
	return !(*this == other);
}

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PADIEvent)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CommonPADIBusConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CommonSTPConfig)
