#pragma once
#include "haldls/vx/jtag.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

inline void insert_highspeed_init(stadls::vx::PlaybackProgramBuilder& builder)
{
	using namespace halco::common;
	using namespace halco::hicann_dls::vx;
	using namespace haldls::vx;

	// Chip reset
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// JTAG init
	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	// PLL init, reconfiguration needed to slow down PPU to a working state
	ADPLL adpll_config;
	for (auto adpll : iter_all<ADPLLOnDLS>()) {
		builder.write(adpll, adpll_config, Backend::JTAGPLLRegister);
	}

	PLLClockOutputBlock config;
	builder.write(PLLClockOutputBlockOnDLS(), config, Backend::JTAGPLLRegister);

	// Wait for PLL and Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(100 * Timer::Value::fpga_clock_cycles_per_us));
	builder.write(TimerOnDLS(), Timer());

	// Configure all FPGA-side Phys
	for (auto phy : iter_all<PhyConfigFPGAOnDLS>()) {
		builder.write(phy, PhyConfigFPGA());
	}

	// Enable all FPGA-side Phys
	builder.write(CommonPhyConfigFPGAOnDLS(), CommonPhyConfigFPGA());

	// Configure all Chip-side Phys
	for (auto phy : iter_all<PhyConfigChipOnDLS>()) {
		builder.write(phy, PhyConfigChip());
	}

	// Enable all Chip-side Phys
	builder.write(CommonPhyConfigChipOnDLS(), CommonPhyConfigChip());

	// wait until highspeed is up (omnibus clock lock + phy config write over JTAG)
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(80 * Timer::Value::fpga_clock_cycles_per_us));

	builder.write(TimerOnDLS(), Timer());
}
