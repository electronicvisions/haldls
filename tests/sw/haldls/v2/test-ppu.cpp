#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "test-helper.h"
#include "haldls/v2/ppu.h"
#include "stadls/visitors.h"

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

typedef std::vector<hardware_address_type> addresses_type;
typedef std::vector<hardware_word_type> words_type;

TEST(PPUMemoryWord, General)
{
	test_generic_functionality_single_value<PPUMemoryWord>();
	test_hex_ostream_operator_single_value<PPUMemoryWord>();
}

TEST(PPUMemoryWord, EncodeDecode)
{
	PPUMemoryWord config;

	config.set_value(PPUMemoryWord::Value(555));

	PPUMemoryWordOnDLS coord(0x123);

	std::array<hardware_address_type, PPUMemoryWord::config_size_in_words> ref_addresses = {{0x123ul}};
	std::array<hardware_word_type, PPUMemoryWord::config_size_in_words> ref_data = {{555ul}};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
			config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
			config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	PPUMemoryWord config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUMemoryWord, CerealizeCoverage)
{
	PPUMemoryWord obj1,obj2;
	obj1.set_value(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));

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

TEST(PPUMemoryBlock, General)
{
	auto min = PPUMemoryWordOnDLS(42);
	auto max = PPUMemoryWordOnDLS(108);
	auto coord = PPUMemoryBlockOnDLS(min, max);
	PPUMemoryBlockSize const container_size = coord.toPPUMemoryBlockSize();
	PPUMemoryBlock block(container_size);

	// test getter/setter
	ASSERT_EQ(block.size(), container_size);

	PPUMemoryBlock::words_type test_mem;
	for (size_t index = 0; index < container_size; index++) {
		test_mem.push_back(PPUMemoryWord(PPUMemoryWord::Value(std::rand())));
	}
	block.set_words(test_mem);
	ASSERT_EQ(block.get_words(), test_mem);

	size_t counter = container_size;
	for (auto word : test_mem) {
		counter--;
		block.at(counter) = word;
		ASSERT_EQ(block.at(counter), word);
	}

	// test default ctor
	PPUMemoryBlock block_default;
	ASSERT_EQ(block_default.size(), PPUMemoryWordOnDLS::size);

	// test illegal calls
	EXPECT_THROW(
	    PPUMemoryBlockSize(halco::hicann_dls::v2::PPUMemoryWordOnDLS::size + 1),
	    std::overflow_error);
	PPUMemoryBlock::words_type to_small_vec(container_size - 1);
	PPUMemoryBlock::words_type to_big_vec(container_size + 1);
	EXPECT_THROW(block.set_words(to_small_vec), std::range_error);
	EXPECT_THROW(block.set_words(to_big_vec), std::range_error);
	EXPECT_THROW(block.at(max.toEnum() + 1), std::out_of_range);
	// test assign
	PPUMemoryBlock block_eq = block;
	PPUMemoryBlock block_ne(block);

	// test compare
	ASSERT_EQ(block_eq, block);
	block_ne.at(2) = PPUMemoryWord(PPUMemoryWord::Value(0x43u));
	ASSERT_NE(block_ne, block);
}

TEST(PPUMemoryBlock, Subblock)
{
	auto min = PPUMemoryWordOnDLS(0);
	auto max = PPUMemoryWordOnDLS(9);
	auto coord = PPUMemoryBlockOnDLS(min, max);
	PPUMemoryBlockSize const container_size = coord.toPPUMemoryBlockSize();
	PPUMemoryBlock block(container_size);

	EXPECT_THROW(block.get_subblock(10, PPUMemoryBlockSize(1)), std::out_of_range);
	EXPECT_THROW(block.get_subblock(0, PPUMemoryBlockSize(container_size + 1)), std::out_of_range);
	EXPECT_THROW(block.get_subblock(1, container_size), std::out_of_range);

	ASSERT_EQ(block.get_subblock(0, PPUMemoryBlockSize(3)).size(), 3);

	PPUMemoryBlock other(PPUMemoryBlockSize(3));
	for (size_t i = 0; i < other.size(); ++i) {
		other.at(i) = PPUMemoryWord(PPUMemoryWord::Value(1));
	}

	EXPECT_THROW(block.set_subblock(8, other), std::out_of_range);

	block.set_subblock(2, other);

	PPUMemoryBlock expect(container_size);
	ASSERT_EQ(expect.at(2), PPUMemoryWord(PPUMemoryWord::Value(0)));
	ASSERT_EQ(expect.at(3), PPUMemoryWord(PPUMemoryWord::Value(0)));
	ASSERT_EQ(expect.at(4), PPUMemoryWord(PPUMemoryWord::Value(0)));
	expect.at(2) = PPUMemoryWord(PPUMemoryWord::Value(1));
	expect.at(3) = PPUMemoryWord(PPUMemoryWord::Value(1));
	expect.at(4) = PPUMemoryWord(PPUMemoryWord::Value(1));

	ASSERT_EQ(block, expect);
}

