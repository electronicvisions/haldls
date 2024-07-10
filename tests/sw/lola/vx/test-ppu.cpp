#include "lola/vx/ppu.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/lola/vx/ppu.h"
#include "haldls/vx/ppu.h"
#include <cstdlib>
#include <cereal/archives/json.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace lola::vx;
using namespace halco::hicann_dls::vx;

static std::string const test_ppu_program = TEST_PPU_PROGRAM;
/**
 * The source code to the pregenerated test program looks like:
 *
 * #include <s2pp.h>
 *
 * uint32_t a;
 *
 * double b;
 *
 * struct S
 * {
 *  int a;
 *  double b;
 * };
 *
 * void start()
 * {
 *  a = 3;
 *  b = 4.0;
 * }
 *
 */

TEST(PPUElfFile, General)
{
	EXPECT_NO_THROW(auto _ = PPUElfFile(test_ppu_program););

	PPUElfFile file(test_ppu_program);
	auto symbols = file.read_symbols();
	auto [block, external_block, external_dram_block] = file.read_program();

	// all numbers below might and will change on changes in build profile, compiler or runtime
	constexpr size_t program_num_words = 42;
	PPUMemoryBlockOnPPU symbol_a_position(PPUMemoryWordOnPPU(38), PPUMemoryWordOnPPU(38));
	PPUMemoryBlockOnPPU symbol_b_position(PPUMemoryWordOnPPU(40), PPUMemoryWordOnPPU(41));
	ExternalPPUMemoryBlockOnFPGA symbol___call_constructors_position(
	    ExternalPPUMemoryByteOnFPGA(0x0), ExternalPPUMemoryByteOnFPGA(0x0 + 0x78 - 1));
	ExternalPPUMemoryBlockOnFPGA symbol___call_destructors_position(
	    ExternalPPUMemoryByteOnFPGA(0xa0), ExternalPPUMemoryByteOnFPGA(0xa0 + 0x78 - 1));
	ExternalPPUMemoryBlockOnFPGA symbol___cxa_pure_virtual_position(
	    ExternalPPUMemoryByteOnFPGA(0x120), ExternalPPUMemoryByteOnFPGA(0x120 + 0x20 - 1));
	ExternalPPUMemoryBlockOnFPGA symbol__start_position(
	    ExternalPPUMemoryByteOnFPGA(0x80), ExternalPPUMemoryByteOnFPGA(0x80 + 0x1c - 1));
	ExternalPPUMemoryBlockOnFPGA symbol_start_position(
	    ExternalPPUMemoryByteOnFPGA(0x150), ExternalPPUMemoryByteOnFPGA(0x150 + 0x40 - 1));

	ASSERT_EQ(block.size(), program_num_words);

	PPUElfFile::symbols_type expected{
	    {"a", PPUProgram::Symbol{PPUProgram::Symbol::Type::object, symbol_a_position}},
	    {"b", PPUProgram::Symbol{PPUProgram::Symbol::Type::object, symbol_b_position}},
	    {"__call_constructors", PPUProgram::Symbol{PPUProgram::Symbol::Type::function,
	                                               symbol___call_constructors_position}},
	    {"__cxa_pure_virtual", PPUProgram::Symbol{PPUProgram::Symbol::Type::function,
	                                              symbol___cxa_pure_virtual_position}},
	    {"__call_destructors", PPUProgram::Symbol{PPUProgram::Symbol::Type::function,
	                                              symbol___call_destructors_position}},
	    {"_Z5startv",
	     PPUProgram::Symbol{PPUProgram::Symbol::Type::function, symbol_start_position}},
	    {"_start", PPUProgram::Symbol{PPUProgram::Symbol::Type::function, symbol__start_position}}};

	ASSERT_EQ(symbols, expected);
}

TEST(Symbol, General)
{
	EXPECT_NO_THROW(PPUProgram::Symbol());

	PPUProgram::Symbol sym;


	ASSERT_EQ(sym.type, PPUProgram::Symbol::Type::other);
	ASSERT_EQ(sym.coordinate, PPUProgram::Symbol::Coordinate(PPUMemoryBlockOnPPU()));

	sym.type = PPUProgram::Symbol::Type::object;
	ASSERT_EQ(sym.type, PPUProgram::Symbol::Type::object);

	PPUMemoryBlockOnPPU coord(PPUMemoryWordOnPPU(10), PPUMemoryWordOnPPU(14));
	sym.coordinate = coord;
	ASSERT_EQ(sym.coordinate, PPUProgram::Symbol::Coordinate(coord));

	PPUProgram::Symbol sym1(sym.type, sym.coordinate);
	ASSERT_EQ(sym1, sym);

	PPUProgram::Symbol sym2 = sym;
	PPUProgram::Symbol sym3;

	ASSERT_EQ(sym, sym2);
	ASSERT_EQ(sym == sym3, false);

	ASSERT_NE(sym2, sym3);
	ASSERT_EQ(sym2 != sym, false);
}

TEST(Symbol, CerealizeCoverage)
{
	PPUProgram::Symbol obj1, obj2;
	obj1.type = PPUProgram::Symbol::Type::function;
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
