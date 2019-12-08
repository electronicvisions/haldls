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

namespace detail {

template <>
struct BackendContainerTrait<ReadoutMuxConfig>
    : public BackendContainerBase<
          ReadoutMuxConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace haldls::vx
