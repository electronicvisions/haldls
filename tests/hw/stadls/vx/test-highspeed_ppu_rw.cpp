#include <gtest/gtest.h>

#include "fisch/vx/constants.h"
#include "fisch/vx/playback_executor.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "hxcomm/vx/arqconnection.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;

/**
 * Enable highspeed omnibus connection and write and read all PPU memory words for verification.
 */
TEST(PPUMemoryWord, WRHighspeed)
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

	// Configure all FPGA-side Phys
	for (auto phy : iter_all<PhyConfigFPGAOnDLS>()) {
		builder.write(phy, PhyConfigFPGA());
	}

	// Enable all FPGA-side Phys
	builder.write(CommonPhyConfigFPGAOnDLS(), CommonPhyConfigFPGA());

	// wait 22us for the omnibus clock to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(22 * fisch::vx::fpga_clock_cycles_per_us));

	// Configure all Chip-side Phys
	for (auto phy : iter_all<PhyConfigChipOnDLS>()) {
		builder.write(phy, PhyConfigChip());
	}

	// Enable all Chip-side Phys
	builder.write(CommonPhyConfigChipOnDLS(), CommonPhyConfigChip());

	// wait until highspeed is up (omnibus clock lock + phy config write over JTAG)
	builder.wait_until(TimerOnDLS(), Timer::Value(80 * fisch::vx::fpga_clock_cycles_per_us));

	// Write all PPU memory words with highspeed backend
	std::vector<PPUMemoryWord> words;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		words.push_back(PPUMemoryWord(draw_ranged_non_default_value<PPUMemoryWord::Value>(0)));
		builder.write(word, words[word.toEnum()], Backend::Omnibus);
	}

	// Read all PPU memory words with highspeed backend
	std::vector<PlaybackProgram::ContainerTicket<PPUMemoryWord>> responses;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		responses.push_back(builder.read<PPUMemoryWord>(word, Backend::Omnibus));
	}

	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	builder.halt();
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program.impl());

	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		EXPECT_TRUE(responses[word.toEnum()].valid());
		EXPECT_EQ(responses[word.toEnum()].get(), words[word.toEnum()]);
	}
}
