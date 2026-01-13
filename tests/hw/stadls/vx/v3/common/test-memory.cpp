#include <random>
#include <string>
#include <typeindex>
#include <vector>
#include <gtest/gtest.h>

#include "halco/common/iter_sparse.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/is_read_and_writable.h"
#include "haldls/vx/is_writereadable.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/container.h"
#include "haldls/vx/v3/traits.h"
#include "hate/type_index.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/container_ticket.h"
#include "stadls/vx/v3/decode.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/run.h"

#ifdef SIMULATION_TEST
constexpr static bool is_simulation = true;
#else
constexpr static bool is_simulation = false;
#endif

using namespace haldls::vx::v3;
using namespace stadls::vx::v3;
using namespace halco::hicann_dls::vx::v3;
using namespace halco::common;

// Optionally reduced memory test coverage
extern std::optional<size_t> max_words_per_reduced_test;

// These containers are not supported in the hardware-setups used for automated testing
typedef hate::type_list<
    haldls::vx::TCA9554Config,
    haldls::vx::TCA9546ChannelConfig,
    haldls::vx::AD5252ChannelConfig,
    haldls::vx::AD5252ChannelConfigPersistent, // Persistent register. DO NOT change randomly
    haldls::vx::DAC6573ChannelConfig,
    haldls::vx::Timer>
    WriteTestsDisabledContainers;

/**
 * Random generator for this test.
 * Initialized with random seed because it leads to coverage over time.
 */
static std::mt19937 random_generator(std::random_device{}());

#define LAST_PLAYBACK_CONTAINER(Name, Type) Type
#define PLAYBACK_CONTAINER(Name, Type) Type,
typedef hate::type_list<
#include "haldls/vx/v3/container.def"
    >
    ContainerList;

typedef hate::filter_type_list_t<haldls::vx::HasLocalData, ContainerList> HasLocalDataContainerList;

typedef hate::filter_type_list_t<haldls::vx::detail::IsWriteReadable, HasLocalDataContainerList>
    ReadableAndWriteableContainerList;

template <typename T, typename TL>
struct to_pairs;

template <typename T, typename... Ts>
struct to_pairs<T, hate::type_list<Ts...>>
{
	typedef hate::type_list<std::pair<T, Ts>...> type;
};

template <typename TL>
struct to_container_backend_pairs;

template <typename... Ts>
struct to_container_backend_pairs<hate::type_list<Ts...>>
{
	typedef hate::multi_concat_t<typename to_pairs<
	    Ts,
	    typename haldls::vx::detail::BackendContainerTrait<Ts>::container_list>::type...>
	    type;
};

template <typename TL>
struct to_testing_types;

template <typename... Ts>
struct to_testing_types<hate::type_list<Ts...>>
{
	typedef ::testing::Types<Ts...> type;
};

/**
 * Filter specifying all containers which don't have side effects leading to some other container
 * being inaccessible.
 */
template <typename T>
struct HasNoSideeffects
{
	constexpr static bool value = !hate::is_in_type_list<
	    T,
	    hate::type_list<
	        InstructionTimeoutConfig,
	        CommonSynramConfig,
	        PhyConfigFPGA,
	        PhyConfigChip,
	        PPUControlRegister,
	        CommonPhyConfigFPGA,
	        CommonPhyConfigChip,
	        PerfTest>>::value;
};

typedef hate::filter_type_list_t<HasNoSideeffects, ReadableAndWriteableContainerList>
    ReadableAndWriteableNoSideeffectsContainerList;

typedef to_testing_types<
    to_container_backend_pairs<ReadableAndWriteableNoSideeffectsContainerList>::type>::type
    ReadableAndWriteableContainers;

