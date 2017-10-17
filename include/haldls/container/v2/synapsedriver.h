#pragma once

#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/common/visibility.h"
#include "haldls/container/v2/common.h"

namespace haldls {
namespace container {
namespace v2 GENPYBIND(tag(haldls_container_v2)) {

class GENPYBIND(visible) SynapseDrivers
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) PulseLength
		: public halco::common::detail::RantWrapper<PulseLength, uint_fast16_t, 31, 0>
	{
		constexpr explicit PulseLength(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	enum class State : uint_fast8_t
	{
		disabled = 0,
		excitatory,
		inhibitory
	};

	typedef std::array<State, halco::hicann_dls::v2::SynapseDriverOnDLS::size> states_type;

	SynapseDrivers() HALDLS_VISIBLE;

	states_type get_states() const HALDLS_VISIBLE;
	void set_states(states_type const& values) HALDLS_VISIBLE;

	State get_state(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver) const
		HALDLS_VISIBLE;
	void set_state(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver, State value)
		HALDLS_VISIBLE;

	PulseLength get_pulse_length() const HALDLS_VISIBLE;
	void set_pulse_length(PulseLength const& value) HALDLS_VISIBLE;

	bool operator==(SynapseDrivers const& other) const HALDLS_VISIBLE;
	bool operator!=(SynapseDrivers const& other) const HALDLS_VISIBLE;

	static hardware_address_type constexpr config_size_in_words GENPYBIND(hidden) = 3;

	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const HALDLS_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) HALDLS_VISIBLE GENPYBIND(hidden);

private:
	PulseLength m_pulse_length;
	states_type m_states;
};

} // namespace v2
} // namespace container
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::SynapseDrivers::PulseLength)

} // namespace std
