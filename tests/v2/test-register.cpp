#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/v2/board.h"
#include "stadls/visitors.h"

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;


template <class Container, class Coord>
void expect_read_only()
{
	Container reg;
	Coord const coord;

	std::array<ocp_address_type::value_type, 0> const ref_addresses = {{}}; // RO
	std::array<ocp_address_type::value_type, 0> const ref_data = {{}};      // RO

	// write addresses
	{
		typedef std::vector<ocp_address_type> ocp_addresses_type;
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			reg, coord, stadls::WriteAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<ocp_address_type::value_type> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// encode data
	{
		typedef std::vector<ocp_word_type> ocp_words_type;
		ocp_words_type ocp_data;
		visit_preorder(reg, coord, stadls::EncodeVisitor<ocp_words_type>{ocp_data});

		std::vector<ocp_address_type::value_type> data;
		data.reserve(ocp_data.size());
		for (auto word : ocp_data)
			data.push_back(word.value);
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}
}

TEST(FlyspiProgramAddress, general)
{
	FlyspiProgramAddress reg(0xc0ffee);
	FlyspiProgramAddress reg2(0x0);

	EXPECT_EQ(reg.get_value(), 0xc0ffee);
	EXPECT_NE(reg, reg2);

	reg2.set_value(0xc0ffee);
	EXPECT_EQ(reg2.get_value(), 0xc0ffee);
	EXPECT_EQ(reg, reg2);
}

TEST(FlyspiProgramAddress, encode)
{
	FlyspiProgramAddress reg(0xc0ffee);
	Unique const coord;

	std::array<ocp_address_type::value_type, 1> const ref_addresses = {{0x8002}};
	std::array<ocp_address_type::value_type, 1> const ref_data = {{0xc0ffee}};

	// write addresses
	{
		typedef std::vector<ocp_address_type> ocp_addresses_type;
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			reg, coord, stadls::WriteAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<ocp_address_type::value_type> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// encode data
	{
		typedef std::vector<ocp_word_type> ocp_words_type;
		ocp_words_type ocp_data;
		visit_preorder(reg, coord, stadls::EncodeVisitor<ocp_words_type>{ocp_data});

		std::vector<ocp_address_type::value_type> data;
		data.reserve(ocp_data.size());
		for (auto word : ocp_data)
			data.push_back(word.value);
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}
}


TEST(FlyspiProgramAddress, decode)
{
	FlyspiProgramAddress reg;
	Unique const coord;

	std::array<ocp_address_type::value_type, 1> const ref_addresses = {{0x8002}};

	// read addresses
	{
		typedef std::vector<ocp_address_type> ocp_addresses_type;
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			reg, coord, stadls::ReadAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<ocp_address_type::value_type> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// decode data
	{
		typedef std::vector<ocp_word_type> ocp_words_type;
		ocp_words_type ocp_data = {{0xc0ffee}};
		visit_preorder(reg, coord, stadls::DecodeVisitor<ocp_words_type>{ocp_data});
		EXPECT_EQ(ocp_data[0].value, reg.get_value());
	}
}


TEST(FlyspiResultSize, encode)
{
	expect_read_only<FlyspiResultSize, Unique>();
}


TEST(FlyspiResultSize, decode)
{
	FlyspiResultSize reg;
	Unique const coord;

	std::array<ocp_address_type::value_type, 1> const ref_addresses = {{0x8005}};

	// read addresses
	{
		typedef std::vector<ocp_address_type> ocp_addresses_type;
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			reg, coord, stadls::ReadAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<ocp_address_type::value_type> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// decode data
	{
		typedef std::vector<ocp_word_type> ocp_words_type;
		ocp_words_type ocp_data = {{0xc0ffee}};
		ASSERT_FALSE(reg.get_value());
		visit_preorder(reg, coord, stadls::DecodeVisitor<ocp_words_type>{ocp_data});
		ASSERT_TRUE(reg.get_value());
		EXPECT_EQ(ocp_data[0].value, reg.get_value().value());
	}
}


TEST(FlyspiConfigTgControl, limits)
{
	EXPECT_NO_THROW(FlyspiConfig::TgControl(0));
	EXPECT_NO_THROW(FlyspiConfig::TgControl(42));
	EXPECT_NO_THROW(FlyspiConfig::TgControl(63));
	EXPECT_ANY_THROW(FlyspiConfig::TgControl(64));
}

TEST(FlyspiConfig, general)
{
	FlyspiConfig config;
	FlyspiConfig empty_config;

	config.set_dls_reset(true);
	EXPECT_EQ(config.get_dls_reset(), true);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_soft_reset(true);
	EXPECT_EQ(config.get_soft_reset(), true);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_tg_control(FlyspiConfig::TgControl(42));
	EXPECT_EQ(config.get_tg_control(), FlyspiConfig::TgControl(42));
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_enable_spike_router(true);
	EXPECT_EQ(config.get_enable_spike_router(), true);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_i_phase_select(true);
	EXPECT_EQ(config.get_i_phase_select(), true);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_o_phase_select(true);
	EXPECT_EQ(config.get_o_phase_select(), true);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_enable_train(true);
	EXPECT_EQ(config.get_enable_train(), true);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_enable_transceiver(false);
	EXPECT_EQ(config.get_enable_transceiver(), false);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_enable_lvds(false);
	EXPECT_EQ(config.get_enable_lvds(), false);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_enable_analog_power(false);
	EXPECT_EQ(config.get_enable_analog_power(), false);
	EXPECT_NE(config, empty_config);
	config = empty_config;

	config.set_enable_dls_loopback(true);
	EXPECT_EQ(config.get_enable_dls_loopback(), true);
	EXPECT_NE(config, empty_config);
	config = empty_config;
}


TEST(FlyspiConfig, encode)
{
	FlyspiConfig reg;
	Unique const coord;

	reg.set_enable_spike_router(true);

	std::array<ocp_address_type::value_type, 1> const ref_addresses = {{0x8020ul}};
	std::array<ocp_address_type::value_type, 1> const ref_data = {{0x0000008eul}};

	typedef std::vector<ocp_address_type> ocp_addresses_type;
	typedef std::vector<ocp_word_type> ocp_words_type;

	// write addresses
	{
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			reg, coord, stadls::WriteAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<ocp_address_type::value_type> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// encode data

	ocp_words_type ocp_data;
	visit_preorder(reg, coord, stadls::EncodeVisitor<ocp_words_type>{ocp_data});

	std::vector<ocp_address_type::value_type> data;
	data.reserve(ocp_data.size());
	for (auto word : ocp_data)
		data.push_back(word.value);
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}


TEST(FlyspiConfig, decode)
{
	FlyspiConfig reg;
	Unique const coord;

	std::array<ocp_address_type::value_type, 1> const ref_addresses = {{0x8020}};

	// read addresses
	{
		typedef std::vector<ocp_address_type> ocp_addresses_type;
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			reg, coord, stadls::ReadAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<ocp_address_type::value_type> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// decode data
	{
		// expection for tg_control if the n-th bit is set when decoding the
		// data
		auto tg_expect = [](size_t const n) -> FlyspiConfig::TgControl {
			if (n >= 8 && n < 14) {
				return FlyspiConfig::TgControl(1 << (n - 8));
			} else {
				return FlyspiConfig::TgControl(0);
			}
		};

		typedef std::vector<ocp_word_type> ocp_words_type;
		// loop over all 32 bits in one word
		for (size_t n = 0; n < 32; n++) {
			ocp_words_type ocp_data = {{ocp_address_type::value_type(1) << n}};
			visit_preorder(reg, coord, stadls::DecodeVisitor<ocp_words_type>{ocp_data});

			EXPECT_THAT(reg.get_dls_reset(), n == 31);
			EXPECT_THAT(reg.get_soft_reset(), n == 30);
			EXPECT_THAT(reg.get_tg_control(), tg_expect(n));
			EXPECT_THAT(reg.get_enable_spike_router(), n == 7);
			EXPECT_THAT(reg.get_i_phase_select(), n == 6);
			EXPECT_THAT(reg.get_o_phase_select(), n == 5);
			EXPECT_THAT(reg.get_enable_train(), n == 4);
			EXPECT_THAT(reg.get_enable_transceiver(), n == 3);
			EXPECT_THAT(reg.get_enable_lvds(), n == 2);
			EXPECT_THAT(reg.get_enable_analog_power(), n == 1);
			EXPECT_THAT(reg.get_enable_dls_loopback(), n == 0);
		}
	}
}


TEST(FlyspiException, encode)
{
	expect_read_only<FlyspiException, Unique>();
}


TEST(FlyspiException, decode)
{
	FlyspiException reg;
	Unique const coord;

	std::array<ocp_address_type::value_type, 1> const ref_addresses = {{0x8001}};

	// read addresses
	{
		typedef std::vector<ocp_address_type> ocp_addresses_type;
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			reg, coord, stadls::ReadAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<ocp_address_type::value_type> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// decode data
	{
		// Expect all optionals to be empty
		EXPECT_FALSE(reg.get_result_read_error());
		EXPECT_FALSE(reg.get_result_read_overflow());
		EXPECT_FALSE(reg.get_result_write_error());
		EXPECT_FALSE(reg.get_result_write_underrun());
		EXPECT_FALSE(reg.get_playback_read_error());
		EXPECT_FALSE(reg.get_playback_read_overflow());
		EXPECT_FALSE(reg.get_playback_write_error());
		EXPECT_FALSE(reg.get_playback_write_underrun());
		EXPECT_FALSE(reg.get_program_exception());
		EXPECT_FALSE(reg.get_serdes_overflow());
		EXPECT_FALSE(reg.get_serdes_pll_unlocked());
		EXPECT_FALSE(reg.get_serdes_race());
		EXPECT_FALSE(reg.get_encode_overflow());

		typedef std::vector<ocp_word_type> ocp_words_type;
		// loop over all 32 bits in one word
		for (size_t n = 0; n < 32; n++) {
			ocp_words_type ocp_data = {{ocp_address_type::value_type(1) << n}};
			visit_preorder(reg, coord, stadls::DecodeVisitor<ocp_words_type>{ocp_data});

			EXPECT_THAT(reg.get_result_read_error().value(), n == 0);
			EXPECT_THAT(reg.get_result_read_overflow().value(), n == 1);
			EXPECT_THAT(reg.get_result_write_error().value(), n == 2);
			EXPECT_THAT(reg.get_result_write_underrun().value(), n == 3);
			EXPECT_THAT(reg.get_playback_read_error().value(), n == 4);
			EXPECT_THAT(reg.get_playback_read_overflow().value(), n == 5);
			EXPECT_THAT(reg.get_playback_write_error().value(), n == 6);
			EXPECT_THAT(reg.get_playback_write_underrun().value(), n == 7);
			EXPECT_THAT(reg.get_program_exception().value(), n == 8);
			EXPECT_THAT(reg.get_serdes_overflow().value(), n == 16);
			EXPECT_THAT(reg.get_serdes_pll_unlocked().value(), n == 17);
			EXPECT_THAT(reg.get_serdes_race().value(), n == 18);
			EXPECT_THAT(reg.get_encode_overflow().value(), n == 20);
		}
	}
}
