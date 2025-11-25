#include <gtest/gtest.h>

#include "fisch/vx/traits.h"
#include "halco/common/iter_sparse.h"
#include "haldls/vx/is_readable.h"
#include "haldls/vx/is_writeable.h"
#include "haldls/vx/v3/container.h"
#include "hate/timer.h"
#include "hate/type_index.h"
#include "hate/type_list.h"
#include "hxcomm/vx/multi_zeromockconnection.h"
#include "lola/vx/v3/container.h"
#include "stadls/visitors.h"
#include "stadls/vx/container_ticket.h"
#include "stadls/vx/v3/decode.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include <type_traits>
#include <log4cxx/logger.h>

using namespace haldls::vx;
using namespace haldls::vx::v3;


// approx. 15.6MHz
constexpr size_t ideal_minimal_messages_per_s = 1000ull * 1000ull *
                                                1000ull /* 1GBit (host <-> FPGA) */ /
                                                8ull /* bit/byte */ / 8ull /* byte/UT-message */;

// since esp. for small containers the ideal rate is not reached, we test here for a rate to which
// we should never degrade below.
constexpr size_t minimal_messages_per_s = ideal_minimal_messages_per_s / 1000ull;

template <typename Container>
inline size_t get_coordinate_size()
{
	if constexpr (std::is_same_v<Container, lola::vx::v3::ExternalPPUMemoryBlock>) {
		return 1;
	} else if constexpr (std::is_same_v<Container, haldls::vx::v3::PPUMemoryBlock>) {
		return 2;
	} else if constexpr (std::is_same_v<Container, lola::vx::v3::LogicalNeuron>) {
		return 1;
	} else {
		return halco::common::iter_sparse<typename Container::coordinate_type>().size();
	}
}

typedef hate::type_list<haldls::vx::JTAGIdCode, haldls::vx::SpikeIOConfig> NotZeroReadable;

constexpr size_t num_containers = 100;


template <typename Type>
std::enable_if_t<
    !std::is_base_of_v<haldls::vx::Container, Type> ||
    !haldls::vx::detail::IsWriteable<Type>::value ||
    !fisch::vx::IsWritable<decltype(
        fisch::vx::container_cast(std::declval<typename haldls::vx::detail::BackendContainerTrait<
                                      Type>::default_container>()))>::value>
test_write()
{}

template <typename Type>
std::enable_if_t<
    std::is_base_of_v<haldls::vx::Container, Type> &&
    haldls::vx::detail::IsWriteable<Type>::value &&
    fisch::vx::IsWritable<decltype(
        fisch::vx::container_cast(std::declval<typename haldls::vx::detail::BackendContainerTrait<
                                      Type>::default_container>()))>::value>
test_write()
{
	std::vector<Type> configs;
	std::mt19937 rng(std::random_device{}());
	for (size_t i = 0; i < num_containers; ++i) {
		configs.push_back(Type());
		stadls::vx::decode_random(rng, configs.at(i));
	}
	typename Type::coordinate_type coord = stadls::vx::detail::get_coord(Type());
	stadls::vx::v3::PlaybackProgramBuilder builder;
	hate::Timer timer;
	for (size_t i = 0; i < num_containers; ++i) {
		builder.write(coord, configs.at(i));
	}
	size_t const ns = timer.get_ns();
	size_t const actual_messages_per_s = builder.size_to_fpga() * 1000 * 1000 * 1000 / ns;
	if (!builder.empty()) {
		auto const logger = log4cxx::Logger::getLogger("stadls.encode_throughput");
		LOG4CXX_TRACE(
		    logger, hate::full_name<Type>()
		                << " write size: " << builder.size_to_fpga() / num_containers);
		LOG4CXX_TRACE(
		    logger, hate::full_name<Type>()
		                << " write: " << actual_messages_per_s << " [UT-Messages/s]");
		LOG4CXX_TRACE(
		    logger, hate::full_name<Type>()
		                << " write coord size: " << get_coordinate_size<Type>());
	}
	EXPECT_GE(actual_messages_per_s, minimal_messages_per_s);
}

template <typename Type>
std::enable_if_t<
    !std::is_base_of_v<haldls::vx::Container, Type> ||
    !haldls::vx::detail::IsReadable<Type>::value ||
    !fisch::vx::IsReadable<decltype(
        fisch::vx::container_cast(std::declval<typename haldls::vx::detail::BackendContainerTrait<
                                      Type>::default_container>()))>::value>
test_read()
{}

template <typename Type>
std::enable_if_t<
    std::is_base_of_v<haldls::vx::Container, Type> && haldls::vx::detail::IsReadable<Type>::value &&
    fisch::vx::IsReadable<decltype(
        fisch::vx::container_cast(std::declval<typename haldls::vx::detail::BackendContainerTrait<
                                      Type>::default_container>()))>::value>
