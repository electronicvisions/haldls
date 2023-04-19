#pragma once
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/extoll.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include <array>
#include <iosfwd>
#include <cereal/macros.hpp>

namespace haldls::vx {

struct EventSwitchSource;
struct EventSwitchReadout;
struct EventSwitchConfig;
struct ExtollSpikeCommBucketTriggerConfig;
struct ExtollSpikeCommBucketDestinationConfig;
struct ExtollSpikeCommBucketNumPktsSent;
struct ExtollSpikeCommBucketCounterReset;
struct ExtollSpikeCommRouterLookupConfig;
struct ExtollSpikeCommRouterConfig;
struct ExtollSpikeCommRouterEventLossDisabled;
struct ExtollSpikeCommRouterEventLossMisconf;
struct ExtollSpikeCommRouterEventsRouted;
struct ExtollSpikeCommRouterCounterReset;
struct ExtollSpikeCommDecoderNumEventsReceived;
struct ExtollSpikeCommDecoderCounterReset;
struct ExtollSpikeCommTimestampDelayConfig;
struct ExtollSpikeCommTimestampDelayNumEventsReceived;
struct ExtollSpikeCommTimestampDelayEventLossFull;
struct ExtollSpikeCommTimestampDelayEventLossExpired;
struct ExtollSpikeCommTimestampDelayCounterReset;
struct ExtollBarrierTriggerReached;
struct ExtollBarrierReleased;
struct ExtollBarrierConfig;
struct ExtollInterruptControl;
struct ExtollInterruptConfig;
struct ExtollBarrierInterruptInportErrorCount;
struct ExtollBarrierInterruptInportCounterReset;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchSource& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchReadout& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::EventSwitchConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketTriggerConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketTriggerConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketDestinationConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommBucketDestinationConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketNumPktsSent& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommBucketCounterReset& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterLookupConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommRouterEventLossDisabled& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommRouterEventLossMisconf& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterEventsRouted& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollSpikeCommRouterCounterReset& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommDecoderNumEventsReceived& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommDecoderCounterReset& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollSpikeCommTimestampDelayCounterReset& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierTriggerReached& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierReleased& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollBarrierConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollInterruptControl& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ExtollInterruptConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollBarrierInterruptInportErrorCount& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::ExtollBarrierInterruptInportCounterReset& value,
    std::uint32_t const version);

} // namespace cereal

