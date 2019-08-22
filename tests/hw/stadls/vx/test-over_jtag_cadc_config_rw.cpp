#include <gtest/gtest.h>

#include "fisch/vx/constants.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable over JTAG omnibus connection and write and read the CADC config for verification.
 */
TEST(CADCConfig, WROverJTAG)
{
	PlaybackProgramBuilder builder;

	// Chip reset
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// JTAG init
	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	// PLL init, reconfiguration needed to slow down PPU Omnibus tree to a working state
	ADPLL adpll_config;
	for (auto adpll : iter_all<ADPLLOnDLS>()) {
		builder.write(adpll, adpll_config, Backend::JTAGPLLRegister);
	}

	PLLClockOutputBlock config;
	builder.write(PLLClockOutputBlockOnDLS(), config, Backend::JTAGPLLRegister);

	// Wait for PLL and Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(1000 * fisch::vx::fpga_clock_cycles_per_us));
	builder.write(TimerOnDLS(), Timer());

	// Write unique configs
	std::vector<CADCConfig> configs;
	for (auto coord : iter_all<CADCConfigOnDLS>()) {
		CADCConfig config;
		config.set_enable(coord);
		config.set_reset_wait(
		    draw_ranged_non_default_value<CADCConfig::ResetWait>(config.get_reset_wait()));
		config.set_dead_time(
		    draw_ranged_non_default_value<CADCConfig::DeadTime>(config.get_dead_time()));
		builder.write(coord, config, Backend::OmnibusChipOverJTAG);
		configs.push_back(config);
	}

	// Read configs with Omnibus over JTAG backend
	std::vector<PlaybackProgram::ContainerTicket<CADCConfig>> responses;
	for (auto coord : iter_all<CADCConfigOnDLS>()) {
		responses.push_back(builder.read(coord, Backend::OmnibusChipOverJTAG));
	}

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto coord : iter_all<CADCConfigOnDLS>()) {
		EXPECT_TRUE(responses.at(coord).valid());
		EXPECT_EQ(responses.at(coord).get(), configs.at(coord));
	}
}
