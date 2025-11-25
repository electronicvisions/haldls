#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "lola/vx/routing_table.h"

#include "cereal/types/lola/vx/routing_table.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(OutputRoutingTable, EncodeDecode)
{
	OutputRoutingTable config;

	OutputRoutingTableEntryOnFPGA coordinate_entry_0(0);
	config.entries[coordinate_entry_0].set_enable(true);
	config.entries[coordinate_entry_0].set_label(OutputRoutingTableEntry::Label(7));

	OutputRoutingTableEntryOnFPGA coordinate_entry_1(12);
	config.entries[coordinate_entry_1].set_enable(true);
	config.entries[coordinate_entry_1].set_label(OutputRoutingTableEntry::Label(5));

	OutputRoutingTableEntryOnFPGA coordinate_entry_2(507);
	config.entries[coordinate_entry_2].set_enable(false);
	config.entries[coordinate_entry_2].set_label(OutputRoutingTableEntry::Label(10));

	OutputRoutingTableOnFPGA coordinate;

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
	OutputRoutingTable config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coordinate, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(OutputRoutingTable, CerealizeCoverage)
{
	OutputRoutingTable obj1, obj2;

	OutputRoutingTableEntryOnFPGA coordinate_entry_0(0);
	obj1.entries[coordinate_entry_0].set_enable(true);
	obj1.entries[coordinate_entry_0].set_label(OutputRoutingTableEntry::Label(7));

	OutputRoutingTableEntryOnFPGA coordinate_entry_1(12);
	obj1.entries[coordinate_entry_1].set_enable(true);
	obj1.entries[coordinate_entry_1].set_label(OutputRoutingTableEntry::Label(5));

	OutputRoutingTableEntryOnFPGA coordinate_entry_2(507);
	obj1.entries[coordinate_entry_2].set_enable(false);
	obj1.entries[coordinate_entry_2].set_label(OutputRoutingTableEntry::Label(10));

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


TEST(InputRoutingTable, EncodeDecode)
{
	InputRoutingTable config;

	InputRoutingTableEntryOnFPGA coordinate_entry_0(0);
	config.entries[coordinate_entry_0].set_enable(true);
	config.entries[coordinate_entry_0].set_label(InputRoutingTableEntry::Label(7));

	InputRoutingTableEntryOnFPGA coordinate_entry_1(12);
	config.entries[coordinate_entry_1].set_enable(true);
	config.entries[coordinate_entry_1].set_label(InputRoutingTableEntry::Label(5));

	InputRoutingTableEntryOnFPGA coordinate_entry_2(507);
	config.entries[coordinate_entry_2].set_enable(false);
	config.entries[coordinate_entry_2].set_label(InputRoutingTableEntry::Label(10));

	InputRoutingTableOnFPGA coordinate;

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
	InputRoutingTable config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coordinate, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(InputRoutingTable, CerealizeCoverage)
{
	InputRoutingTable obj1, obj2;

	InputRoutingTableEntryOnFPGA coordinate_entry_0(0);
	obj1.entries[coordinate_entry_0].set_enable(true);
	obj1.entries[coordinate_entry_0].set_label(InputRoutingTableEntry::Label(7));

	InputRoutingTableEntryOnFPGA coordinate_entry_1(12);
	obj1.entries[coordinate_entry_1].set_enable(true);
	obj1.entries[coordinate_entry_1].set_label(InputRoutingTableEntry::Label(5));

	InputRoutingTableEntryOnFPGA coordinate_entry_2(507);
	obj1.entries[coordinate_entry_2].set_enable(false);
	obj1.entries[coordinate_entry_2].set_label(InputRoutingTableEntry::Label(10));

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