TEST(PPUMemoryBlock, EncodeDecode)
{
	auto const min = PPUMemoryWordOnDLS(42);
	auto const max = PPUMemoryWordOnDLS(50);
	auto const coord = PPUMemoryBlockOnDLS(min, max);
	PPUMemoryBlock config(coord.toPPUMemoryBlockSize());
	PPUMemoryBlock::words_type memory(config.size());

	std::array<hardware_address_type, 9> ref_addresses{{}};
	std::array<hardware_word_type, 9> ref_data{{}};

	ASSERT_EQ(ref_addresses.size(), memory.size());
	ASSERT_EQ(ref_data.size(), memory.size());

	for (size_t ii = 0; ii < memory.size(); ++ii) {
		ref_addresses[ii] = min.toEnum() + ii;
		ref_data[ii] = 50 + ii;
		memory[ii].set_value(PPUMemoryWord::Value(ref_data[ii]));
	}

	config.set_words(memory);

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

	EXPECT_THROW(
	    visit_preorder(
	        config, PPUMemoryBlock::coordinate_type(PPUMemoryWordOnDLS(40), PPUMemoryWordOnDLS(50)),
	        stadls::EncodeVisitor<words_type>{data}),
	    std::runtime_error);

	EXPECT_THROW(
	    visit_preorder(
	        config, PPUMemoryBlock::coordinate_type(max, min),
	        stadls::EncodeVisitor<words_type>{data}),
	    std::runtime_error);

	PPUMemoryBlock config_copy(coord.toPPUMemoryBlockSize());
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUMemoryBlock, CerealizeCoverage)
{
	PPUMemoryBlock obj1, obj2;
	auto data = obj1.get_words();
	for (auto& word : data) {
		word = PPUMemoryWord(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));
	}
	obj1.set_words(data);

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

TEST(PPUMemory, General)
{
	PPUMemory memory;

	// test getter/setter
	PPUMemory::words_type test_mem{{PPUMemoryWord(PPUMemoryWord::Value(0))}};
	std::for_each(
			test_mem.begin(),
			test_mem.end(),
			[](PPUMemoryWord& x) { x = PPUMemoryWord(PPUMemoryWord::Value(std::rand())); });
	memory.set_words(test_mem);
	ASSERT_EQ(memory.get_words(), test_mem);

	memory.set_word(PPUMemoryWordOnDLS(14), PPUMemoryWord::Value(0x42u));
	ASSERT_EQ(memory.get_word(PPUMemoryWordOnDLS(14)), PPUMemoryWord::Value(0x42u));

	// test assign
	PPUMemory memory_eq = memory;
	PPUMemory memory_ne(memory);

	// test compare
	ASSERT_EQ(memory_eq, memory);
	memory_ne.set_word(PPUMemoryWordOnDLS(14), PPUMemoryWord::Value(0x43u));
	ASSERT_NE(memory_ne, memory);

	PPUMemory memory_from_file;

	EXPECT_NO_THROW(
	    memory_from_file.load_from_file("../haldls/tests/sw/haldls/ppu_test_binary.binary"));

	memory_from_file.load_from_file("../haldls/tests/sw/haldls/ppu_test_binary.binary");
	ASSERT_EQ(memory_from_file.get_word(PPUMemoryWordOnDLS(0)), PPUMemoryWord::Value(0x48000034));
	ASSERT_EQ(memory_from_file.get_word(PPUMemoryWordOnDLS(91)), PPUMemoryWord::Value(0x4e800020));
	for (auto word: iter_all<PPUMemoryWordOnDLS>()) {
		if (word >= 92) {
			ASSERT_EQ(memory_from_file.get_word(word), PPUMemoryWord::Value(0x0));
		}
	}
}

