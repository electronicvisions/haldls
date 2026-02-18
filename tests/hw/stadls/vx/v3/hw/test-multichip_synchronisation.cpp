#include "fisch/vx/constants.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"

#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

#include "multichip_test_helper.h"

#include <cmath>
#include <stdlib.h>

TEST(Multichip, SyncEmptyRun)
{
	using namespace haldls::vx::v3;

	auto connection = hxcomm::vx::get_connection_from_env(4);
	if (!is_jboa_multichip_setup(connection)) {
		GTEST_SKIP();
	}
	const size_t connection_size = std::visit([](auto& conn) { return conn.size(); }, connection);


	stadls::vx::v3::PlaybackProgramBuilder builder;
	// Increased timout so that barriers do not time out
	auto timeout = InstructionTimeoutConfig();
	timeout.set_value(Timer::Value(int(10 * fisch::vx::fpga_clock_cycles_per_s)));
	builder.write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);

	// Sync Barrier for all fpgas.
	builder.block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);

	auto program = builder.done();

	std::vector<stadls::vx::v3::PlaybackProgram> programs;
	std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;
	for (size_t i = 0; i < connection_size; i++) {
		programs.push_back(program);
	}
	for (size_t i = 0; i < connection_size; i++) {
		programs_wrapped.push_back(programs.at(i));
	}

	stadls::vx::v3::run(connection, programs_wrapped);
}

TEST(Multichip, SyncTimeout)
{
	using namespace haldls::vx::v3;

	auto connection = hxcomm::vx::get_connection_from_env();
	if (!is_jboa_multichip_setup(connection)) {
		GTEST_SKIP();
	}
	const size_t connection_size = std::visit([](auto& conn) { return conn.size(); }, connection);

	auto timeout_on_different_waits = [&connection, &connection_size](
	                                      size_t timeout_seconds, bool expect_throw) {
		auto timeout = InstructionTimeoutConfig();
		timeout.set_value(Timer::Value(int(timeout_seconds * fisch::vx::fpga_clock_cycles_per_s)));

		std::vector<stadls::vx::v3::PlaybackProgram> programs;
		for (size_t i = 0; i < connection_size; i++) {
			stadls::vx::v3::PlaybackProgramBuilder builder;
			builder.write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);

			builder.block_until(halco::hicann_dls::vx::v3::BarrierOnFPGA(), Barrier::multi_fpga);

			builder.write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer());
			builder.block_until(
			    halco::hicann_dls::vx::v3::TimerOnDLS(),
			    haldls::vx::v3::Timer::Value(
			        2 * fisch::vx::fpga_clock_cycles_per_s *
			        (i + 1))); // Also wait for i=0 so that the refactory period between the two
			                   // sync signals passes.

			// This instruction might time out due to other FPGA(s) waiting too long(previous call).
			builder.block_until(halco::hicann_dls::vx::v3::BarrierOnFPGA(), Barrier::multi_fpga);

			programs.push_back(builder.done());
		}

		std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;
		for (size_t i = 0; i < connection_size; i++) {
			programs_wrapped.push_back(programs.at(i));
		}
		if (expect_throw) {
			EXPECT_THROW(stadls::vx::v3::run(connection, programs_wrapped), std::runtime_error);
		} else {
			EXPECT_NO_THROW(stadls::vx::v3::run(connection, programs_wrapped));
		}
	};
	timeout_on_different_waits(1, connection_size >= 2);
	timeout_on_different_waits(3, connection_size >= 3);
	timeout_on_different_waits(5, connection_size >= 4);
	timeout_on_different_waits(15, connection_size >= 9);
}

