#include <gtest/gtest.h>

#include "haldls/vx/jtag.h"
#include "haldls/vx/ppu.h"
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
 * Enable over JTAG omnibus connection and write and read PPU memory words for verification.
 */
TEST(PPUMemoryWord, WROverJTAG)
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

	// Write all PPU memory words with JTAG backend
	typed_array<PPUMemoryWord, PPUMemoryWordOnDLS> words;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		words[word] = PPUMemoryWord(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));
		builder.write(word, words[word], Backend::OmnibusChipOverJTAG);
	}

	// Read all PPU memory words with JTAG backend
	std::vector<PlaybackProgram::ContainerTicket<PPUMemoryWord>> responses;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		responses.push_back(builder.read(word, Backend::OmnibusChipOverJTAG));
	}

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		EXPECT_TRUE(responses[word.toEnum()].valid());
		EXPECT_EQ(responses[word.toEnum()].get(), words[word]);
	}
}

/**
 * Write and read control register
 */
TEST(PPUControlRegister, WROverJTAG)
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
	reg1_tickets.push_back(builder.read(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord1),
	    Backend::OmnibusChipOverJTAG));
	reg1_tickets.push_back(builder.read(
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
	reg2_tickets.push_back(builder.read(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord1),
	    Backend::OmnibusChipOverJTAG));
	reg2_tickets.push_back(builder.read(
	    PPUControlRegisterOnDLS(PPUControlRegisterOnPPU(), ppu_coord2),
	    Backend::OmnibusChipOverJTAG));

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	EXPECT_TRUE(reg1_tickets[0].valid());
	EXPECT_TRUE(reg1_tickets[1].valid());
	EXPECT_TRUE(reg1_tickets[0].get() == reg1);
	EXPECT_TRUE(reg1_tickets[1].get() == reg1);

	EXPECT_TRUE(reg2_tickets[0].valid());
	EXPECT_TRUE(reg2_tickets[1].valid());
	EXPECT_TRUE(reg2_tickets[0].get() == reg2);
	EXPECT_TRUE(reg2_tickets[1].get() == reg2);
}
