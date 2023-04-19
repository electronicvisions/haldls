#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "lola/vx/capmem.h"
#include "stadls/visitors.h"
#include "test-helper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#include "cereal/types/lola/vx/v3/capmem.h"
#else
#error "Unknown CHIP_REVISION"
#endif

using namespace haldls::vx;
using namespace lola::vx::CHIP_REVISION_STR;
using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;
using namespace halco::common;

TEST(CapMem, General)
{
	{
		CapMem config;
		config.blocks[CapMemBlockConfigOnDLS()].set_enable_capmem(
		    !config.blocks[CapMemBlockConfigOnDLS()].get_enable_capmem());
		EXPECT_NE(config, CapMem());
	}
	{
		CapMem config;
		auto bias = config.synapse_bias_selection.get_enable_internal_dac_bias();
		bias[CapMemBlockOnDLS()] = !bias[CapMemBlockOnDLS()];
		config.synapse_bias_selection.set_enable_internal_dac_bias(bias);
		EXPECT_NE(config, CapMem());
	}
	{
		CapMem config;
		config.reference_generator.set_enable_internal_reference(
		    !config.reference_generator.get_enable_internal_reference());
		EXPECT_NE(config, CapMem());
	}
	{
		CapMem config;
		config.unused_cells[UnusedCapMemCellOnDLS()] = CapMem::AnalogValue(123);
		EXPECT_NE(config, CapMem());
	}
}

TEST(CapMem, CerealizeCoverage)
{
	CapMem obj1, obj2;

	obj1.blocks[CapMemBlockConfigOnDLS()].set_enable_capmem(
	    !obj1.blocks[CapMemBlockConfigOnDLS()].get_enable_capmem());
	auto bias = obj1.synapse_bias_selection.get_enable_internal_dac_bias();
	bias[CapMemBlockOnDLS()] = !bias[CapMemBlockOnDLS()];
	obj1.synapse_bias_selection.set_enable_internal_dac_bias(bias);
	obj1.reference_generator.set_enable_internal_reference(
	    !obj1.reference_generator.get_enable_internal_reference());
	obj1.unused_cells[UnusedCapMemCellOnDLS()] = CapMem::AnalogValue(123);

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

TEST(CapMem, EncodeDecode)
{
	CapMem config;
	config.blocks[CapMemBlockConfigOnDLS()].set_enable_capmem(
	    !config.blocks[CapMemBlockConfigOnDLS()].get_enable_capmem());
	auto bias = config.synapse_bias_selection.get_enable_internal_dac_bias();
	bias[CapMemBlockOnDLS()] = !bias[CapMemBlockOnDLS()];
	config.synapse_bias_selection.set_enable_internal_dac_bias(bias);
	config.reference_generator.set_enable_internal_reference(
	    !config.reference_generator.get_enable_internal_reference());
	config.unused_cells[UnusedCapMemCellOnDLS()] = CapMem::AnalogValue(123);

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	CapMemOnDLS coord;

	addresses_type read_addresses;
	addresses_type write_addresses;
	visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(read_addresses, write_addresses);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});

	CapMem config_decoded;
	visit_preorder(config_decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	EXPECT_EQ(config_decoded, config);
}
