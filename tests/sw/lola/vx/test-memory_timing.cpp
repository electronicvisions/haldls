#include "cereal/types/lola/vx/memory_timing.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "lola/vx/memory_timing.h"
#include "stadls/visitors.h"
#include "test-helper.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace haldls::vx;
using namespace lola::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(MemoryTiming, General)
{
	{
		MemoryTiming config;
		config.cadc_offset[CADCOffsetSRAMTimingConfigOnDLS()].set_read_delay(
		    CADCOffsetSRAMTimingConfig::ReadDelay(0));
		EXPECT_NE(config, MemoryTiming());
	}
	{
		MemoryTiming config;
		config.synapse_driver[SynapseDriverSRAMTimingConfigOnDLS()].set_read_delay(
		    SynapseDriverSRAMTimingConfig::ReadDelay(1));
		EXPECT_NE(config, MemoryTiming());
	}
	{
		MemoryTiming config;
		config.neuron[NeuronSRAMTimingConfigOnDLS()].set_read_delay(
		    NeuronSRAMTimingConfig::ReadDelay(2));
		EXPECT_NE(config, MemoryTiming());
	}
	{
		MemoryTiming config;
		config.neuron_backend[NeuronBackendSRAMTimingConfigOnDLS()].set_read_delay(
		    NeuronBackendSRAMTimingConfig::ReadDelay(3));
		EXPECT_NE(config, MemoryTiming());
	}
	{
		MemoryTiming config;
		config.synram[CommonSynramConfigOnDLS()].set_pc_conf_west(CommonSynramConfig::PCConf(4));
		EXPECT_NE(config, MemoryTiming());
	}
}

TEST(MemoryTiming, CerealizeCoverage)
{
	MemoryTiming obj1, obj2;

	obj1.cadc_offset[CADCOffsetSRAMTimingConfigOnDLS()].set_read_delay(
	    CADCOffsetSRAMTimingConfig::ReadDelay(0));
	obj1.synapse_driver[SynapseDriverSRAMTimingConfigOnDLS()].set_read_delay(
	    SynapseDriverSRAMTimingConfig::ReadDelay(1));
	obj1.neuron[NeuronSRAMTimingConfigOnDLS()].set_read_delay(NeuronSRAMTimingConfig::ReadDelay(2));
	obj1.neuron_backend[NeuronBackendSRAMTimingConfigOnDLS()].set_read_delay(
	    NeuronBackendSRAMTimingConfig::ReadDelay(3));
	obj1.synram[CommonSynramConfigOnDLS()].set_pc_conf_west(CommonSynramConfig::PCConf(4));

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

TEST(MemoryTiming, EncodeDecode)
{
	MemoryTiming config;
	config.cadc_offset[CADCOffsetSRAMTimingConfigOnDLS()].set_read_delay(
	    CADCOffsetSRAMTimingConfig::ReadDelay(0));
	config.synapse_driver[SynapseDriverSRAMTimingConfigOnDLS()].set_read_delay(
	    SynapseDriverSRAMTimingConfig::ReadDelay(1));
	config.neuron[NeuronSRAMTimingConfigOnDLS()].set_read_delay(
	    NeuronSRAMTimingConfig::ReadDelay(2));
	config.neuron_backend[NeuronBackendSRAMTimingConfigOnDLS()].set_read_delay(
	    NeuronBackendSRAMTimingConfig::ReadDelay(3));
	config.synram[CommonSynramConfigOnDLS()].set_pc_conf_west(CommonSynramConfig::PCConf(4));

	typedef std::vector<OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	addresses_type ref_addresses;
	words_type ref_data;
	for (auto const coord : iter_all<MemoryTiming::CADCOffset::key_type>()) {
		visit_preorder(
		    config.cadc_offset[coord], coord,
		    stadls::WriteAddressVisitor<addresses_type>{ref_addresses});
		visit_preorder(
		    config.cadc_offset[coord], coord, stadls::EncodeVisitor<words_type>{ref_data});
	}
	for (auto const coord : iter_all<MemoryTiming::SynapseDriver::key_type>()) {
		visit_preorder(
		    config.synapse_driver[coord], coord,
		    stadls::WriteAddressVisitor<addresses_type>{ref_addresses});
		visit_preorder(
		    config.synapse_driver[coord], coord, stadls::EncodeVisitor<words_type>{ref_data});
	}
	for (auto const coord : iter_all<MemoryTiming::Neuron::key_type>()) {
		visit_preorder(
		    config.neuron[coord], coord,
		    stadls::WriteAddressVisitor<addresses_type>{ref_addresses});
		visit_preorder(config.neuron[coord], coord, stadls::EncodeVisitor<words_type>{ref_data});
	}
	for (auto const coord : iter_all<MemoryTiming::NeuronBackend::key_type>()) {
		visit_preorder(
		    config.neuron_backend[coord], coord,
		    stadls::WriteAddressVisitor<addresses_type>{ref_addresses});
		visit_preorder(
		    config.neuron_backend[coord], coord, stadls::EncodeVisitor<words_type>{ref_data});
	}
	for (auto const coord : iter_all<MemoryTiming::Synram::key_type>()) {
		visit_preorder(
		    config.synram[coord], coord,
		    stadls::WriteAddressVisitor<addresses_type>{ref_addresses});
		visit_preorder(config.synram[coord], coord, stadls::EncodeVisitor<words_type>{ref_data});
	}

	MemoryTimingOnDLS coord;
	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ref_addresses);
	}
	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ref_addresses);
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	MemoryTiming config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}
