#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/spi.h"
#include "hate/nil.h"
#include "hate/visibility.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/playback_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

/**
 * Generator for a complete initialization of the xBoard, the highspeed link and the chip up to
 * allowing access to all digital settings and systime annotation of response data.
 */
class InitGenerator
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

	/** JTAG clock scaler setting. */
	haldls::vx::JTAGClockScaler jtag_clock_scaler;

	/** PLL clock output block setting. */
	haldls::vx::PLLClockOutputBlock pll_clock_output_block;

	/** ADPLL setting. */
	halco::common::typed_array<haldls::vx::ADPLL, halco::hicann_dls::vx::ADPLLOnDLS> adplls;

	struct GENPYBIND(visible) HighspeedLink
	{
		/** Default constructor. */
		HighspeedLink() SYMBOL_VISIBLE;

		/** Highspeed-link PHY enable values for the FPGA side. */
		haldls::vx::CommonPhyConfigFPGA common_phy_config_fpga;

		/** Highspeed-link PHY enable values for the chip side. */
		haldls::vx::CommonPhyConfigChip common_phy_config_chip;

		/** Highspeed-link PHY settings for the FPGA side. */
		halco::common::
		    typed_array<haldls::vx::PhyConfigFPGA, halco::hicann_dls::vx::PhyConfigFPGAOnDLS>
		        phy_configs_fpga;

		/** Highspeed-link PHY settings for the chip side. */
		halco::common::
		    typed_array<haldls::vx::PhyConfigChip, halco::hicann_dls::vx::PhyConfigChipOnDLS>
		        phy_configs_chip;

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
	halco::common::
	    typed_array<haldls::vx::CommonSynramConfig, halco::hicann_dls::vx::CommonSynramConfigOnDLS>
	        common_synram_config;

	typedef hate::Nil Result;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, InitGenerator const& sequence) SYMBOL_VISIBLE;

private:
	friend auto generate<InitGenerator>(InitGenerator const&);

	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;
};


namespace detail {

class GENPYBIND(expose_as(InitGenerator), inline_base("*InitGenerator*")) PyInitGenerator
    : public InitGenerator
    , public PlaybackGenerator
{
	virtual pybind11::tuple generate() const override
	{
		return detail::py_generate_impl(static_cast<InitGenerator>(*this));
	}
};

} // namespace detail

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::InitGenerator const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

} // namespace stadls::vx
