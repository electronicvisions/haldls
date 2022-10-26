#pragma once
#include <iosfwd>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/capmem.h"
#include "halco/hicann-dls/vx/chip.h"
#include "halco/hicann-dls/vx/readout.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace fisch::vx {

class OmnibusChipOverJTAG;
class Omnibus;

} // namespace fisch::vx

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

/**
 * A configuration container for the top-level readout mux, selecting the connections between
 * an analog readout pad and different components on the chip.
 * There are two equal instances of this mux on the chip, one for each of the two pads.
 */
class GENPYBIND(visible) PadMultiplexerConfig
{
public:
	typedef halco::hicann_dls::vx::PadMultiplexerConfigOnDLS coordinate_type;

	typedef std::true_type is_leaf_node;

	/** Default constructor with all inputs disabled. */
	PadMultiplexerConfig() SYMBOL_VISIBLE;

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::CapMemBlockOnDLS>
	    capmem_quadrant_type GENPYBIND(opaque(false));
	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::HemisphereOnDLS> hemisphere_type
	    GENPYBIND(opaque(false));
	typedef halco::common::
	    typed_array<bool, halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection>
	        buffer_type GENPYBIND(opaque(false));

	/** Select CADC v_ramp outputs per quadrant. */
	GENPYBIND(getter_for(cadc_v_ramp_mux), return_value_policy(reference_internal))
	capmem_quadrant_type const& get_cadc_v_ramp_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cadc_v_ramp_mux))
	void set_cadc_v_ramp_mux(capmem_quadrant_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of cadc_v_ramp_mux to pad. */
	GENPYBIND(getter_for(cadc_v_ramp_mux_to_pad))
	bool get_cadc_v_ramp_mux_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cadc_v_ramp_mux_to_pad))
	void set_cadc_v_ramp_mux_to_pad(bool value) SYMBOL_VISIBLE;

	/** Select CapMem i_out outputs per quadrant. */
	GENPYBIND(getter_for(capmem_i_out_mux), return_value_policy(reference_internal))
	capmem_quadrant_type const& get_capmem_i_out_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_i_out_mux))
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
	GENPYBIND(getter_for(capmem_v_out_mux), return_value_policy(reference_internal))
	capmem_quadrant_type const& get_capmem_v_out_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_out_mux))
	void set_capmem_v_out_mux(capmem_quadrant_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of capmem_v_out_mux to intermediate mux.
	 * To reach the pad, the connection between the CapMem intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(capmem_v_out_mux_to_capmem_intermediate_mux))
	bool get_capmem_v_out_mux_to_capmem_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_out_mux_to_capmem_intermediate_mux))
	void set_capmem_v_out_mux_to_capmem_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Select CapMem v_ref outputs per quadrant. */
	GENPYBIND(getter_for(capmem_v_ref_mux), return_value_policy(reference_internal))
	capmem_quadrant_type const& get_capmem_v_ref_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_ref_mux))
	void set_capmem_v_ref_mux(capmem_quadrant_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of capmem_v_ref_mux to intermediate mux.
	 * To reach the pad, the connection between the CapMem intermediate mux and the pad has to be
	 * enabled as well. */
	GENPYBIND(getter_for(capmem_v_ref_mux_to_capmem_intermediate_mux))
	bool get_capmem_v_ref_mux_to_capmem_intermediate_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_v_ref_mux_to_capmem_intermediate_mux))
	void set_capmem_v_ref_mux_to_capmem_intermediate_mux(bool value) SYMBOL_VISIBLE;

	/** Select neuron unbuffered membrane access i_stim per hemisphere. */
	GENPYBIND(getter_for(neuron_i_stim_mux), return_value_policy(reference_internal))
	hemisphere_type const& get_neuron_i_stim_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(neuron_i_stim_mux))
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
	 * The connections to the voltage readout muxes/buffers are handled in the
	 * ReadoutSourceSelection container. */
	GENPYBIND(getter_for(buffer_to_pad), return_value_policy(reference_internal))
	buffer_type const& get_buffer_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(buffer_to_pad))
	void set_buffer_to_pad(buffer_type const& value) SYMBOL_VISIBLE;

	/** Enable connection of debug lines to pad.
	 * Enabling this connection on pad 0 connects the `debug_plus` line to that pad,
	 * enabling this connection on pad 1 connects the `debug_minus` line to that pad.
	 */
	GENPYBIND(getter_for(debug_to_pad))
	bool get_debug_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(debug_to_pad))
	void set_debug_to_pad(bool value) SYMBOL_VISIBLE;

	bool operator==(PadMultiplexerConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(PadMultiplexerConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PadMultiplexerConfig const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;

	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& words) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

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
	bool m_debug_to_pad;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PadMultiplexerConfig)

