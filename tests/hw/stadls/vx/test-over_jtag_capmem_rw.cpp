#include <gtest/gtest.h>

#include "halco/common/typed_array.h"
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

	// Wait Omnibus to come up
	builder.wait_until(TimerOnDLS(), Timer::Value(25 * Timer::Value::fpga_clock_cycles_per_us));
	builder.write(TimerOnDLS(), Timer());

	typed_array<CapMemBlock, CapMemBlockOnDLS> blocks;
	// Fill blocks with random data
	for (auto cell : iter_all<CapMemCellOnCapMemBlock>()) {
		for (auto block : iter_all<CapMemBlockOnDLS>()) {
			auto const val = draw_ranged_non_default_value<CapMemCell::Value>();
			blocks[block].set_cell(cell, val);
		}
	}

	// Write blocks down using JTAG
	for (auto block : iter_all<CapMemBlockOnDLS>()) {
		builder.write(block, blocks[block], Backend::OmnibusChipOverJTAG);
	}

	std::vector<PlaybackProgram::ContainerTicket<CapMemBlock>> block_tickets;
	// Read blocks back
	for (auto block : iter_all<CapMemBlockOnDLS>()) {
		block_tickets.push_back(builder.read(block, Backend::OmnibusChipOverJTAG));
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
