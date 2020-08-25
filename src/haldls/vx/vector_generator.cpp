#include "haldls/vx/vector_generator.h"

#include <string>

#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"

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

std::array<fisch::vx::Omnibus, VectorGeneratorControl::write_config_size_in_words>
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
	return {fisch::vx::Omnibus(fisch::vx::Omnibus::Value(bitfield.u.raw[0]))};
}

void VectorGeneratorControl::decode(
    std::array<fisch::vx::Omnibus, VectorGeneratorControl::read_config_size_in_words> const&)
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
	ss << "\tsource_waits: [" << hate::join_string(config.m_source_waits, ", ") << "],"
	   << "\n";
	ss << "\tsources: [" << hate::join_string(config.m_sources, ", ") << "],"
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

template <class Archive>
void VectorGeneratorControl::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_source_count));
	ar(CEREAL_NVP(m_resend_count));
	ar(CEREAL_NVP(m_source_waits));
	ar(CEREAL_NVP(m_sources));
	ar(CEREAL_NVP(m_event_pack));
	ar(CEREAL_NVP(m_enable_ignore_zeros));
	ar(CEREAL_NVP(m_lookup_table_entry_start));
	ar(CEREAL_NVP(m_notification));
	ar(CEREAL_NVP(m_trigger));
	ar(CEREAL_NVP(m_signal));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(VectorGeneratorControl)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::VectorGeneratorControl, 0)
