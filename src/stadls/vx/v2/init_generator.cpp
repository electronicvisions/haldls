#include "stadls/vx/v2/init_generator.h"

#include "haldls/vx/constants.h"

namespace stadls::vx::v2 {

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
    cadc_offset_sram_timing_config(),
    synapse_driver_sram_timing_config(),
    neuron_sram_timing_config(),
    neuron_backend_sram_timing_config(),
    synapse_bias_selection(),
    enable_capmem(false),
    reference_generator_config(),
    capmem_block_config(),
    capmem_config()
{}

InitGenerator::HighspeedLink::HighspeedLink() :
    common_phy_config_fpga(),
    common_phy_config_chip(),
    phy_configs_fpga(),
    phy_configs_chip(),
    systime_sync_base(),
    enable_systime(true)
{}

PlaybackGeneratorReturn<typename InitGenerator::Result> InitGenerator::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v2;
	using namespace halco::common;

	InitGenerator::Builder builder;

	if (enable_xboard) {
		// Set shift register values
		builder.write(ShiftRegisterOnBoard(), shift_register);

		// Set DAC on xBoard channel values
		builder.write(DACChannelBlockOnBoard(), dac_channel_block);

		// Set DAC on xBoard enable values
		builder.write(DACControlBlockOnBoard(), dac_control_block);

		// Wait until DAC config is set
		builder.write(TimerOnDLS(), Timer());
		builder.block_until(TimerOnDLS(), xboard_dac_settling_duration);
	}

	// Reset chip
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), chip_reset_high_duration);
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.block_until(TimerOnDLS(), chip_reset_low_duration);

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
		builder.block_until(TimerOnDLS(), pll_and_omnibus_settling_duration);

		builder.write(CommonPhyConfigFPGAOnDLS(), highspeed_link.common_phy_config_fpga);
		builder.write(CommonPhyConfigChipOnDLS(), highspeed_link.common_phy_config_chip);

		if (highspeed_link.enable_systime) {
			builder.write(
			    SystimeSyncBaseOnDLS(), highspeed_link.systime_sync_base, Backend::Omnibus);
			// Block until omnibus is idle -> HS-Link is up
			builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
			builder.write(SystimeSyncOnFPGA(), SystimeSync());

			// Block until systime sync is complete
			builder.block_until(BarrierOnFPGA(), Barrier::systime);
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
		haldls::vx::v2::ReferenceGeneratorConfig config_with_reset = reference_generator_config;
		config_with_reset.set_enable_reset(true);
		builder.write(ReferenceGeneratorConfigOnDLS(), config_with_reset);
		// Block until omnibus is idle
		builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
		builder.write(TimerOnDLS(), Timer());
		builder.block_until(TimerOnDLS(), reference_generator_reset_duration);
		builder.write(ReferenceGeneratorConfigOnDLS(), reference_generator_config);

		// Initialize the CapMem with usable default values.
		for (auto coord : iter_all<CapMemBlockConfigOnDLS>()) {
			builder.write(coord, capmem_block_config[coord]);
		}

		// Set all CapMem cells
		for (auto coord : iter_all<halco::hicann_dls::vx::v2::CapMemBlockOnDLS>()) {
			builder.write(coord, capmem_config[coord]);
		}
	}

	// Set Synram SRAM configs
	for (auto coord : iter_all<CommonSynramConfigOnDLS>()) {
		builder.write(
		    coord, common_synram_config[coord],
		    enable_highspeed_link ? Backend::Omnibus : Backend::OmnibusChipOverJTAG);
	}

	// Set CADC offset SRAM configs
	for (auto coord : iter_all<CADCOffsetSRAMTimingConfigOnDLS>()) {
		builder.write(
		    coord, cadc_offset_sram_timing_config[coord],
		    enable_highspeed_link ? Backend::Omnibus : Backend::OmnibusChipOverJTAG);
	}

	// Set synapse driver SRAM configs
	for (auto coord : iter_all<SynapseDriverSRAMTimingConfigOnDLS>()) {
		builder.write(
		    coord, synapse_driver_sram_timing_config[coord],
		    enable_highspeed_link ? Backend::Omnibus : Backend::OmnibusChipOverJTAG);
	}

	// Set neuron SRAM configs
	for (auto coord : iter_all<NeuronSRAMTimingConfigOnDLS>()) {
		builder.write(
		    coord, neuron_sram_timing_config[coord],
		    enable_highspeed_link ? Backend::Omnibus : Backend::OmnibusChipOverJTAG);
	}

	// Set neuron backend SRAM configs
	for (auto coord : iter_all<NeuronBackendSRAMTimingConfigOnDLS>()) {
		builder.write(
		    coord, neuron_backend_sram_timing_config[coord],
		    enable_highspeed_link ? Backend::Omnibus : Backend::OmnibusChipOverJTAG);
	}

	// Block until omnibus or JTAG is idle
	builder.block_until(BarrierOnFPGA(), enable_highspeed_link ? Barrier::omnibus : Barrier::jtag);

	return {std::move(builder), Result{}};
}

std::ostream& operator<<(std::ostream& os, InitGenerator const&)
{
	return os;
}

} // namespace detail


DigitalInit::DigitalInit() : detail::InitGenerator() {}

ExperimentInit::ExperimentInit() :
    detail::InitGenerator(),
    common_neuron_backend_config(),
    column_correlation_quad_config(),
    column_current_quad_config()
{
	this->enable_capmem = true;
	for (auto const coord :
	     halco::common::iter_all<halco::hicann_dls::vx::v2::CommonNeuronBackendConfigOnDLS>()) {
		common_neuron_backend_config[coord].set_enable_clocks(true);
	}
}

PlaybackGeneratorReturn<ExperimentInit::Result> ExperimentInit::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v2;
	using namespace halco::common;

	auto [builder, res] = stadls::vx::generate(*static_cast<detail::InitGenerator const*>(this));

	// Write common neuron backend config
	for (auto coord : iter_all<CommonNeuronBackendConfigOnDLS>()) {
		builder.write(coord, common_neuron_backend_config[coord]);
	}

	// Set column correlation quad config
	for (auto coord : iter_all<ColumnCorrelationQuadOnDLS>()) {
		builder.write(coord, column_correlation_quad_config[coord]);
	}

	// Set column current quad config
	for (auto coord : iter_all<ColumnCurrentQuadOnDLS>()) {
		builder.write(coord, column_current_quad_config[coord]);
	}

	return {std::move(builder), res};
}

} // namespace stadls::vx::v2
