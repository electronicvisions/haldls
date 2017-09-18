#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/container/v2/board.h"
#include "haldls/io/visitors.h"

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(FPGAConfig, Encode)
{
	FPGAConfig config;
	Unique const coord;

	config.set_enable_spike_router(true);

	std::array<std::uint32_t, 1> const ref_addresses = {{0x8020ul}};
	std::array<std::uint32_t, 1> const ref_data = {{0x8000008eul}};

	typedef std::vector<ocp_address_type> ocp_addresses_type;
	typedef std::vector<ocp_word_type> ocp_words_type;

	{ // write addresses
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			config, coord, haldls::io::WriteAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<std::uint32_t> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	//  --- with default settings ----------------------------------------------

	ocp_words_type ocp_data;
	visit_preorder(config, coord, haldls::io::EncodeVisitor<ocp_words_type>{ocp_data});

	std::vector<std::uint32_t> data;
	data.reserve(ocp_data.size());
	for (auto word : ocp_data)
		data.push_back(word.value);
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(SpikeRouter, Encode)
{
	SpikeRouter config;
	Unique const coord;

	// clang-format off
	std::array<std::uint32_t, 65> const ref_addresses = {{0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8031ul, 0x8030ul, 0x8030ul}};
	std::array<std::uint32_t, 65> const ref_data_default = {{0x0ul, 0x0ul, 0x0ul, 0x40ul, 0x0ul, 0x80ul, 0x0ul, 0xc0ul, 0x0ul, 0x100ul, 0x0ul, 0x140ul, 0x0ul, 0x180ul, 0x0ul, 0x1c0ul, 0x0ul, 0x200ul, 0x0ul, 0x240ul, 0x0ul, 0x280ul, 0x0ul, 0x2c0ul, 0x0ul, 0x300ul, 0x0ul, 0x340ul, 0x0ul, 0x380ul, 0x0ul, 0x3c0ul, 0x0ul, 0x400ul, 0x0ul, 0x440ul, 0x0ul, 0x480ul, 0x0ul, 0x4c0ul, 0x0ul, 0x500ul, 0x0ul, 0x540ul, 0x0ul, 0x580ul, 0x0ul, 0x5c0ul, 0x0ul, 0x600ul, 0x0ul, 0x640ul, 0x0ul, 0x680ul, 0x0ul, 0x6c0ul, 0x0ul, 0x700ul, 0x0ul, 0x740ul, 0x0ul, 0x780ul, 0x0ul, 0x7c0ul, 0x0ul}};
	// clang-format on

	typedef std::vector<ocp_address_type> ocp_addresses_type;
	typedef std::vector<ocp_word_type> ocp_words_type;

	{ // write addresses
		ocp_addresses_type ocp_addresses;
		visit_preorder(
			config, coord, haldls::io::WriteAddressVisitor<ocp_addresses_type>{ocp_addresses});

		std::vector<std::uint32_t> addresses;
		addresses.reserve(ocp_addresses.size());
		for (auto word : ocp_addresses)
			addresses.push_back(word.value);
		EXPECT_THAT(addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	//  --- with default settings ----------------------------------------------

	ocp_words_type ocp_data;
	visit_preorder(config, coord, haldls::io::EncodeVisitor<ocp_words_type>{ocp_data});

	std::vector<std::uint32_t> data;
	data.reserve(ocp_data.size());
	for (auto word : ocp_data)
		data.push_back(word.value);
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data_default));

	//  --- with squeeze mode enabled ------------------------------------------

	ocp_data.clear();
	data.clear();

	auto ref_data_squeeze = ref_data_default;
	ref_data_squeeze.back() = 0x134aa800ul;

	config.enable_squeeze_mode(SynapseBlock::Synapse::Address(42), SpikeRouter::Delay(1234));

	visit_preorder(config, coord, haldls::io::EncodeVisitor<ocp_words_type>{ocp_data});
	data.reserve(ocp_data.size());
	for (auto word : ocp_data)
		data.push_back(word.value);

	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data_squeeze));

	//  --- with arbitrary routing table ---------------------------------------

	// clang-format off
	std::array<std::uint32_t, 65> const ref_data = {{0xfffffffful, 0x2aul, 0xfffffffeul, 0x6aul, 0xfffffffdul, 0xaaul, 0xfffffffcul, 0xeaul, 0xfffffffbul, 0x12aul, 0xfffffffaul, 0x16aul, 0xfffffff9ul, 0x1aaul, 0xfffffff8ul, 0x1eaul, 0xfffffff7ul, 0x22aul, 0xfffffff6ul, 0x26aul, 0xfffffff5ul, 0x2aaul, 0xfffffff4ul, 0x2eaul, 0xfffffff3ul, 0x32aul, 0xfffffff2ul, 0x36aul, 0xfffffff1ul, 0x3aaul, 0xfffffff0ul, 0x3eaul, 0xffffffeful, 0x42aul, 0xffffffeeul, 0x46aul, 0xffffffedul, 0x4aaul, 0xffffffecul, 0x4eaul, 0xffffffebul, 0x52aul, 0xffffffeaul, 0x56aul, 0xffffffe9ul, 0x5aaul, 0xffffffe8ul, 0x5eaul, 0xffffffe7ul, 0x62aul, 0xffffffe6ul, 0x66aul, 0xffffffe5ul, 0x6aaul, 0xffffffe4ul, 0x6eaul, 0xffffffe3ul, 0x72aul, 0xffffffe2ul, 0x76aul, 0xffffffe1ul, 0x7aaul, 0xffffffe0ul, 0x7eaul, 0x0ul}};
	// clang-format on

	config.reset();
	ocp_data.clear();
	data.clear();

	for (auto const neuron : iter_all<NeuronOnDLS>()) {
		std::bitset<32> target_rows(neuron.value());
		target_rows.flip();
		config.set_neuron_route(neuron, SynapseBlock::Synapse::Address(42), target_rows);
	}

	visit_preorder(config, coord, haldls::io::EncodeVisitor<ocp_words_type>{ocp_data});
	data.reserve(ocp_data.size());
	for (auto word : ocp_data)
		data.push_back(word.value);

	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}
