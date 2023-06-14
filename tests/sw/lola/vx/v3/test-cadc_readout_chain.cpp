#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v3/cadc.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/lola/vx/v3/cadc.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/v3/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace lola::vx::v3;
using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx::v3;
using namespace halco::common;

TEST(CADCReadoutChain, General)
{
	{
		CADCReadoutChain config;
		config.ramp.enable = !config.ramp.enable;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.reset_wait = draw_ranged_non_default_value<CADCReadoutChain::Ramp::ResetWait>(
		    config.ramp.reset_wait);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.dead_time =
		    draw_ranged_non_default_value<CADCReadoutChain::Ramp::DeadTime>(config.ramp.dead_time);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.v_offset_assignment[CapMemBlockOnHemisphere(0)] = CapMemBlockOnHemisphere(1);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.v_offset_assignment[CapMemBlockOnHemisphere(1)] = CapMemBlockOnHemisphere(0);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.v_offset[CapMemBlockOnHemisphere(0)] = CADCReadoutChain::Ramp::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.v_offset[CapMemBlockOnHemisphere(1)] = CADCReadoutChain::Ramp::AnalogValue(321);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.i_slope[CapMemBlockOnHemisphere(0)] = CADCReadoutChain::Ramp::AnalogValue(234);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.i_slope[CapMemBlockOnHemisphere(1)] = CADCReadoutChain::Ramp::AnalogValue(432);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.i_bias_comparator[CapMemBlockOnHemisphere(0)] =
		    CADCReadoutChain::Ramp::AnalogValue(345);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.i_bias_comparator[CapMemBlockOnHemisphere(1)] =
		    CADCReadoutChain::Ramp::AnalogValue(543);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.i_bias_vreset_buffer[CapMemBlockOnHemisphere(0)] =
		    CADCReadoutChain::Ramp::AnalogValue(456);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.i_bias_vreset_buffer[CapMemBlockOnHemisphere(1)] =
		    CADCReadoutChain::Ramp::AnalogValue(654);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.v_bias_buffer[CapMemBlockOnHemisphere(0)] =
		    CADCReadoutChain::Ramp::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.ramp.v_bias_buffer[CapMemBlockOnHemisphere(1)] =
		    CADCReadoutChain::Ramp::AnalogValue(321);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_causal[CADCChannelColumnOnSynram()].offset =
		    CADCReadoutChain::Channel::Offset(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_correlation =
		    !config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_correlation;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_debug =
		    !config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_debug;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_neuron =
		    !config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_neuron;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_acausal[CADCChannelColumnOnSynram()].offset =
		    CADCReadoutChain::Channel::Offset(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_correlation =
		    !config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_correlation;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_debug =
		    !config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_debug;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_neuron =
		    !config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_neuron;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.sense_delay =
		    draw_ranged_non_default_value<CADCReadoutChain::Correlation::SenseDelay>(
		        config.correlation.sense_delay);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.reset_duration =
		    draw_ranged_non_default_value<CADCReadoutChain::Correlation::ResetDuration>(
		        config.correlation.reset_duration);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.reset_fall_time =
		    draw_ranged_non_default_value<CADCReadoutChain::Correlation::ResetFallTime>(
		        config.correlation.reset_fall_time);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.reset_mode = CADCReadoutChain::Correlation::ResetMode::auto_calibrating;
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.i_bias_ramp[CapMemBlockOnHemisphere(0)] =
		    CADCReadoutChain::Correlation::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.i_bias_ramp[CapMemBlockOnHemisphere(1)] =
		    CADCReadoutChain::Correlation::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.i_bias_store[CapMemBlockOnHemisphere(0)] =
		    CADCReadoutChain::Correlation::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.i_bias_store[CapMemBlockOnHemisphere(1)] =
		    CADCReadoutChain::Correlation::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.i_bias_corout[CapMemBlockOnHemisphere(0)] =
		    CADCReadoutChain::Correlation::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
	{
		CADCReadoutChain config;
		config.correlation.i_bias_corout[CapMemBlockOnHemisphere(1)] =
		    CADCReadoutChain::Correlation::AnalogValue(123);
		EXPECT_NE(config, CADCReadoutChain());
	}
}

TEST(CADCReadoutChain, CerealizeCoverage)
{
	CADCReadoutChain obj1, obj2;

	obj1.ramp.enable = !obj1.ramp.enable;
	obj1.ramp.reset_wait =
	    draw_ranged_non_default_value<CADCReadoutChain::Ramp::ResetWait>(obj1.ramp.reset_wait);
	obj1.ramp.dead_time =
	    draw_ranged_non_default_value<CADCReadoutChain::Ramp::DeadTime>(obj1.ramp.dead_time);
	obj1.ramp.v_offset_assignment[CapMemBlockOnHemisphere(0)] = CapMemBlockOnHemisphere(1);
	obj1.ramp.v_offset_assignment[CapMemBlockOnHemisphere(1)] = CapMemBlockOnHemisphere(0);
	obj1.ramp.v_offset[CapMemBlockOnHemisphere(0)] = CADCReadoutChain::Ramp::AnalogValue(123);
	obj1.ramp.v_offset[CapMemBlockOnHemisphere(1)] = CADCReadoutChain::Ramp::AnalogValue(321);
	obj1.ramp.i_slope[CapMemBlockOnHemisphere(0)] = CADCReadoutChain::Ramp::AnalogValue(234);
	obj1.ramp.i_slope[CapMemBlockOnHemisphere(1)] = CADCReadoutChain::Ramp::AnalogValue(432);
	obj1.ramp.i_bias_comparator[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Ramp::AnalogValue(345);
	obj1.ramp.i_bias_comparator[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Ramp::AnalogValue(543);
	obj1.ramp.i_bias_vreset_buffer[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Ramp::AnalogValue(456);
	obj1.ramp.i_bias_vreset_buffer[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Ramp::AnalogValue(654);
	obj1.ramp.v_bias_buffer[CapMemBlockOnHemisphere(0)] = CADCReadoutChain::Ramp::AnalogValue(123);
	obj1.ramp.v_bias_buffer[CapMemBlockOnHemisphere(1)] = CADCReadoutChain::Ramp::AnalogValue(321);
	obj1.channels_causal[CADCChannelColumnOnSynram()].offset =
	    CADCReadoutChain::Channel::Offset(123);
	obj1.channels_causal[CADCChannelColumnOnSynram()].enable_connect_correlation =
	    !obj1.channels_causal[CADCChannelColumnOnSynram()].enable_connect_correlation;
	obj1.channels_causal[CADCChannelColumnOnSynram()].enable_connect_debug =
	    !obj1.channels_causal[CADCChannelColumnOnSynram()].enable_connect_debug;
	obj1.channels_causal[CADCChannelColumnOnSynram()].enable_connect_neuron =
	    !obj1.channels_causal[CADCChannelColumnOnSynram()].enable_connect_neuron;
	obj1.channels_acausal[CADCChannelColumnOnSynram()].offset =
	    CADCReadoutChain::Channel::Offset(123);
	obj1.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_correlation =
	    !obj1.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_correlation;
	obj1.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_debug =
	    !obj1.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_debug;
	obj1.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_neuron =
	    !obj1.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_neuron;
	obj1.correlation.sense_delay =
	    draw_ranged_non_default_value<CADCReadoutChain::Correlation::SenseDelay>(
	        obj1.correlation.sense_delay);
	obj1.correlation.reset_duration =
	    draw_ranged_non_default_value<CADCReadoutChain::Correlation::ResetDuration>(
	        obj1.correlation.reset_duration);
	obj1.correlation.reset_fall_time =
	    draw_ranged_non_default_value<CADCReadoutChain::Correlation::ResetFallTime>(
	        obj1.correlation.reset_fall_time);
	obj1.correlation.reset_mode = CADCReadoutChain::Correlation::ResetMode::auto_calibrating;
	obj1.correlation.i_bias_ramp[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	obj1.correlation.i_bias_ramp[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	obj1.correlation.i_bias_store[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	obj1.correlation.i_bias_store[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	obj1.correlation.i_bias_corout[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	obj1.correlation.i_bias_corout[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);

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

TEST(CADCReadoutChain, EncodeDecode)
{
	typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
	typedef std::vector<fisch::vx::word_access_type::Omnibus> words_type;

	CADCOnDLS coord;

	CADCReadoutChain config;
	config.ramp.enable = !config.ramp.enable;
	config.ramp.reset_wait =
	    draw_ranged_non_default_value<CADCReadoutChain::Ramp::ResetWait>(config.ramp.reset_wait);
	config.ramp.dead_time =
	    draw_ranged_non_default_value<CADCReadoutChain::Ramp::DeadTime>(config.ramp.dead_time);
	config.ramp.v_offset_assignment[CapMemBlockOnHemisphere(0)] = CapMemBlockOnHemisphere(1);
	config.ramp.v_offset_assignment[CapMemBlockOnHemisphere(1)] = CapMemBlockOnHemisphere(0);
	config.ramp.v_offset[CapMemBlockOnHemisphere(0)] = CADCReadoutChain::Ramp::AnalogValue(123);
	config.ramp.v_offset[CapMemBlockOnHemisphere(1)] = CADCReadoutChain::Ramp::AnalogValue(321);
	config.ramp.i_slope[CapMemBlockOnHemisphere(0)] = CADCReadoutChain::Ramp::AnalogValue(234);
	config.ramp.i_slope[CapMemBlockOnHemisphere(1)] = CADCReadoutChain::Ramp::AnalogValue(432);
	config.ramp.i_bias_comparator[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Ramp::AnalogValue(345);
	config.ramp.i_bias_comparator[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Ramp::AnalogValue(543);
	config.ramp.i_bias_vreset_buffer[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Ramp::AnalogValue(456);
	config.ramp.i_bias_vreset_buffer[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Ramp::AnalogValue(654);
	config.ramp.v_bias_buffer[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Ramp::AnalogValue(123);
	config.ramp.v_bias_buffer[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Ramp::AnalogValue(321);
	config.channels_causal[CADCChannelColumnOnSynram()].offset =
	    CADCReadoutChain::Channel::Offset(123);
	config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_correlation =
	    !config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_correlation;
	config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_debug =
	    !config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_debug;
	config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_neuron =
	    !config.channels_causal[CADCChannelColumnOnSynram()].enable_connect_neuron;
	config.channels_acausal[CADCChannelColumnOnSynram()].offset =
	    CADCReadoutChain::Channel::Offset(123);
	config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_correlation =
	    !config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_correlation;
	config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_debug =
	    !config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_debug;
	config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_neuron =
	    !config.channels_acausal[CADCChannelColumnOnSynram()].enable_connect_neuron;
	config.correlation.sense_delay =
	    draw_ranged_non_default_value<CADCReadoutChain::Correlation::SenseDelay>(
	        config.correlation.sense_delay);
	config.correlation.reset_duration =
	    draw_ranged_non_default_value<CADCReadoutChain::Correlation::ResetDuration>(
	        config.correlation.reset_duration);
	config.correlation.reset_fall_time =
	    draw_ranged_non_default_value<CADCReadoutChain::Correlation::ResetFallTime>(
	        config.correlation.reset_fall_time);
	config.correlation.reset_mode = CADCReadoutChain::Correlation::ResetMode::auto_calibrating;
	config.correlation.i_bias_ramp[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	config.correlation.i_bias_ramp[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	config.correlation.i_bias_store[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	config.correlation.i_bias_store[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	config.correlation.i_bias_corout[CapMemBlockOnHemisphere(0)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);
	config.correlation.i_bias_corout[CapMemBlockOnHemisphere(1)] =
	    CADCReadoutChain::Correlation::AnalogValue(123);

	addresses_type read_addresses;
	addresses_type write_addresses;
	visit_preorder(config, coord, stadls::ReadAddressVisitor<addresses_type>{read_addresses});
	visit_preorder(config, coord, stadls::WriteAddressVisitor<addresses_type>{write_addresses});
	EXPECT_THAT(read_addresses, write_addresses);

	words_type data;
	visit_preorder(config, coord, stadls::EncodeVisitor<words_type>{data});

	CADCReadoutChain config_decoded;
	visit_preorder(config_decoded, coord, stadls::DecodeVisitor<words_type>{std::move(data)});
	EXPECT_EQ(config_decoded, config);
}
