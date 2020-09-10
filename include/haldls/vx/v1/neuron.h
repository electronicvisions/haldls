#pragma once
#include "halco/hicann-dls/vx/v1/coordinates.h"
#include "halco/hicann-dls/vx/v1/neuron.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/neuron.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls::vx {
NEURON_EXTERN_TEMPLATE(halco::hicann_dls::vx::v1::Coordinates)
} // namespace haldls::vx

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using CommonNeuronBackendConfig GENPYBIND(visible) = haldls::vx::CommonNeuronBackendConfig;
using NeuronBackendConfig GENPYBIND(opaque) =
    haldls::vx::NeuronBackendConfig<halco::hicann_dls::vx::v1::Coordinates>;
using NeuronReset GENPYBIND(visible) = haldls::vx::NeuronReset;
using BlockPostPulse GENPYBIND(visible) = haldls::vx::BlockPostPulse;
using SpikeCounterRead GENPYBIND(visible) = haldls::vx::SpikeCounterRead;
using SpikeCounterReset GENPYBIND(visible) = haldls::vx::SpikeCounterReset;
using NeuronBackendSRAMTimingConfig GENPYBIND(visible) = haldls::vx::NeuronBackendSRAMTimingConfig;
using NeuronSRAMTimingConfig GENPYBIND(visible) = haldls::vx::NeuronSRAMTimingConfig;

/*
 * Container for configuration of (digital) neuron parameters.
 */
