#include <gtest/gtest.h>

#include <map>

#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/capmem.h"
#include "haldls/vx/v1/jtag.h"
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

	builder.block_until(BarrierOnFPGA(), Barrier::jtag);
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	for (auto const [cell, ticket] : cell_tickets) {
		EXPECT_TRUE(ticket.valid());
		EXPECT_EQ(ticket.get(), cells.at(cell));
	}
}
