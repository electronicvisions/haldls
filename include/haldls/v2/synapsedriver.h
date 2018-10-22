#pragma once

#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"
#include "hate/visibility.h"
#include "haldls/v2/common.h"

#include "haldls/cerealization.h"

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

	enum class Mode : uint_fast8_t
	{
		disabled = 0,
		excitatory,
		inhibitory
	};

	typedef std::array<Mode, halco::hicann_dls::v2::SynapseDriverOnDLS::size> modes_type;

	SynapseDriverBlock() SYMBOL_VISIBLE;

	modes_type get_modes() const SYMBOL_VISIBLE;
	void set_modes(modes_type const& values) SYMBOL_VISIBLE;

	Mode get_mode(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver) const
		SYMBOL_VISIBLE;
	void set_mode(halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver, Mode value)
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
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	PulseLength m_pulse_length;
	modes_type m_modes;
};

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SynapseDriverBlock::PulseLength)

} // namespace std
