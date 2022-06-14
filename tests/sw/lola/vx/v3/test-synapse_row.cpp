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

TEST(SynapseRow, General)
{
	SynapseRow config;

	// test getter/setter
	{
		auto value = config.weights;
		value[SynapseOnSynapseRow(23)] =
		    draw_ranged_non_default_value<SynapseRow::Weight>(value[SynapseOnSynapseRow(23)]);
		config.weights = value;
		EXPECT_EQ(config.weights, value);
	}

	{
		auto value = config.labels;
		value[SynapseOnSynapseRow(24)] =
		    draw_ranged_non_default_value<SynapseRow::Label>(value[SynapseOnSynapseRow(24)]);
		config.labels = value;
		EXPECT_EQ(config.labels, value);
	}

	{
		auto value = config.time_calibs;
		value[SynapseOnSynapseRow(25)] =
		    draw_ranged_non_default_value<SynapseRow::TimeCalib>(value[SynapseOnSynapseRow(25)]);
		config.time_calibs = value;
		EXPECT_EQ(config.time_calibs, value);
	}

	{
		auto value = config.amp_calibs;
		value[SynapseOnSynapseRow(26)] =
		    draw_ranged_non_default_value<SynapseRow::AmpCalib>(value[SynapseOnSynapseRow(26)]);
		config.amp_calibs = value;
		EXPECT_EQ(config.amp_calibs, value);
	}

	SynapseRow config_eq = config;
	SynapseRow config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(SynapseRow, CerealizeCoverage)
{
	SynapseRow obj1, obj2;
	obj1.weights[SynapseOnSynapseRow(23)] =
	    draw_ranged_non_default_value<SynapseRow::Weight>(obj1.weights[SynapseOnSynapseRow(23)]);
	obj1.labels[SynapseOnSynapseRow(24)] =
	    draw_ranged_non_default_value<SynapseRow::Label>(obj1.labels[SynapseOnSynapseRow(24)]);
	obj1.time_calibs[SynapseOnSynapseRow(25)] =
	    draw_ranged_non_default_value<SynapseRow::TimeCalib>(
	        obj1.time_calibs[SynapseOnSynapseRow(25)]);
	obj1.amp_calibs[SynapseOnSynapseRow(26)] = draw_ranged_non_default_value<SynapseRow::AmpCalib>(
	    obj1.amp_calibs[SynapseOnSynapseRow(26)]);

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

TEST(SynapseRow, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	SynapseRow config;
	config.weights[SynapseOnSynapseRow(23)] = SynapseRow::Weight(63);

	SynapseRowOnDLS coord(Enum(12));

	std::array<
	    typename addresses_type::value_type,
	    SynapseQuad::config_size_in_words * SynapseQuadColumnOnDLS::size>
	    ref_addresses;
	std::array<
	    fisch::vx::word_access_type::Omnibus,
	    SynapseQuad::config_size_in_words * SynapseQuadColumnOnDLS::size>
	    ref_data;
	for (auto c : iter_all<SynapseQuadColumnOnDLS>()) {
		auto syn = SynapseQuadOnDLS(
		    SynapseQuadOnSynram(coord.toSynapseRowOnSynram(), c), coord.toSynramOnDLS());

		SynapseQuad quad;
		ref_addresses[c.toEnum() * 2] = quad.addresses<typename addresses_type::value_type>(syn)[0];
		ref_addresses[c.toEnum() * 2 + 1] =
		    quad.addresses<typename addresses_type::value_type>(syn)[1];
		ref_data[c.toEnum() * 2] = fisch::vx::word_access_type::Omnibus(0);
		ref_data[c.toEnum() * 2 + 1] = fisch::vx::word_access_type::Omnibus(0);
	}
	ref_data[68] = fisch::vx::word_access_type::Omnibus(0x0000'003ful);

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

	SynapseRow config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
