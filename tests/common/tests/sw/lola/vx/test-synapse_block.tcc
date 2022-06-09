#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "lola/vx/synapse_block.h"
#include "stadls/visitors.h"
#include "test-helper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#if CHIP_REVISION == 2
#define CHIP_REVISION_STR v2
#elif CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

using namespace haldls::vx;
using namespace lola::vx::CHIP_REVISION_STR;
using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;
using namespace halco::common;

TEST(SynapseBlock, General)
{
	{
		SynapseBlock config;
		config.matrix.weights[SynapseRowOnDLS()][SynapseOnSynapseRow()] = SynapseMatrix::Weight(12);
		EXPECT_NE(config, SynapseBlock());
	}
	{
		SynapseBlock config;
		config.i_bias_dac[CapMemBlockOnHemisphere(0)] = SynapseBlock::AnalogValue(123);
		EXPECT_NE(config, SynapseBlock());
	}
	{
		SynapseBlock config;
		config.i_bias_dac[CapMemBlockOnHemisphere(1)] = SynapseBlock::AnalogValue(321);
		EXPECT_NE(config, SynapseBlock());
	}
}

TEST(SynapseBlock, CerealizeCoverage)
{
	SynapseBlock obj1, obj2;

	obj1.matrix.weights[SynapseRowOnDLS()][SynapseOnSynapseRow()] = SynapseMatrix::Weight(12);
	obj1.i_bias_dac[CapMemBlockOnHemisphere(0)] = SynapseBlock::AnalogValue(123);
	obj1.i_bias_dac[CapMemBlockOnHemisphere(1)] = SynapseBlock::AnalogValue(321);

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

TEST(SynapseBlock, EncodeDecode)
{
	SynapseBlock config;
	config.matrix.weights[SynapseRowOnDLS()][SynapseOnSynapseRow()] = SynapseMatrix::Weight(12);
	config.i_bias_dac[CapMemBlockOnHemisphere(0)] = SynapseBlock::AnalogValue(123);
	config.i_bias_dac[CapMemBlockOnHemisphere(1)] = SynapseBlock::AnalogValue(321);

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	SynapseBlockOnDLS coord;

	addresses_type read_addresses;
	addresses_type write_addresses;
	visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(read_addresses, write_addresses);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});

	SynapseBlock config_decoded;
	visit_preorder(config_decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	EXPECT_EQ(config_decoded, config);
}
