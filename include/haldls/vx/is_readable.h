#pragma once
#include "fisch/vx/traits.h"
#include "haldls/vx/v1/container.h"
#include "hate/type_list.h"
#include "lola/vx/v1/container.h"

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
    v1::PPUMemoryBlock,
    v1::CapMemBlock,
    lola::vx::v1::CADCSampleRow,
    lola::vx::v1::SynapseRow,
    lola::vx::v1::SynapseWeightRow,
    lola::vx::v1::SynapseLabelRow,
    lola::vx::v1::SynapseCorrelationCalibRow,
    lola::vx::v1::SynapseMatrix,
    lola::vx::v1::SynapseWeightMatrix,
    lola::vx::v1::SynapseLabelMatrix,
    lola::vx::v1::SynapseCorrelationCalibMatrix>
    NonLeafNodeReadableContainerList;

// manually add all non-leaf node containers which are readable
template <typename T>
struct IsReadable<
    T,
    std::enable_if_t<hate::is_in_type_list<T, NonLeafNodeReadableContainerList>::value>>
    : std::true_type
{};

} // namespace haldls::vx::detail
