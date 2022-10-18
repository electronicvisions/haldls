#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/fpga.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

HALDLS_TEST(FPGASystimeSyncNumRetries, (value))

TEST(FPGASystimeSyncNumRetries, EncodeDecode)
{
	FPGASystimeSyncNumRetries config;
	config.set_value(FPGASystimeSyncNumRetries::Value(0xa));

	typename FPGASystimeSyncNumRetries::coordinate_type coord;

	std::array<OmnibusAddress, FPGASystimeSyncNumRetries::config_size_in_words> ref_addresses = {
	    OmnibusAddress{0x8800'2000ul}};

	std::array<
	    fisch::vx::word_access_type::Omnibus, FPGASystimeSyncNumRetries::config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::Omnibus(0xa)};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(FPGASystimeSyncLastRTT, (value))

TEST(FPGASystimeSyncLastRTT, Decode)
{
	FPGASystimeSyncLastRTT config;
	config.set_value(FPGASystimeSyncLastRTT::Systime(0x700'0000'022b));

	typename FPGASystimeSyncLastRTT::coordinate_type coord;

	std::array<OmnibusAddress, FPGASystimeSyncLastRTT::read_config_size_in_words> ref_addresses = {
	    OmnibusAddress{0x8800'2001ul}, OmnibusAddress{0x8800'2002ul}};

	std::array<
	    fisch::vx::word_access_type::Omnibus, FPGASystimeSyncLastRTT::read_config_size_in_words>
	    ref_data = {
	        fisch::vx::word_access_type::Omnibus(0x700),
	        fisch::vx::word_access_type::Omnibus(0x22b)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(FPGASystimeSyncLastAsicSystime, (value))

TEST(FPGASystimeSyncLastAsicSystime, Decode)
{
	FPGASystimeSyncLastAsicSystime config;
	config.set_value(FPGASystimeSyncLastAsicSystime::Systime(0x700'0000'022b));

	typename FPGASystimeSyncLastAsicSystime::coordinate_type coord;

	std::array<OmnibusAddress, FPGASystimeSyncLastAsicSystime::read_config_size_in_words>
	    ref_addresses = {OmnibusAddress{0x8800'2003ul}, OmnibusAddress{0x8800'2004ul}};

	std::array<
	    fisch::vx::word_access_type::Omnibus,
	    FPGASystimeSyncLastAsicSystime::read_config_size_in_words>
	    ref_data = {
	        fisch::vx::word_access_type::Omnibus(0x700),
	        fisch::vx::word_access_type::Omnibus(0x22b)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}


HALDLS_TEST(FPGASystimeSyncActiveState, (active_state))

TEST(FPGASystimeSyncActiveState, Decode)
{
	FPGASystimeSyncActiveState config;
	config.set_active_state(true);

	typename FPGASystimeSyncActiveState::coordinate_type coord;

	std::array<OmnibusAddress, FPGASystimeSyncActiveState::read_config_size_in_words>
	    ref_addresses = {OmnibusAddress{0x8800'2005ul}};

	std::array<
	    fisch::vx::word_access_type::Omnibus, FPGASystimeSyncActiveState::read_config_size_in_words>
	    ref_data = {fisch::vx::word_access_type::Omnibus(0x1)};

	HALDLS_TEST_DECODE(config, coord, ref_addresses, ref_data)
}
