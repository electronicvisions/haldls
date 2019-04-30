#include <gtest/gtest.h>

#include "halco/hicann-dls/v2/coordinates.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/playback.h"
#include "stadls/v2/local_board_control.h"

using namespace halco::common;
using namespace halco::hicann_dls::v2;
using namespace haldls::v2;
using namespace stadls::v2;

#ifndef NO_LOCAL_BOARD

class ReadbackTest : public ::testing::Test {
protected:
	void SetUp() override
	{
		// get the board id
		std::vector<std::string> board_ids = available_board_usb_serial_numbers();
		ASSERT_EQ(1, board_ids.size()) << "number of allocated boards is not one";

		test_board = board_ids.front();
	}

	std::string test_board;
}; // ReadbackTest

TEST_F(ReadbackTest, ExternalNeuronSwitchesAreDisabled) {
	Board board;
	Chip chip;

	Unique const unique;
	NeuronOnDLS const output_neuron{0};

	chip.enable_buffered_readout(output_neuron);
	EXPECT_EQ(output_neuron, chip.get_buffered_readout_neuron());

	PlaybackProgramBuilder builder;
	auto chip_ticket = builder.read(unique);
	auto output_neuron_ticket = builder.read(output_neuron);
	builder.halt();
	auto program = builder.done();

	{
		LocalBoardControl ctrl(test_board);
		ctrl.configure_static(board, chip);
		ctrl.run(program);
	}

	auto const chip_config = chip_ticket.get();
	EXPECT_EQ(output_neuron, chip_config.get_buffered_readout_neuron());

	auto const output_neuron_config = output_neuron_ticket.get();
	EXPECT_FALSE(output_neuron_config.get_enable_buffered_readout());
}

#endif
