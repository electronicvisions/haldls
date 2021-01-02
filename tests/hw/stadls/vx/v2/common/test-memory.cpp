#include <random>
#include <vector>
#include <gtest/gtest.h>

#include "halco/common/iter_sparse.h"
#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "haldls/vx/is_writereadable.h"
#include "haldls/vx/v2/barrier.h"
#include "haldls/vx/v2/container.h"
#include "haldls/vx/v2/traits.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v2/decode.h"
#include "stadls/vx/v2/init_generator.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/run.h"
#include "ztl/debug.h"

#ifdef SIMULATION_TEST
constexpr static bool is_simulation = true;
#else
constexpr static bool is_simulation = false;
#endif

using namespace haldls::vx::v2;
using namespace stadls::vx::v2;
using namespace halco::hicann_dls::vx::v2;
using namespace halco::common;

/**
 * Random generator for this test.
 * Initialized with random seed because it leads to coverage over time.
 */
static std::mt19937 random_generator(std::random_device{}());

#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
#define PLAYBACK_CONTAINER(Name, Type) Type,
typedef hate::type_list<
#include "haldls/vx/v2/container.def"
    >
    ContainerList;

typedef hate::filter_type_list_t<haldls::vx::HasLocalData, ContainerList> HasLocalDataContainerList;

typedef hate::filter_type_list_t<haldls::vx::detail::IsWriteReadable, HasLocalDataContainerList>
    ReadableAndWriteableContainerList;

template <typename TL>
struct to_testing_types;

template <typename... Ts>
struct to_testing_types<hate::type_list<Ts...>>
{
	typedef ::testing::Types<Ts...> type;
};

typedef to_testing_types<ReadableAndWriteableContainerList>::type ReadableAndWriteableContainers;


/**
 * Test fixture for generic write-read memory tests of a single container type.
 * @tparam ContainerUnderTest Container type to be written and read back
 */
template <class ContainerUnderTest>
class SingleContainerWriteReadMemoryTest : public testing::Test
{};


namespace haldls::vx {
class TypeNameToString
{
public:
	template <typename T>
	static std::string GetName(int)
	{
		return ZTL::typestring<T>().substr(strlen("haldls::vx::"));
	}
};
} // namespace haldls::vx

TYPED_TEST_SUITE(
    SingleContainerWriteReadMemoryTest,
    ReadableAndWriteableContainers,
    haldls::vx::TypeNameToString);

/**
 * Write random data to (a subset of) all coordinates, read it back and compare.
 *
 * For read- as well as write instructions, the container's default backend is used.
 */
TYPED_TEST(SingleContainerWriteReadMemoryTest, SequentialRandomWriteRead)
{
	// test only carried out for the default word type
	typedef
	    typename haldls::vx::detail::BackendContainerTrait<TypeParam>::default_container word_type;
	if constexpr (
	    !fisch::vx::IsReadable<word_type>::value || !fisch::vx::IsWritable<word_type>::value) {
		GTEST_SKIP() << "Backend word type does not support read and write operation.";
	}

	PlaybackProgramBuilder write_builder;
	PlaybackProgramBuilder read_builder;

	std::vector<TypeParam> reference_containers;
	std::vector<PlaybackProgram::ContainerTicket<TypeParam>> read_tickets;

	for (auto const coord :
	     iter_sparse<typename TypeParam::coordinate_type>(MAX_WORDS_PER_REDUCED_TEST)) {
		TypeParam reference_container;

		stadls::vx::decode_random(random_generator, reference_container);

		reference_containers.push_back(reference_container);

		write_builder.write(coord, reference_container);

		auto ticket = read_builder.read(coord);
		read_tickets.push_back(ticket);
	}

	EXPECT_EQ(reference_containers.size(), read_tickets.size());

	Barrier barrier;
	barrier.set_enable_jtag(true);
	barrier.set_enable_omnibus(true);

	DigitalInit init;
	init.enable_xboard = !is_simulation;
	auto [run_builder, _] = generate(init);
	run_builder.block_until(BarrierOnFPGA(), barrier);

	run_builder.merge_back(write_builder);
	run_builder.merge_back(read_builder);

	run_builder.block_until(BarrierOnFPGA(), barrier);
	auto run_program = run_builder.done();
	{
		auto connection = hxcomm::vx::get_connection_from_env();

		// check that operation for given connection is permitted
		auto const target_unsupported = [run_program](auto& conn) {
			std::vector<hxcomm::vx::Target> remaining;

			auto const supported = std::set(conn.supported_targets);
			auto const unsupported = std::set(
			    run_program.get_unsupported_targets().begin(),
			    run_program.get_unsupported_targets().end());
			std::set_difference(
			    supported.begin(), supported.end(), unsupported.begin(), unsupported.end(),
			    std::back_inserter(remaining));
			return remaining.empty();
		};
		if (std::visit(target_unsupported, connection)) {
			GTEST_SKIP() << "Unsupported target.";
		}

		stadls::vx::run(connection, run_program);
	}

	for (size_t i = 0; i < reference_containers.size(); ++i) {
		EXPECT_TRUE(read_tickets.at(i).valid());
		EXPECT_EQ(reference_containers.at(i), read_tickets.at(i).get());
	}
}
