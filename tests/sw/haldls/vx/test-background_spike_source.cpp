#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/background.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

HALDLS_TEST(BackgroundSpikeSource, (enable)(enable_random)(period)(rate)(seed)(mask)(neuron_label))

TEST(BackgroundSpikeSource, EncodeDecode)
{
	BackgroundSpikeSource config;
	config.set_enable(true);
	config.set_enable_random(false);
	config.set_period(BackgroundSpikeSource::Period(12));
	config.set_rate(BackgroundSpikeSource::Rate(13));
	config.set_seed(BackgroundSpikeSource::Seed(14));
	config.set_mask(BackgroundSpikeSource::Mask(15));
	config.set_neuron_label(halco::hicann_dls::vx::NeuronLabel(16));

	auto coord = typename BackgroundSpikeSource::coordinate_type(3);

	std::array<OmnibusChipOverJTAGAddress, BackgroundSpikeSource::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x130001 + coord.toEnum() * 3 + 1},
	                     OmnibusChipOverJTAGAddress{0x130001 + coord.toEnum() * 3 + 2},
	                     OmnibusChipOverJTAGAddress{0x130001 + coord.toEnum() * 3 + 0}};

	std::array<
	    fisch::vx::word_access_type::OmnibusChipOverJTAG,
	    BackgroundSpikeSource::config_size_in_words>
	    ref_data = {
	        fisch::vx::word_access_type::OmnibusChipOverJTAG(14ul), // seed
	        fisch::vx::word_access_type::OmnibusChipOverJTAG(
	            15ul | (13ul << 8) | (16ul << 16)), // rate, mask, neuron_label
	        fisch::vx::word_access_type::OmnibusChipOverJTAG(
	            1ul | (12ul << 16))}; // period, enable, enable_random, written last

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}
