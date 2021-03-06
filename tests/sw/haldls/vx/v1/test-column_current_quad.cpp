#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/v1/quad.h"
#include "haldls/vx/v1/synapse.h"
#include "stadls/visitors.h"
#include "test-helper.h"


using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;
using namespace halco::common;

TEST(ColumnCurrentQuad_ColumnCurrentSwitch, General)
{
	ColumnCurrentQuad::ColumnCurrentSwitch current_switch;

	{
		auto value = !current_switch.get_enable_synaptic_current_excitatory();
		current_switch.set_enable_synaptic_current_excitatory(value);
		ASSERT_EQ(current_switch.get_enable_synaptic_current_excitatory(), value);
	}

	{
		auto value = !current_switch.get_enable_synaptic_current_inhibitory();
		current_switch.set_enable_synaptic_current_inhibitory(value);
		ASSERT_EQ(current_switch.get_enable_synaptic_current_inhibitory(), value);
	}

	{
		auto value = !current_switch.get_enable_debug_excitatory();
		current_switch.set_enable_debug_excitatory(value);
		ASSERT_EQ(current_switch.get_enable_debug_excitatory(), value);
	}

	{
		auto value = !current_switch.get_enable_debug_inhibitory();
		current_switch.set_enable_debug_inhibitory(value);
		ASSERT_EQ(current_switch.get_enable_debug_inhibitory(), value);
	}

	ColumnCurrentQuad::ColumnCurrentSwitch current_switch_eq = current_switch;
	ColumnCurrentQuad::ColumnCurrentSwitch current_switch_ne;

	ASSERT_EQ(current_switch, current_switch_eq);
	ASSERT_EQ(current_switch == current_switch_ne, false);

	ASSERT_NE(current_switch, current_switch_ne);
	ASSERT_EQ(current_switch != current_switch_eq, false);
}

TEST(ColumnCurrentQuad_ColumnCurrentSwitch, CerealizeCoverage)
{
	ColumnCurrentQuad::ColumnCurrentSwitch obj1, obj2;

	{
		auto value = !obj1.get_enable_synaptic_current_excitatory();
		obj1.set_enable_synaptic_current_excitatory(value);
	}

	{
		auto value = !obj1.get_enable_synaptic_current_inhibitory();
		obj1.set_enable_synaptic_current_inhibitory(value);
	}

	{
		auto value = !obj1.get_enable_debug_excitatory();
		obj1.set_enable_debug_excitatory(value);
	}

	{
		auto value = !obj1.get_enable_debug_inhibitory();
		obj1.set_enable_debug_inhibitory(value);
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

TEST(ColumnCurrentQuad, General)
{
	ColumnCurrentQuad block;
	ColumnCurrentQuad::ColumnCurrentSwitch current_switch;
	current_switch.set_enable_debug_excitatory(!current_switch.get_enable_debug_excitatory());

	block.set_switch(EntryOnQuad(3), current_switch);
	ASSERT_EQ(block.get_switch(EntryOnQuad(3)), current_switch);

	ColumnCurrentQuad block_eq = block;
	ColumnCurrentQuad block_ne(block);
	block_ne.set_switch(EntryOnQuad(2), current_switch);

	ASSERT_EQ(block, block_eq);
	ASSERT_FALSE(block == block_ne);

	ASSERT_NE(block, block_ne);
	ASSERT_FALSE(block != block_eq);
}

TEST(ColumnCurrentQuad, EncodeDecode)
{
	EntryOnQuad current_switch_coord(2);
	ColumnCurrentQuadOnDLS block_coord(ColumnCurrentQuadOnSynram(X(3)), SynramOnDLS::top);
	ColumnCurrentQuad current_switch_block;
	ColumnCurrentQuad::ColumnCurrentSwitch current_switch;
	current_switch.set_enable_debug_excitatory(true);
	current_switch.set_enable_synaptic_current_inhibitory(true);
	current_switch_block.set_switch(current_switch_coord, current_switch);

	std::array<halco::hicann_dls::vx::OmnibusAddress, ColumnCurrentQuad::config_size_in_words>
	    ref_addresses = {{halco::hicann_dls::vx::OmnibusAddress(0x02c1'4000),
	                      halco::hicann_dls::vx::OmnibusAddress(0x02c2'4000)}};
	std::array<fisch::vx::Omnibus, ColumnCurrentQuad::config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::OmnibusData(0x0002'0000)),
	    fisch::vx::Omnibus(fisch::vx::OmnibusData(0x0001'0000))};

	{ // write addresses
		std::vector<halco::hicann_dls::vx::OmnibusAddress> write_addresses;
		visit_preorder(
		    current_switch_block, block_coord,
		    stadls::WriteAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusAddress> >{
		        write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		std::vector<halco::hicann_dls::vx::OmnibusAddress> read_addresses;
		visit_preorder(
		    current_switch_block, block_coord,
		    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusAddress> >{
		        read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::Omnibus> data;
	visit_preorder(
	    current_switch_block, block_coord,
	    stadls::EncodeVisitor<std::vector<fisch::vx::Omnibus> >{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	ColumnCurrentQuad block_copy;
	ASSERT_NE(current_switch_block, block_copy);
	visit_preorder(
	    block_copy, block_coord,
	    stadls::DecodeVisitor<std::vector<fisch::vx::Omnibus> >{std::move(data)});
	ASSERT_EQ(current_switch_block, block_copy);
}

TEST(ColumnCurrentQuad, CerealizeCoverage)
{
	ColumnCurrentQuad obj1, obj2;
	for (auto current_switch_coord : iter_all<halco::hicann_dls::vx::EntryOnQuad>()) {
		ColumnCurrentQuad::ColumnCurrentSwitch current_switch;
		current_switch.set_enable_debug_excitatory(true);
		obj1.set_switch(current_switch_coord, current_switch);
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
