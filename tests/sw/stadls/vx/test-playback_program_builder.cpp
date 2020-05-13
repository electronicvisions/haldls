#include <gtest/gtest.h>

#include "hate/variant.h"
#include "hxcomm/vx/target.h"
#include "stadls/vx/v1/dumper.h"

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"
#include "lola/vx/cerealization.h"

#include "stadls/vx/v1/playback_program_builder.h"

#include "fisch/vx/playback_program_builder.h"
#include "haldls/vx/v1/barrier.h"
#include "haldls/vx/v1/capmem.h"
#include "haldls/vx/v1/padi.h"
#include "haldls/vx/v1/timer.h"

using namespace stadls::vx::v1;
using namespace haldls::vx::v1;
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

TEST(PlaybackProgramBuilderDumper, Dumpstuff)
{
	PlaybackProgramBuilderDumper builder;
	Dumper::done_type cocos_written;
	cocos_written.values.push_back(
	    std::make_pair(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123))));
	cocos_written.values.push_back(
	    std::make_pair(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(456))));
	cocos_written.values.push_back(std::make_pair(TimerOnDLS(), Timer::Value(1234)));
	cocos_written.values.push_back(
	    std::make_pair(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(456))));

	for (auto& item : cocos_written.values) {
		// clang-format off
		std::visit(
			hate::overloaded {
				[&](auto const& coco_pair) {
					builder.write(std::get<0>(coco_pair), std::get<1>(coco_pair));
				},
				[&](std::pair<typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value> const& coco_pair) {
					builder.block_until(std::get<0>(coco_pair), std::get<1>(coco_pair));
				},
				[&](std::pair<halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier> const& coco_pair) {
					builder.block_until(std::get<0>(coco_pair), std::get<1>(coco_pair));
				}
			}, item
		);
		// clang-format on
	}

	EXPECT_ANY_THROW(builder.read(CapMemCellOnDLS()));

	PlaybackProgramBuilderDumper builder_copy;
	builder_copy.copy_back(builder);
	auto cocos_saved = builder.done();
	EXPECT_EQ(cocos_saved.values.size(), 4);

	EXPECT_EQ(cocos_written, cocos_saved);

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
	for (auto& item : cocos_saved.values) {
		// clang-format off
		std::visit(
			hate::overloaded {
				[&](auto const& coco_pair) {
					real_builder.write(std::get<0>(coco_pair), std::get<1>(coco_pair));
				},
				[&](std::pair<typename haldls::vx::Timer::coordinate_type, haldls::vx::Timer::Value> const& coco_pair) {
					real_builder.block_until(std::get<0>(coco_pair), std::get<1>(coco_pair));
				},
				[&](std::pair<halco::hicann_dls::vx::BarrierOnFPGA, haldls::vx::Barrier> const& coco_pair) {
					real_builder.block_until(std::get<0>(coco_pair), std::get<1>(coco_pair));
				}
			}, item
		);
		// clang-format on
	}

	auto program = real_builder.done();
	EXPECT_EQ(program, convert_to_builder(cocos_saved).done());

	PlaybackProgramBuilderDumper builder_copy_copy;
	builder_copy_copy.copy_back(builder_copy);
	EXPECT_EQ(program, convert_to_builder(builder_copy).done());
	EXPECT_EQ(program, convert_to_builder(std::move(builder_copy_copy)).done());
}
