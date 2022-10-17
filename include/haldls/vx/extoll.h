#pragma once
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/extoll.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include <array>
#include <iosfwd>

namespace halco::hicann_dls::vx {
class ExtollAddress;
class OmnibusAddress;
} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {


/**
 * Container for configuring the FPGA-Event-Switch.
 */
class GENPYBIND(visible) EventSwitchSource
{
public:
	typedef halco::hicann_dls::vx::EventSwitchSourceOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * The Source for the Executor.
	 */
	struct GENPYBIND(inline_base("*")) Source
	    : public halco::common::detail::RantWrapper<Source, uint8_t, 3, 0>
	{
		constexpr explicit Source(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}

		static const Source executor SYMBOL_VISIBLE;
		static const Source asic SYMBOL_VISIBLE;
		static const Source external SYMBOL_VISIBLE;
		static const Source off SYMBOL_VISIBLE;
	};

	EventSwitchSource() SYMBOL_VISIBLE;

	/**
	 * Get source.
	 * @return Source
	 */
	GENPYBIND(getter_for(source))
	Source get_source() const SYMBOL_VISIBLE;

	/**
	 * Set source.
	 * @param value Source to set
	 */
	GENPYBIND(setter_for(source))
	void set_source(Source value) SYMBOL_VISIBLE;

	bool operator==(EventSwitchSource const& other) const SYMBOL_VISIBLE;
	bool operator!=(EventSwitchSource const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, EventSwitchSource const& config)
	    SYMBOL_VISIBLE;

	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	Source m_source;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(EventSwitchSource)


/**
 * Container for configuring the individual buckets for routing via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommBucketTriggerConfig
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommBucketTriggerConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Number of FPGA clock-cycles without receiving an event until the
	 * accumulated packet is flushed.
	 */
	struct GENPYBIND(inline_base("*")) PulseTimeout
	    : public halco::common::detail::RantWrapper<PulseTimeout, uint16_t, 0xff, 0>
	{
		constexpr explicit PulseTimeout(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Number of total FPGA clock-cycles until the accumulated packet is flushed.
	 */
	struct GENPYBIND(inline_base("*")) PacketTimeout
	    : public halco::common::detail::RantWrapper<PacketTimeout, uint16_t, 0xff, 0>
	{
		constexpr explicit PacketTimeout(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	ExtollSpikeCommBucketTriggerConfig() SYMBOL_VISIBLE;

	/**
	 * Get pulse timeout.
	 * @return PulseTimeout
	 */
	GENPYBIND(getter_for(pulse_timeout))
	PulseTimeout get_pulse_timeout() const SYMBOL_VISIBLE;

	/**
	 * Set pulse timeout.
	 * @param value PulseTimeout to set
	 */
	GENPYBIND(setter_for(pulse_timeout))
	void set_pulse_timeout(PulseTimeout value) SYMBOL_VISIBLE;

	/**
	 * Get packet timeout.
	 * @return PacketTimeout
	 */
	GENPYBIND(getter_for(packet_timeout))
	PacketTimeout get_packet_timeout() const SYMBOL_VISIBLE;

	/**
	 * Set packet timeout.
	 * @param value PacketTimeout to set
	 */
	GENPYBIND(setter_for(packet_timeout))
	void set_packet_timeout(PacketTimeout value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommBucketTriggerConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommBucketTriggerConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommBucketTriggerConfig const& config) SYMBOL_VISIBLE;

	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Extoll, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Extoll, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	PulseTimeout m_pulse_timeout;
	PacketTimeout m_packet_timeout;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketTriggerConfig)


/**
 * Container for configuring the individual buckets for routing via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommBucketDestinationConfig
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommBucketDestinationConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Defines the destination NodeID or MulticastGroupID to which the Bucket
	 * sends its accumulated packets.
	 */
	struct GENPYBIND(inline_base("*")) DestinationNodeID
	    : public halco::common::detail::RantWrapper<DestinationNodeID, uint32_t, 0xffffull, 0>
	{
		constexpr explicit DestinationNodeID(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Defines the axonal delay specific to this destination, given in systime
	 * clock-cycles (125MHz).
	 * The delay should be long enough in order to at least flatten all expected
	 * network jitter. For this it must be larger than the expected network
	 * delay to the programmed destination.
	 * In case of Multicast (MC), the delay must be larger than the longest
	 * network-delay in that MC-group.
	 */
	struct GENPYBIND(inline_base("*")) DestinationDelay
	    : public halco::common::detail::RantWrapper<DestinationDelay, uint16_t, 0x3ffull, 0>
	{
		constexpr explicit DestinationDelay(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommBucketDestinationConfig() SYMBOL_VISIBLE;

	/**
	 * Get destination node id.
	 * @return DestinationNodeID
	 */
	GENPYBIND(getter_for(destination_node_id))
	DestinationNodeID get_destination_node_id() const SYMBOL_VISIBLE;

	/**
	 * Set destination node id.
	 * @param value DestinationNodeID to set
	 */
	GENPYBIND(setter_for(destination_node_id))
	void set_destination_node_id(DestinationNodeID value) SYMBOL_VISIBLE;

	/**
	 * Get enable_multicast.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable_multicast))
	bool get_enable_multicast() const SYMBOL_VISIBLE;

	/**
	 * Set enable_multicast.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable_multicast))
	void set_enable_multicast(bool value) SYMBOL_VISIBLE;

	/**
	 * Get destination delay.
	 * @return DestinationDelay
	 */
	GENPYBIND(getter_for(destination_delay))
	DestinationDelay get_destination_delay() const SYMBOL_VISIBLE;

	/**
	 * Set destination delay.
	 * @param value DestinationDelay to set
	 */
	GENPYBIND(setter_for(destination_delay))
	void set_destination_delay(DestinationDelay value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommBucketDestinationConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommBucketDestinationConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommBucketDestinationConfig const& config) SYMBOL_VISIBLE;

	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Extoll, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Extoll, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	DestinationNodeID m_destination_node_id;
	bool m_enable_multicast;
	DestinationDelay m_destination_delay;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketDestinationConfig)


/**
 * Container for monitoring the individual buckets for routing via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommBucketNumPktsSent
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommBucketNumPktsSentOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of packets sent to the network from this bucket.
	 */
	struct GENPYBIND(inline_base("*")) PacketCount
	    : public halco::common::detail::RantWrapper<PacketCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit PacketCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	ExtollSpikeCommBucketNumPktsSent() SYMBOL_VISIBLE;

	/**
	 * Get the packet count.
	 * @return PacketCount
	 */
	GENPYBIND(getter_for(packet_count))
	PacketCount get_packet_count() const SYMBOL_VISIBLE;

	/**
	 * Set the packet count.
	 * @param value PacketCount to set
	 */
	GENPYBIND(setter_for(packet_count))
	void set_packet_count(PacketCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommBucketNumPktsSent const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommBucketNumPktsSent const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommBucketNumPktsSent const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	PacketCount m_packet_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketNumPktsSent)


/**
 * Container for resetting the pkt-counter (NumPktsSent) via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommBucketCounterReset
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommBucketCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommBucketCounterReset() SYMBOL_VISIBLE;

	/**
	 * Get counter reset.
	 * @return bool
	 */
	GENPYBIND(getter_for(counter_reset))
	bool get_counter_reset() const SYMBOL_VISIBLE;

	/**
	 * Set counter reset.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(counter_reset))
	void set_counter_reset(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommBucketCounterReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommBucketCounterReset const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommBucketCounterReset const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 0;
	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_counter_reset;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommBucketCounterReset)


/**
 * Container for configuring the send lookup table for routing via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommRouterLookupConfig
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommRouterLookupConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Represents the spike-label to be transmitted to the destination chip(s).
	 */
	struct GENPYBIND(inline_base("*")) DestinationEvent
	    : public halco::common::detail::RantWrapper<DestinationEvent, uint32_t, 0xffff, 0>
	{
		constexpr explicit DestinationEvent(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Represents the Bucket-ID to use for the respective spike-label.
	 */
	struct GENPYBIND(inline_base("*")) BucketID
	    : public halco::common::detail::RantWrapper<BucketID, uint8_t, 0x7, 0>
	{
		constexpr explicit BucketID(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommRouterLookupConfig() SYMBOL_VISIBLE;

	/**
	 * Get entry_valid.
	 * @return bool
	 */
	GENPYBIND(getter_for(entry_valid))
	bool get_entry_valid() const SYMBOL_VISIBLE;

	/**
	 * Set entry_valid.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(entry_valid))
	void set_entry_valid(bool value) SYMBOL_VISIBLE;

	/**
	 * Get destination_event.
	 * @return DestinationEvent
	 */
	GENPYBIND(getter_for(destination_event))
	DestinationEvent get_destination_event() const SYMBOL_VISIBLE;

	/**
	 * Set destination_event.
	 * @param value DestinationEvent to set
	 */
	GENPYBIND(setter_for(destination_event))
	void set_destination_event(DestinationEvent value) SYMBOL_VISIBLE;

	/**
	 * Get bucket_id.
	 * @return BucketID
	 */
	GENPYBIND(getter_for(bucket_id))
	BucketID get_bucket_id() const SYMBOL_VISIBLE;

	/**
	 * Set bucket_id.
	 * @param value BucketID to set
	 */
	GENPYBIND(setter_for(bucket_id))
	void set_bucket_id(BucketID value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommRouterLookupConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommRouterLookupConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommRouterLookupConfig const& config) SYMBOL_VISIBLE;

	// We need to write the config data into two tables on the FPGA which need
	// to be identically configured, however we keep a one word config-size and handle this
	// at higher sw-layers.
	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::Extoll, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, config_size_in_words> const& words)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_entry_valid;
	DestinationEvent m_destination_event;
	BucketID m_bucket_id;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterLookupConfig)


/**
 * Container for setting the router configuration state.
 */
class GENPYBIND(visible) ExtollSpikeCommRouterConfig
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommRouterConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommRouterConfig() SYMBOL_VISIBLE;

	/**
	 * Get enable routing flag.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable_routing))
	bool get_enable_routing() const SYMBOL_VISIBLE;

	/**
	 * Set enable routing flag.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable_routing))
	void set_enable_routing(bool value) SYMBOL_VISIBLE;

	/**
	 * Get drop if routing disabled flag.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable_drop_if_routing_disabled))
	bool get_enable_drop_if_routing_disabled() const SYMBOL_VISIBLE;

	/**
	 * Set drop if routing disabled flag.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable_drop_if_routing_disabled))
	void set_enable_drop_if_routing_disabled(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommRouterConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommRouterConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExtollSpikeCommRouterConfig const& config)
	    SYMBOL_VISIBLE;

	// We need to write the config data into two tables on the FPGA which need
	// to be identically configured, however we keep a one word config-size and handle this
	// at higher sw-layers.
	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::Extoll, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, config_size_in_words> const& words)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable_routing;
	bool m_enable_drop_if_routing_disabled;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterConfig)


/**
 * Container for getting the number of lost pulse-events due to disabled routing.
 */
class GENPYBIND(visible) ExtollSpikeCommRouterEventLossDisabled
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommRouterEventLossDisabledOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of lost pulse-events due to disabled routing.
	 */
	struct GENPYBIND(inline_base("*")) LostEventCount
	    : public halco::common::detail::
	          RantWrapper<LostEventCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit LostEventCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommRouterEventLossDisabled() SYMBOL_VISIBLE;

	/**
	 * Get lost event count.
	 * @return LostEventCount
	 */
	GENPYBIND(getter_for(lost_event_count))
	LostEventCount get_lost_event_count() const SYMBOL_VISIBLE;

	/**
	 * Set lost event count.
	 * @param value LostEventCount to set
	 */
	GENPYBIND(setter_for(lost_event_count))
	void set_lost_event_count(LostEventCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommRouterEventLossDisabled const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommRouterEventLossDisabled const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommRouterEventLossDisabled const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<typename fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	        words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	LostEventCount m_lost_event_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterEventLossDisabled)


/**
 * Container for getting the number of lost pulse-events due to invalid routing-table entries.
 */
class GENPYBIND(visible) ExtollSpikeCommRouterEventLossMisconf
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommRouterEventLossMisconfOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of lost pulse-events due to invalid routing-table entries.
	 */
	struct GENPYBIND(inline_base("*")) LostEventCount
	    : public halco::common::detail::
	          RantWrapper<LostEventCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit LostEventCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommRouterEventLossMisconf() SYMBOL_VISIBLE;

	/**
	 * Get lost event count.
	 * @return LostEventCount
	 */
	GENPYBIND(getter_for(lost_event_count))
	LostEventCount get_lost_event_count() const SYMBOL_VISIBLE;

	/**
	 * Set lost event count.
	 * @param value LostEventCount to set
	 */
	GENPYBIND(setter_for(lost_event_count))
	void set_lost_event_count(LostEventCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommRouterEventLossMisconf const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommRouterEventLossMisconf const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommRouterEventLossMisconf const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<typename fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	        words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	LostEventCount m_lost_event_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterEventLossMisconf)


/**
 * Container for getting the number of routed pulse-events.
 */
class GENPYBIND(visible) ExtollSpikeCommRouterEventsRouted
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommRouterEventsRoutedOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of routed pulse-events.
	 */
	struct GENPYBIND(inline_base("*")) EventCount
	    : public halco::common::detail::RantWrapper<EventCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit EventCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommRouterEventsRouted() SYMBOL_VISIBLE;

	/**
	 * Get event count.
	 * @return EventCount
	 */
	GENPYBIND(getter_for(event_count))
	EventCount get_event_count() const SYMBOL_VISIBLE;

	/**
	 * Set event count.
	 * @param value EventCount to set
	 */
	GENPYBIND(setter_for(event_count))
	void set_event_count(EventCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommRouterEventsRouted const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommRouterEventsRouted const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommRouterEventsRouted const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<typename fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	        words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterEventsRouted)


/**
 * Container for resetting the event-counters
 * (EventLossDisabled, EventLossMisconf, EventsRouted) via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommRouterCounterReset
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommRouterCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommRouterCounterReset() SYMBOL_VISIBLE;

	/**
	 * Get destination node id.
	 * @return bool
	 */
	GENPYBIND(getter_for(counter_reset))
	bool get_counter_reset() const SYMBOL_VISIBLE;

	/**
	 * Set destination node id.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(counter_reset))
	void set_counter_reset(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommRouterCounterReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommRouterCounterReset const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommRouterCounterReset const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 0;
	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_counter_reset;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommRouterCounterReset)


/**
 * Container for getting the number of received spike-events in the rx-decoders.
 */
class GENPYBIND(visible) ExtollSpikeCommDecoderNumEventsReceived
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommDecoderNumEventsReceivedOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of received spike-events.
	 */
	struct GENPYBIND(inline_base("*")) EventCount
	    : public halco::common::detail::RantWrapper<EventCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit EventCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommDecoderNumEventsReceived() SYMBOL_VISIBLE;

	/**
	 * Get event count.
	 * @return EventCount
	 */
	GENPYBIND(getter_for(event_count))
	EventCount get_event_count() const SYMBOL_VISIBLE;

	/**
	 * Set event count.
	 * @param value EventCount to set
	 */
	GENPYBIND(setter_for(event_count))
	void set_event_count(EventCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommDecoderNumEventsReceived const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommDecoderNumEventsReceived const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommDecoderNumEventsReceived const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<typename fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	        words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommDecoderNumEventsReceived)


/**
 * Container for resetting the spike counters of the decoder (NumEventsReceived) via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommDecoderCounterReset
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommDecoderCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommDecoderCounterReset() SYMBOL_VISIBLE;

	/**
	 * Get destination node id.
	 * @return bool
	 */
	GENPYBIND(getter_for(counter_reset))
	bool get_counter_reset() const SYMBOL_VISIBLE;

	/**
	 * Set destination node id.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(counter_reset))
	void set_counter_reset(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommDecoderCounterReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommDecoderCounterReset const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommDecoderCounterReset const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 0;
	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_counter_reset;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommDecoderCounterReset)


/**
 * Container for configuring the spike-delay module on the receiving side.
 */
class GENPYBIND(visible) ExtollSpikeCommTimestampDelayConfig
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommTimestampDelayConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommTimestampDelayConfig() SYMBOL_VISIBLE;

	/**
	 * Get forward immediately flag (forward events regardless of the timestamp).
	 * @return bool
	 */
	GENPYBIND(getter_for(forward_immediately))
	bool get_forward_immediately() const SYMBOL_VISIBLE;

	/**
	 * Set forward immediately flag (forward events regardless of the timestamp).
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(forward_immediately))
	void set_forward_immediately(bool value) SYMBOL_VISIBLE;

	/**
	 * Get timestamp-to-systime flag (dump the event's timestamp to the current systime).
	 * @return bool
	 */
	GENPYBIND(getter_for(timestamp_to_systime))
	bool get_timestamp_to_systime() const SYMBOL_VISIBLE;

	/**
	 * Set timestamp-to-systime flag (dump the event's timestamp to the current systime).
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(timestamp_to_systime))
	void set_timestamp_to_systime(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommTimestampDelayConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommTimestampDelayConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommTimestampDelayConfig const& config) SYMBOL_VISIBLE;

	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::Extoll, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, config_size_in_words> const& words)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_forward_immediately;
	bool m_timestamp_to_systime;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayConfig)


/**
 * Container for getting the number of received spike-events in the rx-decoders.
 */
class GENPYBIND(visible) ExtollSpikeCommTimestampDelayNumEventsReceived
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommTimestampDelayNumEventsReceivedOnFPGA
	    coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of received spike-events.
	 */
	struct GENPYBIND(inline_base("*")) EventCount
	    : public halco::common::detail::RantWrapper<EventCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit EventCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommTimestampDelayNumEventsReceived() SYMBOL_VISIBLE;

	/**
	 * Get event count.
	 * @return EventCount
	 */
	GENPYBIND(getter_for(event_count))
	EventCount get_event_count() const SYMBOL_VISIBLE;

	/**
	 * Set event count.
	 * @param value EventCount to set
	 */
	GENPYBIND(setter_for(event_count))
	void set_event_count(EventCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommTimestampDelayNumEventsReceived const& other) const
	    SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommTimestampDelayNumEventsReceived const& other) const
	    SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os,
	    ExtollSpikeCommTimestampDelayNumEventsReceived const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<typename fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	        words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayNumEventsReceived)


/**
 * Container for getting the number of lost spike-events in the rx-decoders
 * due to full delay-buffer.
 */
class GENPYBIND(visible) ExtollSpikeCommTimestampDelayEventLossFull
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommTimestampDelayEventLossFullOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of lost spike-events.
	 */
	struct GENPYBIND(inline_base("*")) EventCount
	    : public halco::common::detail::RantWrapper<EventCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit EventCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommTimestampDelayEventLossFull() SYMBOL_VISIBLE;

	/**
	 * Get lost event count.
	 * @return EventCount
	 */
	GENPYBIND(getter_for(event_count))
	EventCount get_event_count() const SYMBOL_VISIBLE;

	/**
	 * Set lost event count.
	 * @param value EventCount to set
	 */
	GENPYBIND(setter_for(event_count))
	void set_event_count(EventCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommTimestampDelayEventLossFull const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommTimestampDelayEventLossFull const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommTimestampDelayEventLossFull const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<typename fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	        words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayEventLossFull)


/**
 * Container for getting the number of lost spike-events in the rx-decoders
 * due to expired timestamp.
 */
class GENPYBIND(visible) ExtollSpikeCommTimestampDelayEventLossExpired
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommTimestampDelayEventLossExpiredOnFPGA
	    coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of lost spike-events.
	 */
	struct GENPYBIND(inline_base("*")) EventCount
	    : public halco::common::detail::RantWrapper<EventCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit EventCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollSpikeCommTimestampDelayEventLossExpired() SYMBOL_VISIBLE;

	/**
	 * Get lost event count.
	 * @return EventCount
	 */
	GENPYBIND(getter_for(event_count))
	EventCount get_event_count() const SYMBOL_VISIBLE;

	/**
	 * Set lost event count.
	 * @param value EventCount to set
	 */
	GENPYBIND(setter_for(event_count))
	void set_event_count(EventCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommTimestampDelayEventLossExpired const& other) const
	    SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommTimestampDelayEventLossExpired const& other) const
	    SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os,
	    ExtollSpikeCommTimestampDelayEventLossExpired const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<typename fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<typename fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	        words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayEventLossExpired)


/**
 * Container for resetting the event (loss) counters in the rx_delay_buffers
 * (NumEventsReceived, EventLossFull, EventLossExpired) via Extoll.
 */
class GENPYBIND(visible) ExtollSpikeCommTimestampDelayCounterReset
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommTimestampDelayCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommTimestampDelayCounterReset() SYMBOL_VISIBLE;

	/**
	 * Get destination node id.
	 * @return bool
	 */
	GENPYBIND(getter_for(counter_reset))
	bool get_counter_reset() const SYMBOL_VISIBLE;

	/**
	 * Set destination node id.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(counter_reset))
	void set_counter_reset(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollSpikeCommTimestampDelayCounterReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollSpikeCommTimestampDelayCounterReset const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollSpikeCommTimestampDelayCounterReset const& config) SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 0;
	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ExtollAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Extoll, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Extoll, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_counter_reset;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExtollSpikeCommTimestampDelayCounterReset)

namespace detail {

template <>
struct BackendContainerTrait<ExtollSpikeCommBucketTriggerConfig>
    : public BackendContainerBase<
          ExtollSpikeCommBucketTriggerConfig,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommBucketDestinationConfig>
    : public BackendContainerBase<
          ExtollSpikeCommBucketDestinationConfig,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommBucketNumPktsSent>
    : public BackendContainerBase<
          ExtollSpikeCommBucketNumPktsSent,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommBucketCounterReset>
    : public BackendContainerBase<
          ExtollSpikeCommBucketCounterReset,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommRouterLookupConfig>
    : public BackendContainerBase<
          ExtollSpikeCommRouterLookupConfig,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommRouterConfig>
    : public BackendContainerBase<ExtollSpikeCommRouterConfig, fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommRouterEventLossDisabled>
    : public BackendContainerBase<
          ExtollSpikeCommRouterEventLossDisabled,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommRouterEventLossMisconf>
    : public BackendContainerBase<
          ExtollSpikeCommRouterEventLossMisconf,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommRouterEventsRouted>
    : public BackendContainerBase<
          ExtollSpikeCommRouterEventsRouted,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommRouterCounterReset>
    : public BackendContainerBase<
          ExtollSpikeCommRouterCounterReset,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommDecoderNumEventsReceived>
    : public BackendContainerBase<
          ExtollSpikeCommDecoderNumEventsReceived,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommDecoderCounterReset>
    : public BackendContainerBase<
          ExtollSpikeCommDecoderCounterReset,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommTimestampDelayConfig>
    : public BackendContainerBase<
          ExtollSpikeCommTimestampDelayConfig,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommTimestampDelayNumEventsReceived>
    : public BackendContainerBase<
          ExtollSpikeCommTimestampDelayNumEventsReceived,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommTimestampDelayEventLossFull>
    : public BackendContainerBase<
          ExtollSpikeCommTimestampDelayEventLossFull,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommTimestampDelayEventLossExpired>
    : public BackendContainerBase<
          ExtollSpikeCommTimestampDelayEventLossExpired,
          fisch::vx::word_access_type::Extoll>
{};
template <>
struct BackendContainerTrait<ExtollSpikeCommTimestampDelayCounterReset>
    : public BackendContainerBase<
          ExtollSpikeCommTimestampDelayCounterReset,
          fisch::vx::word_access_type::Extoll>
{};

} // namespace detail

} // namespace vx
} // namespace haldls