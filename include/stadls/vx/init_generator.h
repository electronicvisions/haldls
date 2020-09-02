#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/synapse_driver.h"
#include "haldls/vx/systime.h"
#include "haldls/vx/v1/capmem.h"
#include "hate/nil.h"
#include "hate/visibility.h"
#include "lola/vx/dac.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

namespace stadls::vx /* no genpybind tag */ {

namespace detail {

template <typename, typename>
class InitGenerator;

template <typename T, typename C>
std::ostream& operator<<(std::ostream& os, InitGenerator<T, C> const& builder) SYMBOL_VISIBLE;

/**
 * Generator for an initialization of the xBoard and the chip up to
 * allowing access to all digital settings and systime annotation of response data.
 * If desired, the highspeed link is enabled (defaults to true).
 * If desired, the CapMem is enabled along with its reference generator and
 * the selection of internal synapse bias currents (defaults to false).
 */
template <typename BuilderType, typename Coordinates>
class InitGenerator
{
public:
	/** Default constructor. */
	InitGenerator() SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef BuilderType Builder;

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
	typedef halco::common::typed_array<haldls::vx::ADPLL, typename Coordinates::ADPLLOnDLS>
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
		    typed_array<haldls::vx::PhyConfigFPGA, typename Coordinates::PhyConfigFPGAOnDLS>
		        phy_configs_fpga_type GENPYBIND(opaque(false));
		phy_configs_fpga_type phy_configs_fpga;

		/** Highspeed-link PHY settings for the chip side. */
		typedef halco::common::
		    typed_array<haldls::vx::PhyConfigChip, typename Coordinates::PhyConfigChipOnDLS>
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
	    typed_array<haldls::vx::CommonSynramConfig, typename Coordinates::CommonSynramConfigOnDLS>
	        common_synram_config_type GENPYBIND(opaque(false));
	common_synram_config_type common_synram_config;

	/** CADC offset SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::CADCOffsetSRAMTimingConfig,
	    typename Coordinates::CADCOffsetSRAMTimingConfigOnDLS>
	    cadc_offset_sram_timing_config_type GENPYBIND(opaque(false));
	cadc_offset_sram_timing_config_type cadc_offset_sram_timing_config;

	/** Synapse driver SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::SynapseDriverSRAMTimingConfig,
	    typename Coordinates::SynapseDriverSRAMTimingConfigOnDLS>
	    synapse_driver_sram_timing_config_type GENPYBIND(opaque(false));
	synapse_driver_sram_timing_config_type synapse_driver_sram_timing_config;

	/** Neuron SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::NeuronSRAMTimingConfig,
	    typename Coordinates::NeuronSRAMTimingConfigOnDLS>
	    neuron_sram_timing_config_type GENPYBIND(opaque(false));
	neuron_sram_timing_config_type neuron_sram_timing_config;

	/** Neuron backend SRAM timing setting. */
	typedef halco::common::typed_array<
	    haldls::vx::NeuronBackendSRAMTimingConfig,
	    typename Coordinates::NeuronBackendSRAMTimingConfigOnDLS>
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
	haldls::vx::ReferenceGeneratorConfig reference_generator_config;

	/** Initialize the CapMem with usable default values. */
	typedef halco::common::typed_array<
	    haldls::vx::CapMemBlockConfig<Coordinates>,
	    typename Coordinates::CapMemBlockConfigOnDLS>
	    capmem_block_config_type GENPYBIND(opaque(false));
	capmem_block_config_type capmem_block_config;

	typedef hate::Nil Result;

	template <typename T, typename C>
	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, InitGenerator<T, C> const& sequence);

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<BuilderType, Result> generate() const SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<InitGenerator>(InitGenerator const&);
};

} // namespace detail

} // namespace stadls::vx::detail

// clang-format off
// TODO: otherwise => "error: ambiguous template instantiation for
//                     'struct halco::common::detail::limits<halco::hicann_dls::vx::ColumnCorrelationQuadOnDLS, void>'"
// clang-format on
namespace halco::common {
template struct halco::common::typed_array<
    haldls::vx::ColumnCorrelationQuad,
    halco::hicann_dls::vx::ColumnCorrelationQuadOnDLS>;
template struct halco::common::
    typed_array<haldls::vx::ColumnCurrentQuad, halco::hicann_dls::vx::ColumnCurrentQuadOnDLS>;
} // namespace halco::common

namespace stadls::vx /* no genpybind tag */ {

/**
 * Generator for initialization required for typical experiments.
 * Uses the InitGenerator() to establish digital communication to the chip, and
 * further initializes the CapMem in a working state and selects internal bias currents for
 * synapses.
 */
template <typename BuilderType, typename Coordinates>
class ExperimentInit : public detail::InitGenerator<BuilderType, Coordinates>
{
public:
	/** Default constructor. */
	ExperimentInit() SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef BuilderType Builder;

	/** Set common neuron backend with clocks enabled.
	 * If clocks are disabled, it may behave strangely. */
	typedef halco::common::typed_array<
	    haldls::vx::CommonNeuronBackendConfig,
	    typename Coordinates::CommonNeuronBackendConfigOnDLS>
	    common_neuron_backend_config_type GENPYBIND(opaque(false));
	common_neuron_backend_config_type common_neuron_backend_config;

	/** Set ColumnCorrelationQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::ColumnCorrelationQuad,
	    typename Coordinates::ColumnCorrelationQuadOnDLS>
	    column_correlation_quad_type GENPYBIND(opaque(false));
	column_correlation_quad_type column_correlation_quad_config;

	/** Set ColumnCurrentQuad/Switch connections. */
	typedef halco::common::
	    typed_array<haldls::vx::ColumnCurrentQuad, typename Coordinates::ColumnCurrentQuadOnDLS>
	        column_current_quad_type GENPYBIND(opaque(false));
	column_current_quad_type column_current_quad_config;

	/** Set initial CapMem config.
	 * By default, a value of zero is written to all cells. */
	typedef halco::common::
	    typed_array<haldls::vx::CapMemBlock<Coordinates>, typename Coordinates::CapMemBlockOnDLS>
	        capmem_block_type GENPYBIND(opaque(false));
	capmem_block_type capmem_config;

	typedef typename detail::InitGenerator<BuilderType, Coordinates>::Result Result;

private:
	friend auto stadls::vx::generate<ExperimentInit>(ExperimentInit const&);

	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<BuilderType, Result> generate() const SYMBOL_VISIBLE;
};


/**
 * Generator for initialization of the chip up to digital communication.
 *
 * Uses the default InitGenerator() to establish digital communication to the chip.
 */
template <typename BuilderType, typename Coordinates>
class DigitalInit : public detail::InitGenerator<BuilderType, Coordinates>
{
public:
	/** Default constructor. */
	DigitalInit() SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<DigitalInit>(DigitalInit const&);
};

} // namespace stadls::vx
