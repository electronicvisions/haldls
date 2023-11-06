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
#include <fstream>
#include <memory>
#include <random>
#include <string>
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


TEST(AbsoluteTimePlaybackProgramBuilder, sweep)
{
	// three times this number will be the largest amount of spikes written to a single builder
	// queue
	const size_t max_spikes = 1000;

	// the number of spikes will be multiplied by this factor each time, the number of spikes
	// changes
	const size_t factor = 10;

	// how often you measure in a row for a single number of spikes
	const size_t iterations = 10;


	// PPB:
	// document the measurement results in a format, that can be read in with numpy.loadtxt
	const size_t column_width =
	    (std::to_string(max_spikes).length() + 1 > 7) ? std::to_string(max_spikes).length() + 1 : 7;
	std::ofstream datastream_PPB("PerformanceSweep_PPB.txt");
	datastream_PPB
	    << "Performance measurements (time in ms) of building a PPB with N Spike Events:\n\n"
	    << "N:";
	for (size_t m = 2; m < column_width; m++)
		datastream_PPB << " ";
	for (size_t k = 0; k < iterations; k++) {
		std::string column_label = "t" + std::to_string(k) + ":";
		datastream_PPB << column_label;
		for (size_t m = column_label.length(); m < column_width; m++)
			datastream_PPB << " ";
	}

	// actual test:
	auto loggerPPB =
	    log4cxx::Logger::getLogger("test.hw.AbsoluteTimePlaybackProgramBuilder.PPBSweep");
	for (size_t j = 1; j <= max_spikes; j *= factor) {
		datastream_PPB << "\n" << j * 3;
		for (size_t m = std::to_string(j).length(); m < column_width; m++)
			datastream_PPB << " ";

		for (size_t k = 0; k < iterations; k++) {
			auto sequence = stadls::vx::v3::DigitalInit();
			auto [builder, _] = stadls::vx::v3::generate(sequence);
			hate::Timer timer;
			builder.write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer());
			size_t num_spikes = j;
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
				    halco::hicann_dls::vx::v3::TimerOnDLS(),
				    haldls::vx::v3::Timer::Value(10 * num_spikes + 10 * i));
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
				    halco::hicann_dls::vx::v3::TimerOnDLS(),
				    haldls::vx::v3::Timer::Value(20 * num_spikes + 10 * i));
			}
			std::string computing_time =
			    std::to_string(static_cast<float>(timer.get_us()) / 1000).substr(0, 6);
			datastream_PPB << computing_time;
			for (size_t m = computing_time.length(); m < column_width; m++)
				datastream_PPB << " ";
			LOG4CXX_TRACE(
			    loggerPPB, "Computing time " << k << " for PPB command queue with " << num_spikes
			                                 << " spikes: " << timer.print() << "\n");
			auto program = builder.done();
			auto connection = hxcomm::vx::get_connection_from_env();
			stadls::vx::v3::run(connection, program);
		}
	}


	// Ordered ATPPB:
	// document the measurement results in a format, that can be read in with numpy.loadtxt
	std::ofstream datastream_ATPPB_ordered("PerformanceSweep_ATPPB_ordered.txt");
	datastream_ATPPB_ordered << "Performance measurements (time in ms) of building an ATPPB with N "
	                            "ordered Spike Events:\n\n"
	                         << "N:";
	for (size_t m = 2; m < column_width; m++)
		datastream_ATPPB_ordered << " ";
	for (size_t k = 0; k < iterations; k++) {
		std::string column_label = "t" + std::to_string(k) + ":";
		datastream_ATPPB_ordered << column_label;
		for (size_t m = column_label.length(); m < column_width; m++)
			datastream_ATPPB_ordered << " ";
	}

	// actual test:
	auto loggerOrdered =
	    log4cxx::Logger::getLogger("test.hw.AbsoluteTimePlaybackProgramBuilder.OrderedSweep");
	for (size_t j = 1; j <= max_spikes; j *= factor) {
		datastream_ATPPB_ordered << "\n" << j * 3;
		for (size_t m = std::to_string(j).length(); m < column_width; m++)
			datastream_ATPPB_ordered << " ";

		for (size_t k = 0; k < iterations; k++) {
			auto sequence = stadls::vx::v3::DigitalInit();
			auto [builder, _] = stadls::vx::v3::generate(sequence);
			hate::Timer timer;
			auto ATPPB_builder = stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
			size_t num_spikes = j;
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
			    haldls::vx::v3::Timer::Value(0),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
			config.set_enable_event_recording(true);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(10 * num_spikes),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
			config.set_enable_event_recording(false);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(20 * num_spikes),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);

			auto builder_appendix = ATPPB_builder.done();

			std::string computing_time =
			    std::to_string(static_cast<float>(timer.get_us()) / 1000).substr(0, 6);
			datastream_ATPPB_ordered << computing_time;
			for (size_t m = computing_time.length(); m < column_width; m++)
				datastream_ATPPB_ordered << " ";
			LOG4CXX_TRACE(
			    loggerOrdered, "Computing time " << k << " for ordered ATPPB command queue with "
			                                     << num_spikes << " spikes: " << timer.print()
			                                     << "\n");
			builder.merge_back(builder_appendix);
			auto program = builder.done();
			auto connection = hxcomm::vx::get_connection_from_env();
			stadls::vx::v3::run(connection, program);
		}
	}

	// Random ATPPB:
	// document the measurement results in a format, that can be read in with numpy.loadtxt
	std::ofstream datastream_ATPPB_random("PerformanceSweep_ATPPB_random.txt");
	datastream_ATPPB_random << "Performance measurements (time in ms) of building an ATPPB with N "
	                           "random Spike Events:\n\n"
	                        << "N:";
	for (size_t m = 2; m < column_width; m++)
		datastream_ATPPB_random << " ";
	for (size_t k = 0; k < iterations; k++) {
		std::string column_label = "t" + std::to_string(k) + ":";
		datastream_ATPPB_random << column_label;
		for (size_t m = column_label.length(); m < column_width; m++)
			datastream_ATPPB_random << " ";
	}

	// actual test:
	auto loggerRandom =
	    log4cxx::Logger::getLogger("test.hw.AbsoluteTimePlaybackProgramBuilder.RandomSweep");
	for (size_t j = 1; j <= max_spikes; j *= factor) {
		datastream_ATPPB_random << "\n" << j * 3;
		for (size_t m = std::to_string(j).length(); m < column_width; m++)
			datastream_ATPPB_random << " ";

		for (size_t k = 0; k < iterations; k++) {
			auto sequence = stadls::vx::v3::DigitalInit();
			auto [builder, _] = stadls::vx::v3::generate(sequence);
			hate::Timer timer;
			auto ATPPB_builder = stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
			size_t num_spikes = j;
			std::mt19937 rng;
			rng.seed(98231206);
			std::uniform_int_distribution<uint32_t> randomtime(0, 30 * num_spikes - 1);
			for (size_t i = 0; i < 3 * num_spikes; i++) {
				halco::hicann_dls::vx::v3::SpikeLabel label(0);
				haldls::vx::v3::SpikePack1ToChip spike({label});
				ATPPB_builder.write(
				    haldls::vx::v3::Timer::Value(randomtime(rng)),
				    halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(), spike);
			}
			haldls::vx::v3::EventRecordingConfig config;
			config.set_enable_event_recording(false);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(0),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
			config.set_enable_event_recording(true);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(10 * num_spikes),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
			config.set_enable_event_recording(false);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(20 * num_spikes),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);

			auto builder_appendix = ATPPB_builder.done();

			std::string computing_time =
			    std::to_string(static_cast<float>(timer.get_us()) / 1000).substr(0, 6);
			datastream_ATPPB_random << computing_time;
			for (size_t m = computing_time.length(); m < column_width; m++)
				datastream_ATPPB_random << " ";

			LOG4CXX_TRACE(
			    loggerRandom, "Computing time " << k << " for random ATPPB command queue with "
			                                    << num_spikes << " spikes: " << timer.print()
			                                    << "\n");
			builder.merge_back(builder_appendix);
			auto program = builder.done();
			auto connection = hxcomm::vx::get_connection_from_env();
			stadls::vx::v3::run(connection, program);
		}
	}

	// Reverse ATPPB:
	// document the measurement results in a format, that can be read in with numpy.loadtxt
	std::ofstream datastream_ATPPB_reverse("PerformanceSweep_ATPPB_reverse.txt");
	datastream_ATPPB_reverse << "Performance measurements (time in ms) of building an ATPPB with N "
	                            "reverse Spike Events:\n\n"
	                         << "N:";
	for (size_t m = 2; m < column_width; m++)
		datastream_ATPPB_reverse << " ";
	for (size_t k = 0; k < iterations; k++) {
		std::string column_label = "t" + std::to_string(k) + ":";
		datastream_ATPPB_reverse << column_label;
		for (size_t m = column_label.length(); m < column_width; m++)
			datastream_ATPPB_reverse << " ";
	}

	// actual test:
	auto loggerReverse =
	    log4cxx::Logger::getLogger("test.hw.AbsoluteTimePlaybackProgramBuilder.ReverseSweep");
	for (size_t j = 1; j <= max_spikes; j *= factor) {
		datastream_ATPPB_reverse << "\n" << j * 3;
		for (size_t m = std::to_string(j).length(); m < column_width; m++)
			datastream_ATPPB_reverse << " ";

		for (size_t k = 0; k < iterations; k++) {
			auto sequence = stadls::vx::v3::DigitalInit();
			auto [builder, _] = stadls::vx::v3::generate(sequence);
			hate::Timer timer;
			auto ATPPB_builder = stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
			size_t num_spikes = j;
			for (size_t i = 3 * num_spikes; i > 0; i--) {
				halco::hicann_dls::vx::v3::SpikeLabel label(0);
				haldls::vx::v3::SpikePack1ToChip spike({label});
				ATPPB_builder.write(
				    haldls::vx::v3::Timer::Value(10 * i),
				    halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(), spike);
			}
			haldls::vx::v3::EventRecordingConfig config;
			config.set_enable_event_recording(false);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(0),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
			config.set_enable_event_recording(true);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(10 * num_spikes),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);
			config.set_enable_event_recording(false);
			ATPPB_builder.write(
			    haldls::vx::v3::Timer::Value(20 * num_spikes),
			    halco::hicann_dls::vx::EventRecordingConfigOnFPGA(), config);

			auto builder_appendix = ATPPB_builder.done();

			std::string computing_time =
			    std::to_string(static_cast<float>(timer.get_us()) / 1000).substr(0, 6);
			datastream_ATPPB_reverse << computing_time;
			for (size_t m = computing_time.length(); m < column_width; m++)
				datastream_ATPPB_reverse << " ";

			LOG4CXX_TRACE(
			    loggerReverse, "Computing time " << k << " for reverse ATPPB command queue with "
			                                     << num_spikes << " spikes: " << timer.print()
			                                     << "\n");
			builder.merge_back(builder_appendix);
			auto program = builder.done();
			auto connection = hxcomm::vx::get_connection_from_env();
			stadls::vx::v3::run(connection, program);
		}
	}
}
