#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "haldls/container/v2/dac.h"
#include "haldls/io/visitors.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(DAC, General)
{
	EXPECT_ANY_THROW(DAC::Value(4096));
	EXPECT_NO_THROW(DAC::Value(4095));
	EXPECT_ANY_THROW(DAC::Channel(8));
	EXPECT_NO_THROW(DAC::Channel(7));

	DAC dac;
	for (auto channel_it : halco::common::iter_all<DAC::Channel>()) {
		ASSERT_EQ(dac.get(channel_it), DAC::Value(0));
	}

	// test getter/setter
	dac.set(DAC::Channel(2), DAC::Value(42));
	ASSERT_EQ(dac.get(DAC::Channel(2)), DAC::Value(42));

	DAC dac2 = dac;
	DAC dac3;
	dac3.set(DAC::Channel(5), DAC::Value(2345));

	ASSERT_EQ(dac, dac2);
	ASSERT_EQ(dac == dac3, false);

	ASSERT_NE(dac, dac3);
	ASSERT_EQ(dac != dac2, false);
}

TEST(DAC, Encode)
{
	DAC config;
	DACOnBoard const coord = DACOnBoard::DAC_12_DECIVOLT;

	std::vector<size_t> values{352, 214, 46, 32, 2451, 578, 999, 1010};
	for (auto const channel : iter_all<DAC::Channel>()) {
		config.set(channel, DAC::Value(values.back()));
		values.pop_back();
	}

	// clang-format off
	std::array<std::uint32_t, 9> const ref_addresses = {{0x4d80ul, 0x4d03ul, 0x4d13ul, 0x4d22ul, 0x4d39ul, 0x4d40ul, 0x4d50ul, 0x4d60ul, 0x4d71ul}};
	std::array<std::uint32_t, 9> const ref_data = {{0xcul, 0xf2ul, 0xe7ul, 0x42ul, 0x93ul, 0x20ul, 0x2eul, 0xd6ul, 0x60ul}};
	// clang-format on

	typedef std::vector<ocp_address_type> ocp_addresses_type;
	typedef std::vector<ocp_word_type> ocp_words_type;

	{ // write addresses
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			config, coord, haldls::io::WriteAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<std::uint32_t> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	ocp_words_type ocp_data;
	visit_preorder(config, coord, haldls::io::EncodeVisitor<ocp_words_type>{ocp_data});

	std::vector<std::uint32_t> data;
	data.reserve(ocp_data.size());
	for (auto word : ocp_data)
		data.push_back(word.value);
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}
