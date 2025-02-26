#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/v4/event.h"
#include "halco/hicann-dls/vx/v4/global_merger_matrix.h"
#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

#ifndef __ppu__
#include "hxcomm/vx/target.h"
#endif

namespace haldls::vx::v4 {

struct GlobalMergerMatrixOutputConfig;
struct GlobalMergerMatrixInputDropCounter;
struct GlobalMergerMatrixOutputEventCounter;
struct GlobalMergerMatrixNode;
struct NeuronLabelToGL1EventLUTEntry;

} // namespace haldls::vx::v4

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::v4::GlobalMergerMatrixOutputConfig& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::v4::GlobalMergerMatrixInputDropCounter& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::v4::GlobalMergerMatrixOutputEventCounter& value,
    std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v4::GlobalMergerMatrixNode& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v4::NeuronLabelToGL1EventLUTEntry& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls::vx { namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) GlobalMergerMatrixOutputConfig
    : public ContainerBase<GlobalMergerMatrixOutputConfig>
{
public:
	typedef halco::hicann_dls::vx::v4::GlobalMergerMatrixOutputConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware};
#endif

	/** Default constructor. */
	GlobalMergerMatrixOutputConfig() SYMBOL_VISIBLE;

	typedef halco::common::
	    typed_array<bool, halco::hicann_dls::vx::v4::GlobalMergerMatrixOutputChannelOnDLS>
	        enable_event_counter_type GENPYBIND(opaque(false));
	typedef halco::common::
	    typed_array<bool, halco::hicann_dls::vx::v4::GlobalMergerMatrixOutputChannelOnDLS>
	        enable_slow_type GENPYBIND(opaque(false));

	/**
	 * Get enable value for event counters.
	 * @return Boolean values
	 */
	GENPYBIND(getter_for(enable_event_counter), return_value_policy(reference_internal))
	enable_event_counter_type const& get_enable_event_counter() const SYMBOL_VISIBLE;

	/**
	 * Set enable value for event counters.
	 * @param value Boolean values
	 */
	GENPYBIND(setter_for(enable_event_counter))
	void set_enable_event_counter(enable_event_counter_type const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_slow), return_value_policy(reference_internal))
	enable_slow_type const& get_enable_slow() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(enable_slow))
	void set_enable_slow(enable_slow_type const& value) SYMBOL_VISIBLE;

	bool operator==(GlobalMergerMatrixOutputConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(GlobalMergerMatrixOutputConfig const& other) const SYMBOL_VISIBLE;

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
	friend std::ostream& operator<<(std::ostream& os, GlobalMergerMatrixOutputConfig const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    GlobalMergerMatrixOutputConfig& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	enable_event_counter_type m_enable_event_counter;
	enable_slow_type m_enable_slow;
};


/**
 * GlobalMergerMatrix input drop counter accumulating drops at all outputs for which the drop
 * counter accumulation is enabled in the corresponding GlobalMergerMatrixNode.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) GlobalMergerMatrixInputDropCounter
    : public ContainerBase<GlobalMergerMatrixInputDropCounter>
{
public:
	typedef halco::hicann_dls::vx::v4::GlobalMergerMatrixInputDropCounterOnDLS coordinate_type;
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
	GlobalMergerMatrixInputDropCounter() SYMBOL_VISIBLE;

	/**
	 * Construct from value.
	 * @param value Value
	 */
	GlobalMergerMatrixInputDropCounter(Value value) SYMBOL_VISIBLE;

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

	bool operator==(GlobalMergerMatrixInputDropCounter const& other) const SYMBOL_VISIBLE;
	bool operator!=(GlobalMergerMatrixInputDropCounter const& other) const SYMBOL_VISIBLE;

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
	friend std::ostream& operator<<(
	    std::ostream& os, GlobalMergerMatrixInputDropCounter const& config) SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    GlobalMergerMatrixInputDropCounter& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value;
};


/**
 * GlobalMergerMatrix output event counter counting events routed to an output.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) GlobalMergerMatrixOutputEventCounter
    : public ContainerBase<GlobalMergerMatrixOutputEventCounter>
{
public:
	typedef halco::hicann_dls::vx::v4::GlobalMergerMatrixOutputEventCounterOnDLS coordinate_type;
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
	GlobalMergerMatrixOutputEventCounter() SYMBOL_VISIBLE;

	/**
	 * Construct from value.
	 * @param value Value
	 */
	GlobalMergerMatrixOutputEventCounter(Value value) SYMBOL_VISIBLE;

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

	bool operator==(GlobalMergerMatrixOutputEventCounter const& other) const SYMBOL_VISIBLE;
	bool operator!=(GlobalMergerMatrixOutputEventCounter const& other) const SYMBOL_VISIBLE;

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
	friend std::ostream& operator<<(
	    std::ostream& os, GlobalMergerMatrixOutputEventCounter const& config) SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    GlobalMergerMatrixOutputEventCounter& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value;
};


