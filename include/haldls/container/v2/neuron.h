#pragma once

#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/passkey.h"
#include "haldls/common/visibility.h"
#include "haldls/container/v2/common.h"

namespace haldls {
namespace container {
namespace v2 {

class Chip;

class CommonNeuronConfig
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	struct PostCorrelationSignalLength
		: public halco::common::detail::
			  RantWrapper<PostCorrelationSignalLength, uint_fast16_t, 15, 0>
	{
		constexpr explicit PostCorrelationSignalLength(uintmax_t const val = 0) HALDLS_VISIBLE
			: rant_t(val)
		{}
	};

	CommonNeuronConfig() HALDLS_VISIBLE;

	bool get_enable_digital_out() const HALDLS_VISIBLE;
	void set_enable_digital_out(bool const value) HALDLS_VISIBLE;
	PostCorrelationSignalLength get_post_correlation_signal_length() const HALDLS_VISIBLE;
	void set_post_correlation_signal_length(PostCorrelationSignalLength const& value) HALDLS_VISIBLE;
	bool get_enable_external_post_correlation_signal() const HALDLS_VISIBLE;
	void set_enable_external_post_correlation_signal(bool const value) HALDLS_VISIBLE;
	bool get_inhibit_spike_comparator() const HALDLS_VISIBLE;
	void set_inhibit_spike_comparator(bool const value) HALDLS_VISIBLE;

	bool operator==(CommonNeuronConfig const& other) const HALDLS_VISIBLE;
	bool operator!=(CommonNeuronConfig const& other) const HALDLS_VISIBLE;

	static size_t constexpr config_size_in_words = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(coordinate_type const& unique) const HALDLS_VISIBLE;
	std::array<hardware_word_type, config_size_in_words> encode() const HALDLS_VISIBLE;
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) HALDLS_VISIBLE;

private:
	bool m_digital_out;
	PostCorrelationSignalLength m_post_correlation_signal_length;
	bool m_external_correlation_signal;
	bool m_inhibit_spike_comparator;
};

/// \brief Digital configuration of a single neuron.
/// \see Figure 1b of the paper "A highly tunable 65-nm CMOS LIF neuron for a large scale neuromorphic system".
class NeuronDigitalConfig
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

	NeuronDigitalConfig() HALDLS_VISIBLE;

	// accessors
	bool get_enable_synapse_input_excitatory() const HALDLS_VISIBLE;
	void set_enable_synapse_input_excitatory(bool const value) HALDLS_VISIBLE;
	bool get_enable_synapse_input_inhibitory() const HALDLS_VISIBLE;
	void set_enable_synapse_input_inhibitory(bool const value) HALDLS_VISIBLE;
	bool get_enable_high_conductance_leak() const HALDLS_VISIBLE;
	void set_enable_high_conductance_leak(bool const value) HALDLS_VISIBLE;
	bool get_enable_leak() const HALDLS_VISIBLE;
	void set_enable_leak(bool const value) HALDLS_VISIBLE;
	bool get_enable_bigcap() const HALDLS_VISIBLE;
	void set_enable_bigcap(bool const value) HALDLS_VISIBLE;
	bool get_enable_smallcap() const HALDLS_VISIBLE;
	void set_enable_smallcap(bool const value) HALDLS_VISIBLE;
	FireOutMode get_fire_out_mode() const HALDLS_VISIBLE;
	void set_fire_out_mode(FireOutMode const value) HALDLS_VISIBLE;
	MuxReadoutMode get_mux_readout_mode() const HALDLS_VISIBLE;
	void set_mux_readout_mode(MuxReadoutMode const value) HALDLS_VISIBLE;

	bool get_enable_external_current_input() const HALDLS_VISIBLE;

	/// \brief Enable or disable external current input for this neuron.
	/// \note As only one neuron per chip can receive external current input, access to
	///       this setting is disabled for single neurons.
	/// \see Chip::enable_external_current_input() and
	///      Chip::disable_external_current_input().
	void set_enable_external_current_input(bool const value, common::Passkey<Chip> const& passkey) HALDLS_VISIBLE;

	bool get_enable_external_voltage_output() const HALDLS_VISIBLE;

	/// \brief Enable or disable external voltage output for this neuron.
	/// \note As only one neuron per chip can provide voltage output, access to this
	///       setting is disabled for single neurons.
	/// \see Chip::enable_external_voltage_output() and
	///      Chip::disable_external_voltage_output().
	void set_enable_external_voltage_output(bool const value, common::Passkey<Chip> const& passkey) HALDLS_VISIBLE;

	bool operator==(NeuronDigitalConfig const& other) const HALDLS_VISIBLE;
	bool operator!=(NeuronDigitalConfig const& other) const HALDLS_VISIBLE;

	static size_t constexpr config_size_in_words = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(coordinate_type const& neurn) const HALDLS_VISIBLE;
	std::array<hardware_word_type, config_size_in_words> encode() const HALDLS_VISIBLE;
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) HALDLS_VISIBLE;

private:
	bool m_synapse_input_exc;
	bool m_synapse_input_inh;
	bool m_leak_high_conductance;
	bool m_leak;
	bool m_bigcap;
	bool m_smallcap;
	FireOutMode m_fire_out_mode;
	MuxReadoutMode m_mux_readout_mode;
	bool m_external_current_input;
	bool m_external_voltage_output;
};

}  // namespace v2
}  // namespace container
}  // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonNeuronConfig::PostCorrelationSignalLength)

} // namespace std
