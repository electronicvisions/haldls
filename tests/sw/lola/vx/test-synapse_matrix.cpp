#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "lola/vx/synapse.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx;
using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(SynapseMatrix, General)
{
	SynapseMatrix config;

	// test getter/setter
	{
		auto value = config.get_weights();
		value[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)] =
		    draw_ranged_non_default_value<SynapseMatrix::Weight>(
		        value[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)]);
		config.set_weights(value);
		EXPECT_EQ(config.get_weights(), value);
	}

	{
		auto value = config.get_addresses();
		value[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)] =
		    draw_ranged_non_default_value<SynapseMatrix::Address>(
		        value[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)]);
		config.set_addresses(value);
		EXPECT_EQ(config.get_addresses(), value);
	}

	{
		auto value = config.get_time_calibs();
		value[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)] =
		    draw_ranged_non_default_value<SynapseMatrix::TimeCalib>(
		        value[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)]);
		config.set_time_calibs(value);
		EXPECT_EQ(config.get_time_calibs(), value);
	}

	{
		auto value = config.get_amp_calibs();
		value[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)] =
		    draw_ranged_non_default_value<SynapseMatrix::AmpCalib>(
		        value[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)]);
		config.set_amp_calibs(value);
		EXPECT_EQ(config.get_amp_calibs(), value);
	}

	SynapseMatrix config_eq = config;
	SynapseMatrix config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(SynapseMatrix, CerealizeCoverage)
{
	SynapseMatrix obj1, obj2;
	{
		auto value = obj1.get_weights();
		value[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)] =
		    draw_ranged_non_default_value<SynapseMatrix::Weight>(
		        value[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)]);
		obj1.set_weights(value);
	}
	{
		auto value = obj1.get_addresses();
		value[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)] =
		    draw_ranged_non_default_value<SynapseMatrix::Address>(
		        value[SynapseRowOnSynram(13)][SynapseOnSynapseRow(24)]);
		obj1.set_addresses(value);
	}
	{
		auto value = obj1.get_time_calibs();
		value[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)] =
		    draw_ranged_non_default_value<SynapseMatrix::TimeCalib>(
		        value[SynapseRowOnSynram(14)][SynapseOnSynapseRow(25)]);
		obj1.set_time_calibs(value);
	}
	{
		auto value = obj1.get_amp_calibs();
		value[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)] =
		    draw_ranged_non_default_value<SynapseMatrix::AmpCalib>(
		        value[SynapseRowOnSynram(15)][SynapseOnSynapseRow(26)]);
		obj1.set_amp_calibs(value);
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

TEST(SynapseMatrix, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusChipAddress> addresses_type;
	typedef std::vector<fisch::vx::OmnibusChip> words_type;

	SynapseMatrix config;
	{
		auto value = config.get_weights();
		value[SynapseRowOnSynram(12)][SynapseOnSynapseRow(23)] = SynapseMatrix::Weight(63);
		config.set_weights(value);
	}

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
