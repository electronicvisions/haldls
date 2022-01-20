#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v1/cadc.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v1/cadc.h"
#include "haldls/vx/v1/common.h"
#include "lola/vx/cerealization.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx::v1;
using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;
using namespace halco::common;

TEST(CADCSampleRow, General)
{
	CADCSampleRow config;

	// test getter/setter
	{
		auto value = config.causal;
		value[SynapseOnSynapseRow(23)] =
		    draw_ranged_non_default_value<CADCSampleRow::Value>(value[SynapseOnSynapseRow(23)]);
		config.causal = value;
		EXPECT_EQ(config.causal, value);
	}

	{
		auto value = config.acausal;
		value[SynapseOnSynapseRow(24)] =
		    draw_ranged_non_default_value<CADCSampleRow::Value>(value[SynapseOnSynapseRow(24)]);
		config.acausal = value;
		EXPECT_EQ(config.acausal, value);
	}

	CADCSampleRow config_eq = config;
	CADCSampleRow config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CADCSampleRow, CerealizeCoverage)
{
	CADCSampleRow obj1, obj2;
	obj1.causal[SynapseOnSynapseRow(23)] =
	    draw_ranged_non_default_value<CADCSampleRow::Value>(obj1.causal[SynapseOnSynapseRow(23)]);
	obj1.acausal[SynapseOnSynapseRow(24)] =
	    draw_ranged_non_default_value<CADCSampleRow::Value>(obj1.acausal[SynapseOnSynapseRow(24)]);

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

TEST(CADCSampleRow, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	CADCSampleRowOnDLS coord(Enum(12));

	std::array<
	    typename addresses_type::value_type,
	    CADCSampleQuad::read_config_size_in_words*(
	        SynapseQuadColumnOnDLS::size * CADCChannelType::size + 1)>
	    ref_addresses;
	words_type ref_data(
	    CADCSampleQuad::read_config_size_in_words *
	    (SynapseQuadColumnOnDLS::size * CADCChannelType::size + 1));
	for (auto& word : ref_data) {
		word = fisch::vx::word_access_type::Omnibus(0);
	}
	ref_data[11] = fisch::vx::word_access_type::Omnibus(0xf);
	ref_data[13 + SynapseQuadColumnOnDLS::size] = fisch::vx::word_access_type::Omnibus(0x10);

	for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
		CADCSampleQuadOnDLS quad_coord(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
		        CADCChannelType::causal, CADCReadoutType::buffered),
		    coord.toSynramOnDLS());
		CADCSampleQuad quad_config;
		ref_addresses[quad_column.toEnum() + 1] = quad_config.read_addresses(quad_coord)[0];
	}

	CADCSampleQuadOnDLS quad_coord_trigger(
	    CADCSampleQuadOnSynram(
	        SynapseQuadOnSynram(
	            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min), coord.toSynapseRowOnSynram()),
	        CADCChannelType::causal, CADCReadoutType::trigger_read),
	    coord.toSynramOnDLS());
	CADCSampleQuad quad;
	ref_addresses[0] = quad.read_addresses(quad_coord_trigger)[0];

	for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
		CADCSampleQuadOnDLS quad_coord(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
		        CADCChannelType::acausal, CADCReadoutType::buffered),
		    coord.toSynramOnDLS());
		CADCSampleQuad quad_config;
		ref_addresses[quad_column.toEnum() + SynapseQuadColumnOnDLS::size + 1] =
		    quad_config.read_addresses(quad_coord)[0];
	}

	CADCSampleRow config;
	{
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(ref_data)});
	ASSERT_EQ(config.causal[SynapseOnSynapseRow(43)], CADCSampleRow::Value(0xf0));
	ASSERT_EQ(config.acausal[SynapseOnSynapseRow(51)], CADCSampleRow::Value(0x8));
}
