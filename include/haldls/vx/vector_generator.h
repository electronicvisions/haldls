#pragma once
#include <array>
#include <ostream>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/quad.h"
#include "halco/hicann-dls/vx/vector_generator.h"

#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/padi.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct VectorGeneratorControl;
struct VectorGeneratorLUTEntry;
struct VectorGeneratorNotificationAddress;
struct VectorGeneratorFIFOWord;
struct VectorGeneratorTrigger;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorControl& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorLUTEntry& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::VectorGeneratorNotificationAddress& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorFIFOWord& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::VectorGeneratorTrigger& value, std::uint32_t const version);

} // namespace cereal

namespace cereal {
struct access;
} // namespace cereal

namespace halco::hicann_dls::vx {
struct OmnibusAddress;
struct PPUMemoryWordOnDLS;
} // namespace halco::hicann_dls::vx

namespace haldls { namespace vx GENPYBIND_TAG_HALDLS_VX {

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) VectorGeneratorControl
    : public ContainerBase<VectorGeneratorControl>
{
public:
	typedef halco::hicann_dls::vx::VectorGeneratorControlOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Number of sources to use.
	 */
	struct GENPYBIND(inline_base("*")) SourceCount
	    : public halco::common::detail::RantWrapper<SourceCount, uint_fast8_t, 2, 1>
	{
		constexpr explicit SourceCount(uintmax_t const val = 1) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Wait value to use.
	 */
	struct GENPYBIND(inline_base("*")) Wait
	    : public halco::common::detail::RantWrapper<Wait, uint_fast8_t, 15, 0>
	{
		constexpr explicit Wait(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Number of times to resend same input.
	 */
	struct GENPYBIND(inline_base("*")) ResendCount
	    : public halco::common::detail::RantWrapper<ResendCount, uint_fast8_t, 15, 0>
	{
		constexpr explicit ResendCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Source to use.
	 */
	enum class GENPYBIND(visible) Source
	{
		omnibus,
		external_0,
		external_1,
		external_2
	};

	/**
	 * Event packing to use.
	 */
	struct GENPYBIND(inline_base("*")) EventPack
	    : public halco::common::detail::RantWrapper<EventPack, uint_fast8_t, 3, 1>
	{
		constexpr explicit EventPack(uintmax_t const val = 1) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Type of notification to send once ready.
	 */
	enum class GENPYBIND(visible) Notification
	{
		omnibus,
		gpio
	};

	/**
	 * Type of trigger signal to listen to.
	 */
	enum class GENPYBIND(visible) Trigger
	{
		omnibus,
		gpio
	};

	/**
	 * Type(s) of signals to use.
	 */
	enum class GENPYBIND(visible) Signal
	{
		disabled,
		notification,
		trigger_and_notification
	};

	VectorGeneratorControl() = default;

	/**
	 * Get lookup table entry position to start from.
	 * @return Entry position
	 */
	GENPYBIND(getter_for(lookup_table_entry_start))
	halco::hicann_dls::vx::VectorGeneratorLUTEntryOnVectorGeneratorLUT
	get_lookup_table_entry_start() const SYMBOL_VISIBLE;

	/**
	 * Set lookup table entry position to start from.
	 * @param value Entry position
	 */
	GENPYBIND(setter_for(lookup_table_entry_start))
	void set_lookup_table_entry_start(
	    halco::hicann_dls::vx::VectorGeneratorLUTEntryOnVectorGeneratorLUT value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(source_count))
	SourceCount get_source_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(source_count))
	void set_source_count(SourceCount value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(resend_count))
	ResendCount get_resend_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(resend_count))
	void set_resend_count(ResendCount value) SYMBOL_VISIBLE;

	typedef halco::common::typed_array<Wait, halco::hicann_dls::vx::SourceOnVectorGenerator>
	    SourceWaits GENPYBIND(opaque);
	typedef halco::common::typed_array<Source, halco::hicann_dls::vx::SourceOnVectorGenerator>
	    Sources GENPYBIND(opaque);

	GENPYBIND(getter_for(source_waits))
	SourceWaits get_source_waits() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(source_waits))
	void set_source_waits(SourceWaits value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(sources))
	Sources get_sources() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(sources))
	void set_sources(Sources value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(event_pack))
	EventPack get_event_pack() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(event_pack))
	void set_event_pack(EventPack value) SYMBOL_VISIBLE;

	/**
	 * Get whether to ignore events with payload of zero.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_ignore_zeros))
	bool get_enable_ignore_zeros() const SYMBOL_VISIBLE;

	/**
	 * Set whether to ignore events with payload of zero.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_ignore_zeros))
	void set_enable_ignore_zeros(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(notification))
	Notification get_notification() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(notification))
	void set_notification(Notification value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(trigger))
	Trigger get_trigger() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(trigger))
	void set_trigger(Trigger value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(signal))
	Signal get_signal() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(signal))
	void set_signal(Signal value) SYMBOL_VISIBLE;

	bool operator==(VectorGeneratorControl const& other) const SYMBOL_VISIBLE;
	bool operator!=(VectorGeneratorControl const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, VectorGeneratorControl const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, VectorGeneratorControl& value, std::uint32_t const version) SYMBOL_VISIBLE;

	halco::hicann_dls::vx::VectorGeneratorLUTEntryOnVectorGeneratorLUT m_lookup_table_entry_start{};
	SourceCount m_source_count{};
	ResendCount m_resend_count{};
	SourceWaits m_source_waits{Wait(), Wait()};
	Sources m_sources{Source::omnibus, Source::omnibus};
	EventPack m_event_pack;
	bool m_enable_ignore_zeros{false};
	Notification m_notification{};
	Trigger m_trigger{};
	Signal m_signal{};
};


namespace detail {

template <>
struct BackendContainerTrait<VectorGeneratorControl>
    : public BackendContainerBase<VectorGeneratorControl, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl::Notification const& config)
    SYMBOL_VISIBLE;

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl::Trigger const& config)
    SYMBOL_VISIBLE;

std::ostream& operator<<(std::ostream& os, VectorGeneratorControl::Signal const& config)
    SYMBOL_VISIBLE;


/**
 * Container for an entry in the lookup-table for generation of spike events from activation values.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) VectorGeneratorLUTEntry
    : public ContainerBase<VectorGeneratorLUTEntry>
{
public:
	typedef std::true_type is_leaf_node;
	typedef halco::hicann_dls::vx::VectorGeneratorLUTEntryOnFPGA coordinate_type;

	/**
	 * Entry value describing the bits [5, 15] of the sent spike label.
	 */
	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast16_t, 2047, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	VectorGeneratorLUTEntry() = default;

	/**
	 * Get value.
	 * @return Value
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	/**
	 * Set value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	bool operator==(VectorGeneratorLUTEntry const& other) const SYMBOL_VISIBLE;
	bool operator!=(VectorGeneratorLUTEntry const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, VectorGeneratorLUTEntry const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar, VectorGeneratorLUTEntry& value, std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value{};
};


namespace detail {

template <>
struct BackendContainerTrait<VectorGeneratorLUTEntry>
    : public BackendContainerBase<VectorGeneratorLUTEntry, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) VectorGeneratorNotificationAddress
    : public ContainerBase<VectorGeneratorNotificationAddress>
{
public:
	typedef std::true_type is_leaf_node;
	typedef halco::hicann_dls::vx::VectorGeneratorNotificationAddressOnFPGA coordinate_type;

	typedef halco::hicann_dls::vx::OmnibusAddress Value GENPYBIND(visible);

	/** Default constructor. */
	VectorGeneratorNotificationAddress() = default;

	/**
	 * Get value.
	 * @return Value
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	/**
	 * Set value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	/**
	 * Construct notification address from word location on PPU memory.
	 * @param word Word location
	 */
	VectorGeneratorNotificationAddress(halco::hicann_dls::vx::PPUMemoryWordOnDLS const& word)
	    SYMBOL_VISIBLE;

	bool operator==(VectorGeneratorNotificationAddress const& other) const SYMBOL_VISIBLE;
	bool operator!=(VectorGeneratorNotificationAddress const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, VectorGeneratorNotificationAddress const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    VectorGeneratorNotificationAddress& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value{};
};


namespace detail {

template <>
struct BackendContainerTrait<VectorGeneratorNotificationAddress>
    : public BackendContainerBase<
          VectorGeneratorNotificationAddress,
          fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) VectorGeneratorTrigger
    : public ContainerBase<VectorGeneratorTrigger>
{
public:
	typedef std::true_type is_leaf_node;
	typedef halco::hicann_dls::vx::VectorGeneratorTriggerOnFPGA coordinate_type;

	/** Default constructor. */
	VectorGeneratorTrigger() = default;

	bool operator==(VectorGeneratorTrigger const& other) const SYMBOL_VISIBLE;
	bool operator!=(VectorGeneratorTrigger const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, VectorGeneratorTrigger const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar, VectorGeneratorTrigger& value, std::uint32_t const version) SYMBOL_VISIBLE;
};


namespace detail {

template <>
struct BackendContainerTrait<VectorGeneratorTrigger>
    : public BackendContainerBase<VectorGeneratorTrigger, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail


/**
 * Container for writing a word of (maximally) four activation values into the FIFO in front of the
 * vector generator.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) VectorGeneratorFIFOWord
    : public ContainerBase<VectorGeneratorFIFOWord>
{
public:
	typedef std::true_type is_leaf_node;
	typedef halco::hicann_dls::vx::VectorGeneratorFIFOWordOnFPGA coordinate_type;

	typedef PADIEvent::HagenActivation Value GENPYBIND(visible);

	typedef halco::common::typed_array<Value, halco::hicann_dls::vx::EntryOnQuad> Values
	    GENPYBIND(opaque);
	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::EntryOnQuad> Enables
	    GENPYBIND(opaque);

	/** Default constructor. */
	VectorGeneratorFIFOWord() SYMBOL_VISIBLE;

	/**
	 * Get values.
	 * @return Values
	 */
	GENPYBIND(getter_for(values), return_value_policy(reference))
	Values const& get_values() const SYMBOL_VISIBLE;

	/**
	 * Set values.
	 * @param value Values
	 */
	GENPYBIND(setter_for(values))
	void set_values(Values value) SYMBOL_VISIBLE;

	/**
	 * Get last enables.
	 * @return Values
	 */
	GENPYBIND(getter_for(last), return_value_policy(reference))
	Enables const& get_last() const SYMBOL_VISIBLE;

	/**
	 * Set last enables.
	 * @param value Values
	 */
	GENPYBIND(setter_for(last))
	void set_last(Enables value) SYMBOL_VISIBLE;

	/**
	 * Get enables for entries to write.
	 * @return Values
	 */
	GENPYBIND(getter_for(enable), return_value_policy(reference))
	Enables const& get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set enables for entries to write.
	 * @param value Values
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(Enables value) SYMBOL_VISIBLE;

	bool operator==(VectorGeneratorFIFOWord const& other) const SYMBOL_VISIBLE;
	bool operator!=(VectorGeneratorFIFOWord const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, VectorGeneratorFIFOWord const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar, VectorGeneratorFIFOWord& value, std::uint32_t const version) SYMBOL_VISIBLE;

	Values m_values;
	Enables m_last;
	Enables m_enable;
};


namespace detail {

template <>
struct BackendContainerTrait<VectorGeneratorFIFOWord>
    : public BackendContainerBase<VectorGeneratorFIFOWord, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
