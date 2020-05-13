#include <gtest/gtest.h>

#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/jtag.h"
#include "haldls/vx/v1/ppu.h"
#include "haldls/vx/v1/reset.h"
#include "haldls/vx/v1/traits.h"
#include "stadls/vx/v1/init_generator.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/playback_program_builder.h"
#include "stadls/vx/v1/run.h"

#include "connection.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx::v1;
using namespace stadls::vx::v1;

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
