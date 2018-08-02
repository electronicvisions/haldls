#pragma once

#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"
#include "hate/visibility.h"
#include "haldls/v2/common.h"

namespace haldls {
namespace v2 GENPYBIND(tag(haldls_v2)) {

class GENPYBIND(visible) SynapseDriverBlock
{
public:
	typedef halco::hicann_dls::v2::SynapseDriverBlockOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) PulseLength
		: public halco::common::detail::RantWrapper<PulseLength, uint_fast16_t, 31, 0>
	{
		constexpr explicit PulseLength(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	enum class State : uint_fast8_t
	{
		disabled = 0,
		excitatory,
		inhibitory
	};

	typedef std::array<State, halco::hicann_dls::v2::SynapseDriverOnDLS::size> states_type;

	SynapseDriverBlock() SYMBOL_VISIBLE;

	states_type get_states() const SYMBOL_VISIBLE;
	void set_states(states_type const& values) SYMBOL_VISIBLE;

	State get_state(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver) const
		SYMBOL_VISIBLE;
	void set_state(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver, State value)
		SYMBOL_VISIBLE;

	PulseLength get_pulse_length() const SYMBOL_VISIBLE;
	void set_pulse_length(PulseLength const& value) SYMBOL_VISIBLE;

	bool operator==(SynapseDriverBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseDriverBlock const& other) const SYMBOL_VISIBLE;

	static hardware_address_type constexpr config_size_in_words GENPYBIND(hidden) = 3;

	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	PulseLength m_pulse_length;
	states_type m_states;
};

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SynapseDriverBlock::PulseLength)

} // namespace std
