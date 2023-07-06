#pragma once
#include "hate/visibility.h"
#include "stadls/vx/v3/playback_generator.h"
#include "stadls/vx/v3/playback_program_builder.h"

namespace stadls::vx::v3 GENPYBIND_TAG_STADLS_VX_V3 {

namespace detail {

class GENPYBIND(expose_as(_InitGenerator)) InitGenerator
{
public:
	/** Default constructor. */
	InitGenerator() SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef v3::PlaybackProgramBuilder Builder;

	/** Instruction timeout duration on FPGA config. */
	haldls::vx::InstructionTimeoutConfig instruction_timeout;

	/** Shift register on xBoard config. */
	haldls::vx::ShiftRegister shift_register;

	/** DAC control config on xBoard. */
	lola::vx::DACControlBlock dac_control_block;

	/** DAC channel configs on xBoard. */
	lola::vx::DACChannelBlock dac_channel_block;

	/**
	 * Enable xBoard DAC and shift register configuration.
	 * Disabling this allows to discard unnecessary configuration when simulating.
	 * @note Disabling this discards all xBoard configuration
	 */
	bool enable_xboard;

	/** JTAG clock scaler setting. */
	haldls::vx::JTAGClockScaler jtag_clock_scaler;

	/** PLL clock output block setting. */
	haldls::vx::v3::PLLClockOutputBlock pll_clock_output_block;

	/** ADPLL setting. */
	typedef halco::common::typed_array<haldls::vx::ADPLL, halco::hicann_dls::vx::v3::ADPLLOnDLS>
	    adplls_type GENPYBIND(opaque(false));
	adplls_type adplls;

	struct GENPYBIND(visible) HighspeedLink
	{
		/** Default constructor. */
		HighspeedLink() SYMBOL_VISIBLE;

		/** Highspeed-link PHY enable values for the FPGA side. */
		haldls::vx::CommonPhyConfigFPGA common_phy_config_fpga;

		/** Highspeed-link PHY enable values for the chip side. */
		haldls::vx::CommonPhyConfigChip common_phy_config_chip;

		/** Highspeed-link PHY settings for the FPGA side. */
		typedef halco::common::
		    typed_array<haldls::vx::PhyConfigFPGA, halco::hicann_dls::vx::v3::PhyConfigFPGAOnDLS>
		        phy_configs_fpga_type GENPYBIND(opaque(false));
		phy_configs_fpga_type phy_configs_fpga;

		/** Highspeed-link PHY settings for the chip side. */
		typedef halco::common::
		    typed_array<haldls::vx::PhyConfigChip, halco::hicann_dls::vx::v3::PhyConfigChipOnDLS>
		        phy_configs_chip_type GENPYBIND(opaque(false));
		phy_configs_chip_type phy_configs_chip;

		/** Systime synchronization base value. */
		haldls::vx::SystimeSyncBase systime_sync_base;

		/**
		 * Enable systime synchronization.
		 * @note Disabling this discards all systime related configuration, i.e. setting the
		 * SystimeSyncBase and initiating the time hand-shake between the chip and the FPGA.
		 * This leads to no time annotation being present in the response stream.
		 */
		bool enable_systime;

	} highspeed_link;

	/**
	 * Enable Highspeed connection.
	 * @note Disabling this discards all highspeed related configuration.
	 */
	bool enable_highspeed_link;

	/**
	 * Event recording config, defaults to active recording.
	 */
	haldls::vx::EventRecordingConfig event_recording;

	/** Memory timing settings. */
	lola::vx::v3::MemoryTiming memory_timing;

	/** Select internal bias currents for synapses. */
	haldls::vx::SynapseBiasSelection synapse_bias_selection;

	/**
	 * Select whether the CapMem should be enabled.
	 * @note The CapMem initialization requires highspeed access.
	 */
	bool enable_capmem;

	/** Generate usable reference current for the CapMem. */
	haldls::vx::v3::ReferenceGeneratorConfig reference_generator_config;

	/** Initialize the CapMem with usable default values. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::CapMemBlockConfig,
	    halco::hicann_dls::vx::v3::CapMemBlockConfigOnDLS>
	    capmem_block_config_type GENPYBIND(visible);
	capmem_block_config_type capmem_block_config;

	/** Set initial CapMem config.
	 * By default, a value of zero is written to all cells. */
	typedef halco::common::
	    typed_array<haldls::vx::v3::CapMemBlock, halco::hicann_dls::vx::v3::CapMemBlockOnDLS>
	        capmem_block_type GENPYBIND(opaque(false));
	capmem_block_type capmem_config;

	typedef hate::Nil Result;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, InitGenerator const& sequence) SYMBOL_VISIBLE;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<InitGenerator>(InitGenerator const&);
};

} // namespace detail

/**
 * Generator for initialization of the chip up to digital communication.
 *
 * Uses the default InitGenerator() to establish digital communication to the chip.
 */
class DigitalInit : public detail::InitGenerator
{
public:
	/** Default constructor. */
	DigitalInit() SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<DigitalInit>(DigitalInit const&);
};

/**
 * Generator for initialization required for typical experiments.
 * Uses the InitGenerator() to establish digital communication to the chip, and
 * further initializes the CapMem in a working state and selects internal bias currents for
 * synapses.
 */
class ExperimentInit : public detail::InitGenerator
{
public:
	/** Default constructor. */
	ExperimentInit() SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef PlaybackProgramBuilder Builder;

	/** Set common neuron backend with clocks enabled.
	 * If clocks are disabled, it may behave strangely. */
	typedef halco::common::typed_array<
	    haldls::vx::CommonNeuronBackendConfig,
	    halco::hicann_dls::vx::v3::CommonNeuronBackendConfigOnDLS>
	    common_neuron_backend_config_type GENPYBIND(visible);
	common_neuron_backend_config_type common_neuron_backend_config;

	/** Set ColumnCorrelationQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::ColumnCorrelationQuad,
	    halco::hicann_dls::vx::v3::ColumnCorrelationQuadOnDLS>
	    column_correlation_quad_type GENPYBIND(opaque(false));
	column_correlation_quad_type column_correlation_quad_config;

	/** Set ColumnCurrentQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::ColumnCurrentQuad,
	    halco::hicann_dls::vx::v3::ColumnCurrentQuadOnDLS>
	    column_current_quad_type GENPYBIND(opaque(false));
	column_current_quad_type column_current_quad_config;

	/** Set common correlation config. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::CommonCorrelationConfig,
	    halco::hicann_dls::vx::v3::CommonCorrelationConfigOnDLS>
	    common_correlation_config_type GENPYBIND(opaque(false));
	common_correlation_config_type common_correlation_config;

	typedef detail::InitGenerator::Result Result;

private:
	friend auto stadls::vx::generate<ExperimentInit>(ExperimentInit const&);

	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;
};

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
namespace detail {

struct GENPYBIND(expose_as(ExperimentInit), inline_base("*ExperimentInit*")) PyExperimentInit
    : public ExperimentInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<ExperimentInit>(*this));
	}
};

// TODO: we can't use the alias above, cf. https://github.com/kljohann/genpybind/issues/32
struct GENPYBIND(expose_as(DigitalInit), inline_base("*DigitalInit*")) PyDigitalInit
    : public DigitalInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<DigitalInit>(*this));
	}
};

} // namespace detail
#endif // defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::ExperimentInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::DigitalInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

} // namespace stadls::vx::v3
