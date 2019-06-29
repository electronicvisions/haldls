#include <gtest/gtest.h>

#include "fisch/vx/constants.h"
#include "fisch/vx/playback_executor.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;

/**
 * Enable over JTAG omnibus connection and write and read PPU memory words for verification.
 */
TEST(PPUMemoryWord, WROverJTAG)
{
	PlaybackProgramBuilder builder;

	// Chip reset
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(true));
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// JTAG init
	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	// PLL init, reconfiguration needed to slow down PPU to a working state
	ADPLL adpll_config;
	for (auto adpll : halco::common::iter_all<halco::hicann_dls::vx::ADPLLOnDLS>()) {
		builder.write(adpll, adpll_config, Backend::JTAGPLLRegister);
	}

	PLLClockOutputBlock config;
	builder.write(halco::hicann_dls::vx::PLLOnDLS(), config, Backend::JTAGPLLRegister);

	// Wait for PLL and Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(100 * fisch::vx::fpga_clock_cycles_per_us));
	builder.write<haldls::vx::Timer>(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());

	constexpr size_t num_words = 10;

	// Write PPU memory words with Omnibus over JTAG backend
	std::vector<PPUMemoryWord> words;
	for (size_t i = 0; i < num_words; ++i) {
		auto value = PPUMemoryWord(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));
		builder.write(PPUMemoryWordOnDLS(Enum(i)), value, Backend::OmnibusChipOverJTAG);
		words.push_back(value);
	}

	// Read PPU memory words with Omnibus over JTAG backend
	std::vector<PlaybackProgram::ContainerTicket<PPUMemoryWord>> responses;
	for (size_t i = 0; i < num_words; ++i) {
		responses.push_back(
		    builder.read<PPUMemoryWord>(PPUMemoryWordOnDLS(Enum(i)), Backend::OmnibusChipOverJTAG));
	}

	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program.impl());

	for (size_t i = 0; i < num_words; ++i) {
		EXPECT_TRUE(responses.at(i).valid());
		EXPECT_EQ(responses.at(i).get(), words.at(i));
	}
}

/**
 * Write and read control register
 */
TEST(PPUControlRegister, WROverJTAG)
{
	PlaybackProgramBuilder builder;

	// Chip reset
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(true));
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write<ResetChip>(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// JTAG init
	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	// PLL init, reconfiguration needed to slow down PPU to a working state
	ADPLL adpll_config;
	for (auto adpll : halco::common::iter_all<halco::hicann_dls::vx::ADPLLOnDLS>()) {
		builder.write(adpll, adpll_config, Backend::JTAGPLLRegister);
	}

	PLLClockOutputBlock config;
	builder.write(halco::hicann_dls::vx::PLLOnDLS(), config, Backend::JTAGPLLRegister);

	// Wait for PLL and Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(100 * fisch::vx::fpga_clock_cycles_per_us));
	builder.write<haldls::vx::Timer>(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());

	PPUControlRegister reg1, reg2;
	PPUOnDLS ppu_coord1(0);
	PPUOnDLS ppu_coord2(1);
	reg1.set_cache_controller_enable(true);
	reg1.set_inhibit_reset(true);
	reg1.set_force_clock_on(true);
	reg1.set_force_clock_off(false);

	reg2.set_cache_controller_enable(false);
	reg2.set_inhibit_reset(false);
	reg2.set_force_clock_on(false);
	reg2.set_force_clock_off(true);

	// Write and read back reg1
	builder.write(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord1), reg1,
	    Backend::OmnibusChipOverJTAG);
	builder.write(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord2), reg1,
	    Backend::OmnibusChipOverJTAG);
	std::vector<PlaybackProgram::ContainerTicket<PPUControlRegister>> reg1_tickets;
	reg1_tickets.push_back(builder.read<PPUControlRegister>(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord1),
	    Backend::OmnibusChipOverJTAG));
	reg1_tickets.push_back(builder.read<PPUControlRegister>(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord2),
	    Backend::OmnibusChipOverJTAG));


	// Write and read back reg2
	builder.write(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord1), reg2,
	    Backend::OmnibusChipOverJTAG);
	builder.write(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord2), reg2,
	    Backend::OmnibusChipOverJTAG);
	std::vector<PlaybackProgram::ContainerTicket<PPUControlRegister>> reg2_tickets;
	reg2_tickets.push_back(builder.read<PPUControlRegister>(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord1),
	    Backend::OmnibusChipOverJTAG));
	reg2_tickets.push_back(builder.read<PPUControlRegister>(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord2),
	    Backend::OmnibusChipOverJTAG));

	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program.impl());

	EXPECT_TRUE(reg1_tickets[0].valid());
	EXPECT_TRUE(reg1_tickets[1].valid());
	EXPECT_TRUE(reg1_tickets[0].get() == reg1);
	EXPECT_TRUE(reg1_tickets[1].get() == reg1);

	EXPECT_TRUE(reg2_tickets[0].valid());
	EXPECT_TRUE(reg2_tickets[1].valid());
	EXPECT_TRUE(reg2_tickets[0].get() == reg2);
	EXPECT_TRUE(reg2_tickets[1].get() == reg2);
}