// TO-DO: Move into one scope once a new connection is not needed to recover from a timed out
// barrier.
TEST(Multichip, SyncOneMissing)
{
	using namespace haldls::vx::v3;

	stadls::vx::v3::PlaybackProgramBuilder builder;

	{
		auto connection = hxcomm::vx::get_connection_from_env();
		if (!is_jboa_multichip_of_min_size(connection, 2)) {
			GTEST_SKIP();
		}
		const size_t connection_size =
		    std::visit([](auto& conn) { return conn.size(); }, connection);


		auto timeout = InstructionTimeoutConfig();
		timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));
		builder.write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);

		auto program_without_sync = builder.done();

		builder.block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
		auto program_with_sync = builder.done();

		std::vector<stadls::vx::v3::PlaybackProgram> programs;
		std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;

		// One FPGA does not send sync barrier. -> Instruction timeout reached.
		programs.push_back(program_without_sync);
		for (size_t i = 1; i < connection_size; i++) {
			programs.push_back(program_with_sync);
		}
		for (size_t i = 0; i < connection_size; i++) {
			programs_wrapped.push_back(programs.at(i));
		}
		EXPECT_THROW(stadls::vx::v3::run(connection, programs_wrapped), std::runtime_error);
	}


	{
		auto connection = hxcomm::vx::get_connection_from_env();
		if (!is_jboa_multichip_of_min_size(connection, 2)) {
			GTEST_SKIP();
		}
		const size_t connection_size =
		    std::visit([](auto& conn) { return conn.size(); }, connection);

		auto timeout = InstructionTimeoutConfig();
		timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));
		builder.write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);

		auto program_without_sync = builder.done();

		builder.block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
		auto program_with_sync = builder.done();

		std::vector<stadls::vx::v3::PlaybackProgram> programs;
		std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;

		// Only one FPGA does send sync barrier. -> Recovery from unsynced state.
		programs.push_back(program_with_sync);
		for (size_t i = 1; i < connection_size; i++) {
			programs.push_back(program_without_sync);
		}
		for (size_t i = 0; i < connection_size; i++) {
			programs_wrapped.push_back(programs.at(i));
		}
		EXPECT_NO_THROW(stadls::vx::v3::run(connection, programs_wrapped));
	}
}

// TO-DO: Move into one scope once a new connection is not needed to recover from a timed out
// barrier.
TEST(Multichip, SyncOnlyOneSend)
{
	using namespace haldls::vx::v3;

	stadls::vx::v3::PlaybackProgramBuilder builder;

	{
		auto connection = hxcomm::vx::get_connection_from_env();
		if (!is_jboa_multichip_of_min_size(connection, 3)) {
			GTEST_SKIP();
		}
		const size_t connection_size =
		    std::visit([](auto& conn) { return conn.size(); }, connection);


		auto timeout = InstructionTimeoutConfig();
		timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));
		builder.write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);

		auto program_without_sync = builder.done();

		builder.block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
		auto program_with_sync = builder.done();

		std::vector<stadls::vx::v3::PlaybackProgram> programs;
		std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;

		// One FPGA does not send sync barrier. -> Instruction timeout reached.
		programs.push_back(program_with_sync);
		for (size_t i = 1; i < connection_size; i++) {
			programs.push_back(program_without_sync);
		}
		for (size_t i = 0; i < connection_size; i++) {
			programs_wrapped.push_back(programs.at(i));
		}
		EXPECT_THROW(stadls::vx::v3::run(connection, programs_wrapped), std::runtime_error);
	}


	{
		auto connection = hxcomm::vx::get_connection_from_env();
		if (!is_jboa_multichip_of_min_size(connection, 3)) {
			GTEST_SKIP();
		}
		const size_t connection_size =
		    std::visit([](auto& conn) { return conn.size(); }, connection);

		auto timeout = InstructionTimeoutConfig();
		timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));
		builder.write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);

		auto program_without_sync = builder.done();

		builder.block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
		auto program_with_sync = builder.done();

		std::vector<stadls::vx::v3::PlaybackProgram> programs;
		std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;

		// Only one FPGA does send sync barrier. -> Recovery from unsynced state.
		programs.push_back(program_without_sync);
		for (size_t i = 1; i < connection_size; i++) {
			programs.push_back(program_with_sync);
		}
		for (size_t i = 0; i < connection_size; i++) {
			programs_wrapped.push_back(programs.at(i));
		}
		EXPECT_NO_THROW(stadls::vx::v3::run(connection, programs_wrapped));
	}
}