#include <gtest/gtest.h>

#include "test-helper.h"

#include "cereal/types/haldls/vx/routing_table_entry.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/routing_table_entry.h"


using namespace haldls::vx;

HALDLS_TEST(OutputRoutingTableEntry, (enable) (label));
HALDLS_TEST(InputRoutingTableEntry, (enable) (label));

TEST(OutputRoutingTableEntry, EncodeDecode)
{
	OutputRoutingTableEntry config;
	config.set_enable(true);
	config.set_label(OutputRoutingTableEntry::Label(7));

	OutputRoutingTableEntry::coordinate_type coord;

	std::array<halco::hicann_dls::vx::OmnibusAddress, OutputRoutingTableEntry::config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusAddress(output_routing_table_base_address)};

	std::array<fisch::vx::word_access_type::Omnibus, OutputRoutingTableEntry::config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::Omnibus((1ul << 0) | (7ul << 1))};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}

TEST(InputRoutingTableEntry, EncodeDecode)
{
	InputRoutingTableEntry config;
	config.set_enable(true);
	config.set_label(InputRoutingTableEntry::Label(7));

	InputRoutingTableEntry::coordinate_type coord;

	std::array<halco::hicann_dls::vx::OmnibusAddress, InputRoutingTableEntry::config_size_in_words>
	    ref_addresses = {halco::hicann_dls::vx::OmnibusAddress(input_routing_table_base_address)};

	std::array<fisch::vx::word_access_type::Omnibus, InputRoutingTableEntry::config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::Omnibus((1ul << 0) | (7ul << 1))};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data);
}