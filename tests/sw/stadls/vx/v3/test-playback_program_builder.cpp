#include <gtest/gtest.h>

#include "hate/variant.h"
#include "hxcomm/vx/target.h"
#include "stadls/vx/v3/dumper.h"

#include "cereal/types/haldls/cereal.h"
#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"

#include "stadls/vx/v3/playback_program_builder.h"

#include "cereal/types/halco/hicann-dls/vx/v3/coordinate.h"
#include "fisch/vx/playback_program_builder.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/arq.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/capmem.h"
#include "haldls/vx/v3/padi.h"
#include "haldls/vx/v3/timer.h"

using namespace stadls::vx::v3;
using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx::v3;

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

TEST(PlaybackProgramBuilderDumper, Dumpstuff)
{
	PlaybackProgramBuilderDumper builder;
	Dumper::done_type cocos_written;
	cocos_written.values.push_back(
	    std::make_pair(std::make_unique<PPUMemoryWordOnDLS>(), std::make_unique<PPUMemoryWord>()));
	cocos_written.values.push_back(std::make_pair(
	    std::make_unique<CapMemCellOnDLS>(), std::make_unique<CapMemCell>(CapMemCell::Value(456))));
	cocos_written.values.push_back(
	    std::make_pair(std::make_unique<TimerOnDLS>(), std::make_unique<Timer::Value>(1234)));
	cocos_written.values.push_back(std::make_pair(
	    std::make_unique<CapMemCellOnDLS>(), std::make_unique<CapMemCell>(CapMemCell::Value(456))));

	for (auto& item : cocos_written.values) {
		if (auto const config_ptr = dynamic_cast<haldls::vx::Container const*>(item.second.get())) {
			builder.write(*item.first, *config_ptr);
		} else if (
		    auto const config_ptr =
		        dynamic_cast<haldls::vx::BlockUntil const*>(item.second.get())) {
			builder.block_until(*item.first, *config_ptr);
		} else {
			throw std::logic_error("Unexpected encodable type.");
		}
	}

	EXPECT_ANY_THROW(builder.read(CapMemCellOnDLS()));

	PlaybackProgramBuilderDumper builder_copy;
	builder_copy.copy_back(builder);
	auto cocos_saved = builder.done();
	builder.copy_back(builder_copy);
	EXPECT_EQ(cocos_saved.values.size(), 4);

	EXPECT_EQ(cocos_written, cocos_saved);
	EXPECT_EQ(cocos_written, convert_to_dumper(cocos_written).done());

	{
		std::ostringstream ostream;
		{
			cereal::JSONOutputArchive oa(ostream);
			oa(builder);
		}

		PlaybackProgramBuilderDumper builder_copy;
		std::istringstream istream(ostream.str());
		{
			cereal::JSONInputArchive ia(istream);
			ia(builder_copy);
		}
		ASSERT_EQ(builder.done(), builder_copy.done());
	}

	/* repack into a real builder */
	PlaybackProgramBuilder real_builder;
	for (auto const& item : cocos_saved.values) {
		if (auto const config_ptr = dynamic_cast<haldls::vx::Container const*>(item.second.get())) {
			real_builder.write(*item.first, *config_ptr);
		} else if (
		    auto const config_ptr =
		        dynamic_cast<haldls::vx::BlockUntil const*>(item.second.get())) {
			real_builder.block_until(*item.first, *config_ptr);
		} else {
			throw std::logic_error("Unexpected encodable type.");
		}
	}

	auto program = real_builder.done();
	EXPECT_EQ(program, convert_to_builder(cocos_saved).done());

	PlaybackProgramBuilderDumper builder_copy_copy;
	builder_copy_copy.copy_back(builder_copy);
	EXPECT_EQ(program, convert_to_builder(builder_copy).done());
	EXPECT_EQ(program, convert_to_builder(std::move(builder_copy_copy)).done());
}

TEST(PlaybackProgramBuilder, PolymorphicWrite)
{
	PlaybackProgramBuilder builder;
	CapMemCellOnDLS const raw_coord;
	// explicit cast due to to be removed overload
	haldls::vx::Container::Coordinate const& coord = raw_coord;
	builder.write(coord, CapMemCell());
	auto program_1 = builder.done();
}

TEST(PlaybackProgramBuilder, PolymorphicRead)
{
	PlaybackProgramBuilder builder;
	HicannARQStatusOnFPGA const raw_coord;
	// explicit cast due to to be removed overload
	haldls::vx::Container::Coordinate const& coord = raw_coord;
	auto ticket = builder.read(coord);
	auto program_1 = builder.done();
}

TEST(PlaybackProgramBuilderDumper, remove_block_until)
{
	PlaybackProgramBuilderDumper builder;
	builder.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	builder.block_until(TimerOnDLS(), Timer::Value(1234));

	auto done = builder.done();
	done.remove_block_until();

	builder.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	auto const expectation = builder.done();
	EXPECT_EQ(done, expectation);
}

TEST(PlaybackProgramBuilderDumper, squash)
{
	PlaybackProgramBuilderDumper builder;
	builder.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123)));
	builder.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(456)));
	builder.block_until(TimerOnDLS(), Timer::Value(1234));
	builder.write(DACChannelOnBoard(), DACChannel());

	auto done = builder.done();
	done.squash();

	builder.write(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(456)));
	builder.block_until(TimerOnDLS(), Timer::Value(1234));
	builder.write(DACChannelOnBoard(), DACChannel());
	auto const expectation = builder.done();
	EXPECT_EQ(done, expectation);
}
