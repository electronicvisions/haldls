#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "lola/vx/ppu.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(ExternalPPUMemoryBlock, General)
{
	auto min = ExternalPPUMemoryByteOnFPGA(42);
	auto max = ExternalPPUMemoryByteOnFPGA(108);
	auto coord = ExternalPPUMemoryBlockOnFPGA(min, max);
	ExternalPPUMemoryBlockSize const container_size = coord.toExternalPPUMemoryBlockSize();
	ExternalPPUMemoryBlock block(container_size);

	// test getter/setter
	ASSERT_EQ(block.size(), container_size);

	ExternalPPUMemoryBlock::bytes_type test_mem;
	for (size_t index = 0; index < container_size; index++) {
		test_mem.push_back(ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(std::rand() & 0xff)));
	}
	block.set_bytes(test_mem);
	ASSERT_EQ(block.get_bytes(), test_mem);

	size_t counter = container_size;
	for (auto word : test_mem) {
		counter--;
		block.at(counter) = word;
		ASSERT_EQ(block.at(counter), word);
	}

	// test default ctor
	ExternalPPUMemoryBlock block_default;
	ASSERT_EQ(block_default.size(), ExternalPPUMemoryByteOnFPGA::size);

	// test illegal calls
	EXPECT_THROW(
	    ExternalPPUMemoryBlockSize(halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA::size + 1),
	    std::overflow_error);
	ExternalPPUMemoryBlock::bytes_type to_small_vec(container_size - 1);
	ExternalPPUMemoryBlock::bytes_type to_big_vec(container_size + 1);
	EXPECT_THROW(block.set_bytes(to_small_vec), std::range_error);
	EXPECT_THROW(block.set_bytes(to_big_vec), std::range_error);
	EXPECT_THROW(block.at(max.toEnum() + 1), std::out_of_range);
	// test assign
	ExternalPPUMemoryBlock block_eq = block;
	ExternalPPUMemoryBlock block_ne(block);

	// test compare
	ASSERT_EQ(block_eq, block);
	block_ne.at(2) = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x43u));
	ASSERT_NE(block_ne, block);
}

TEST(ExternalPPUMemoryBlock, Subblock)
{
	auto min = ExternalPPUMemoryByteOnFPGA(0);
	auto max = ExternalPPUMemoryByteOnFPGA(9);
	auto coord = ExternalPPUMemoryBlockOnFPGA(min, max);
	ExternalPPUMemoryBlockSize const container_size = coord.toExternalPPUMemoryBlockSize();
	ExternalPPUMemoryBlock block(container_size);

	EXPECT_THROW(block.get_subblock(10, ExternalPPUMemoryBlockSize(1)), std::out_of_range);
	EXPECT_THROW(
	    block.get_subblock(0, ExternalPPUMemoryBlockSize(container_size + 1)), std::out_of_range);
	EXPECT_THROW(block.get_subblock(1, container_size), std::out_of_range);

	ASSERT_EQ(block.get_subblock(0, ExternalPPUMemoryBlockSize(3)).size(), 3);

	ExternalPPUMemoryBlock other(ExternalPPUMemoryBlockSize(3));
	for (size_t i = 0; i < other.size(); ++i) {
		other.at(i) = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(1));
	}

	EXPECT_THROW(block.set_subblock(8, other), std::out_of_range);

	block.set_subblock(2, other);

	ExternalPPUMemoryBlock expect(container_size);
	ASSERT_EQ(expect.at(2), ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0)));
	ASSERT_EQ(expect.at(3), ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0)));
	ASSERT_EQ(expect.at(4), ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0)));
	expect.at(2) = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(1));
	expect.at(3) = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(1));
	expect.at(4) = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(1));

	ASSERT_EQ(block, expect);
}

