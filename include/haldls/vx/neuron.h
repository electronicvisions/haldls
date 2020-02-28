#pragma once

#include <algorithm>
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

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {


/**
 * Read/write access to common neuron parameters
 */
class GENPYBIND(visible) CommonNeuronBackendConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::CommonNeuronBackendConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * The ClockScale determines the range of the clock and at its precision
	 * It's an exponential power-of-two clock scaling factor: For a given reference clock of
	 * f_clk = 250MHz, the resulting frequency is f_clk / (2 ^ (clock_scale + 1)). This enables
	 * clock frequencies from 125MHz (clock_scale = 0) to ~4kHz (clock_scale = 15)
	 * See documentation and implementation of RefractoryTime for more background information.
	 */
	struct GENPYBIND(inline_base("*")) ClockScale
	    : public halco::common::detail::RantWrapper<ClockScale, uint_fast8_t, 15, 0>
	{
		constexpr explicit ClockScale(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Duration of the pulse triggering global post pulses for all neurons connected to that backend block.
	 */
	struct GENPYBIND(inline_base("*")) WaitGlobalPostPulse
	    : public halco::common::detail::RantWrapper<WaitGlobalPostPulse, uint_fast16_t, 255, 0>
	{
		constexpr explicit WaitGlobalPostPulse(uintmax_t const val = 128)
		    GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Duration of the pulse triggering spike counter resets.
	 */
	struct GENPYBIND(inline_base("*")) WaitSpikeCounterReset
	    : public halco::common::detail::RantWrapper<WaitSpikeCounterReset, uint_fast16_t, 255, 0>
	{
		constexpr explicit WaitSpikeCounterReset(uintmax_t const val = 4)
		    GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Wait time for letting the neuron backend circuits drive the counter content to the controller logic.
	 */
	struct GENPYBIND(inline_base("*")) WaitSpikeCounterRead
	    : public halco::common::detail::RantWrapper<WaitSpikeCounterRead, uint_fast16_t, 255, 0>
	{
		constexpr explicit WaitSpikeCounterRead(uintmax_t const val = 112)
		    GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Duration of pulse triggering the artificial neuron spikes.
	 */
	struct GENPYBIND(inline_base("*")) WaitFireNeuron
	    : public halco::common::detail::RantWrapper<WaitFireNeuron, uint_fast16_t, 255, 0>
	{
		constexpr explicit WaitFireNeuron(uintmax_t const val = 4) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	CommonNeuronBackendConfig() SYMBOL_VISIBLE;

	// accessors
	GENPYBIND(getter_for(enable_event_registers))
	bool get_enable_event_registers() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_event_registers))
	void set_enable_event_registers(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(force_reset))
	bool get_force_reset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(force_reset))
	void set_force_reset(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_clocks))
	bool get_enable_clocks() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_clocks))
	void set_enable_clocks(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(clock_scale_slow))
	ClockScale get_clock_scale_slow() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(clock_scale_slow))
	void set_clock_scale_slow(ClockScale const val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(clock_scale_fast))
	ClockScale get_clock_scale_fast() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(clock_scale_fast))
	void set_clock_scale_fast(ClockScale const val) SYMBOL_VISIBLE;

	bool get_sample_positive_edge(
	    halco::hicann_dls::vx::EventOutputOnNeuronBackendBlock coord) const SYMBOL_VISIBLE;
	void set_sample_positive_edge(
	    halco::hicann_dls::vx::EventOutputOnNeuronBackendBlock coord, bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(clock_scale_adaptation_pulse))
	ClockScale get_clock_scale_adaptation_pulse() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(clock_scale_adaptation_pulse))
	void set_clock_scale_adaptation_pulse(ClockScale const val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(clock_scale_post_pulse))
	ClockScale get_clock_scale_post_pulse() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(clock_scale_post_pulse))
	void set_clock_scale_post_pulse(ClockScale const val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(wait_global_post_pulse))
	WaitGlobalPostPulse get_wait_global_post_pulse() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wait_global_post_pulse))
	void set_wait_global_post_pulse(WaitGlobalPostPulse const val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(wait_spike_counter_reset))
	WaitSpikeCounterReset get_wait_spike_counter_reset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wait_spike_counter_reset))
	void set_wait_spike_counter_reset(WaitSpikeCounterReset const val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(wait_spike_counter_read))
	WaitSpikeCounterRead get_wait_spike_counter_read() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wait_spike_counter_read))
	void set_wait_spike_counter_read(WaitSpikeCounterRead const val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(wait_fire_neuron))
	WaitFireNeuron get_wait_fire_neuron() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wait_fire_neuron))
	void set_wait_fire_neuron(WaitFireNeuron const val) SYMBOL_VISIBLE;

	bool operator==(CommonNeuronBackendConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonNeuronBackendConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CommonNeuronBackendConfig const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	bool m_en_event_regs;
	bool m_force_reset;
	bool m_en_clocks;
	ClockScale m_clock_scale_slow;
	ClockScale m_clock_scale_fast;
	halco::common::typed_array<bool, halco::hicann_dls::vx::EventOutputOnNeuronBackendBlock>
	    m_sample_pos_edge;
	ClockScale m_clock_scale_adapt_pulse;
	ClockScale m_clock_scale_post_pulse;
	WaitGlobalPostPulse m_wait_global_post_pulse;
	WaitSpikeCounterReset m_wait_spike_counter_reset;
	WaitSpikeCounterRead m_wait_spike_counter_read;
	WaitFireNeuron m_wait_fire_neuron;
};

