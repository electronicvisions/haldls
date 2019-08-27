#include <gtest/gtest.h>

#include "haldls/vx/capmem.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable over JTAG omnibus connection and write and read all four CapMemBlocks for verification.
 */
TEST(CapMemBlock, WROverJTAG)
{
	PlaybackProgramBuilder builder;

	// Chip reset
	builder.write(ResetChipOnDLS(), ResetChip(true));
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(10));
	builder.write(ResetChipOnDLS(), ResetChip(false));
	builder.wait_until(TimerOnDLS(), Timer::Value(100));

	// JTAG init
	builder.write(JTAGClockScalerOnDLS(), JTAGClockScaler(JTAGClockScaler::Value(3)));
	builder.write(ResetJTAGTapOnDLS(), ResetJTAGTap());

	std::array<CapMemBlock, CapMemBlockOnDLS::size> blocks;
	std::array<CapMemBlockOnDLS, 4> block_coords = {CapMemBlockOnDLS(0), CapMemBlockOnDLS(1),
	                                                CapMemBlockOnDLS(2), CapMemBlockOnDLS(3)};
	// Wait Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(25 * Timer::Value::fpga_clock_cycles_per_us));
	builder.write(TimerOnDLS(), Timer());
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
		block_tickets.push_back(builder.read(block_coords[i], Backend::OmnibusChipOverJTAG));
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
