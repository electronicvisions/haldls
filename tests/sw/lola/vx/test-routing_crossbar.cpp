#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "lola/vx/routing_crossbar.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(Crossbar, EncodeDecode)
{
	Crossbar config;

	auto enable_event_counter = config.outputs.get_enable_event_counter();
	enable_event_counter[CrossbarOutputOnDLS()] = !enable_event_counter[CrossbarOutputOnDLS()];
	config.outputs.set_enable_event_counter(enable_event_counter);

	config.nodes[CrossbarNodeOnDLS(Enum(42))].set_enable_drop_counter(
	    !config.nodes[CrossbarNodeOnDLS(Enum(42))].get_enable_drop_counter());

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;
	addresses_type ref_addresses;
	words_type ref_data;

	visit_preorder(
	    config.outputs, CrossbarOutputConfigOnDLS(),
	    stadls::WriteAddressVisitor<addresses_type>{ref_addresses});
	visit_preorder(
	    config.outputs, CrossbarOutputConfigOnDLS(), stadls::EncodeVisitor<words_type>{ref_data});
	for (auto coord : iter_all<CrossbarNodeOnDLS>()) {
		visit_preorder(
		    config.nodes[coord], coord, stadls::WriteAddressVisitor<addresses_type>{ref_addresses});
		visit_preorder(config.nodes[coord], coord, stadls::EncodeVisitor<words_type>{ref_data});
	}

	CrossbarOnDLS coord;

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ref_addresses);
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ref_addresses);
	}

	words_type data;

	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	Crossbar config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(Crossbar, CerealizeCoverage)
{
	Crossbar obj1, obj2;

	auto enable_event_counter = obj1.outputs.get_enable_event_counter();
	enable_event_counter[CrossbarOutputOnDLS()] = !enable_event_counter[CrossbarOutputOnDLS()];
	obj1.outputs.set_enable_event_counter(enable_event_counter);

	obj1.nodes[CrossbarNodeOnDLS(Enum(42))].set_enable_drop_counter(
	    !obj1.nodes[CrossbarNodeOnDLS(Enum(42))].get_enable_drop_counter());

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
