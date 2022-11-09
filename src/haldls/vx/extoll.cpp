#include "haldls/vx/extoll.h"

#include <iomanip>

#include "fisch/vx/word_access/type/extoll.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/extoll.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/extoll_constants.h"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"
#include "hate/math.h"

namespace haldls {
namespace vx {


/**
 * implementation of EventSwitchSource for Coord
 * EventSwitchSourceOnFPGA
 */

EventSwitchSource::Source const EventSwitchSource::Source::executor{0};
EventSwitchSource::Source const EventSwitchSource::Source::asic{1};
EventSwitchSource::Source const EventSwitchSource::Source::external{2};
EventSwitchSource::Source const EventSwitchSource::Source::off{3};

EventSwitchSource::EventSwitchSource() : m_source(Source::asic) {}

EventSwitchSource::Source EventSwitchSource::get_source() const
{
	return m_source;
}

void EventSwitchSource::set_source(Source const value)
{
	m_source = value;
}

bool EventSwitchSource::operator==(EventSwitchSource const& other) const
{
	return (m_source == other.m_source);
}

bool EventSwitchSource::operator!=(EventSwitchSource const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, EventSwitchSource const& config)
{
	std::stringstream ss;
	ss << "EventSwitchSource(source: " << std::hex << config.m_source << ")";
	return (os << ss.str());
}

std::array<halco::hicann_dls::vx::OmnibusAddress, EventSwitchSource::config_size_in_words>
EventSwitchSource::addresses(coordinate_type const& coord)
{
	halco::hicann_dls::vx::OmnibusAddress addr;

	if (coord == coordinate_type::to_executor) {
		return {halco::hicann_dls::vx::OmnibusAddress(evswitch_to_exec_source_address)};
	}
	if (coord == coordinate_type::to_asic) {
		return {halco::hicann_dls::vx::OmnibusAddress(evswitch_to_asic_source_address)};
	}
	if (coord == coordinate_type::to_external) {
		return {halco::hicann_dls::vx::OmnibusAddress(evswitch_to_external_source_address)};
	}

	return {halco::hicann_dls::vx::OmnibusAddress(evswitch_to_exec_source_address)};
}

namespace {

struct EventSwitchSourceBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t source     :  2;
			uint32_t /*unused*/ : 30;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	EventSwitchSourceBitfield()
	{
		u.raw = 0u;
	}

	EventSwitchSourceBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

void EventSwitchSource::decode(
    std::array<fisch::vx::word_access_type::Omnibus, EventSwitchSource::config_size_in_words> const&
        data)
{
	EventSwitchSourceBitfield bitfield;
	bitfield.u.raw = data[0];

	m_source = Source(bitfield.u.m.source);
}

std::array<fisch::vx::word_access_type::Omnibus, EventSwitchSource::config_size_in_words>
EventSwitchSource::encode() const
{
	EventSwitchSourceBitfield bitfield;
	bitfield.u.m.source = m_source;

	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw)};
}

template <class Archive>
void EventSwitchSource::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_source));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(EventSwitchSource)


/**
 * implementation of ExtollSpikeCommBucketTriggerConfig for Coord
 * ExtollSpikeCommBucketTriggerConfigOnFPGA
 */

ExtollSpikeCommBucketTriggerConfig::ExtollSpikeCommBucketTriggerConfig() :
    m_pulse_timeout(), m_packet_timeout()
{}

ExtollSpikeCommBucketTriggerConfig::PulseTimeout
ExtollSpikeCommBucketTriggerConfig::get_pulse_timeout() const
{
	return m_pulse_timeout;
}

void ExtollSpikeCommBucketTriggerConfig::set_pulse_timeout(PulseTimeout const value)
{
	m_pulse_timeout = value;
}

ExtollSpikeCommBucketTriggerConfig::PacketTimeout
ExtollSpikeCommBucketTriggerConfig::get_packet_timeout() const
{
	return m_packet_timeout;
}

void ExtollSpikeCommBucketTriggerConfig::set_packet_timeout(PacketTimeout const value)
{
	m_packet_timeout = value;
}

bool ExtollSpikeCommBucketTriggerConfig::operator==(
    ExtollSpikeCommBucketTriggerConfig const& other) const
{
	return (m_pulse_timeout == other.m_pulse_timeout) &&
	       (m_packet_timeout == other.m_packet_timeout);
}

bool ExtollSpikeCommBucketTriggerConfig::operator!=(
    ExtollSpikeCommBucketTriggerConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommBucketTriggerConfig const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommBucketTriggerConfig(pulse timeout: " << std::hex << config.m_pulse_timeout
	   << ", packet timeout: " << std::hex << config.m_packet_timeout << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommBucketTriggerConfig::config_size_in_words>
ExtollSpikeCommBucketTriggerConfig::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_bucket_rf_addresses.at(coord.toEnum()) + static_cast<int>(BucketRegs::TRIGGER))};
}

namespace {

struct ExtollSpikeCommBucketTriggerConfigBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t pulse_timeout  :  8;
			uint64_t packet_timeout :  8;
			uint64_t /* unused */   : 48;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommBucketTriggerConfigBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommBucketTriggerConfigBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommBucketTriggerConfig::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommBucketTriggerConfig::config_size_in_words> const& data)
{
	ExtollSpikeCommBucketTriggerConfigBitfield bitfield;
	bitfield.u.raw = data[0];

	m_pulse_timeout = PulseTimeout(bitfield.u.m.pulse_timeout);
	m_packet_timeout = PacketTimeout(bitfield.u.m.packet_timeout);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommBucketTriggerConfig::config_size_in_words>
ExtollSpikeCommBucketTriggerConfig::encode() const
{
	ExtollSpikeCommBucketTriggerConfigBitfield bitfield;
	bitfield.u.m.pulse_timeout = m_pulse_timeout;
	bitfield.u.m.packet_timeout = m_packet_timeout;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommBucketTriggerConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_pulse_timeout));
	ar(CEREAL_NVP(m_packet_timeout));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketTriggerConfig)


/**
 * implementation of ExtollSpikeCommBucketDestinationConfig for Coord
 * ExtollSpikeCommBucketDestinationConfigOnFPGA
 */

ExtollSpikeCommBucketDestinationConfig::ExtollSpikeCommBucketDestinationConfig() :
    m_destination_node_id(), m_enable_multicast(), m_destination_delay()
{}

ExtollSpikeCommBucketDestinationConfig::DestinationNodeID
ExtollSpikeCommBucketDestinationConfig::get_destination_node_id() const
{
	return m_destination_node_id;
}

void ExtollSpikeCommBucketDestinationConfig::set_destination_node_id(DestinationNodeID const value)
{
	m_destination_node_id = value;
}

bool ExtollSpikeCommBucketDestinationConfig::get_enable_multicast() const
{
	return m_enable_multicast;
}

void ExtollSpikeCommBucketDestinationConfig::set_enable_multicast(bool const value)
{
	m_enable_multicast = value;
}

ExtollSpikeCommBucketDestinationConfig::DestinationDelay
ExtollSpikeCommBucketDestinationConfig::get_destination_delay() const
{
	return m_destination_delay;
}

void ExtollSpikeCommBucketDestinationConfig::set_destination_delay(DestinationDelay const value)
{
	m_destination_delay = value;
}

bool ExtollSpikeCommBucketDestinationConfig::operator==(
    ExtollSpikeCommBucketDestinationConfig const& other) const
{
	return (m_destination_node_id == other.m_destination_node_id) &&
	       (m_enable_multicast == other.m_enable_multicast) &&
	       (m_destination_delay == other.m_destination_delay);
}

bool ExtollSpikeCommBucketDestinationConfig::operator!=(
    ExtollSpikeCommBucketDestinationConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommBucketDestinationConfig const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommBucketDestinationConfig(destination node id: " << std::hex
	   << config.m_destination_node_id << ", is multicast: " << std::hex
	   << config.m_enable_multicast << ", destination delay: " << std::hex
	   << config.m_destination_delay << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommBucketDestinationConfig::config_size_in_words>
ExtollSpikeCommBucketDestinationConfig::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_bucket_rf_addresses.at(coord.toEnum()) +
	    static_cast<int>(BucketRegs::DESTINATION))};
}

namespace {

struct ExtollSpikeCommBucketDestinationConfigBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t destination_node_id : 16;
			uint64_t enable_multicast    :  1;
			uint64_t destination_delay   : 10;
			uint64_t /* unused */        : 37;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommBucketDestinationConfigBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommBucketDestinationConfigBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommBucketDestinationConfig::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommBucketDestinationConfig::config_size_in_words> const& data)
{
	ExtollSpikeCommBucketDestinationConfigBitfield bitfield;
	bitfield.u.raw = data[0];

	m_destination_node_id = DestinationNodeID(bitfield.u.m.destination_node_id);
	m_enable_multicast = bitfield.u.m.enable_multicast;
	m_destination_delay = DestinationDelay(bitfield.u.m.destination_delay);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommBucketDestinationConfig::config_size_in_words>
ExtollSpikeCommBucketDestinationConfig::encode() const
{
	ExtollSpikeCommBucketDestinationConfigBitfield bitfield;
	bitfield.u.m.destination_node_id = m_destination_node_id;
	bitfield.u.m.enable_multicast = m_enable_multicast;
	bitfield.u.m.destination_delay = m_destination_delay;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommBucketDestinationConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_destination_node_id));
	ar(CEREAL_NVP(m_enable_multicast));
	ar(CEREAL_NVP(m_destination_delay));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketDestinationConfig)


