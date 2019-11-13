#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "haldls/vx/ppu.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace fisch::vx;

typedef std::vector<OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<OmnibusChipOverJTAG> words_type;

TEST(PPUMemoryWord, General)
{
	test_generic_functionality_single_value<PPUMemoryWord>();
	test_hex_ostream_operator_single_value<PPUMemoryWord>();
}

TEST(PPUMemoryWord, EncodeDecode)
{
	PPUMemoryWord config;

	config.set_value(PPUMemoryWord::Value(555));

	PPUMemoryWordOnDLS coord(Enum(0x123));

	std::array<OmnibusChipOverJTAGAddress, PPUMemoryWord::config_size_in_words> ref_addresses = {
	    OmnibusChipOverJTAGAddress{0x02800123ul}};
	std::array<OmnibusChipOverJTAG, PPUMemoryWord::config_size_in_words> ref_data = {
	    OmnibusData{555ul}};

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

	PPUMemoryWord config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUMemoryWord, CerealizeCoverage)
{
	PPUMemoryWord obj1, obj2;
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
	auto min = PPUMemoryWordOnPPU(42);
	auto max = PPUMemoryWordOnPPU(108);
	auto coord = PPUMemoryBlockOnPPU(min, max);
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
	ASSERT_EQ(block_default.size(), PPUMemoryWordOnPPU::size);

	// test illegal calls
	EXPECT_THROW(
	    PPUMemoryBlockSize(halco::hicann_dls::vx::PPUMemoryWordOnPPU::size + 1),
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
	auto min = PPUMemoryWordOnPPU(0);
	auto max = PPUMemoryWordOnPPU(9);
	auto coord = PPUMemoryBlockOnPPU(min, max);
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
	auto const min = PPUMemoryWordOnPPU(42);
	auto const max = PPUMemoryWordOnPPU(50);
	auto const coord = PPUMemoryBlockOnPPU(min, max);
	auto const coord_on_dls = PPUMemoryBlockOnDLS(coord);
	PPUMemoryBlock config(coord.toPPUMemoryBlockSize());
	PPUMemoryBlock::words_type memory(config.size());

	std::array<OmnibusChipOverJTAGAddress, 9> ref_addresses;
	std::array<OmnibusChipOverJTAG, 9> ref_data{{}};

	ASSERT_EQ(ref_addresses.size(), memory.size());
	ASSERT_EQ(ref_data.size(), memory.size());


	for (size_t ii = 0; ii < memory.size(); ++ii) {
		ref_addresses[ii] = static_cast<OmnibusChipOverJTAGAddress>(0x02800000 + min.toEnum() + ii);
		ref_data[ii] = OmnibusChipOverJTAG(static_cast<OmnibusData>(50 + ii));
		memory[ii].set_value(PPUMemoryWord::Value(ref_data[ii].get()));
	}

	config.set_words(memory);

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
		    config, coord_on_dls, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
		    config, coord_on_dls, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;

	visit_preorder(config, coord_on_dls, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	EXPECT_THROW(
	    visit_preorder(
	        config,
	        PPUMemoryBlock::coordinate_type(
	            PPUMemoryBlockOnPPU(PPUMemoryWordOnPPU(40), PPUMemoryWordOnPPU(50))),
	        stadls::EncodeVisitor<words_type>{data}),
	    std::runtime_error);

	EXPECT_THROW(
	    visit_preorder(
	        config, PPUMemoryBlock::coordinate_type(PPUMemoryBlockOnPPU(max, min)),
	        stadls::EncodeVisitor<words_type>{data}),
	    std::runtime_error);

	PPUMemoryBlock config_copy(coord.toPPUMemoryBlockSize());
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord_on_dls, stadls::DecodeVisitor<words_type>{std::move(data)});
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
	PPUMemory::words_type test_mem;
	std::for_each(test_mem.begin(), test_mem.end(), [](PPUMemoryWord& x) {
		x = PPUMemoryWord(PPUMemoryWord::Value(std::rand()));
	});
	memory.set_words(test_mem);
	ASSERT_EQ(memory.get_words(), test_mem);

	memory.set_word(PPUMemoryWordOnPPU(14), PPUMemoryWord::Value(0x42u));
	ASSERT_EQ(memory.get_word(PPUMemoryWordOnPPU(14)), PPUMemoryWord::Value(0x42u));

	// test assign
	PPUMemory memory_eq = memory;
	PPUMemory memory_ne(memory);

	// test compare
	ASSERT_EQ(memory_eq, memory);
	memory_ne.set_word(PPUMemoryWordOnPPU(14), PPUMemoryWord::Value(0x43u));
	ASSERT_NE(memory_ne, memory);

	PPUMemory memory_from_file;

	EXPECT_NO_THROW(
	    memory_from_file.load_from_file("../haldls/tests/sw/haldls/ppu_test_binary.binary"));

	memory_from_file.load_from_file("../haldls/tests/sw/haldls/ppu_test_binary.binary");
	ASSERT_EQ(memory_from_file.get_word(PPUMemoryWordOnPPU(0)), PPUMemoryWord::Value(0x48000034));
	ASSERT_EQ(memory_from_file.get_word(PPUMemoryWordOnPPU(91)), PPUMemoryWord::Value(0x4e800020));
	for (auto word : iter_all<PPUMemoryWordOnPPU>()) {
		if (word >= 92) {
			ASSERT_EQ(memory_from_file.get_word(word), PPUMemoryWord::Value(0x0));
		}
	}
}

TEST(PPUMemory, SetGetBlock)
{
	PPUMemory memy;
	typename PPUMemory::words_type memy_words;
	std::fill_n(std::begin(memy_words) + 5, 23, PPUMemoryWord(PPUMemoryWord::Value(0xbeefdead)));
	memy.set_words(memy_words);
	PPUMemoryBlockOnPPU first_blocky_coord(PPUMemoryWordOnPPU(5), PPUMemoryWordOnPPU(27));
	PPUMemoryBlock blocky = memy.get_block(first_blocky_coord);
	for (auto word : blocky.get_words()) {
		ASSERT_EQ(word, PPUMemoryWord(PPUMemoryWord::Value(0xbeefdead)));
	}

	typename PPUMemoryBlock::words_type blocky_words(42);
	PPUMemoryBlock write_blocky(PPUMemoryBlockSize(42));
	std::fill(
	    std::begin(blocky_words), std::end(blocky_words),
	    PPUMemoryWord(PPUMemoryWord::Value(0xdeadbeef)));
	write_blocky.set_words(blocky_words);
	PPUMemoryBlockOnPPU wrong_blocky_coord(PPUMemoryWordOnPPU(4), PPUMemoryWordOnPPU(10));
	EXPECT_THROW(memy.set_block(wrong_blocky_coord, write_blocky), std::out_of_range);
	PPUMemoryBlockOnPPU right_blocky_coord(PPUMemoryWordOnPPU(4), PPUMemoryWordOnPPU(4 + 41));
	memy.set_block(right_blocky_coord, write_blocky);
	for (size_t i = 4; i < 4 + 42; i++) {
		ASSERT_EQ(memy.get_word(PPUMemoryWordOnPPU(i)), PPUMemoryWord::Value(0xdeadbeef));
	}
}

TEST(PPUMemory, EncodeDecode)
{
	PPUMemory config;

	PPUMemoryOnDLS coord_on_dls;

	PPUMemory::words_type memory;

	std::array<OmnibusChipOverJTAGAddress, halco::hicann_dls::vx::PPUMemoryWordOnPPU::size>
	    ref_addresses;
	std::array<OmnibusChipOverJTAG, halco::hicann_dls::vx::PPUMemoryWordOnPPU::size> ref_data{{}};

	ASSERT_EQ(ref_addresses.size(), memory.size());
	ASSERT_EQ(ref_data.size(), memory.size());

	for (size_t ii = 0; ii < memory.size(); ++ii) {
		ref_addresses[ii] = static_cast<OmnibusChipOverJTAGAddress>(0x02800000 + ii);
		ref_data[ii] = OmnibusChipOverJTAG(static_cast<OmnibusData>(50 + ii));
		memory[ii].set_value(PPUMemoryWord::Value(ref_data[ii].get()));
	}

	config.set_words(memory);

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(
		    config, coord_on_dls, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(
		    config, coord_on_dls, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord_on_dls, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	PPUMemory config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord_on_dls, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUMemory, CerealizeCoverage)
{
	PPUMemory obj1, obj2;
	PPUMemory::words_type data;
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

TEST(PPUControlRegister, General)
{
	PPUControlRegister r1, r2;
	r1.set_inhibit_reset(true);
	ASSERT_TRUE(r1.get_inhibit_reset());
	r1.set_force_clock_on(true);
	ASSERT_TRUE(r1.get_force_clock_on());
	r1.set_force_clock_off(true);
	ASSERT_TRUE(r1.get_force_clock_off());
	r1.set_cache_controller_enable(true);
	ASSERT_TRUE(r1.get_cache_controller_enable());
	r1.set_inhibit_reset(false);
	ASSERT_FALSE(r1.get_inhibit_reset());
	r1.set_force_clock_on(false);
	ASSERT_FALSE(r1.get_force_clock_on());
	r1.set_force_clock_off(false);
	ASSERT_FALSE(r1.get_force_clock_off());
	r1.set_cache_controller_enable(false);
	ASSERT_FALSE(r1.get_cache_controller_enable());

	r2 = r1;
	ASSERT_EQ(r1, r2);
	ASSERT_TRUE(r1 == r2);
	ASSERT_FALSE(r1 != r1);
	r2.set_inhibit_reset(!r1.get_inhibit_reset());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);

	r2 = r1;
	r2.set_force_clock_on(!r1.get_force_clock_on());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);

	r2 = r1;
	r2.set_force_clock_off(!r1.get_force_clock_off());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);

	r2 = r1;
	r2.set_cache_controller_enable(!r1.get_cache_controller_enable());
	ASSERT_NE(r1, r2);
	ASSERT_TRUE(r1 != r2);
	ASSERT_FALSE(r1 == r2);
}

TEST(PPUControlRegister, EncodeDecode)
{
	PPUControlRegister config;

	config.set_cache_controller_enable(true);
	config.set_inhibit_reset(true);
	config.set_force_clock_on(false);
	config.set_force_clock_off(true);

	PPUControlRegisterOnDLS coord;

	std::array<OmnibusChipOverJTAGAddress, PPUControlRegister::config_size_in_words> ref_addresses =
	    {OmnibusChipOverJTAGAddress{0x2a00000}};
	std::array<OmnibusChipOverJTAG, PPUControlRegister::config_size_in_words> ref_data = {
	    OmnibusData{0b1101ul}};

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

	PPUControlRegister config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(PPUControlRegister, CerealizeCoverage)
{
	PPUControlRegister obj1, obj2;

	obj1.set_cache_controller_enable(!obj1.get_cache_controller_enable());
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

	std::array<OmnibusChipOverJTAGAddress, PPUStatusRegister::config_size_in_words> ref_addresses =
	    {OmnibusChipOverJTAGAddress{0x2a00001ul}};
	std::array<OmnibusChipOverJTAG, PPUStatusRegister::config_size_in_words> ref_data = {
	    OmnibusData{0b0ul}};

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

	PPUStatusRegister config_copy;
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
	ASSERT_FALSE(config_copy.get_sleep());

	data.clear();
	data.push_back(OmnibusData{0b1ul});
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_TRUE(config_copy.get_sleep());
}

TEST(PPUStatusRegister, CerealizeCoverage)
{
	PPUStatusRegister obj1, obj2;
	std::array<OmnibusChipOverJTAG, PPUStatusRegister::config_size_in_words> data = {
	    {static_cast<OmnibusData>(rand())}};
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
