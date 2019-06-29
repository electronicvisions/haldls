#include <gtest/gtest.h>

#include "fisch/vx/constants.h"
#include "fisch/vx/playback_executor.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/playback.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;

/**
 * Enable over JTAG omnibus connection and write and read all four CapMemBlocks for verification.
 */
TEST(CapMemBlock, WROverJTAG)
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

	std::array<CapMemBlock, CapMemBlockOnDLS::size> blocks;
	std::array<CapMemBlockOnDLS, 4> block_coords = {CapMemBlockOnDLS(0), CapMemBlockOnDLS(1),
	                                                CapMemBlockOnDLS(2), CapMemBlockOnDLS(3)};
	// Wait Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(25 * fisch::vx::fpga_clock_cycles_per_us));
	builder.write<Timer>(TimerOnDLS(), Timer());
	// Fill blocks with random data
	for (auto coord : iter_all<CapMemCellOnCapMemBlock>()) {
		for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
			auto const val = draw_ranged_non_default_value<typename CapMemCell::Value>();
			blocks[i].set_cell(CapMemCellOnDLS(coord, block_coords[i]), val);
		}
	}


	// Write blocks down using JTAG
	for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
		builder.write(block_coords[i], blocks[i], Backend::OmnibusChipOverJTAG);
	}


	std::array<CapMemBlock, CapMemBlockOnDLS::size> recv_blocks;
	std::vector<PlaybackProgram::ContainerTicket<CapMemBlock>> block_tickets;
	// Read blocks back
	for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
		block_tickets.push_back(
		    builder.read<CapMemBlock>(block_coords[i], Backend::OmnibusChipOverJTAG));
	}
	builder.write<Timer>(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(40000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program.impl());

	for (size_t i = 0; i < CapMemBlockOnDLS::size; i++) {
		EXPECT_TRUE(block_tickets[i].valid());
		EXPECT_TRUE(block_tickets[i].get() == blocks[i]);
	}
}
