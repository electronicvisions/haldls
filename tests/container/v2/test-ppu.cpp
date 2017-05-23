#include <gtest/gtest.h>

#include "haldls/container/v2/ppu.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(PPUMemoryWord, General)
{
	//test range
	EXPECT_NO_THROW(PPUMemoryWord::Value(0xffffffff));

	//test getter/setter
	PPUMemoryWord word;
	word.set_value(PPUMemoryWord::Value(145));
	ASSERT_EQ(word.get_value(),PPUMemoryWord::Value(145));

	//test assign
	PPUMemoryWord word_eq = word;
	PPUMemoryWord word_ne(PPUMemoryWord::Value(139));

	//test comparison
	ASSERT_EQ(word, word_eq);
	ASSERT_FALSE(word == word_ne);

	ASSERT_NE(word, word_ne);
	ASSERT_FALSE(word != word_eq);
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

	memory.set_word(PPUMemoryWordOnDLS(14), PPUMemoryWord(PPUMemoryWord::Value(0x42u)));
	ASSERT_EQ(memory.get_word(PPUMemoryWordOnDLS(14)), PPUMemoryWord(PPUMemoryWord::Value(0x42u)));

	// test assign
	PPUMemory memory_eq = memory;
	PPUMemory memory_ne(memory);

	// test compare
	ASSERT_EQ(memory_eq, memory);
	memory_ne.set_word(PPUMemoryWordOnDLS(14), PPUMemoryWord(PPUMemoryWord::Value(0x43u)));
	ASSERT_NE(memory_ne, memory);
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