test_read()
{
	typename Type::coordinate_type coord = stadls::vx::detail::get_coord(Type());
	stadls::vx::v3::PlaybackProgramBuilder builder;
	hate::Timer timer;
	for (size_t i = 0; i < num_containers; ++i) {
		builder.read(coord);
	}
	size_t const ns = timer.get_ns();
	size_t const actual_messages_per_s = builder.size_to_fpga() * 1000 * 1000 * 1000 / ns;
	if (!builder.empty()) {
		auto const logger = log4cxx::Logger::getLogger("stadls.encode_throughput");
		LOG4CXX_TRACE(
		    logger, hate::full_name<Type>()
		                << " read size: " << builder.size_to_fpga() / num_containers);
		LOG4CXX_TRACE(
		    logger, hate::full_name<Type>()
		                << " read: " << actual_messages_per_s << " [UT-Messages/s]");
		LOG4CXX_TRACE(
		    logger, hate::full_name<Type>() << " read coord size: " << get_coordinate_size<Type>());
	}
	EXPECT_GE(actual_messages_per_s, minimal_messages_per_s);
}

template <typename Type>
std::enable_if_t<
    !std::is_base_of_v<haldls::vx::Container, Type> ||
    !haldls::vx::detail::IsReadable<Type>::value ||
    !fisch::vx::IsReadable<decltype(
        fisch::vx::container_cast(std::declval<typename haldls::vx::detail::BackendContainerTrait<
                                      Type>::default_container>()))>::value ||
    hate::is_in_type_list<Type, NotZeroReadable>::value>
test_read_get()
{}

template <typename Type>
std::enable_if_t<
    std::is_base_of_v<haldls::vx::Container, Type> && haldls::vx::detail::IsReadable<Type>::value &&
    fisch::vx::IsReadable<decltype(
        fisch::vx::container_cast(std::declval<typename haldls::vx::detail::BackendContainerTrait<
                                      Type>::default_container>()))>::value &&
    !hate::is_in_type_list<Type, NotZeroReadable>::value>
test_read_get()
{
	typename Type::coordinate_type coord = stadls::vx::detail::get_coord(Type());
	stadls::vx::v3::PlaybackProgramBuilder builder;
	std::vector<stadls::vx::ContainerTicket> tickets;
	for (size_t i = 0; i < num_containers; ++i) {
		tickets.push_back(builder.read(coord));
	}
	auto const size_from_fpga = builder.size_from_fpga();
	// execute the read commands with the zero mock connection in order to inject response data to
	// decode in the tickets afterwards. test, that the execution is supported on the zero mock
	// target and if not skip the test.
	hxcomm::vx::MultiZeroMockConnection conn;
	auto const supported = std::set(conn.supported_targets);
	auto program = builder.done();
	auto const& unsupported_unsorted = program.get_unsupported_targets();
	auto const unsupported = std::set(unsupported_unsorted.begin(), unsupported_unsorted.end());
	std::vector<hxcomm::vx::Target> remaining;
	std::set_difference(
	    supported.begin(), supported.end(), unsupported.begin(), unsupported.end(),
	    std::back_inserter(remaining));
	if (remaining.empty()) {
		return;
	}
	stadls::vx::v3::run(conn, program);
	hate::Timer timer;
	for (size_t i = 0; i < num_containers; ++i) {
		tickets[i].get();
	}
	size_t const ns = timer.get_ns();
	size_t const actual_messages_per_s = size_from_fpga * 1000 * 1000 * 1000 / ns;
	auto const logger = log4cxx::Logger::getLogger("stadls.encode_throughput");
	LOG4CXX_TRACE(
	    logger, hate::full_name<Type>() << " readget size: " << size_from_fpga / num_containers);
	LOG4CXX_TRACE(
	    logger, hate::full_name<Type>()
	                << " readget: " << actual_messages_per_s << " [UT-Messages/s]");
	LOG4CXX_TRACE(
	    logger, hate::full_name<Type>() << " readget coord size: " << get_coordinate_size<Type>());
	EXPECT_GE(actual_messages_per_s, minimal_messages_per_s);
}


#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	TEST(Name, Write)                                                                              \
	{                                                                                              \
		test_write<Type>();                                                                        \
	}                                                                                              \
                                                                                                   \
	TEST(Name, Read)                                                                               \
	{                                                                                              \
		test_read<Type>();                                                                         \
	}                                                                                              \
                                                                                                   \
	TEST(Name, ReadGet)                                                                            \
	{                                                                                              \
		test_read_get<Type>();                                                                     \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v3/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v3/container.def"
