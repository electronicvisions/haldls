#include "stadls/vx/v3/init_generator.h"

#include "haldls/vx/constants.h"
#include "hwdb4cpp/hwdb4cpp.h"
#include "hxcomm/common/hwdb_entry.h"
#include <variant>

namespace stadls::vx::v3 {

ASICAdapterBoardInit::~ASICAdapterBoardInit() {}

PlaybackGeneratorReturn<typename ASICAdapterBoardInit::Result> ASICAdapterBoardInit::generate()
    const
{
	return {};
}

std::unique_ptr<ASICAdapterBoardInit> ASICAdapterBoardInit::copy() const
{
	return std::make_unique<ASICAdapterBoardInit>(*this);
}


CubeASICAdapterBoardInit::CubeASICAdapterBoardInit() :
    shift_register(),
    dac_control_block(),
    dac_channel_block(lola::vx::DACChannelBlock::default_ldo_2)
{
}

PlaybackGeneratorReturn<typename CubeASICAdapterBoardInit::Result>
CubeASICAdapterBoardInit::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v3;

	auto [builder, _] = ASICAdapterBoardInit::generate();

	// Set shift register values
	builder.write(ShiftRegisterOnBoard(), shift_register);

	// Set DAC on xBoard channel values
	builder.write(DACChannelBlockOnBoard(), dac_channel_block);

	// Set DAC on xBoard enable values
	builder.write(DACControlBlockOnBoard(), dac_control_block);

	// Wait until DAC config is set
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), xboard_dac_settling_duration);

	return {std::move(builder), Result{}};
}

std::unique_ptr<ASICAdapterBoardInit> CubeASICAdapterBoardInit::copy() const
{
	return std::make_unique<CubeASICAdapterBoardInit>(*this);
}


JboaASICAdapterBoardInit::JboaASICAdapterBoardInit() :
    multiplexer(haldls::vx::TCA9546ChannelConfig::default_mux),
    io_expander(), // default constructor is sufficient
    // 3 potentiometers with 2 channels each with different register addresses
    DigitalPotiArray(
        {haldls::vx::AD5252ChannelConfig::default_pot_25,
         haldls::vx::AD5252ChannelConfig::default_pot_12,
         haldls::vx::AD5252ChannelConfig::default_pot_12,
         haldls::vx::AD5252ChannelConfig::default_pot_12,
         haldls::vx::AD5252ChannelConfig::default_pot_12,
         haldls::vx::AD5252ChannelConfig::default_pot_25})
{
}

PlaybackGeneratorReturn<typename JboaASICAdapterBoardInit::Result>
JboaASICAdapterBoardInit::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v3;
	using namespace halco::common;

	auto [builder, _] = ASICAdapterBoardInit::generate();

	// Configure multiplexer
	builder.write(TCA9546OnBoard(), multiplexer);

	// Configure potentiometers
	for (auto coord : iter_all<AD5252ChannelConfigOnBoard>()) {
		builder.write(coord, DigitalPotiArray[coord]);
	}

	// Configure pins of input/output expander as outputs
	builder.write(TCA9554ConfigOnBoard(), io_expander);

	// Wait until DAC config is set
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), jboa_dac_settling_duration);

	return {std::move(builder), Result{}};
}

std::unique_ptr<ASICAdapterBoardInit> JboaASICAdapterBoardInit::copy() const
{
	return std::make_unique<JboaASICAdapterBoardInit>(*this);
}


ChipInit::ChipInit() :
    jtag_clock_scaler(),
    pll_clock_output_block(),
    adplls(),
    highspeed_link(),
    enable_highspeed_link(true),
    event_recording(),
    memory_timing(),
    synapse_bias_selection(),
    enable_capmem(false),
    reference_generator_config(),
    capmem_block_config(),
    capmem_config()
{
	event_recording.set_enable_event_recording(true);
}

ChipInit::HighspeedLink::HighspeedLink() :
    common_phy_config_fpga(),
    common_phy_config_chip(),
    phy_configs_fpga(),
    phy_configs_chip(),
    systime_sync_base(),
    enable_systime(true)
{}

