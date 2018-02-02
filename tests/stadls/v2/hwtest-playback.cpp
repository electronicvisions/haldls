#include <tuple>

#include <gtest/gtest.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/container/v2/capmem.h"
#include "haldls/container/v2/playback.h"
#include "haldls/exception/exceptions.h"
#include "stadls/v2/experiment.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::container::v2;
using namespace stadls::v2;

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
	Unique const unique;

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
	builder.set_container(unique, capmem_config);
	builder.wait_until(100);
	auto capmem_ticket = builder.get_container(unique, capmem_config);
	auto capmemcell_ticket = builder.get_container<CapMemCell>(cell);
	builder.halt();

	auto program = builder.done();

	// Builder is reset (starts a new program) every time .done() is called
	auto empty_new_program = builder.done();
	EXPECT_NE(program.serial_number(), empty_new_program.serial_number());

	// No data available yet
	EXPECT_THROW(std::ignore = program.get(capmem_ticket), std::runtime_error);
	EXPECT_THROW(std::ignore = program.get(capmemcell_ticket), std::runtime_error);

	auto capmem_ticket_ = builder.get_container(unique, capmem_config);
	auto program_ = builder.done();

	// Using Ticket issued for a different program
	EXPECT_THROW(std::ignore = program.get(capmem_ticket_), std::invalid_argument);
	EXPECT_THROW(std::ignore = program_.get(capmem_ticket), std::invalid_argument);

	{
		ExperimentControl ctrl(test_board);
		ctrl.run(program);
	}

	EXPECT_THROW(std::ignore = program.get(capmem_ticket_), std::invalid_argument);

	auto capmem_copy = program.get(capmem_ticket);
	auto capmemcell_copy = program.get(capmemcell_ticket);

	EXPECT_EQ(capmem_config, capmem_copy);
	EXPECT_EQ(capmemvalue, capmemcell_copy.get_value());
}

TEST_F(PlaybackTest, InvalidState) {
	PlaybackProgram invalid_program; // not obtained via builder
	EXPECT_EQ(PlaybackProgram::invalid_serial_number, invalid_program.serial_number());

	ExperimentControl ctrl(test_board);
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

	ExperimentControl ctrl(test_board);
	ctrl.configure_static(board, chip);
	ctrl.transfer(program);
	EXPECT_THROW(ctrl.execute(), haldls::exception::InvalidConfiguration);
	EXPECT_THROW(ctrl.run(program), haldls::exception::InvalidConfiguration);
}
