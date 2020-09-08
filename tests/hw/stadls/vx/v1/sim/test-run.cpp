#include <gtest/gtest.h>
#include "halco/hicann-dls/vx/v1/barrier.h"
#include "halco/hicann-dls/vx/v1/neuron.h"
#include "halco/hicann-dls/vx/v1/routing_crossbar.h"
#include "haldls/vx/v1/barrier.h"
#include "stadls/vx/v1/init_generator.h"
#include "stadls/vx/v1/playback_program.h"
#include "stadls/vx/v1/playback_program_builder.h"
#include "stadls/vx/v1/run.h"

#include "connection.h"

using namespace stadls::vx::v1;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	auto sim_builder = generate(DigitalInit()).builder;
	sim_builder.read(halco::hicann_dls::vx::v1::CrossbarNodeOnDLS());
	sim_builder.block_until(
	    halco::hicann_dls::vx::v1::BarrierOnFPGA(), haldls::vx::Barrier::omnibus);
	auto program_simulation_restricted = sim_builder.done();

	auto connection = generate_test_connection();

	EXPECT_NO_THROW(run(connection, program_unrestricted));
	EXPECT_NO_THROW(run(connection, program_simulation_restricted));

	auto hw_builder = generate(DigitalInit()).builder;
	hw_builder.read(halco::hicann_dls::vx::v1::NeuronConfigOnDLS());
	hw_builder.block_until(
	    halco::hicann_dls::vx::v1::BarrierOnFPGA(), haldls::vx::Barrier::omnibus);
	auto program_hw_restricted = hw_builder.done();
	EXPECT_THROW(run(connection, program_hw_restricted), std::runtime_error);
}
