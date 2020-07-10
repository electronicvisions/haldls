#include <gtest/gtest.h>

#include "hate/variant.h"
#include "hxcomm/vx/target.h"
#include "stadls/vx/dumper.h"
#include "stadls/vx/playback_program_builder.h"

#include "fisch/vx/playback_program_builder.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/padi.h"
#include "haldls/vx/timer.h"

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

TEST(PlaybackProgramBuilderDumper, Dumpstuff)
{
	PlaybackProgramBuilderDumper builder;
	Dumper::done_type cocos_written;
	cocos_written.push_back(std::make_pair(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(123))));
	cocos_written.push_back(std::make_pair(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(456))));
	cocos_written.push_back(std::make_pair(TimerOnDLS(), Timer::Value(1234)));
	cocos_written.push_back(std::make_pair(CapMemCellOnDLS(), CapMemCell(CapMemCell::Value(456))));

	for (auto& item : cocos_written) {
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

	auto cocos_saved = builder.done();
	EXPECT_EQ(cocos_saved.size(), 4);

	EXPECT_EQ(cocos_written, cocos_saved);

	/* repack into a real builder */
	PlaybackProgramBuilder real_builder;
	for (auto& item : cocos_saved) {
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
}
