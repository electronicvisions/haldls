#pragma once
#include <type_traits>
#include <boost/utility/enable_if.hpp>
#include "hate/type_list.h"

namespace halco::hicann_dls::vx::v3 {
struct Coordinates;
} // namespace halco::hicann_dls::vx::v3

namespace haldls::vx {

template <typename Coordinates>
class CapMemBlock;
class PPUMemoryBlock;
class PPUMemory;

namespace v3 {
using CapMemBlock = haldls::vx::CapMemBlock<halco::hicann_dls::vx::v3::Coordinates>;
using PPUMemoryBlock = haldls::vx::PPUMemoryBlock;
using PPUMemory = haldls::vx::PPUMemory;
} // namespace v3

} // namespace haldls::vx

namespace lola::vx {
class ExternalPPUMemoryBlock;
class CorrelationResetRow;
class DACChannelBlock;
class DACControlBlock;
class SynapseCorrelationCalibRow;
class SynapseCorrelationCalibMatrix;
class SynapseLabelMatrix;
class SynapseLabelRow;
class SynapseMatrix;
class SynapseRow;
class SynapseWeightMatrix;
class SynapseWeightRow;
} // namespace lola::vx

namespace lola::vx::v3 {
class AtomicNeuron;
class CADCSampleRow;
class CADCSamples;
using DACChannelBlock = lola::vx::DACChannelBlock;
using DACControlBlock = lola::vx::DACControlBlock;
using ExternalPPUMemoryBlock = lola::vx::ExternalPPUMemoryBlock;
} // namespace lola::vx::v3

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
    lola::vx::CorrelationResetRow,
    lola::vx::SynapseRow,
    lola::vx::SynapseMatrix,
    lola::vx::SynapseWeightMatrix,
    lola::vx::SynapseLabelMatrix,
    lola::vx::SynapseCorrelationCalibMatrix,
    lola::vx::SynapseWeightRow,
    lola::vx::SynapseLabelRow,
    lola::vx::SynapseCorrelationCalibRow,
    v3::PPUMemoryBlock,
    v3::PPUMemory,
    v3::CapMemBlock,
    lola::vx::v3::AtomicNeuron,
    lola::vx::v3::DACChannelBlock,
    lola::vx::v3::DACControlBlock,
    lola::vx::v3::ExternalPPUMemoryBlock>
    NonLeafNodeWriteableContainerList;

// manually add all non-leaf node containers which are writeable
template <typename T>
struct IsWriteable<
    T,
    std::enable_if_t<hate::is_in_type_list<T, NonLeafNodeWriteableContainerList>::value>>
    : std::true_type
{};

} // namespace haldls::vx::detail