/**
 * Read/write access to the NeuronBackend container.
 * All relevant settings of the NeuronBackend can be accessed and set via the NeuronBackendConfig.
 * The choice of the parameters (e.g. RefractoryTime) depends on the use case of the user and the
 * targeted biological model/experiment. Implementation examples are yet to be written and will
 * then be found in the CI test environment.
 */
class GENPYBIND(visible) NeuronBackendConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::NeuronBackendConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Address of the spikes sent out by a neuron.
	 */
	struct GENPYBIND(inline_base("*")) AddressOut
	    : public halco::common::detail::RantWrapper<AddressOut, uint_fast16_t, 255, 0>
	{
		constexpr explicit AddressOut(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * ResetHoldoff period: The time delta between the reset and the refractory period.
	 * The reset-holdoff time is configured by comparison with the four LSB of the refractory-
	 * counter: The mechanism stops the reset as soon as the bits programmed in the reset-holdoff
	 * mask match the LSB of the refractory-counter and all MSB of the refractory-counter are set.
	 * Until the end of the programmed refractory time, the circuit does still reject new threshold
	 * crossings. This combination of still rejecting spikes, but stopping the reset defines the
	 * reset-holdoff time. For the user it is important to note, that the reset-holdoff time is
	 * subtracted from the refractory period, to turn it off, one can use the maximum code (0xF)
	 * for the holdoff time, resulting in the reset-holdoff time dropping to zero.
	 *
	 * Example: The reset is turned off before the end of the refractory period. As the neuron is
	 * still refractory, no new spike is registered, although the fire signal is still high. Only
	 * after the end of the refractory period the circuits accepts new fire signals. The difference
	 * between refractory period and reset duration defines the reset-holdoff period.
	 */
	struct GENPYBIND(inline_base("*")) ResetHoldoff
	    : public halco::common::detail::RantWrapper<ResetHoldoff, uint_fast8_t, 15, 0>
	{
		constexpr explicit ResetHoldoff(uintmax_t const val = 15) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * The refractory time can be configured to be used for different applications (short and long)
	 * Refactory Periods usually range from 0 to 5 ms. Accounting for the speedup they correspond to
	 * 5 µs in hardware time. Some special applications like NMDA require significantly longer
	 * times.
	 *
	 * tau_ref = RefractoryTime * 1/f_clock
	 *
	 * Use case 1 (standard): tau_min = 0.1µs => f_clock = 10MHz
	 * tau_max = 12.7µs (12.7ms bio time)
	 * Use case 2 (NMDA): f_clock = 4kHz -> tau_max = 127ms (127s bio time)
	 * Use case 3 (short): f_clock = 250MHz -> tau_min = 4ns (4µs in bio time)
	 * @note The reset conductance is limited and thus will not pull down the membrane in 4 ns!
	 *       The fastest analog "reset time constant" is approx. 0.5 μs.
	 */
	struct GENPYBIND(inline_base("*")) RefractoryTime
	    : public halco::common::detail::RantWrapper<RefractoryTime, uint_fast16_t, 255, 0>
	{
		constexpr explicit RefractoryTime(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * There are two independent but equivalent clocks available.
	 * Both clocks can be configured as desired for the use case. See e.g. RefractoryTime
	 */
	struct GENPYBIND(inline_base("*")) InputClock
	    : public halco::common::detail::RantWrapper<InputClock, uint_fast8_t, 1, 0>
	{
		constexpr explicit InputClock(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	NeuronBackendConfig() SYMBOL_VISIBLE;

	// accessors
	GENPYBIND(getter_for(address_out))
	AddressOut get_address_out() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(address_out))
	void set_address_out(AddressOut addr) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(reset_holdoff))
	ResetHoldoff get_reset_holdoff() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(reset_holdoff))
	void set_reset_holdoff(ResetHoldoff val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(refractory_time))
	RefractoryTime get_refractory_time() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(refractory_time))
	void set_refractory_time(RefractoryTime val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(post_overwrite))
	bool get_post_overwrite() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(post_overwrite))
	void set_post_overwrite(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(select_input_clock))
	InputClock get_select_input_clock() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(select_input_clock))
	void set_select_input_clock(InputClock src) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_adaptation_pulse))
	bool get_enable_adaptation_pulse() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_adaptation_pulse))
	void set_enable_adaptation_pulse(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_bayesian_extension))
	bool get_enable_bayesian_extension() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_bayesian_extension))
	void set_enable_bayesian_extension(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_neuron_slave))
	bool get_enable_neuron_slave() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_neuron_slave))
	void set_enable_neuron_slave(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(connect_fire_bottom))
	bool get_connect_fire_bottom() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_fire_bottom))
	void set_connect_fire_bottom(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(connect_fire_from_right))
	bool get_connect_fire_from_right() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_fire_from_right))
	void set_connect_fire_from_right(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(connect_fire_to_right))
	bool get_connect_fire_to_right() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_fire_to_right))
	void set_connect_fire_to_right(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_spike_out))
	bool get_enable_spike_out() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_spike_out))
	void set_enable_spike_out(bool val) SYMBOL_VISIBLE;

	/** Enable the fire output of a neuron.
	 * If the threshold comparator creates a fire signal, all connected neurons
	 * receive a fire input. Even if no neurons are connected, this setting
	 * needs to be switched on in order to process the fire signal.
	 * If this setting is disabled, spikes will not trigger post pulses in the synapses.
	 */
	GENPYBIND(getter_for(enable_neuron_master))
	bool get_enable_neuron_master() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_neuron_master))
	void set_enable_neuron_master(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_bayesian_0))
	bool get_enable_bayesian_0() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_bayesian_0))
	void set_enable_bayesian_0(bool val) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_bayesian_1))
	bool get_enable_bayesian_1() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_bayesian_1))
	void set_enable_bayesian_1(bool val) SYMBOL_VISIBLE;

	bool operator==(NeuronBackendConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronBackendConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 4;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, NeuronBackendConfig const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar);

	AddressOut m_address_out;
	ResetHoldoff m_reset_holdoff;
	RefractoryTime m_refractory_time;
	bool m_post_overwrite;
	InputClock m_select_input_clock;
	bool m_en_adapt_pulse;
	bool m_en_baesian_extension;
	bool m_en_neuron_slave;
	bool m_connect_fire_bottom;
	bool m_connect_fire_from_right;
	bool m_connect_fire_to_right;
	bool m_en_spike_out;
	bool m_en_neuron_master;
	bool m_en_0_baesian;
	bool m_en_1_baesian;
};


