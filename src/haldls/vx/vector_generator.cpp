#include "haldls/vx/vector_generator.h"

#include <string>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/ppu.h"
#include "hate/join.h"

namespace haldls::vx {

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl::Source const& mode)
{
	switch (mode) {
		case VectorGeneratorControl::Source::omnibus:
			return os << "omnibus";
		case VectorGeneratorControl::Source::external_0:
			return os << "external_0";
		case VectorGeneratorControl::Source::external_1:
			return os << "external_1";
		case VectorGeneratorControl::Source::external_2:
			return os << "external_2";
		default:
			throw std::logic_error("Ostream operator of specified Source not implemented");
	}
}

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl::Notification const& mode)
{
	switch (mode) {
		case VectorGeneratorControl::Notification::omnibus:
			return os << "omnibus";
		case VectorGeneratorControl::Notification::gpio:
			return os << "gpio";
		default:
			throw std::logic_error("Ostream operator of specified Notification not implemented");
	}
}

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl::Trigger const& mode)
{
	switch (mode) {
		case VectorGeneratorControl::Trigger::omnibus:
			return os << "omnibus";
		case VectorGeneratorControl::Trigger::gpio:
			return os << "gpio";
		default:
			throw std::logic_error("Ostream operator of specified Trigger not implemented");
	}
}

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl::Signal const& mode)
{
	switch (mode) {
		case VectorGeneratorControl::Signal::disabled:
			return os << "disabled";
		case VectorGeneratorControl::Signal::notification:
			return os << "notification";
		case VectorGeneratorControl::Signal::trigger_and_notification:
			return os << "trigger_and_notification";
		default:
			throw std::logic_error("Ostream operator of specified Signal not implemented");
	}
}

halco::hicann_dls::vx::VectorGeneratorLUTEntryOnVectorGeneratorLUT
VectorGeneratorControl::get_lookup_table_entry_start() const
{
	return m_lookup_table_entry_start;
}

void VectorGeneratorControl::set_lookup_table_entry_start(
    halco::hicann_dls::vx::VectorGeneratorLUTEntryOnVectorGeneratorLUT const value)
{
	m_lookup_table_entry_start = value;
}

VectorGeneratorControl::SourceCount VectorGeneratorControl::get_source_count() const
{
	return m_source_count;
}

void VectorGeneratorControl::set_source_count(SourceCount const value)
{
	m_source_count = value;
}

VectorGeneratorControl::ResendCount VectorGeneratorControl::get_resend_count() const
{
	return m_resend_count;
}

void VectorGeneratorControl::set_resend_count(ResendCount const value)
{
	m_resend_count = value;
}

VectorGeneratorControl::SourceWaits VectorGeneratorControl::get_source_waits() const
{
	return m_source_waits;
}

void VectorGeneratorControl::set_source_waits(SourceWaits const value)
{
	m_source_waits = value;
}

VectorGeneratorControl::Sources VectorGeneratorControl::get_sources() const
{
	return m_sources;
}

void VectorGeneratorControl::set_sources(Sources const value)
{
	m_sources = value;
}

VectorGeneratorControl::EventPack VectorGeneratorControl::get_event_pack() const
{
	return m_event_pack;
}

void VectorGeneratorControl::set_event_pack(EventPack const value)
{
	m_event_pack = value;
}

bool VectorGeneratorControl::get_enable_ignore_zeros() const
{
	return m_enable_ignore_zeros;
}

void VectorGeneratorControl::set_enable_ignore_zeros(bool const value)
{
	m_enable_ignore_zeros = value;
}

VectorGeneratorControl::Notification VectorGeneratorControl::get_notification() const
{
	return m_notification;
}

void VectorGeneratorControl::set_notification(Notification const value)
{
	m_notification = value;
}

VectorGeneratorControl::Trigger VectorGeneratorControl::get_trigger() const
{
	return m_trigger;
}

void VectorGeneratorControl::set_trigger(Trigger const value)
{
	m_trigger = value;
}

VectorGeneratorControl::Signal VectorGeneratorControl::get_signal() const
{
	return m_signal;
}
void VectorGeneratorControl::set_signal(Signal const value)
{
	m_signal = value;
}

