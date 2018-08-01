#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/v2/ppu.h"
#include "stadls/visitors.h"

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

typedef std::vector<hardware_address_type> addresses_type;
typedef std::vector<hardware_word_type> words_type;

TEST(PPUMemoryWord, General)
{
	//test range
	EXPECT_NO_THROW(PPUMemoryWord::Value(0xffffffff));

	//test getter/setter
	PPUMemoryWord word;
	word.set(PPUMemoryWord::Value(145));
	ASSERT_EQ(word.get(),PPUMemoryWord::Value(145));

	//test assign
	PPUMemoryWord word_eq = word;
	PPUMemoryWord word_ne(PPUMemoryWord::Value(139));

	//test comparison
	ASSERT_EQ(word, word_eq);
	ASSERT_FALSE(word == word_ne);

	ASSERT_NE(word, word_ne);
	ASSERT_FALSE(word != word_eq);

	// test ostream operator
	std::stringstream out;
	word.set(PPUMemoryWord::Value(0xdeadbeef));
	out << word;
	ASSERT_EQ(out.str(), "0xdeadbeef");
}

TEST(PPUMemoryWord, EncodeDecode)
{
	PPUMemoryWord config;

	config.set(PPUMemoryWord::Value(555));

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
}

TEST(PPUMemory, EncodeDecode)
{
	PPUMemory config;

	Unique coord;

	PPUMemory::words_type memory{{}};

	std::array<hardware_address_type, halco::hicann_dls::v2::PPUMemoryWordOnDLS::size>
		ref_addresses{{}};
	std::array<hardware_word_type, halco::hicann_dls::v2::PPUMemoryWordOnDLS::size> ref_data{{}};

	ASSERT_EQ(ref_addresses.size(), memory.size());
	ASSERT_EQ(ref_data.size(), memory.size());

	for (size_t ii = 0; ii < memory.size(); ++ii) {
		ref_addresses[ii] = ii;
		ref_data[ii] = 50 + ii;
		memory[ii].set(PPUMemoryWord::Value(ref_data[ii]));
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

	Unique coord;

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

TEST(PPUStatusRegister, EncodeDecode)
{
	PPUStatusRegister config;

	Unique coord;

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
