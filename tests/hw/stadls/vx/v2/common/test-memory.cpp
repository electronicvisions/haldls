#include <random>
#include <vector>
#include <gtest/gtest.h>

#include "halco/common/iter_sparse.h"
#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "haldls/vx/is_read_and_writable.h"
#include "haldls/vx/is_writereadable.h"
#include "haldls/vx/v2/barrier.h"
#include "haldls/vx/v2/container.h"
#include "haldls/vx/v2/traits.h"
#include "hate/type_index.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v2/decode.h"
#include "stadls/vx/v2/init_generator.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/run.h"

#ifdef SIMULATION_TEST
constexpr static bool is_simulation = true;
#else
constexpr static bool is_simulation = false;
#endif

using namespace haldls::vx::v2;
using namespace stadls::vx::v2;
using namespace halco::hicann_dls::vx::v2;
using namespace halco::common;

// These containers are not supported in the hardware-setups used for automated testing
typedef hate::type_list<
    haldls::vx::TCA9554Config,
    haldls::vx::AD5252ChannelConfig,
    haldls::vx::AD5252ChannelConfigPersistent, // Persistent register. DO NOT change randomly
    haldls::vx::DAC6573ChannelConfig,
    haldls::vx::Timer,
    haldls::vx::EventSwitchSource,
    haldls::vx::EventSwitchReadout,
    haldls::vx::EventSwitchConfig,
    haldls::vx::ExtollSpikeCommBucketTriggerConfig,
    haldls::vx::ExtollSpikeCommBucketDestinationConfig,
    haldls::vx::ExtollSpikeCommBucketNumPktsSent,
    haldls::vx::ExtollSpikeCommBucketCounterReset,
    haldls::vx::ExtollSpikeCommRouterLookupConfig,
    haldls::vx::ExtollSpikeCommRouterConfig,
    haldls::vx::ExtollSpikeCommRouterEventLossDisabled,
    haldls::vx::ExtollSpikeCommRouterEventLossMisconf,
    haldls::vx::ExtollSpikeCommRouterEventsRouted,
    haldls::vx::ExtollSpikeCommRouterCounterReset,
    haldls::vx::ExtollSpikeCommDecoderNumEventsReceived,
    haldls::vx::ExtollSpikeCommDecoderCounterReset,
    haldls::vx::ExtollSpikeCommTimestampDelayConfig,
    haldls::vx::ExtollSpikeCommTimestampDelayNumEventsReceived,
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossFull,
    haldls::vx::ExtollSpikeCommTimestampDelayEventLossExpired,
    haldls::vx::ExtollSpikeCommTimestampDelayCounterReset,
    haldls::vx::ExtollBarrierTriggerReached,
    haldls::vx::ExtollBarrierReleased,
    haldls::vx::ExtollBarrierConfig,
    haldls::vx::ExtollInterruptControl,
    haldls::vx::ExtollInterruptConfig,
    haldls::vx::ExtollBarrierInterruptInportErrorCount,
    haldls::vx::ExtollBarrierInterruptInportCounterReset>
    WriteTestsDisabledContainers;

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
		return hate::full_name<T>().substr(strlen("haldls::vx::"));
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
	std::vector<PlaybackProgram::ContainerTicket<Container>> read_tickets;

	for (auto const coord :
	     iter_sparse<typename Container::coordinate_type>(MAX_WORDS_PER_REDUCED_TEST)) {
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
