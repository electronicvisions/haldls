#include <gtest/gtest.h>

#include "haldls/vx/capmem.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"

#include "executor.h"
#include "test-helper.h"
#include "test-init_helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable Highspeed omnibus connection and write and read all four CapMemBlocks for verification.
 */
TEST(CapMemBlock, WROverHighspeed)
{
	PlaybackProgramBuilder builder;

	insert_highspeed_init(builder);

	std::array<CapMemBlock, CapMemBlockOnDLS::size> blocks;
	std::array<CapMemBlockOnDLS, CapMemBlockOnDLS::size> block_coords = {
	    CapMemBlockOnDLS(0), CapMemBlockOnDLS(1), CapMemBlockOnDLS(2), CapMemBlockOnDLS(3)};
	// Fill blocks with random data
	for (auto coord : iter_all<CapMemCellOnCapMemBlock>()) {
		for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
			auto const val = draw_ranged_non_default_value<typename CapMemCell::Value>();
			blocks[i].set_cell(CapMemCellOnDLS(coord, block_coords[i]), val);
		}
	}

	// Write blocks down using Highspeed
	for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
		builder.write(block_coords[i], blocks[i], Backend::OmnibusChip);
	}

	std::array<CapMemBlock, CapMemBlockOnDLS::size> recv_blocks;
	std::vector<PlaybackProgram::ContainerTicket<CapMemBlock>> block_tickets;
	// Read blocks back
	for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
		block_tickets.push_back(builder.read(block_coords[i], Backend::OmnibusChip));
	}
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(40000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
		EXPECT_TRUE(block_tickets[i].valid());
		EXPECT_TRUE(block_tickets[i].get() == blocks[i]);
	}
}
