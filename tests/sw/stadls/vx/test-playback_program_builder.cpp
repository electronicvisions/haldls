#include <gtest/gtest.h>

#include "hxcomm/vx/target.h"
#include "stadls/vx/playback_program_builder.h"

#include "fisch/vx/playback_program_builder.h"
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

TEST(PlaybackProgramBuilder, UnsupportedTargets)
{
	PlaybackProgramBuilder builder;
	auto const program_unrestricted = builder.done();
	EXPECT_TRUE(program_unrestricted.get_unsupported_targets().empty());

	builder.read(CrossbarNodeOnDLS());
	auto const program_simulation_restricted = builder.done();
	EXPECT_FALSE(program_simulation_restricted.get_unsupported_targets().empty());
	EXPECT_EQ(
	    program_simulation_restricted.get_unsupported_targets(),
	    std::unordered_set{hxcomm::vx::Target::hardware});
}

TEST(PlaybackProgramBuilder, MergeBackAPI)
{
	PlaybackProgramBuilder builder;
	{
		PlaybackProgramBuilder other;
		builder.merge_back(other);
	}
	{
		// rvalue overload
		builder.merge_back(PlaybackProgramBuilder());
	}
	{
		fisch::vx::PlaybackProgramBuilder other;
		builder.merge_back(other);
	}
	{
		// rvalue overload
		builder.merge_back(fisch::vx::PlaybackProgramBuilder());
	}
}
