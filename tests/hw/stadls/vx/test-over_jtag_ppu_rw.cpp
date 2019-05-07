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
	builder.halt();
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program.impl());

	for (size_t i = 0; i < num_words; ++i) {
		EXPECT_TRUE(responses.at(i).valid());
		EXPECT_EQ(responses.at(i).get(), words.at(i));
	}
}
