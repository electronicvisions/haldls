#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "lola/vx/fpga.h"

#include "cereal/types/lola/vx/fpga.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(MultichipJboaLeafFPGA, EncodeDecode)
{
	MultichipJboaLeafFPGA config;

	OutputRoutingTableEntryOnFPGA output_coordinate_entry(12);
	config.output_routing_table.entries[output_coordinate_entry].set_enable(true);
	config.output_routing_table.entries[output_coordinate_entry].set_label(
	    OutputRoutingTableEntry::Label(7));

	InputRoutingTableEntryOnFPGA input_coordinate_entry(46);
	config.input_routing_table.entries[input_coordinate_entry].set_enable(true);
	config.input_routing_table.entries[input_coordinate_entry].set_label(
	    InputRoutingTableEntry::Label(3));

	FPGAOnChipAndMultichipJboaLeafFPGA coordinate;

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	addresses_type ref_addresses;
	visit_preorder(config, coordinate, stadls::WriteAddressVisitor<addresses_type>{ref_addresses});

	// Encode data from config.
	words_type ref_data;
	visit_preorder(config, coordinate, stadls::EncodeVisitor<words_type>{ref_data});

	// Write addresses
	addresses_type write_addresses;
	visit_preorder(
	    config, coordinate, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(write_addresses, ref_addresses);

	// Read addresses
	addresses_type read_addresses;
	visit_preorder(config, coordinate, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	EXPECT_THAT(read_addresses, ref_addresses);

	// Encode data from config.
	words_type data;
	visit_preorder(config, coordinate, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	// Decode data to config_copy
	MultichipJboaLeafFPGA config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coordinate, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(MultichipJboaLeafFPGA, CerealizeCoverage)
{
	MultichipJboaLeafFPGA obj1, obj2;

	OutputRoutingTableEntryOnFPGA output_coordinate_entry(12);
	obj2.output_routing_table.entries[output_coordinate_entry].set_enable(true);
	obj2.output_routing_table.entries[output_coordinate_entry].set_label(
	    OutputRoutingTableEntry::Label(7));

	InputRoutingTableEntryOnFPGA input_coordinate_entry(46);
	obj2.input_routing_table.entries[input_coordinate_entry].set_enable(true);
	obj2.input_routing_table.entries[input_coordinate_entry].set_label(
	    InputRoutingTableEntry::Label(3));

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

TEST(SinglechipFPGA, EncodeDecode)
{
	SinglechipFPGA config;

	FPGAOnChipAndSinglechipFPGA coordinate;

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	addresses_type ref_addresses;
	visit_preorder(config, coordinate, stadls::WriteAddressVisitor<addresses_type>{ref_addresses});

	// Encode data from config.
	words_type ref_data;
	visit_preorder(config, coordinate, stadls::EncodeVisitor<words_type>{ref_data});

	// Write addresses
	addresses_type write_addresses;
	visit_preorder(
	    config, coordinate, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(write_addresses, ref_addresses);

	// Read addresses
	addresses_type read_addresses;
	visit_preorder(config, coordinate, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	EXPECT_THAT(read_addresses, ref_addresses);

	// Encode data from config.
	words_type data;
	visit_preorder(config, coordinate, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	// Decode data to config_copy
	SinglechipFPGA config_copy;
	// ASSERT_NE(config, config_copy); TO-DO Change config
	visit_preorder(config_copy, coordinate, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(SinglechipFPGA, CerealizeCoverage)
{
	SinglechipFPGA obj1, obj2;

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
