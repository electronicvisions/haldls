#include <gtest/gtest.h>

#include <map>

#include "haldls/vx/capmem.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/reset.h"
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
 * Enable over JTAG omnibus connection and write and read all CapMemCells for verification.
 */
TEST(CapMemCell, WROverJTAG)
{
	auto sequence = DigitalInit();
	sequence.enable_highspeed_link = false;
	auto [builder, _] = generate(sequence);

	std::map<CapMemCellOnDLS, CapMemCell> cells;
	std::map<CapMemCellOnDLS, PlaybackProgram::ContainerTicket<CapMemCell>> cell_tickets;

	PlaybackProgramBuilder read_builder;
	for (auto const cell : iter_sparse<CapMemCellOnDLS>(max_words_per_reduced_test)) {
		auto const val = draw_ranged_non_default_value<CapMemCell::Value>();
		cells.insert(std::make_pair(cell, CapMemCell(val)));
		builder.write(cell, cells.at(cell), Backend::OmnibusChipOverJTAG);
		cell_tickets.emplace(
		    std::make_pair(cell, read_builder.read(cell, Backend::OmnibusChipOverJTAG)));
	}
	builder.merge_back(read_builder);

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(40000));
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	for (auto const [cell, ticket] : cell_tickets) {
		EXPECT_TRUE(ticket.valid());
		EXPECT_EQ(ticket.get(), cells.at(cell));
	}
}
