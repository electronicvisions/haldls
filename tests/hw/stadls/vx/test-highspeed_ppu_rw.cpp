#include <gtest/gtest.h>

#include "halco/common/typed_array.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
#include "test-helper.h"
#include "test-init_helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable highspeed omnibus connection and write and read all PPU memory words for verification.
 */
TEST(PPUMemoryWord, WRHighspeed)
{
	PlaybackProgramBuilder builder;

	insert_highspeed_init(builder);

	// Write all PPU memory words with highspeed backend
	typed_array<PPUMemoryWord, PPUMemoryWordOnDLS> words;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		words[word] = PPUMemoryWord(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));
		builder.write(word, words[word], Backend::OmnibusChip);
	}

	// Read all PPU memory words with highspeed backend
	std::vector<PlaybackProgram::ContainerTicket<PPUMemoryWord>> responses;
	for (auto word : iter_all<PPUMemoryWordOnDLS>()) {
		responses.push_back(builder.read(word, Backend::OmnibusChip));
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
