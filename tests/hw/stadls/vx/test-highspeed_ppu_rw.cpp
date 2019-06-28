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
#include "test-init_helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;

/**
 * Enable highspeed omnibus connection and write and read all PPU memory words for verification.
 */
TEST(PPUMemoryWord, WRHighspeed)
{
	PlaybackProgramBuilder builder;

	insert_highspeed_init(builder);

	// Write all PPU memory words with highspeed backend
	std::vector<PPUMemoryWord> words;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		words.push_back(PPUMemoryWord(draw_ranged_non_default_value<PPUMemoryWord::Value>(0)));
		builder.write(word, words[word.toEnum()], Backend::OmnibusChip);
	}

	// Read all PPU memory words with highspeed backend
	std::vector<PlaybackProgram::ContainerTicket<PPUMemoryWord>> responses;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		responses.push_back(builder.read<PPUMemoryWord>(word, Backend::OmnibusChip));
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