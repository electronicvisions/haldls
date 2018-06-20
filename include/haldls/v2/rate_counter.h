#pragma once

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/genpybind.h"

#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {

/**
 * Configuration of neuron spike-rate counters.
 */
class GENPYBIND(visible) RateCounterConfig
{
public:
	typedef halco::hicann_dls::v2::RateCounterConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Default construct confiuration container.
	 */
	RateCounterConfig() SYMBOL_VISIBLE;

	/**
	 * Get enable rate counter value for a neuron.
	 * @param neuron Neuron to get value for
	 * @return Boolean enable value
	 */
	bool get_enable_neuron(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const SYMBOL_VISIBLE;

	/**
	 * Set enable rate counter value for a neuron.
	 * @param neuron Neuron to set value for
	 * @param value Boolean enable value to set
	 */
	void set_enable_neuron(halco::hicann_dls::v2::NeuronOnDLS const& neuron, bool const value) SYMBOL_VISIBLE;

	/**
	 * Get enable interrupt on fire value.
	 * @return Boolean enable value
	 */
	GENPYBIND(getter_for(enable_fire_interrupt))
	bool get_enable_fire_interrupt() const SYMBOL_VISIBLE;

	/**
	 * Set enable interrupt on fire value.
	 * @param value Boolean enable value to set
	 */
	GENPYBIND(setter_for(enable_fire_interrupt))
	void set_enable_fire_interrupt(bool const value) SYMBOL_VISIBLE;

	/**
	 * Get enable clear on read value.
	 * On true, rate counter counts are reset to RateCounterEntry::Count(0) on read operation.
	 * @return Boolean enable value
	 */
	GENPYBIND(getter_for(enable_clear_on_read))
	bool get_enable_clear_on_read() const SYMBOL_VISIBLE;

	/**
	 * Set enable clear on read value.
	 * On true, rate counter counts are reset to RateCounterEntry::Count(0) on read operation.
	 * @param value Boolean enable value
	 */
	GENPYBIND(setter_for(enable_clear_on_read))
	void set_enable_clear_on_read(bool const value) SYMBOL_VISIBLE;

	/**
	 * Equality operator.
	 * @param other RateCounterConfig to compare to
	 * @return Result of comparison
	 */
	bool operator==(RateCounterConfig const& other) const SYMBOL_VISIBLE;

	/**
	 * Inequality operator.
	 * @param other RateCounterConfig to compare to
	 * @return Result of comparison
	 */
	bool operator!=(RateCounterConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<bool, halco::hicann_dls::v2::NeuronOnDLS> m_enable_neuron;
	bool m_clear_on_read;
	bool m_fire_interrupt;
};

/**
 * Rate counter count for one neuron.
 * Writing this container with any value results in the counter being set to RateCounterEntry::Count(0).
 */
class GENPYBIND(visible) RateCounterEntry
{
public:
	typedef halco::hicann_dls::v2::NeuronOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Count of rate counter.
	 */
	struct GENPYBIND(inline_base("*")) Count
		: public halco::common::detail::RantWrapper<Count, uint_fast16_t, 1023, 0>
	{
		constexpr explicit Count(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	/**
	 * Default constructor.
	 */
	RateCounterEntry() SYMBOL_VISIBLE;

	/**
	 * Get count, i.e. number of spikes since last reset.
	 * @return Count
	 */
	GENPYBIND(getter_for(count))
	Count get_count() const SYMBOL_VISIBLE;

	/**
	 * Equality operator.
	 * @param other RateCounterEntry to compare to
	 * @return Result of comparison
	 */
	bool operator==(RateCounterEntry const& other) const SYMBOL_VISIBLE;

	/**
	 * Inequality operator.
	 * @param other RateCounterEntry to compare to
	 * @return Result of comparison
	 */
	bool operator!=(RateCounterEntry const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const&) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	Count m_count;
};

/**
 * Ensemble of RateCounterEntry containers for all neurons.
 * Writing this container with any value results in the counters being set to RateCounterEntry::Count(0).
 */
class GENPYBIND(visible) RateCounter
{
public:
	typedef halco::hicann_dls::v2::RateCounterOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	typedef RateCounterEntry::Count Count;

	/**
	 * Default constructor.
	 */
	RateCounter() SYMBOL_VISIBLE;

	/**
	 * Get count, i.e. number of spikes since last reset for a specified neuron.
	 * @param neuron Neuron to get count for
	 * @return Count
	 */
	Count get_count(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const SYMBOL_VISIBLE;

	/**
	 * Equality operator.
	 * @param other RateCounter to compare to
	 * @return Result of comparison
	 */
	bool operator==(RateCounter const& other) const SYMBOL_VISIBLE;

	/**
	 * Inequality operator.
	 * @param other RateCounter to compare to
	 * @return Result of comparison
	 */
	bool operator!=(RateCounter const& other) const SYMBOL_VISIBLE;

private:
	friend detail::VisitPreorderImpl<RateCounter>;

	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<RateCounterEntry, halco::hicann_dls::v2::NeuronOnDLS> m_counts;
};

namespace detail {

template <>
struct VisitPreorderImpl<RateCounter> {
	template <typename ContainerT, typename VisitorT>
	static void call(ContainerT& config, halco::hicann_dls::v2::RateCounterOnDLS const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::v2;

		visitor(coord, config);

		for (auto const neuron: iter_all<NeuronOnDLS>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			visit_preorder(config.m_counts[neuron], neuron, visitor);
		}
	}
};

} // namespace detail

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::RateCounterEntry::Count)

} // namespace std
