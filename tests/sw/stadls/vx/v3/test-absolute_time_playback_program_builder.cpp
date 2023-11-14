#include "haldls/vx/container.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/event.h"
#include "haldls/vx/v3/timer.h"
#include "hate/timer.h"
#include "stadls/vx/v3/absolute_time_playback_program_builder.h"
#include "stadls/vx/v3/playback_program_builder.h"

#include <fstream>
#include <random>
#include <string>
#include <gtest/gtest.h>
#include <log4cxx/logger.h>

TEST(AbsoluteTimePlaybackProgramBuilder, general)
{
	auto logger = log4cxx::Logger::getLogger("test.AbsoluteTimePlaybackProgramBuilder");
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();

	// testing the empty function
	EXPECT_EQ(builder.empty(), true);

	// testing the << operator
	LOG4CXX_DEBUG(logger, "builder's contents are: \n" << builder << "\n");

	// testing the is_write_only function
	EXPECT_EQ(builder.is_write_only(), true);

	// testing the merge function
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder2 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	halco::hicann_dls::vx::v3::SpikeLabel label2(0);
	haldls::vx::v3::SpikePack1ToChip spike2({label2});
	builder2.write(
	    haldls::vx::v3::Timer::Value(10), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike2);

	builder.merge(builder2);

	LOG4CXX_DEBUG(logger, "builder's contents are now \n" << builder << "\n");
	EXPECT_EQ(builder2.empty(), true);

	// testing the write function
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder3 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	halco::hicann_dls::vx::v3::SpikeLabel label3(0);
	haldls::vx::v3::SpikePack1ToChip spike3({label3});
	builder3.write(
	    haldls::vx::v3::Timer::Value(20), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike3);

	// testing the differential write function
	haldls::vx::SynapseQuad synapse_quad1;
	haldls::vx::SynapseWeightQuad::values_type weights1 = synapse_quad1.get_weights();
	weights1[halco::hicann_dls::vx::v3::EntryOnQuad()] = haldls::vx::v3::SynapseQuad::Weight(20);
	synapse_quad1.set_weights(weights1);

	haldls::vx::SynapseQuad synapse_quad2;
	haldls::vx::SynapseWeightQuad::values_type weights2 = synapse_quad2.get_weights();
	weights2[halco::hicann_dls::vx::v3::EntryOnQuad()] = haldls::vx::v3::SynapseQuad::Weight(40);
	synapse_quad2.set_weights(weights2);

	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder sample_builder =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	sample_builder.write(
	    haldls::vx::v3::Timer::Value(50), halco::hicann_dls::vx::SynapseQuadOnDLS(), synapse_quad1);
	sample_builder.write(
	    haldls::vx::v3::Timer::Value(100), halco::hicann_dls::vx::SynapseQuadOnDLS(), synapse_quad2,
	    synapse_quad1);

	stadls::vx::v3::PlaybackProgramBuilder reference_builder =
	    stadls::vx::v3::PlaybackProgramBuilder();
	reference_builder.write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::Timer());
	reference_builder.block_until(
	    halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer::Value(49));
	reference_builder.write(halco::hicann_dls::vx::v3::SynapseQuadOnDLS(), synapse_quad1);
	reference_builder.block_until(
	    halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer::Value(99));
	reference_builder.write(
	    halco::hicann_dls::vx::v3::SynapseQuadOnDLS(), synapse_quad2, synapse_quad1);

	EXPECT_EQ(sample_builder.done().done(), reference_builder.done());

	// testing the copy function
	builder.copy(builder3);
	EXPECT_EQ(builder3.empty(), false);

	EXPECT_NE(builder.done().done(), builder3.done().done());

	LOG4CXX_DEBUG(logger, "builder's contents are now: \n" << builder << "\n");
	LOG4CXX_DEBUG(logger, "builder3's contents are now: \n" << builder3 << "\n");
	EXPECT_EQ(builder3.empty(), true);

	// testing the ban on accessing the FPGA-Timer
	EXPECT_THROW(
	    builder3.write(
	        haldls::vx::v3::Timer::Value(10), halco::hicann_dls::vx::v3::TimerOnDLS(),
	        haldls::vx::v3::Timer()),
	    std::runtime_error);

	// testing the offset operators + and +=
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder4 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	halco::hicann_dls::vx::v3::SpikeLabel label4(0);
	haldls::vx::v3::SpikePack1ToChip spike4({label4});
	builder4.write(
	    haldls::vx::v3::Timer::Value(0), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike4);

	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder5 =
	    builder4 + haldls::vx::v3::Timer::Value(10);

	builder4 += haldls::vx::v3::Timer::Value(10);

	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder6 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	builder6.write(
	    haldls::vx::v3::Timer::Value(10), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike4);

	auto program4 = builder4.done().done();
	auto program5 = builder5.done().done();
	auto program6 = builder6.done().done();

	EXPECT_EQ(program4, program5);
	EXPECT_EQ(program4, program6);


	// testing the scaling operators * and *=
	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder7 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	halco::hicann_dls::vx::v3::SpikeLabel label7(0);
	haldls::vx::v3::SpikePack1ToChip spike7({label7});
	builder7.write(
	    haldls::vx::v3::Timer::Value(10), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike7);

	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder8 = builder7 * 10.0;

	builder7 *= 10.0;

	stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder builder9 =
	    stadls::vx::v3::AbsoluteTimePlaybackProgramBuilder();
	builder9.write(
	    haldls::vx::v3::Timer::Value(100), halco::hicann_dls::vx::v3::SpikePack1ToChipOnDLS(),
	    spike7);

	auto program7 = builder7.done().done();
	auto program8 = builder8.done().done();
	auto program9 = builder9.done().done();

	EXPECT_EQ(program7, program8);
	EXPECT_EQ(program7, program9);
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
			stadls::vx::v3::PlaybackProgramBuilder builder;
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
		}
	}
}
