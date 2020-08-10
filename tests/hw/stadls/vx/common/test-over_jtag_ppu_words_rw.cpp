#include <gtest/gtest.h>

#include "haldls/vx/barrier.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
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
 * Enable over JTAG omnibus connection and write and read PPU memory words for verification.
 */
TEST(PPUMemoryWord, WROverJTAG)
{
	auto sequence = DigitalInit();
	sequence.enable_highspeed_link = false;
	auto [builder, _] = generate(sequence);

	std::map<PPUMemoryWordOnDLS, PPUMemoryWord> words;
	std::map<PPUMemoryWordOnDLS, PlaybackProgram::ContainerTicket<PPUMemoryWord>> word_tickets;

	PlaybackProgramBuilder read_builder;
	for (auto const word : iter_sparse<PPUMemoryWordOnDLS>(max_words_per_reduced_test)) {
		auto const val = draw_ranged_non_default_value<PPUMemoryWord::Value>();
		words.insert(std::make_pair(word, PPUMemoryWord(val)));
		builder.write(word, words.at(word), Backend::OmnibusChipOverJTAG);
		word_tickets.emplace(
		    std::make_pair(word, read_builder.read(word, Backend::OmnibusChipOverJTAG)));
	}
	builder.merge_back(read_builder);

	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	for (auto const [word, ticket] : word_tickets) {
		EXPECT_TRUE(ticket.valid());
		EXPECT_EQ(ticket.get(), words.at(word));
	}
}