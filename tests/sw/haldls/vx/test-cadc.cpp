#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"


using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace fisch::vx;

HALDLS_TEST(CADCConfig, (enable)(reset_wait)(dead_time))

TEST(CADCConfig, EncodeDecode)
{
	CADCConfig config;

	config.set_enable(true);
	config.set_reset_wait(CADCConfig::ResetWait(12));
	config.set_dead_time(CADCConfig::DeadTime(0));

	CADCConfigOnDLS coord(0);

	std::array<OmnibusChipOverJTAGAddress, CADCConfig::config_size_in_words> ref_addresses = {
	    OmnibusChipOverJTAGAddress{0x0240'1000ul}};
	std::array<OmnibusChipOverJTAG, CADCConfig::config_size_in_words> ref_data = {
	    OmnibusData{1 | 12 << 4}};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}

HALDLS_TEST(CADCOffsetSRAMTimingConfig, (read_delay)(address_setup_time)(enable_width))

TEST(CADCOffsetSRAMTimingConfig, EncodeDecode)
{
	CADCOffsetSRAMTimingConfig config;

	config.set_read_delay(CADCOffsetSRAMTimingConfig::ReadDelay(100));
	config.set_address_setup_time(CADCOffsetSRAMTimingConfig::AddressSetupTime(5));
	config.set_enable_width(CADCOffsetSRAMTimingConfig::EnableWidth(7));

	auto coord = CADCOffsetSRAMTimingConfigOnDLS(1);

	std::array<OmnibusChipOverJTAGAddress, CADCOffsetSRAMTimingConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{cadc_top_sram_timing_east_base_address},
	                     OmnibusChipOverJTAGAddress{cadc_top_sram_timing_east_base_address + 1}};
	std::array<OmnibusChipOverJTAG, CADCOffsetSRAMTimingConfig::config_size_in_words> ref_data = {
	    OmnibusData{100}, OmnibusData{5 | 7 << 4}};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}