TEST(ExternalPPUMemoryBlock, EncodeDecode)
{
	auto const min = ExternalPPUMemoryByteOnFPGA(42);
	auto const max = ExternalPPUMemoryByteOnFPGA(50);
	auto const coord = ExternalPPUMemoryBlockOnFPGA(min, max);
	ExternalPPUMemoryBlock config(coord.toExternalPPUMemoryBlockSize());
	ExternalPPUMemoryBlock::bytes_type memory(config.size());

	std::array<OmnibusAddress, 9> ref_addresses;
	std::array<fisch::vx::word_access_type::Omnibus, 9> ref_data;

	ASSERT_EQ(ref_addresses.size(), memory.size());
	ASSERT_EQ(ref_data.size(), memory.size());


	for (size_t ii = 0; ii < memory.size(); ++ii) {
		ref_addresses[ii] =
		    static_cast<OmnibusAddress>(0x8000'0000 + ((min.toEnum() + ii) / sizeof(uint32_t)));
		fisch::vx::word_access_type::Omnibus::ByteEnables byte_enables{};
		byte_enables[(sizeof(uint32_t) - 1) - ((min.toEnum() + ii) % sizeof(uint32_t))] = true;
		ref_data[ii] = fisch::vx::word_access_type::Omnibus(
		    (50 + ii)
		        << (((sizeof(uint32_t) - 1) - ((min.toEnum() + ii) % sizeof(uint32_t))) * CHAR_BIT),
		    byte_enables);
		memory[ii].set_value(ExternalPPUMemoryByte::Value(50 + ii));
	}

	config.set_bytes(memory);

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;
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
	        config,
	        ExternalPPUMemoryBlock::coordinate_type(ExternalPPUMemoryBlockOnFPGA(
	            ExternalPPUMemoryByteOnFPGA(40), ExternalPPUMemoryByteOnFPGA(50))),
	        stadls::EncodeVisitor<words_type>{data}),
	    std::runtime_error);

	EXPECT_THROW(
	    visit_preorder(
	        config, ExternalPPUMemoryBlock::coordinate_type(ExternalPPUMemoryBlockOnFPGA(max, min)),
	        stadls::EncodeVisitor<words_type>{data}),
	    std::runtime_error);

	ExternalPPUMemoryBlock config_copy(coord.toExternalPPUMemoryBlockSize());
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(ExternalPPUMemoryBlock, CerealizeCoverage)
{
	ExternalPPUMemoryBlock obj1, obj2;
	auto data = obj1.get_bytes();
	for (auto& byte : data) {
		byte =
		    ExternalPPUMemoryByte(draw_ranged_non_default_value<ExternalPPUMemoryByte::Value>(0));
	}
	obj1.set_bytes(data);

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

TEST(ExternalPPUMemoryBlock, to_string)
{
	static std::mt19937 random_generator(123456789);
	ExternalPPUMemoryBlock block;
	auto bytes = block.get_bytes();

	// Encoded "Hello World!\0Hel"
	bytes[0] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x48));
	bytes[1] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x65));
	bytes[2] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x6c));
	bytes[3] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x6c));
	bytes[4] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x6f));
	bytes[5] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x20));
	bytes[6] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x57));
	bytes[7] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x6f));
	bytes[8] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x72));
	bytes[9] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x6c));
	bytes[10] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x64));
	bytes[11] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x21));
	bytes[12] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x00));
	bytes[13] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x48));
	bytes[14] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x65));
	bytes[15] = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(0x6c));
	block.set_bytes(bytes);
	ASSERT_EQ(block.to_string(), "Hello World!");

	// test random memory contents
	std::uniform_int_distribution<std::mt19937::result_type> dist(
	    ExternalPPUMemoryByte::Value::min, ExternalPPUMemoryByte::Value::max);
	for (unsigned int i = 0; i < 10000; ++i) {
		for (auto& byte : bytes) {
			byte = ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value(dist(random_generator)));
		}
		block.set_bytes(bytes);
		EXPECT_NO_THROW(block.to_string());
	}
}
