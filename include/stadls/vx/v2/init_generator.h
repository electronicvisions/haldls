#pragma once
#include "hate/visibility.h"
#include "stadls/vx/v2/playback_generator.h"
#include "stadls/vx/v2/playback_program_builder.h"

namespace stadls::vx::v2 GENPYBIND_TAG_STADLS_VX_V2 {

namespace detail {

class GENPYBIND(expose_as(_InitGenerator)) InitGenerator
{
public:
	/** Default constructor. */
	InitGenerator() SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef v2::PlaybackProgramBuilder Builder;

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
	haldls::vx::PLLClockOutputBlock pll_clock_output_block;

	/** ADPLL setting. */
	typedef halco::common::typed_array<haldls::vx::ADPLL, halco::hicann_dls::vx::v2::ADPLLOnDLS>
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
		    typed_array<haldls::vx::PhyConfigFPGA, halco::hicann_dls::vx::v2::PhyConfigFPGAOnDLS>
		        phy_configs_fpga_type GENPYBIND(opaque(false));
		phy_configs_fpga_type phy_configs_fpga;

		/** Highspeed-link PHY settings for the chip side. */
		typedef halco::common::
		    typed_array<haldls::vx::PhyConfigChip, halco::hicann_dls::vx::v2::PhyConfigChipOnDLS>
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

	/** Synram SRAM setting. */
	typedef halco::common::typed_array<
	    haldls::vx::CommonSynramConfig,
	    halco::hicann_dls::vx::v2::CommonSynramConfigOnDLS>
	    common_synram_config_type GENPYBIND(opaque(false));
	common_synram_config_type common_synram_config;

	/** CADC offset SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::CADCOffsetSRAMTimingConfig,
	    halco::hicann_dls::vx::v2::CADCOffsetSRAMTimingConfigOnDLS>
	    cadc_offset_sram_timing_config_type GENPYBIND(opaque(false));
	cadc_offset_sram_timing_config_type cadc_offset_sram_timing_config;

	/** Synapse driver SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::SynapseDriverSRAMTimingConfig,
	    halco::hicann_dls::vx::v2::SynapseDriverSRAMTimingConfigOnDLS>
	    synapse_driver_sram_timing_config_type GENPYBIND(opaque(false));
	synapse_driver_sram_timing_config_type synapse_driver_sram_timing_config;

	/** Neuron SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::NeuronSRAMTimingConfig,
	    halco::hicann_dls::vx::v2::NeuronSRAMTimingConfigOnDLS>
	    neuron_sram_timing_config_type GENPYBIND(opaque(false));
	neuron_sram_timing_config_type neuron_sram_timing_config;

	/** Neuron backend SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::NeuronBackendSRAMTimingConfig,
	    halco::hicann_dls::vx::v2::NeuronBackendSRAMTimingConfigOnDLS>
	    neuron_backend_sram_timing_config_type GENPYBIND(opaque(false));
	neuron_backend_sram_timing_config_type neuron_backend_sram_timing_config;

	/** Select internal bias currents for synapses. */
	haldls::vx::SynapseBiasSelection synapse_bias_selection;

	/**
	 * Select whether the CapMem should be enabled.
	 * @note The CapMem initialization requires highspeed access.
	 */
	bool enable_capmem;

	/** Generate usable reference current for the CapMem. */
	haldls::vx::v2::ReferenceGeneratorConfig reference_generator_config;

	/** Initialize the CapMem with usable default values. */
	typedef halco::common::typed_array<
	    haldls::vx::v2::CapMemBlockConfig,
	    halco::hicann_dls::vx::v2::CapMemBlockConfigOnDLS>
	    capmem_block_config_type GENPYBIND(opaque(false));
	capmem_block_config_type capmem_block_config;

	/** Set initial CapMem config.
	 * By default, a value of zero is written to all cells. */
	typedef halco::common::
	    typed_array<haldls::vx::v2::CapMemBlock, halco::hicann_dls::vx::v2::CapMemBlockOnDLS>
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
	    halco::hicann_dls::vx::v2::CommonNeuronBackendConfigOnDLS>
	    common_neuron_backend_config_type GENPYBIND(opaque(false));
	common_neuron_backend_config_type common_neuron_backend_config;

	/** Set ColumnCorrelationQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v2::ColumnCorrelationQuad,
	    halco::hicann_dls::vx::v2::ColumnCorrelationQuadOnDLS>
	    column_correlation_quad_type GENPYBIND(opaque(false));
	column_correlation_quad_type column_correlation_quad_config;

	/** Set ColumnCurrentQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v2::ColumnCurrentQuad,
	    halco::hicann_dls::vx::v2::ColumnCurrentQuadOnDLS>
	    column_current_quad_type GENPYBIND(opaque(false));
	column_current_quad_type column_current_quad_config;

	typedef detail::InitGenerator::Result Result;

private:
	friend auto stadls::vx::generate<ExperimentInit>(ExperimentInit const&);

	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;
};

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

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v2::ExperimentInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v2::DigitalInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

} // namespace stadls::vx::v2
