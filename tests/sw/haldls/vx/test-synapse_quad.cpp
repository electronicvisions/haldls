#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/synapse.h"

#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"


using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(SynapseQuad_Synapse, General)
{
	SynapseQuad::Synapse synapse;

	EXPECT_ANY_THROW(SynapseQuad::Synapse::Weight(64));
	EXPECT_NO_THROW(SynapseQuad::Synapse::Weight(63));
	EXPECT_ANY_THROW(SynapseQuad::Synapse::Address(64));
	EXPECT_NO_THROW(SynapseQuad::Synapse::Address(63));

	synapse.set_weight(SynapseQuad::Synapse::Weight(63));
	ASSERT_EQ(synapse.get_weight(), SynapseQuad::Synapse::Weight(63));

	synapse.set_address(SynapseQuad::Synapse::Address(63));
	ASSERT_EQ(synapse.get_address(), SynapseQuad::Synapse::Address(63));

	synapse.set_time_calib(SynapseQuad::Synapse::TimeCalib(0x2));
	ASSERT_EQ(synapse.get_time_calib(), SynapseQuad::Synapse::TimeCalib(0x2));

	synapse.set_amp_calib(SynapseQuad::Synapse::AmpCalib(0x2));
	ASSERT_EQ(synapse.get_amp_calib(), SynapseQuad::Synapse::AmpCalib(0x2));

	SynapseQuad::Synapse synapse_eq = synapse;
	SynapseQuad::Synapse synapse_ne(synapse);
	synapse_ne.set_address(SynapseQuad::Synapse::Address(58));

	ASSERT_EQ(synapse, synapse_eq);
	ASSERT_EQ(synapse == synapse_ne, false);

	ASSERT_NE(synapse, synapse_ne);
	ASSERT_EQ(synapse != synapse_eq, false);
}

TEST(SynapseQuad_Synapse, CerealizeCoverage)
{
	SynapseQuad::Synapse obj1, obj2;
	obj1.set_weight(draw_ranged_non_default_value<SynapseQuad::Synapse::Weight>(obj1.get_weight()));
	obj1.set_address(
	    draw_ranged_non_default_value<SynapseQuad::Synapse::Address>(obj1.get_address()));
	obj1.set_time_calib(
	    draw_ranged_non_default_value<SynapseQuad::Synapse::TimeCalib>(obj1.get_time_calib()));
	obj1.set_amp_calib(
	    draw_ranged_non_default_value<SynapseQuad::Synapse::AmpCalib>(obj1.get_amp_calib()));

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

TEST(SynapseQuad, General)
{
	SynapseQuad block;
	SynapseQuad::Synapse synapse;
	synapse.set_weight(SynapseQuad::Synapse::Weight(1));
	synapse.set_address(SynapseQuad::Synapse::Address(1));

	block.set_synapse(EntryOnQuad(3), synapse);
	ASSERT_EQ(block.get_synapse(EntryOnQuad(3)), synapse);

	SynapseQuad block_eq = block;
	SynapseQuad block_ne(block);
	block_ne.set_synapse(EntryOnQuad(2), synapse);

	ASSERT_EQ(block, block_eq);
	ASSERT_FALSE(block == block_ne);

	ASSERT_NE(block, block_ne);
	ASSERT_FALSE(block != block_eq);
}

TEST(SynapseQuad, EncodeDecode)
{
	EntryOnQuad synapse_coord(2);
	SynapseQuadOnDLS block_coord(SynapseQuadOnSynram(X(3), Y(1)), SynramOnDLS::top);
	SynapseQuad synapse_block;
	SynapseQuad::Synapse synapse;
	synapse.set_weight(SynapseQuad::Synapse::Weight(61));
	synapse.set_address(SynapseQuad::Synapse::Address(63));
	synapse.set_time_calib(SynapseQuad::Synapse::TimeCalib(0x2));
	synapse.set_amp_calib(SynapseQuad::Synapse::AmpCalib(0x1));
	synapse_block.set_synapse(synapse_coord, synapse);

	std::array<halco::hicann_dls::vx::OmnibusChipAddress, SynapseQuad::config_size_in_words>
	    ref_addresses = {{halco::hicann_dls::vx::OmnibusChipAddress(0x02cf'0080),
	                      halco::hicann_dls::vx::OmnibusChipAddress(0x02cf'00c0)}};
	std::array<fisch::vx::OmnibusChip, SynapseQuad::config_size_in_words> ref_data = {
	    {fisch::vx::OmnibusData(0x00B7'0000), fisch::vx::OmnibusData(0x007F'0000)}};

	{ // write addresses
		std::vector<halco::hicann_dls::vx::OmnibusChipAddress> write_addresses;
		visit_preorder(
		    synapse_block, block_coord,
		    stadls::WriteAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipAddress> >{
		        write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<halco::hicann_dls::vx::OmnibusChipAddress> read_addresses;
		visit_preorder(
		    synapse_block, block_coord,
		    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipAddress> >{
		        read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::OmnibusChip> data;
	visit_preorder(
	    synapse_block, block_coord,
	    stadls::EncodeVisitor<std::vector<fisch::vx::OmnibusChip> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	SynapseQuad block_copy;
	ASSERT_NE(synapse_block, block_copy);
	visit_preorder(
	    block_copy, block_coord,
	    stadls::DecodeVisitor<std::vector<fisch::vx::OmnibusChip> >{std::move(data)});
	ASSERT_EQ(synapse_block, block_copy);
}

TEST(SynapseQuad, CerealizeCoverage)
{
	SynapseQuad obj1, obj2;
	for (auto syn : iter_all<halco::hicann_dls::vx::EntryOnQuad>()) {
		SynapseQuad::Synapse synapse;
		synapse.set_weight(
		    draw_ranged_non_default_value<SynapseQuad::Synapse::Weight>(synapse.get_weight()));
		synapse.set_address(
		    draw_ranged_non_default_value<SynapseQuad::Synapse::Address>(synapse.get_address()));
		synapse.set_time_calib(draw_ranged_non_default_value<SynapseQuad::Synapse::TimeCalib>(
		    synapse.get_time_calib()));
		synapse.set_amp_calib(
		    draw_ranged_non_default_value<SynapseQuad::Synapse::AmpCalib>(synapse.get_amp_calib()));
		obj1.set_synapse(syn, synapse);
	}

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

TEST(CorrelationReset, General)
{
	CorrelationReset config;

	CorrelationReset default_config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(CorrelationReset, EncodeDecode)
{
	CorrelationReset config;

	auto coord = CorrelationResetOnDLS(
	    SynapseQuadOnSynram(SynapseQuadColumnOnDLS(3), SynapseRowOnSynram(31)), SynramOnDLS(1));

	std::array<OmnibusChipOverJTAGAddress, CorrelationReset::write_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x03c907c0}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get(), 0x0);
	ASSERT_EQ(data.size(), CorrelationReset::write_config_size_in_words);
}

TEST(CorrelationReset, CerealizeCoverage)
{
	CorrelationReset c1, c2;
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}