namespace halco::hicann_dls::vx {
class ExtollAddress;
struct OmnibusAddress;
} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {


/**
 * Container for configuring the FPGA-Event-Switch.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) EventSwitchSource
    : public ContainerBase<EventSwitchSource>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, EventSwitchSource& value, std::uint32_t const version) SYMBOL_VISIBLE;

	Source m_source;
};


/**
 * Container for configuring the FPGA-Event-Switch.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) EventSwitchReadout
    : public ContainerBase<EventSwitchReadout>
{
public:
	typedef halco::hicann_dls::vx::EventSwitchReadoutOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Systime value type. */
	struct GENPYBIND(inline_base("*")) Systime
	    : public halco::common::detail::RantWrapper<Systime, uint64_t, 0x07ff'ffff'ffffull, 0>
	{
		constexpr explicit Systime(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	EventSwitchReadout(Systime const systime = Systime(0)) SYMBOL_VISIBLE;

	/**
	 * Get systime.
	 * @return Systime
	 */
	GENPYBIND(getter_for(systime))
	Systime get_systime() const SYMBOL_VISIBLE;

	/**
	 * Set systime.
	 * @param value Systime to set
	 */
	GENPYBIND(setter_for(systime))
	void set_systime(Systime value) SYMBOL_VISIBLE;

	bool operator==(EventSwitchReadout const& other) const SYMBOL_VISIBLE;
	bool operator!=(EventSwitchReadout const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, EventSwitchReadout const& config)
	    SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 2;
	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<fisch::vx::word_access_type::Omnibus, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, EventSwitchReadout& value, std::uint32_t const version) SYMBOL_VISIBLE;

	Systime m_systime;
};


/**
 * Container for configuring the FPGA-Event-Switch.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) EventSwitchConfig
    : public ContainerBase<EventSwitchConfig>
{
public:
	typedef halco::hicann_dls::vx::EventSwitchConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	EventSwitchConfig(bool const interrupt_armed = false) SYMBOL_VISIBLE;

	/**
	 * Get interrupt_armed.
	 * @return bool
	 */
	GENPYBIND(getter_for(interrupt_armed))
	bool get_interrupt_armed() const SYMBOL_VISIBLE;

	/**
	 * Set interrupt_armed.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(interrupt_armed))
	void set_interrupt_armed(bool value) SYMBOL_VISIBLE;

	bool operator==(EventSwitchConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(EventSwitchConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, EventSwitchConfig const& config)
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
	friend void ::cereal::serialize(
	    Archive& ar, EventSwitchConfig& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_interrupt_armed;
};


/**
 * Container for configuring the individual buckets for routing via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommBucketTriggerConfig
    : public ContainerBase<ExtollSpikeCommBucketTriggerConfig>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommBucketTriggerConfig& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	PulseTimeout m_pulse_timeout;
	PacketTimeout m_packet_timeout;
};


/**
 * Container for configuring the individual buckets for routing via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollSpikeCommBucketDestinationConfig
    : public ContainerBase<ExtollSpikeCommBucketDestinationConfig>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommBucketDestinationConfig& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	DestinationNodeID m_destination_node_id;
	bool m_enable_multicast;
	DestinationDelay m_destination_delay;
};


/**
 * Container for monitoring the individual buckets for routing via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommBucketNumPktsSent
    : public ContainerBase<ExtollSpikeCommBucketNumPktsSent>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommBucketNumPktsSent& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	PacketCount m_packet_count;
};


/**
 * Container for resetting the pkt-counter (NumPktsSent) via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommBucketCounterReset
    : public ContainerBase<ExtollSpikeCommBucketCounterReset>
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommBucketCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommBucketCounterReset() SYMBOL_VISIBLE;

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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommBucketCounterReset& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;
};


/**
 * Container for configuring the send lookup table for routing via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommRouterLookupConfig
    : public ContainerBase<ExtollSpikeCommRouterLookupConfig>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommRouterLookupConfig& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_entry_valid;
	DestinationEvent m_destination_event;
	BucketID m_bucket_id;
};


/**
 * Container for setting the router configuration state.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommRouterConfig
    : public ContainerBase<ExtollSpikeCommRouterConfig>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommRouterConfig& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable_routing;
	bool m_enable_drop_if_routing_disabled;
};


/**
 * Container for getting the number of lost pulse-events due to disabled routing.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollSpikeCommRouterEventLossDisabled
    : public ContainerBase<ExtollSpikeCommRouterEventLossDisabled>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommRouterEventLossDisabled& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	LostEventCount m_lost_event_count;
};


/**
 * Container for getting the number of lost pulse-events due to invalid routing-table entries.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommRouterEventLossMisconf
    : public ContainerBase<ExtollSpikeCommRouterEventLossMisconf>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommRouterEventLossMisconf& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	LostEventCount m_lost_event_count;
};


/**
 * Container for getting the number of routed pulse-events.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommRouterEventsRouted
    : public ContainerBase<ExtollSpikeCommRouterEventsRouted>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommRouterEventsRouted& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};


/**
 * Container for resetting the event-counters
 * (EventLossDisabled, EventLossMisconf, EventsRouted) via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommRouterCounterReset
    : public ContainerBase<ExtollSpikeCommRouterCounterReset>
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommRouterCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommRouterCounterReset() SYMBOL_VISIBLE;

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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommRouterCounterReset& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;
};


/**
 * Container for getting the number of received spike-events in the rx-decoders.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollSpikeCommDecoderNumEventsReceived
    : public ContainerBase<ExtollSpikeCommDecoderNumEventsReceived>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommDecoderNumEventsReceived& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};


/**
 * Container for resetting the spike counters of the decoder (NumEventsReceived) via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommDecoderCounterReset
    : public ContainerBase<ExtollSpikeCommDecoderCounterReset>
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommDecoderCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommDecoderCounterReset() SYMBOL_VISIBLE;

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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommDecoderCounterReset& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;
};


/**
 * Container for configuring the spike-delay module on the receiving side.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollSpikeCommTimestampDelayConfig
    : public ContainerBase<ExtollSpikeCommTimestampDelayConfig>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommTimestampDelayConfig& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_forward_immediately;
	bool m_timestamp_to_systime;
};


/**
 * Container for getting the number of received spike-events in the rx-decoders.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollSpikeCommTimestampDelayNumEventsReceived
    : public ContainerBase<ExtollSpikeCommTimestampDelayNumEventsReceived>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommTimestampDelayNumEventsReceived& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};


/**
 * Container for getting the number of lost spike-events in the rx-decoders
 * due to full delay-buffer.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollSpikeCommTimestampDelayEventLossFull
    : public ContainerBase<ExtollSpikeCommTimestampDelayEventLossFull>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommTimestampDelayEventLossFull& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};


/**
 * Container for getting the number of lost spike-events in the rx-decoders
 * due to expired timestamp.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollSpikeCommTimestampDelayEventLossExpired
    : public ContainerBase<ExtollSpikeCommTimestampDelayEventLossExpired>
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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommTimestampDelayEventLossExpired& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	EventCount m_event_count;
};


/**
 * Container for resetting the event (loss) counters in the rx_delay_buffers
 * (NumEventsReceived, EventLossFull, EventLossExpired) via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollSpikeCommTimestampDelayCounterReset
    : public ContainerBase<ExtollSpikeCommTimestampDelayCounterReset>
{
public:
	typedef halco::hicann_dls::vx::ExtollSpikeCommTimestampDelayCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollSpikeCommTimestampDelayCounterReset() SYMBOL_VISIBLE;

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
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollSpikeCommTimestampDelayCounterReset& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;
};


/**
 * Container for controlling an Extoll Barrier.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollBarrierTriggerReached
    : public ContainerBase<ExtollBarrierTriggerReached>
{
public:
	typedef halco::hicann_dls::vx::ExtollBarrierTriggerReachedOnExtollNetwork coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollBarrierTriggerReached() SYMBOL_VISIBLE;

	bool operator==(ExtollBarrierTriggerReached const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollBarrierTriggerReached const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExtollBarrierTriggerReached const& config)
	    SYMBOL_VISIBLE;

	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 1;
	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 0;

	static std::
	    array<halco::hicann_dls::vx::ExtollAddressOnExtollNetwork, write_config_size_in_words>
	    write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::
	    array<halco::hicann_dls::vx::ExtollAddressOnExtollNetwork, read_config_size_in_words>
	    read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::ExtollOnNwNode, write_config_size_in_words>
	encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(std::array<
	            typename fisch::vx::word_access_type::ExtollOnNwNode,
	            read_config_size_in_words> const& words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollBarrierTriggerReached& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;
};


/**
 * Container for controlling an Extoll Barrier.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollBarrierReleased
    : public ContainerBase<ExtollBarrierReleased>
{
public:
	typedef halco::hicann_dls::vx::ExtollBarrierReleasedOnExtollNetwork coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollBarrierReleased() SYMBOL_VISIBLE;

	/**
	 * Get released_0 flag.
	 * @return bool
	 */
	GENPYBIND(getter_for(released_0))
	bool get_released_0() const SYMBOL_VISIBLE;

	/**
	 * Set released_0 flag.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(released_0))
	void set_released_0(bool value) SYMBOL_VISIBLE;

	/**
	 * Get released_1 flag.
	 * @return bool
	 */
	GENPYBIND(getter_for(released_1))
	bool get_released_1() const SYMBOL_VISIBLE;

	/**
	 * Set released_1 flag.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(released_1))
	void set_released_1(bool value) SYMBOL_VISIBLE;

	/**
	 * Get released_active flag.
	 * It determines, which of the {released_0, released_1} is to be interpreted.
	 * @return bool
	 */
	GENPYBIND(getter_for(released_active))
	bool get_released_active() const SYMBOL_VISIBLE;

	/**
	 * Set released_active flag.
	 * It determines, which of the {released_0, released_1} is to be interpreted.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(released_active))
	void set_released_active(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollBarrierReleased const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollBarrierReleased const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExtollBarrierReleased const& config)
	    SYMBOL_VISIBLE;

	constexpr static size_t read_config_size_in_words GENPYBIND(hidden) = 1;
	constexpr static size_t write_config_size_in_words GENPYBIND(hidden) = 0;

	static std::
	    array<halco::hicann_dls::vx::ExtollAddressOnExtollNetwork, read_config_size_in_words>
	    read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::
	    array<halco::hicann_dls::vx::ExtollAddressOnExtollNetwork, write_config_size_in_words>
	    write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::ExtollOnNwNode, write_config_size_in_words>
	encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(std::array<
	            typename fisch::vx::word_access_type::ExtollOnNwNode,
	            read_config_size_in_words> const& words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, ExtollBarrierReleased& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_released_0;
	bool m_released_1;
	bool m_released_active;
};


/**
 * Container for configuring an Extoll Barrier.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollBarrierConfig
    : public ContainerBase<ExtollBarrierConfig>
{
public:
	typedef halco::hicann_dls::vx::ExtollBarrierConfigOnExtollNetwork coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollBarrierConfig() SYMBOL_VISIBLE;

	/**
	 * Bit flags for each of up to 7 links.
	 */
	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::LinkOnExtollNode> LinkFlags
	    GENPYBIND(opaque(false));

	/**
	 * Delay Value for Delay and Measure-Counter
	 */
	struct GENPYBIND(inline_base("*")) HostCount
	    : public halco::common::detail::RantWrapper<HostCount, uint8_t, 0xf, 0>
	{
		constexpr explicit HostCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Delay Value for Delay and Measure-Counter
	 */
	struct GENPYBIND(inline_base("*")) ClientCount
	    : public halco::common::detail::RantWrapper<ClientCount, uint8_t, 0xf, 0>
	{
		constexpr explicit ClientCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Get enable flag.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set enable flag.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * Get child_nodes setting.
	 * Bit mask for the child nodes. One bit for each link.
	 * @return LinkFlags
	 */
	GENPYBIND(getter_for(child_nodes))
	LinkFlags get_child_nodes() const SYMBOL_VISIBLE;

	/**
	 * Set child_nodes setting.
	 * Bit mask for the child nodes. One bit for each link.
	 * FPGAs have one link, while Tourmalets have 7 links.
	 * @param value LinkFlags to set
	 */
	GENPYBIND(setter_for(child_nodes))
	void set_child_nodes(LinkFlags value) SYMBOL_VISIBLE;

	/**
	 * Get one-hot parent_nodes setting.
	 * Indicates the link to the parent node.
	 * @return LinkFlags
	 */
	GENPYBIND(getter_for(parent_nodes))
	LinkFlags get_parent_nodes() const SYMBOL_VISIBLE;

	/**
	 * Set one-hot parent_nodes setting.
	 * Indicates the link to the parent node.
	 * FPGAs have one link, while Tourmalets have 7 links.
	 * @param value LinkFlags to set
	 */
	GENPYBIND(setter_for(parent_nodes))
	void set_parent_nodes(LinkFlags value) SYMBOL_VISIBLE;

	/**
	 * Get host_count setting.
	 * Number of local sw-processes taking part in the barrier.
	 * @return HostCount
	 */
	GENPYBIND(getter_for(host_count))
	HostCount get_host_count() const SYMBOL_VISIBLE;

	/**
	 * Set host_count setting.
	 * Number of local sw-processes taking part in the barrier.
	 * Does only exist for FPGA
	 * Configuring host_count higher than (0xF - client_count)
	 * leads to undefined behaviour
	 * @param value HostCount to set
	 */
	GENPYBIND(setter_for(host_count))
	void set_host_count(HostCount value) SYMBOL_VISIBLE;

	/**
	 * Get client_count setting.
	 * Number of local hw-clients taking part in the barrier.
	 * @return ClientCount
	 */
	GENPYBIND(getter_for(client_count))
	ClientCount get_client_count() const SYMBOL_VISIBLE;

	/**
	 * Set client_count setting.
	 * Number of local hw-clients taking part in the barrier.
	 * Does only exist for FPGA nodes.
	 * Configuring host_count higher than (0xF - client_count)
	 * leads to undefined behaviour
	 * client_count is READ_ONLY (fixed in Hardware)!
	 * @param value ClientCount to set
	 */
	GENPYBIND(setter_for(client_count))
	void set_client_count(ClientCount value) SYMBOL_VISIBLE;

	/**
	 * Get enable_reset flag.
	 * Resets the barrier unit.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable_reset))
	bool get_enable_reset() const SYMBOL_VISIBLE;

	/**
	 * Set enable_reset flag.
	 * Resets the barrier unit.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable_reset))
	void set_enable_reset(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollBarrierConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollBarrierConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExtollBarrierConfig const& config)
	    SYMBOL_VISIBLE;

	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddressOnExtollNetwork, config_size_in_words>
	addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::ExtollOnNwNode, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(std::array<
	            typename fisch::vx::word_access_type::ExtollOnNwNode,
	            config_size_in_words> const& words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, ExtollBarrierConfig& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable;
	LinkFlags m_child_nodes;
	LinkFlags m_parent_nodes;
	HostCount m_host_count;
	ClientCount m_client_count;
	bool m_enable_reset;
};


/**
 * Container for controlling an Extoll Barrier.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollInterruptControl
    : public ContainerBase<ExtollInterruptControl>
{
public:
	typedef halco::hicann_dls::vx::ExtollInterruptControlOnExtollNetwork coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollInterruptControl() SYMBOL_VISIBLE;

	static SYMBOL_VISIBLE const ExtollInterruptControl trigger GENPYBIND(visible);
	static SYMBOL_VISIBLE const ExtollInterruptControl reset GENPYBIND(visible);

	/**
	 * Get operation_trigger.
	 * @return bool
	 */
	GENPYBIND(getter_for(operation_trigger))
	bool get_operation_trigger() const SYMBOL_VISIBLE;

	/**
	 * Set operation_trigger.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(operation_trigger))
	void set_operation_trigger(bool value) SYMBOL_VISIBLE;

	/**
	 * Get interrupt flag.
	 * @return bool
	 */
	GENPYBIND(getter_for(interrupt))
	bool get_interrupt() const SYMBOL_VISIBLE;

	/**
	 * Set interrupt flag.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(interrupt))
	void set_interrupt(bool value) SYMBOL_VISIBLE;

	bool operator==(ExtollInterruptControl const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollInterruptControl const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExtollInterruptControl const& config)
	    SYMBOL_VISIBLE;

	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddressOnExtollNetwork, config_size_in_words>
	addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::ExtollOnNwNode, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(std::array<
	            typename fisch::vx::word_access_type::ExtollOnNwNode,
	            config_size_in_words> const& words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, ExtollInterruptControl& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_operation_trigger;
	bool m_interrupt;
};


/**
 * Container for configuring an Extoll Barrier.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ExtollInterruptConfig
    : public ContainerBase<ExtollInterruptConfig>
{
public:
	typedef halco::hicann_dls::vx::ExtollInterruptConfigOnExtollNetwork coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Bit flags for each of up to 7 links.
	 */
	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::LinkOnExtollNode> LinkFlags
	    GENPYBIND(opaque(false));

	/**
	 * Delay Value for Delay and Measure-Counter
	 */
	struct GENPYBIND(inline_base("*")) DelayValue
	    : public halco::common::detail::RantWrapper<DelayValue, uint32_t, 0xffffull, 0>
	{
		constexpr explicit DelayValue(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	ExtollInterruptConfig() SYMBOL_VISIBLE;

	/**
	 * Get enable flag.
	 * Enables the interrupt unit.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set enable flag.
	 * Enables the interrupt unit.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * Get child_nodes setting.
	 * Bit mask for the child nodes. One bit for each link.
	 * @return LinkFlags
	 */
	GENPYBIND(getter_for(child_nodes))
	LinkFlags get_child_nodes() const SYMBOL_VISIBLE;

	/**
	 * Set child_nodes setting.
	 * Bit mask for the child nodes. One bit for each link.
	 * FPGAs have one link, while Tourmalets have 7 links.
	 * @param value LinkFlags to set
	 */
	GENPYBIND(setter_for(child_nodes))
	void set_child_nodes(LinkFlags value) SYMBOL_VISIBLE;

	/**
	 * Get one-hot delay_count setting.
	 * Number of clock cycles before the local interrupt gets triggered.
	 * @return DelayValue
	 */
	GENPYBIND(getter_for(delay_count))
	DelayValue get_delay_count() const SYMBOL_VISIBLE;

	/**
	 * Set one-hot delay_count setting.
	 * Number of clock cycles before the local interrupt gets triggered.
	 * @param value DelayValue to set
	 */
	GENPYBIND(setter_for(delay_count))
	void set_delay_count(DelayValue value) SYMBOL_VISIBLE;

	/**
	 * Get enable_reset flag.
	 * Resets the interrupt unit.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable_reset))
	bool get_enable_reset() const SYMBOL_VISIBLE;

	/**
	 * Set enable_reset flag.
	 * Resets the interrupt unit.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable_reset))
	void set_enable_reset(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable_measure setting.
	 * Enables interrupt measurement between two nodes directly connected to each other.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable_measure))
	bool get_enable_measure() const SYMBOL_VISIBLE;

	/**
	 * Set enable_measure setting.
	 * Enables interrupt measurement between two nodes directly connected to each other.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable_measure))
	void set_enable_measure(bool value) SYMBOL_VISIBLE;

	/**
	 * Get measure_counter setting.
	 * Starts counting when trigger is set and stops on a received interrupt message.
	 * @return DelayValue
	 */
	GENPYBIND(getter_for(measure_counter))
	DelayValue get_measure_counter() const SYMBOL_VISIBLE;

	/**
	 * Set measure_counter setting.
	 * Starts counting when trigger is set and stops on a received interrupt message.
	 * @param value DelayValue to set
	 */
	GENPYBIND(setter_for(measure_counter))
	void set_measure_counter(DelayValue value) SYMBOL_VISIBLE;

	bool operator==(ExtollInterruptConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollInterruptConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExtollInterruptConfig const& config)
	    SYMBOL_VISIBLE;

	constexpr static size_t config_size_in_words GENPYBIND(hidden) = 1;

	static std::array<halco::hicann_dls::vx::ExtollAddressOnExtollNetwork, config_size_in_words>
	addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	std::array<typename fisch::vx::word_access_type::ExtollOnNwNode, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);

	void decode(std::array<
	            typename fisch::vx::word_access_type::ExtollOnNwNode,
	            config_size_in_words> const& words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, ExtollInterruptConfig& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable;
	LinkFlags m_child_nodes;
	DelayValue m_delay_count;
	bool m_enable_reset;
	bool m_enable_measure;
	DelayValue m_measure_counter;
};


/**
 * Container for reading the interrupt / barrier error counter via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollBarrierInterruptInportErrorCount
    : public ContainerBase<ExtollBarrierInterruptInportErrorCount>
{
public:
	typedef halco::hicann_dls::vx::ExtollBarrierInterruptInportErrorCountOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Counts the number of barrier-cells for not-implemented unit-ids at the Inport
	 * of the barrier- / interrupt-unit.
	 */
	struct GENPYBIND(inline_base("*")) ErrorCount
	    : public halco::common::detail::RantWrapper<ErrorCount, uint64_t, 0xffff'ffff'ffffull, 0>
	{
		constexpr explicit ErrorCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	ExtollBarrierInterruptInportErrorCount() SYMBOL_VISIBLE;

	/**
	 * Get destination node id.
	 * @return ErrorCount
	 */
	GENPYBIND(getter_for(error_count))
	ErrorCount get_error_count() const SYMBOL_VISIBLE;

	/**
	 * Set destination node id.
	 * @param value ErrorCount to set
	 */
	GENPYBIND(setter_for(error_count))
	void set_error_count(ErrorCount value) SYMBOL_VISIBLE;

	bool operator==(ExtollBarrierInterruptInportErrorCount const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollBarrierInterruptInportErrorCount const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollBarrierInterruptInportErrorCount const& config) SYMBOL_VISIBLE;

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
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollBarrierInterruptInportErrorCount& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	ErrorCount m_error_count;
};


/**
 * Container for configuring the individual buckets for routing via Extoll.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*"))
    ExtollBarrierInterruptInportCounterReset
    : public ContainerBase<ExtollBarrierInterruptInportCounterReset>
{
public:
	typedef halco::hicann_dls::vx::ExtollBarrierInterruptInportCounterResetOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	ExtollBarrierInterruptInportCounterReset() SYMBOL_VISIBLE;

	bool operator==(ExtollBarrierInterruptInportCounterReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExtollBarrierInterruptInportCounterReset const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, ExtollBarrierInterruptInportCounterReset const& config) SYMBOL_VISIBLE;

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
	friend void ::cereal::serialize(
	    Archive& ar,
	    ExtollBarrierInterruptInportCounterReset& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;
};


namespace detail {

template <>
struct BackendContainerTrait<EventSwitchSource>
    : public BackendContainerBase<EventSwitchSource, fisch::vx::word_access_type::Omnibus>
{};
template <>
struct BackendContainerTrait<EventSwitchReadout>
    : public BackendContainerBase<EventSwitchReadout, fisch::vx::word_access_type::Omnibus>
{};
template <>
struct BackendContainerTrait<EventSwitchConfig>
    : public BackendContainerBase<EventSwitchConfig, fisch::vx::word_access_type::Omnibus>
{};
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


template <>
struct BackendContainerTrait<ExtollBarrierTriggerReached>
    : public BackendContainerBase<
          ExtollBarrierTriggerReached,
          fisch::vx::word_access_type::ExtollOnNwNode>
{};

template <>
struct BackendContainerTrait<ExtollBarrierReleased>
    : public BackendContainerBase<
          ExtollBarrierReleased,
          fisch::vx::word_access_type::ExtollOnNwNode>
{};

template <>
struct BackendContainerTrait<ExtollBarrierConfig>
    : public BackendContainerBase<ExtollBarrierConfig, fisch::vx::word_access_type::ExtollOnNwNode>
{};

template <>
struct BackendContainerTrait<ExtollInterruptControl>
    : public BackendContainerBase<
          ExtollInterruptControl,
          fisch::vx::word_access_type::ExtollOnNwNode>
{};

template <>
struct BackendContainerTrait<ExtollInterruptConfig>
    : public BackendContainerBase<
          ExtollInterruptConfig,
          fisch::vx::word_access_type::ExtollOnNwNode>
{};

template <>
struct BackendContainerTrait<ExtollBarrierInterruptInportErrorCount>
    : public BackendContainerBase<
          ExtollBarrierInterruptInportErrorCount,
          fisch::vx::word_access_type::Extoll>
{};

template <>
struct BackendContainerTrait<ExtollBarrierInterruptInportCounterReset>
    : public BackendContainerBase<
          ExtollBarrierInterruptInportCounterReset,
          fisch::vx::word_access_type::Extoll>
{};

} // namespace detail

} // namespace vx
} // namespace haldls