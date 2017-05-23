#pragma once

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"

namespace haldls {
namespace container {
namespace v2 {

class RateCounter
{
public:
	struct Count
		: public halco::common::detail::RantWrapper<Count, uint_fast16_t, 1023, 0>
	{
		constexpr explicit Count(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	RateCounter() HALDLS_VISIBLE;

	// Read-only property, available when reading the rate counter
	Count get_count(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const HALDLS_VISIBLE;

	bool get_neuron_enable(halco::hicann_dls::v2::NeuronOnDLS const& neuron) const HALDLS_VISIBLE;
	void set_neuron_enable(halco::hicann_dls::v2::NeuronOnDLS const& neuron, bool const value) HALDLS_VISIBLE;

	bool get_fire_interrupt() const HALDLS_VISIBLE;
	void set_fire_interrupt(bool const value) HALDLS_VISIBLE;

	bool get_clear_on_read() const HALDLS_VISIBLE;
	void set_clear_on_read(bool const value) HALDLS_VISIBLE;

	bool operator==(RateCounter const& other) const HALDLS_VISIBLE;
	bool operator!=(RateCounter const& other) const HALDLS_VISIBLE;

private:
	halco::common::typed_array<Count, halco::hicann_dls::v2::NeuronOnDLS> m_counts;
	halco::common::typed_array<bool, halco::hicann_dls::v2::NeuronOnDLS> m_neuron_enable;
	bool m_clear_on_read;
	bool m_fire_interrupt;
};

} // namespace v2
} // namespace container
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::RateCounter::Count)

} // namespace std
