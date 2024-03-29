#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v4/synapse.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/lola/vx/synapse.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/v4/coordinates.h"
#include "haldls/vx/v4/common.h"
#include "haldls/vx/v4/synapse.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx::v4;
using namespace haldls::vx::v4;
using namespace halco::hicann_dls::vx::v4;
using namespace halco::common;

TEST(ColumnCurrentRow, General)
{
	ColumnCurrentRow config;

	// test getter/setter
	{
		auto value = config.values;
		value[SynapseOnSynapseRow(23)].set_enable_synaptic_current_excitatory(
		    !value[SynapseOnSynapseRow(23)].get_enable_synaptic_current_excitatory());
		config.values = value;
		EXPECT_EQ(config.values, value);
	}

	ColumnCurrentRow config_eq = config;
	ColumnCurrentRow config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(ColumnCurrentRow, CerealizeCoverage)
{
	ColumnCurrentRow obj1, obj2;
	obj1.values[SynapseOnSynapseRow(23)].set_enable_synaptic_current_excitatory(
	    !obj1.values[SynapseOnSynapseRow(23)].get_enable_synaptic_current_excitatory());

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

TEST(ColumnCurrentRow, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	ColumnCurrentRow config;
	config.values[SynapseOnSynapseRow(23)].set_enable_synaptic_current_excitatory(
	    !config.values[SynapseOnSynapseRow(23)].get_enable_synaptic_current_excitatory());

	ColumnCurrentRowOnDLS coord(Enum(1));

	std::array<
	    typename addresses_type::value_type,
	    ColumnCurrentQuad::config_size_in_words * SynapseQuadColumnOnDLS::size>
	    ref_addresses;
	std::array<
	    fisch::vx::word_access_type::Omnibus,
	    ColumnCurrentQuad::config_size_in_words * SynapseQuadColumnOnDLS::size>
	    ref_data;
	for (auto c : iter_all<SynapseQuadColumnOnDLS>()) {
		auto syn = ColumnCurrentQuadOnDLS(c.toColumnCurrentQuadOnSynram(), coord.toSynramOnDLS());

		ColumnCurrentQuad quad;
		ref_addresses[c.toEnum() * 2] = quad.addresses<typename addresses_type::value_type>(syn)[0];
		ref_addresses[c.toEnum() * 2 + 1] =
		    quad.addresses<typename addresses_type::value_type>(syn)[1];
		ref_data[c.toEnum() * 2] = fisch::vx::word_access_type::Omnibus(0);
		ref_data[c.toEnum() * 2 + 1] = fisch::vx::word_access_type::Omnibus(0);
	}
	ref_data[68] = fisch::vx::word_access_type::Omnibus(0x0000'0001ul);

	{
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}


	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	ColumnCurrentRow config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
