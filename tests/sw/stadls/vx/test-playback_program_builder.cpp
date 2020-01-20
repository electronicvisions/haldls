#include <gtest/gtest.h>

#include "stadls/vx/playback_program_builder.h"

#include "haldls/vx/capmem.h"
#include "haldls/vx/padi.h"

using namespace stadls::vx;
using namespace haldls::vx;
using namespace halco::hicann_dls::vx;

TEST(PlaybackProgramBuilder, WriteAddsInstructions)
{
	PlaybackProgramBuilder builder;
	builder.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	auto program_1 = builder.done();

	PlaybackProgramBuilder builder2;
	builder2.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	builder2.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	auto program_2 = builder2.done();

	EXPECT_NE(program_1, program_2);
}

TEST(PlaybackProgramBuilder, EnabledDifferentialWrite)
{
	PlaybackProgramBuilder builder;
	builder.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	auto program_1 = builder.done();

	PlaybackProgramBuilder builder2;
	builder2.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	builder2.write(
	    CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)), CapMemCell(CapMemCell::Value(123)));
	auto program_2 = builder2.done();

	EXPECT_EQ(program_1, program_2);
}

TEST(PlaybackProgramBuilder, NoDifferentialWriteAllowed)
{
	PlaybackProgramBuilder builder;
	EXPECT_THROW(builder.write(PADIEventOnDLS(), PADIEvent(), PADIEvent()), std::logic_error);
}

TEST(PlaybackProgramBuilder, ExecutableRestriction)
{
	PlaybackProgramBuilder builder;
	auto const program_unrestricted = builder.done();
	EXPECT_FALSE(program_unrestricted.get_executable_restriction());

	builder.read(CrossbarNodeOnDLS());
	auto const program_simulation_restricted = builder.done();
	EXPECT_TRUE(program_simulation_restricted.get_executable_restriction());
	EXPECT_EQ(
	    *(program_simulation_restricted.get_executable_restriction()), ExecutorBackend::simulation);

	{
		PlaybackProgramBuilder builder(ExecutorBackend::hardware);
		auto const program = builder.done();
		EXPECT_TRUE(program.get_executable_restriction());
		EXPECT_EQ(*(program.get_executable_restriction()), ExecutorBackend::hardware);
	}

	{
		PlaybackProgramBuilder builder(ExecutorBackend::simulation);
		auto const program = builder.done();
		EXPECT_TRUE(program.get_executable_restriction());
		EXPECT_EQ(*(program.get_executable_restriction()), ExecutorBackend::simulation);
	}
}
