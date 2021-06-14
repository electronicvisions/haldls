#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/chip.h"
#include "halco/hicann-dls/vx/madc.h"
#include "halco/hicann-dls/vx/readout.h"
#include "halco/hicann-dls/vx/synapse.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for MADC control state machine.
 * Flow chart of state machine:
 * https://chat.bioai.eu/visions/pl/s4o4mq36xtyeigtzkw9kgfihcc
 * FIXME: Add more explanatory description (issue #3471)
 */
class GENPYBIND(visible) MADCControl
{
public:
	typedef halco::hicann_dls::vx::MADCControlOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	MADCControl() SYMBOL_VISIBLE;

	// accessors

	/** Enable switched capacitor amplifier for current measurements. */
	GENPYBIND(getter_for(enable_iconv_amplifier))
	bool get_enable_iconv_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_iconv_amplifier))
	void set_enable_iconv_amplifier(bool const value) SYMBOL_VISIBLE;

	/** Enable switched capacitor pre-amplifier for voltage measurements. */
	GENPYBIND(getter_for(enable_pre_amplifier))
	bool get_enable_pre_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_pre_amplifier))
	void set_enable_pre_amplifier(bool const value) SYMBOL_VISIBLE;

	/** Let the MADC sample continuously. */
	GENPYBIND(getter_for(enable_continuous_sampling))
	bool get_enable_continuous_sampling() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_continuous_sampling))
	void set_enable_continuous_sampling(bool const value) SYMBOL_VISIBLE;

	/** Let the MADC power down after it finished sampling. */
	GENPYBIND(getter_for(enable_power_down_after_sampling))
	bool get_enable_power_down_after_sampling() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_power_down_after_sampling))
	void set_enable_power_down_after_sampling(bool const value) SYMBOL_VISIBLE;

	/** Stop a currently active recording. */
	GENPYBIND(getter_for(stop_recording))
	bool get_stop_recording() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(stop_recording))
	void set_stop_recording(bool const value) SYMBOL_VISIBLE;

	/** Start a recording. */
	GENPYBIND(getter_for(start_recording))
	bool get_start_recording() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(start_recording))
	void set_start_recording(bool const value) SYMBOL_VISIBLE;

	/** Wake up the MADC. */
	GENPYBIND(getter_for(wake_up))
	bool get_wake_up() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wake_up))
	void set_wake_up(bool const value) SYMBOL_VISIBLE;

	bool operator==(MADCControl const& other) const SYMBOL_VISIBLE;
	bool operator!=(MADCControl const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MADCControl const& event) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable_iconv_amplifier;
	bool m_enable_pre_amplifier;
	bool m_enable_continuous_sampling;
	bool m_enable_power_down_after_sampling;
	bool m_stop_recording;
	bool m_start_recording;
	bool m_wake_up;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(MADCControl)