/**
 * Node of the global merger matrix routing events from a GlobalMergerMatrixInputOnDLS to a
 * GlobalMergerMatrixOutputOnDLS. A event is routed exactly if the following statement is true:
 *     ((event_label & mask) == target) && event_gl1_index \in node_gl1_indices
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) GlobalMergerMatrixNode
    : public ContainerBase<GlobalMergerMatrixNode>
{
public:
	typedef halco::hicann_dls::vx::v4::GlobalMergerMatrixNodeOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware};
#endif

	typedef halco::hicann_dls::vx::v4::NeuronLabel neuron_label_type;

	constexpr static size_t GMM_WIDTH = 18;
	constexpr static size_t IDX_WIDTH =
	    GMM_WIDTH - hate::math::num_bits(halco::hicann_dls::vx::v4::SpikeLabel::max);

	struct GENPYBIND(inline_base("*")) GL1Index
	    : public halco::common::detail::
	          RantWrapper<GL1Index, uint_fast32_t, hate::math::pow(2, IDX_WIDTH) - 1, 0>
	{
		constexpr explicit GL1Index(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	typedef halco::common::typed_array<bool, GL1Index> accept_gl1_index_type GENPYBIND(opaque);

	/** Default constructor. */
	GlobalMergerMatrixNode() SYMBOL_VISIBLE;

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

	/**
	 * Get values for gl1 index acceptance.
	 * @return Boolean values
	 */
	GENPYBIND(getter_for(accept_gl1_index), return_value_policy(reference_internal))
	accept_gl1_index_type const& get_accept_gl1_index() const SYMBOL_VISIBLE;

	/**
	 * Set values for gl1 index acceptance.
	 * @param value Boolean values
	 */
	GENPYBIND(setter_for(accept_gl1_index))
	void set_accept_gl1_index(accept_gl1_index_type const& value) SYMBOL_VISIBLE;

	bool operator==(GlobalMergerMatrixNode const& other) const SYMBOL_VISIBLE;
	bool operator!=(GlobalMergerMatrixNode const& other) const SYMBOL_VISIBLE;

	static const SYMBOL_VISIBLE GlobalMergerMatrixNode drop_all;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, GlobalMergerMatrixNode const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, GlobalMergerMatrixNode& value, std::uint32_t const version) SYMBOL_VISIBLE;

	neuron_label_type m_mask;
	neuron_label_type m_target;
	bool m_enable_drop_counter;
	accept_gl1_index_type m_accept_gl1_index;
};


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) NeuronLabelToGL1EventLUTEntry
    : public ContainerBase<NeuronLabelToGL1EventLUTEntry>
{
public:
	typedef halco::hicann_dls::vx::v4::NeuronLabelToGL1EventLUTEntryOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware};
#endif

	/** Default constructor. */
	NeuronLabelToGL1EventLUTEntry() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(global_merger_matrix_mutable), return_value_policy(reference_internal))
	halco::hicann_dls::vx::v4::SpikeLabel::GlobalMergerMatrixMutable
	get_global_merger_matrix_mutable() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(global_merger_matrix_mutable))
	void set_global_merger_matrix_mutable(
	    halco::hicann_dls::vx::v4::SpikeLabel::GlobalMergerMatrixMutable value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(gl1_index), return_value_policy(reference_internal))
	GlobalMergerMatrixNode::GL1Index get_gl1_index() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(gl1_index))
	void set_gl1_index(GlobalMergerMatrixNode::GL1Index value) SYMBOL_VISIBLE;

	bool operator==(NeuronLabelToGL1EventLUTEntry const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronLabelToGL1EventLUTEntry const& other) const SYMBOL_VISIBLE;

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
	friend std::ostream& operator<<(std::ostream& os, NeuronLabelToGL1EventLUTEntry const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    NeuronLabelToGL1EventLUTEntry& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	halco::hicann_dls::vx::v4::SpikeLabel::GlobalMergerMatrixMutable m_global_merger_matrix_mutable;
	GlobalMergerMatrixNode::GL1Index m_gl1_index;
};

} // namespace v4
} // namespace haldls::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<v4::GlobalMergerMatrixOutputConfig>
    : public BackendContainerBase<
          v4::GlobalMergerMatrixOutputConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v4::GlobalMergerMatrixInputDropCounter>
    : public BackendContainerBase<
          v4::GlobalMergerMatrixInputDropCounter,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v4::GlobalMergerMatrixOutputEventCounter>
    : public BackendContainerBase<
          v4::GlobalMergerMatrixOutputEventCounter,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v4::GlobalMergerMatrixNode>
    : public BackendContainerBase<
          v4::GlobalMergerMatrixNode,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v4::NeuronLabelToGL1EventLUTEntry>
    : public BackendContainerBase<
          v4::NeuronLabelToGL1EventLUTEntry,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace haldls::vx::detail