PlaybackGeneratorReturn<typename ChipInit::Result> ChipInit::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v3;
	using namespace halco::common;

	ChipInit::Builder builder;

	// Reset chip
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.block_until(TimerOnDLS(), chip_reset_high_duration);
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.write(TimerOnDLS(), Timer());
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

	builder.write(EventRecordingConfigOnFPGA(), event_recording);

	if (enable_capmem) {
		// Require highspeed to be turned on in chip init
		if (enable_highspeed_link == false) {
			throw std::runtime_error("Highspeed link needs to be enabled to configure CapMem.");
		}

		// Enable internal synapse bias currents
		builder.write(SynapseBiasSelectionOnDLS(), synapse_bias_selection);

		// Generate usable reference current for the CapMem
		haldls::vx::v3::ReferenceGeneratorConfig config_with_reset = reference_generator_config;
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
		for (auto coord : iter_all<halco::hicann_dls::vx::v3::CapMemBlockOnDLS>()) {
			builder.write(coord, capmem_config[coord]);
		}
	}

	// Set memory timing configs
	builder.write(
	    MemoryTimingOnDLS(), memory_timing,
	    enable_highspeed_link ? Backend::Omnibus : Backend::OmnibusChipOverJTAG);

	// Block until omnibus or JTAG is idle
	builder.block_until(BarrierOnFPGA(), enable_highspeed_link ? Barrier::omnibus : Barrier::jtag);

	return {std::move(builder), Result{}};
}

namespace detail {

InitGenerator::InitGenerator(hxcomm::HwdbEntry const& hwdb_entry) :
    instruction_timeout(),
    enable_asic_adapter_board(true),
    chip(),
    enable_chip(true),
    m_asic_adapter_board(std::make_unique<CubeASICAdapterBoardInit>())
{
	if (std::holds_alternative<hwdb4cpp::JboaSetupEntry>(hwdb_entry)) {
		throw std::logic_error("jBOA adapter board init not implemented yet.");
	}
}

InitGenerator::InitGenerator(InitGenerator const& other) :
    instruction_timeout(other.instruction_timeout),
    enable_asic_adapter_board(other.enable_asic_adapter_board),
    chip(other.chip),
    enable_chip(other.enable_chip),
    m_asic_adapter_board(other.m_asic_adapter_board ? other.m_asic_adapter_board->copy() : nullptr)
{
}

InitGenerator& InitGenerator::operator=(InitGenerator const& other)
{
	if (&other != this) {
		instruction_timeout = other.instruction_timeout;
		enable_asic_adapter_board = other.enable_asic_adapter_board;
		chip = other.chip;
		enable_chip = other.enable_chip;
		m_asic_adapter_board =
		    other.m_asic_adapter_board ? other.m_asic_adapter_board->copy() : nullptr;
	}
	return *this;
}


ASICAdapterBoardInit const& InitGenerator::get_asic_adapter_board() const
{
	if (!m_asic_adapter_board) {
		throw std::logic_error("Unexpected access to moved-from object.");
	}
	return *m_asic_adapter_board;
}

void InitGenerator::set_asic_adapter_board(ASICAdapterBoardInit const& value)
{
	m_asic_adapter_board = value.copy();
}

PlaybackGeneratorReturn<typename InitGenerator::Result> InitGenerator::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v3;

	InitGenerator::Builder builder;

	// Configure playback instruction timeout
	builder.write(InstructionTimeoutConfigOnFPGA(), instruction_timeout);

	if (enable_asic_adapter_board) {
		builder.merge_back(get_asic_adapter_board().generate().builder);
	}

	if (enable_chip) {
		auto [chip_builder, _] = stadls::vx::generate(chip);
		builder.merge_back(chip_builder);
	}

	return {std::move(builder), Result{}};
}

std::ostream& operator<<(std::ostream& os, InitGenerator const&)
{
	return os;
}

} // namespace detail


DigitalInit::DigitalInit(hxcomm::HwdbEntry const& hwdb_entry) : detail::InitGenerator(hwdb_entry) {}

ExperimentInit::ExperimentInit(hxcomm::HwdbEntry const& hwdb_entry) :
    detail::InitGenerator(hwdb_entry),
    common_neuron_backend_config(),
    column_correlation_quad_config(),
    column_current_quad_config(),
    common_correlation_config()
{
	this->chip.enable_capmem = true;
}

PlaybackGeneratorReturn<ExperimentInit::Result> ExperimentInit::generate() const
{
	using namespace haldls::vx;
	using namespace halco::hicann_dls::vx::v3;
	using namespace halco::common;

	auto [builder, res] = InitGenerator::generate();

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

	// Write common correlation config
	for (auto coord : iter_all<CommonCorrelationConfigOnDLS>()) {
		builder.write(coord, common_correlation_config[coord]);
	}

	return {std::move(builder), res};
}

} // namespace stadls::vx::v3