namespace detail {

template <>
struct BackendContainerTrait<MADCControl>
    : public BackendContainerBase<MADCControl, fisch::vx::Omnibus, fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Configuration container for MADC and related circuitry.
 */
class GENPYBIND(visible) MADCConfig
{
public:
	typedef halco::hicann_dls::vx::MADCConfigOnDLS coordinate_type;
	typedef halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection input_selection_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) ActiveMuxInputSelectLength
	    : public halco::common::detail::RantWrapper<ActiveMuxInputSelectLength, uint_fast8_t, 15, 0>
	{
		constexpr explicit ActiveMuxInputSelectLength(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) SampleDurationAdjust
	    : public halco::common::detail::RantWrapper<SampleDurationAdjust, uint_fast8_t, 7, 0>
	{
		constexpr explicit SampleDurationAdjust(uintmax_t const val = 5)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) SARResetWait
	    : public halco::common::detail::RantWrapper<SARResetWait, uint_fast8_t, 3, 0>
	{
		constexpr explicit SARResetWait(uintmax_t const val = 3)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) SARResetLength
	    : public halco::common::detail::RantWrapper<SARResetLength, uint_fast8_t, 3, 0>
	{
		constexpr explicit SARResetLength(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) PowerupWaitValue
	    : public halco::common::detail::RantWrapper<PowerupWaitValue, uint_fast16_t, 1023, 0>
	{
		constexpr explicit PowerupWaitValue(uintmax_t const val = 96)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) ConversionCyclesOffset
	    : public halco::common::detail::RantWrapper<ConversionCyclesOffset, uint_fast8_t, 15, 0>
	{
		constexpr explicit ConversionCyclesOffset(uintmax_t const val = 12)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) CalibrationWaitValue
	    : public halco::common::detail::RantWrapper<CalibrationWaitValue, uint_fast16_t, 1023, 0>
	{
		constexpr explicit CalibrationWaitValue(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) NumberOfSamples
	    : public halco::common::detail::RantWrapper<NumberOfSamples, uint_fast16_t, 65535, 0>
	{
		constexpr explicit NumberOfSamples(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) SamplingWindowTiming
	    : public halco::common::detail::RantWrapper<SamplingWindowTiming, uint_fast8_t, 31, 0>
	{
		constexpr explicit SamplingWindowTiming(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) PreampGainCapacitorSize
	    : public halco::common::detail::RantWrapper<PreampGainCapacitorSize, uint_fast8_t, 31, 0>
	{
		constexpr explicit PreampGainCapacitorSize(uintmax_t const val = 31)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) MADCClockScaleValue
	    : public halco::common::detail::RantWrapper<MADCClockScaleValue, uint_fast16_t, 4095, 0>
	{
		constexpr explicit MADCClockScaleValue(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::HemisphereOnDLS> hemisphere_type
	    GENPYBIND(opaque(false));

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::SynapticInputOnNeuron>
	    synapse_target_type GENPYBIND(opaque(false));

	/**
	 * Default constructor for the MADC config.
	 * The default values are in a usable state, the MADC is connected
	 * to the SourceMultiplexerOnReadoutSourceSelection(0).
	 * Only the number of samples to record is required to be set.
	 */
	MADCConfig() SYMBOL_VISIBLE;

	// accessors

	/**
	 * Set the initially selected input of the active mux.
	 *
	 * Caution: the property is called `initially_selected_amp` in RTL.
	 */
	GENPYBIND(getter_for(active_mux_initially_selected_input))
	input_selection_type get_active_mux_initially_selected_input() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(active_mux_initially_selected_input))
	void set_active_mux_initially_selected_input(input_selection_type const& value) SYMBOL_VISIBLE;

	/**
	 * Set the number of samples the mux's inputs are selected for in direct succession.
	 *
	 * In the following example the MADC would record *four* samples on one input, before the mux
	 * switches to the other one:
	 * input 0: ||||    ||||     …
	 * input 1:     ||||    |||| …
	 *
	 * Caution: the property is called `amp_select_length` in RTL.
	 */
	GENPYBIND(getter_for(active_mux_input_select_length))
	ActiveMuxInputSelectLength get_active_mux_input_select_length() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(active_mux_input_select_length))
	void set_active_mux_input_select_length(ActiveMuxInputSelectLength const& value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the sample duration adjustment.
	 * This seems to be redundant to `conversion_cycles_offset`. Both settings are summed up in
	 * hardware.
	 *
	 * Caution: the property is called `sample_adjust` in RTL.
	 * Caution: the meaning of this setting was reverse engineered due to a lack of documentation!
	 */
	GENPYBIND(getter_for(sample_duration_adjust))
	SampleDurationAdjust get_sample_duration_adjust() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(sample_duration_adjust))
	void set_sample_duration_adjust(SampleDurationAdjust const& value) SYMBOL_VISIBLE;

	/**
	 * Configuration of the MADC's SAR reset timing.
	 */
	GENPYBIND(getter_for(enable_sar_reset_on_fall))
	bool get_enable_sar_reset_on_fall() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_sar_reset_on_fall))
	void set_enable_sar_reset_on_fall(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(sar_reset_wait))
	SARResetWait get_sar_reset_wait() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(sar_reset_wait))
	void set_sar_reset_wait(SARResetWait const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(sar_reset_length))
	SARResetLength get_sar_reset_length() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(sar_reset_length))
	void set_sar_reset_length(SARResetLength const& value) SYMBOL_VISIBLE;

	/**
	 * Configuration of the powerup wait value.
	 * This MADC remains in the wake up state for a number of MADC clock cycles determined by this
	 * property before transitioning to the `READY` state (via `ENABLE`).
	 */
	GENPYBIND(getter_for(powerup_wait_value))
	PowerupWaitValue get_powerup_wait_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(powerup_wait_value))
	void set_powerup_wait_value(PowerupWaitValue const& value) SYMBOL_VISIBLE;

