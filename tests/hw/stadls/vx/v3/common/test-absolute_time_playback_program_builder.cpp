#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/event.h"
#include "haldls/vx/v3/fpga.h"
#include "haldls/vx/v3/timer.h"
#include "hate/timer.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/absolute_time_playback_program_builder.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"

#include <algorithm>
#include <memory>
#include <vector>
#include <gtest/gtest.h>
#include <log4cxx/logger.h>

TEST(AbsoluteTimePlaybackProgramBuilder, reference)
{
	auto logger =
	    log4cxx::Logger::getLogger("test.hw.AbsoluteTimePlaybackProgramBuilder.reference");
	auto sequence = stadls::vx::v3::DigitalInit();
	auto [builder, _] = stadls::vx::v3::generate(sequence);
	hate::Timer timer;
	builder.write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer());
	size_t num_spikes = 1000;
	haldls::vx::v3::EventRecordingConfig config;
	config.set_enable_event_recording(false);
	builder.write(halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
	builder.block_until(
	    halco::hicann_dls::vx::v3::BarrierOnFPGA(), haldls::vx::v3::Barrier::omnibus);
	for (size_t i = 0; i < num_spikes; ++i) {
		halco::hicann_dls::vx::v3::SpikeLabel label(0);
		haldls::vx::v3::SpikePack1ToChip spike({label});
		builder.write(halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(), spike);
		builder.block_until(
		    halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer::Value(10 * i));
	}
	config.set_enable_event_recording(true);
	builder.write(halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
	builder.block_until(
	    halco::hicann_dls::vx::v3::BarrierOnFPGA(), haldls::vx::v3::Barrier::omnibus);


	for (size_t i = 0; i < num_spikes; ++i) {
		halco::hicann_dls::vx::v3::SpikeLabel label(0);
		haldls::vx::v3::SpikePack1ToChip spike({label});
		builder.write(halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(), spike);
		builder.block_until(
		    halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer::Value(10000 + 10 * i));
	}
	config.set_enable_event_recording(false);
	builder.write(halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
	builder.block_until(
	    halco::hicann_dls::vx::v3::BarrierOnFPGA(), haldls::vx::v3::Barrier::omnibus);

	for (size_t i = 0; i < num_spikes; ++i) {
		halco::hicann_dls::vx::v3::SpikeLabel label(0);
		haldls::vx::v3::SpikePack1ToChip spike({label});
		builder.write(halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(), spike);
		builder.block_until(
		    halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer::Value(20000 + 10 * i));
	}
	LOG4CXX_TRACE(logger, "Computing time for PPB command queue: " << timer.print() << "\n");
	auto program = builder.done();
	auto connection = hxcomm::vx::get_connection_from_env();
	stadls::vx::v3::run(connection, program);
	auto spikes = program.get_spikes();
	EXPECT_LE(spikes.size(), num_spikes);
	EXPECT_GT(spikes.size(), 0.9 * num_spikes);
}


TEST(AbsoluteTimePlaybackProgramBuilder, general)
{
	auto logger = log4cxx::Logger::getLogger("test.hw.AbsoluteTimePlaybackProgramBuilder.general");
	auto sequence = stadls::vx::v3::DigitalInit();
	auto [builder, _] = stadls::vx::v3::generate(sequence);
	hate::Timer timer;
	auto ATPPB_builder = stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	size_t num_spikes = 1000;
	for (size_t i = 0; i < 3 * num_spikes; i++) {
		halco::hicann_dls::vx::v3::SpikeLabel label(0);
		haldls::vx::v3::SpikePack1ToChip spike({label});
		ATPPB_builder.write(
		    haldls::vx::v3::Timer::Value(10 * i),
		    halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(), spike);
	}
	haldls::vx::v3::EventRecordingConfig config;
	config.set_enable_event_recording(false);
	ATPPB_builder.write(
	    haldls::vx::v3::Timer::Value(0), halco::hicann_dls::vx::EventRecordingConfigOnFPGA(),
	    config);
	config.set_enable_event_recording(true);
	ATPPB_builder.write(
	    haldls::vx::v3::Timer::Value(10 * num_spikes),
	    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
	config.set_enable_event_recording(false);
	ATPPB_builder.write(
	    haldls::vx::v3::Timer::Value(20 * num_spikes),
	    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);

	halco::hicann_dls::vx::v3::SynapseQuadOnDLS synapse_weight_coord;
	haldls::vx::v3::SynapseQuad container;
	auto weights = container.get_weights();
	weights[halco::hicann_dls::vx::v3::EntryOnQuad()] = haldls::vx::v3::SynapseQuad::Weight(3);
	container.set_weights(weights);
	ATPPB_builder.write(
	    haldls::vx::v3::Timer::Value(num_spikes * 10), synapse_weight_coord, container);
	EXPECT_TRUE(ATPPB_builder.is_write_only());
	stadls::vx::AbsoluteTimePlaybackProgramContainerTicket ticket =
	    ATPPB_builder.read(haldls::vx::v3::Timer::Value(num_spikes * 15), synapse_weight_coord);
	EXPECT_FALSE(ATPPB_builder.is_write_only());
	EXPECT_FALSE(ticket.valid());

	auto builder_appendix = ATPPB_builder.done();
	LOG4CXX_TRACE(logger, "Computing time for ATPPB command queue: " << timer.print() << "\n");
	builder.merge_back(builder_appendix);
	EXPECT_FALSE(ticket.valid());
	auto program = builder.done();
	EXPECT_FALSE(ticket.valid());
	auto connection = hxcomm::vx::get_connection_from_env();
	stadls::vx::v3::run(connection, program);
	EXPECT_TRUE(ticket.valid());
	auto spikes = program.get_spikes();
	EXPECT_LE(spikes.size(), num_spikes);
	EXPECT_GE(spikes.size(), 0.9 * num_spikes);
	EXPECT_EQ(ticket.get(), container);
	EXPECT_EQ(ticket.get_coordinate(), synapse_weight_coord);
	LOG4CXX_TRACE(logger, "Timestamp of last synapse response: " << ticket.get_fpga_time() << "\n");
}