/*
 * Container for configuration of (digital) neuron parameters.
 */
class GENPYBIND(visible) NeuronConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::NeuronConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

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
	void serialize(Archive& ar) SYMBOL_VISIBLE;

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

namespace detail {

template <>
struct BackendContainerTrait<CommonNeuronBackendConfig>
    : public BackendContainerBase<
          CommonNeuronBackendConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<NeuronBackendConfig>
    : public BackendContainerBase<
          NeuronBackendConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<NeuronConfig>
    : public BackendContainerBase<
          NeuronConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container to trigger reset of a single neuron.
 */
class GENPYBIND(visible) NeuronReset
{
public:
	typedef halco::hicann_dls::vx::NeuronResetOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor */
	NeuronReset() SYMBOL_VISIBLE;

	bool operator==(NeuronReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronReset const& other) const SYMBOL_VISIBLE;

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
	friend std::ostream& operator<<(std::ostream& os, NeuronReset const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;
};

namespace detail {

template <>
struct BackendContainerTrait<NeuronReset>
    : public BackendContainerBase<
          NeuronReset,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container to trigger reset of a quad of neurons at once.
 * Currently, also the correlation in the corresponding synapse quad in row zero
 * is reset. This behaviour will be fixed for HX-v2 (issue 3346).
 */
class GENPYBIND(visible) NeuronResetQuad
{
public:
	typedef halco::hicann_dls::vx::NeuronResetQuadOnDLS coordinate_type;
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
	void serialize(Archive& ar) SYMBOL_VISIBLE;
};

namespace detail {

template <>
struct BackendContainerTrait<NeuronResetQuad>
    : public BackendContainerBase<
          NeuronResetQuad,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container to send post pulses to one half of the neurons (left/right).
 * Enable post_overwrite in the neurons to forward the pulse into the synapse array.
 * The timing this pulse arrives may not be precise, it may vary between
 * neurons in the block.
 */
class GENPYBIND(visible) BlockPostPulse
{
public:
	typedef halco::hicann_dls::vx::BlockPostPulseOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor */
	BlockPostPulse() SYMBOL_VISIBLE;

