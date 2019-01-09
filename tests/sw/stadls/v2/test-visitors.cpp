#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <set>

#include <boost/type_index.hpp>

#include "stadls/visitors.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"

namespace {

struct TypeNameVisitor {
	std::set<std::string> names;

	template <typename CoordinateT, typename ContainerT>
	void operator()(CoordinateT const&, ContainerT const&)
	{
		auto type_index = boost::typeindex::type_id<ContainerT>();
		names.insert(type_index.pretty_name());
	}
};

} // namespace

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(Visitors, Board) {
	TypeNameVisitor visitor;
	Unique const coord;
	Board const config;

	visit_preorder(config, coord, visitor);

	EXPECT_THAT(
		visitor.names,
		std::set<std::string>({
			"haldls::v2::Board",
			"haldls::v2::DAC",
			"haldls::v2::FlyspiConfig",
			"haldls::v2::FlyspiException",
			"haldls::v2::SpikeRouter"}));
}

TEST(Visitors, CapMem) {
	TypeNameVisitor visitor;
	CapMemOnDLS const coord;
	CapMem const config;

	visit_preorder(config, coord, visitor);

	EXPECT_THAT(
		visitor.names,
		std::set<std::string>({
			"haldls::v2::CapMem",
			"haldls::v2::CapMemCell"}));
}

TEST(Visitors, Chip) {
	TypeNameVisitor visitor;
	Unique const coord;
	Chip const config;

	visit_preorder(config, coord, visitor);

	EXPECT_THAT(
		visitor.names,
		std::set<std::string>({
			"haldls::v2::AcausalCorrelationBlock",
			"haldls::v2::CapMem",
			"haldls::v2::CapMemCell",
			"haldls::v2::CapMemConfig",
			"haldls::v2::CausalCorrelationBlock",
			"haldls::v2::Chip",
			"haldls::v2::ColumnCorrelationBlock",
			"haldls::v2::ColumnCurrentBlock",
			"haldls::v2::CommonNeuronConfig",
			"haldls::v2::CommonSynramConfig",
			"haldls::v2::CorrelationConfig",
			"haldls::v2::NeuronDigitalConfig",
			"haldls::v2::PPUControlRegister",
			"haldls::v2::PPUMemory",
			"haldls::v2::PPUMemoryWord",
			"haldls::v2::PPUStatusRegister",
			"haldls::v2::RateCounter",
			"haldls::v2::RateCounterConfig",
			"haldls::v2::RateCounterEntry",
			"haldls::v2::SynapseBlock",
			"haldls::v2::SynapseDriverBlock"}));
}

TEST(Visitors, PPUMemory) {
	TypeNameVisitor visitor;
	PPUMemoryOnDLS const coord;
	PPUMemory const config;

	visit_preorder(config, coord, visitor);

	EXPECT_THAT(
		visitor.names,
		std::set<std::string>({
			"haldls::v2::PPUMemory",
			"haldls::v2::PPUMemoryWord"}));
}

TEST(Visitors, RateCounter) {
	TypeNameVisitor visitor;
	RateCounterOnDLS const coord;
	RateCounter const config;

	visit_preorder(config, coord, visitor);

	EXPECT_THAT(
		visitor.names,
		std::set<std::string>({
			"haldls::v2::RateCounter",
			"haldls::v2::RateCounterEntry"}));
}
