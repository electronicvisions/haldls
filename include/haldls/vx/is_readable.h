#pragma once
#include "fisch/vx/traits.h"
#include "haldls/vx/container.h"
#include "hate/type_list.h"
#include "lola/vx/container.h"

namespace haldls::vx::detail {

template <typename T, typename = void>
struct IsReadable : public std::false_type
{};

template <typename T>
struct IsReadable<T, typename boost::enable_if_has_type<decltype(T::config_size_in_words)>::type>
    : public std::true_type
{};

template <typename T>
struct IsReadable<
    T,
    typename boost::enable_if_has_type<decltype(T::read_config_size_in_words)>::type>
{
	constexpr static bool value = T::read_config_size_in_words;
};

typedef hate::type_list<
    PPUMemoryBlock,
    CapMemBlock,
    lola::vx::CADCSampleRow,
    lola::vx::SynapseRow,
    lola::vx::SynapseWeightRow,
    lola::vx::SynapseLabelRow,
    lola::vx::SynapseCorrelationCalibRow,
    lola::vx::SynapseMatrix,
    lola::vx::SynapseWeightMatrix,
    lola::vx::SynapseLabelMatrix,
    lola::vx::SynapseCorrelationCalibMatrix>
    NonLeafNodeReadableContainerList;

// manually add all non-leaf node containers which are readable
template <typename T>
struct IsReadable<
    T,
    std::enable_if_t<hate::is_in_type_list<T, NonLeafNodeReadableContainerList>::value>>
    : std::true_type
{};

} // namespace haldls::vx::detail