std::
    array<halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorControl::write_config_size_in_words>
    VectorGeneratorControl::write_addresses(VectorGeneratorControl::coordinate_type const& coord)
{
	return {
	    halco::hicann_dls::vx::OmnibusAddress(vector_generator_base_addresses.at(coord.toEnum()))};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorControl::read_config_size_in_words>
VectorGeneratorControl::read_addresses(VectorGeneratorControl::coordinate_type const&)
{
	return {};
}

namespace {

struct VectorGeneratorControlBitfield
{
	union
	{
		std::array<uint32_t, VectorGeneratorControl::write_config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t trigger                  : 1;
			uint32_t enable_trigger           : 1;
			uint32_t notification             : 1;
			uint32_t enable_notification      : 1;
			uint32_t resend_count             : 4;
			uint32_t enable_ignore_zeros      : 1;
			uint32_t event_pack               : 2;
			uint32_t source_1_wait            : 4;
			uint32_t source_1                 : 2;
			uint32_t source_0_wait            : 4;
			uint32_t source_0                 : 2;
			uint32_t source_count             : 1;
			uint32_t lookup_table_entry_start : 8;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	VectorGeneratorControlBitfield()
	{
		u.raw = {{0}};
	}
	VectorGeneratorControlBitfield(
	    std::array<uint32_t, VectorGeneratorControl::write_config_size_in_words> const& data)
	{
		u.raw = data;
	}
};

} // anonymous namespace

std::array<fisch::vx::word_access_type::Omnibus, VectorGeneratorControl::write_config_size_in_words>
VectorGeneratorControl::encode() const
{
	VectorGeneratorControlBitfield bitfield;
	bitfield.u.m.lookup_table_entry_start = m_lookup_table_entry_start;
	bitfield.u.m.source_count = m_source_count.value() - 1;
	bitfield.u.m.resend_count = m_resend_count.value();
	bitfield.u.m.source_0_wait =
	    m_source_waits.at(halco::hicann_dls::vx::SourceOnVectorGenerator(0));
	bitfield.u.m.source_1_wait =
	    m_source_waits.at(halco::hicann_dls::vx::SourceOnVectorGenerator(1));
	bitfield.u.m.source_0 =
	    static_cast<uint32_t>(m_sources.at(halco::hicann_dls::vx::SourceOnVectorGenerator(0)));
	bitfield.u.m.source_1 =
	    static_cast<uint32_t>(m_sources.at(halco::hicann_dls::vx::SourceOnVectorGenerator(1)));
	bitfield.u.m.event_pack = m_event_pack.value() - 1;
	bitfield.u.m.enable_ignore_zeros = m_enable_ignore_zeros;
	bitfield.u.m.notification = static_cast<uint32_t>(m_notification);
	bitfield.u.m.trigger = static_cast<uint32_t>(m_trigger);
	switch (m_signal) {
		case VectorGeneratorControl::Signal::disabled: {
			bitfield.u.m.enable_notification = false;
			bitfield.u.m.enable_trigger = false;
			break;
		}
		case VectorGeneratorControl::Signal::notification: {
			bitfield.u.m.enable_notification = true;
			bitfield.u.m.enable_trigger = false;
			break;
		}
		case VectorGeneratorControl::Signal::trigger_and_notification: {
			bitfield.u.m.enable_notification = true;
			bitfield.u.m.enable_trigger = true;
			break;
		}
		default:
			throw std::logic_error("Encoding of specified Signal not implemented");
	}
	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw[0])};
}

void VectorGeneratorControl::decode(std::array<
                                    fisch::vx::word_access_type::Omnibus,
                                    VectorGeneratorControl::read_config_size_in_words> const&)
{}

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl const& config)
{
	std::stringstream ss;
	ss << std::boolalpha << "VectorGeneratorControl("
	   << "\n";
	ss << "\tsource_count: " << config.m_source_count << ","
	   << "\n";
	ss << "\tresend_count: " << config.m_resend_count << ","
	   << "\n";
	ss << "\tsource_waits: [" << hate::join(config.m_source_waits, ", ") << "],"
	   << "\n";
	ss << "\tsources: [" << hate::join(config.m_sources, ", ") << "],"
	   << "\n";
	ss << "\tevent_pack: " << config.m_event_pack << ","
	   << "\n";
	ss << "\tenable_ignore_zeros: " << config.m_enable_ignore_zeros << ","
	   << "\n";
	ss << "\tlookup_table_entry_start: " << config.m_lookup_table_entry_start << ","
	   << "\n";
	ss << "\tnotification: " << config.m_notification << ","
	   << "\n";
	ss << "\ttrigger: " << config.m_trigger << ","
	   << "\n";
	ss << "\tsignal: " << config.m_signal << "\n";
	ss << ")";
	return os << ss.str();
}

bool VectorGeneratorControl::operator==(VectorGeneratorControl const& other) const
{
	return (
	    (m_source_count == other.m_source_count) && (m_source_waits == other.m_source_waits) &&
	    (m_resend_count == other.m_resend_count) && (m_sources == other.m_sources) &&
	    (m_event_pack == other.m_event_pack) &&
	    (m_enable_ignore_zeros == other.m_enable_ignore_zeros) &&
	    (m_lookup_table_entry_start == other.m_lookup_table_entry_start) &&
	    (m_notification == other.m_notification) && (m_trigger == other.m_trigger) &&
	    (m_signal == other.m_signal));
}