TEST(PPUMemory, EncodeDecode)
{
	PPUMemory config;

	PPUMemoryOnDLS coord;

	PPUMemory::words_type memory{{}};

	std::array<hardware_address_type, halco::hicann_dls::v2::PPUMemoryWordOnDLS::size>
		ref_addresses{{}};
	std::array<hardware_word_type, halco::hicann_dls::v2::PPUMemoryWordOnDLS::size> ref_data{{}};

	ASSERT_EQ(ref_addresses.size(), memory.size());
	ASSERT_EQ(ref_data.size(), memory.size());

	for (size_t ii = 0; ii < memory.size(); ++ii) {
		ref_addresses[ii] = ii;
		ref_data[ii] = 50 + ii;
		memory[ii].set_value(PPUMemoryWord::Value(ref_data[ii]));
	}

	config.set_words(memory);

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
			config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
			config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	PPUMemory config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUMemory, CerealizeCoverage)
{
	PPUMemory obj1,obj2;
	PPUMemory::words_type data;
	for (auto& word: data) {
		word = PPUMemoryWord(draw_ranged_non_default_value<PPUMemoryWord::Value>(0));
	}
	obj1.set_words(data);

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

TEST(PPUControlRegister, General)
{
	PPUControlRegister ppu_control_register;

	// test getter/setter
	ASSERT_FALSE(ppu_control_register.get_inhibit_reset());
	ppu_control_register.set_inhibit_reset(true);
	ASSERT_TRUE(ppu_control_register.get_inhibit_reset());

	ASSERT_FALSE(ppu_control_register.get_force_clock_on());
	ppu_control_register.set_force_clock_on(true);
	ASSERT_TRUE(ppu_control_register.get_force_clock_on());

	ASSERT_FALSE(ppu_control_register.get_force_clock_off());
	ppu_control_register.set_force_clock_off(true);
	ASSERT_TRUE(ppu_control_register.get_force_clock_off());

	// test assign
	PPUControlRegister register_eq = ppu_control_register;
	PPUControlRegister register_ne(ppu_control_register);
	register_ne.set_force_clock_on(false);

	// test compare
	ASSERT_EQ(register_eq, ppu_control_register);
	ASSERT_FALSE(register_ne == ppu_control_register);
	ASSERT_NE(register_ne, ppu_control_register);
	ASSERT_FALSE(register_eq != ppu_control_register);
}

TEST(PPUControlRegister, EncodeDecode)
{
	PPUControlRegister config;

	config.set_inhibit_reset(true);
	config.set_force_clock_on(false);
	config.set_force_clock_off(true);

	PPUControlRegisterOnDLS coord;

	std::array<hardware_address_type, PPUControlRegister::config_size_in_words> ref_addresses = {{0x20000ul}};
	std::array<hardware_word_type, PPUControlRegister::config_size_in_words> ref_data = {{0b0101ul}};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
			config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
			config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	PPUControlRegister config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUControlRegister, CerealizeCoverage)
{
	PPUControlRegister obj1,obj2;
	obj1.set_inhibit_reset(!obj1.get_inhibit_reset());
	obj1.set_force_clock_on(!obj1.get_force_clock_on());
	obj1.set_force_clock_off(!obj1.get_force_clock_off());

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

TEST(PPUStatusRegister, EncodeDecode)
{
	PPUStatusRegister config;

	PPUStatusRegisterOnDLS coord;

	std::array<hardware_address_type, PPUStatusRegister::config_size_in_words> ref_addresses = {{0x20001ul}};
	std::array<hardware_word_type, PPUStatusRegister::config_size_in_words> ref_data = {{0b0ul}};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
			config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
			config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	PPUStatusRegister config_copy;
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
	ASSERT_FALSE(config_copy.get_sleep());

	data.clear();
	data.push_back(0b1ul);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_TRUE(config_copy.get_sleep());
}

TEST(PPUStatusRegister, CerealizeCoverage)
{
	PPUStatusRegister obj1,obj2;
	std::array<hardware_word_type, PPUStatusRegister::config_size_in_words> data = {{static_cast<uint32_t>(rand())}};
	obj1.decode(data);

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