/**
 * implementation of ExtollSpikeCommBucketNumPktsSent for Coord
 * ExtollSpikeCommBucketNumPktsSentOnFPGA
 */

ExtollSpikeCommBucketNumPktsSent::ExtollSpikeCommBucketNumPktsSent() : m_packet_count() {}

ExtollSpikeCommBucketNumPktsSent::PacketCount ExtollSpikeCommBucketNumPktsSent::get_packet_count()
    const
{
	return m_packet_count;
}

void ExtollSpikeCommBucketNumPktsSent::set_packet_count(PacketCount const value)
{
	m_packet_count = value;
}

bool ExtollSpikeCommBucketNumPktsSent::operator==(
    ExtollSpikeCommBucketNumPktsSent const& other) const
{
	return (m_packet_count == other.m_packet_count);
}

bool ExtollSpikeCommBucketNumPktsSent::operator!=(
    ExtollSpikeCommBucketNumPktsSent const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommBucketNumPktsSent const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommBucketNumPktsSent(packet count: " << std::hex << config.m_packet_count
	   << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommBucketNumPktsSent::read_config_size_in_words>
ExtollSpikeCommBucketNumPktsSent::read_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_bucket_rf_addresses.at(coord.toEnum()) + static_cast<int>(BucketRegs::PKTS_SENT))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommBucketNumPktsSent::write_config_size_in_words>
ExtollSpikeCommBucketNumPktsSent::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommBucketNumPktsSentBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t packet_count : 48;
			uint64_t /* unused */ : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommBucketNumPktsSentBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommBucketNumPktsSentBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommBucketNumPktsSent::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommBucketNumPktsSent::read_config_size_in_words> const& data)
{
	ExtollSpikeCommBucketNumPktsSentBitfield bitfield;
	bitfield.u.raw = data[0];

	m_packet_count = PacketCount(bitfield.u.m.packet_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommBucketNumPktsSent::write_config_size_in_words>
ExtollSpikeCommBucketNumPktsSent::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommBucketNumPktsSent::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_packet_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketNumPktsSent)


/**
 * implementation of ExtollSpikeCommBucketCounterReset for Coord
 * ExtollSpikeCommBucketCounterResetOnFPGA
 */

ExtollSpikeCommBucketCounterReset::ExtollSpikeCommBucketCounterReset() {}

bool ExtollSpikeCommBucketCounterReset::operator==(
    ExtollSpikeCommBucketCounterReset const& /*other*/) const
{
	return true;
}

bool ExtollSpikeCommBucketCounterReset::operator!=(
    ExtollSpikeCommBucketCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommBucketCounterReset const& /*config*/)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommBucketCounterReset()";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommBucketCounterReset::write_config_size_in_words>
ExtollSpikeCommBucketCounterReset::write_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_bucket_rf_addresses.at(coord.toEnum()) + static_cast<int>(BucketRegs::CNT_RES))};
}
std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommBucketCounterReset::read_config_size_in_words>
ExtollSpikeCommBucketCounterReset::read_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommBucketCounterResetBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t counter_reset :  1;
			uint64_t /* unused */  : 63;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommBucketCounterResetBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommBucketCounterResetBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommBucketCounterReset::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommBucketCounterReset::read_config_size_in_words> const&)
{}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommBucketCounterReset::write_config_size_in_words>
ExtollSpikeCommBucketCounterReset::encode() const
{
	ExtollSpikeCommBucketCounterResetBitfield bitfield;
	bitfield.u.m.counter_reset = 0x1;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommBucketCounterReset::serialize(Archive& /*ar*/, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketCounterReset)


/**
 * implementation of ExtollSpikeCommRouterLookupConfig for Coord
 * ExtollSpikeCommRouterLookupConfigOnFPGA
 */


ExtollSpikeCommRouterLookupConfig::ExtollSpikeCommRouterLookupConfig() :
    m_entry_valid(false), m_destination_event(), m_bucket_id()
{}

bool ExtollSpikeCommRouterLookupConfig::get_entry_valid() const
{
	return m_entry_valid;
}

void ExtollSpikeCommRouterLookupConfig::set_entry_valid(bool const value)
{
	m_entry_valid = value;
}

ExtollSpikeCommRouterLookupConfig::DestinationEvent
ExtollSpikeCommRouterLookupConfig::get_destination_event() const
{
	return m_destination_event;
}

void ExtollSpikeCommRouterLookupConfig::set_destination_event(DestinationEvent const value)
{
	m_destination_event = value;
}

ExtollSpikeCommRouterLookupConfig::BucketID ExtollSpikeCommRouterLookupConfig::get_bucket_id() const
{
	return m_bucket_id;
}

void ExtollSpikeCommRouterLookupConfig::set_bucket_id(BucketID const value)
{
	m_bucket_id = value;
}

bool ExtollSpikeCommRouterLookupConfig::operator==(
    ExtollSpikeCommRouterLookupConfig const& other) const
{
	return (m_destination_event == other.m_destination_event) &&
	       (m_bucket_id == other.m_bucket_id) && (m_entry_valid == other.m_entry_valid);
}

bool ExtollSpikeCommRouterLookupConfig::operator!=(
    ExtollSpikeCommRouterLookupConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterLookupConfig const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommRouterLookupConfig(destination event: " << std::hex
	   << config.m_destination_event << ", bucket id: " << std::hex << config.m_bucket_id
	   << ", entry valid: " << std::hex << config.m_entry_valid << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterLookupConfig::config_size_in_words>
ExtollSpikeCommRouterLookupConfig::addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();
	auto coordOnSplit = coord.toExtollSpikeCommRouterLookupConfigOnSpikeCommSplit();

	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_router_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(RouterRegs::SEND_LOOKUP) + (coordOnSplit.toEnum() * 0x8))};
}

namespace {

struct ExtollSpikeCommRouterLookupConfigBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t bucket_id         :  3;
			uint64_t destination_event : 14;
			uint64_t entry_valid       :  1;
			uint64_t /* unused */      : 46;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommRouterLookupConfigBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommRouterLookupConfigBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommRouterLookupConfig::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommRouterLookupConfig::config_size_in_words> const& data)
{
	ExtollSpikeCommRouterLookupConfigBitfield bitfield;
	bitfield.u.raw = data[0];

	m_entry_valid = bitfield.u.m.entry_valid;
	m_destination_event = DestinationEvent(bitfield.u.m.destination_event);
	m_bucket_id = BucketID(bitfield.u.m.bucket_id);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommRouterLookupConfig::config_size_in_words>
ExtollSpikeCommRouterLookupConfig::encode() const
{
	ExtollSpikeCommRouterLookupConfigBitfield bitfield;

	bitfield.u.m.entry_valid = m_entry_valid;
	bitfield.u.m.destination_event = m_destination_event;
	bitfield.u.m.bucket_id = m_bucket_id;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommRouterLookupConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_entry_valid));
	ar(CEREAL_NVP(m_destination_event));
	ar(CEREAL_NVP(m_bucket_id));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterLookupConfig)


/**
 * implementation of ExtollSpikeCommRouterConfig for Coord ExtollSpikeCommRouterConfigOnFPGA
 */


ExtollSpikeCommRouterConfig::ExtollSpikeCommRouterConfig() :
    m_enable_routing(), m_enable_drop_if_routing_disabled()
{}

bool ExtollSpikeCommRouterConfig::get_enable_routing() const
{
	return m_enable_routing;
}

void ExtollSpikeCommRouterConfig::set_enable_routing(bool const value)
{
	m_enable_routing = value;
}

bool ExtollSpikeCommRouterConfig::get_enable_drop_if_routing_disabled() const
{
	return m_enable_drop_if_routing_disabled;
}

void ExtollSpikeCommRouterConfig::set_enable_drop_if_routing_disabled(bool const value)
{
	m_enable_drop_if_routing_disabled = value;
}

bool ExtollSpikeCommRouterConfig::operator==(ExtollSpikeCommRouterConfig const& other) const
{
	return (m_enable_routing == other.m_enable_routing) &&
	       (m_enable_drop_if_routing_disabled == other.m_enable_drop_if_routing_disabled);
}

bool ExtollSpikeCommRouterConfig::operator!=(ExtollSpikeCommRouterConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterConfig const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommRouterConfig(enable routing: " << std::hex << config.m_enable_routing
	   << ", drop if routing disabled: " << std::hex << config.m_enable_drop_if_routing_disabled
	   << ")";
	return (os << ss.str());
}

std::array<halco::hicann_dls::vx::ExtollAddress, ExtollSpikeCommRouterConfig::config_size_in_words>
ExtollSpikeCommRouterConfig::addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_router_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(RouterRegs::CONFIG))};
}

