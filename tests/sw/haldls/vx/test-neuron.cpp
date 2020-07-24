#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "fisch/vx/jtag.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/common.h"
#include "haldls/vx/neuron.h"
#include "haldls/vx/omnibus_constants.h"
#include "test-helper.h"

using namespace fisch::vx;
using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> addresses_type;
typedef std::vector<fisch::vx::OmnibusChipOverJTAG> words_type;

TEST(CommonNeuronBackendConfig, General)
{
	CommonNeuronBackendConfig config;

	auto value = !config.get_enable_event_registers();
	config.set_enable_event_registers(value);
	ASSERT_EQ(config.get_enable_event_registers(), value);

	value = !config.get_force_reset();
	config.set_force_reset(value);
	ASSERT_EQ(config.get_force_reset(), value);

	value = !config.get_enable_clocks();
	config.set_enable_clocks(value);
	ASSERT_EQ(config.get_enable_clocks(), value);

	auto clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_slow(clock);
	ASSERT_EQ(config.get_clock_scale_slow(), clock);

	clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_fast(clock);
	ASSERT_EQ(config.get_clock_scale_fast(), clock);

	config.set_sample_positive_edge(NeuronEventOutputOnNeuronBackendBlock(0), false);
	ASSERT_EQ(config.get_sample_positive_edge(NeuronEventOutputOnNeuronBackendBlock(0)), false);

	clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_adaptation_pulse(clock);
	ASSERT_EQ(config.get_clock_scale_adaptation_pulse(), clock);

	clock = draw_ranged_non_default_value<CommonNeuronBackendConfig::ClockScale>();
	config.set_clock_scale_post_pulse(clock);
	ASSERT_EQ(config.get_clock_scale_post_pulse(), clock);

	auto w_post_debug = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitGlobalPostPulse>();
	config.set_wait_global_post_pulse(w_post_debug);
	ASSERT_EQ(config.get_wait_global_post_pulse(), w_post_debug);

	auto w_counter_reset = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitSpikeCounterReset>();
	config.set_wait_spike_counter_reset(w_counter_reset);
	ASSERT_EQ(config.get_wait_spike_counter_reset(), w_counter_reset);

	auto w_counter_read = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitSpikeCounterRead>();
	config.set_wait_spike_counter_read(w_counter_read);
	ASSERT_EQ(config.get_wait_spike_counter_read(), w_counter_read);

	auto w_post_ind = draw_ranged_non_default_value<CommonNeuronBackendConfig::WaitFireNeuron>();
	config.set_wait_fire_neuron(w_post_ind);
	ASSERT_EQ(config.get_wait_fire_neuron(), w_post_ind);

	CommonNeuronBackendConfig default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	CommonNeuronBackendConfig copy_config = config;
	ASSERT_EQ(config, copy_config);
	ASSERT_TRUE(config == copy_config);
	ASSERT_FALSE(config != copy_config);
}

