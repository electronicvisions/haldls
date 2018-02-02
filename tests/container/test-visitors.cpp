#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <set>

// if we only had boost.TypeIndex :(
#include <cxxabi.h>
#include <string>
#include <typeinfo>

#include "stadls/visitors.h"
#include "haldls/container/v2/chip.h"

namespace {

template <typename T>
std::string demangled_typename(T const* = nullptr) {
	int status = 0;
	char const* mangled_name = typeid(T).name();
	std::unique_ptr<char[], decltype(&free)> demangled_name(
		abi::__cxa_demangle(mangled_name, /*output_buffer=*/nullptr, /*length=*/0, &status), &free);
	return {status == 0 ? demangled_name.get() : mangled_name};
}

struct TypeNameVisitor {
	std::set<std::string> names;

	template <typename CoordinateT, typename ContainerT>
	void operator()(CoordinateT const&, ContainerT const& config)
	{
		names.insert(demangled_typename(&config));
	}
};

} // namespace

using namespace haldls::container::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(Visitors, Chip) {
	TypeNameVisitor visitor;
	Unique const unique;
	Chip const chip;

	visit_preorder(chip, unique, visitor);
	EXPECT_THAT(
		visitor.names,
		::testing::WhenSorted(::testing::ElementsAreArray({
			"haldls::container::v2::CapMem",
			"haldls::container::v2::CapMemCell",
			"haldls::container::v2::Chip",
			"haldls::container::v2::ColumnCorrelationBlock",
			"haldls::container::v2::ColumnCurrentBlock",
			"haldls::container::v2::CommonCapMemConfig",
			"haldls::container::v2::CommonNeuronConfig",
			"haldls::container::v2::CommonSynramConfig",
			"haldls::container::v2::CorrelationConfig",
			"haldls::container::v2::NeuronDigitalConfig",
			"haldls::container::v2::PPUControlRegister",
			"haldls::container::v2::PPUMemory",
			"haldls::container::v2::PPUMemoryWord",
			"haldls::container::v2::PPUStatusRegister",
			"haldls::container::v2::RateCounter",
			"haldls::container::v2::SynapseBlock",
			"haldls::container::v2::SynapseDrivers"})));
}
