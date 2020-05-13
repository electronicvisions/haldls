#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v1/synapse.h"
#include "stadls/visitors.h"
#include "test-helper.h"


using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(CommonSynramConfig, General)
{
	CommonSynramConfig config;

	// test getter/setter
	{
		auto value =
		    draw_ranged_non_default_value<CommonSynramConfig::PCConf>(config.get_pc_conf_east());
		config.set_pc_conf_east(value);
		EXPECT_EQ(config.get_pc_conf_east(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<CommonSynramConfig::PCConf>(config.get_pc_conf_west());
		config.set_pc_conf_west(value);
		EXPECT_EQ(config.get_pc_conf_west(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<CommonSynramConfig::WConf>(config.get_w_conf_east());
		config.set_w_conf_east(value);
		EXPECT_EQ(config.get_w_conf_east(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<CommonSynramConfig::WConf>(config.get_w_conf_west());
		config.set_w_conf_west(value);
		EXPECT_EQ(config.get_w_conf_west(), value);
	}

	{
		auto value = draw_ranged_non_default_value<CommonSynramConfig::WaitCtrClear>(
		    config.get_wait_ctr_clear());
		config.set_wait_ctr_clear(value);
		EXPECT_EQ(config.get_wait_ctr_clear(), value);
	}

	CommonSynramConfig config_eq = config;
	CommonSynramConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CommonSynramConfig, EncodeDecode)
{
	CommonSynramConfig config;

	auto coord = CommonSynramConfigOnDLS::top;

	// Omnibus over JTAG access
	{
		config.set_w_conf_east(CommonSynramConfig::WConf(4));

		std::array<
		    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
		    CommonSynramConfig::config_size_in_words>
		    ref_addresses = {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x0200'0000),
		                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x0200'0001),
		                     halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x0200'0002)};
		std::array<fisch::vx::OmnibusChipOverJTAG, CommonSynramConfig::config_size_in_words>
		    ref_data = {
		        fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(
		            config.get_pc_conf_west().value() | (config.get_pc_conf_east().value() << 4))),
		        fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(
		            config.get_w_conf_west().value() | (config.get_w_conf_east().value() << 8))),
		        fisch::vx::OmnibusChipOverJTAG(
		            fisch::vx::OmnibusData(config.get_wait_ctr_clear().value()))};

		{ // write addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<
			        std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{
			        write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		{ // read addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> read_addresses;
			visit_preorder(
			    config, coord,
			    stadls::ReadAddressVisitor<
			        std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{
			        read_addresses});
			EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::OmnibusChipOverJTAG> data;
		visit_preorder(
		    config, coord,
		    stadls::EncodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

		CommonSynramConfig config_2;
		visit_preorder(
		    config_2, coord,
		    stadls::DecodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>(data));
		EXPECT_EQ(config, config_2);
	}
}

TEST(CommonSynramConfig, CerealizeCoverage)
{
	CommonSynramConfig obj1, obj2;

	obj1.set_pc_conf_east(
	    draw_ranged_non_default_value<CommonSynramConfig::PCConf>(obj1.get_pc_conf_east()));
	obj1.set_pc_conf_west(
	    draw_ranged_non_default_value<CommonSynramConfig::PCConf>(obj1.get_pc_conf_west()));
	obj1.set_w_conf_east(
	    draw_ranged_non_default_value<CommonSynramConfig::WConf>(obj1.get_w_conf_east()));
	obj1.set_w_conf_west(
	    draw_ranged_non_default_value<CommonSynramConfig::WConf>(obj1.get_w_conf_west()));
	obj1.set_wait_ctr_clear(
	    draw_ranged_non_default_value<CommonSynramConfig::WaitCtrClear>(obj1.get_wait_ctr_clear()));

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
