#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/phy.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
typedef std::vector<fisch::vx::Omnibus> words_type;

TEST(PhyStatus, General)
{
	PhyStatus config;

	// test getter/setter
	{
		PhyStatus::CRCErrorCount value =
		    PhyStatus::CRCErrorCount(draw_non_default_value<PhyStatus::CRCErrorCount::value_type>(
		        config.get_crc_error_count()));
		config.set_crc_error_count(value);
		EXPECT_EQ(config.get_crc_error_count(), value);
	}

	{
		PhyStatus::OnlineTime value = PhyStatus::OnlineTime(
		    draw_non_default_value<PhyStatus::OnlineTime::value_type>(config.get_online_time()));
		config.set_online_time(value);
		EXPECT_EQ(config.get_online_time(), value);
	}

	{
		PhyStatus::RxDroppedCount value =
		    PhyStatus::RxDroppedCount(draw_non_default_value<PhyStatus::RxDroppedCount::value_type>(
		        config.get_rx_dropped_count()));
		config.set_rx_dropped_count(value);
		EXPECT_EQ(config.get_rx_dropped_count(), value);
	}

	{
		PhyStatus::RxCount value = PhyStatus::RxCount(
		    draw_non_default_value<PhyStatus::RxCount::value_type>(config.get_rx_count()));
		config.set_rx_count(value);
		EXPECT_EQ(config.get_rx_count(), value);
	}

	{
		PhyStatus::TxCount value = PhyStatus::TxCount(
		    draw_non_default_value<PhyStatus::TxCount::value_type>(config.get_tx_count()));
		config.set_tx_count(value);
		EXPECT_EQ(config.get_tx_count(), value);
	}

	PhyStatus config_eq = config;
	PhyStatus config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(PhyStatus, CerealizeCoverage)
{
	PhyStatus obj1, obj2;

	{
		PhyStatus::CRCErrorCount value =
		    PhyStatus::CRCErrorCount(draw_non_default_value<PhyStatus::CRCErrorCount::value_type>(
		        obj1.get_crc_error_count()));
		obj1.set_crc_error_count(value);
	}

	{
		PhyStatus::OnlineTime value = PhyStatus::OnlineTime(
		    draw_non_default_value<PhyStatus::OnlineTime::value_type>(obj1.get_online_time()));
		obj1.set_online_time(value);
	}

	{
		PhyStatus::RxDroppedCount value =
		    PhyStatus::RxDroppedCount(draw_non_default_value<PhyStatus::RxDroppedCount::value_type>(
		        obj1.get_rx_dropped_count()));
		obj1.set_rx_dropped_count(value);
	}

	{
		PhyStatus::RxCount value = PhyStatus::RxCount(
		    draw_non_default_value<PhyStatus::RxCount::value_type>(obj1.get_rx_count()));
		obj1.set_rx_count(value);
	}

	{
		PhyStatus::TxCount value = PhyStatus::TxCount(
		    draw_non_default_value<PhyStatus::TxCount::value_type>(obj1.get_tx_count()));
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

TEST(PhyStatus, EncodeDecode)
{
	PhyStatus config;
	config.set_crc_error_count(PhyStatus::CRCErrorCount(0x1));
	config.set_online_time(PhyStatus::OnlineTime(0x2));
	config.set_rx_dropped_count(PhyStatus::RxDroppedCount(0x3));
	config.set_rx_count(PhyStatus::RxCount(0x4));
	config.set_tx_count(PhyStatus::TxCount(0x5));

	PhyStatusOnFPGA coord(3);

	std::array<halco::hicann_dls::vx::OmnibusAddress, PhyStatus::read_config_size_in_words>
	    ref_addresses = {
	        OmnibusAddress(0x88004007), OmnibusAddress(0x8800400f), OmnibusAddress(0x88004017),
	        OmnibusAddress(0x8800c006), OmnibusAddress(0x8800c007)};

	std::array<fisch::vx::Omnibus, PhyStatus::read_config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x1}),
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x2}),
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x3}),
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x5}),
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x4})};

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	PhyStatus read_config;
	visit_preorder(
	    read_config, coord,
	    stadls::DecodeVisitor<std::array<fisch::vx::Omnibus, PhyStatus::read_config_size_in_words>>{
	        ref_data});
	EXPECT_EQ(config, read_config);
}
