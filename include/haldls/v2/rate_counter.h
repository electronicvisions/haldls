#pragma once

#include "halco/common/genpybind.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/visibility.h"
#include "haldls/v2/common.h"

namespace haldls {
namespace v2 GENPYBIND(tag(haldls_v2)) {

class GENPYBIND(visible) RateCounter
{
public:
	typedef halco::hicann_dls::v2::RateCounterOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Count
		: public halco::common::detail::RantWrapper<Count, uint_fast16_t, 1023, 0>
	{
		constexpr explicit Count(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	RateCounter() SYMBOL_VISIBLE;

	// Read-only property, available when reading the rate counter
	Count get_count(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const SYMBOL_VISIBLE;

	bool get_neuron_enable(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const SYMBOL_VISIBLE;
	void set_neuron_enable(halco::hicann_dls::v2::NeuronOnDLS const& neuron, bool const value) SYMBOL_VISIBLE;

	bool get_fire_interrupt() const SYMBOL_VISIBLE;
	void set_fire_interrupt(bool const value) SYMBOL_VISIBLE;

	bool get_clear_on_read() const SYMBOL_VISIBLE;
	void set_clear_on_read(bool const value) SYMBOL_VISIBLE;

	bool operator==(RateCounter const& other) const SYMBOL_VISIBLE;
	bool operator!=(RateCounter const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = halco::hicann_dls::v2::NeuronOnDLS::size + 2;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	halco::common::typed_array<Count, halco::hicann_dls::v2::NeuronOnDLS> m_counts;
	halco::common::typed_array<bool, halco::hicann_dls::v2::NeuronOnDLS> m_neuron_enable;
	bool m_clear_on_read;
	bool m_fire_interrupt;
};

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::RateCounter::Count)

} // namespace std
