#pragma once

#include <ostream>

#include "halco/hicann-dls/vx/coordinates.h"

#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"


namespace cereal {

class access;

} // namespace cereal

namespace fisch::vx {

class OmnibusChipOverJTAG;
class OmnibusChip;

} // namespace fisch::vx

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

/**
 * A configuration container for the top-level readout mux, selecting the connections between
 * the two analog readout pads and different components of the chip.
 * There are two equal instances of this mux on the chip, for the two pads.
 */
class GENPYBIND(visible) ReadoutMuxConfig
{
public:
	typedef halco::hicann_dls::vx::ReadoutMuxConfigOnDLS coordinate_type;

	typedef std::true_type is_leaf_node;

	/** Default constructor with all inputs disabled. */
	ReadoutMuxConfig() SYMBOL_VISIBLE;

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::CapMemBlockOnDLS>
	    capmem_quadrant_type GENPYBIND(opaque(false));
	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::HemisphereOnDLS> hemisphere_type
	    GENPYBIND(opaque(false));
	typedef halco::common::
	    typed_array<bool, halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock>
	        buffer_type GENPYBIND(opaque(false));

	/** Select CADC v_ramp outputs per quadrant. */
	GENPYBIND(getter_for(cadc_v_ramp_mux), return_value_policy(reference))
	capmem_quadrant_type const& get_cadc_v_ramp_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cadc_v_ramp_mux), return_value_policy(reference))
	void set_cadc_v_ramp_mux(capmem_quadrant_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of cadc_v_ramp_mux to pad. */
	GENPYBIND(getter_for(cadc_v_ramp_mux_to_pad))
	bool get_cadc_v_ramp_mux_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cadc_v_ramp_mux_to_pad))
	void set_cadc_v_ramp_mux_to_pad(bool value) SYMBOL_VISIBLE;

	/** Select CapMem i_out outputs per quadrant. */
	GENPYBIND(getter_for(capmem_i_out_mux), return_value_policy(reference))
	capmem_quadrant_type const& get_capmem_i_out_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_i_out_mux), return_value_policy(reference))
	void set_capmem_i_out_mux(capmem_quadrant_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of capmem_i_out_mux to intermediate mux.
	 * To reach the pad, the connection between the CapMem intermediate mux and the pad has to be
	 * enabled as well.
	 */
	GENPYBIND(getter_for(capmem_i_out_mux_to_capmem_intermediate_mux))
	bool get_capmem_i_out_mux_to_capmem_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_i_out_mux_to_capmem_intermediate_mux))
	void set_capmem_i_out_mux_to_capmem_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Enable connection of CapMem intermediate mux to pad.
	 * The intermediate mux selects between different capmem-specific sources. */
	GENPYBIND(getter_for(capmem_intermediate_mux_to_pad))
	bool get_capmem_intermediate_mux_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_intermediate_mux_to_pad))
	void set_capmem_intermediate_mux_to_pad(bool value) SYMBOL_VISIBLE;

	/** Select CapMem v_out outputs per quadrant. */
	GENPYBIND(getter_for(capmem_v_out_mux), return_value_policy(reference))
	capmem_quadrant_type const& get_capmem_v_out_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_out_mux), return_value_policy(reference))
	void set_capmem_v_out_mux(capmem_quadrant_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of capmem_v_out_mux to intermediate mux.
	 * To reach the pad, the connection between the CapMem intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(capmem_v_out_mux_to_capmem_intermediate_mux))
	bool get_capmem_v_out_mux_to_capmem_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_out_mux_to_capmem_intermediate_mux))
	void set_capmem_v_out_mux_to_capmem_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Select CapMem v_ref outputs per quadrant. */
	GENPYBIND(getter_for(capmem_v_ref_mux), return_value_policy(reference))
	capmem_quadrant_type const& get_capmem_v_ref_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_ref_mux), return_value_policy(reference))
	void set_capmem_v_ref_mux(capmem_quadrant_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of capmem_v_ref_mux to intermediate mux.
	 * To reach the pad, the connection between the CapMem intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(capmem_v_ref_mux_to_capmem_intermediate_mux))
	bool get_capmem_v_ref_mux_to_capmem_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_ref_mux_to_capmem_intermediate_mux))
	void set_capmem_v_ref_mux_to_capmem_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Select neuron unbuffered membrane access i_stim per hemisphere. */
	GENPYBIND(getter_for(neuron_i_stim_mux), return_value_policy(reference))
	hemisphere_type const& get_neuron_i_stim_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(neuron_i_stim_mux), return_value_policy(reference))
	void set_neuron_i_stim_mux(hemisphere_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of neuron unbuffered membrane access neuron_i_stim_mux to pad. */
	GENPYBIND(getter_for(neuron_i_stim_mux_to_pad))
	bool get_neuron_i_stim_mux_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(neuron_i_stim_mux_to_pad))
	void set_neuron_i_stim_mux_to_pad(bool value) SYMBOL_VISIBLE;

	/** Enable connection of acausal CADC debug line to intermediate mux.
	 * To reach the pad, the connection between the synapse intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(cadc_debug_acausal_to_synapse_intermediate_mux))
	bool get_cadc_debug_acausal_to_synapse_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cadc_debug_acausal_to_synapse_intermediate_mux))
	void set_cadc_debug_acausal_to_synapse_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Enable connection of causal CADC debug line to intermediate mux.
	 * To reach the pad, the connection between the synapse intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(cadc_debug_causal_to_synapse_intermediate_mux))
	bool get_cadc_debug_causal_to_synapse_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cadc_debug_causal_to_synapse_intermediate_mux))
	void set_cadc_debug_causal_to_synapse_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Enable connection of inhibitory synaptic input debug line to intermediate mux.
	 * To reach the pad, the connection between the synapse intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(synin_debug_inhibitory_to_synapse_intermediate_mux))
	bool get_synin_debug_inhibitory_to_synapse_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(synin_debug_inhibitory_to_synapse_intermediate_mux))
	void set_synin_debug_inhibitory_to_synapse_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Enable connection of excitatory synaptic input debug line to intermediate mux.
	 * To reach the pad, the connection between the synapse intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(synin_debug_excitatory_to_synapse_intermediate_mux))
	bool get_synin_debug_excitatory_to_synapse_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(synin_debug_excitatory_to_synapse_intermediate_mux))
	void set_synin_debug_excitatory_to_synapse_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Enable connection of synapse intermediate mux to pad.
	 * The intermediate mux selects between different sources in the synapse array,
	 * namely the debug lines at the synaptic input and the correlation readout. */
	GENPYBIND(getter_for(synapse_intermediate_mux_to_pad))
	bool get_synapse_intermediate_mux_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(synapse_intermediate_mux_to_pad))
	void set_synapse_intermediate_mux_to_pad(bool value) SYMBOL_VISIBLE;

	/** Enable connection of voltage readout buffers to pad.
	 * The connections to the voltage readout muxes/buffers are handled in a separate container. */
	GENPYBIND(getter_for(buffer_to_pad), return_value_policy(reference))
	buffer_type const& get_buffer_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(buffer_to_pad), return_value_policy(reference))
	void set_buffer_to_pad(buffer_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of differential debug output to pad.
	 * Enabling this connection on pad 0 yields the plus/signal part of the differential output,
	 * enabling this connection on pad 1 yields the minus/reference part of the differential output.
	 */
	GENPYBIND(getter_for(differential_to_pad))
	bool get_differential_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(differential_to_pad))
	void set_differential_to_pad(bool value) SYMBOL_VISIBLE;

	bool operator==(ReadoutMuxConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ReadoutMuxConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ReadoutMuxConfig const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;

	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& words) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	// CADC
	capmem_quadrant_type m_cadc_v_ramp_mux;
	bool m_cadc_v_ramp_mux_to_pad;

	// CapMem
	capmem_quadrant_type m_capmem_i_out_mux;
	bool m_capmem_i_out_mux_to_inter;
	bool m_capmem_inter_mux_to_pad;
	capmem_quadrant_type m_capmem_v_out_mux;
	bool m_capmem_v_out_mux_to_inter;
	capmem_quadrant_type m_capmem_v_ref_mux;
	bool m_capmem_v_ref_mux_to_inter;

	// Neuron
	hemisphere_type m_neuron_i_stim_mux;
	bool m_neuron_i_stim_mux_to_pad;

	// Synapse array
	bool m_cadc_debug_acausal_to_inter;
	bool m_cadc_debug_causal_to_inter;
	bool m_synin_debug_inhibitory_to_inter;
	bool m_synin_debug_excitatory_to_inter;
	bool m_synapse_inter_mux_to_pad;

	// Other readout chain outputs
	buffer_type m_buffer_to_pad;
	bool m_differential_to_pad;
};


