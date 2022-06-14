#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/v3/quad.h"
#include "halco/hicann-dls/vx/v3/switch_rows.h"
#include "halco/hicann-dls/vx/v3/synram.h"
#include "haldls/vx/v3/synapse.h"
#include "stadls/visitors.h"
#include "test-helper.h"


using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx::v3;
using namespace halco::common;

TEST(ColumnCorrelationQuad_ColumnCorrelationSwitch, General)
{
	ColumnCorrelationQuad::ColumnCorrelationSwitch correlation_switch;

	{
		auto value = !correlation_switch.get_enable_internal_causal();
		correlation_switch.set_enable_internal_causal(value);
		ASSERT_EQ(correlation_switch.get_enable_internal_causal(), value);
	}

	{
		auto value = !correlation_switch.get_enable_internal_acausal();
		correlation_switch.set_enable_internal_acausal(value);
		ASSERT_EQ(correlation_switch.get_enable_internal_acausal(), value);
	}

	{
		auto value = !correlation_switch.get_enable_debug_causal();
		correlation_switch.set_enable_debug_causal(value);
		ASSERT_EQ(correlation_switch.get_enable_debug_causal(), value);
	}

	{
		auto value = !correlation_switch.get_enable_debug_acausal();
		correlation_switch.set_enable_debug_acausal(value);
		ASSERT_EQ(correlation_switch.get_enable_debug_acausal(), value);
	}

	{
		auto value = !correlation_switch.get_enable_cadc_neuron_readout_causal();
		correlation_switch.set_enable_cadc_neuron_readout_causal(value);
		ASSERT_EQ(correlation_switch.get_enable_cadc_neuron_readout_causal(), value);
	}

	{
		auto value = !correlation_switch.get_enable_cadc_neuron_readout_acausal();
		correlation_switch.set_enable_cadc_neuron_readout_acausal(value);
		ASSERT_EQ(correlation_switch.get_enable_cadc_neuron_readout_acausal(), value);
	}

	ColumnCorrelationQuad::ColumnCorrelationSwitch correlation_switch_eq = correlation_switch;
	ColumnCorrelationQuad::ColumnCorrelationSwitch correlation_switch_ne;

	ASSERT_EQ(correlation_switch, correlation_switch_eq);
	ASSERT_EQ(correlation_switch == correlation_switch_ne, false);

	ASSERT_NE(correlation_switch, correlation_switch_ne);
	ASSERT_EQ(correlation_switch != correlation_switch_eq, false);
}

TEST(ColumnCorrelationQuad_ColumnCorrelationSwitch, CerealizeCoverage)
{
	ColumnCorrelationQuad::ColumnCorrelationSwitch obj1, obj2;

	{
		auto value = !obj1.get_enable_internal_causal();
		obj1.set_enable_internal_causal(value);
	}

	{
		auto value = !obj1.get_enable_internal_acausal();
		obj1.set_enable_internal_acausal(value);
	}

	{
		auto value = !obj1.get_enable_debug_causal();
		obj1.set_enable_debug_causal(value);
	}

	{
		auto value = !obj1.get_enable_debug_acausal();
		obj1.set_enable_debug_acausal(value);
	}

	{
		auto value = !obj1.get_enable_cadc_neuron_readout_causal();
		obj1.set_enable_cadc_neuron_readout_causal(value);
	}

	{
		auto value = !obj1.get_enable_cadc_neuron_readout_acausal();
		obj1.set_enable_cadc_neuron_readout_acausal(value);
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

TEST(ColumnCorrelationQuad, General)
{
	ColumnCorrelationQuad block;
	ColumnCorrelationQuad::ColumnCorrelationSwitch correlation_switch;
	correlation_switch.set_enable_internal_causal(!correlation_switch.get_enable_internal_causal());

	block.set_switch(EntryOnQuad(3), correlation_switch);
	ASSERT_EQ(block.get_switch(EntryOnQuad(3)), correlation_switch);

	ColumnCorrelationQuad block_eq = block;
	ColumnCorrelationQuad block_ne(block);
	block_ne.set_switch(EntryOnQuad(2), correlation_switch);

	ASSERT_EQ(block, block_eq);
	ASSERT_FALSE(block == block_ne);

	ASSERT_NE(block, block_ne);
	ASSERT_FALSE(block != block_eq);
}

TEST(ColumnCorrelationQuad, EncodeDecode)
{
	EntryOnQuad correlation_switch_coord(2);
	ColumnCorrelationQuadOnDLS block_coord(ColumnCorrelationQuadOnSynram(X(3)), SynramOnDLS::top);
	ColumnCorrelationQuad correlation_switch_block;
	ColumnCorrelationQuad::ColumnCorrelationSwitch correlation_switch;
	correlation_switch.set_enable_internal_causal(true);
	correlation_switch.set_enable_debug_acausal(true);
	correlation_switch.set_enable_cadc_neuron_readout_acausal(true);
	correlation_switch_block.set_switch(correlation_switch_coord, correlation_switch);

	std::array<halco::hicann_dls::vx::OmnibusAddress, ColumnCorrelationQuad::config_size_in_words>
	    ref_addresses = {{halco::hicann_dls::vx::OmnibusAddress(0x02cf'8083),
	                      halco::hicann_dls::vx::OmnibusAddress(0x02cf'80c3),
	                      halco::hicann_dls::vx::OmnibusAddress(0x02c0'4003),
	                      halco::hicann_dls::vx::OmnibusAddress(0x02c4'4003)}};
	std::array<fisch::vx::word_access_type::Omnibus, ColumnCorrelationQuad::config_size_in_words>
	    ref_data = {
	        {fisch::vx::word_access_type::Omnibus(0x0040'0000),
	         fisch::vx::word_access_type::Omnibus(0x0080'0000),
	         fisch::vx::word_access_type::Omnibus(0x0000'0000),
	         fisch::vx::word_access_type::Omnibus(0x0020'0000)}};

	{ // write addresses
		std::vector<halco::hicann_dls::vx::OmnibusAddress> write_addresses;
		visit_preorder(
		    correlation_switch_block, block_coord,
		    stadls::WriteAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusAddress> >{
		        write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<halco::hicann_dls::vx::OmnibusAddress> read_addresses;
		visit_preorder(
		    correlation_switch_block, block_coord,
		    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusAddress> >{
		        read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::word_access_type::Omnibus> data;
	visit_preorder(
	    correlation_switch_block, block_coord,
	    stadls::EncodeVisitor<std::vector<fisch::vx::word_access_type::Omnibus> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	ColumnCorrelationQuad block_copy;
	ASSERT_NE(correlation_switch_block, block_copy);
	visit_preorder(
	    block_copy, block_coord,
	    stadls::DecodeVisitor<std::vector<fisch::vx::word_access_type::Omnibus> >{std::move(data)});
	ASSERT_EQ(correlation_switch_block, block_copy);
}

TEST(ColumnCorrelationQuad, CerealizeCoverage)
{
	ColumnCorrelationQuad obj1, obj2;
	for (auto correlation_switch_coord : iter_all<halco::hicann_dls::vx::EntryOnQuad>()) {
		ColumnCorrelationQuad::ColumnCorrelationSwitch correlation_switch;
		correlation_switch.set_enable_internal_causal(true);
		obj1.set_switch(correlation_switch_coord, correlation_switch);
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
