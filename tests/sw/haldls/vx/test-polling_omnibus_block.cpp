#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/block.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

HALDLS_TEST(PollingOmnibusBlockConfig, (address)(mask)(target))

TEST(PollingOmnibusBlockConfig, EncodeDecode)
{
	PollingOmnibusBlockConfig config;
	config.set_address(PollingOmnibusBlockConfig::Address(0x12345678));
	config.set_mask(PollingOmnibusBlockConfig::Value(0x87654321));
	config.set_target(PollingOmnibusBlockConfig::Value(0x12121212));

	typename PollingOmnibusBlockConfig::coordinate_type coord;

	std::array<OmnibusAddress, PollingOmnibusBlockConfig::config_size_in_words> ref_addresses = {
	    OmnibusAddress{0x8800'0006}, OmnibusAddress{0x8800'0007}, OmnibusAddress{0x8800'0008}};

	std::array<
	    fisch::vx::word_access_type::Omnibus, PollingOmnibusBlockConfig::config_size_in_words>
	    ref_data = {
	        fisch::vx::word_access_type::Omnibus(0x12345678),
	        fisch::vx::word_access_type::Omnibus(0x12121212),
	        fisch::vx::word_access_type::Omnibus(0x87654321)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}

HALDLS_TEST(PollingOmnibusBlock, (enable_expects_equality))

TEST(PollingOmnibusBlock, Encode)
{
	PollingOmnibusBlock config(true);

	auto const encoded = config.encode().at(0);
	EXPECT_EQ(encoded, fisch::vx::word_access_type::PollingOmnibusBlock(true));
}
