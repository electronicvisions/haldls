#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "halco/common/iter_all.h"
#include "haldls/v2/correlation.h"
#include "stadls/visitors.h"

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

typedef std::vector<hardware_address_type> addresses_type;
typedef std::vector<hardware_word_type> words_type;

TEST(CorrelationConfig, General)
{
	EXPECT_ANY_THROW(CorrelationConfig::Delay(4096));
	EXPECT_NO_THROW(CorrelationConfig::Delay(4095));

	CorrelationConfig config;
	ASSERT_EQ(config.get_sense_delay(), CorrelationConfig::Delay(0));
	ASSERT_EQ(config.get_reset_delay_1(), CorrelationConfig::Delay(0));
	ASSERT_EQ(config.get_reset_delay_2(), CorrelationConfig::Delay(0));

	// test getter/setter
	config.set_sense_delay(CorrelationConfig::Delay(12));
	ASSERT_EQ(config.get_sense_delay(), CorrelationConfig::Delay(12));

	config.set_reset_delay_1(CorrelationConfig::Delay(42));
	ASSERT_EQ(config.get_reset_delay_1(), CorrelationConfig::Delay(42));

	config.set_reset_delay_2(CorrelationConfig::Delay(69));
	ASSERT_EQ(config.get_reset_delay_2(), CorrelationConfig::Delay(69));


	CorrelationConfig config2 = config;
	CorrelationConfig config3 = config;
	config3.set_reset_delay_2(CorrelationConfig::Delay(78));


	ASSERT_EQ(config, config2);
	ASSERT_EQ(config == config3, false);

	ASSERT_NE(config, config3);
	ASSERT_EQ(config != config2, false);
}

TEST(CorrelationConfig, EncodeDecode)
{
	CorrelationConfig config;

	config.set_sense_delay(CorrelationConfig::Delay(12));
	config.set_reset_delay_1(CorrelationConfig::Delay(42));
	config.set_reset_delay_2(CorrelationConfig::Delay(69));

	CorrelationConfigOnDLS coord;

	std::array<hardware_address_type, 3> ref_addresses = {{0x0c000000, 0x0C000001, 0x0C000002}};
	std::array<hardware_word_type, 3> ref_data{{12, 42, 69}};

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

	CorrelationConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

template <class T>
void correlationblock_tester(
    SynapseOnSynapseBlock const& synapse_coord,
    SynapseBlockOnDLS const& block_coord,
    std::array<hardware_address_type, T::read_config_size_in_words> const& read_ref_addresses)
{
	T block;
	// some random test data
	typename T::Correlation correlation(0xAF);

	std::array<hardware_address_type, T::write_config_size_in_words> write_ref_addresses = {{}};
	std::array<hardware_word_type, T::read_config_size_in_words> read_ref_data = {{0x0000AF00}};
	std::array<hardware_word_type, T::write_config_size_in_words> write_ref_data = {{}};

	words_type data(read_ref_data.begin(), read_ref_data.end());
	visit_preorder(
	    block, block_coord,
	    stadls::DecodeVisitor<std::vector<hardware_word_type> >{std::move(data)});

	ASSERT_EQ(block.get_correlation(synapse_coord), correlation);

	T block_eq = block;
	T block_ne;

	ASSERT_EQ(block, block_eq);
	ASSERT_FALSE(block == block_ne);

	ASSERT_NE(block, block_ne);
	ASSERT_FALSE(block != block_eq);

	visit_preorder(
	    block, block_coord, stadls::EncodeVisitor<std::vector<hardware_word_type> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(write_ref_data));

	{ // write addresses
		std::vector<hardware_address_type> write_addresses;
		visit_preorder(
		    block, block_coord,
		    stadls::WriteAddressVisitor<std::vector<hardware_address_type> >{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(write_ref_addresses));
	}

	{ // read addresses
		std::vector<hardware_address_type> read_addresses;
		visit_preorder(
		    block, block_coord,
		    stadls::ReadAddressVisitor<std::vector<hardware_address_type> >{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(read_ref_addresses));
	}
}

TEST(CausalCorrelationBlock, General)
{
	// some random test coordinates
	SynapseOnSynapseBlock synapse_coord(2);
	SynapseBlockOnDLS block_coord(X(3), Y(1));
	// corresponding read address
	std::array<hardware_address_type, CausalCorrelationBlock::read_config_size_in_words>
	    read_ref_addresses = {{0x00018013}};
	correlationblock_tester<CausalCorrelationBlock>(synapse_coord, block_coord, read_ref_addresses);
}

TEST(AcausalCorrelationBlock, General)
{
	// some random test coordinates
	SynapseOnSynapseBlock synapse_coord(2);
	SynapseBlockOnDLS block_coord(X(3), Y(1));
	std::array<hardware_address_type, AcausalCorrelationBlock::read_config_size_in_words>
	    read_ref_addresses = {{0x0001C013}};
	// corresponding read address
	correlationblock_tester<AcausalCorrelationBlock>(
	    synapse_coord, block_coord, read_ref_addresses);
}
