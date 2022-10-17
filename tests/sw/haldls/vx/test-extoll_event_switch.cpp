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

typedef fisch::vx::word_access_type::Omnibus word_type;

HALDLS_TEST(EventSwitchSource, (source))

TEST(EventSwitchToExecutorSource, EncodeDecode)
{
	EventSwitchSource config;
	config.set_source(EventSwitchSource::Source(EventSwitchSource::Source::external));

	EventSwitchSourceOnFPGA coord(EventSwitchSourceOnFPGA::to_executor);

	std::array<OmnibusAddress, EventSwitchSource::config_size_in_words> ref_addresses = {
	    OmnibusAddress{0x8800'7c00}};

	std::array<word_type, EventSwitchSource::config_size_in_words> ref_data = {word_type(0x2)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


TEST(EventSwitchToAsicSource, EncodeDecode)
{
	EventSwitchSource config;
	config.set_source(EventSwitchSource::Source(EventSwitchSource::Source::off));

	EventSwitchSourceOnFPGA coord(EventSwitchSourceOnFPGA::to_asic);

	std::array<OmnibusAddress, EventSwitchSource::config_size_in_words> ref_addresses = {
	    OmnibusAddress{0x8800'7c01}};

	std::array<word_type, EventSwitchSource::config_size_in_words> ref_data = {word_type(0x3)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


TEST(EventSwitchToExternalSource, EncodeDecode)
{
	EventSwitchSource config;
	config.set_source(EventSwitchSource::Source(EventSwitchSource::Source::executor));

	EventSwitchSourceOnFPGA coord(EventSwitchSourceOnFPGA::to_external);

	std::array<OmnibusAddress, EventSwitchSource::config_size_in_words> ref_addresses = {
	    OmnibusAddress{0x8800'7c02}};

	std::array<word_type, EventSwitchSource::config_size_in_words> ref_data = {word_type(0x0)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}
