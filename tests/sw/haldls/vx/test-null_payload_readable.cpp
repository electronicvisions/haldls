#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/null_payload_readable.h"

#include "cereal/types/haldls/cereal.tcc"
#include "fisch/vx/word_access/type/null_payload_readable.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;

typedef std::vector<NullPayloadReadableOnFPGA> addresses_type;
typedef std::vector<fisch::vx::word_access_type::NullPayloadReadable> words_type;

TEST(NullPayloadReadable, General)
{
	EXPECT_NO_THROW(NullPayloadReadable());

	NullPayloadReadable config;
	NullPayloadReadable other_config = config;

	EXPECT_TRUE(other_config == config);
	EXPECT_FALSE(other_config != config);
}

TEST(NullPayloadReadable, EncodeDecode)
{
	NullPayloadReadable config;
	NullPayloadReadableOnFPGA coord;

	std::array<NullPayloadReadableOnFPGA, NullPayloadReadable::read_config_size_in_words>
	    ref_read_addresses = {NullPayloadReadableOnFPGA()};
	std::array<NullPayloadReadableOnFPGA, NullPayloadReadable::write_config_size_in_words>
	    ref_write_addresses = {};
	std::array<
	    fisch::vx::word_access_type::NullPayloadReadable,
	    NullPayloadReadable::write_config_size_in_words>
	    ref_write_data = {};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_write_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_read_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_write_data));

	NullPayloadReadable config_copy;
	data = {{}};
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(NullPayloadReadable, Ostream)
{
	NullPayloadReadable obj;

	std::stringstream stream;
	stream << obj;

	EXPECT_EQ(stream.str(), "NullPayloadReadable()");
}

TEST(NullPayloadReadable, CerealizeCoverage)
{
	NullPayloadReadable obj1, obj2;

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
