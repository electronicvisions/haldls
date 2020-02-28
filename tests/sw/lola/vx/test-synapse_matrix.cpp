#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/synapse.h"

#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_heap_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "lola/vx/cerealization.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx;
using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(SynapseMatrix, General)
{
	auto config_ptr = std::make_unique<SynapseMatrix>();
	SynapseMatrix& config = *config_ptr;

	// test properties
	{
		auto value = config.weights;
		value[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)] =
		    draw_ranged_non_default_value<SynapseMatrix::Weight>(
		        value[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)]);
		config.weights = value;
		EXPECT_EQ(config.weights, value);
	}

	{
		auto value = config.addresses;
		value[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)] =
		    draw_ranged_non_default_value<SynapseMatrix::Address>(
		        value[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)]);
		config.addresses = value;
		EXPECT_EQ(config.addresses, value);
	}

	{
		auto value = config.time_calibs;
		value[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)] =
		    draw_ranged_non_default_value<SynapseMatrix::TimeCalib>(
		        value[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)]);
		config.time_calibs = value;
		EXPECT_EQ(config.time_calibs, value);
	}

	{
		auto value = config.amp_calibs;
		value[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)] =
		    draw_ranged_non_default_value<SynapseMatrix::AmpCalib>(
		        value[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)]);
		config.amp_calibs = value;
		EXPECT_EQ(config.amp_calibs, value);
	}

	auto const config_eq_ptr = std::make_unique<SynapseMatrix>(config);
	auto const config_default_ptr = std::make_unique<SynapseMatrix>();
	auto const& config_eq = *config_eq_ptr;
	auto const& config_default = *config_default_ptr;

	// test comparison
	//	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	//	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(SynapseMatrix, CerealizeCoverage)
{
	auto obj1_ptr = std::make_unique<SynapseMatrix>();
	auto obj2_ptr = std::make_unique<SynapseMatrix>();
	auto& obj1 = *obj1_ptr;
	auto& obj2 = *obj2_ptr;
	obj1.weights[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)] =
	    draw_ranged_non_default_value<SynapseMatrix::Weight>(
	        obj1.weights[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)]);
	obj1.addresses[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)] =
	    draw_ranged_non_default_value<SynapseMatrix::Address>(
	        obj1.addresses[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)]);
	obj1.time_calibs[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)] =
	    draw_ranged_non_default_value<SynapseMatrix::TimeCalib>(
	        obj1.time_calibs[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)]);
	obj1.amp_calibs[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)] =
	    draw_ranged_non_default_value<SynapseMatrix::AmpCalib>(
	        obj1.amp_calibs[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)]);

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

TEST(SynapseMatrix, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusChipAddress> addresses_type;
	typedef std::vector<fisch::vx::OmnibusChip> words_type;

	SynapseMatrix config;
	config.weights[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)] = SynapseMatrix::Weight(63);

	SynramOnDLS coord;

	std::array<
	    typename addresses_type::value_type,
	    SynapseQuad::config_size_in_words * SynapseQuadColumnOnDLS::size * SynapseRowOnSynram::size>
	    ref_addresses;
	std::array<
	    fisch::vx::OmnibusChip,
	    SynapseQuad::config_size_in_words * SynapseQuadColumnOnDLS::size * SynapseRowOnSynram::size>
	    ref_data;
	for (auto c : iter_all<SynapseQuadOnSynram>()) {
		auto syn = SynapseQuadOnDLS(c, coord);

		SynapseQuad quad;
		ref_addresses[c.toEnum() * 2] = quad.addresses<typename addresses_type::value_type>(syn)[0];
		ref_addresses[c.toEnum() * 2 + 1] =
		    quad.addresses<typename addresses_type::value_type>(syn)[1];
		ref_data[c.toEnum() * 2] = fisch::vx::OmnibusChip(fisch::vx::OmnibusData(0));
		ref_data[c.toEnum() * 2 + 1] = fisch::vx::OmnibusChip(fisch::vx::OmnibusData(0));
	}
	ref_data[10 + 12 * SynapseQuadColumnOnDLS::size * SynapseQuad::config_size_in_words] =
	    fisch::vx::OmnibusChip(fisch::vx::OmnibusData(0x3f00'0000ul));

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

	SynapseMatrix config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
