#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "haldls/vx/fpga.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<fisch::vx::Omnibus> words_type;

TEST(EventRecordingConfig, General)
{
	// test getter/setter
	EventRecordingConfig word;
	word.set_enable_event_recording(true);
	ASSERT_EQ(word.get_enable_event_recording(), true);

	// test assign
	EventRecordingConfig word_eq = word;
	EventRecordingConfig word_ne;
	word_ne.set_enable_event_recording(false);

	// test comparison
	ASSERT_EQ(word, word_eq);
	ASSERT_FALSE(word == word_ne);

	ASSERT_NE(word, word_ne);
	ASSERT_FALSE(word != word_eq);

	// test ostream operator
	std::stringstream out;
	word.set_enable_event_recording(true);
	out << word;
	EXPECT_NE(out.str().find("true"), std::string::npos);
}

TEST(EventRecordingConfig, EncodeDecode)
{
	typedef std::vector<fisch::vx::Omnibus::coordinate_type> addresses_type;

	EventRecordingConfig config;
	config.set_enable_event_recording(true);

	EventRecordingConfigOnFPGA coord;

	std::array<
	    typename fisch::vx::Omnibus::coordinate_type,
	    EventRecordingConfig::read_config_size_in_words>
	    ref_read_addresses = {typename fisch::vx::Omnibus::coordinate_type{0x8000'0005ul}};
	std::array<
	    typename fisch::vx::Omnibus::coordinate_type,
	    EventRecordingConfig::write_config_size_in_words>
	    ref_write_addresses = {typename fisch::vx::Omnibus::coordinate_type{0x8000'0005ul}};

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

	// Encode
	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_TRUE(data.at(0).get() == 1);

	// Decode back
	EventRecordingConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(EventRecordingConfig, CerealizeCoverage)
{
	EventRecordingConfig obj1, obj2;
	obj1.set_enable_event_recording(true);

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