	bool operator==(BlockPostPulse const& other) const SYMBOL_VISIBLE;
	bool operator!=(BlockPostPulse const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& block) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& block) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, BlockPostPulse const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;
};

namespace detail {

template <>
struct BackendContainerTrait<BlockPostPulse>
    : public BackendContainerBase<
          BlockPostPulse,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container to read the spike counter of a single neuron.
 */
class GENPYBIND(visible) SpikeCounterRead
{
public:
	typedef halco::hicann_dls::vx::SpikeCounterReadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Count of rate counter.
	 */
	struct GENPYBIND(inline_base("*")) Count
	    : public halco::common::detail::RantWrapper<Count, uint_fast16_t, 255, 0>
	{
		constexpr explicit Count(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor */
	SpikeCounterRead() SYMBOL_VISIBLE;

	/**
	 * Get count, i.e. number of spikes since last reset.
	 * @return Count
	 */
	GENPYBIND(getter_for(count))
	Count get_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(count))
	void set_count(Count const cnt) SYMBOL_VISIBLE;

	// Overflow bit
	GENPYBIND(getter_for(overflow))
	bool get_overflow() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(overflow))
	void set_overflow(bool const ovrflw) SYMBOL_VISIBLE;

	bool operator==(SpikeCounterRead const& other) const SYMBOL_VISIBLE;
	bool operator!=(SpikeCounterRead const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
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
	friend std::ostream& operator<<(std::ostream& os, SpikeCounterRead const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	Count m_count;
	bool m_overflow;
};

namespace detail {

template <>
struct BackendContainerTrait<SpikeCounterRead>
    : public BackendContainerBase<
          SpikeCounterRead,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container to reset the spike counter of a single neuron.
 */
class GENPYBIND(visible) SpikeCounterReset
{
public:
	typedef halco::hicann_dls::vx::SpikeCounterResetOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor */
	SpikeCounterReset() SYMBOL_VISIBLE;

	bool operator==(SpikeCounterReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(SpikeCounterReset const& other) const SYMBOL_VISIBLE;

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
	friend std::ostream& operator<<(std::ostream& os, SpikeCounterReset const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;
};

namespace detail {

template <>
struct BackendContainerTrait<SpikeCounterReset>
    : public BackendContainerBase<
          SpikeCounterReset,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
