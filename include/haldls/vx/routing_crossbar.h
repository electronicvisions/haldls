#pragma once
#include <array>
#include <ostream>

#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/event.h"
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class OmnibusChip;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) CrossbarOutputConfig
{
public:
	typedef halco::hicann_dls::vx::CrossbarOutputConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor. */
	CrossbarOutputConfig() SYMBOL_VISIBLE;

	/**
	 * Get enable value for event counter of specified output.
	 * @param coord Output coordinate
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_event_counter))
	bool get_enable_event_counter(halco::hicann_dls::vx::CrossbarOutputOnDLS const& coord) const
	    SYMBOL_VISIBLE;

	/**
	 * Set enable value for event counter of specified output.
	 * @param coord Output coordinate
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_event_counter))
	void set_enable_event_counter(
	    halco::hicann_dls::vx::CrossbarOutputOnDLS const& coord, bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_slow))
	bool get_enable_slow(halco::hicann_dls::vx::CrossbarL2OutputOnDLS const& coord) const
	    SYMBOL_VISIBLE;

	GENPYBIND(setter_for(enable_slow))
	void set_enable_slow(halco::hicann_dls::vx::CrossbarL2OutputOnDLS const& coord, bool value)
	    SYMBOL_VISIBLE;

	bool operator==(CrossbarOutputConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CrossbarOutputConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CrossbarOutputConfig const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	halco::common::typed_array<bool, halco::hicann_dls::vx::CrossbarOutputOnDLS>
	    m_enable_event_counter;
	halco::common::typed_array<bool, halco::hicann_dls::vx::CrossbarL2OutputOnDLS> m_enable_slow;
};

namespace detail {

template <>
struct BackendContainerTrait<CrossbarOutputConfig>
    : public BackendContainerBase<
          CrossbarOutputConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Crossbar input drop counter accumulating drops at all outputs for which the drop counter
 * accumulation is enabled in the corresponding CrossbarNode.
 */
class GENPYBIND(visible) CrossbarInputDropCounter
{
public:
	typedef halco::hicann_dls::vx::CrossbarInputOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::
	          RantWrapper<Value, uint_fast32_t, hate::math::pow(2, 16) - 1, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	CrossbarInputDropCounter() SYMBOL_VISIBLE;

	/**
	 * Construct from value.
	 * @param value Value
	 */
	CrossbarInputDropCounter(Value value) SYMBOL_VISIBLE;

	/**
	 * Get accumulated drop counter value.
	 * @return Value
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	/**
	 * Set accumulated drop counter value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	bool operator==(CrossbarInputDropCounter const& other) const SYMBOL_VISIBLE;
	bool operator!=(CrossbarInputDropCounter const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CrossbarInputDropCounter const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<CrossbarInputDropCounter>
    : public BackendContainerBase<
          CrossbarInputDropCounter,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Crossbar output event counter counting events routed to an output.
 */
class GENPYBIND(visible) CrossbarOutputEventCounter
{
public:
	typedef halco::hicann_dls::vx::CrossbarOutputOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::
	          RantWrapper<Value, uint_fast32_t, hate::math::pow(2, 24) - 1, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	CrossbarOutputEventCounter() SYMBOL_VISIBLE;

	/**
	 * Construct from value.
	 * @param value Value
	 */
	CrossbarOutputEventCounter(Value value) SYMBOL_VISIBLE;

	/**
	 * Get event counter value.
	 * @return Value
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	/**
	 * Set event counter value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	bool operator==(CrossbarOutputEventCounter const& other) const SYMBOL_VISIBLE;
	bool operator!=(CrossbarOutputEventCounter const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CrossbarOutputEventCounter const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<CrossbarOutputEventCounter>
    : public BackendContainerBase<
          CrossbarOutputEventCounter,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Node of the crossbar routing events from a CrossbarInputOnDLS to a CrossbarOutputOnDLS.
 * A event is routed exactly if the following statement is true:
 *     (event_label & mask) == target
 */
class GENPYBIND(visible) CrossbarNode
{
public:
	typedef halco::hicann_dls::vx::CrossbarNodeOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	typedef halco::hicann_dls::vx::NeuronLabel neuron_label_type;

	/** Default constructor. */
	CrossbarNode() SYMBOL_VISIBLE;

	/**
	 * Get mask to apply with a logical bit-wise AND operation onto incoming event labels.
	 * @return NeuronLabel
	 */
	GENPYBIND(getter_for(mask))
	neuron_label_type get_mask() const SYMBOL_VISIBLE;

	/**
	 * Set mask to apply with a bit-wise AND operation onto incoming event labels.
	 * @param value NeuronLabel
	 */
	GENPYBIND(setter_for(mask))
	void set_mask(neuron_label_type value) SYMBOL_VISIBLE;

	/**
	 * Get target to compare masked incoming event labels to with a bit-wise equality operation.
	 * @return NeuronLabel
	 */
	GENPYBIND(getter_for(target))
	neuron_label_type get_target() const SYMBOL_VISIBLE;

	/**
	 * Set target to compare masked incoming event labels to with a bit-wise equality operation.
	 * @param value NeuronLabel
	 */
	GENPYBIND(setter_for(target))
	void set_target(neuron_label_type value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_drop_counter))
	bool get_enable_drop_counter() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(enable_drop_counter))
	void set_enable_drop_counter(bool value) SYMBOL_VISIBLE;

	bool operator==(CrossbarNode const& other) const SYMBOL_VISIBLE;
	bool operator!=(CrossbarNode const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CrossbarNode const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	neuron_label_type m_mask;
	neuron_label_type m_target;
	bool m_enable_drop_counter;
};

namespace detail {

template <>
struct BackendContainerTrait<CrossbarNode>
    : public BackendContainerBase<
          CrossbarNode,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
