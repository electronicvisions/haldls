#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v1/dac.h"

#include "fisch/vx/spi.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/spi.h"
#include "haldls/vx/v1/common.h"
#include "haldls/vx/v1/spi.h"
#include "lola/vx/cerealization.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx::v1;
using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(DACControlBlock, General)
{
	DACControlBlock config;

	// test getter/setter
	auto value = config.enable;
	value[DACChannelOnBoard(Enum(0))] = !value[DACChannelOnBoard(Enum(0))];
	config.enable = value;
	EXPECT_EQ(config.enable, value);

	DACControlBlock config_eq = config;
	DACControlBlock config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(DACControlBlock, CerealizeCoverage)
{
	DACControlBlock obj1, obj2;
	for (auto channel : iter_all<DACChannelOnBoard>()) {
		obj1.enable[channel] = !obj1.enable[channel];
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}

TEST(DACControlBlock, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::SPIDACControlRegisterOnBoard> addresses_type;
	typedef std::vector<fisch::vx::SPIDACControlRegister> words_type;

	DACControlBlock config;
	for (auto ch : iter_all<DACChannelOnBoard>()) {
		config.enable[ch] = false;
	}
	config.enable[DACChannelOnBoard(Enum(2))] = true;

	DACControlBlockOnBoard coord;

	std::array<
	    typename addresses_type::value_type, DACOnBoard::size* DACControl::config_size_in_words>
	    ref_addresses = {
	        typename addresses_type::value_type(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::gain_reference, DACOnBoard(0)),
	        typename addresses_type::value_type(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::power_down, DACOnBoard(0)),
	        typename addresses_type::value_type(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::gain_reference, DACOnBoard(1)),
	        typename addresses_type::value_type(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::power_down, DACOnBoard(1))};
	std::array<fisch::vx::SPIDACControlRegister, DACOnBoard::size* DACControl::config_size_in_words>
	    ref_data = {
	        fisch::vx::SPIDACControlRegister(fisch::vx::SPIDACControlRegister::Value(0b001100)),
	        fisch::vx::SPIDACControlRegister(
	            fisch::vx::SPIDACControlRegister::Value(0xff ^ (1u << 2))),
	        fisch::vx::SPIDACControlRegister(fisch::vx::SPIDACControlRegister::Value(0b001100)),
	        fisch::vx::SPIDACControlRegister(fisch::vx::SPIDACControlRegister::Value(0xff))};

	{
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(DACChannelBlock, General)
{
	DACChannelBlock config;

	// test getter/setter
	auto value = config.value;
	value[DACChannelOnBoard(Enum(0))] = DACChannelBlock::Value(0x123);
	config.value = value;
	EXPECT_EQ(config.value, value);

	DACChannelBlock config_eq = config;
	DACChannelBlock config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(DACChannelBlock, CerealizeCoverage)
{
	DACChannelBlock obj1, obj2;
	for (auto channel : iter_all<DACChannelOnBoard>()) {
		obj1.value[channel] =
		    draw_ranged_non_default_value<DACChannelBlock::Value>(obj1.value[channel]);
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}

TEST(DACChannelBlock, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::SPIDACDataRegisterOnBoard> addresses_type;
	typedef std::vector<fisch::vx::SPIDACDataRegister> words_type;

	DACChannelBlock config;
	for (auto ch : iter_all<DACChannelOnBoard>()) {
		config.value[ch] = draw_ranged_non_default_value<DACChannelBlock::Value>(config.value[ch]);
	}

	DACChannelBlockOnBoard coord;

	std::array<SPIDACDataRegisterOnBoard, DACChannelOnBoard::size> ref_addresses;
	for (auto coord : iter_all<SPIDACDataRegisterOnBoard>()) {
		ref_addresses[coord.toEnum()] = coord;
	}
	std::array<
	    fisch::vx::SPIDACDataRegister, DACChannelOnBoard::size * DACChannel::config_size_in_words>
	    ref_data;
	for (auto coord : iter_all<DACChannelOnBoard>()) {
		ref_data[coord.toEnum()] = fisch::vx::SPIDACDataRegister(
		    fisch::vx::SPIDACDataRegister::Value(config.value[coord]));
	}

	{
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}
