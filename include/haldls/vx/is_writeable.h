#pragma once
#include <type_traits>
#include <boost/utility/enable_if.hpp>
#include "hate/type_list.h"

namespace halco::hicann_dls::vx::v1 {
struct Coordinates;
} // namespace halco::hicann_dls::vx::v1

namespace halco::hicann_dls::vx::v2 {
struct Coordinates;
} // namespace halco::hicann_dls::vx::v2

namespace haldls::vx {

template <typename Coordinates>
class CapMemBlock;

namespace v1 {
using CapMemBlock = haldls::vx::CapMemBlock<halco::hicann_dls::vx::v1::Coordinates>;
} // namespace v1

namespace v2 {
using CapMemBlock = haldls::vx::CapMemBlock<halco::hicann_dls::vx::v2::Coordinates>;
} // namespace v2

} // namespace haldls::vx

namespace haldls::vx::detail {

template <typename T, typename = void>
struct IsWriteable : public std::false_type
{};

template <typename T>
struct IsWriteable<T, typename boost::enable_if_has_type<decltype(T::config_size_in_words)>::type>
    : public std::true_type
{};

template <typename T>
struct IsWriteable<
    T,
    typename boost::enable_if_has_type<decltype(T::write_config_size_in_words)>::type>
{
	constexpr static bool value = T::write_config_size_in_words;
};

typedef hate::type_list<
    PPUMemoryBlock,
    v1::CapMemBlock,
    v2::CapMemBlock,
    lola::vx::SynapseRow,
    lola::vx::SynapseMatrix>
    NonLeafNodeWriteableContainerList;

// manually add all non-leaf node containers which are writeable
template <typename T>
struct IsWriteable<
    T,
    std::enable_if_t<hate::is_in_type_list<T, NonLeafNodeWriteableContainerList>::value>>
    : std::true_type
{};

} // namespace haldls::vx::detail
