#include <gtest/gtest.h>

#include "halco/common/typed_array.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/phy.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/run.h"

#include "connection.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

extern std::optional<size_t> const max_words_per_reduced_test;

/**
 * Enable highspeed omnibus connection and write and read all PPU memory words for verification.
 */
TEST(PPUMemoryWord, WRHighspeed)
{
	auto sequence = DigitalInit();
	sequence.highspeed_link.enable_systime = false;
	auto [builder, _] = generate(sequence);

	std::map<PPUMemoryWordOnDLS, PPUMemoryWord> words;
	std::map<PPUMemoryWordOnDLS, PlaybackProgram::ContainerTicket<PPUMemoryWord>> word_tickets;

	PlaybackProgramBuilder read_builder;
	for (auto const word : iter_sparse<PPUMemoryWordOnDLS>(max_words_per_reduced_test)) {
		auto const val = draw_ranged_non_default_value<PPUMemoryWord::Value>();
		words.insert(std::make_pair(word, PPUMemoryWord(val)));
		builder.write(word, words.at(word));
		word_tickets.emplace(std::make_pair(word, read_builder.read(word)));
	}
	builder.merge_back(read_builder);

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	for (auto const [word, ticket] : word_tickets) {
		EXPECT_TRUE(ticket.valid());
		EXPECT_EQ(ticket.get(), words.at(word));
	}
}
