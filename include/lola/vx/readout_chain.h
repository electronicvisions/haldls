#pragma once

#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#define ID(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
// clang-format off
#define CONCAT(w, x, y, z) STR(ID(w)ID(/)ID(x)ID(/)ID(v)ID(y)ID(/)ID(z))
#include CONCAT(haldls,vx,CHIP_REVISION,capmem.h)
// clang-format on
#undef ID
#undef XSTR
#undef STR
#undef CONCAT

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/readout.h"
#include "halco/hicann-dls/vx/current_dac.h"
#include "haldls/vx/madc.h"
#include "haldls/vx/current_dac.h"
#include "haldls/vx/readout.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

#if CHIP_REVISION == 3
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V3
#define CHIP_REVISION_STR v3
#elif CHIP_REVISION == 4
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V4
#define CHIP_REVISION_STR v4
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR GENPYBIND_TAG_LOLA_VX_VY {
#undef GENPYBIND_TAG_LOLA_VX_VY

/**
 * Configuration of all circuits composing the analog readout chain.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ReadoutChain
    : public haldls::vx::ContainerBase<ReadoutChain>
{
public:
	typedef halco::hicann_dls::vx::ReadoutChainOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::Value AnalogValue GENPYBIND(visible);
	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::DisableRefresh AnalogDisableRefresh
	    GENPYBIND(visible);
	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::value_type AnalogValueVariant;

	ReadoutChain() = default;

	/**
	 * Configuration of the multiplexer for the different input signals from chip
	 *
	 * These source multiplexers connect to the Dynamic multiplexer and are the
	 * standard way of selecting the readout source of the MADC. They
	 * can also be connected to the pads.
	 */
	typedef halco::common::typed_array<
	    haldls::vx::ReadoutSourceSelection::SourceMultiplexer,
	    halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection>
	    InputMux GENPYBIND(opaque(false));

	/**
	 * Configuration of the buffer connection the pads
	 *
	 * The source multiplexers can drive buffered signals to the two pad multiplexers.
	 * Note that you still have to select the corresponding connection at the pad
	 * multiplexers in order to make the connection.
	 */
	struct GENPYBIND(visible) BufferToPadT
	{
		BufferToPadT() SYMBOL_VISIBLE;

		/**
		 * Connect input multiplexer to PAD multiplexer.
		 */
		bool enable;

		/**
		 * Bias current for buffer connecting to PAD.
		 */
		AnalogValueVariant amp_i_bias;

		bool operator==(BufferToPadT const& other) const SYMBOL_VISIBLE;
		bool operator!=(BufferToPadT const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, BufferToPadT const& config)
		    SYMBOL_VISIBLE;
	};

	typedef halco::common::
	    typed_array<BufferToPadT, halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection>
	        BufferToPad GENPYBIND(opaque(false));

	/**
	 * Configuration of the Dynamic multiplexer
	 *
	 * This multiplexer switches between the two available input channels
	 * (source multiplexers) on a per-sample basis.
	 */
	struct GENPYBIND(visible) DynamicMux
	{
		DynamicMux() = default;

		typedef haldls::vx::MADCConfig::ActiveMuxInputSelectLength InputSelectLength
		    GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::input_selection_type InputSelect GENPYBIND(visible);
		/**
		 * Enable Mux amplifiers.
		 */
		bool enable_amplifiers{true};

		/**
		 * Input channel (one of two multiplexers) to acquire the first batch of samples from.
		 */
		InputSelect initially_selected_input{};

		/**
		 * Number of samples after which the other input channel is selected. Use 1 to sample both
		 * channels alternately.
		 */
		InputSelectLength input_select_length{};

		/**
		 * Bias current for amplifier in dynamic mux.
		 */
		AnalogValueVariant i_bias{AnalogValue{500}};

		bool operator==(DynamicMux const& other) const SYMBOL_VISIBLE;
		bool operator!=(DynamicMux const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, DynamicMux const& config) SYMBOL_VISIBLE;
	};

	typedef haldls::vx::MADCConfig::hemisphere_type HemisphereType GENPYBIND(visible);
	typedef haldls::vx::MADCConfig::synapse_target_type SynapseTargetType GENPYBIND(visible);

	/**
	 * Configuration of the pseudo differential converter.
	 *
	 * This circuit adds a reference to the signal measured by one of the two
	 * source multiplexers. It is forwarded to the MADC as a differential signal.
	 */
	struct GENPYBIND(visible) PseudoDifferentialConverter
	{
		PseudoDifferentialConverter() = default;

		/**
		 * Connect reference to a wire on north or south hemisphere.
		 */
		HemisphereType enable_reference{{false, false}};

		/**
		 * Reference voltage.
		 */
		AnalogValueVariant v_ref{AnalogValue{400}};

		/**
		 * Bias current for the buffer providing the reference voltage.
		 */
		AnalogValueVariant buffer_bias{AnalogValue{0}};

		bool operator==(PseudoDifferentialConverter const& other) const SYMBOL_VISIBLE;
		bool operator!=(PseudoDifferentialConverter const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, PseudoDifferentialConverter const& config)
		    SYMBOL_VISIBLE;
	};

	/**
	 * Configuration of the crossbar connecting the MADC to the different measure/readout circuits.
	 */
	struct GENPYBIND(visible) MADCInputCrossbar
	{
		MADCInputCrossbar() SYMBOL_VISIBLE;

		/**
		 * Connect the SourceMeasureUnit to the input crossbar.
		 */
		bool connect_source_measure_unit;

		/**
		 * Connect the DynamicMux to the input crossbar.
		 */
		bool connect_dynamic_mux;

		/**
		 * Connect the PADMux to the input crossbar, using the debug_plus and debug_minus lines.
		 */
		bool connect_pad_mux;

		/**
		 * Connect the MADCPreamp to the input crossbar.
		 */
		bool connect_madc_preamp;


		bool operator==(MADCInputCrossbar const& other) const SYMBOL_VISIBLE;
		bool operator!=(MADCInputCrossbar const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, MADCInputCrossbar const& config)
		    SYMBOL_VISIBLE;
	};


	typedef haldls::vx::MADCConfig::SamplingWindowTiming SamplingWindowTiming GENPYBIND(visible);

	/**
	 * Configuration of the MADC preamplifier
	 */
	struct GENPYBIND(visible) MADCPreamp
	{
		MADCPreamp() = default;

		typedef haldls::vx::MADCConfig::PreampGainCapacitorSize GainCapacitorSize
		    GENPYBIND(visible);

		/**
		 * Gain capacitor size.
		 */
		GainCapacitorSize gain_cap_size{};

		/**
		 * Start of sampling window.
		 */
		SamplingWindowTiming sampling_window_start{3};

		/**
		 * End of sampling window.
		 */
		SamplingWindowTiming sampling_window_end{8};

		/**
		 * Reference voltage.
		 */
		AnalogValueVariant v_ref{AnalogValue{400}};

		/**
		 * Bias current.
		 */
		AnalogValueVariant i_bias{AnalogValue{500}};

		bool operator==(MADCPreamp const& other) const SYMBOL_VISIBLE;
		bool operator!=(MADCPreamp const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, MADCPreamp const& config) SYMBOL_VISIBLE;
	};

	/**
	 * Configuration of the crossbar connecting the MADC to debug pads
	 */
	struct GENPYBIND(visible) MADCDebugCrossbar
	{
		MADCDebugCrossbar() SYMBOL_VISIBLE;

		typedef haldls::vx::MADCConfig::PreampGainCapacitorSize GainCapacitorSize
		    GENPYBIND(visible);

		/**
		 * Connect MADCPreamp to MADC.
		 */
		bool connect_preamp_to_madc;

		/**
		 * Connect MADCDebugPads to MADC.
		 */
		bool connect_pads_to_madc;

		/**
		 * Connect MADCPreamp to MADCDebugPads.
		 */
		bool connect_preamp_to_pads;

		bool operator==(MADCDebugCrossbar const& other) const SYMBOL_VISIBLE;
		bool operator!=(MADCDebugCrossbar const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, MADCDebugCrossbar const& config)
		    SYMBOL_VISIBLE;
	};

	/**
	 * Configuration of the MADC
	 */
	struct GENPYBIND(visible) MADC
	{
		MADC() = default;

		typedef haldls::vx::MADCConfig::SampleDurationAdjust SampleDurationAdjust
		    GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::SARResetWait SARResetWait GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::SARResetLength SARResetLength GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::PowerupWaitValue PowerupWaitValue GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::CalibrationWaitValue CalibrationWaitValue
		    GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::NumberOfSamples NumberOfSamples GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::MADCClockScaleValue ClockScaleValue GENPYBIND(visible);
		typedef haldls::vx::MADCConfig::ConversionCyclesOffset ConversionCyclesOffset
		    GENPYBIND(visible);

		/**
		 * Sample duration adjustment.
		 * This seems to be redundant to `conversion_cycles_offset`. Both settings are summed up in
		 * hardware.
		 */
		SampleDurationAdjust sample_duration_adjust{SampleDurationAdjust()};

		/**
		 * Configuration of the MADC's SAR reset timing.
		 */
		bool enable_sar_reset_on_fall{false};
		SARResetWait sar_reset_wait{};
		SARResetLength sar_reset_length{};

		/**
		 * Powerup wait value.
		 * The MADC remains in the wake up state for a number of MADC clock cycles determined by
		 * this property before transitioning to the `READY` state (via `ENABLE`).
		 */
		PowerupWaitValue powerup_wait_value{};

		/**
		 * Conversion cycles offset.
		 * This seems to be redundant to `sample_duration_adjust`. Both settings are summed up in
		 * hardware.
		 */
		ConversionCyclesOffset conversion_cycles_offset{};

		/**
		 * Enable built-in calibration of the MADC.
		 */
		bool enable_calibration{false};

		/**
		 * Built-in calibration's timing.
		 */
		CalibrationWaitValue calibration_wait_value{};

		/**
		 * Number of samples recorded by the MADC.
		 * The MADC records a fixed number of samples as long as it is not
		 * configured to sample continuously (c.f. `enable_sample_continously` in MADCControl).
		 */
		NumberOfSamples number_of_samples{};

		/*
		 * Let the MADC sample on the positive clock edge.
		 */
		bool sample_on_positive_edge{true};

		/*
		 * Enable dummy data generation.
		 * If enabled, the MADC will stream out the last 10 bit of the sample
		 * counter value as a test pattern (sawtooth).
		 */
		bool enable_dummy_data{false};

		/**
		 * Enable clock scaling.
		 * The clock is derived from a PLL madc_clk output.
		 */
		bool enable_madc_clock_scaling{false};

		/**
		 * Clock scale value.
		 * The clock is derived from a PLL madc_clk output.
		 */
		ClockScaleValue clock_scale_value{};

		/**
		 * 500nA reference current.
		 */
		AnalogValueVariant in_500na{AnalogValue{500}};

		bool operator==(MADC const& other) const SYMBOL_VISIBLE;
		bool operator!=(MADC const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, MADC const& config) SYMBOL_VISIBLE;
	};

	/**
	 * Configuration of the Pad multiplexers
	 *
	 * These provide are a multitude of possibilities for reading out voltages from chip
	 * or making direct (unbuffered) connections to outside the chip.
	 */
	typedef halco::common::typed_array<
	    haldls::vx::PadMultiplexerConfig,
	    halco::hicann_dls::vx::PadMultiplexerConfigOnDLS>
	    PadMux GENPYBIND(opaque(false));

	/**
	 * Configuration of the source measure unit
	 *
	 * An input current from neuron membranes or synapse lines can be measured:
	 * the current is converted to a voltage that is sampled by the MADC.
	 */
	struct GENPYBIND(visible) SourceMeasureUnit
	{
		SourceMeasureUnit() = default;

		/**
		 * Start of sampling window.
		 */
		SamplingWindowTiming sampling_window_start{9};

		/**
		 * End of sampling window.
		 */
		SamplingWindowTiming sampling_window_end{14};

		/**
		 * Connect neuron stimulus per hemisphere.
		 */
		HemisphereType connect_neuron_stimulus{{false, false}};

		/**
		 * Connect synapse debug wires.
		 */
		SynapseTargetType connect_synapse_debug{{false, false}};

		/**
		 * Test reference voltage.
		 */
		AnalogValueVariant test_voltage{AnalogValue{400}};

		/**
		 * Buffer bias current.
		 */
		AnalogValueVariant buffer_i_bias{};

		/**
		 * Amplifier reference voltage.
		 */
		AnalogValueVariant amp_v_ref{AnalogValue{400}};

		/**
		 * Amplifier bias current.
		 */
		AnalogValueVariant amp_i_bias{};

		bool operator==(SourceMeasureUnit const& other) const SYMBOL_VISIBLE;
		bool operator!=(SourceMeasureUnit const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, SourceMeasureUnit const& config)
		    SYMBOL_VISIBLE;
	};

	/**
	 * Configuration of the current DAC of the readout chain.
	 *
	 * The DAC's output can be connected to neuron membranes or synapse lines.
	 * The potential there can be sampled using the MADC concurrently.
	 */
	struct GENPYBIND(visible) CurrentDAC
	{
		typedef haldls::vx::CurrentDAC::Current Current GENPYBIND(visible);
		typedef haldls::vx::CurrentDAC::Sign Sign GENPYBIND(visible);

		CurrentDAC() SYMBOL_VISIBLE;

		/**
		 * Enable current DAC.
		 */
		bool enable_current;

		/**
		 * Current value of DAC.
		 */
		Current current;

		/**
		 * Sing of current, either source or sink.
		 */
		Sign sign;

		/**
		 * Connect neuron stimulus per hemisphere.
		 */
		HemisphereType connect_neuron_stimulus;

		/**
		 * Connect synapse debug wires.
		 */
		SynapseTargetType connect_synapse_debug;

		/**
		 * Bias current.
		 */
		AnalogValueVariant i_bias;

		/**
		 * Cascode bias current.
		 */
		AnalogValueVariant i_bias_casc;

		bool operator==(CurrentDAC const& other) const SYMBOL_VISIBLE;
		bool operator!=(CurrentDAC const& other) const SYMBOL_VISIBLE;
		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, CurrentDAC const& config) SYMBOL_VISIBLE;
	};

	InputMux input_mux;
	BufferToPad buffer_to_pad;
	DynamicMux dynamic_mux;
	PseudoDifferentialConverter pseudo_diff_converter;
	MADCInputCrossbar madc_input_crossbar;
	MADCPreamp madc_preamp;
	MADCDebugCrossbar madc_debug_crossbar;
	MADC madc;
	PadMux pad_mux;
	SourceMeasureUnit source_measure_unit;
	CurrentDAC current_dac;

	bool operator==(ReadoutChain const& other) const SYMBOL_VISIBLE;
	bool operator!=(ReadoutChain const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ReadoutChain const& config) SYMBOL_VISIBLE;

private:
	friend haldls::vx::detail::VisitPreorderImpl<ReadoutChain>;
};

} // lola::vx::CHIP_REVISION_STR

#include "lola/vx/readout_chain.tcc"
