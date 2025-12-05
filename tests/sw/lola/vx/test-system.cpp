#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "lola/vx/system.h"

#include "cereal/types/lola/vx/system.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(ChipAndMultichipJboaLeafFPGA, EncodeDecode)
{
	ChipAndMultichipJboaLeafFPGA config;

	OutputRoutingTableEntryOnFPGA output_coordinate_entry(12);
	config.fpga.output_routing_table.entries[output_coordinate_entry].set_enable(true);
	config.fpga.output_routing_table.entries[output_coordinate_entry].set_label(
	    OutputRoutingTableEntry::Label(7));

	PPUMemory::words_type test_mem;
	std::for_each(test_mem.begin(), test_mem.end(), [](PPUMemoryWord& x) {
		x = PPUMemoryWord(PPUMemoryWord::Value(std::rand()));
	});
	config.chip.ppu_memory[PPUMemoryOnDLS()].set_words(test_mem);

	ChipAndMultichipJboaLeafFPGAOnSystem coordinate;

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
	ChipAndMultichipJboaLeafFPGA config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coordinate, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(ChipAndMultichipJboaLeafFPGA, CerealizeCoverage)
{
	ChipAndMultichipJboaLeafFPGA obj1, obj2;

	OutputRoutingTableEntryOnFPGA output_coordinate_entry(12);
	obj1.fpga.output_routing_table.entries[output_coordinate_entry].set_enable(true);
	obj1.fpga.output_routing_table.entries[output_coordinate_entry].set_label(
	    OutputRoutingTableEntry::Label(7));

	PPUMemory::words_type test_mem;
	std::for_each(test_mem.begin(), test_mem.end(), [](PPUMemoryWord& x) {
		x = PPUMemoryWord(PPUMemoryWord::Value(std::rand()));
	});
	obj1.chip.ppu_memory[PPUMemoryOnDLS()].set_words(test_mem);

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

TEST(ChipAndSinglechipFPGA, EncodeDecode)
{
	ChipAndSinglechipFPGA config;

	PPUMemory::words_type test_mem;
	std::for_each(test_mem.begin(), test_mem.end(), [](PPUMemoryWord& x) {
		x = PPUMemoryWord(PPUMemoryWord::Value(std::rand()));
	});
	config.chip.ppu_memory[PPUMemoryOnDLS()].set_words(test_mem);

	ChipAndSinglechipFPGAOnSystem coordinate;

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
	ChipAndSinglechipFPGA config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coordinate, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(ChipAndSinglechipFPGA, CerealizeCoverage)
{
	ChipAndSinglechipFPGA obj1, obj2;

	PPUMemory::words_type test_mem;
	std::for_each(test_mem.begin(), test_mem.end(), [](PPUMemoryWord& x) {
		x = PPUMemoryWord(PPUMemoryWord::Value(std::rand()));
	});
	obj1.chip.ppu_memory[PPUMemoryOnDLS()].set_words(test_mem);

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