/**
 * Configuration container for the two mux and buffer blocks for voltage readout.
 * Allows selection of various signal sources on chip and drives the signals.
 * To reach the pads, the buffer_to_pad option in the ReadoutMuxConfig needs to be set.
 *
 * Some of the selectable voltages in this container can also be selected in the
 * ReadoutMuxConfig directly. Note that there the connection is direct, not buffered,
 * while here the signal is amplified. Do not enable both connections, as there
 * will be feedback otherwise!
 */
class GENPYBIND(visible) ReadoutBufferConfigBlock
{
public:
	typedef halco::hicann_dls::vx::ReadoutBufferConfigBlockOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct ReadoutBufferConfig
	{
		/** Default constructor with all inputs disabled. */
		ReadoutBufferConfig() SYMBOL_VISIBLE;

		typedef halco::common::typed_array<bool, halco::hicann_dls::vx::HemisphereOnDLS>
		    hemisphere_type GENPYBIND(opaque(false));

		/** Connect plus/signal line of differential debug readout to mux. */
		GENPYBIND(getter_for(differential_signal))
		bool get_differential_signal() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(differential_signal))
		void set_differential_signal(bool value) SYMBOL_VISIBLE;

		/** Connect minus/reference line of differential debug readout to mux. */
		GENPYBIND(getter_for(differential_reference))
		bool get_differential_reference() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(differential_reference))
		void set_differential_reference(bool value) SYMBOL_VISIBLE;

		/** Connect output line of idac_i_out to mux.
		 * This current-dac in the readout section can also
		 * be connected to neuron membranes or the synaptic input, to mux. */
		GENPYBIND(getter_for(current_dac))
		bool get_current_dac() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(current_dac))
		void set_current_dac(bool value) SYMBOL_VISIBLE;

		/** Connect synaptic input inhibitory debug line to mux. */
		GENPYBIND(getter_for(synin_debug_inhibitory))
		bool get_synin_debug_inhibitory() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(synin_debug_inhibitory))
		void set_synin_debug_inhibitory(bool value) SYMBOL_VISIBLE;

		/** Connect synaptic input excitatory debug line to mux. */
		GENPYBIND(getter_for(synin_debug_excitatory))
		bool get_synin_debug_excitatory() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(synin_debug_excitatory))
		void set_synin_debug_excitatory(bool value) SYMBOL_VISIBLE;

		/** Connect CADC causal debug line to mux. */
		GENPYBIND(getter_for(cadc_debug_causal))
		bool get_cadc_debug_causal() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(cadc_debug_causal))
		void set_cadc_debug_causal(bool value) SYMBOL_VISIBLE;

		/** Connect CADC acausal debug line to mux. */
		GENPYBIND(getter_for(cadc_debug_acausal))
		bool get_cadc_debug_acausal() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(cadc_debug_acausal))
		void set_cadc_debug_acausal(bool value) SYMBOL_VISIBLE;

		/** Connect synapse driver debug voltage readout to mux. */
		GENPYBIND(getter_for(synapse_driver_debug), return_value_policy(reference))
		hemisphere_type const& get_synapse_driver_debug() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(synapse_driver_debug), return_value_policy(reference))
		void set_synapse_driver_debug(hemisphere_type const& value) SYMBOL_VISIBLE;

		/** Connect analog neuron readout for odd neurons (1, 3, ..., 255) to mux. */
		GENPYBIND(getter_for(neuron_odd), return_value_policy(reference))
		hemisphere_type const& get_neuron_odd() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(neuron_odd), return_value_policy(reference))
		void set_neuron_odd(hemisphere_type const& value) SYMBOL_VISIBLE;

		/** Connect analog neuron readout for even neurons (0, 2, ..., 254) to mux. */
		GENPYBIND(getter_for(neuron_even), return_value_policy(reference))
		hemisphere_type const& get_neuron_even() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(neuron_even), return_value_policy(reference))
		void set_neuron_even(hemisphere_type const& value) SYMBOL_VISIBLE;

		/** Enable buffer after the mux. */
		GENPYBIND(getter_for(enable_buffer))
		bool get_enable_buffer() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(enable_buffer))
		void set_enable_buffer(bool value) SYMBOL_VISIBLE;

		bool operator==(ReadoutBufferConfig const& other) const SYMBOL_VISIBLE;
		bool operator!=(ReadoutBufferConfig const& other) const SYMBOL_VISIBLE;

	private:
		friend class ReadoutBufferConfigBlock;
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& ar) SYMBOL_VISIBLE;

		bool m_differential_signal;
		bool m_differential_reference;
		bool m_current_dac;
		bool m_synin_debug_inhibitory;
		bool m_synin_debug_excitatory;
		bool m_cadc_debug_causal;
		bool m_cadc_debug_acausal;
		hemisphere_type m_synapse_driver_debug;
		hemisphere_type m_neuron_odd;
		hemisphere_type m_neuron_even;
		bool m_enable_buffer;
	};

	/** Default constructor with both buffers/muxes disabled. */
	ReadoutBufferConfigBlock() SYMBOL_VISIBLE;

	ReadoutBufferConfig get_buffer(
	    halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock const& buffer) const
	    SYMBOL_VISIBLE;
	void set_buffer(
	    halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock const& buffer,
	    ReadoutBufferConfig const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;

	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block) const
	    GENPYBIND(hidden);

	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);

	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ReadoutBufferConfigBlock const& config)
	    SYMBOL_VISIBLE;

	bool operator==(ReadoutBufferConfigBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(ReadoutBufferConfigBlock const& other) const SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar);

	halco::common::typed_array<
	    ReadoutBufferConfig,
	    halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock>
	    m_buffers;
};

namespace detail {

template <>
struct BackendContainerTrait<ReadoutMuxConfig>
    : public BackendContainerBase<
          ReadoutMuxConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<ReadoutBufferConfigBlock>
    : public BackendContainerBase<
          ReadoutBufferConfigBlock,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace haldls::vx