class GENPYBIND(visible) NeuronConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::v1::NeuronConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};

	/** Source of readout output. */
	enum class ReadoutSource : uint_fast8_t
	{
		membrane = 0b00,
		exc_synin = 0b01,
		inh_synin = 0b10,
		adaptation = 0b11
	};

	/** Strength of exponential term. */
	struct GENPYBIND(inline_base("*")) ExponentialTermStrength
	    : public halco::common::detail::RantWrapper<ExponentialTermStrength, uint_fast8_t, 7, 0>
	{
		constexpr explicit ExponentialTermStrength(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Size of membrane capacitor. Maximal size is ~2.3pF. */
	struct GENPYBIND(inline_base("*")) MembraneCapacitorSize
	    : public halco::common::detail::RantWrapper<MembraneCapacitorSize, uint_fast8_t, 63, 0>
	{
		constexpr explicit MembraneCapacitorSize(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor */
	NeuronConfig() SYMBOL_VISIBLE;

	// accessors

	// divide inter-compartment conductance bias by 4
	GENPYBIND(getter_for(enable_divide_multicomp_conductance_bias))
	bool get_enable_divide_multicomp_conductance_bias() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_divide_multicomp_conductance_bias))
	void set_enable_divide_multicomp_conductance_bias(bool value) SYMBOL_VISIBLE;

	// multiply inter-compartment conductance bias by 4
	GENPYBIND(getter_for(enable_multiply_multicomp_conductance_bias))
	bool get_enable_multiply_multicomp_conductance_bias() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_multiply_multicomp_conductance_bias))
	void set_enable_multiply_multicomp_conductance_bias(bool value) SYMBOL_VISIBLE;

	// connect local membrane to soma
	GENPYBIND(getter_for(connect_soma))
	bool get_connect_soma() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_soma))
	void set_connect_soma(bool value) SYMBOL_VISIBLE;

	// connect local membrane to membrane on right
	GENPYBIND(getter_for(connect_membrane_right))
	bool get_connect_membrane_right() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_membrane_right))
	void set_connect_membrane_right(bool value) SYMBOL_VISIBLE;

	// enable inter-compartment conductance
	GENPYBIND(getter_for(enable_multicomp_conductance))
	bool get_enable_multicomp_conductance() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_multicomp_conductance))
	void set_enable_multicomp_conductance(bool value) SYMBOL_VISIBLE;

	// connect local membrane to bottom membrane
	GENPYBIND(getter_for(connect_bottom))
	bool get_connect_bottom() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_bottom))
	void set_connect_bottom(bool value) SYMBOL_VISIBLE;

	// connect soma to soma on the right
	GENPYBIND(getter_for(connect_soma_right))
	bool get_connect_soma_right() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_soma_right))
	void set_connect_soma_right(bool value) SYMBOL_VISIBLE;

	// enable fire output of neuron (also gates bypass circuits)
	GENPYBIND(getter_for(enable_fire))
	bool get_enable_fire() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_fire))
	void set_enable_fire(bool value) SYMBOL_VISIBLE;

	// enable threshold comparator
	GENPYBIND(getter_for(enable_threshold_comparator))
	bool get_enable_threshold_comparator() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_threshold_comparator))
	void set_enable_threshold_comparator(bool value) SYMBOL_VISIBLE;

	// enable exc. synaptic input
	GENPYBIND(getter_for(enable_synaptic_input_excitatory))
	bool get_enable_synaptic_input_excitatory() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_synaptic_input_excitatory))
	void set_enable_synaptic_input_excitatory(bool value) SYMBOL_VISIBLE;

	// enable inh. synaptic input
	GENPYBIND(getter_for(enable_synaptic_input_inhibitory))
	bool get_enable_synaptic_input_inhibitory() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_synaptic_input_inhibitory))
	void set_enable_synaptic_input_inhibitory(bool value) SYMBOL_VISIBLE;

	// enable exc. bypass circuit
	GENPYBIND(getter_for(enable_bypass_excitatory))
	bool get_enable_bypass_excitatory() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_bypass_excitatory))
	void set_enable_bypass_excitatory(bool value) SYMBOL_VISIBLE;

	// enable inh. bypass circuit
	GENPYBIND(getter_for(enable_bypass_inhibitory))
	bool get_enable_bypass_inhibitory() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_bypass_inhibitory))
	void set_enable_bypass_inhibitory(bool value) SYMBOL_VISIBLE;

	// enable membrane offset current (can also be used for stimulus/step current)
	GENPYBIND(getter_for(enable_membrane_offset))
	bool get_enable_membrane_offset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_membrane_offset))
	void set_enable_membrane_offset(bool value) SYMBOL_VISIBLE;

	// enable merging of membrane and adaptation capacitances
	GENPYBIND(getter_for(enable_capacitor_merge))
	bool get_enable_capacitor_merge() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_capacitor_merge))
	void set_enable_capacitor_merge(bool value) SYMBOL_VISIBLE;

	// configure membrane size
	GENPYBIND(getter_for(membrane_capacitor_size))
	MembraneCapacitorSize get_membrane_capacitor_size() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(membrane_capacitor_size))
	void set_membrane_capacitor_size(MembraneCapacitorSize value) SYMBOL_VISIBLE;

	// flip the sign of a (sub-threshold adaptation)
	GENPYBIND(getter_for(invert_adaptation_a))
	bool get_invert_adaptation_a() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(invert_adaptation_a))
	void set_invert_adaptation_a(bool value) SYMBOL_VISIBLE;

	// flip the sign of b (spike-triggered adaptation)
	GENPYBIND(getter_for(invert_adaptation_b))
	bool get_invert_adaptation_b() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(invert_adaptation_b))
	void set_invert_adaptation_b(bool value) SYMBOL_VISIBLE;

	// enable adaptation
	GENPYBIND(getter_for(enable_adaptation))
	bool get_enable_adaptation() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_adaptation))
	void set_enable_adaptation(bool value) SYMBOL_VISIBLE;

	// enable adaptation capacitance
	GENPYBIND(getter_for(enable_adaptation_capacitor))
	bool get_enable_adaptation_capacitor() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_adaptation_capacitor))
	void set_enable_adaptation_capacitor(bool value) SYMBOL_VISIBLE;

	// strength of exponential term
	GENPYBIND(getter_for(exponential_term_strength))
	ExponentialTermStrength get_exponential_term_strength() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(exponential_term_strength))
	void set_exponential_term_strength(ExponentialTermStrength value) SYMBOL_VISIBLE;

	// enable exponential term
	GENPYBIND(getter_for(enable_exponential))
	bool get_enable_exponential() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_exponential))
	void set_enable_exponential(bool value) SYMBOL_VISIBLE;

	// enable readout of adaptation voltage (user must also configure readout_select!)
	GENPYBIND(getter_for(enable_adaptation_readout))
	bool get_enable_adaptation_readout() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_adaptation_readout))
	void set_enable_adaptation_readout(bool value) SYMBOL_VISIBLE;

	// enable direct, unbuffered access to membrane
	GENPYBIND(getter_for(enable_unbuffered_access))
	bool get_enable_unbuffered_access() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_unbuffered_access))
	void set_enable_unbuffered_access(bool value) SYMBOL_VISIBLE;

	// enable readout amplifier
	GENPYBIND(getter_for(enable_readout_amplifier))
	bool get_enable_readout_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_readout_amplifier))
	void set_enable_readout_amplifier(bool value) SYMBOL_VISIBLE;

	// select readout source
	GENPYBIND(getter_for(readout_source))
	ReadoutSource get_readout_source() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(readout_source))
	void set_readout_source(ReadoutSource value) SYMBOL_VISIBLE;

	// enable readout
	GENPYBIND(getter_for(enable_readout))
	bool get_enable_readout() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_readout))
	void set_enable_readout(bool value) SYMBOL_VISIBLE;

	// enable source degeneration of leak/reset OTA in reset mode
	GENPYBIND(getter_for(enable_reset_degeneration))
	bool get_enable_reset_degeneration() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_reset_degeneration))
	void set_enable_reset_degeneration(bool value) SYMBOL_VISIBLE;

	// enable division (8x) of conductance in reset mode
	GENPYBIND(getter_for(enable_reset_division))
	bool get_enable_reset_division() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_reset_division))
	void set_enable_reset_division(bool value) SYMBOL_VISIBLE;

	// enable multiplication (8x) of conductance in reset mode
	GENPYBIND(getter_for(enable_reset_multiplication))
	bool get_enable_reset_multiplication() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_reset_multiplication))
	void set_enable_reset_multiplication(bool value) SYMBOL_VISIBLE;

	// enable source degeneration of leak/reset OTA in leak mode
	GENPYBIND(getter_for(enable_leak_degeneration))
	bool get_enable_leak_degeneration() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_leak_degeneration))
	void set_enable_leak_degeneration(bool value) SYMBOL_VISIBLE;

	// enable division (8x) of conductance in leak mode
	GENPYBIND(getter_for(enable_leak_division))
	bool get_enable_leak_division() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_leak_division))
	void set_enable_leak_division(bool value) SYMBOL_VISIBLE;

	// enable multiplication (8x) of conductance in leak mode
	GENPYBIND(getter_for(enable_leak_multiplication))
	bool get_enable_leak_multiplication() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_leak_multiplication))
	void set_enable_leak_multiplication(bool value) SYMBOL_VISIBLE;

	bool operator==(NeuronConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 6;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, NeuronConfig const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_en_comp_cond_div;
	bool m_en_comp_cond_mul;
	bool m_connect_soma;
	bool m_connect_membrane_right;
	bool m_en_comp_cond;
	bool m_connect_bottom;
	bool m_connect_somata;
	bool m_en_fire;
	bool m_en_thresh_comp;
	bool m_en_synin_inh;
	bool m_en_synin_exc;
	bool m_en_byp_inh;
	bool m_en_byp_exc;
	bool m_en_mem_off;
	bool m_en_cap_merge;
	MembraneCapacitorSize m_mem_cap_size;
	bool m_invert_adapt_a;
	bool m_invert_adapt_b;
	bool m_en_adapt;
	bool m_en_adapt_cap;
	ExponentialTermStrength m_exp_weight;
	bool m_en_exp;
	bool m_en_read_vw;
	bool m_en_unbuf_access;
	bool m_en_readout_amp;
	ReadoutSource m_readout_select;
	bool m_en_readout;
	bool m_en_reset_deg;
	bool m_en_reset_div;
	bool m_en_reset_mul;
	bool m_en_leak_deg;
	bool m_en_leak_div;
	bool m_en_leak_mul;
};

/**
 * Container to trigger reset of a quad of neurons at once.
 * Currently, also the correlation in the corresponding synapse quad in row zero
 * is reset. This behaviour will be fixed for HX-v2 (issue 3346).
 */
class GENPYBIND(visible) NeuronResetQuad
{
public:
	typedef halco::hicann_dls::vx::v1::NeuronResetQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor */
	NeuronResetQuad() SYMBOL_VISIBLE;

	bool operator==(NeuronResetQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronResetQuad const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& neuron) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& neuron) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, NeuronResetQuad const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

} // namespace haldls::vx::v1

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<v1::NeuronConfig>
    : public BackendContainerBase<
          v1::NeuronConfig,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v1::NeuronResetQuad>
    : public BackendContainerBase<
          v1::NeuronResetQuad,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace haldls::vx::detail