namespace {

struct ExtollSpikeCommRouterConfigBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t enable_routing                  :  1;
			uint64_t enable_drop_if_routing_disabled :  1;
			uint64_t /* unused */                    : 62;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommRouterConfigBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommRouterConfigBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommRouterConfig::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommRouterConfig::config_size_in_words> const& data)
{
	ExtollSpikeCommRouterConfigBitfield bitfield;
	bitfield.u.raw = data[0];

	m_enable_routing = bitfield.u.m.enable_routing;
	m_enable_drop_if_routing_disabled = bitfield.u.m.enable_drop_if_routing_disabled;
}

std::array<fisch::vx::word_access_type::Extoll, ExtollSpikeCommRouterConfig::config_size_in_words>
ExtollSpikeCommRouterConfig::encode() const
{
	ExtollSpikeCommRouterConfigBitfield bitfield;

	bitfield.u.m.enable_routing = m_enable_routing;
	bitfield.u.m.enable_drop_if_routing_disabled = m_enable_drop_if_routing_disabled;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommRouterConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_routing));
	ar(CEREAL_NVP(m_enable_drop_if_routing_disabled));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterConfig)


/**
 * implementation of ExtollSpikeCommRouterEventLossDisabled for Coord
 * ExtollSpikeCommRouterEventLossDisabledOnFPGA
 */

ExtollSpikeCommRouterEventLossDisabled::ExtollSpikeCommRouterEventLossDisabled() :
    m_lost_event_count()
{}

ExtollSpikeCommRouterEventLossDisabled::LostEventCount
ExtollSpikeCommRouterEventLossDisabled::get_lost_event_count() const
{
	return m_lost_event_count;
}

void ExtollSpikeCommRouterEventLossDisabled::set_lost_event_count(LostEventCount const value)
{
	m_lost_event_count = value;
}

bool ExtollSpikeCommRouterEventLossDisabled::operator==(
    ExtollSpikeCommRouterEventLossDisabled const& other) const
{
	return m_lost_event_count == other.m_lost_event_count;
}

