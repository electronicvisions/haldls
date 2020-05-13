#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v1/omnibus_constants.h"
#include "haldls/vx/v1/readout.h"
#include "test-helper.h"

using namespace haldls::vx::v1;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(PadMultiplexerConfig, General)
{
	PadMultiplexerConfig config;

	{
		auto member = config.get_cadc_v_ramp_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_cadc_v_ramp_mux(member);
		EXPECT_EQ(config.get_cadc_v_ramp_mux(), member);
	}

	{
		auto member = config.get_cadc_v_ramp_mux_to_pad();
		member = !member;
		config.set_cadc_v_ramp_mux_to_pad(member);
		EXPECT_EQ(config.get_cadc_v_ramp_mux_to_pad(), member);
	}

	{
		auto member = config.get_capmem_i_out_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_capmem_i_out_mux(member);
		EXPECT_EQ(config.get_capmem_i_out_mux(), member);
	}

	{
		auto member = config.get_capmem_i_out_mux_to_capmem_intermediate_mux();
		member = !member;
		config.set_capmem_i_out_mux_to_capmem_intermediate_mux(member);
		EXPECT_EQ(config.get_capmem_i_out_mux_to_capmem_intermediate_mux(), member);
	}

	{
		auto member = config.get_capmem_intermediate_mux_to_pad();
		member = !member;
		config.set_capmem_intermediate_mux_to_pad(member);
		EXPECT_EQ(config.get_capmem_intermediate_mux_to_pad(), member);
	}

	{
		auto member = config.get_capmem_v_out_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_capmem_v_out_mux(member);
		EXPECT_EQ(config.get_capmem_v_out_mux(), member);
	}

	{
		auto member = config.get_capmem_v_out_mux_to_capmem_intermediate_mux();
		member = !member;
		config.set_capmem_v_out_mux_to_capmem_intermediate_mux(member);
		EXPECT_EQ(config.get_capmem_v_out_mux_to_capmem_intermediate_mux(), member);
	}

	{
		auto member = config.get_capmem_v_ref_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_capmem_v_ref_mux(member);
		EXPECT_EQ(config.get_capmem_v_ref_mux(), member);
	}

	{
		auto member = config.get_capmem_v_ref_mux_to_capmem_intermediate_mux();
		member = !member;
		config.set_capmem_v_ref_mux_to_capmem_intermediate_mux(member);
		EXPECT_EQ(config.get_capmem_v_ref_mux_to_capmem_intermediate_mux(), member);
	}

	{
		auto member = config.get_neuron_i_stim_mux();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_neuron_i_stim_mux(member);
		EXPECT_EQ(config.get_neuron_i_stim_mux(), member);
	}

	{
		auto member = config.get_neuron_i_stim_mux_to_pad();
		member = !member;
		config.set_neuron_i_stim_mux_to_pad(member);
		EXPECT_EQ(config.get_neuron_i_stim_mux_to_pad(), member);
	}

	{
		auto member = config.get_cadc_debug_acausal_to_synapse_intermediate_mux();
		member = !member;
		config.set_cadc_debug_acausal_to_synapse_intermediate_mux(member);
		EXPECT_EQ(config.get_cadc_debug_acausal_to_synapse_intermediate_mux(), member);
	}

	{
		auto member = config.get_cadc_debug_causal_to_synapse_intermediate_mux();
		member = !member;
		config.set_cadc_debug_causal_to_synapse_intermediate_mux(member);
		EXPECT_EQ(config.get_cadc_debug_causal_to_synapse_intermediate_mux(), member);
	}

	{
		auto member = config.get_synin_debug_inhibitory_to_synapse_intermediate_mux();
		member = !member;
		config.set_synin_debug_inhibitory_to_synapse_intermediate_mux(member);
		EXPECT_EQ(config.get_synin_debug_inhibitory_to_synapse_intermediate_mux(), member);
	}

	{
		auto member = config.get_synin_debug_excitatory_to_synapse_intermediate_mux();
		member = !member;
		config.set_synin_debug_excitatory_to_synapse_intermediate_mux(member);
		EXPECT_EQ(config.get_synin_debug_excitatory_to_synapse_intermediate_mux(), member);
	}

	{
		auto member = config.get_synapse_intermediate_mux_to_pad();
		member = !member;
		config.set_synapse_intermediate_mux_to_pad(member);
		EXPECT_EQ(config.get_synapse_intermediate_mux_to_pad(), member);
	}

	{
		auto member = config.get_buffer_to_pad();
		for (auto coord : iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
			member[coord] = !member[coord];
		}
		config.set_buffer_to_pad(member);
		EXPECT_EQ(config.get_buffer_to_pad(), member);
	}

	PadMultiplexerConfig default_config;

	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);

	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(PadMultiplexerConfig, EncodeDecode)
{
	PadMultiplexerConfig config;

	auto coord = typename PadMultiplexerConfig::coordinate_type();

	std::array<OmnibusChipOverJTAGAddress, PadMultiplexerConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0xc0000 + 14},
	                     OmnibusChipOverJTAGAddress{0xc0000 + 15}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode default
	words_type default_data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{default_data});
	EXPECT_EQ(default_data[0].get(), 0x0);
	EXPECT_EQ(default_data[1].get(), 0x0);

	// Encode some connections
	{
		auto member = config.get_cadc_v_ramp_mux();
		member[CapMemBlockOnDLS(3)] = true;
		config.set_cadc_v_ramp_mux(member);
	}
	config.set_cadc_v_ramp_mux_to_pad(true);

	{
		auto member = config.get_capmem_v_ref_mux();
		member[CapMemBlockOnDLS(1)] = true;
		config.set_capmem_v_ref_mux(member);
	}
	config.set_capmem_v_ref_mux_to_capmem_intermediate_mux(true);
	config.set_capmem_intermediate_mux_to_pad(true);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_EQ(data[0].get(), 0x411);
	EXPECT_EQ(data[1].get(), 0x14);

	// Decode
	PadMultiplexerConfig decoded;
	ASSERT_NE(config, decoded);
	visit_preorder(decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, decoded);
}