/**
 * Configuration container for the two mux and buffer blocks for voltage readout.
 * Allows selection of various signal sources on chip and drives the signals.
 * To reach the pads, the buffer_to_pad option in the PadMultiplexerConfig needs to be set.
 *
 * Some of the selectable voltages in this container can also be selected in the
 * PadMultiplexerConfig directly. Note that there the connection is direct, not buffered,
 * while here the signal is amplified. Do not enable both connections, as there
 * will be feedback otherwise!
 */
class GENPYBIND(visible) ReadoutSourceSelection
{
public:
	typedef halco::hicann_dls::vx::ReadoutSourceSelectionOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct SourceMultiplexer
	{
		/** Default constructor with all inputs disabled. */
		SourceMultiplexer() SYMBOL_VISIBLE;

		typedef halco::common::typed_array<bool, halco::hicann_dls::vx::HemisphereOnDLS>
		    hemisphere_type GENPYBIND(opaque(false));

		/** Connect `debug_plus` line to mux. */
		GENPYBIND(getter_for(debug_plus))
		bool get_debug_plus() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(debug_plus))
		void set_debug_plus(bool value) SYMBOL_VISIBLE;

		/** Connect `debug_minus` line to mux. */
		GENPYBIND(getter_for(debug_minus))
		bool get_debug_minus() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(debug_minus))
		void set_debug_minus(bool value) SYMBOL_VISIBLE;

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
		GENPYBIND(getter_for(synapse_driver_debug), return_value_policy(reference_internal))
		hemisphere_type const& get_synapse_driver_debug() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(synapse_driver_debug))
		void set_synapse_driver_debug(hemisphere_type const& value) SYMBOL_VISIBLE;

		/** Connect analog neuron readout for odd neurons (1, 3, ..., 255) to mux. */
		GENPYBIND(getter_for(neuron_odd), return_value_policy(reference_internal))
		hemisphere_type const& get_neuron_odd() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(neuron_odd))
		void set_neuron_odd(hemisphere_type const& value) SYMBOL_VISIBLE;

		/** Connect analog neuron readout for even neurons (0, 2, ..., 254) to mux. */
		GENPYBIND(getter_for(neuron_even), return_value_policy(reference_internal))
		hemisphere_type const& get_neuron_even() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(neuron_even))
		void set_neuron_even(hemisphere_type const& value) SYMBOL_VISIBLE;

		bool operator==(SourceMultiplexer const& other) const SYMBOL_VISIBLE;
		bool operator!=(SourceMultiplexer const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, SourceMultiplexer const& config)
		    SYMBOL_VISIBLE;

	private:
		friend class ReadoutSourceSelection;
		friend struct cereal::access;
		template <class Archive>
		void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

		bool m_debug_plus;
		bool m_debug_minus;
		bool m_current_dac;
		bool m_synin_debug_inhibitory;
		bool m_synin_debug_excitatory;
		bool m_cadc_debug_causal;
		bool m_cadc_debug_acausal;
		hemisphere_type m_synapse_driver_debug;
		hemisphere_type m_neuron_odd;
		hemisphere_type m_neuron_even;
	};

	/** Default constructor with both buffers and muxes disabled. */
	ReadoutSourceSelection() SYMBOL_VISIBLE;

	SourceMultiplexer get_buffer(
	    halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection const& buffer) const
	    SYMBOL_VISIBLE;
	void set_buffer(
	    halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection const& buffer,
	    SourceMultiplexer const& value) SYMBOL_VISIBLE;

	typedef halco::common::
	    typed_array<bool, halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection>
	        source_multiplexer_type GENPYBIND(opaque(false));
	/** Enable buffer after the muxes, connecting to the pads (c.f. PadMultiplexerConfig container).
	 * The active mux, which can be connected to the MADC, is reached even without enabling this
	 * buffer.*/
	GENPYBIND(getter_for(enable_buffer_to_pad), return_value_policy(reference_internal))
	source_multiplexer_type const& get_enable_buffer_to_pad() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_buffer_to_pad))
	void set_enable_buffer_to_pad(source_multiplexer_type const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;

	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block)
	    GENPYBIND(hidden);

	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);

	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ReadoutSourceSelection const& config)
	    SYMBOL_VISIBLE;

	bool operator==(ReadoutSourceSelection const& other) const SYMBOL_VISIBLE;
	bool operator!=(ReadoutSourceSelection const& other) const SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	halco::common::typed_array<
	    SourceMultiplexer,
	    halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection>
	    m_buffers;
	source_multiplexer_type m_enable_buffer_to_pad;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ReadoutSourceSelection::SourceMultiplexer)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ReadoutSourceSelection)

namespace detail {

template <>
struct BackendContainerTrait<PadMultiplexerConfig>
    : public BackendContainerBase<
          PadMultiplexerConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<ReadoutSourceSelection>
    : public BackendContainerBase<
          ReadoutSourceSelection,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace haldls::vx