bool ExtollSpikeCommRouterEventLossDisabled::operator!=(
    ExtollSpikeCommRouterEventLossDisabled const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterEventLossDisabled const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommRouterEventLossDisabled(lost events due to disabled routing: " << std::hex
	   << config.m_lost_event_count << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterEventLossDisabled::read_config_size_in_words>
ExtollSpikeCommRouterEventLossDisabled::read_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_router_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(RouterRegs::EVT_LOSS_DISABLED))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterEventLossDisabled::write_config_size_in_words>
ExtollSpikeCommRouterEventLossDisabled::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommRouterEventLossDisabledBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t lost_event_count : 48;
			uint64_t /* unused */     : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommRouterEventLossDisabledBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommRouterEventLossDisabledBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommRouterEventLossDisabled::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommRouterEventLossDisabled::read_config_size_in_words> const& data)
{
	ExtollSpikeCommRouterEventLossDisabledBitfield bitfield;
	bitfield.u.raw = data[0];

	m_lost_event_count = LostEventCount(bitfield.u.m.lost_event_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommRouterEventLossDisabled::write_config_size_in_words>
ExtollSpikeCommRouterEventLossDisabled::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommRouterEventLossDisabled::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_lost_event_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterEventLossDisabled)


/**
 * implementation of ExtollSpikeCommRouterEventLossMisconf for Coord
 * ExtollSpikeCommRouterEventLossMisconfOnFPGA
 */

ExtollSpikeCommRouterEventLossMisconf::ExtollSpikeCommRouterEventLossMisconf() :
    m_lost_event_count()
{}

ExtollSpikeCommRouterEventLossMisconf::LostEventCount
ExtollSpikeCommRouterEventLossMisconf::get_lost_event_count() const
{
	return m_lost_event_count;
}

void ExtollSpikeCommRouterEventLossMisconf::set_lost_event_count(LostEventCount const value)
{
	m_lost_event_count = value;
}

bool ExtollSpikeCommRouterEventLossMisconf::operator==(
    ExtollSpikeCommRouterEventLossMisconf const& other) const
{
	return m_lost_event_count == other.m_lost_event_count;
}

bool ExtollSpikeCommRouterEventLossMisconf::operator!=(
    ExtollSpikeCommRouterEventLossMisconf const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterEventLossMisconf const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommRouterEventLossMisconf(lost events due to misconfigured table: "
	   << std::hex << config.m_lost_event_count << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterEventLossMisconf::read_config_size_in_words>
ExtollSpikeCommRouterEventLossMisconf::read_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_router_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(RouterRegs::EVT_LOSS_MISCONF))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterEventLossMisconf::write_config_size_in_words>
ExtollSpikeCommRouterEventLossMisconf::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommRouterEventLossMisconfBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t lost_event_count : 48;
			uint64_t /* unused */     : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommRouterEventLossMisconfBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommRouterEventLossMisconfBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommRouterEventLossMisconf::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommRouterEventLossMisconf::read_config_size_in_words> const& data)
{
	ExtollSpikeCommRouterEventLossMisconfBitfield bitfield;
	bitfield.u.raw = data[0];

	m_lost_event_count = LostEventCount(bitfield.u.m.lost_event_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommRouterEventLossMisconf::write_config_size_in_words>
ExtollSpikeCommRouterEventLossMisconf::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommRouterEventLossMisconf::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_lost_event_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterEventLossMisconf)


/**
 * implementation of ExtollSpikeCommRouterEventsRouted for Coord
 * ExtollSpikeCommRouterEventsRoutedOnFPGA
 */

ExtollSpikeCommRouterEventsRouted::ExtollSpikeCommRouterEventsRouted() : m_event_count() {}

ExtollSpikeCommRouterEventsRouted::EventCount ExtollSpikeCommRouterEventsRouted::get_event_count()
    const
{
	return m_event_count;
}

void ExtollSpikeCommRouterEventsRouted::set_event_count(EventCount const value)
{
	m_event_count = value;
}

bool ExtollSpikeCommRouterEventsRouted::operator==(
    ExtollSpikeCommRouterEventsRouted const& other) const
{
	return m_event_count == other.m_event_count;
}

bool ExtollSpikeCommRouterEventsRouted::operator!=(
    ExtollSpikeCommRouterEventsRouted const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterEventsRouted const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommRouterEventsRouted(routed events: " << std::hex << config.m_event_count
	   << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterEventsRouted::read_config_size_in_words>
ExtollSpikeCommRouterEventsRouted::read_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_router_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(RouterRegs::EVTS_ROUTED))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterEventsRouted::write_config_size_in_words>
ExtollSpikeCommRouterEventsRouted::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommRouterEventsRoutedBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t event_count  : 48;
			uint64_t /* unused */ : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommRouterEventsRoutedBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommRouterEventsRoutedBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommRouterEventsRouted::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommRouterEventsRouted::read_config_size_in_words> const& data)
{
	ExtollSpikeCommRouterEventsRoutedBitfield bitfield;
	bitfield.u.raw = data[0];

	m_event_count = EventCount(bitfield.u.m.event_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommRouterEventsRouted::write_config_size_in_words>
ExtollSpikeCommRouterEventsRouted::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommRouterEventsRouted::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_event_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterEventsRouted)


/**
 * implementation of ExtollSpikeCommRouterCounterReset for Coord
 * ExtollSpikeCommRouterCounterResetOnFPGA
 */

ExtollSpikeCommRouterCounterReset::ExtollSpikeCommRouterCounterReset() {}

bool ExtollSpikeCommRouterCounterReset::operator==(
    ExtollSpikeCommRouterCounterReset const& /*other*/) const
{
	return true;
}

bool ExtollSpikeCommRouterCounterReset::operator!=(
    ExtollSpikeCommRouterCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterCounterReset const& /*config*/)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommRouterCounterReset()";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterCounterReset::write_config_size_in_words>
ExtollSpikeCommRouterCounterReset::write_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    tx_accum_router_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(RouterRegs::CNT_RES))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommRouterCounterReset::read_config_size_in_words>
ExtollSpikeCommRouterCounterReset::read_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommRouterCounterResetBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t counter_reset :  1;
			uint64_t /* unused */  : 63;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommRouterCounterResetBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommRouterCounterResetBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommRouterCounterReset::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommRouterCounterReset::read_config_size_in_words> const&)
{}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommRouterCounterReset::write_config_size_in_words>
ExtollSpikeCommRouterCounterReset::encode() const
{
	ExtollSpikeCommRouterCounterResetBitfield bitfield;
	bitfield.u.m.counter_reset = 0x1;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommRouterCounterReset::serialize(Archive& /*ar*/, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterCounterReset)


/**
 * implementation of ExtollSpikeCommDecoderNumEventsReceived for Coord
 * ExtollSpikeCommDecoderNumEventsReceivedOnFPGA
 */

ExtollSpikeCommDecoderNumEventsReceived::ExtollSpikeCommDecoderNumEventsReceived() : m_event_count()
{}

ExtollSpikeCommDecoderNumEventsReceived::EventCount
ExtollSpikeCommDecoderNumEventsReceived::get_event_count() const
{
	return m_event_count;
}

void ExtollSpikeCommDecoderNumEventsReceived::set_event_count(EventCount const value)
{
	m_event_count = value;
}

bool ExtollSpikeCommDecoderNumEventsReceived::operator==(
    ExtollSpikeCommDecoderNumEventsReceived const& other) const
{
	return m_event_count == other.m_event_count;
}

bool ExtollSpikeCommDecoderNumEventsReceived::operator!=(
    ExtollSpikeCommDecoderNumEventsReceived const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommDecoderNumEventsReceived const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommDecoderNumEventsReceived(received events: " << std::hex
	   << config.m_event_count << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommDecoderNumEventsReceived::read_config_size_in_words>
ExtollSpikeCommDecoderNumEventsReceived::read_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    rx_decoder_rf_addr + rx_decoder_rf_evts_rcvd_addresses.at(coordSplitOnFPGA.toEnum()))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommDecoderNumEventsReceived::write_config_size_in_words>
ExtollSpikeCommDecoderNumEventsReceived::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommDecoderNumEventsReceivedBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t event_count  : 48;
			uint64_t /* unused */ : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommDecoderNumEventsReceivedBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommDecoderNumEventsReceivedBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommDecoderNumEventsReceived::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommDecoderNumEventsReceived::read_config_size_in_words> const& data)
{
	ExtollSpikeCommDecoderNumEventsReceivedBitfield bitfield;
	bitfield.u.raw = data[0];

	m_event_count = EventCount(bitfield.u.m.event_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommDecoderNumEventsReceived::write_config_size_in_words>
ExtollSpikeCommDecoderNumEventsReceived::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommDecoderNumEventsReceived::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_event_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommDecoderNumEventsReceived)


/**
 * implementation of ExtollSpikeCommDecoderCounterReset for Coord
 * ExtollSpikeCommDecoderCounterResetOnFPGA
 */

ExtollSpikeCommDecoderCounterReset::ExtollSpikeCommDecoderCounterReset() {}

bool ExtollSpikeCommDecoderCounterReset::operator==(
    ExtollSpikeCommDecoderCounterReset const& /*other*/) const
{
	return true;
}

bool ExtollSpikeCommDecoderCounterReset::operator!=(
    ExtollSpikeCommDecoderCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommDecoderCounterReset const& /*config*/)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommDecoderCounterReset()";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommDecoderCounterReset::write_config_size_in_words>
ExtollSpikeCommDecoderCounterReset::write_addresses(coordinate_type const&)
{
	return {halco::hicann_dls::vx::ExtollAddress(rx_decoder_rf_addr + DecoderRegs::CNT_RES)};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommDecoderCounterReset::read_config_size_in_words>
ExtollSpikeCommDecoderCounterReset::read_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommDecoderCounterResetBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t counter_reset :  1;
			uint64_t /* unused */  : 63;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommDecoderCounterResetBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommDecoderCounterResetBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommDecoderCounterReset::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommDecoderCounterReset::read_config_size_in_words> const&)
{}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommDecoderCounterReset::write_config_size_in_words>
ExtollSpikeCommDecoderCounterReset::encode() const
{
	ExtollSpikeCommDecoderCounterResetBitfield bitfield;
	bitfield.u.m.counter_reset = 0x1;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommDecoderCounterReset::serialize(Archive& /*ar*/, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommDecoderCounterReset)


/**
 * implementation of ExtollSpikeCommTimestampDelayConfig for Coord
 * ExtollSpikeCommTimestampDelayConfigOnFPGA
 */


ExtollSpikeCommTimestampDelayConfig::ExtollSpikeCommTimestampDelayConfig() :
    m_forward_immediately(), m_timestamp_to_systime()
{}

bool ExtollSpikeCommTimestampDelayConfig::get_forward_immediately() const
{
	return m_forward_immediately;
}

void ExtollSpikeCommTimestampDelayConfig::set_forward_immediately(bool const value)
{
	m_forward_immediately = value;
}

bool ExtollSpikeCommTimestampDelayConfig::get_timestamp_to_systime() const
{
	return m_timestamp_to_systime;
}

void ExtollSpikeCommTimestampDelayConfig::set_timestamp_to_systime(bool const value)
{
	m_timestamp_to_systime = value;
}

bool ExtollSpikeCommTimestampDelayConfig::operator==(
    ExtollSpikeCommTimestampDelayConfig const& other) const
{
	return (m_forward_immediately == other.m_forward_immediately) &&
	       (m_timestamp_to_systime == other.m_timestamp_to_systime);
}

bool ExtollSpikeCommTimestampDelayConfig::operator!=(
    ExtollSpikeCommTimestampDelayConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommTimestampDelayConfig const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommTimestampDelayConfig(enable routing: " << std::hex
	   << config.m_forward_immediately << ", drop if routing disabled: " << std::hex
	   << config.m_timestamp_to_systime << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayConfig::config_size_in_words>
ExtollSpikeCommTimestampDelayConfig::addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    rx_timestamp_delay_buffer_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(TimestampDelayRegs::CONFIG))};
}

namespace {

struct ExtollSpikeCommTimestampDelayConfigBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t forward_immediately  :  1;
			uint64_t timestamp_to_systime :  1;
			uint64_t /* unused */         : 62;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommTimestampDelayConfigBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommTimestampDelayConfigBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommTimestampDelayConfig::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommTimestampDelayConfig::config_size_in_words> const& data)
{
	ExtollSpikeCommTimestampDelayConfigBitfield bitfield;
	bitfield.u.raw = data[0];

	m_forward_immediately = bitfield.u.m.forward_immediately;
	m_timestamp_to_systime = bitfield.u.m.timestamp_to_systime;
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommTimestampDelayConfig::config_size_in_words>
ExtollSpikeCommTimestampDelayConfig::encode() const
{
	ExtollSpikeCommTimestampDelayConfigBitfield bitfield;

	bitfield.u.m.forward_immediately = m_forward_immediately;
	bitfield.u.m.timestamp_to_systime = m_timestamp_to_systime;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommTimestampDelayConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_forward_immediately));
	ar(CEREAL_NVP(m_timestamp_to_systime));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayConfig)


/**
 * implementation of ExtollSpikeCommTimestampDelayNumEventsReceived for Coord
 * ExtollSpikeCommTimestampDelayNumEventsReceivedOnFPGA
 */

ExtollSpikeCommTimestampDelayNumEventsReceived::ExtollSpikeCommTimestampDelayNumEventsReceived() :
    m_event_count()
{}

ExtollSpikeCommTimestampDelayNumEventsReceived::EventCount
ExtollSpikeCommTimestampDelayNumEventsReceived::get_event_count() const
{
	return m_event_count;
}

void ExtollSpikeCommTimestampDelayNumEventsReceived::set_event_count(EventCount const value)
{
	m_event_count = value;
}

bool ExtollSpikeCommTimestampDelayNumEventsReceived::operator==(
    ExtollSpikeCommTimestampDelayNumEventsReceived const& other) const
{
	return m_event_count == other.m_event_count;
}

bool ExtollSpikeCommTimestampDelayNumEventsReceived::operator!=(
    ExtollSpikeCommTimestampDelayNumEventsReceived const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(
    std::ostream& os, ExtollSpikeCommTimestampDelayNumEventsReceived const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommTimestampDelayNumEventsReceived(received events: " << std::hex
	   << config.m_event_count << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayNumEventsReceived::read_config_size_in_words>
ExtollSpikeCommTimestampDelayNumEventsReceived::read_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    rx_timestamp_delay_buffer_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(TimestampDelayRegs::EVTS_RCVD))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayNumEventsReceived::write_config_size_in_words>
ExtollSpikeCommTimestampDelayNumEventsReceived::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommTimestampDelayNumEventsReceivedBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t event_count  : 48;
			uint64_t /* unused */ : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommTimestampDelayNumEventsReceivedBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommTimestampDelayNumEventsReceivedBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommTimestampDelayNumEventsReceived::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommTimestampDelayNumEventsReceived::read_config_size_in_words> const& data)
{
	ExtollSpikeCommTimestampDelayNumEventsReceivedBitfield bitfield;
	bitfield.u.raw = data[0];

	m_event_count = EventCount(bitfield.u.m.event_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommTimestampDelayNumEventsReceived::write_config_size_in_words>
ExtollSpikeCommTimestampDelayNumEventsReceived::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommTimestampDelayNumEventsReceived::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_event_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayNumEventsReceived)


/**
 * implementation of ExtollSpikeCommTimestampDelayEventLossFull for Coord
 * ExtollSpikeCommTimestampDelayEventLossFullOnFPGA
 */

ExtollSpikeCommTimestampDelayEventLossFull::ExtollSpikeCommTimestampDelayEventLossFull() :
    m_event_count()
{}

ExtollSpikeCommTimestampDelayEventLossFull::EventCount
ExtollSpikeCommTimestampDelayEventLossFull::get_event_count() const
{
	return m_event_count;
}

void ExtollSpikeCommTimestampDelayEventLossFull::set_event_count(EventCount const value)
{
	m_event_count = value;
}

bool ExtollSpikeCommTimestampDelayEventLossFull::operator==(
    ExtollSpikeCommTimestampDelayEventLossFull const& other) const
{
	return m_event_count == other.m_event_count;
}

bool ExtollSpikeCommTimestampDelayEventLossFull::operator!=(
    ExtollSpikeCommTimestampDelayEventLossFull const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommTimestampDelayEventLossFull const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommTimestampDelayEventLossFull(received events: " << std::hex
	   << config.m_event_count << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayEventLossFull::read_config_size_in_words>
ExtollSpikeCommTimestampDelayEventLossFull::read_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    rx_timestamp_delay_buffer_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(TimestampDelayRegs::EVT_LOSS_FULL))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayEventLossFull::write_config_size_in_words>
ExtollSpikeCommTimestampDelayEventLossFull::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommTimestampDelayEventLossFullBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t event_count  : 48;
			uint64_t /* unused */ : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommTimestampDelayEventLossFullBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommTimestampDelayEventLossFullBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommTimestampDelayEventLossFull::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommTimestampDelayEventLossFull::read_config_size_in_words> const& data)
{
	ExtollSpikeCommTimestampDelayEventLossFullBitfield bitfield;
	bitfield.u.raw = data[0];

	m_event_count = EventCount(bitfield.u.m.event_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommTimestampDelayEventLossFull::write_config_size_in_words>
ExtollSpikeCommTimestampDelayEventLossFull::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommTimestampDelayEventLossFull::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_event_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayEventLossFull)


/**
 * implementation of ExtollSpikeCommTimestampDelayEventLossExpired for Coord
 * ExtollSpikeCommTimestampDelayEventLossExpiredOnFPGA
 */

ExtollSpikeCommTimestampDelayEventLossExpired::ExtollSpikeCommTimestampDelayEventLossExpired() :
    m_event_count()
{}

ExtollSpikeCommTimestampDelayEventLossExpired::EventCount
ExtollSpikeCommTimestampDelayEventLossExpired::get_event_count() const
{
	return m_event_count;
}

void ExtollSpikeCommTimestampDelayEventLossExpired::set_event_count(EventCount const value)
{
	m_event_count = value;
}

bool ExtollSpikeCommTimestampDelayEventLossExpired::operator==(
    ExtollSpikeCommTimestampDelayEventLossExpired const& other) const
{
	return m_event_count == other.m_event_count;
}

bool ExtollSpikeCommTimestampDelayEventLossExpired::operator!=(
    ExtollSpikeCommTimestampDelayEventLossExpired const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(
    std::ostream& os, ExtollSpikeCommTimestampDelayEventLossExpired const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommTimestampDelayEventLossExpired(received events: " << std::hex
	   << config.m_event_count << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayEventLossExpired::read_config_size_in_words>
ExtollSpikeCommTimestampDelayEventLossExpired::read_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    rx_timestamp_delay_buffer_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(TimestampDelayRegs::EVT_LOSS_EXPIRED))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayEventLossExpired::write_config_size_in_words>
ExtollSpikeCommTimestampDelayEventLossExpired::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommTimestampDelayEventLossExpiredBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t event_count  : 48;
			uint64_t /* unused */ : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommTimestampDelayEventLossExpiredBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommTimestampDelayEventLossExpiredBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommTimestampDelayEventLossExpired::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommTimestampDelayEventLossExpired::read_config_size_in_words> const& data)
{
	ExtollSpikeCommTimestampDelayEventLossExpiredBitfield bitfield;
	bitfield.u.raw = data[0];

	m_event_count = EventCount(bitfield.u.m.event_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommTimestampDelayEventLossExpired::write_config_size_in_words>
ExtollSpikeCommTimestampDelayEventLossExpired::encode() const
{
	return {};
}

template <class Archive>
void ExtollSpikeCommTimestampDelayEventLossExpired::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_event_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayEventLossExpired)


/**
 * implementation of ExtollSpikeCommTimestampDelayCounterReset for Coord
 * ExtollSpikeCommTimestampDelayCounterResetOnFPGA
 */

ExtollSpikeCommTimestampDelayCounterReset::ExtollSpikeCommTimestampDelayCounterReset() {}

bool ExtollSpikeCommTimestampDelayCounterReset::operator==(
    ExtollSpikeCommTimestampDelayCounterReset const& /*other*/) const
{
	return true;
}

bool ExtollSpikeCommTimestampDelayCounterReset::operator!=(
    ExtollSpikeCommTimestampDelayCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(
    std::ostream& os, ExtollSpikeCommTimestampDelayCounterReset const& /*config*/)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommTimestampDelayCounterReset()";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayCounterReset::write_config_size_in_words>
ExtollSpikeCommTimestampDelayCounterReset::write_addresses(coordinate_type const& coord)
{
	auto coordSplitOnFPGA = coord.toExtollSpikeCommSplitOnFPGA();

	return {halco::hicann_dls::vx::ExtollAddress(
	    rx_timestamp_delay_buffer_rf_addresses.at(coordSplitOnFPGA.toEnum()) +
	    static_cast<int>(TimestampDelayRegs::CNT_RES))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollSpikeCommTimestampDelayCounterReset::read_config_size_in_words>
ExtollSpikeCommTimestampDelayCounterReset::read_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollSpikeCommTimestampDelayCounterResetBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t counter_reset :  1;
			uint64_t /* unused */  : 63;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollSpikeCommTimestampDelayCounterResetBitfield()
	{
		u.raw = 0u;
	}

	ExtollSpikeCommTimestampDelayCounterResetBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollSpikeCommTimestampDelayCounterReset::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollSpikeCommTimestampDelayCounterReset::read_config_size_in_words> const&)
{}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollSpikeCommTimestampDelayCounterReset::write_config_size_in_words>
ExtollSpikeCommTimestampDelayCounterReset::encode() const
{
	ExtollSpikeCommTimestampDelayCounterResetBitfield bitfield;
	bitfield.u.m.counter_reset = 0x1;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommTimestampDelayCounterReset::serialize(Archive& /*ar*/, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayCounterReset)


/**
 * implementation of ExtollBarrierTriggerReached for Coord
 * ExtollBarrierTriggerReachedOnExtollNetwork
 */


ExtollBarrierTriggerReached::ExtollBarrierTriggerReached() {}

bool ExtollBarrierTriggerReached::operator==(ExtollBarrierTriggerReached const& /*other*/) const
{
	return true;
}

bool ExtollBarrierTriggerReached::operator!=(ExtollBarrierTriggerReached const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollBarrierTriggerReached const& /*config*/)
{
	std::stringstream ss;
	ss << "ExtollBarrierTriggerReached()";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddressOnExtollNetwork,
    ExtollBarrierTriggerReached::write_config_size_in_words>
ExtollBarrierTriggerReached::write_addresses(coordinate_type const& coord)
{
	auto coordNodeIdOnNetwork = coord.toExtollNodeIdOnExtollNetwork();
	auto coordChipType = coordNodeIdOnNetwork.toExtollChipType();
	auto coordBarrierReachedOnNodeId = coord.toExtollBarrierTriggerReachedOnExtollNodeId();

	uint64_t address_mask = 0;
	if (coordChipType == halco::hicann_dls::vx::ExtollChipType::fpga) {
		address_mask = fpga_barrier_top_rf_mask;
	} else if (coordChipType == halco::hicann_dls::vx::ExtollChipType::tourmalet) {
		address_mask = tourmalet_barrier_top_rf_mask;
	} else {
		throw std::logic_error("Unknown ExtollChipType");
	}

	return {halco::hicann_dls::vx::ExtollAddressOnExtollNetwork(
	    halco::hicann_dls::vx::ExtollAddress(
	        address_mask + barrier_control_rf_offsets.at(coordBarrierReachedOnNodeId.toEnum()) +
	        static_cast<int>(BarrierControlRegs::CONTROL)),
	    coordNodeIdOnNetwork)};
}

std::array<
    halco::hicann_dls::vx::ExtollAddressOnExtollNetwork,
    ExtollBarrierTriggerReached::read_config_size_in_words>
ExtollBarrierTriggerReached::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

namespace {

struct ExtollBarrierTriggerReachedBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t reached           :  1;
			uint64_t /* unused */      : 63;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollBarrierTriggerReachedBitfield()
	{
		u.raw = 0u;
	}

	ExtollBarrierTriggerReachedBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollBarrierTriggerReached::decode(
    std::array<
        fisch::vx::word_access_type::ExtollOnNwNode,
        ExtollBarrierTriggerReached::read_config_size_in_words> const& /*data*/)
{}

std::array<
    fisch::vx::word_access_type::ExtollOnNwNode,
    ExtollBarrierTriggerReached::write_config_size_in_words>
ExtollBarrierTriggerReached::encode() const
{
	ExtollBarrierTriggerReachedBitfield bitfield;

	bitfield.u.m.reached = 0x1;

	return {fisch::vx::word_access_type::ExtollOnNwNode(bitfield.u.raw)};
}

template <class Archive>
void ExtollBarrierTriggerReached::serialize(Archive& /*ar*/, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollBarrierTriggerReached)


/**
 * implementation of ExtollBarrierReleased for Coord
 * ExtollBarrierReleasedOnExtollNetwork
 */


ExtollBarrierReleased::ExtollBarrierReleased() : m_released_0(), m_released_1(), m_released_active()
{}

bool ExtollBarrierReleased::get_released_0() const
{
	return m_released_0;
}

void ExtollBarrierReleased::set_released_0(bool const value)
{
	m_released_0 = value;
}

bool ExtollBarrierReleased::get_released_1() const
{
	return m_released_1;
}

void ExtollBarrierReleased::set_released_1(bool const value)
{
	m_released_1 = value;
}

bool ExtollBarrierReleased::get_released_active() const
{
	return m_released_active;
}

void ExtollBarrierReleased::set_released_active(bool const value)
{
	m_released_active = value;
}

bool ExtollBarrierReleased::operator==(ExtollBarrierReleased const& other) const
{
	return (m_released_0 == other.m_released_0) && (m_released_1 == other.m_released_1) &&
	       (m_released_active == other.m_released_active);
}

bool ExtollBarrierReleased::operator!=(ExtollBarrierReleased const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollBarrierReleased const& config)
{
	std::stringstream ss;
	ss << "ExtollBarrierReleased(released_0: " << std::boolalpha << config.m_released_0
	   << ", released_1: " << config.m_released_1
	   << ", released_active: " << config.m_released_active << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddressOnExtollNetwork,
    ExtollBarrierReleased::read_config_size_in_words>
ExtollBarrierReleased::read_addresses(coordinate_type const& coord)
{
	auto coordNodeIdOnNetwork = coord.toExtollNodeIdOnExtollNetwork();
	auto coordChipType = coordNodeIdOnNetwork.toExtollChipType();
	auto coordBarrierReleasedOnNodeId = coord.toExtollBarrierReleasedOnExtollNodeId();

	uint64_t address_mask = 0;
	if (coordChipType == halco::hicann_dls::vx::ExtollChipType::fpga) {
		address_mask = fpga_barrier_top_rf_mask;
	} else if (coordChipType == halco::hicann_dls::vx::ExtollChipType::tourmalet) {
		address_mask = tourmalet_barrier_top_rf_mask;
	} else {
		throw std::logic_error("Unknown ExtollChipType");
	}

	return {halco::hicann_dls::vx::ExtollAddressOnExtollNetwork(
	    halco::hicann_dls::vx::ExtollAddress(
	        address_mask + barrier_control_rf_offsets.at(coordBarrierReleasedOnNodeId.toEnum()) +
	        static_cast<int>(BarrierControlRegs::CONTROL)),
	    coordNodeIdOnNetwork)};
}

std::array<
    halco::hicann_dls::vx::ExtollAddressOnExtollNetwork,
    ExtollBarrierReleased::write_config_size_in_words>
ExtollBarrierReleased::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

namespace {

struct ExtollBarrierReleasedBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t /* reached */     :  1;
			uint64_t released_0        :  1;
			uint64_t released_1        :  1;
			uint64_t released_active   :  1;
			uint64_t /* unused */      : 60;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollBarrierReleasedBitfield()
	{
		u.raw = 0u;
	}

	ExtollBarrierReleasedBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollBarrierReleased::decode(std::array<
                                   fisch::vx::word_access_type::ExtollOnNwNode,
                                   ExtollBarrierReleased::read_config_size_in_words> const& data)
{
	ExtollBarrierReleasedBitfield bitfield;
	bitfield.u.raw = data[0];

	m_released_0 = bitfield.u.m.released_0;
	m_released_1 = bitfield.u.m.released_1;
	m_released_active = bitfield.u.m.released_active;
}

std::array<
    fisch::vx::word_access_type::ExtollOnNwNode,
    ExtollBarrierReleased::write_config_size_in_words>
ExtollBarrierReleased::encode() const
{
	return {};
}

template <class Archive>
void ExtollBarrierReleased::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_released_0));
	ar(CEREAL_NVP(m_released_1));
	ar(CEREAL_NVP(m_released_active));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollBarrierReleased)

/**
 * implementation of ExtollBarrierConfig for Coord
 * ExtollBarrierConfigOnExtollNetwork
 */

ExtollBarrierConfig::ExtollBarrierConfig() :
    m_enable(),
    m_child_nodes(),
    m_parent_nodes(),
    m_host_count(),
    m_client_count(),
    m_enable_reset()
{}

bool ExtollBarrierConfig::get_enable() const
{
	return m_enable;
}

void ExtollBarrierConfig::set_enable(bool const value)
{
	m_enable = value;
}

ExtollBarrierConfig::LinkFlags ExtollBarrierConfig::get_child_nodes() const
{
	return m_child_nodes;
}

void ExtollBarrierConfig::set_child_nodes(LinkFlags const value)
{
	m_child_nodes = value;
}

ExtollBarrierConfig::LinkFlags ExtollBarrierConfig::get_parent_nodes() const
{
	return m_parent_nodes;
}

void ExtollBarrierConfig::set_parent_nodes(LinkFlags const value)
{
	m_parent_nodes = value;
}

ExtollBarrierConfig::HostCount ExtollBarrierConfig::get_host_count() const
{
	return m_host_count;
}

void ExtollBarrierConfig::set_host_count(HostCount const value)
{
	m_host_count = value;
}

ExtollBarrierConfig::ClientCount ExtollBarrierConfig::get_client_count() const
{
	return m_client_count;
}

void ExtollBarrierConfig::set_client_count(ClientCount const value)
{
	m_client_count = value;
}

bool ExtollBarrierConfig::get_enable_reset() const
{
	return m_enable_reset;
}

void ExtollBarrierConfig::set_enable_reset(bool const value)
{
	m_enable_reset = value;
}

bool ExtollBarrierConfig::operator==(ExtollBarrierConfig const& other) const
{
	return (m_enable == other.m_enable) && (m_child_nodes == other.m_child_nodes) &&
	       (m_parent_nodes == other.m_parent_nodes) && (m_host_count == other.m_host_count) &&
	       (m_client_count == other.m_client_count) && (m_enable_reset == other.m_enable_reset);
}

bool ExtollBarrierConfig::operator!=(ExtollBarrierConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollBarrierConfig const& config)
{
	std::stringstream ss;
	ss << "ExtollBarrierConfig(enable: " << std::hex << config.m_enable << ", child_nodes: ("
	   << std::boolalpha;
	hate::join(ss, config.m_child_nodes, ",");
	ss << "), parent_nodes: (" << std::boolalpha;
	hate::join(ss, config.m_parent_nodes, ",");
	ss << "), host_count: " << std::hex << config.m_host_count << ", client_count: " << std::hex
	   << config.m_client_count << ", enable_reset: " << std::hex << config.m_enable_reset << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddressOnExtollNetwork,
    ExtollBarrierConfig::config_size_in_words>
ExtollBarrierConfig::addresses(coordinate_type const& coord)
{
	auto coordNodeIdOnNetwork = coord.toExtollNodeIdOnExtollNetwork();
	auto coordChipType = coordNodeIdOnNetwork.toExtollChipType();
	auto coordBarrierConfigOnNodeId = coord.toExtollBarrierConfigOnExtollNodeId();

	uint64_t address_mask = 0;
	if (coordChipType == halco::hicann_dls::vx::ExtollChipType::fpga) {
		address_mask = fpga_barrier_top_rf_mask;
	} else if (coordChipType == halco::hicann_dls::vx::ExtollChipType::tourmalet) {
		address_mask = tourmalet_barrier_top_rf_mask;
	} else {
		throw std::logic_error("Unknown ExtollChipType");
	}

	return {halco::hicann_dls::vx::ExtollAddressOnExtollNetwork(
	    halco::hicann_dls::vx::ExtollAddress(
	        address_mask + barrier_config_rf_offsets.at(coordBarrierConfigOnNodeId.toEnum()) +
	        static_cast<int>(BarrierConfigRegs::CONFIG)),
	    coordNodeIdOnNetwork)};
}

namespace {

struct ExtollBarrierConfigBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t enable            :  1;
			uint64_t child_nodes       :  7;
			uint64_t parent_nodes      :  7;
			uint64_t host_count        :  4;
			uint64_t enable_reset      :  1;
			uint64_t client_count      :  4;
			uint64_t /* unused */      : 40;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollBarrierConfigBitfield()
	{
		u.raw = 0u;
	}

	ExtollBarrierConfigBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollBarrierConfig::decode(std::array<
                                 fisch::vx::word_access_type::ExtollOnNwNode,
                                 ExtollBarrierConfig::config_size_in_words> const& data)
{
	ExtollBarrierConfigBitfield bitfield;
	bitfield.u.raw = data[0];

	for (auto i : halco::common::iter_all<halco::hicann_dls::vx::LinkOnExtollNode>()) {
		m_child_nodes[i] = static_cast<bool>(bitfield.u.m.child_nodes & (0x1 << i));
	}

	for (auto i : halco::common::iter_all<halco::hicann_dls::vx::LinkOnExtollNode>()) {
		m_parent_nodes[i] = static_cast<bool>(bitfield.u.m.parent_nodes & (0x1 << i));
	}

	m_enable = bitfield.u.m.enable;
	m_host_count = HostCount(bitfield.u.m.host_count);
	m_enable_reset = bitfield.u.m.enable_reset;
	m_client_count = ClientCount(bitfield.u.m.client_count);
}

std::array<fisch::vx::word_access_type::ExtollOnNwNode, ExtollBarrierConfig::config_size_in_words>
ExtollBarrierConfig::encode() const
{
	ExtollBarrierConfigBitfield bitfield;

	uint8_t child_nodes = 0;
	for (auto i : halco::common::iter_all<halco::hicann_dls::vx::LinkOnExtollNode>()) {
		child_nodes |= static_cast<uint8_t>(m_child_nodes[i]) << i;
	}
	uint8_t parent_nodes = 0;
	for (auto i : halco::common::iter_all<halco::hicann_dls::vx::LinkOnExtollNode>()) {
		parent_nodes |= static_cast<uint8_t>(m_parent_nodes[i]) << i;
	}

	bitfield.u.m.enable = m_enable;
	bitfield.u.m.child_nodes = child_nodes;
	bitfield.u.m.parent_nodes = parent_nodes;
	bitfield.u.m.host_count = m_host_count;
	bitfield.u.m.enable_reset = m_enable_reset;
	// bitfield.u.m.client_count = *READ_ONLY*;

	return {fisch::vx::word_access_type::ExtollOnNwNode(bitfield.u.raw)};
}

template <class Archive>
void ExtollBarrierConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable));
	ar(CEREAL_NVP(m_child_nodes));
	ar(CEREAL_NVP(m_parent_nodes));
	ar(CEREAL_NVP(m_host_count));
	ar(CEREAL_NVP(m_enable_reset));
	ar(CEREAL_NVP(m_client_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollBarrierConfig)


/**
 * implementation of ExtollInterruptControl for Coord
 * ExtollInterruptControlOnExtollNetwork
 */


ExtollInterruptControl::ExtollInterruptControl() : m_trigger(), m_interrupt() {}

bool ExtollInterruptControl::get_trigger() const
{
	return m_trigger;
}

void ExtollInterruptControl::set_trigger(bool const value)
{
	m_trigger = value;
}

bool ExtollInterruptControl::get_interrupt() const
{
	return m_interrupt;
}

void ExtollInterruptControl::set_interrupt(bool const value)
{
	m_interrupt = value;
}

bool ExtollInterruptControl::operator==(ExtollInterruptControl const& other) const
{
	return (m_trigger == other.m_trigger) && (m_interrupt == other.m_interrupt);
}

bool ExtollInterruptControl::operator!=(ExtollInterruptControl const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollInterruptControl const& config)
{
	std::stringstream ss;
	ss << "ExtollInterruptControl(trigger: " << std::boolalpha << config.m_trigger
	   << ", interrupt: " << config.m_interrupt << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddressOnExtollNetwork,
    ExtollInterruptControl::config_size_in_words>
ExtollInterruptControl::addresses(coordinate_type const& coord)
{
	auto coordNodeIdOnNetwork = coord.toExtollNodeIdOnExtollNetwork();
	auto coordChipType = coordNodeIdOnNetwork.toExtollChipType();
	auto coordInterruptControlOnNodeId = coord.toExtollInterruptControlOnExtollNodeId();

	uint64_t address_mask = 0;
	if (coordChipType == halco::hicann_dls::vx::ExtollChipType::fpga) {
		address_mask = fpga_barrier_top_rf_mask;
	} else if (coordChipType == halco::hicann_dls::vx::ExtollChipType::tourmalet) {
		address_mask = tourmalet_barrier_top_rf_mask;
	} else {
		throw std::logic_error("Unknown ExtollChipType");
	}

	return {halco::hicann_dls::vx::ExtollAddressOnExtollNetwork(
	    halco::hicann_dls::vx::ExtollAddress(
	        address_mask + interrupt_control_rf_offsets.at(coordInterruptControlOnNodeId.toEnum()) +
	        static_cast<int>(InterruptControlRegs::CONTROL)),
	    coordNodeIdOnNetwork)};
}

namespace {

struct ExtollInterruptControlBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t trigger           :  1;
			uint64_t interrupt         :  1;
			uint64_t /* unused */      : 62;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollInterruptControlBitfield()
	{
		u.raw = 0u;
	}

	ExtollInterruptControlBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollInterruptControl::decode(std::array<
                                    fisch::vx::word_access_type::ExtollOnNwNode,
                                    ExtollInterruptControl::config_size_in_words> const& data)
{
	ExtollInterruptControlBitfield bitfield;
	bitfield.u.raw = data[0];

	m_trigger = bitfield.u.m.trigger;
	m_interrupt = bitfield.u.m.interrupt;
}

std::
    array<fisch::vx::word_access_type::ExtollOnNwNode, ExtollInterruptControl::config_size_in_words>
    ExtollInterruptControl::encode() const
{
	ExtollInterruptControlBitfield bitfield;

	bitfield.u.m.trigger = m_trigger;
	bitfield.u.m.interrupt = m_interrupt;

	return {fisch::vx::word_access_type::ExtollOnNwNode(bitfield.u.raw)};
}

template <class Archive>
void ExtollInterruptControl::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_trigger));
	ar(CEREAL_NVP(m_interrupt));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollInterruptControl)


/**
 * implementation of ExtollInterruptConfig for Coord
 * ExtollInterruptConfigOnExtollNetwork
 */

ExtollInterruptConfig::ExtollInterruptConfig() :
    m_enable(),
    m_child_nodes(),
    m_delay_count(),
    m_enable_reset(),
    m_enable_measure(),
    m_measure_counter()
{}

bool ExtollInterruptConfig::get_enable() const
{
	return m_enable;
}

void ExtollInterruptConfig::set_enable(bool const value)
{
	m_enable = value;
}

ExtollInterruptConfig::LinkFlags ExtollInterruptConfig::get_child_nodes() const
{
	return m_child_nodes;
}

void ExtollInterruptConfig::set_child_nodes(LinkFlags const value)
{
	m_child_nodes = value;
}

ExtollInterruptConfig::DelayValue ExtollInterruptConfig::get_delay_count() const
{
	return m_delay_count;
}

void ExtollInterruptConfig::set_delay_count(DelayValue const value)
{
	m_delay_count = value;
}

ExtollInterruptConfig::DelayValue ExtollInterruptConfig::get_measure_counter() const
{
	return m_measure_counter;
}

void ExtollInterruptConfig::set_measure_counter(DelayValue const value)
{
	m_measure_counter = value;
}

bool ExtollInterruptConfig::get_enable_measure() const
{
	return m_enable_measure;
}

void ExtollInterruptConfig::set_enable_measure(bool const value)
{
	m_enable_measure = value;
}

bool ExtollInterruptConfig::get_enable_reset() const
{
	return m_enable_reset;
}

void ExtollInterruptConfig::set_enable_reset(bool const value)
{
	m_enable_reset = value;
}

bool ExtollInterruptConfig::operator==(ExtollInterruptConfig const& other) const
{
	return (m_enable == other.m_enable) && (m_child_nodes == other.m_child_nodes) &&
	       (m_delay_count == other.m_delay_count) && (m_enable_reset == other.m_enable_reset) &&
	       (m_enable_measure == other.m_enable_measure) &&
	       (m_measure_counter == other.m_measure_counter);
}

bool ExtollInterruptConfig::operator!=(ExtollInterruptConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollInterruptConfig const& config)
{
	std::stringstream ss;
	ss << "ExtollInterruptConfig(enable: " << std::hex << config.m_enable << ", child_nodes: ("
	   << std::boolalpha;
	hate::join(ss, config.m_child_nodes, ",");
	ss << "), delay_count: " << std::hex << config.m_delay_count << ", enable_reset: " << std::hex
	   << config.m_enable_reset << ", enable_measure: " << std::boolalpha << config.m_enable_measure
	   << ", measure_counter: " << std::hex << config.m_measure_counter << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddressOnExtollNetwork,
    ExtollInterruptConfig::config_size_in_words>
ExtollInterruptConfig::addresses(coordinate_type const& coord)
{
	auto coordNodeIdOnNetwork = coord.toExtollNodeIdOnExtollNetwork();
	auto coordChipType = coordNodeIdOnNetwork.toExtollChipType();
	auto coordInterruptConfigOnNodeId = coord.toExtollInterruptConfigOnExtollNodeId();

	uint64_t address_mask = 0;
	if (coordChipType == halco::hicann_dls::vx::ExtollChipType::fpga) {
		address_mask = fpga_barrier_top_rf_mask;
	} else if (coordChipType == halco::hicann_dls::vx::ExtollChipType::tourmalet) {
		address_mask = tourmalet_barrier_top_rf_mask;
	} else {
		throw std::logic_error("Unknown ExtollChipType");
	}

	return {halco::hicann_dls::vx::ExtollAddressOnExtollNetwork(
	    halco::hicann_dls::vx::ExtollAddress(
	        address_mask + interrupt_config_rf_offsets.at(coordInterruptConfigOnNodeId.toEnum()) +
	        static_cast<int>(InterruptConfigRegs::CONFIG)),
	    coordNodeIdOnNetwork)};
}

namespace {

struct ExtollInterruptConfigBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t enable            :  1;
			uint64_t child_nodes       :  7;
			uint64_t delay_count       : 16;
			uint64_t enable_reset      :  1;
			uint64_t enable_measure           :  1;
			uint64_t measure_counter   : 16;
			uint64_t /* unused */      : 22;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollInterruptConfigBitfield()
	{
		u.raw = 0u;
	}

	ExtollInterruptConfigBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollInterruptConfig::decode(std::array<
                                   fisch::vx::word_access_type::ExtollOnNwNode,
                                   ExtollInterruptConfig::config_size_in_words> const& data)
{
	ExtollInterruptConfigBitfield bitfield;
	bitfield.u.raw = data[0];

	for (auto i : halco::common::iter_all<halco::hicann_dls::vx::LinkOnExtollNode>()) {
		m_child_nodes[i] = static_cast<bool>(bitfield.u.m.child_nodes & (0x1 << i));
	}

	m_enable = bitfield.u.m.enable;
	// m_child_nodes = LinkFlags(bitfield.u.m.child_nodes);
	m_delay_count = DelayValue(bitfield.u.m.delay_count);
	m_enable_reset = bitfield.u.m.enable_reset;
	m_enable_measure = bitfield.u.m.enable_measure;
	m_measure_counter = DelayValue(bitfield.u.m.measure_counter);
}

std::array<fisch::vx::word_access_type::ExtollOnNwNode, ExtollInterruptConfig::config_size_in_words>
ExtollInterruptConfig::encode() const
{
	ExtollInterruptConfigBitfield bitfield;

	uint8_t child_nodes = 0;
	for (auto i : halco::common::iter_all<halco::hicann_dls::vx::LinkOnExtollNode>()) {
		child_nodes |= static_cast<uint8_t>(m_child_nodes[i]) << i;
	}

	bitfield.u.m.enable = m_enable;
	bitfield.u.m.child_nodes = child_nodes;
	bitfield.u.m.delay_count = m_delay_count;
	bitfield.u.m.enable_reset = m_enable_reset;
	bitfield.u.m.enable_measure = m_enable_measure;
	bitfield.u.m.measure_counter = m_measure_counter;

	return {fisch::vx::word_access_type::ExtollOnNwNode(bitfield.u.raw)};
}

template <class Archive>
void ExtollInterruptConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable));
	ar(CEREAL_NVP(m_child_nodes));
	ar(CEREAL_NVP(m_delay_count));
	ar(CEREAL_NVP(m_enable_reset));
	ar(CEREAL_NVP(m_enable_measure));
	ar(CEREAL_NVP(m_measure_counter));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollInterruptConfig)


/**
 * implementation of ExtollBarrierInterruptInportErrorCount for Coord
 * ExtollBarrierInterruptInportErrorCountOnFPGA
 */

ExtollBarrierInterruptInportErrorCount::ExtollBarrierInterruptInportErrorCount() : m_error_count()
{}

ExtollBarrierInterruptInportErrorCount::ErrorCount
ExtollBarrierInterruptInportErrorCount::get_error_count() const
{
	return m_error_count;
}

void ExtollBarrierInterruptInportErrorCount::set_error_count(ErrorCount const value)
{
	m_error_count = value;
}

bool ExtollBarrierInterruptInportErrorCount::operator==(
    ExtollBarrierInterruptInportErrorCount const& other) const
{
	return m_error_count == other.m_error_count;
}

bool ExtollBarrierInterruptInportErrorCount::operator!=(
    ExtollBarrierInterruptInportErrorCount const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollBarrierInterruptInportErrorCount const& config)
{
	std::stringstream ss;
	ss << "ExtollBarrierInterruptInportErrorCount(barrier-cells to non-implemented unit: "
	   << std::hex << config.m_error_count << ")";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollBarrierInterruptInportErrorCount::read_config_size_in_words>
ExtollBarrierInterruptInportErrorCount::read_addresses(coordinate_type const&)
{
	return {halco::hicann_dls::vx::ExtollAddress(
	    fpga_barrier_top_rf_mask + barrier_interrupt_inport_rf_offset +
	    static_cast<int>(BarrierInterruptInportRegs::ERROR_CNT))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollBarrierInterruptInportErrorCount::write_config_size_in_words>
ExtollBarrierInterruptInportErrorCount::write_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollBarrierInterruptInportErrorCountBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t error_count  : 48;
			uint64_t /* unused */ : 16;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollBarrierInterruptInportErrorCountBitfield()
	{
		u.raw = 0u;
	}

	ExtollBarrierInterruptInportErrorCountBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollBarrierInterruptInportErrorCount::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollBarrierInterruptInportErrorCount::read_config_size_in_words> const& data)
{
	ExtollBarrierInterruptInportErrorCountBitfield bitfield;
	bitfield.u.raw = data[0];

	m_error_count = ErrorCount(bitfield.u.m.error_count);
}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollBarrierInterruptInportErrorCount::write_config_size_in_words>
ExtollBarrierInterruptInportErrorCount::encode() const
{
	return {};
}

template <class Archive>
void ExtollBarrierInterruptInportErrorCount::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_error_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollBarrierInterruptInportErrorCount)


/**
 * implementation of ExtollBarrierInterruptInportCounterReset for Coord
 * ExtollBarrierInterruptInportCounterResetOnFPGA
 */

ExtollBarrierInterruptInportCounterReset::ExtollBarrierInterruptInportCounterReset() {}

bool ExtollBarrierInterruptInportCounterReset::operator==(
    ExtollBarrierInterruptInportCounterReset const& /*other*/) const
{
	return true;
}

bool ExtollBarrierInterruptInportCounterReset::operator!=(
    ExtollBarrierInterruptInportCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(
    std::ostream& os, ExtollBarrierInterruptInportCounterReset const& /*config*/)
{
	std::stringstream ss;
	ss << "ExtollBarrierInterruptInportCounterReset()";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollBarrierInterruptInportCounterReset::write_config_size_in_words>
ExtollBarrierInterruptInportCounterReset::write_addresses(coordinate_type const&)
{
	return {halco::hicann_dls::vx::ExtollAddress(
	    fpga_barrier_top_rf_mask + barrier_interrupt_inport_rf_offset +
	    static_cast<int>(BarrierInterruptInportRegs::CNT_RES))};
}

std::array<
    halco::hicann_dls::vx::ExtollAddress,
    ExtollBarrierInterruptInportCounterReset::read_config_size_in_words>
ExtollBarrierInterruptInportCounterReset::read_addresses(coordinate_type const&)
{
	return {};
}

namespace {

struct ExtollBarrierInterruptInportCounterResetBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint64_t counter_reset :  1;
			uint64_t /* unused */  : 63;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExtollBarrierInterruptInportCounterResetBitfield()
	{
		u.raw = 0u;
	}

	ExtollBarrierInterruptInportCounterResetBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

void ExtollBarrierInterruptInportCounterReset::decode(
    std::array<
        fisch::vx::word_access_type::Extoll,
        ExtollBarrierInterruptInportCounterReset::read_config_size_in_words> const&)
{}

std::array<
    fisch::vx::word_access_type::Extoll,
    ExtollBarrierInterruptInportCounterReset::write_config_size_in_words>
ExtollBarrierInterruptInportCounterReset::encode() const
{
	ExtollBarrierInterruptInportCounterResetBitfield bitfield;
	bitfield.u.m.counter_reset = 0x1;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollBarrierInterruptInportCounterReset::serialize(Archive& /*ar*/, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollBarrierInterruptInportCounterReset)


} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::EventSwitchSource, 0)

CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketTriggerConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketDestinationConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketNumPktsSent, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketCounterReset, 1)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterLookupConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterEventLossDisabled, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterEventLossMisconf, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterEventsRouted, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterCounterReset, 1)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommDecoderNumEventsReceived, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommDecoderCounterReset, 1)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayCounterReset, 1)

CEREAL_CLASS_VERSION(haldls::vx::ExtollBarrierTriggerReached, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollBarrierReleased, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollBarrierConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollInterruptControl, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollInterruptConfig, 0)

CEREAL_CLASS_VERSION(haldls::vx::ExtollBarrierInterruptInportErrorCount, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollBarrierInterruptInportCounterReset, 0)
