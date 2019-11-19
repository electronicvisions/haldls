#include <gtest/gtest.h>

#include "haldls/vx/jtag.h"
#include "haldls/vx/ppu.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
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

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto const [word, ticket] : word_tickets) {
		EXPECT_TRUE(ticket.valid());
		EXPECT_EQ(ticket.get(), words.at(word));
	}
}

/**
 * Write and read control register
 */
TEST(PPUControlRegister, WROverJTAG)
{
	auto sequence = DigitalInit();
	sequence.enable_highspeed_link = false;
	auto [builder, _] = generate(sequence);

	PPUControlRegister reg1, reg2;
	PPUControlRegisterOnDLS ppu_coord1(0);
	PPUControlRegisterOnDLS ppu_coord2(1);
	reg1.set_cache_controller_enable(true);
	reg1.set_inhibit_reset(true);
	reg1.set_force_clock_on(true);
	reg1.set_force_clock_off(false);

	reg2.set_cache_controller_enable(false);
	reg2.set_inhibit_reset(false);
	reg2.set_force_clock_on(false);
	reg2.set_force_clock_off(true);

	// Write and read back reg1
	builder.write(ppu_coord1, reg1, Backend::OmnibusChipOverJTAG);
	builder.write(ppu_coord2, reg1, Backend::OmnibusChipOverJTAG);
	std::vector<PlaybackProgram::ContainerTicket<PPUControlRegister>> reg1_tickets;
	reg1_tickets.push_back(builder.read(ppu_coord1, Backend::OmnibusChipOverJTAG));
	reg1_tickets.push_back(builder.read(ppu_coord2, Backend::OmnibusChipOverJTAG));


	// Write and read back reg2
	builder.write(ppu_coord1, reg2, Backend::OmnibusChipOverJTAG);
	builder.write(ppu_coord2, reg2, Backend::OmnibusChipOverJTAG);
	std::vector<PlaybackProgram::ContainerTicket<PPUControlRegister>> reg2_tickets;
	reg2_tickets.push_back(builder.read(ppu_coord1, Backend::OmnibusChipOverJTAG));
	reg2_tickets.push_back(builder.read(ppu_coord2, Backend::OmnibusChipOverJTAG));

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
