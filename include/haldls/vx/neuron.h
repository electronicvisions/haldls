#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/neuron.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/sram_controller.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

#ifndef __ppu__
#include "haldls/cerealization.h"
#include "hxcomm/vx/target.h"
#endif

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
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
	 * Duration of the pulse triggering global post pulses for all neurons connected to that backend
	 * block.
	 */
	struct GENPYBIND(inline_base("*")) WaitGlobalPostPulse
	    : public halco::common::detail::RantWrapper<WaitGlobalPostPulse, uint_fast16_t, 255, 0>
	{
		constexpr explicit WaitGlobalPostPulse(uintmax_t const val = 1)
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
	 * Wait time for letting the neuron backend circuits drive the counter content to the controller
	 * logic.
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
	    halco::hicann_dls::vx::NeuronEventOutputOnNeuronBackendBlock coord) const SYMBOL_VISIBLE;
	void set_sample_positive_edge(
	    halco::hicann_dls::vx::NeuronEventOutputOnNeuronBackendBlock coord,
	    bool val) SYMBOL_VISIBLE;

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
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_en_event_regs;
	bool m_force_reset;
	bool m_en_clocks;
	ClockScale m_clock_scale_slow;
	ClockScale m_clock_scale_fast;
	halco::common::typed_array<bool, halco::hicann_dls::vx::NeuronEventOutputOnNeuronBackendBlock>
	    m_sample_pos_edge;
	ClockScale m_clock_scale_adapt_pulse;
	ClockScale m_clock_scale_post_pulse;
	WaitGlobalPostPulse m_wait_global_post_pulse;
	WaitSpikeCounterReset m_wait_spike_counter_reset;
	WaitSpikeCounterRead m_wait_spike_counter_read;
	WaitFireNeuron m_wait_fire_neuron;
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CommonNeuronBackendConfig)
#endif

/**
 * Address of the spikes sent out by a neuron.
 */
struct GENPYBIND(inline_base("*")) NeuronBackendAddressOut
    : public halco::common::detail::RantWrapper<NeuronBackendAddressOut, uint_fast16_t, 255, 0>
{
	typedef halco::common::detail::RantWrapper<NeuronBackendAddressOut, uint_fast16_t, 255, 0>
	    rant_t;
	constexpr explicit NeuronBackendAddressOut(uintmax_t const val = 0)
	    GENPYBIND(implicit_conversion) :
	    rant_t(val)
	{}
};

template <typename Coordinates>
class NeuronBackendConfig;

/**
 * Read/write access to the NeuronBackend container.
 * All relevant settings of the NeuronBackend can be accessed and set via the NeuronBackendConfig.
 * The choice of the parameters (e.g. RefractoryTime) depends on the use case of the user and the
 * targeted biological model/experiment. Implementation examples are yet to be written and will
 * then be found in the CI test environment.
 */
template <typename Coordinates>
class NeuronBackendConfig : public DifferentialWriteTrait
{
public:
	typedef typename Coordinates::NeuronBackendConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

	typedef NeuronBackendAddressOut AddressOut GENPYBIND(visible);