std::map<std::type_index, size_t> max_words_per_reduced_test_per_container{
    {std::type_index(typeid(ExternalPPUDRAMMemoryByte)), 1000},
    {std::type_index(typeid(ExternalPPUDRAMMemoryQuad)), 1000}};

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
		std::string const full_name = hate::full_name<T>();
		// full_name are in the form of std::pair<haldls::vx::CommonNeuronBackendConfig,
		// fisch::vx::word_access_type::OmnibusChipOverJTAG> will be converted to
		// CommonNeuronBackendConfig_OmnibusChipOverJTAG
		size_t begin_first = 0;
		if (full_name.find("std::pair<haldls::vx::v") != std::string::npos) {
			begin_first = strlen("std::pair<haldls::vx::vy::");
		} else {
			begin_first = strlen("std::pair<haldls::vx::");
		}
		size_t const end_first = full_name.find(",");
		std::string first = full_name.substr(begin_first, end_first - begin_first);
		// some types are dependent on a coordinate -> also remove the coordinate NS
		size_t coord_pos = first.find("halco::hicann_dls::vx::v");
		if (coord_pos != std::string::npos) {
			first.erase(coord_pos, strlen("halco::hicann_dls::vx::vy::"));
		}
		coord_pos = first.find("halco::hicann_dls::vx::");
		if (coord_pos != std::string::npos) {
			first.erase(coord_pos, strlen("halco::hicann_dls::vx::"));
		}

		size_t const begin_second = full_name.find("fisch::vx::word_access_type::") +
		                            strlen("fisch::vx::word_access_type::");
		size_t const end_second = full_name.find(">", begin_second);

		std::string const second = full_name.substr(begin_second, end_second - begin_second);
		return first + "_" + second;
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
 * All container backends supporting read and write instructions are used.
 */
TYPED_TEST(SingleContainerWriteReadMemoryTest, SequentialRandomWriteRead)
{
	if constexpr (hate::is_in_type_list<
	                  typename TypeParam::first_type, WriteTestsDisabledContainers>::value) {
		GTEST_SKIP() << "Test is manually disabled for this container.";
	}

	typedef typename TypeParam::first_type Container;
	constexpr auto backend = haldls::vx::detail::backend_from_backend_container_type<
	    typename TypeParam::second_type>::backend;
	// check if backend supports read and write
	if (!haldls::vx::detail::is_read_and_writeable(backend)) {
		GTEST_SKIP() << "Backend not read- and writeable.";
	}

	PlaybackProgramBuilder write_builder;
	PlaybackProgramBuilder read_builder;

	std::vector<Container> reference_containers;
	std::vector<ContainerTicket> read_tickets;

	std::optional<size_t> num_locations = max_words_per_reduced_test;
	if (!num_locations &&
	    max_words_per_reduced_test_per_container.contains(std::type_index(typeid(Container)))) {
		num_locations =
		    max_words_per_reduced_test_per_container.at(std::type_index(typeid(Container)));
	}

	for (auto const& coord : iter_sparse<typename Container::coordinate_type>(num_locations)) {
		Container reference_container;

		stadls::vx::decode_random(random_generator, reference_container);

		reference_containers.push_back(reference_container);

		write_builder.write(coord, reference_container, backend);

		auto ticket = read_builder.read(coord, backend);
		read_tickets.push_back(ticket);
	}

	EXPECT_EQ(reference_containers.size(), read_tickets.size());

	Barrier barrier;
	barrier.set_enable_jtag(true);
	barrier.set_enable_omnibus(true);

	{
		auto connection = hxcomm::vx::get_connection_from_env();

		SystemInit init(std::visit(
		    [](auto const& connection) { return connection.get_hwdb_entry().at(0); }, connection));
		init.enable_asic_adapter_board = !is_simulation;
		init.chip.enable_capmem = false;
		auto [run_builder, _] = generate(init);
		run_builder.block_until(BarrierOnFPGA(), barrier);

		run_builder.merge_back(write_builder);
		run_builder.merge_back(read_builder);

		run_builder.block_until(BarrierOnFPGA(), barrier);
		auto run_program = run_builder.done();

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
		EXPECT_EQ(
		    reference_containers.at(i),
		    dynamic_cast<TypeParam::first_type const&>(read_tickets.at(i).get()));
	}
}
