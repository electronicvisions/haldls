#include <gtest/gtest.h>

#include "flyspi-rw_api/flyspi_com.h"

#include "haldls/v2/register.h"
#include "stadls/v2/local_board_control.h"
#include "stadls/v2/ocp.h"

#ifndef NO_LOCAL_BOARD

class OcpTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		// get the board id
		std::vector<std::string> board_ids = stadls::v2::available_board_usb_serial_numbers();
		ASSERT_EQ(1, board_ids.size()) << "number of allocated boards is not one";

		test_board = board_ids.front();
	}

	std::string test_board;
};

TEST_F(OcpTest, FlyspiProgramAddress)
{
	namespace con = haldls::v2;
	namespace io = stadls::v2;

	halco::common::Unique coord;

	// Connect to board
	rw_api::FlyspiCom com(test_board);

	// Write default config
	io::ocp_write_container(com, coord, con::FlyspiConfig());

	std::vector<con::ocp_word_type::value_type> values{0, 0xffffffff, 0x01020304};
	for (auto const value : values) {
		// Write the program address
		io::ocp_write_container(com, coord, con::FlyspiProgramAddress(value));

		// Read the program address
		auto result_reg = io::ocp_read_container<con::FlyspiProgramAddress>(com, coord);
		EXPECT_EQ(value, result_reg.get_value());
	}
}

TEST_F(OcpTest, FlyspiProgramSize)
{
	namespace con = haldls::v2;
	namespace io = stadls::v2;

	halco::common::Unique coord;

	// Connect to board
	rw_api::FlyspiCom com(test_board);

	// Write default config
	io::ocp_write_container(com, coord, con::FlyspiConfig());

	std::vector<con::ocp_word_type::value_type> values{0, 0xffffffff, 0x01020304};
	for (auto const value : values) {
		// Write the program size
		io::ocp_write_container(com, coord, con::FlyspiProgramSize(value));

		// Read the program size
		auto result_reg = io::ocp_read_container<con::FlyspiProgramSize>(com, coord);
		EXPECT_EQ(value, result_reg.get_value());
	}
}

TEST_F(OcpTest, FlyspiResultAddress)
{
	namespace con = haldls::v2;
	namespace io = stadls::v2;

	halco::common::Unique coord;

	// Connect to board
	rw_api::FlyspiCom com(test_board);

	// Write default config
	io::ocp_write_container(com, coord, con::FlyspiConfig());

	std::vector<con::ocp_word_type::value_type> values{0, 0xffffffff, 0x01020304};
	for (auto const value : values) {
		// Write the result address
		io::ocp_write_container(com, coord, con::FlyspiResultAddress(value));

		// Read the result address
		auto result_reg = io::ocp_read_container<con::FlyspiResultAddress>(com, coord);
		EXPECT_EQ(value, result_reg.get_value());
	}
}

TEST_F(OcpTest, FlyspiResultSize)
{
	namespace con = haldls::v2;
	namespace io = stadls::v2;

	halco::common::Unique coord;

	// Connect to board
	rw_api::FlyspiCom com(test_board);

	// Write default config
	io::ocp_write_container(com, coord, con::FlyspiConfig());

	// Read the result size
	auto result_reg = io::ocp_read_container<con::FlyspiResultSize>(com, coord);

	// Expect that the optional has a value
	EXPECT_TRUE(result_reg.get_value());
}

TEST_F(OcpTest, FlyspiState)
{
	namespace con = haldls::v2;
	namespace io = stadls::v2;

	halco::common::Unique coord;

	// Connect to board
	rw_api::FlyspiCom com(test_board);

	// Write default config
	io::ocp_write_container(com, coord, con::FlyspiConfig());

	// Read the result size
	auto result_reg = io::ocp_read_container<con::FlyspiState>(com, coord);

	// Expect that the optional has a value
	EXPECT_TRUE(result_reg.get_value());
}

TEST_F(OcpTest, FlyspiConfig)
{
	namespace con = haldls::v2;
	namespace io = stadls::v2;

	halco::common::Unique coord;

	// Connect to board
	rw_api::FlyspiCom com(test_board);

	for (bool const value : {true, false}) {
		// Write config with spike router en/disabled
		con::FlyspiConfig reg;
		reg.set_enable_spike_router(value);
		io::ocp_write_container(com, coord, reg);

		// Read the program address
		auto result_reg = io::ocp_read_container<con::FlyspiConfig>(com, coord);
		EXPECT_EQ(result_reg.get_dls_reset(), reg.get_dls_reset());
		EXPECT_EQ(reg, result_reg);
	}
}

TEST_F(OcpTest, FlyspiException)
{
	namespace con = haldls::v2;
	namespace io = stadls::v2;

	halco::common::Unique coord;

	// Connect to board
	rw_api::FlyspiCom com(test_board);

	// Write default config
	io::ocp_write_container(com, coord, con::FlyspiConfig());

	// Read exception register
	auto result_reg = io::ocp_read_container<con::FlyspiException>(com, coord);

	// All optionals should be filled
	ASSERT_TRUE(result_reg.get_result_read_error());
	ASSERT_TRUE(result_reg.get_result_read_overflow());
	ASSERT_TRUE(result_reg.get_result_write_error());
	ASSERT_TRUE(result_reg.get_result_write_underrun());
	ASSERT_TRUE(result_reg.get_playback_read_error());
	ASSERT_TRUE(result_reg.get_playback_read_overflow());
	ASSERT_TRUE(result_reg.get_playback_write_error());
	ASSERT_TRUE(result_reg.get_playback_write_underrun());
	ASSERT_TRUE(result_reg.get_program_exception());
	ASSERT_TRUE(result_reg.get_serdes_overflow());
	ASSERT_TRUE(result_reg.get_serdes_pll_unlocked());
	ASSERT_TRUE(result_reg.get_serdes_race());
	ASSERT_TRUE(result_reg.get_encode_overflow());

	// There should be no exceptions
	EXPECT_EQ(result_reg.get_result_read_error().value_or(true), false);
	EXPECT_EQ(result_reg.get_result_read_overflow().value_or(true), false);
	EXPECT_EQ(result_reg.get_result_write_error().value_or(true), false);
	EXPECT_EQ(result_reg.get_result_write_underrun().value_or(true), false);
	EXPECT_EQ(result_reg.get_playback_read_error().value_or(true), false);
	EXPECT_EQ(result_reg.get_playback_read_overflow().value_or(true), false);
	EXPECT_EQ(result_reg.get_playback_write_error().value_or(true), false);
	EXPECT_EQ(result_reg.get_playback_write_underrun().value_or(true), false);
	EXPECT_EQ(result_reg.get_program_exception().value_or(true), false);
	EXPECT_EQ(result_reg.get_serdes_overflow().value_or(true), false);
	EXPECT_EQ(result_reg.get_serdes_pll_unlocked().value_or(true), false);
	EXPECT_EQ(result_reg.get_serdes_race().value_or(true), false);
	EXPECT_EQ(result_reg.get_encode_overflow().value_or(true), false);
}

#endif
