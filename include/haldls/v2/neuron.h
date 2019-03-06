#pragma once

#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/passkey.h"
#include "hate/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/genpybind.h"

#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {

class Chip;
class PlaybackProgram;

class GENPYBIND(visible) CommonNeuronConfig
{
public:
	typedef halco::hicann_dls::v2::CommonNeuronConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) PostCorrelationSignalLength
		: public halco::common::detail::
			  RantWrapper<PostCorrelationSignalLength, uint_fast16_t, 15, 0>
	{
		constexpr explicit PostCorrelationSignalLength(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE
			: rant_t(val)
		{}
	};

	CommonNeuronConfig() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_digital_out))
	bool get_enable_digital_out() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_digital_out))
	void set_enable_digital_out(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(post_correlation_signal_length))
	PostCorrelationSignalLength get_post_correlation_signal_length() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(post_correlation_signal_length))
	void set_post_correlation_signal_length(PostCorrelationSignalLength const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_external_post_correlation_signal))
	bool get_enable_external_post_correlation_signal() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_external_post_correlation_signal))
	void set_enable_external_post_correlation_signal(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(inhibit_spike_comparator))
	bool get_inhibit_spike_comparator() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(inhibit_spike_comparator))
	void set_inhibit_spike_comparator(bool const value) SYMBOL_VISIBLE;

	bool operator==(CommonNeuronConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonNeuronConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	bool m_digital_out;
	PostCorrelationSignalLength m_post_correlation_signal_length;
	bool m_external_correlation_signal;
	bool m_inhibit_spike_comparator;
};

/// \brief Digital configuration of a single neuron.
/// \see Figure 1b of the paper "A highly tunable 65-nm CMOS LIF neuron for a large scale neuromorphic system".
class GENPYBIND(visible) NeuronDigitalConfig
{
public:
	typedef halco::hicann_dls::v2::NeuronOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	enum class FireOutMode : uint_fast8_t
	{
		disabled = 0,
		enabled = 1,
		bypass_exc = 2,
		bypass_inh = 3
	};

	enum class MuxReadoutMode : uint_fast8_t
	{
		v_mem = 0,
		e_psp = 1,
		i_psp = 2,
		spike = 3
	};

	NeuronDigitalConfig() SYMBOL_VISIBLE;

	// accessors
	GENPYBIND(getter_for(enable_synapse_input_excitatory))
	bool get_enable_synapse_input_excitatory() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_synapse_input_excitatory))
	void set_enable_synapse_input_excitatory(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_synapse_input_inhibitory))
	bool get_enable_synapse_input_inhibitory() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_synapse_input_inhibitory))
	void set_enable_synapse_input_inhibitory(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_high_conductance_leak))
	bool get_enable_high_conductance_leak() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_high_conductance_leak))
	void set_enable_high_conductance_leak(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_leak))
	bool get_enable_leak() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_leak))
	void set_enable_leak(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_bigcap))
	bool get_enable_bigcap() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_bigcap))
	void set_enable_bigcap(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_smallcap))
	bool get_enable_smallcap() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_smallcap))
	void set_enable_smallcap(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(fire_out_mode))
	FireOutMode get_fire_out_mode() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(fire_out_mode))
	void set_fire_out_mode(FireOutMode const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(mux_readout_mode))
	MuxReadoutMode get_mux_readout_mode() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(mux_readout_mode))
	void set_mux_readout_mode(MuxReadoutMode const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_unbuffered_readout))
	bool get_enable_unbuffered_readout() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_unbuffered_readout))
	void set_enable_unbuffered_readout(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_buffered_readout))
	bool get_enable_buffered_readout() const SYMBOL_VISIBLE;

	/// \brief Enable or disable external voltage output for this neuron.
	/// \note As only one neuron per chip can provide voltage output, access to this
	///       setting is disabled for single neurons.
	/// \see Chip::enable_buffered_readout() and
	///      Chip::disable_buffered_readout().
	void set_enable_buffered_readout(
		bool const value,
		hate::Passkey<Chip, PlaybackProgram> const& passkey) SYMBOL_VISIBLE;

	bool operator==(NeuronDigitalConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronDigitalConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& neurn) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	bool m_synapse_input_exc;
	bool m_synapse_input_inh;
	bool m_leak_high_conductance;
	bool m_leak;
	bool m_bigcap;
	bool m_smallcap;
	FireOutMode m_fire_out_mode;
	MuxReadoutMode m_mux_readout_mode;
	bool m_unbuffered_readout;
	bool m_buffered_readout;
};

}  // namespace v2
}  // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::CommonNeuronConfig::PostCorrelationSignalLength)

} // namespace std