TEST(PadMultiplexerConfig, CerealizeCoverage)
{
	PadMultiplexerConfig config, c2;

	{
		auto member = config.get_cadc_v_ramp_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_cadc_v_ramp_mux(member);
	}

	{
		auto member = config.get_cadc_v_ramp_mux_to_pad();
		member = !member;
		config.set_cadc_v_ramp_mux_to_pad(member);
	}

	{
		auto member = config.get_capmem_i_out_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_capmem_i_out_mux(member);
	}

	{
		auto member = config.get_capmem_i_out_mux_to_capmem_intermediate_mux();
		member = !member;
		config.set_capmem_i_out_mux_to_capmem_intermediate_mux(member);
	}

	{
		auto member = config.get_capmem_intermediate_mux_to_pad();
		member = !member;
		config.set_capmem_intermediate_mux_to_pad(member);
	}

	{
		auto member = config.get_capmem_v_out_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_capmem_v_out_mux(member);
	}

	{
		auto member = config.get_capmem_v_out_mux_to_capmem_intermediate_mux();
		member = !member;
		config.set_capmem_v_out_mux_to_capmem_intermediate_mux(member);
	}

	{
		auto member = config.get_capmem_v_ref_mux();
		for (auto coord : iter_all<CapMemBlockOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_capmem_v_ref_mux(member);
	}

	{
		auto member = config.get_capmem_v_ref_mux_to_capmem_intermediate_mux();
		member = !member;
		config.set_capmem_v_ref_mux_to_capmem_intermediate_mux(member);
	}

	{
		auto member = config.get_neuron_i_stim_mux();
		for (auto coord : iter_all<HemisphereOnDLS>()) {
			member[coord] = !member[coord];
		}
		config.set_neuron_i_stim_mux(member);
	}

	{
		auto member = config.get_neuron_i_stim_mux_to_pad();
		member = !member;
		config.set_neuron_i_stim_mux_to_pad(member);
	}

	{
		auto member = config.get_cadc_debug_acausal_to_synapse_intermediate_mux();
		member = !member;
		config.set_cadc_debug_acausal_to_synapse_intermediate_mux(member);
	}

	{
		auto member = config.get_cadc_debug_causal_to_synapse_intermediate_mux();
		member = !member;
		config.set_cadc_debug_causal_to_synapse_intermediate_mux(member);
	}

	{
		auto member = config.get_synin_debug_inhibitory_to_synapse_intermediate_mux();
		member = !member;
		config.set_synin_debug_inhibitory_to_synapse_intermediate_mux(member);
	}

	{
		auto member = config.get_synin_debug_excitatory_to_synapse_intermediate_mux();
		member = !member;
		config.set_synin_debug_excitatory_to_synapse_intermediate_mux(member);
	}

	{
		auto member = config.get_synapse_intermediate_mux_to_pad();
		member = !member;
		config.set_synapse_intermediate_mux_to_pad(member);
	}

	{
		auto member = config.get_buffer_to_pad();
		for (auto coord : iter_all<SourceMultiplexerOnReadoutSourceSelection>()) {
			member[coord] = !member[coord];
		}
		config.set_buffer_to_pad(member);
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(config);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(config, c2);
}
