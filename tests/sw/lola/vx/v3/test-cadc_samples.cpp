#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v3/cadc.h"

#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v3/cadc.h"
#include "haldls/vx/v3/common.h"
#include "lola/vx/cerealization.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx::v3;
using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx::v3;
using namespace halco::common;

TEST(CADCSamples, General)
{
	CADCSamples config;

	// test getter/setter
	{
		for (auto const s : iter_all<SynramOnDLS>()) {
			auto value = config.causal[s];
			value[SynapseOnSynapseRow(23)] =
			    draw_ranged_non_default_value<CADCSamples::Value>(value[SynapseOnSynapseRow(23)]);
			config.causal[s] = value;
			EXPECT_EQ(config.causal[s], value);
		}
	}

	{
		for (auto const s : iter_all<SynramOnDLS>()) {
			auto value = config.acausal[s];
			value[SynapseOnSynapseRow(24)] =
			    draw_ranged_non_default_value<CADCSamples::Value>(value[SynapseOnSynapseRow(24)]);
			config.acausal[s] = value;
			EXPECT_EQ(config.acausal[s], value);
		}
	}

	CADCSamples config_eq = config;
	CADCSamples config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(CADCSamples, CerealizeCoverage)
{
	CADCSamples obj1, obj2;
	obj1.causal[SynramOnDLS::top][SynapseOnSynapseRow(23)] =
	    draw_ranged_non_default_value<CADCSamples::Value>(
	        obj1.causal[SynramOnDLS::top][SynapseOnSynapseRow(23)]);
	obj1.acausal[SynramOnDLS::bottom][SynapseOnSynapseRow(24)] =
	    draw_ranged_non_default_value<CADCSamples::Value>(
	        obj1.acausal[SynramOnDLS::bottom][SynapseOnSynapseRow(24)]);

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

TEST(CADCSamples, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::Omnibus> words_type;

	CADCSamplesOnDLS coord;

	std::array<
	    typename addresses_type::value_type, CADCSampleQuad::read_config_size_in_words *
	                                             SynapseQuadColumnOnDLS::size *
	                                             CADCChannelType::size * SynramOnDLS::size>
	    ref_addresses;
	words_type ref_data(
	    CADCSampleQuad::read_config_size_in_words * SynapseQuadColumnOnDLS::size *
	    CADCChannelType::size * SynramOnDLS::size);
	for (auto& word : ref_data) {
		word = fisch::vx::Omnibus(fisch::vx::OmnibusData(0));
	}
	ref_data[11] = fisch::vx::Omnibus(fisch::vx::OmnibusData(0xf));
	ref_data[13 + SynapseQuadColumnOnDLS::size] = fisch::vx::Omnibus(fisch::vx::OmnibusData(0x10));

	{
		CADCSampleQuadOnDLS quad_coord_trigger(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(
		            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min), SynapseRowOnSynram()),
		        CADCChannelType::causal, CADCReadoutType::trigger_read),
		    SynramOnDLS::top);
		CADCSampleQuad quad;
		ref_addresses[0] = quad.read_addresses(quad_coord_trigger)[0];
	}

	{
		CADCSampleQuadOnDLS quad_coord_trigger(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(
		            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min), SynapseRowOnSynram()),
		        CADCChannelType::causal, CADCReadoutType::trigger_read),
		    SynramOnDLS::bottom);
		CADCSampleQuad quad;
		ref_addresses[1] = quad.read_addresses(quad_coord_trigger)[0];
	}

	for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max; ++i) {
		auto quad_column = SynapseQuadColumnOnDLS(i);
		CADCSampleQuadOnDLS quad_coord(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(quad_column, SynapseRowOnSynram()), CADCChannelType::causal,
		        CADCReadoutType::buffered),
		    SynramOnDLS::top);
		CADCSampleQuad quad_config;
		ref_addresses[i + 1] = quad_config.read_addresses(quad_coord)[0];
	}

	for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
		CADCSampleQuadOnDLS quad_coord(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(quad_column, SynapseRowOnSynram()), CADCChannelType::acausal,
		        CADCReadoutType::buffered),
		    SynramOnDLS::top);
		CADCSampleQuad quad_config;
		ref_addresses[quad_column.toEnum() + SynapseQuadColumnOnDLS::size + 1] =
		    quad_config.read_addresses(quad_coord)[0];
	}

	for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max; ++i) {
		auto quad_column = SynapseQuadColumnOnDLS(i);
		CADCSampleQuadOnDLS quad_coord(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(quad_column, SynapseRowOnSynram()), CADCChannelType::causal,
		        CADCReadoutType::buffered),
		    SynramOnDLS::bottom);
		CADCSampleQuad quad_config;
		ref_addresses[i + 2 * SynapseQuadColumnOnDLS::size] =
		    quad_config.read_addresses(quad_coord)[0];
	}

	for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
		CADCSampleQuadOnDLS quad_coord(
		    CADCSampleQuadOnSynram(
		        SynapseQuadOnSynram(quad_column, SynapseRowOnSynram()), CADCChannelType::acausal,
		        CADCReadoutType::buffered),
		    SynramOnDLS::bottom);
		CADCSampleQuad quad_config;
		ref_addresses[quad_column.toEnum() + 3 * SynapseQuadColumnOnDLS::size] =
		    quad_config.read_addresses(quad_coord)[0];
	}

	CADCSamples config;
	{
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	visit_preorder(config, coord, stadls::DecodeVisitor<words_type>{std::move(ref_data)});

	ASSERT_EQ(config.causal[SynramOnDLS::top][SynapseOnSynapseRow(86)], CADCSamples::Value(0xf));
	ASSERT_EQ(config.acausal[SynramOnDLS::top][SynapseOnSynapseRow(102)], CADCSamples::Value(0x10));
}
