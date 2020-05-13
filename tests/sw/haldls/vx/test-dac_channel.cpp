#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/v1/spi.h"

#include "fisch/vx/spi.h"
#include "halco/hicann-dls/vx/spi.h"
#include "haldls/vx/v1/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::SPIDACDataRegisterOnBoard> addresses_type;
typedef std::vector<fisch::vx::SPIDACDataRegister> words_type;

TEST(DACChannel, General)
{
	test_generic_functionality_single_value<DACChannel>();
}

TEST(DACChannel, EncodeDecode)
{
	DACChannel config;

	config.set_value(draw_ranged_non_default_value<DACChannel::Value>(config.get_value()));

	auto dac_coord = draw_ranged_non_default_value<DACOnBoard>(0);
	auto channel_coord = draw_ranged_non_default_value<DACChannelOnDAC>(0);
	auto coord = DACChannelOnBoard(channel_coord, dac_coord);

	halco::hicann_dls::vx::SPIDACDataRegisterOnBoard ref_address(coord.toEnum());
	std::array<halco::hicann_dls::vx::SPIDACDataRegisterOnBoard, DACChannel::config_size_in_words>
	    ref_addresses = {ref_address};
	std::array<fisch::vx::SPIDACDataRegister, DACChannel::config_size_in_words> ref_data = {
	    {fisch::vx::SPIDACDataRegister::Value(config.get_value())}};

	{
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(DACChannel, CerealizeCoverage)
{
	DACChannel obj1, obj2;
	obj1.set_value(draw_ranged_non_default_value<DACChannel::Value>(obj1.get_value()));

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
