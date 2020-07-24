#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/v1/synapse.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/v1/omnibus.h"
#include "halco/hicann-dls/vx/v1/quad.h"
#include "haldls/vx/v1/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"


using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(SynapseQuad, General)
{
	SynapseQuad block;

	EXPECT_ANY_THROW(SynapseQuad::Weight(64));
	EXPECT_ANY_THROW(SynapseQuad::Label(64));
	EXPECT_ANY_THROW(SynapseQuad::TimeCalib(4));
	EXPECT_ANY_THROW(SynapseQuad::AmpCalib(4));
	{
		auto weights = block.get_weights();
		for (auto& weight : weights) {
			weight = draw_ranged_non_default_value<SynapseQuad::Weight>(0);
		}
		block.set_weights(weights);
		EXPECT_EQ(block.get_weights(), weights);
	}
	{
		auto labels = block.get_labels();
		for (auto& address : labels) {
			address = draw_ranged_non_default_value<SynapseQuad::Label>(0);
		}
		block.set_labels(labels);
		EXPECT_EQ(block.get_labels(), labels);
	}
	{
		auto time_calibs = block.get_time_calibs();
		for (auto& time_calib : time_calibs) {
			time_calib = draw_ranged_non_default_value<SynapseQuad::TimeCalib>(0);
		}
		block.set_time_calibs(time_calibs);
		EXPECT_EQ(block.get_time_calibs(), time_calibs);
	}
	{
		auto amp_calibs = block.get_amp_calibs();
		for (auto& amp_calib : amp_calibs) {
			amp_calib = draw_ranged_non_default_value<SynapseQuad::AmpCalib>(0);
		}
		block.set_amp_calibs(amp_calibs);
		EXPECT_EQ(block.get_amp_calibs(), amp_calibs);
	}

	SynapseQuad block_eq = block;
	SynapseQuad block_ne(block);
	auto weights = block_ne.get_weights();
	weights.at(EntryOnQuad(0)) = SynapseQuad::Weight(0);
	block_ne.set_weights(weights);

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
	{
		auto weights = synapse_block.get_weights();
		weights.at(synapse_coord) = SynapseQuad::Weight(61);
		synapse_block.set_weights(weights);
	}
	{
		auto labels = synapse_block.get_labels();
		labels.at(synapse_coord) = SynapseQuad::Label(63);
		synapse_block.set_labels(labels);
	}
	{
		auto time_calibs = synapse_block.get_time_calibs();
		time_calibs.at(synapse_coord) = SynapseQuad::TimeCalib(2);
		synapse_block.set_time_calibs(time_calibs);
	}
	{
		auto amp_calibs = synapse_block.get_amp_calibs();
		amp_calibs.at(synapse_coord) = SynapseQuad::AmpCalib(1);
		synapse_block.set_amp_calibs(amp_calibs);
	}

	std::array<halco::hicann_dls::vx::OmnibusAddress, SynapseQuad::config_size_in_words>
	    ref_addresses = {{halco::hicann_dls::vx::OmnibusAddress(0x02cf'0080),
	                      halco::hicann_dls::vx::OmnibusAddress(0x02cf'00c0)}};
	std::array<fisch::vx::Omnibus, SynapseQuad::config_size_in_words> ref_data = {
	    {fisch::vx::Omnibus(fisch::vx::OmnibusData(0x00B7'0000)),
	     fisch::vx::Omnibus(fisch::vx::OmnibusData(0x007F'0000))}};

	{ // write addresses
		std::vector<halco::hicann_dls::vx::OmnibusAddress> write_addresses;
		visit_preorder(
		    synapse_block, block_coord,
		    stadls::WriteAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusAddress> >{
		        write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<halco::hicann_dls::vx::OmnibusAddress> read_addresses;
		visit_preorder(
		    synapse_block, block_coord,
		    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusAddress> >{
		        read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::Omnibus> data;
	visit_preorder(
	    synapse_block, block_coord, stadls::EncodeVisitor<std::vector<fisch::vx::Omnibus> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	SynapseQuad block_copy;
	ASSERT_NE(synapse_block, block_copy);
	visit_preorder(
	    block_copy, block_coord,
	    stadls::DecodeVisitor<std::vector<fisch::vx::Omnibus> >{std::move(data)});
	ASSERT_EQ(synapse_block, block_copy);
}

TEST(SynapseQuad, CerealizeCoverage)
{
	SynapseQuad obj1, obj2;
	{
		auto weights = obj1.get_weights();
		for (auto& weight : weights) {
			weight = draw_ranged_non_default_value<SynapseQuad::Weight>(0);
		}
		obj1.set_weights(weights);
	}
	{
		auto labels = obj1.get_labels();
		for (auto& address : labels) {
			address = draw_ranged_non_default_value<SynapseQuad::Label>(0);
		}
		obj1.set_labels(labels);
	}
	{
		auto time_calibs = obj1.get_time_calibs();
		for (auto& time_calib : time_calibs) {
			time_calib = draw_ranged_non_default_value<SynapseQuad::TimeCalib>(0);
		}
		obj1.set_time_calibs(time_calibs);
	}
	{
		auto amp_calibs = obj1.get_amp_calibs();
		for (auto& amp_calib : amp_calibs) {
			amp_calib = draw_ranged_non_default_value<SynapseQuad::AmpCalib>(0);
		}
		obj1.set_amp_calibs(amp_calibs);
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