bool VectorGeneratorControl::operator!=(VectorGeneratorControl const& other) const
{
	return !(*this == other);
}


typename VectorGeneratorLUTEntry::Value VectorGeneratorLUTEntry::get_value() const
{
	return m_value;
}

void VectorGeneratorLUTEntry::set_value(Value const value)
{
	m_value = value;
}

bool VectorGeneratorLUTEntry::operator==(VectorGeneratorLUTEntry const& other) const
{
	return m_value == other.m_value;
}

bool VectorGeneratorLUTEntry::operator!=(VectorGeneratorLUTEntry const& other) const
{
	return !(*this == other);
}

namespace {

struct VectorGeneratorLUTEntryBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t value       :  11;
			uint32_t /* unused */ : 21;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	VectorGeneratorLUTEntryBitfield()
	{
		u.raw = 0u;
	}

	VectorGeneratorLUTEntryBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorLUTEntry::config_size_in_words>
VectorGeneratorLUTEntry::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    vector_generator_base_addresses.at(coord.toVectorGeneratorLUTOnFPGA().toEnum()) +
	    coord.toVectorGeneratorLUTEntryOnVectorGeneratorLUT() + 0x100)};
}

std::array<fisch::vx::word_access_type::Omnibus, VectorGeneratorLUTEntry::config_size_in_words>
VectorGeneratorLUTEntry::encode() const
{
	VectorGeneratorLUTEntryBitfield bitfield;
	bitfield.u.m.value = m_value;

	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw)};
}

void VectorGeneratorLUTEntry::decode(std::array<
                                     fisch::vx::word_access_type::Omnibus,
                                     VectorGeneratorLUTEntry::config_size_in_words> const& data)
{
	VectorGeneratorLUTEntryBitfield bitfield(data[0]);
	m_value = Value(bitfield.u.m.value);
}

std::ostream& operator<<(std::ostream& os, VectorGeneratorLUTEntry const& config)
{
	os << "VectorGeneratorLUTEntry(" << config.m_value << ")";
	return os;
}


VectorGeneratorNotificationAddress::VectorGeneratorNotificationAddress(
    halco::hicann_dls::vx::PPUMemoryWordOnDLS const& word)
{
	m_value = PPUMemoryWord::addresses<Value>(word).at(0);
}

typename VectorGeneratorNotificationAddress::Value VectorGeneratorNotificationAddress::get_value()
    const
{
	return m_value;
}

void VectorGeneratorNotificationAddress::set_value(Value const value)
{
	m_value = value;
}

bool VectorGeneratorNotificationAddress::operator==(
    VectorGeneratorNotificationAddress const& other) const
{
	return m_value == other.m_value;
}

bool VectorGeneratorNotificationAddress::operator!=(
    VectorGeneratorNotificationAddress const& other) const
{
	return !(*this == other);
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    VectorGeneratorNotificationAddress::config_size_in_words>
VectorGeneratorNotificationAddress::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    vector_generator_base_addresses.at(coord.toEnum()) + 0x1)};
}

std::array<
    fisch::vx::word_access_type::Omnibus,
    VectorGeneratorNotificationAddress::config_size_in_words>
VectorGeneratorNotificationAddress::encode() const
{
	return {fisch::vx::word_access_type::Omnibus(m_value)};
}

void VectorGeneratorNotificationAddress::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        VectorGeneratorNotificationAddress::config_size_in_words> const& data)
{
	m_value = Value(data[0].word.value());
}

std::ostream& operator<<(std::ostream& os, VectorGeneratorNotificationAddress const& config)
{
	os << "VectorGeneratorNotificationAddress(" << config.m_value << ")";
	return os;
}


bool VectorGeneratorTrigger::operator==(VectorGeneratorTrigger const&) const
{
	return true;
}

bool VectorGeneratorTrigger::operator!=(VectorGeneratorTrigger const& other) const
{
	return !(*this == other);
}

std::
    array<halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorTrigger::write_config_size_in_words>
    VectorGeneratorTrigger::write_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    vector_generator_base_addresses.at(coord.toEnum()) + 0x2)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorTrigger::read_config_size_in_words>
VectorGeneratorTrigger::read_addresses(coordinate_type const&)
{
	return {};
}

std::array<fisch::vx::word_access_type::Omnibus, VectorGeneratorTrigger::write_config_size_in_words>
VectorGeneratorTrigger::encode() const
{
	return {fisch::vx::word_access_type::Omnibus()};
}