TEST(CommonNeuronBackendConfig, EncodeDecode)
{
	CommonNeuronBackendConfig config;
	config.set_enable_event_registers(false);
	config.set_force_reset(true);
	config.set_enable_clocks(true);
	config.set_clock_scale_slow(CommonNeuronBackendConfig::ClockScale(3));
	config.set_clock_scale_fast(CommonNeuronBackendConfig::ClockScale(7));
	config.set_sample_positive_edge(NeuronEventOutputOnNeuronBackendBlock(1), false);
	config.set_clock_scale_adaptation_pulse(CommonNeuronBackendConfig::ClockScale(4));
	config.set_clock_scale_post_pulse(CommonNeuronBackendConfig::ClockScale(8));
	config.set_wait_global_post_pulse(CommonNeuronBackendConfig::WaitGlobalPostPulse(186));
	config.set_wait_spike_counter_reset(CommonNeuronBackendConfig::WaitSpikeCounterReset(220));
	config.set_wait_spike_counter_read(CommonNeuronBackendConfig::WaitSpikeCounterRead(13));
	config.set_wait_fire_neuron(CommonNeuronBackendConfig::WaitFireNeuron(237));

	auto backend_coord = CommonNeuronBackendConfigOnDLS(1);

	std::array<OmnibusChipOverJTAGAddress, CommonNeuronBackendConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x1a9800 + 0},
	                     OmnibusChipOverJTAGAddress{0x1a9800 + 1}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, backend_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // check if read addresses are correct
		addresses_type read_addresses;
		visit_preorder(
		    config, backend_coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, backend_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_FALSE(data[0].get() & 0b1);                                 // en_event_regs
	ASSERT_TRUE(data[0].get() & 0b10);                                 // force_reset
	ASSERT_TRUE(data[0].get() & 0b100);                                // en_clocks
	ASSERT_TRUE((data[0].get() & 0b1111'0000) == (3 << 4));            // clock_scale_slow
	ASSERT_TRUE((data[0].get() & 0b1111'0000'0000) == (7 << 8));       // clock_scale_fast
	ASSERT_TRUE((data[0].get() & 0b1111'0000'0000'0000) == (13 << 12)); // sample_pos_edge
	ASSERT_TRUE(
	    (data[0].get() & 0b1111'0000'0000'0000'0000) == (4 << 16)); // clock_scale_adapt_pulse
	ASSERT_TRUE(
	    (data[0].get() & 0b1111'0000'0000'0000'0000'0000) == (8 << 20)); // clock_scale_post_pulse
	ASSERT_TRUE(
	    (data[1].get() & 0xFF) == (unsigned int)(186 << 0));          // wait_global_post_pulse
	ASSERT_TRUE(
	    (data[1].get() & 0xFF00) == (unsigned int)(220 << 8));        // wait_spike_counter_reset
	ASSERT_TRUE(
	    (data[1].get() & 0xFF0000) == (unsigned int)(13 << 16));      // wait_spike_counter_read
	ASSERT_TRUE(
	    (data[1].get() & 0xFF000000) == (unsigned int)(237 << 24));   // wait_fire_neuron

	// Decode back
	CommonNeuronBackendConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, backend_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(CommonNeuronBackendConfig, CerealizeCoverage)
{
	CommonNeuronBackendConfig c1, c2;
	c1.set_enable_event_registers(true);
	c1.set_force_reset(true);
	c1.set_enable_clocks(true);
	c1.set_clock_scale_fast(CommonNeuronBackendConfig::ClockScale(7));
	c1.set_clock_scale_slow(CommonNeuronBackendConfig::ClockScale(7));
	c1.set_sample_positive_edge(NeuronEventOutputOnNeuronBackendBlock(0), true);
	c1.set_clock_scale_adaptation_pulse(CommonNeuronBackendConfig::ClockScale(10));
	c1.set_clock_scale_post_pulse(CommonNeuronBackendConfig::ClockScale(10));
	c1.set_wait_global_post_pulse(CommonNeuronBackendConfig::WaitGlobalPostPulse(186));
	c1.set_wait_spike_counter_reset(CommonNeuronBackendConfig::WaitSpikeCounterReset(220));
	c1.set_wait_spike_counter_read(CommonNeuronBackendConfig::WaitSpikeCounterRead(13));
	c1.set_wait_fire_neuron(CommonNeuronBackendConfig::WaitFireNeuron(237));
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(NeuronReset, General)
{
	NeuronReset config;

	NeuronReset default_config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(NeuronReset, EncodeDecode)
{
	NeuronReset config;

	auto neuron_coord =
	    NeuronResetOnDLS(NeuronResetOnNeuronResetBlock(23), NeuronResetBlockOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, NeuronReset::write_config_size_in_words> ref_addresses =
	    {OmnibusChipOverJTAGAddress{0x12205c}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get(), 0x0);
}

TEST(NeuronReset, CerealizeCoverage)
{
	NeuronReset c1, c2;
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(BlockPostPulse, General)
{
	BlockPostPulse config;

	BlockPostPulse default_config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(BlockPostPulse, EncodeDecode)
{
	BlockPostPulse config;

	auto neuron_coord = BlockPostPulseOnDLS(BlockPostPulseOnDLS(1));

	std::array<OmnibusChipOverJTAGAddress, BlockPostPulse::write_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x0012'a400}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get(), 0x0);
}

TEST(BlockPostPulse, CerealizeCoverage)
{
	BlockPostPulse c1, c2;
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(SpikeCounterRead, General)
{
	SpikeCounterRead config;
	config.set_count(SpikeCounterRead::Count(100));
	config.set_overflow(true);

	SpikeCounterRead default_config;
	ASSERT_NE(config, default_config);
	ASSERT_TRUE(config != default_config);
	ASSERT_FALSE(config == default_config);
	default_config = config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(SpikeCounterRead, EncodeDecode)
{
	SpikeCounterRead config;
	config.set_count(SpikeCounterRead::Count(100));
	config.set_overflow(true);

	auto neuron_coord = SpikeCounterReadOnDLS(
	    SpikeCounterReadOnSpikeCounterReadBlock(22), SpikeCounterReadBlockOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, SpikeCounterRead::read_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x122858}};

	{ // check if read addresses are correct
		addresses_type read_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Decode
	words_type data = {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x164))};
	SpikeCounterRead config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, neuron_coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(SpikeCounterRead, CerealizeCoverage)
{
	SpikeCounterRead c1, c2;
	c1.set_count(SpikeCounterRead::Count(100));
	c1.set_overflow(true);
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(SpikeCounterReset, General)
{
	SpikeCounterReset config;

	SpikeCounterReset default_config;
	ASSERT_EQ(config, default_config);
	ASSERT_TRUE(config == default_config);
	ASSERT_FALSE(config != default_config);
}

TEST(SpikeCounterReset, EncodeDecode)
{
	SpikeCounterReset config;

	auto neuron_coord = SpikeCounterResetOnDLS(
	    SpikeCounterResetOnSpikeCounterResetBlock(21), SpikeCounterResetBlockOnDLS(0));

	std::array<OmnibusChipOverJTAGAddress, SpikeCounterReset::write_config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{0x122c54}};

	{ // check if write addresses are correct
		addresses_type write_addresses;
		visit_preorder(
		    config, neuron_coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	// Encode
	words_type data;
	visit_preorder(config, neuron_coord, stadls::EncodeVisitor<words_type>{data});
	ASSERT_EQ(data[0].get(), 0x0);
}

TEST(SpikeCounterReset, CerealizeCoverage)
{
	SpikeCounterReset c1, c2;
	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(c1);
	}
	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(c2);
	}
	ASSERT_EQ(c1, c2);
}

TEST(NeuronSRAMTimingConfig, General)
{
	NeuronSRAMTimingConfig config;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<NeuronSRAMTimingConfig::ReadDelay>(
		    config.get_read_delay());
		config.set_read_delay(value);
		EXPECT_EQ(config.get_read_delay(), value);
	}

	{
		auto value = draw_ranged_non_default_value<NeuronSRAMTimingConfig::AddressSetupTime>(
		    config.get_address_setup_time());
		config.set_address_setup_time(value);
		EXPECT_EQ(config.get_address_setup_time(), value);
	}

	{
		auto value = draw_ranged_non_default_value<NeuronSRAMTimingConfig::EnableWidth>(
		    config.get_enable_width());
		config.set_enable_width(value);
		EXPECT_EQ(config.get_enable_width(), value);
	}

	NeuronSRAMTimingConfig config_eq = config;
	NeuronSRAMTimingConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(NeuronSRAMTimingConfig, EncodeDecode)
{
	NeuronSRAMTimingConfig config;

	config.set_read_delay(NeuronSRAMTimingConfig::ReadDelay(100));
	config.set_address_setup_time(NeuronSRAMTimingConfig::AddressSetupTime(5));
	config.set_enable_width(NeuronSRAMTimingConfig::EnableWidth(7));

	auto coord = NeuronSRAMTimingConfigOnDLS(1);

	std::array<OmnibusChipOverJTAGAddress, NeuronSRAMTimingConfig::config_size_in_words>
	    ref_addresses = {OmnibusChipOverJTAGAddress{neuron_ne_sram_timing_base_address},
	                     OmnibusChipOverJTAGAddress{neuron_ne_sram_timing_base_address + 1}};
	std::array<OmnibusChipOverJTAG, NeuronSRAMTimingConfig::config_size_in_words> ref_data = {
	    OmnibusChipOverJTAG(OmnibusData{100}), OmnibusChipOverJTAG(OmnibusData{5 | 7 << 4})};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	NeuronSRAMTimingConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(NeuronSRAMTimingConfig, CerealizeCoverage)
{
	NeuronSRAMTimingConfig obj1, obj2;
	{
		auto value =
		    draw_ranged_non_default_value<NeuronSRAMTimingConfig::ReadDelay>(obj1.get_read_delay());
		obj1.set_read_delay(value);
	}
	{
		auto value = draw_ranged_non_default_value<NeuronSRAMTimingConfig::AddressSetupTime>(
		    obj1.get_address_setup_time());
		obj1.set_address_setup_time(value);
	}
	{
		auto value = draw_ranged_non_default_value<NeuronSRAMTimingConfig::EnableWidth>(
		    obj1.get_enable_width());
		obj1.set_enable_width(value);
	}

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

TEST(NeuronBackendSRAMTimingConfig, General)
{
	NeuronBackendSRAMTimingConfig config;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<NeuronBackendSRAMTimingConfig::ReadDelay>(
		    config.get_read_delay());
		config.set_read_delay(value);
		EXPECT_EQ(config.get_read_delay(), value);
	}

	{
		auto value = draw_ranged_non_default_value<NeuronBackendSRAMTimingConfig::AddressSetupTime>(
		    config.get_address_setup_time());
		config.set_address_setup_time(value);
		EXPECT_EQ(config.get_address_setup_time(), value);
	}

	{
		auto value = draw_ranged_non_default_value<NeuronBackendSRAMTimingConfig::EnableWidth>(
		    config.get_enable_width());
		config.set_enable_width(value);
		EXPECT_EQ(config.get_enable_width(), value);
	}

	NeuronBackendSRAMTimingConfig config_eq = config;
	NeuronBackendSRAMTimingConfig config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(NeuronBackendSRAMTimingConfig, EncodeDecode)
{
	NeuronBackendSRAMTimingConfig config;

	config.set_read_delay(NeuronBackendSRAMTimingConfig::ReadDelay(100));
	config.set_address_setup_time(NeuronBackendSRAMTimingConfig::AddressSetupTime(5));
	config.set_enable_width(NeuronBackendSRAMTimingConfig::EnableWidth(7));

	auto coord = NeuronBackendSRAMTimingConfigOnDLS(1);

	std::array<OmnibusChipOverJTAGAddress, NeuronBackendSRAMTimingConfig::config_size_in_words>
	    ref_addresses = {
	        OmnibusChipOverJTAGAddress{neuron_backend_east_sram_timing_base_address},
	        OmnibusChipOverJTAGAddress{neuron_backend_east_sram_timing_base_address + 1}};
	std::array<OmnibusChipOverJTAG, NeuronBackendSRAMTimingConfig::config_size_in_words> ref_data =
	    {OmnibusChipOverJTAG(OmnibusData{100}), OmnibusChipOverJTAG(OmnibusData{5 | 7 << 4})};

	{ // write addresses
		addresses_type write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	{ // read addresses
		addresses_type read_addresses;
		visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
		EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

	NeuronBackendSRAMTimingConfig config_copy;
	ASSERT_NE(config, config_copy);
	visit_preorder(config_copy, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	ASSERT_EQ(config, config_copy);
}

TEST(NeuronBackendSRAMTimingConfig, CerealizeCoverage)
{
	NeuronBackendSRAMTimingConfig obj1, obj2;
	{
		auto value = draw_ranged_non_default_value<NeuronBackendSRAMTimingConfig::ReadDelay>(
		    obj1.get_read_delay());
		obj1.set_read_delay(value);
	}
	{
		auto value = draw_ranged_non_default_value<NeuronBackendSRAMTimingConfig::AddressSetupTime>(
		    obj1.get_address_setup_time());
		obj1.set_address_setup_time(value);
	}
	{
		auto value = draw_ranged_non_default_value<NeuronBackendSRAMTimingConfig::EnableWidth>(
		    obj1.get_enable_width());
		obj1.set_enable_width(value);
	}

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
