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

typedef fisch::vx::word_access_type::Extoll word_type;

HALDLS_TEST(ExtollSpikeCommBucketTriggerConfig, (pulse_timeout)(packet_timeout))

TEST(ExtollSpikeCommBucketTriggerConfig, EncodeDecode)
{
	ExtollSpikeCommBucketTriggerConfig config;
	config.set_pulse_timeout(ExtollSpikeCommBucketTriggerConfig::PulseTimeout(0xab));
	config.set_packet_timeout(ExtollSpikeCommBucketTriggerConfig::PacketTimeout(0xba));

	typename ExtollSpikeCommBucketTriggerConfig::coordinate_type coord(5);

	std::array<ExtollAddress, ExtollSpikeCommBucketTriggerConfig::config_size_in_words>
	    ref_addresses = {ExtollAddress{0x20'00a0}};

	std::array<word_type, ExtollSpikeCommBucketTriggerConfig::config_size_in_words> ref_data = {
	    word_type(0xbaab)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(
    ExtollSpikeCommBucketDestinationConfig,
    (destination_node_id)(enable_multicast)(destination_delay))

TEST(ExtollSpikeCommBucketDestinationConfig, EncodeDecode)
{
	ExtollSpikeCommBucketDestinationConfig config;
	config.set_destination_node_id(
	    ExtollSpikeCommBucketDestinationConfig::DestinationNodeID(0xabab));
	config.set_enable_multicast(true);
	config.set_destination_delay(ExtollSpikeCommBucketDestinationConfig::DestinationDelay(0x2ab));

	typename ExtollSpikeCommBucketDestinationConfig::coordinate_type coord(5);

	std::array<ExtollAddress, ExtollSpikeCommBucketDestinationConfig::config_size_in_words>
	    ref_addresses = {ExtollAddress{0x20'00a8}};

	std::array<word_type, ExtollSpikeCommBucketDestinationConfig::config_size_in_words> ref_data = {
	    word_type(0x557'abab)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommBucketNumPktsSent, (packet_count))

TEST(ExtollSpikeCommBucketNumPktsSent, Decode)
{
	ExtollSpikeCommBucketNumPktsSent config;
	config.set_packet_count(ExtollSpikeCommBucketNumPktsSent::PacketCount(0xbeaf'affe'cafe));

	typename ExtollSpikeCommBucketNumPktsSent::coordinate_type coord(5);

	std::array<ExtollAddress, ExtollSpikeCommBucketNumPktsSent::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x20'00b0}};

	std::array<word_type, ExtollSpikeCommBucketNumPktsSent::read_config_size_in_words> ref_data = {
	    word_type(0xbeaf'affe'cafe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


TEST(ExtollSpikeCommBucketCounterReset, Encode)
{
	ExtollSpikeCommBucketCounterReset config;

	typename ExtollSpikeCommBucketCounterReset::coordinate_type coord(5);

	std::array<ExtollAddress, ExtollSpikeCommBucketCounterReset::write_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x20'00b8}};

	std::array<word_type, ExtollSpikeCommBucketCounterReset::write_config_size_in_words> ref_data =
	    {word_type(0x1)};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommRouterLookupConfig, (entry_valid)(destination_event)(bucket_id))

TEST(ExtollSpikeCommRouterLookupConfig, EncodeDecode)
{
	ExtollSpikeCommRouterLookupConfig config;
	config.set_bucket_id(ExtollSpikeCommRouterLookupConfig::BucketID(0x5));
	config.set_destination_event(ExtollSpikeCommRouterLookupConfig::DestinationEvent(0x2caf));
	config.set_entry_valid(true);

	typename ExtollSpikeCommRouterLookupConfig::coordinate_type coord(
	    ExtollSpikeCommRouterLookupConfigOnSpikeCommSplit(0x2bab), ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommRouterLookupConfig::config_size_in_words>
	    ref_addresses = {ExtollAddress{0x29'5d58}};

	std::array<word_type, ExtollSpikeCommRouterLookupConfig::config_size_in_words> ref_data = {
	    word_type(0x3'657d)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommRouterConfig, (enable_routing)(enable_drop_if_routing_disabled))

TEST(ExtollSpikeCommRouterConfig, EncodeDecode)
{
	ExtollSpikeCommRouterConfig config;
	config.set_enable_routing(true);
	config.set_enable_drop_if_routing_disabled(true);

	typename ExtollSpikeCommRouterConfig::coordinate_type coord(ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommRouterConfig::config_size_in_words> ref_addresses = {
	    ExtollAddress{0x2a'0000}};

	std::array<word_type, ExtollSpikeCommRouterConfig::config_size_in_words> ref_data = {
	    word_type(0x3)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommRouterEventLossDisabled, (lost_event_count))

TEST(ExtollSpikeCommRouterEventLossDisabled, Decode)
{
	ExtollSpikeCommRouterEventLossDisabled config;
	config.set_lost_event_count(
	    ExtollSpikeCommRouterEventLossDisabled::LostEventCount(0xbeaf'cafe'babe));

	typename ExtollSpikeCommRouterEventLossDisabled::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommRouterEventLossDisabled::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x2a'0008}};

	std::array<word_type, ExtollSpikeCommRouterEventLossDisabled::read_config_size_in_words>
	    ref_data = {word_type(0xbeaf'cafe'babe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommRouterEventLossMisconf, (lost_event_count))

TEST(ExtollSpikeCommRouterEventLossMisconf, Decode)
{
	ExtollSpikeCommRouterEventLossMisconf config;
	config.set_lost_event_count(
	    ExtollSpikeCommRouterEventLossMisconf::LostEventCount(0xbeaf'cafe'babe));

	typename ExtollSpikeCommRouterEventLossMisconf::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommRouterEventLossMisconf::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x2a'0010}};

	std::array<word_type, ExtollSpikeCommRouterEventLossMisconf::read_config_size_in_words>
	    ref_data = {word_type(0xbeaf'cafe'babe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommRouterEventsRouted, (event_count))

TEST(ExtollSpikeCommRouterEventsRouted, Decode)
{
	ExtollSpikeCommRouterEventsRouted config;
	config.set_event_count(ExtollSpikeCommRouterEventsRouted::EventCount(0xbeaf'cafe'babe));

	typename ExtollSpikeCommRouterEventsRouted::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommRouterEventsRouted::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x2a'0018}};

	std::array<word_type, ExtollSpikeCommRouterEventsRouted::read_config_size_in_words> ref_data = {
	    word_type(0xbeaf'cafe'babe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


TEST(ExtollSpikeCommRouterCounterReset, Encode)
{
	ExtollSpikeCommRouterCounterReset config;

	typename ExtollSpikeCommRouterCounterReset::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommRouterCounterReset::write_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x2a'0020}};

	std::array<word_type, ExtollSpikeCommRouterCounterReset::write_config_size_in_words> ref_data =
	    {word_type(0x1)};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommDecoderNumEventsReceived, (event_count))

TEST(ExtollSpikeCommDecoderNumEventsReceived, Decode)
{
	ExtollSpikeCommDecoderNumEventsReceived config;
	config.set_event_count(ExtollSpikeCommDecoderNumEventsReceived::EventCount(0xbeaf'cafe'babe));

	typename ExtollSpikeCommDecoderNumEventsReceived::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommDecoderNumEventsReceived::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x30'0008}};

	std::array<word_type, ExtollSpikeCommDecoderNumEventsReceived::read_config_size_in_words>
	    ref_data = {word_type(0xbeaf'cafe'babe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


TEST(ExtollSpikeCommDecoderCounterReset, Encode)
{
	ExtollSpikeCommDecoderCounterReset config;

	typename ExtollSpikeCommDecoderCounterReset::coordinate_type coord;

	std::array<ExtollAddress, ExtollSpikeCommDecoderCounterReset::write_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x30'0010}};

	std::array<word_type, ExtollSpikeCommDecoderCounterReset::write_config_size_in_words> ref_data =
	    {word_type(0x1)};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommTimestampDelayConfig, (forward_immediately)(timestamp_to_systime))

TEST(ExtollSpikeCommTimestampDelayConfig, EncodeDecode)
{
	ExtollSpikeCommTimestampDelayConfig config;
	config.set_forward_immediately(true);
	config.set_timestamp_to_systime(true);

	typename ExtollSpikeCommTimestampDelayConfig::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommTimestampDelayConfig::config_size_in_words>
	    ref_addresses = {ExtollAddress{0x30'0080}};

	std::array<word_type, ExtollSpikeCommTimestampDelayConfig::config_size_in_words> ref_data = {
	    word_type(0x3)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommTimestampDelayNumEventsReceived, (event_count))

TEST(ExtollSpikeCommTimestampDelayNumEventsReceived, Decode)
{
	ExtollSpikeCommTimestampDelayNumEventsReceived config;
	config.set_event_count(
	    ExtollSpikeCommTimestampDelayNumEventsReceived::EventCount(0xbeaf'cafe'babe));

	typename ExtollSpikeCommTimestampDelayNumEventsReceived::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<
	    ExtollAddress, ExtollSpikeCommTimestampDelayNumEventsReceived::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x30'0088}};

	std::array<word_type, ExtollSpikeCommTimestampDelayNumEventsReceived::read_config_size_in_words>
	    ref_data = {word_type(0xbeaf'cafe'babe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommTimestampDelayEventLossFull, (event_count))

TEST(ExtollSpikeCommTimestampDelayEventLossFull, Decode)
{
	ExtollSpikeCommTimestampDelayEventLossFull config;
	config.set_event_count(
	    ExtollSpikeCommTimestampDelayEventLossFull::EventCount(0xbeaf'cafe'babe));

	typename ExtollSpikeCommTimestampDelayEventLossFull::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommTimestampDelayEventLossFull::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x30'0090}};

	std::array<word_type, ExtollSpikeCommTimestampDelayEventLossFull::read_config_size_in_words>
	    ref_data = {word_type(0xbeaf'cafe'babe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(ExtollSpikeCommTimestampDelayEventLossExpired, (event_count))

TEST(ExtollSpikeCommTimestampDelayEventLossExpired, Decode)
{
	ExtollSpikeCommTimestampDelayEventLossExpired config;
	config.set_event_count(
	    ExtollSpikeCommTimestampDelayEventLossExpired::EventCount(0xbeaf'cafe'babe));

	typename ExtollSpikeCommTimestampDelayEventLossExpired::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<
	    ExtollAddress, ExtollSpikeCommTimestampDelayEventLossExpired::read_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x30'0098}};

	std::array<word_type, ExtollSpikeCommTimestampDelayEventLossExpired::read_config_size_in_words>
	    ref_data = {word_type(0xbeaf'cafe'babe)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


TEST(ExtollSpikeCommTimestampDelayCounterReset, Encode)
{
	ExtollSpikeCommTimestampDelayCounterReset config;

	typename ExtollSpikeCommTimestampDelayCounterReset::coordinate_type coord(
	    ExtollSpikeCommSplitOnFPGA(1));

	std::array<ExtollAddress, ExtollSpikeCommTimestampDelayCounterReset::write_config_size_in_words>
	    ref_addresses = {ExtollAddress{0x30'00a0}};

	std::array<word_type, ExtollSpikeCommTimestampDelayCounterReset::write_config_size_in_words>
	    ref_data = {word_type(0x1)};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}
