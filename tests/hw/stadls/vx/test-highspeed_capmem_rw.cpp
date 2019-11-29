#include <gtest/gtest.h>

#include "halco/common/typed_array.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable Highspeed omnibus connection and write and read all four CapMemBlocks for verification.
 */
TEST(CapMemBlock, WROverHighspeed)
{
	auto sequence = DigitalInit();
	sequence.highspeed_link.enable_systime = false;
	auto [builder, _] = generate(sequence);

	typed_array<CapMemBlock, CapMemBlockOnDLS> blocks;
	// Fill blocks with random data
	for (auto cell : iter_all<CapMemCellOnCapMemBlock>()) {
		for (auto block : iter_all<CapMemBlockOnDLS>()) {
			auto const val = draw_ranged_non_default_value<CapMemCell::Value>();
			blocks[block].set_cell(cell, val);
		}
	}

	// Write blocks down using Highspeed
	for (auto block : iter_all<CapMemBlockOnDLS>()) {
		builder.write(block, blocks[block], Backend::OmnibusChip);
	}

	std::vector<PlaybackProgram::ContainerTicket<CapMemBlock>> block_tickets;
	// Read blocks back
	for (auto block : iter_all<CapMemBlockOnDLS>()) {
		block_tickets.push_back(builder.read(block, Backend::OmnibusChip));
	}
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(40000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (auto block : iter_all<CapMemBlockOnDLS>()) {
		EXPECT_TRUE(block_tickets[block].valid());
		EXPECT_TRUE(block_tickets[block].get() == blocks[block]);
	}
}
