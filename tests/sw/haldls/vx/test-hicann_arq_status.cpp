#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/arq.h"
#include "haldls/vx/omnibus_constants.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

TEST(HicannARQStatus, General)
{
	HicannARQStatus config;

	// test getter/setter
	{
		HicannARQStatus::ReadCount value = HicannARQStatus::ReadCount(
		    draw_non_default_value<HicannARQStatus::ReadCount::value_type>(
		        config.get_read_count()));
		config.set_read_count(value);
		EXPECT_EQ(config.get_read_count(), value);
	}

	{
		HicannARQStatus::WriteCount value = HicannARQStatus::WriteCount(
		    draw_non_default_value<HicannARQStatus::WriteCount::value_type>(
		        config.get_write_count()));
		config.set_write_count(value);
		EXPECT_EQ(config.get_write_count(), value);
	}

	{
		HicannARQStatus::RxCount value = HicannARQStatus::RxCount(
		    draw_non_default_value<HicannARQStatus::RxCount::value_type>(config.get_rx_count()));
		config.set_rx_count(value);
		EXPECT_EQ(config.get_rx_count(), value);
	}

	{
		HicannARQStatus::TxCount value = HicannARQStatus::TxCount(
		    draw_non_default_value<HicannARQStatus::TxCount::value_type>(config.get_tx_count()));
		config.set_tx_count(value);
		EXPECT_EQ(config.get_tx_count(), value);
	}

	HicannARQStatus config_eq = config;
	HicannARQStatus config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(HicannARQStatus, CerealizeCoverage)
{
	HicannARQStatus obj1, obj2;

	{
		HicannARQStatus::ReadCount value = HicannARQStatus::ReadCount(
		    draw_non_default_value<HicannARQStatus::ReadCount::value_type>(obj1.get_read_count()));
		obj1.set_read_count(value);
	}

	{
		HicannARQStatus::WriteCount value = HicannARQStatus::WriteCount(
		    draw_non_default_value<HicannARQStatus::WriteCount::value_type>(
		        obj1.get_write_count()));
		obj1.set_write_count(value);
	}

	{
		HicannARQStatus::RxCount value = HicannARQStatus::RxCount(
		    draw_non_default_value<HicannARQStatus::RxCount::value_type>(obj1.get_rx_count()));
		obj1.set_rx_count(value);
	}

	{
		HicannARQStatus::TxCount value = HicannARQStatus::TxCount(
		    draw_non_default_value<HicannARQStatus::TxCount::value_type>(obj1.get_tx_count()));
		obj1.set_tx_count(value);
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

TEST(HicannARQStatus, EncodeDecode)
{
	HicannARQStatus config;
	config.set_read_count(HicannARQStatus::ReadCount(0x1));
	config.set_write_count(HicannARQStatus::WriteCount(0x2));
	config.set_rx_count(HicannARQStatus::RxCount(0x3));
	config.set_tx_count(HicannARQStatus::TxCount(0x4));

	HicannARQStatusOnFPGA coord;

	std::array<halco::hicann_dls::vx::OmnibusAddress, HicannARQStatus::read_config_size_in_words>
	    ref_addresses = {
	        OmnibusAddress(0x8800c010), OmnibusAddress(0x8800c011), OmnibusAddress(0x8800c012),
	        OmnibusAddress(0x8800c013)};

	std::array<fisch::vx::word_access_type::Omnibus, HicannARQStatus::read_config_size_in_words>
	    ref_data = {
	        fisch::vx::word_access_type::Omnibus(0x2), fisch::vx::word_access_type::Omnibus(0x4),
	        fisch::vx::word_access_type::Omnibus(0x3), fisch::vx::word_access_type::Omnibus(0x1)};

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	HicannARQStatus read_config;
	visit_preorder(
	    read_config, coord,
	    stadls::DecodeVisitor<std::array<
	        fisch::vx::word_access_type::Omnibus, HicannARQStatus::read_config_size_in_words>>{
	        ref_data});
	EXPECT_EQ(config, read_config);
}
