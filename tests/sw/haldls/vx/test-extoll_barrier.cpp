#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/extoll.h"
#include "halco/hicann-dls/vx/extoll.h"
#include "haldls/vx/common.h"
#include "haldls/vx/extoll.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef fisch::vx::word_access_type::ExtollOnNwNode network_word_type;
typedef fisch::vx::word_access_type::Extoll word_type;

TEST(ExtollBarrierTriggerReached, Encode)
{
	ExtollBarrierTriggerReached config;

	std::array<network_word_type, ExtollBarrierTriggerReached::write_config_size_in_words>
	    ref_data = {network_word_type(0x1)};

	{
		ExtollNodeIdOnExtollNetwork node_fpga(
		    ExtollNodeId(0x4), ExtollChipType(ExtollChipType::fpga));

		typename ExtollBarrierTriggerReached::coordinate_type coord_fpga(
		    ExtollBarrierTriggerReachedOnExtollNodeId(7), node_fpga);

		std::array<
		    ExtollAddressOnExtollNetwork, ExtollBarrierTriggerReached::write_config_size_in_words>
		    ref_addresses_fpga = {ExtollAddressOnExtollNetwork(ExtollAddress(0x6'7000), node_fpga)};

		HALDLS_TEST_ENCODE(config, coord_fpga, ref_addresses_fpga, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_self(
		    ExtollNodeId(ExtollNodeId::self), ExtollChipType(ExtollChipType::fpga));

		typename ExtollBarrierTriggerReached::coordinate_type coord_self(
		    ExtollBarrierTriggerReachedOnExtollNodeId(7), node_self);

		std::array<
		    ExtollAddressOnExtollNetwork, ExtollBarrierTriggerReached::write_config_size_in_words>
		    ref_addresses_self = {ExtollAddressOnExtollNetwork(ExtollAddress(0x6'7000), node_self)};

		HALDLS_TEST_ENCODE(config, coord_self, ref_addresses_self, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_tourmalet(
		    ExtollNodeId(0x3), ExtollChipType(ExtollChipType::tourmalet));

		typename ExtollBarrierTriggerReached::coordinate_type coord_tourmalet(
		    ExtollBarrierTriggerReachedOnExtollNodeId(7), node_tourmalet);

		std::array<
		    ExtollAddressOnExtollNetwork, ExtollBarrierTriggerReached::write_config_size_in_words>
		    ref_addresses_tourmalet = {
		        ExtollAddressOnExtollNetwork(ExtollAddress(0x44'7000), node_tourmalet)};

		HALDLS_TEST_ENCODE(config, coord_tourmalet, ref_addresses_tourmalet, ref_data)
	}
}


HALDLS_TEST(ExtollBarrierReleased, (released_0)(released_1)(released_active))

TEST(ExtollBarrierReleased, Decode)
{
	ExtollBarrierReleased config;
	config.set_released_0(true);
	config.set_released_1(false);
	config.set_released_active(true);

	std::array<network_word_type, ExtollBarrierReleased::read_config_size_in_words> ref_data = {
	    network_word_type(0xa)};

	{
		ExtollNodeIdOnExtollNetwork node_fpga(
		    ExtollNodeId(0x4), ExtollChipType(ExtollChipType::fpga));

		typename ExtollBarrierReleased::coordinate_type coord_fpga(
		    ExtollBarrierReleasedOnExtollNodeId(0xa), node_fpga);

		std::array<ExtollAddressOnExtollNetwork, ExtollBarrierReleased::read_config_size_in_words>
		    ref_addresses_fpga = {ExtollAddressOnExtollNetwork(ExtollAddress(0x6'a000), node_fpga)};


		HALDLS_TEST_DECODE(config, coord_fpga, ref_addresses_fpga, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_self(
		    ExtollNodeId(ExtollNodeId::self), ExtollChipType(ExtollChipType::fpga));

		typename ExtollBarrierReleased::coordinate_type coord_self(
		    ExtollBarrierReleasedOnExtollNodeId(0xa), node_self);

		std::array<ExtollAddressOnExtollNetwork, ExtollBarrierReleased::read_config_size_in_words>
		    ref_addresses_self = {ExtollAddressOnExtollNetwork(ExtollAddress(0x6'a000), node_self)};

		HALDLS_TEST_DECODE(config, coord_self, ref_addresses_self, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_tourmalet(
		    ExtollNodeId(0x3), ExtollChipType(ExtollChipType::tourmalet));

		typename ExtollBarrierReleased::coordinate_type coord_tourmalet(
		    ExtollBarrierReleasedOnExtollNodeId(0xa), node_tourmalet);

		std::array<ExtollAddressOnExtollNetwork, ExtollBarrierReleased::read_config_size_in_words>
		    ref_addresses_tourmalet = {
		        ExtollAddressOnExtollNetwork(ExtollAddress(0x44'a000), node_tourmalet)};

		HALDLS_TEST_DECODE(config, coord_tourmalet, ref_addresses_tourmalet, ref_data)
	}
}


HALDLS_TEST(
    ExtollBarrierConfig,
    (enable)(child_nodes)(parent_nodes)(host_count)(client_count)(enable_reset))

TEST(ExtollBarrierConfig, Decode)
{
	ExtollBarrierConfig::LinkFlags child_links;
	child_links.fill(false);
	ExtollBarrierConfig::LinkFlags parent_links;
	parent_links.fill(false);
	child_links[LinkOnExtollNode(1)] = true;
	child_links[LinkOnExtollNode(3)] = true;
	parent_links[LinkOnExtollNode(0)] = true;
	parent_links[LinkOnExtollNode(2)] = true;

	ExtollBarrierConfig config;
	config.set_enable(true);
	config.set_child_nodes(child_links);   // 0xa
	config.set_parent_nodes(parent_links); // 0x5
	config.set_host_count(ExtollBarrierConfig::HostCount(0x3));
	config.set_client_count(ExtollBarrierConfig::ClientCount(0x0)); // *READ_ONLY*
	config.set_enable_reset(true);

	std::array<network_word_type, ExtollBarrierConfig::config_size_in_words> ref_data = {
	    network_word_type(0x09'8515)};

	{
		ExtollNodeIdOnExtollNetwork node_fpga(
		    ExtollNodeId(0x4), ExtollChipType(ExtollChipType::fpga));

		typename ExtollBarrierConfig::coordinate_type coord_fpga(
		    ExtollBarrierConfigOnExtollNodeId(5), node_fpga);

		std::array<ExtollAddressOnExtollNetwork, ExtollBarrierConfig::config_size_in_words>
		    ref_addresses_fpga = {ExtollAddressOnExtollNetwork(ExtollAddress(0x7'4028), node_fpga)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_fpga, ref_addresses_fpga, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_self(
		    ExtollNodeId(ExtollNodeId::self), ExtollChipType(ExtollChipType::fpga));

		typename ExtollBarrierConfig::coordinate_type coord_self(
		    ExtollBarrierConfigOnExtollNodeId(5), node_self);

		std::array<ExtollAddressOnExtollNetwork, ExtollBarrierConfig::config_size_in_words>
		    ref_addresses_self = {ExtollAddressOnExtollNetwork(ExtollAddress(0x7'4028), node_self)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_self, ref_addresses_self, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_tourmalet(
		    ExtollNodeId(0x3), ExtollChipType(ExtollChipType::tourmalet));

		typename ExtollBarrierConfig::coordinate_type coord_tourmalet(
		    ExtollBarrierConfigOnExtollNodeId(5), node_tourmalet);

		std::array<ExtollAddressOnExtollNetwork, ExtollBarrierConfig::config_size_in_words>
		    ref_addresses_tourmalet = {
		        ExtollAddressOnExtollNetwork(ExtollAddress(0x45'4028), node_tourmalet)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_tourmalet, ref_addresses_tourmalet, ref_data)
	}
}


HALDLS_TEST(ExtollInterruptControl, (operation_trigger)(interrupt))

TEST(ExtollInterruptControl, EncodeDecode)
{
	ExtollInterruptControl config;
	config.set_operation_trigger(true);
	config.set_interrupt(true);

	std::array<network_word_type, ExtollInterruptControl::config_size_in_words> ref_data = {
	    network_word_type(0x3)};

	{
		ExtollNodeIdOnExtollNetwork node_fpga(
		    ExtollNodeId(0x4), ExtollChipType(ExtollChipType::fpga));

		typename ExtollInterruptControl::coordinate_type coord_fpga(
		    ExtollInterruptControlOnExtollNodeId(2), node_fpga);

		std::array<ExtollAddressOnExtollNetwork, ExtollInterruptControl::config_size_in_words>
		    ref_addresses_fpga = {ExtollAddressOnExtollNetwork(ExtollAddress(0x7'2000), node_fpga)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_fpga, ref_addresses_fpga, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_self(
		    ExtollNodeId(ExtollNodeId::self), ExtollChipType(ExtollChipType::fpga));

		typename ExtollInterruptControl::coordinate_type coord_self(
		    ExtollInterruptControlOnExtollNodeId(2), node_self);

		std::array<ExtollAddressOnExtollNetwork, ExtollInterruptControl::config_size_in_words>
		    ref_addresses_self = {ExtollAddressOnExtollNetwork(ExtollAddress(0x7'2000), node_self)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_self, ref_addresses_self, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_tourmalet(
		    ExtollNodeId(0x3), ExtollChipType(ExtollChipType::tourmalet));

		typename ExtollInterruptControl::coordinate_type coord_tourmalet(
		    ExtollInterruptControlOnExtollNodeId(2), node_tourmalet);

		std::array<ExtollAddressOnExtollNetwork, ExtollInterruptControl::config_size_in_words>
		    ref_addresses_tourmalet = {
		        ExtollAddressOnExtollNetwork(ExtollAddress(0x45'2000), node_tourmalet)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_tourmalet, ref_addresses_tourmalet, ref_data)
	}
}


HALDLS_TEST(
    ExtollInterruptConfig,
    (enable)(child_nodes)(delay_count)(enable_reset)(enable_measure)(measure_counter))

TEST(ExtollInterruptConfig, Decode)
{
	ExtollInterruptConfig::LinkFlags child_links;
	child_links.fill(false);
	child_links[LinkOnExtollNode(1)] = true;
	child_links[LinkOnExtollNode(3)] = true;

	ExtollInterruptConfig config;
	config.set_enable(true);
	config.set_child_nodes(child_links); // 0xa
	config.set_delay_count(ExtollInterruptConfig::DelayValue(0xa5a5));
	config.set_enable_reset(true);
	config.set_enable_measure(false);
	config.set_measure_counter(ExtollInterruptConfig::DelayValue(0x5a5a));

	std::array<network_word_type, ExtollInterruptConfig::config_size_in_words> ref_data = {
	    network_word_type(0x0169'69a5'a515)};

	{
		ExtollNodeIdOnExtollNetwork node_fpga(
		    ExtollNodeId(0x4), ExtollChipType(ExtollChipType::fpga));

		typename ExtollInterruptConfig::coordinate_type coord_fpga(
		    ExtollInterruptConfigOnExtollNodeId(2), node_fpga);

		std::array<ExtollAddressOnExtollNetwork, ExtollInterruptConfig::config_size_in_words>
		    ref_addresses_fpga = {ExtollAddressOnExtollNetwork(ExtollAddress(0x7'4090), node_fpga)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_fpga, ref_addresses_fpga, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_self(
		    ExtollNodeId(ExtollNodeId::self), ExtollChipType(ExtollChipType::fpga));

		typename ExtollInterruptConfig::coordinate_type coord_self(
		    ExtollInterruptConfigOnExtollNodeId(2), node_self);

		std::array<ExtollAddressOnExtollNetwork, ExtollInterruptConfig::config_size_in_words>
		    ref_addresses_self = {ExtollAddressOnExtollNetwork(ExtollAddress(0x7'4090), node_self)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_self, ref_addresses_self, ref_data)
	}

	{
		ExtollNodeIdOnExtollNetwork node_tourmalet(
		    ExtollNodeId(0x3), ExtollChipType(ExtollChipType::tourmalet));

		typename ExtollInterruptConfig::coordinate_type coord_tourmalet(
		    ExtollInterruptConfigOnExtollNodeId(2), node_tourmalet);

		std::array<ExtollAddressOnExtollNetwork, ExtollInterruptConfig::config_size_in_words>
		    ref_addresses_tourmalet = {
		        ExtollAddressOnExtollNetwork(ExtollAddress(0x45'4090), node_tourmalet)};

		HALDLS_TEST_ENCODE_DECODE(config, coord_tourmalet, ref_addresses_tourmalet, ref_data)
	}
}


HALDLS_TEST(ExtollBarrierInterruptInportErrorCount, (error_count))

TEST(ExtollBarrierInterruptInportErrorCount, Decode)
{
	ExtollBarrierInterruptInportErrorCount config;
	config.set_error_count(ExtollBarrierInterruptInportErrorCount::ErrorCount(0xaffe'beaf'cafe));

	typename ExtollBarrierInterruptInportErrorCount::coordinate_type coord;

	std::array<ExtollAddress, ExtollBarrierInterruptInportErrorCount::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x7'4100}};

	std::array<word_type, ExtollBarrierInterruptInportErrorCount::read_config_size_in_words>
	    ref_data = {word_type(0xaffe'beaf'cafe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


TEST(ExtollBarrierInterruptInportCounterReset, Encode)
{
	ExtollBarrierInterruptInportCounterReset config;

	typename ExtollBarrierInterruptInportCounterReset::coordinate_type coord;

	std::array<ExtollAddress, ExtollBarrierInterruptInportCounterReset::write_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x7'4800}};

	std::array<word_type, ExtollBarrierInterruptInportCounterReset::write_config_size_in_words>
	    ref_data = {word_type(0x1)};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}