	/**
	 * Configuration for the conversion cycles offset.
	 * This seems to be redundant to `sample_duration_adjust`. Both settings are summed up in
	 * hardware.
	 *
	 * Caution: the meaning of this setting was reverse engineered due to a lack of documentation!
	 */
	GENPYBIND(getter_for(conversion_cycles_offset))
	ConversionCyclesOffset get_conversion_cycles_offset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(conversion_cycles_offset))
	void set_conversion_cycles_offset(ConversionCyclesOffset const& value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the built-in calibration's timing of the MADC.
	 */
	GENPYBIND(getter_for(calibration_wait_value))
	CalibrationWaitValue get_calibration_wait_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(calibration_wait_value))
	void set_calibration_wait_value(CalibrationWaitValue const& value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the built-in self-calibration.
	 */
	GENPYBIND(getter_for(enable_calibration))
	bool get_enable_calibration() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_calibration))
	void set_enable_calibration(bool const value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the number of samples recorded by the MADC.
	 * The MADC records a fixed number of samples as long as it is not
	 * configured to sample continuously (c.f. `enable_sample_continously`).
	 *
	 * Caution: this property is called `num_samples` in the RTL description.
	 */
	GENPYBIND(getter_for(number_of_samples))
	NumberOfSamples get_number_of_samples() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(number_of_samples))
	void set_number_of_samples(NumberOfSamples const& value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the timing of the sampling phase for both, the
	 * (voltage recording) preamp and the current conversion circuit.
	 *
	 * The sampling window is determined by the timing of its rising and
	 * falling edge.
	 *
	 * Caution: these properties are called `phi_vga_toggle[0,1]` and
	 *   `phi_2quadrant_toggle[0,1]` for the preamp and current conversion,
	 *   respectively.
	 */
	GENPYBIND(getter_for(preamp_sampling_window_start))
	SamplingWindowTiming get_preamp_sampling_window_start() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(preamp_sampling_window_start))
	void set_preamp_sampling_window_start(SamplingWindowTiming const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(preamp_sampling_window_end))
	SamplingWindowTiming get_preamp_sampling_window_end() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(preamp_sampling_window_end))
	void set_preamp_sampling_window_end(SamplingWindowTiming const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(iconv_sampling_window_start))
	SamplingWindowTiming get_iconv_sampling_window_start() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(iconv_sampling_window_start))
	void set_iconv_sampling_window_start(SamplingWindowTiming const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(iconv_sampling_window_end))
	SamplingWindowTiming get_iconv_sampling_window_end() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(iconv_sampling_window_end))
	void set_iconv_sampling_window_end(SamplingWindowTiming const& value) SYMBOL_VISIBLE;

	/*
	 * Let the MADC sample on the positive clock edge.
	 *
	 * Caution: this property is called `sample_pos_edge` in the RTL description.
	 */
	GENPYBIND(getter_for(sample_on_positive_edge))
	bool get_sample_on_positive_edge() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(sample_on_positive_edge))
	void set_sample_on_positive_edge(bool const value) SYMBOL_VISIBLE;

	/*
	 * Accessor for enabling dummy data generation.
	 * If enabled, the MADC will stream out the last 10 bit of the sample
	 * counter value as a test pattern (sawtooth).
	 */
	GENPYBIND(getter_for(enable_dummy_data))
	bool get_enable_dummy_data() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_dummy_data))
	void set_enable_dummy_data(bool const value) SYMBOL_VISIBLE;

	/*
	 * Accessors for configuration of the MADC input multiplexer.
	 * This multiplexer allows to patch any combination of the MADC's
	 * differential inputs, the preamp, and the two analog I/Os of the
	 * chip.
	 * In the vast majority of use cases, the MADC is set to record from
	 * the preamp.
	 * Connecting the preamp's outputs or the MADC's inputs to the pads
	 * allows to characterize the two circuits in isolation.
	 *
	 * Caution: the corresponding properties are called
	 *   `en_amp_to_adc`, `en_adc_to_pad`, and `en_amp_to_pad` in the RTL
	 *   description, where the two latter properties further seem to be
	 *   reversed.
	 */
	GENPYBIND(getter_for(connect_preamp_to_madc))
	bool get_connect_preamp_to_madc() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_preamp_to_madc))
	void set_connect_preamp_to_madc(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(connect_pads_to_madc))
	bool get_connect_pads_to_madc() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_pads_to_madc))
	void set_connect_pads_to_madc(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(connect_preamp_to_pads))
	bool get_connect_preamp_to_pads() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_preamp_to_pads))
	void set_connect_preamp_to_pads(bool const value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the preamplifier's gain selection capacitor.
	 */
	GENPYBIND(getter_for(preamp_gain_capacitor_size))
	PreampGainCapacitorSize get_preamp_gain_capacitor_size() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(preamp_gain_capacitor_size))
	void set_preamp_gain_capacitor_size(PreampGainCapacitorSize const& value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the clock scaling configuration.
	 * The clock is derived from a PLL madc_clk output.
	 */
	GENPYBIND(getter_for(enable_madc_clock_scaling))
	bool get_enable_madc_clock_scaling() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_madc_clock_scaling))
	void set_enable_madc_clock_scaling(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(madc_clock_scale_value))
	MADCClockScaleValue get_madc_clock_scale_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(madc_clock_scale_value))
	void set_madc_clock_scale_value(MADCClockScaleValue const& value) SYMBOL_VISIBLE;

	/*
	 * Accessors for the enabledness of the active mux amplifiers.
	 *
	 * Caution: the corresponding property is called `act_mux_en_amps` in the
	 *   RTL description.
	 */
	GENPYBIND(getter_for(enable_active_mux_amplifiers))
	bool get_enable_active_mux_amplifiers() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_active_mux_amplifiers))
	void set_enable_active_mux_amplifiers(bool const value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the configuration of the pseudo-differential reference antenna.
	 */
	GENPYBIND(getter_for(enable_pseudo_differential_reference), return_value_policy(reference))
	hemisphere_type const& get_enable_pseudo_differential_reference() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_pseudo_differential_reference), return_value_policy(reference))
	void set_enable_pseudo_differential_reference(hemisphere_type const& value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the preamp mux located before the preamplifier.
	 * This mux has four (pairs of) inputs: the active mux (for voltage measurements),
	 * the current conversion circuit, two debug lines (which might be applied to
	 * the chip's pads), and the preamplifier.
	 * The first two serve as signal sources, the latter as a sink. The pads may serve
	 * as either.
	 * To connect two (or more) sources/sinks, the user must connect any of them.
	 *
	 * Caution: the respective properties are called `fmc_2_quadrant`, `fmc_act_mux`,
	 *   `fmc_debug`, and `fmc_to_vga`.
	 */
	GENPYBIND(getter_for(signal_selection_connect_iconv))
	bool get_signal_selection_connect_iconv() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(signal_selection_connect_iconv))
	void set_signal_selection_connect_iconv(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(signal_selection_connect_active_mux))
	bool get_signal_selection_connect_active_mux() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(signal_selection_connect_active_mux))
	void set_signal_selection_connect_active_mux(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(signal_selection_connect_debug))
	bool get_signal_selection_connect_debug() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(signal_selection_connect_debug))
	void set_signal_selection_connect_debug(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(signal_selection_connect_preamp))
	bool get_signal_selection_connect_preamp() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(signal_selection_connect_preamp))
	void set_signal_selection_connect_preamp(bool const value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the configuration of the current conversion circuit.
	 *
	 * The first property enables the internal amplifier of the circuit.
	 * The other properties allow to connect either the direct neuron access
	 * or the synaptic debug lines.
	 */
	GENPYBIND(getter_for(enable_iconv_amplifier))
	bool get_enable_iconv_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_iconv_amplifier))
	void set_enable_iconv_amplifier(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(connect_iconv_neuron), return_value_policy(reference))
	hemisphere_type const& get_connect_iconv_neuron() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_iconv_neuron), return_value_policy(reference))
	void set_connect_iconv_neuron(hemisphere_type const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(connect_iconv_synapse), return_value_policy(reference))
	synapse_target_type const& get_connect_iconv_synapse() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(connect_iconv_synapse), return_value_policy(reference))
	void set_connect_iconv_synapse(synapse_target_type const& value) SYMBOL_VISIBLE;

	/**
	 * Accessors for the sampling rate of the madc in MHz.
	 *
	 * Caution: The formula of the underlying function was derived from sampled data.
	 * The calculation considers only three settings of the MADC state:
	 *   * m_sample_duration_adjust
	 *   * m_enable_madc_clock_scaling
	 *   * m_madc_clock_scale_value
	 */
	double get_sample_rate() const SYMBOL_VISIBLE;

	bool operator==(MADCConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(MADCConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 10;
	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord) const
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MADCConfig const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const);

	input_selection_type m_active_mux_initially_selected_input;
	ActiveMuxInputSelectLength m_active_mux_input_select_length;
	SampleDurationAdjust m_sample_duration_adjust;
	bool m_enable_sar_reset_on_fall;
	SARResetWait m_sar_reset_wait;
	SARResetLength m_sar_reset_length;
	PowerupWaitValue m_powerup_wait_value;
	ConversionCyclesOffset m_conversion_cycles_offset;
	CalibrationWaitValue m_calibration_wait_value;
	bool m_enable_calibration;
	NumberOfSamples m_number_of_samples;
	SamplingWindowTiming m_preamp_sampling_window_start;
	SamplingWindowTiming m_preamp_sampling_window_end;
	SamplingWindowTiming m_iconv_sampling_window_start;
	SamplingWindowTiming m_iconv_sampling_window_end;
	bool m_sample_on_positive_edge;
	bool m_enable_dummy_data;
	bool m_connect_preamp_to_madc;
	bool m_connect_pads_to_madc;
	bool m_connect_preamp_to_pads;
	PreampGainCapacitorSize m_preamp_gain_capacitor_size;
	bool m_enable_madc_clock_scaling;
	MADCClockScaleValue m_madc_clock_scale_value;
	bool m_enable_active_mux_amplifiers;
	hemisphere_type m_enable_pseudo_differential_reference;
	bool m_signal_selection_connect_iconv;
	bool m_signal_selection_connect_active_mux;
	bool m_signal_selection_connect_debug;
	bool m_signal_selection_connect_preamp;
	bool m_enable_iconv_amplifier;
	hemisphere_type m_connect_iconv_neuron;
	synapse_target_type m_connect_iconv_synapse;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(MADCConfig)

namespace detail {

template <>
struct BackendContainerTrait<MADCConfig>
    : public BackendContainerBase<MADCConfig, fisch::vx::Omnibus, fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
