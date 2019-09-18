#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "lola/vx/cadc.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx;
using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(CADCSampleRow, General)
{
	CADCSampleRow config;

	// test getter/setter
	{
		auto value = config.get_causal();
		value[SynapseOnSynapseRow(23)] =
		    draw_ranged_non_default_value<CADCSampleRow::Value>(value[SynapseOnSynapseRow(23)]);
		config.set_causal(value);
		EXPECT_EQ(config.get_causal(), value);
	}

	{
		auto value = config.get_acausal();
		value[SynapseOnSynapseRow(24)] =
		    draw_ranged_non_default_value<CADCSampleRow::Value>(value[SynapseOnSynapseRow(24)]);
		config.set_acausal(value);
		EXPECT_EQ(config.get_acausal(), value);
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
	{
		auto value = obj1.get_causal();
		value[SynapseOnSynapseRow(23)] =
		    draw_ranged_non_default_value<CADCSampleRow::Value>(value[SynapseOnSynapseRow(23)]);
		obj1.set_causal(value);
	}
	{
		auto value = obj1.get_acausal();
		value[SynapseOnSynapseRow(24)] =
		    draw_ranged_non_default_value<CADCSampleRow::Value>(value[SynapseOnSynapseRow(24)]);
		obj1.set_acausal(value);
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

TEST(CADCSampleRow, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusChipAddress> addresses_type;
	typedef std::vector<fisch::vx::OmnibusChip> words_type;

	CADCSampleRowOnDLS coord(Enum(12));

	std::array<
	    typename addresses_type::value_type, CADCSampleQuad::read_config_size_in_words *
	                                             SynapseQuadColumnOnDLS::size *
	                                             CADCChannelType::size>
	    ref_addresses;
	words_type ref_data(
	    CADCSampleQuad::read_config_size_in_words * SynapseQuadColumnOnDLS::size *
	    CADCChannelType::size);
	for (auto& word : ref_data) {
		word = fisch::vx::OmnibusChip(fisch::vx::OmnibusData(0));
	}
	ref_data[10] = fisch::vx::OmnibusChip(fisch::vx::OmnibusData(0xf));
	ref_data[12 + SynapseQuadColumnOnDLS::size] =
	    fisch::vx::OmnibusChip(fisch::vx::OmnibusData(0x10));

	for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max; ++i) {
		auto quad_column = SynapseQuadColumnOnDLS(i);
		CADCSampleQuadOnDLS quad_coord(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
		        CADCChannelType::causal, CADCReadoutType::buffered),
		    coord.toSynramOnDLS());
		CADCSampleQuad quad_config;
		ref_addresses[i] = quad_config.read_addresses(quad_coord)[0];
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
		ref_addresses[quad_column.toEnum() + SynapseQuadColumnOnDLS::size] =
		    quad_config.read_addresses(quad_coord)[0];
	}

	CADCSampleRow config;
	{
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(ref_data)});
	ASSERT_EQ(config.get_causal()[SynapseOnSynapseRow(43)], CADCSampleRow::Value(0xf0));
	ASSERT_EQ(config.get_acausal()[SynapseOnSynapseRow(51)], CADCSampleRow::Value(0x8));
}
