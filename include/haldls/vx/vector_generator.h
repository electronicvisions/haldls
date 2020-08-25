#pragma once
#include <array>
#include <ostream>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/vector_generator.h"

#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace halco::hicann_dls::vx {
class OmnibusAddress;
} // namespace halco::hicann_dls::vx

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) VectorGeneratorControl
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
	std::array<fisch::vx::Omnibus, write_config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Omnibus, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, VectorGeneratorControl const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

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

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(VectorGeneratorControl)

namespace detail {

template <>
struct BackendContainerTrait<VectorGeneratorControl>
    : public BackendContainerBase<VectorGeneratorControl, fisch::vx::Omnibus>
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
class GENPYBIND(visible) VectorGeneratorLUTEntry
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
	std::array<fisch::vx::Omnibus, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Omnibus, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value{};
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(VectorGeneratorLUTEntry)

namespace detail {

template <>
struct BackendContainerTrait<VectorGeneratorLUTEntry>
    : public BackendContainerBase<VectorGeneratorLUTEntry, fisch::vx::Omnibus>
{};

} // namespace detail


} // namespace haldls::vx