void VectorGeneratorTrigger::decode(std::array<
                                    fisch::vx::word_access_type::Omnibus,
                                    VectorGeneratorTrigger::read_config_size_in_words> const&)
{}

std::ostream& operator<<(std::ostream& os, VectorGeneratorTrigger const&)
{
	os << "VectorGeneratorTrigger()";
	return os;
}


VectorGeneratorFIFOWord::VectorGeneratorFIFOWord() :
    m_values(), m_last({false, false, false, false}), m_enable({false, false, false, false})
{}

VectorGeneratorFIFOWord::Values const& VectorGeneratorFIFOWord::get_values() const
{
	return m_values;
}

void VectorGeneratorFIFOWord::set_values(Values const value)
{
	m_values = value;
}

VectorGeneratorFIFOWord::Enables const& VectorGeneratorFIFOWord::get_last() const
{
	return m_last;
}

void VectorGeneratorFIFOWord::set_last(Enables const value)
{
	m_last = value;
}

VectorGeneratorFIFOWord::Enables const& VectorGeneratorFIFOWord::get_enable() const
{
	return m_enable;
}

void VectorGeneratorFIFOWord::set_enable(Enables const value)
{
	m_enable = value;
}

bool VectorGeneratorFIFOWord::operator==(VectorGeneratorFIFOWord const& other) const
{
	return (m_values == other.m_values) && (m_last == other.m_last) && (m_enable == other.m_enable);
}

bool VectorGeneratorFIFOWord::operator!=(VectorGeneratorFIFOWord const& other) const
{
	return !(*this == other);
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    VectorGeneratorFIFOWord::write_config_size_in_words>
VectorGeneratorFIFOWord::write_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    vector_generator_base_addresses.at(coord.toEnum()) + 0x200)};
}

std::
    array<halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorFIFOWord::read_config_size_in_words>
    VectorGeneratorFIFOWord::read_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct VectorGeneratorFIFOWordBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t value_3      : 5;
			uint32_t /* unused */ : 2;
			uint32_t last_3       : 1;
			uint32_t value_2      : 5;
			uint32_t /* unused */ : 2;
			uint32_t last_2       : 1;
			uint32_t value_1      : 5;
			uint32_t /* unused */ : 2;
			uint32_t last_1       : 1;
			uint32_t value_0      : 5;
			uint32_t /* unused */ : 2;
			uint32_t last_0       : 1;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	VectorGeneratorFIFOWordBitfield()
	{
		u.raw = 0u;
	}

	VectorGeneratorFIFOWordBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::
    array<fisch::vx::word_access_type::Omnibus, VectorGeneratorFIFOWord::write_config_size_in_words>
    VectorGeneratorFIFOWord::encode() const
{
	using namespace halco::hicann_dls::vx;
	VectorGeneratorFIFOWordBitfield bitfield;
	bitfield.u.m.value_0 = m_values.at(EntryOnQuad(0));
	bitfield.u.m.value_1 = m_values.at(EntryOnQuad(1));
	bitfield.u.m.value_2 = m_values.at(EntryOnQuad(2));
	bitfield.u.m.value_3 = m_values.at(EntryOnQuad(3));
	bitfield.u.m.last_0 = m_last.at(EntryOnQuad(0));
	bitfield.u.m.last_1 = m_last.at(EntryOnQuad(1));
	bitfield.u.m.last_2 = m_last.at(EntryOnQuad(2));
	bitfield.u.m.last_3 = m_last.at(EntryOnQuad(3));
	fisch::vx::word_access_type::Omnibus::ByteEnables enables = {
	    m_enable.at(EntryOnQuad(3)), m_enable.at(EntryOnQuad(2)), m_enable.at(EntryOnQuad(1)),
	    m_enable.at(EntryOnQuad(0))};
	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw, enables)};
}

void VectorGeneratorFIFOWord::decode(std::array<
                                     fisch::vx::word_access_type::Omnibus,
                                     VectorGeneratorFIFOWord::read_config_size_in_words> const&)
{}

std::ostream& operator<<(std::ostream& os, VectorGeneratorFIFOWord const& data)
{
	os << "VectorGeneratorFIFOWord(" << std::endl
	   << "values: [" << hate::join(data.m_values, ", ") << "]" << std::endl
	   << "enable: [" << hate::join(data.m_enable, ", ") << "]" << std::endl
	   << "last: [" << hate::join(data.m_last, ", ") << "]" << std::endl
	   << ")";
	return os;
}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::VectorGeneratorControl)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::VectorGeneratorLUTEntry)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::VectorGeneratorNotificationAddress)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::VectorGeneratorFIFOWord)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::VectorGeneratorTrigger)
