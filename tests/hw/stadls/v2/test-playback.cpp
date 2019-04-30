#include <tuple>

#include <gtest/gtest.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/exception/exceptions.h"
#include "haldls/v2/capmem.h"
#include "haldls/v2/playback.h"
#include "stadls/v2/local_board_control.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::v2;
using namespace stadls::v2;

#ifndef NO_LOCAL_BOARD

class PlaybackTest : public ::testing::Test {
protected:
	void SetUp() override
	{
		// get the board id
		std::vector<std::string> board_ids = available_board_usb_serial_numbers();
		ASSERT_EQ(1, board_ids.size()) << "number of allocated boards is not one";

		test_board = board_ids.front();
	}

	std::string test_board;
}; // PlaybackTest

TEST_F(PlaybackTest, CapMem) {
	CapMemOnDLS const coord;

	CapMem capmem_config;

	for (auto cell : iter_all<CapMemCellOnDLS>()) {
		capmem_config.set(cell, CapMemCell::Value(cell.toEnum()));
	}

	CapMemCellOnDLS const cell(Enum(2));
	CapMemCell::Value const capmemvalue(334);
	capmem_config.set(CapMemCellOnDLS(Enum(0)), CapMemCell::Value(123));
	capmem_config.set(CapMemCellOnDLS(Enum(1)), CapMemCell::Value(321));
	capmem_config.set(cell, capmemvalue);

	PlaybackProgramBuilder builder;
	builder.write(coord, capmem_config);
	builder.wait_until(100);
	auto capmem_ticket = builder.read(coord);
	auto capmemcell_ticket = builder.read(cell);
	builder.halt();

	auto program = builder.done();

	// Builder is reset (starts a new program) every time .done() is called
	auto empty_new_program = builder.done();
	EXPECT_NE(program, empty_new_program);

	// No data available yet
	EXPECT_THROW(std::ignore = capmem_ticket.get(), std::runtime_error);
	EXPECT_THROW(std::ignore = capmemcell_ticket.get(), std::runtime_error);

	{
		LocalBoardControl ctrl(test_board);
		ctrl.run(program);
	}

	auto capmem_copy = capmem_ticket.get();
	auto capmemcell_copy = capmemcell_ticket.get();

	EXPECT_EQ(capmem_config, capmem_copy);
	EXPECT_EQ(capmemvalue, capmemcell_copy.get_value());
}

TEST_F(PlaybackTest, InvalidState) {
	std::shared_ptr<PlaybackProgram> invalid_program; // not obtained via builder
	ASSERT_FALSE(invalid_program->valid());

	LocalBoardControl ctrl(test_board);
	EXPECT_THROW(ctrl.transfer(invalid_program), std::logic_error);
	EXPECT_THROW(ctrl.run(invalid_program), std::logic_error);
}

TEST_F(PlaybackTest, InvalidFPGA)
{
	Board board;
	Chip chip;

	// enable dls reset in the flyspi config
	auto config = board.get_flyspi_config();
	config.set_dls_reset(true);
	board.set_flyspi_config(config);

	// prepare dummy program
	PlaybackProgramBuilder builder;
	builder.wait_for(10000);
	builder.halt();

	auto program = builder.done();

	LocalBoardControl ctrl(test_board);
	ctrl.configure_static(board, chip);
	ctrl.transfer(program);
	EXPECT_THROW(ctrl.execute(), haldls::exception::InvalidConfiguration);
	EXPECT_THROW(ctrl.run(program), haldls::exception::InvalidConfiguration);
}

#endif
