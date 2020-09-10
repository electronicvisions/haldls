#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/v1/quad.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/v1/cadc.h"
#include "haldls/vx/v1/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;
using namespace halco::common;
using namespace fisch::vx;

typedef std::vector<OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<OmnibusChipOverJTAG> words_type;

TEST(CADCSampleQuad, General)
{
	CADCSampleQuad config;

	// test getter/setter
	{
		auto coord = EntryOnQuad(1);
		auto value = draw_ranged_non_default_value<CADCSampleQuad::Value>(config.get_sample(coord));
		config.set_sample(coord, value);
		EXPECT_EQ(config.get_sample(coord), value);
	}

	CADCSampleQuad config_eq = config;
	CADCSampleQuad config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CADCSampleQuad, EncodeDecode)
{
	CADCSampleQuad config;

	config.set_sample(EntryOnQuad(1), CADCSampleQuad::Value(0x12));

	CADCSampleQuadOnDLS coord(
	    CADCSampleQuadOnSynram(
	        SynapseQuadOnSynram(X(3), Y(5)), CADCChannelType::acausal, CADCReadoutType::buffered),
	    SynramOnDLS(0));

	std::array<OmnibusAddress, CADCSampleQuad::read_config_size_in_words> ref_addresses = {
	    OmnibusAddress{0x02ec'017ful}};
	std::array<Omnibus, CADCSampleQuad::read_config_size_in_words> ref_data = {
	    OmnibusData{0x48 << 16}};

	{ // write addresses
		std::vector<OmnibusAddress> write_addresses;
		visit_preorder(
		    config, coord, stadls::WriteAddressVisitor<decltype(write_addresses)>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(std::array<OmnibusAddress, 0>{}));
	}

	{ // read addresses
		std::vector<OmnibusAddress> read_addresses;
		visit_preorder(
		    config, coord, stadls::ReadAddressVisitor<decltype(read_addresses)>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<Omnibus> data;
	visit_preorder(config, coord, stadls::EncodeVisitor<decltype(data)>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(std::array<Omnibus, 0>{}));

	CADCSampleQuad config_copy;
	ASSERT_NE(config, config_copy);
	data.clear();
	data.push_back(ref_data.at(0));
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<decltype(data)>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CADCSampleQuad, CerealizeCoverage)
{
	CADCSampleQuad obj1, obj2;
	obj1.set_sample(
	    EntryOnQuad(),
	    draw_ranged_non_default_value<CADCSampleQuad::Value>(obj1.get_sample(EntryOnQuad())));

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

TEST(CADCChannelConfig, General)
{
	CADCChannelConfig config;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<CADCChannelConfig::Offset>(config.get_offset());
		config.set_offset(value);
		EXPECT_EQ(config.get_offset(), value);
	}

	CADCChannelConfig config_eq = config;
	CADCChannelConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CADCChannelConfig, EncodeDecode)
{
	CADCChannelConfig config;

	config.set_offset(CADCChannelConfig::Offset(12));

	CADCChannelConfigOnDLS coord(
	    CADCChannelConfigOnSynram(CADCChannelColumnOnSynram(3), CADCChannelType::acausal),
	    SynramOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, CADCChannelConfig::config_size_in_words> ref_addresses =
	    {OmnibusChipOverJTAGAddress{0x0240'0807ul}};
	std::array<OmnibusChipOverJTAG, CADCChannelConfig::config_size_in_words> ref_data = {
	    OmnibusData{12 + 128}};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	CADCChannelConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CADCChannelConfig, CerealizeCoverage)
{
	CADCChannelConfig obj1, obj2;
	obj1.set_offset(draw_ranged_non_default_value<CADCChannelConfig::Offset>(obj1.get_offset()));

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
