#pragma once
#include "fisch/vx/traits.h"
#include "hate/type_list.h"

namespace halco::hicann_dls::vx::v3 {
class Coordinates;
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
class ExternalPPUDRAMMemoryBlock;
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
using ExternalPPUMemoryBlock = lola::vx::ExternalPPUMemoryBlock;
using ExternalPPUDRAMMemoryBlock = lola::vx::ExternalPPUDRAMMemoryBlock;
} // namespace lola::vx::v3

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
    lola::vx::SynapseCorrelationCalibRow,
    lola::vx::SynapseCorrelationCalibMatrix,
    lola::vx::SynapseLabelMatrix,
    lola::vx::SynapseLabelRow,
    lola::vx::SynapseMatrix,
    lola::vx::SynapseRow,
    lola::vx::SynapseWeightMatrix,
    lola::vx::SynapseWeightRow,
    v3::PPUMemory,
    v3::PPUMemoryBlock,
    v3::CapMemBlock,
    lola::vx::v3::AtomicNeuron,
    lola::vx::v3::CADCSampleRow,
    lola::vx::v3::CADCSamples,
    lola::vx::v3::ExternalPPUMemoryBlock,
    lola::vx::v3::ExternalPPUDRAMMemoryBlock>
    NonLeafNodeReadableContainerList;

// manually add all non-leaf node containers which are readable
template <typename T>
struct IsReadable<
    T,
    std::enable_if_t<hate::is_in_type_list<T, NonLeafNodeReadableContainerList>::value>>
    : std::true_type
{};

} // namespace haldls::vx::detail
