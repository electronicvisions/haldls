#include <gtest/gtest.h>
#include "halco/hicann-dls/vx/barrier.h"
#include "halco/hicann-dls/vx/neuron.h"
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "haldls/vx/barrier.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/run.h"

#include "connection.h"

using namespace stadls::vx;

TEST(Run, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto program_unrestricted = builder.done();

	auto sim_builder = generate(DigitalInit()).builder;
	sim_builder.read(halco::hicann_dls::vx::CrossbarNodeOnDLS());
	sim_builder.block_until(halco::hicann_dls::vx::BarrierOnFPGA(), haldls::vx::Barrier::omnibus);
	auto program_simulation_restricted = sim_builder.done();

	auto connection = generate_test_connection();

	EXPECT_NO_THROW(run(connection, program_unrestricted));
	EXPECT_NO_THROW(run(connection, program_simulation_restricted));

	auto hw_builder = generate(DigitalInit()).builder;
	hw_builder.read(halco::hicann_dls::vx::NeuronConfigOnDLS());
	hw_builder.block_until(halco::hicann_dls::vx::BarrierOnFPGA(), haldls::vx::Barrier::omnibus);
	auto program_hw_restricted = hw_builder.done();
	EXPECT_THROW(run(connection, program_hw_restricted), std::runtime_error);
}
