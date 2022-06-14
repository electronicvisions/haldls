#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v3/synapse.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_heap_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/common.h"
#include "haldls/vx/v3/synapse.h"
#include "lola/vx/cerealization.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx::v3;
using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx::v3;
using namespace halco::common;

TEST(ColumnCorrelationRow, General)
{
	ColumnCorrelationRow config;

	// test getter/setter
	{
		auto value = config.values;
		value[SynapseOnSynapseRow(23)].set_enable_internal_causal(
		    !value[SynapseOnSynapseRow(23)].get_enable_internal_causal());
		config.values = value;
		EXPECT_EQ(config.values, value);
	}

	ColumnCorrelationRow config_eq = config;
	ColumnCorrelationRow config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(ColumnCorrelationRow, CerealizeCoverage)
{
	ColumnCorrelationRow obj1, obj2;
	obj1.values[SynapseOnSynapseRow(23)].set_enable_internal_causal(
	    !obj1.values[SynapseOnSynapseRow(23)].get_enable_internal_causal());

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

TEST(ColumnCorrelationRow, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	ColumnCorrelationRow config;
	config.values[SynapseOnSynapseRow(23)].set_enable_internal_causal(
	    !config.values[SynapseOnSynapseRow(23)].get_enable_internal_causal());

	ColumnCorrelationRowOnDLS coord(Enum(1));

	std::array<
	    typename addresses_type::value_type,
	    ColumnCorrelationQuad::config_size_in_words * SynapseQuadColumnOnDLS::size>
	    ref_addresses;
	std::array<
	    fisch::vx::word_access_type::Omnibus,
	    ColumnCorrelationQuad::config_size_in_words * SynapseQuadColumnOnDLS::size>
	    ref_data;
	for (auto c : iter_all<SynapseQuadColumnOnDLS>()) {
		auto syn =
		    ColumnCorrelationQuadOnDLS(c.toColumnCorrelationQuadOnSynram(), coord.toSynramOnDLS());

		ColumnCorrelationQuad quad;
		ref_addresses[c.toEnum() * 4] = quad.addresses<typename addresses_type::value_type>(syn)[0];
		ref_addresses[c.toEnum() * 4 + 1] =
		    quad.addresses<typename addresses_type::value_type>(syn)[1];
		ref_addresses[c.toEnum() * 4 + 2] =
		    quad.addresses<typename addresses_type::value_type>(syn)[2];
		ref_addresses[c.toEnum() * 4 + 3] =
		    quad.addresses<typename addresses_type::value_type>(syn)[3];
		ref_data[c.toEnum() * 4] = fisch::vx::word_access_type::Omnibus(0);
		ref_data[c.toEnum() * 4 + 1] = fisch::vx::word_access_type::Omnibus(0);
		ref_data[c.toEnum() * 4 + 2] = fisch::vx::word_access_type::Omnibus(0);
		ref_data[c.toEnum() * 4 + 3] = fisch::vx::word_access_type::Omnibus(0);
	}
	ref_data[136] = fisch::vx::word_access_type::Omnibus(0x0000'0040ul);

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

	ColumnCorrelationRow config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
