#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/synapse.h"
#include "hate/nil.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

namespace detail {
/**
 * Generator for an initialization of the xBoard and the chip up to
 * allowing access to all digital settings and systime annotation of response data.
 * If desired, the highspeed link is enabled (defaults to true).
 * If desired, the CapMem is enabled along with its reference generator and
 * the selection of internal synapse bias currents (defaults to false).
 */
class GENPYBIND(expose_as(_InitGenerator)) InitGenerator
{
public:
	/** Default constructor. */
	InitGenerator() SYMBOL_VISIBLE;

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
	typedef halco::common::typed_array<haldls::vx::ADPLL, halco::hicann_dls::vx::ADPLLOnDLS>
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
		    typed_array<haldls::vx::PhyConfigFPGA, halco::hicann_dls::vx::PhyConfigFPGAOnDLS>
		        phy_configs_fpga_type GENPYBIND(opaque(false));
		phy_configs_fpga_type phy_configs_fpga;

		/** Highspeed-link PHY settings for the chip side. */
		typedef halco::common::
		    typed_array<haldls::vx::PhyConfigChip, halco::hicann_dls::vx::PhyConfigChipOnDLS>
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
	typedef halco::common::
	    typed_array<haldls::vx::CommonSynramConfig, halco::hicann_dls::vx::CommonSynramConfigOnDLS>
	        common_synram_config_type GENPYBIND(opaque(false));
	common_synram_config_type common_synram_config;

	/** Select internal bias currents for synapses. */
	haldls::vx::SynapseBiasSelection synapse_bias_selection;

	/**
	 * Select whether the CapMem should be enabled.
	 * @note The CapMem initialization requires highspeed access.
	 */
	bool enable_capmem;

	/** Generate usable reference current for the CapMem. */
	haldls::vx::ReferenceGeneratorConfig reference_generator_config;

	/** Initialize the CapMem with usable default values. */
	typedef halco::common::
	    typed_array<haldls::vx::CapMemBlockConfig, halco::hicann_dls::vx::CapMemBlockConfigOnDLS>
	        capmem_block_config_type GENPYBIND(opaque(false));
	capmem_block_config_type capmem_block_config;

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

	/** Set common neuron backend with clocks enabled.
	 * If clocks are disabled, it may behave strangely. */
	typedef halco::common::typed_array<
	    haldls::vx::CommonNeuronBackendConfig,
	    halco::hicann_dls::vx::CommonNeuronBackendConfigOnDLS>
	    common_neuron_backend_config_type GENPYBIND(opaque(false));
	common_neuron_backend_config_type common_neuron_backend_config;

	/** Set ColumnCorrelationQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::ColumnCorrelationQuad,
	    halco::hicann_dls::vx::ColumnCorrelationQuadOnDLS>
	    column_correlation_quad_type GENPYBIND(opaque(false));
	column_correlation_quad_type column_correlation_quad_config;

	/** Set ColumnCurrentQuad/Switch connections. */
	typedef halco::common::
	    typed_array<haldls::vx::ColumnCurrentQuad, halco::hicann_dls::vx::ColumnCurrentQuadOnDLS>
	        column_current_quad_type GENPYBIND(opaque(false));
	column_current_quad_type column_current_quad_config;

private:
	friend auto stadls::vx::generate<ExperimentInit>(ExperimentInit const&);

	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;
};


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

namespace detail {

struct GENPYBIND(expose_as(ExperimentInit), inline_base("*ExperimentInit*")) PyExperimentInit
    : public ExperimentInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return detail::py_generate_impl(static_cast<ExperimentInit>(*this));
	}
};

struct GENPYBIND(expose_as(DigitalInit), inline_base("*DigitalInit*")) PyDigitalInit
    : public DigitalInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return detail::py_generate_impl(static_cast<DigitalInit>(*this));
	}
};

} // namespace detail

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::ExperimentInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::DigitalInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

} // namespace stadls::vx