	/**
	 * ResetHoldoff period: Adjusts the time delta between the reset and the refractory period.
	 * The reset is released before the end of the refractory time.
	 *
	 * The refractory-counter starts at a value of (0xFF - refractory_time) and is increased every
	 * clock cycle until its maximum value of 0xFF is reached; this time describes the refractory
	 * period.
	 * The reset is not active during the whole refractory period. The time in which the reset is
	 * not active is called 'reset-holdoff time'
	 * The time difference between reset and refractory period is configured by comparison of the
	 * five LSBs of the refractory counter to a mask: the reset stops as soon as the mask matches
	 * the LSB of the refractory counter and all MSB of the refractory counter are set.
	 * The mask is composed of a zero as LSB and the four bit reset-holdoff.
	 *
	 * Due to the fact that the lowest bit is always set to zero, there is always a period in
	 * which the reset is not active during the refractory period. This period is minimal if
	 * the reset-holdoff is maximal.
	 * In general the reset period is (0x1F - reset_holdoff << 1) clock cycles shorter than the
	 * refractory period.
	 *
	 * Example: The reset is turned off before the end of the refractory period. As the neuron is
	 * still refractory, no new spike is registered, although the fire signal is still high. Only
	 * after the end of the refractory period the circuits accepts new fire signals.
	 *
	 * Note: Setting a small refractory counter value in combination with a reset
	 * holdoff will result in glitchy behaviour, as the counter value triggering the
	 * end of the reset will never be reached if refractory_counter < (0x1F - reset_holdoff << 1).
	 */
	struct GENPYBIND(inline_base("*")) ResetHoldoff
	    : public halco::common::detail::RantWrapper<ResetHoldoff, uint_fast8_t, 15, 0>
	{
		typedef halco::common::detail::RantWrapper<ResetHoldoff, uint_fast8_t, 15, 0> rant_t;
		constexpr explicit ResetHoldoff() : rant_t(15) {}
		constexpr explicit ResetHoldoff(uintmax_t const val) GENPYBIND(implicit_conversion) :
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
	 * @note The reset is released at least one clock cycle before the end of the refractory period.
	 *       To avoid unexpected behavior the refractory period may not be too small, compare
	 *       `ResetHoldoff`.
	 */
	struct GENPYBIND(inline_base("*")) RefractoryTime
	    : public halco::common::detail::RantWrapper<RefractoryTime, uint_fast16_t, 255, 0>
	{
		typedef halco::common::detail::RantWrapper<RefractoryTime, uint_fast16_t, 255, 0> rant_t;
		constexpr explicit RefractoryTime() : rant_t(0) {}
		constexpr explicit RefractoryTime(uintmax_t const val) GENPYBIND(implicit_conversion) :
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
		typedef halco::common::detail::RantWrapper<InputClock, uint_fast8_t, 1, 0> rant_t;
		constexpr explicit InputClock() : rant_t(0) {}
		constexpr explicit InputClock(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	NeuronBackendConfig();

	// accessors
	GENPYBIND(getter_for(address_out))
	AddressOut get_address_out() const;
	GENPYBIND(setter_for(address_out))
	void set_address_out(AddressOut addr);

	GENPYBIND(getter_for(reset_holdoff))
	ResetHoldoff get_reset_holdoff() const;
	GENPYBIND(setter_for(reset_holdoff))
	void set_reset_holdoff(ResetHoldoff val);

	GENPYBIND(getter_for(refractory_time))
	RefractoryTime get_refractory_time() const;
	GENPYBIND(setter_for(refractory_time))
	void set_refractory_time(RefractoryTime val);

	GENPYBIND(getter_for(post_overwrite))
	bool get_post_overwrite() const;
	GENPYBIND(setter_for(post_overwrite))
	void set_post_overwrite(bool val);

	GENPYBIND(getter_for(select_input_clock))
	InputClock get_select_input_clock() const;
	GENPYBIND(setter_for(select_input_clock))
	void set_select_input_clock(InputClock src);

	GENPYBIND(getter_for(enable_adaptation_pulse))
	bool get_enable_adaptation_pulse() const;
	GENPYBIND(setter_for(enable_adaptation_pulse))
	void set_enable_adaptation_pulse(bool val);

	GENPYBIND(getter_for(enable_bayesian_extension))
	bool get_enable_bayesian_extension() const;
	GENPYBIND(setter_for(enable_bayesian_extension))
	void set_enable_bayesian_extension(bool val);

	GENPYBIND(getter_for(enable_neuron_slave))
	bool get_enable_neuron_slave() const;
	GENPYBIND(setter_for(enable_neuron_slave))
	void set_enable_neuron_slave(bool val);

	GENPYBIND(getter_for(connect_fire_bottom))
	bool get_connect_fire_bottom() const;
	GENPYBIND(setter_for(connect_fire_bottom))
	void set_connect_fire_bottom(bool val);

	GENPYBIND(getter_for(connect_fire_from_right))
	bool get_connect_fire_from_right() const;
	GENPYBIND(setter_for(connect_fire_from_right))
	void set_connect_fire_from_right(bool val);

	GENPYBIND(getter_for(connect_fire_to_right))
	bool get_connect_fire_to_right() const;
	GENPYBIND(setter_for(connect_fire_to_right))
	void set_connect_fire_to_right(bool val);

	GENPYBIND(getter_for(enable_spike_out))
	bool get_enable_spike_out() const;
	GENPYBIND(setter_for(enable_spike_out))
	void set_enable_spike_out(bool val);

	/** Enable the fire output of a neuron.
	 * If the threshold comparator creates a fire signal, all connected neurons
	 * receive a fire input. Even if no neurons are connected, this setting
	 * needs to be switched on in order to process the fire signal.
	 * If this setting is disabled, spikes will not trigger post pulses in the synapses.
	 */
	GENPYBIND(getter_for(enable_neuron_master))
	bool get_enable_neuron_master() const;
	GENPYBIND(setter_for(enable_neuron_master))
	void set_enable_neuron_master(bool val);

	GENPYBIND(getter_for(enable_bayesian_0))
	bool get_enable_bayesian_0() const;
	GENPYBIND(setter_for(enable_bayesian_0))
	void set_enable_bayesian_0(bool val);

	GENPYBIND(getter_for(enable_bayesian_1))
	bool get_enable_bayesian_1() const;
	GENPYBIND(setter_for(enable_bayesian_1))
	void set_enable_bayesian_1(bool val);

	bool operator==(NeuronBackendConfig const& other) const;
	bool operator!=(NeuronBackendConfig const& other) const;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 4;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(
	    std::ostream& os, NeuronBackendConfig<Coordinates> const& config)
	{
		std::stringstream ss;
		ss << "NeuronBackendConfig(\n" << std::boolalpha;
		// clang-format off
		ss << "\taddress_out:               \t" << std::to_string(config.m_address_out) << "\n"
		   << "\treset_holdoff_config:      \t" << std::to_string(config.m_reset_holdoff) << "\n"
		   << "\trefractory_time:           \t" << std::to_string(config.m_refractory_time) << "\n"
		   << "\tpost_overwrite:            \t" << config.m_post_overwrite << "\n"
		   << "\tselect_input_clock:        \t" << config.m_select_input_clock << "\n"
		   << "\tenable_adaptation_pulse:   \t" << config.m_en_adapt_pulse << "\n"
		   << "\tenable_bayesian_extension: \t" << config.m_en_baesian_extension << "\n"
		   << "\tenable_neuron_slave:       \t" << config.m_en_neuron_slave << "\n"
		   << "\tconnect_fire_bottom:       \t" << config.m_connect_fire_bottom << "\n"
		   << "\tconnect_fire_from_right:   \t" << config.m_connect_fire_from_right << "\n"
		   << "\tconnect_fire_to_right:     \t" << config.m_connect_fire_to_right << "\n"
		   << "\tenable_spike_out:          \t" << config.m_en_spike_out << "\n"
		   << "\tenable_neuron_master:      \t" << config.m_en_neuron_master << "\n"
		   << "\tenable_0_bayesian:         \t" << config.m_en_0_baesian << "\n"
		   << "\tenable_1_bayesian:         \t" << config.m_en_1_baesian << "\n)";
		// clang-format on
		return (os << ss.str());
	}

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

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

namespace detail {

template <>
struct BackendContainerTrait<CommonNeuronBackendConfig>
    : public BackendContainerBase<
          CommonNeuronBackendConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <typename Coordinates>
struct BackendContainerTrait<NeuronBackendConfig<Coordinates>>
    : public BackendContainerBase<
          NeuronBackendConfig<Coordinates>,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
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
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(NeuronReset)
#endif

namespace detail {

template <>
struct BackendContainerTrait<NeuronReset>
    : public BackendContainerBase<
          NeuronReset,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
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
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(BlockPostPulse)
#endif

namespace detail {

template <>
struct BackendContainerTrait<BlockPostPulse>
    : public BackendContainerBase<
          BlockPostPulse,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
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
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	Count m_count;
	bool m_overflow;
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(SpikeCounterRead)
#endif

namespace detail {

template <>
struct BackendContainerTrait<SpikeCounterRead>
    : public BackendContainerBase<
          SpikeCounterRead,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
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
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

#ifndef __ppu
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(SpikeCounterReset)
#endif

namespace detail {

template <>
struct BackendContainerTrait<SpikeCounterReset>
    : public BackendContainerBase<
          SpikeCounterReset,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail


// TODO: Switch to CRTP pattern when https://github.com/kljohann/genpybind/issues/28 is solved
class GENPYBIND(visible) NeuronSRAMTimingConfig : public detail::SRAMTimingConfig
{
public:
	typedef halco::hicann_dls::vx::NeuronSRAMTimingConfigOnDLS coordinate_type;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

	bool operator==(NeuronSRAMTimingConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronSRAMTimingConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, NeuronSRAMTimingConfig const& config)
	    SYMBOL_VISIBLE;

	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(NeuronSRAMTimingConfig)
#endif

namespace detail {

template <>
struct BackendContainerTrait<NeuronSRAMTimingConfig>
    : public BackendContainerBase<
          NeuronSRAMTimingConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail


// TODO: Switch to CRTP pattern when https://github.com/kljohann/genpybind/issues/28 is solved
class GENPYBIND(visible) NeuronBackendSRAMTimingConfig : public detail::SRAMTimingConfig
{
public:
	typedef halco::hicann_dls::vx::NeuronBackendSRAMTimingConfigOnDLS coordinate_type;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation, hxcomm::vx::Target::hardware};
#endif

	bool operator==(NeuronBackendSRAMTimingConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(NeuronBackendSRAMTimingConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, NeuronBackendSRAMTimingConfig const& config)
	    SYMBOL_VISIBLE;

	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(NeuronBackendSRAMTimingConfig)
#endif

namespace detail {

template <>
struct BackendContainerTrait<NeuronBackendSRAMTimingConfig>
    : public BackendContainerBase<
          NeuronBackendSRAMTimingConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail

#define NEURON_EXTERN_TEMPLATE(Coordinates)                                                        \
	extern template class SYMBOL_VISIBLE NeuronBackendConfig<Coordinates>;                         \
                                                                                                   \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::addresses(coordinate_type const& coord);                     \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusAddress,                                                     \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::addresses(coordinate_type const& coord);                     \
                                                                                                   \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    fisch::vx::word_access_type::OmnibusChipOverJTAG,                                          \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::encode() const;                                              \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    fisch::vx::word_access_type::Omnibus,                                                      \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::encode() const;                                              \
                                                                                                   \
	extern template SYMBOL_VISIBLE void NeuronBackendConfig<Coordinates>::decode(                  \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::OmnibusChipOverJTAG,                                      \
	        NeuronBackendConfig<Coordinates>::config_size_in_words> const& data);                  \
	extern template SYMBOL_VISIBLE void NeuronBackendConfig<Coordinates>::decode(                  \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::Omnibus,                                                  \
	        NeuronBackendConfig<Coordinates>::config_size_in_words> const& data);

} // namespace vx
} // namespace haldls
