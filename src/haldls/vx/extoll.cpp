#include "haldls/vx/extoll.h"

#include <iomanip>

#include "fisch/vx/word_access/type/extoll.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/extoll.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/extoll_constants.h"
#include "hate/math.h"

namespace haldls {
namespace vx {


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

ExtollSpikeCommBucketCounterReset::ExtollSpikeCommBucketCounterReset() : m_counter_reset() {}

bool ExtollSpikeCommBucketCounterReset::get_counter_reset() const
{
	return m_counter_reset;
}

void ExtollSpikeCommBucketCounterReset::set_counter_reset(bool const value)
{
	m_counter_reset = value;
}

bool ExtollSpikeCommBucketCounterReset::operator==(
    ExtollSpikeCommBucketCounterReset const& other) const
{
	return (m_counter_reset == other.m_counter_reset);
}

bool ExtollSpikeCommBucketCounterReset::operator!=(
    ExtollSpikeCommBucketCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommBucketCounterReset const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommBucketCounterReset(counter reset: " << std::hex << config.m_counter_reset
	   << ")";
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
	bitfield.u.m.counter_reset = m_counter_reset;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommBucketCounterReset::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_counter_reset));
}

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

ExtollSpikeCommRouterCounterReset::ExtollSpikeCommRouterCounterReset() : m_counter_reset() {}

bool ExtollSpikeCommRouterCounterReset::get_counter_reset() const
{
	return m_counter_reset;
}

void ExtollSpikeCommRouterCounterReset::set_counter_reset(bool const value)
{
	m_counter_reset = value;
}

bool ExtollSpikeCommRouterCounterReset::operator==(
    ExtollSpikeCommRouterCounterReset const& other) const
{
	return (m_counter_reset == other.m_counter_reset);
}

bool ExtollSpikeCommRouterCounterReset::operator!=(
    ExtollSpikeCommRouterCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterCounterReset const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommRouterCounterReset(counter reset: " << std::hex << config.m_counter_reset
	   << ")";
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
	bitfield.u.m.counter_reset = m_counter_reset;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommRouterCounterReset::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_counter_reset));
}

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

ExtollSpikeCommDecoderCounterReset::ExtollSpikeCommDecoderCounterReset() : m_counter_reset() {}

bool ExtollSpikeCommDecoderCounterReset::get_counter_reset() const
{
	return m_counter_reset;
}

void ExtollSpikeCommDecoderCounterReset::set_counter_reset(bool const value)
{
	m_counter_reset = value;
}

bool ExtollSpikeCommDecoderCounterReset::operator==(
    ExtollSpikeCommDecoderCounterReset const& other) const
{
	return (m_counter_reset == other.m_counter_reset);
}

bool ExtollSpikeCommDecoderCounterReset::operator!=(
    ExtollSpikeCommDecoderCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommDecoderCounterReset const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommDecoderCounterReset(counter reset: " << std::hex << config.m_counter_reset
	   << ")";
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
	bitfield.u.m.counter_reset = m_counter_reset;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommDecoderCounterReset::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_counter_reset));
}

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

ExtollSpikeCommTimestampDelayCounterReset::ExtollSpikeCommTimestampDelayCounterReset() :
    m_counter_reset()
{}

bool ExtollSpikeCommTimestampDelayCounterReset::get_counter_reset() const
{
	return m_counter_reset;
}

void ExtollSpikeCommTimestampDelayCounterReset::set_counter_reset(bool const value)
{
	m_counter_reset = value;
}

bool ExtollSpikeCommTimestampDelayCounterReset::operator==(
    ExtollSpikeCommTimestampDelayCounterReset const& other) const
{
	return (m_counter_reset == other.m_counter_reset);
}

bool ExtollSpikeCommTimestampDelayCounterReset::operator!=(
    ExtollSpikeCommTimestampDelayCounterReset const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExtollSpikeCommTimestampDelayCounterReset const& config)
{
	std::stringstream ss;
	ss << "ExtollSpikeCommTimestampDelayCounterReset(counter reset: " << std::hex
	   << config.m_counter_reset << ")";
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
	bitfield.u.m.counter_reset = m_counter_reset;

	return {fisch::vx::word_access_type::Extoll(bitfield.u.raw)};
}

template <class Archive>
void ExtollSpikeCommTimestampDelayCounterReset::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_counter_reset));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayCounterReset)


} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketTriggerConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketDestinationConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketNumPktsSent, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommBucketCounterReset, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterLookupConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterEventLossDisabled, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterEventLossMisconf, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterEventsRouted, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommRouterCounterReset, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommDecoderNumEventsReceived, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommDecoderCounterReset, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExtollSpikeCommTimestampDelayCounterReset, 0)
