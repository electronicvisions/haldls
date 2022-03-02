#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v1/neuron.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v1/common.h"
#include "lola/vx/cerealization.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx::v1;
using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx::v1;
using namespace halco::common;

TEST(NeuronBlock, General)
{
	{
		NeuronBlock config;
		config.atomic_neurons[AtomicNeuronOnDLS()].event_routing.enable_digital =
		    !config.atomic_neurons[AtomicNeuronOnDLS()].event_routing.enable_digital;
		EXPECT_NE(config, NeuronBlock());
	}
	{
		NeuronBlock config;
		config.backends[CommonNeuronBackendConfigOnDLS()].set_enable_event_registers(
		    !config.backends[CommonNeuronBackendConfigOnDLS()].get_enable_event_registers());
		EXPECT_NE(config, NeuronBlock());
	}
	{
		NeuronBlock config;
		config.current_rows[ColumnCurrentRowOnDLS()]
		    .values[SynapseOnSynapseRow()]
		    .set_enable_debug_excitatory(!config.current_rows[ColumnCurrentRowOnDLS()]
		                                      .values[SynapseOnSynapseRow()]
		                                      .get_enable_debug_excitatory());
		EXPECT_NE(config, NeuronBlock());
	}
	{
		NeuronBlock config;
		config.i_bias_synin_sd_exc[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(123);
		EXPECT_NE(config, NeuronBlock());
	}
	{
		NeuronBlock config;
		config.i_bias_synin_sd_inh[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(234);
		EXPECT_NE(config, NeuronBlock());
	}
	{
		NeuronBlock config;
		config.i_bias_threshold_comparator[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(345);
		EXPECT_NE(config, NeuronBlock());
	}
}

TEST(NeuronBlock, CerealizeCoverage)
{
	NeuronBlock obj1, obj2;

	obj1.atomic_neurons[AtomicNeuronOnDLS()].event_routing.enable_digital =
	    !obj1.atomic_neurons[AtomicNeuronOnDLS()].event_routing.enable_digital;
	obj1.backends[CommonNeuronBackendConfigOnDLS()].set_enable_event_registers(
	    !obj1.backends[CommonNeuronBackendConfigOnDLS()].get_enable_event_registers());
	obj1.current_rows[ColumnCurrentRowOnDLS()]
	    .values[SynapseOnSynapseRow()]
	    .set_enable_debug_excitatory(!obj1.current_rows[ColumnCurrentRowOnDLS()]
	                                      .values[SynapseOnSynapseRow()]
	                                      .get_enable_debug_excitatory());
	obj1.i_bias_synin_sd_exc[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(123);
	obj1.i_bias_synin_sd_inh[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(234);
	obj1.i_bias_threshold_comparator[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(345);

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

TEST(NeuronBlock, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	NeuronBlockOnDLS coord;

	NeuronBlock config;
	config.atomic_neurons[AtomicNeuronOnDLS()].event_routing.enable_digital =
	    !config.atomic_neurons[AtomicNeuronOnDLS()].event_routing.enable_digital;
	config.backends[CommonNeuronBackendConfigOnDLS()].set_enable_event_registers(
	    !config.backends[CommonNeuronBackendConfigOnDLS()].get_enable_event_registers());
	config.current_rows[ColumnCurrentRowOnDLS()]
	    .values[SynapseOnSynapseRow()]
	    .set_enable_debug_excitatory(!config.current_rows[ColumnCurrentRowOnDLS()]
	                                      .values[SynapseOnSynapseRow()]
	                                      .get_enable_debug_excitatory());
	config.i_bias_synin_sd_exc[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(123);
	config.i_bias_synin_sd_inh[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(234);
	config.i_bias_threshold_comparator[CapMemBlockOnDLS()] = NeuronBlock::AnalogValue(345);

	addresses_type read_addresses;
	addresses_type write_addresses;
	visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(read_addresses, write_addresses);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});

	NeuronBlock config_decoded;
	visit_preorder(config_decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	EXPECT_EQ(config_decoded, config);
}
