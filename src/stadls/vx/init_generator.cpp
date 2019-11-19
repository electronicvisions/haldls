#include <stdexcept>

#include "stadls/vx/init_generator.h"

#include "haldls/vx/capmem.h"
#include "haldls/vx/constants.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program_builder.h"

namespace stadls::vx {

namespace detail {

InitGenerator::InitGenerator() :
    shift_register(),
    dac_control_block(),
    dac_channel_block(lola::vx::DACChannelBlock::default_ldo_2),
    enable_xboard(true),
    jtag_clock_scaler(),
    pll_clock_output_block(),
    adplls(),
    highspeed_link(),
    enable_highspeed_link(true),
    common_synram_config(),
    synapse_bias_selection(),
    enable_capmem(false),
    reference_generator_config(),
    capmem_block_config()
{}

InitGenerator::HighspeedLink::HighspeedLink() :
    common_phy_config_fpga(),
    common_phy_config_chip(),
    phy_configs_fpga(),
    phy_configs_chip(),
    systime_sync_base(),
    enable_systime(true)
{}

PlaybackGeneratorReturn<InitGenerator::Result> InitGenerator::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;

	PlaybackProgramBuilder builder;

	if (enable_xboard) {
		// Set shift register values
		builder.write(ShiftRegisterOnBoard(), shift_register);

		// Set DAC on xBoard channel values
		builder.write(DACChannelBlockOnBoard(), dac_channel_block);

		// Set DAC on xBoard enable values
		builder.write(DACControlBlockOnBoard(), dac_control_block);

		// Wait until DAC config is set
		builder.write(TimerOnDLS(), Timer());
		builder.wait_until(TimerOnDLS(), xboard_dac_settling_duration);
	}

	// Reset chip
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), chip_reset_high_duration);
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), chip_reset_low_duration);

	// Reset JTAG TAP
	builder.write(JTAGClockScalerOnDLS(), jtag_clock_scaler);
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	// Init PLL
	for (auto coord : iter_all<ADPLLOnDLS>()) {
		builder.write(coord, adplls[coord], Backend::JTAGPLLRegister);
	}
	builder.write(PLLClockOutputBlockOnDLS(), pll_clock_output_block, Backend::JTAGPLLRegister);

	if (enable_highspeed_link) {
		// Set PHY configs on the FPGA side
		for (auto coord : iter_all<PhyConfigFPGAOnDLS>()) {
			builder.write(coord, highspeed_link.phy_configs_fpga[coord]);
		}

		// Set PHY configs on the chip side
		for (auto coord : iter_all<PhyConfigChipOnDLS>()) {
			builder.write(coord, highspeed_link.phy_configs_chip[coord]);
		}

		// Wait until PLL and Omnibus is up
		builder.wait_until(TimerOnDLS(), pll_and_omnibus_settling_duration);

		builder.write(CommonPhyConfigFPGAOnDLS(), highspeed_link.common_phy_config_fpga);
		builder.write(CommonPhyConfigChipOnDLS(), highspeed_link.common_phy_config_chip);

		// Wait until Highspeed is up
		builder.write(TimerOnDLS(), Timer());
		builder.wait_until(TimerOnDLS(), highspeed_initialization_duration);

		if (highspeed_link.enable_systime) {
			builder.write(
			    SystimeSyncBaseOnDLS(), highspeed_link.systime_sync_base, Backend::OmnibusChip);
			builder.write(SystimeSyncOnFPGA(), SystimeSync());

			// Wait until systime sync is complete
			builder.write(TimerOnDLS(), Timer());
			builder.wait_until(TimerOnDLS(), systime_sync_duration);
		}
	}

	if (enable_capmem) {
		// Require highspeed to be turned on in chip init
		if (enable_highspeed_link == false) {
			throw std::runtime_error("Highspeed link needs to be enabled to configure CapMem.");
		}

		// Enable internal synapse bias currents
		builder.write(SynapseBiasSelectionOnDLS(), synapse_bias_selection);

		// Generate usable reference current for the CapMem
		ReferenceGeneratorConfig config_with_reset = reference_generator_config;
		config_with_reset.set_enable_reset(true);
		builder.write(ReferenceGeneratorConfigOnDLS(), config_with_reset);
		builder.write(TimerOnDLS(), Timer());
		builder.wait_until(TimerOnDLS(), reference_generator_reset_duration);
		builder.write(ReferenceGeneratorConfigOnDLS(), reference_generator_config);

		// Set all CapMem cells to value zero
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			builder.write(coord, CapMemBlock());
		}

		// Initialize the CapMem with usable default values.
		for (auto coord : iter_all<CapMemBlockConfigOnDLS>()) {
			builder.write(coord, capmem_block_config[coord]);
		}
	}

	// Set Synram SRAM configs
	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		builder.write(coord, common_synram_config[coord], Backend::OmnibusChipOverJTAG);
	}

	return {std::move(builder), Result{}};
}

std::ostream& operator<<(std::ostream& os, InitGenerator const&)
{
	return os;
}

} // namespace detail


ExperimentInit::ExperimentInit() : detail::InitGenerator()
{
	enable_capmem = true;
}


DigitalInit::DigitalInit() : detail::InitGenerator() {}

} // namespace stadls::vx
