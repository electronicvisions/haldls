#pragma once
#include "fisch/vx/traits.h"
#include "hate/type_list.h"

namespace halco::hicann_dls::vx::v1 {
class Coordinates;
} // namespace halco::hicann_dls::vx::v1

namespace halco::hicann_dls::vx::v2 {
class Coordinates;
} // namespace halco::hicann_dls::vx::v2

namespace halco::hicann_dls::vx::v3 {
class Coordinates;
} // namespace halco::hicann_dls::vx::v3

namespace haldls::vx {

template <typename Coordinates>
class CapMemBlock;
class PPUMemoryBlock;
class PPUMemory;

namespace v1 {
using CapMemBlock = haldls::vx::CapMemBlock<halco::hicann_dls::vx::v1::Coordinates>;
using PPUMemoryBlock = haldls::vx::PPUMemoryBlock;
using PPUMemory = haldls::vx::PPUMemory;
} // namespace v1

namespace v2 {
using CapMemBlock = haldls::vx::CapMemBlock<halco::hicann_dls::vx::v2::Coordinates>;
using PPUMemoryBlock = haldls::vx::PPUMemoryBlock;
using PPUMemory = haldls::vx::PPUMemory;
} // namespace v2

namespace v3 {
using CapMemBlock = haldls::vx::CapMemBlock<halco::hicann_dls::vx::v3::Coordinates>;
using PPUMemoryBlock = haldls::vx::PPUMemoryBlock;
using PPUMemory = haldls::vx::PPUMemory;
} // namespace v3

} // namespace haldls::vx

namespace lola::vx {
class ExternalPPUMemoryBlock;
} // namespace lola::vx

namespace lola::vx::v1 {
class AtomicNeuron;
class CADCSampleRow;
class CADCSamples;
using ExternalPPUMemoryBlock = lola::vx::ExternalPPUMemoryBlock;
class SynapseCorrelationCalibRow;
class SynapseCorrelationCalibMatrix;
class SynapseLabelMatrix;
class SynapseLabelRow;
class SynapseMatrix;
class SynapseRow;
class SynapseWeightMatrix;
class SynapseWeightRow;
} // namespace lola::vx::v1

namespace lola::vx::v2 {
class AtomicNeuron;
class CADCSampleRow;
class CADCSamples;
using ExternalPPUMemoryBlock = lola::vx::ExternalPPUMemoryBlock;
class SynapseCorrelationCalibRow;
class SynapseCorrelationCalibMatrix;
class SynapseLabelMatrix;
class SynapseLabelRow;
class SynapseMatrix;
class SynapseRow;
class SynapseWeightMatrix;
class SynapseWeightRow;
} // namespace lola::vx::v2

namespace lola::vx::v3 {
class AtomicNeuron;
class CADCSampleRow;
class CADCSamples;
using ExternalPPUMemoryBlock = lola::vx::ExternalPPUMemoryBlock;
class SynapseCorrelationCalibRow;
class SynapseCorrelationCalibMatrix;
class SynapseLabelMatrix;
class SynapseLabelRow;
class SynapseMatrix;
class SynapseRow;
class SynapseWeightMatrix;
class SynapseWeightRow;
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
    v1::PPUMemory,
    v1::PPUMemoryBlock,
    v1::CapMemBlock,
    lola::vx::v1::AtomicNeuron,
    lola::vx::v1::CADCSampleRow,
    lola::vx::v1::CADCSamples,
    lola::vx::v1::ExternalPPUMemoryBlock,
    lola::vx::v1::SynapseCorrelationCalibRow,
    lola::vx::v1::SynapseCorrelationCalibMatrix,
    lola::vx::v1::SynapseLabelMatrix,
    lola::vx::v1::SynapseLabelRow,
    lola::vx::v1::SynapseMatrix,
    lola::vx::v1::SynapseRow,
    lola::vx::v1::SynapseWeightMatrix,
    lola::vx::v1::SynapseWeightRow,
    v2::PPUMemory,
    v2::PPUMemoryBlock,
    v2::CapMemBlock,
    lola::vx::v2::AtomicNeuron,
    lola::vx::v2::CADCSampleRow,
    lola::vx::v2::CADCSamples,
    lola::vx::v2::ExternalPPUMemoryBlock,
    lola::vx::v2::SynapseCorrelationCalibRow,
    lola::vx::v2::SynapseCorrelationCalibMatrix,
    lola::vx::v2::SynapseLabelMatrix,
    lola::vx::v2::SynapseLabelRow,
    lola::vx::v2::SynapseMatrix,
    lola::vx::v2::SynapseRow,
    lola::vx::v2::SynapseWeightMatrix,
    lola::vx::v2::SynapseWeightRow,
    v3::PPUMemory,
    v3::PPUMemoryBlock,
    v3::CapMemBlock,
    lola::vx::v3::AtomicNeuron,
    lola::vx::v3::CADCSampleRow,
    lola::vx::v3::CADCSamples,
    lola::vx::v3::ExternalPPUMemoryBlock,
    lola::vx::v3::SynapseCorrelationCalibRow,
    lola::vx::v3::SynapseCorrelationCalibMatrix,
    lola::vx::v3::SynapseLabelMatrix,
    lola::vx::v3::SynapseLabelRow,
    lola::vx::v3::SynapseMatrix,
    lola::vx::v3::SynapseRow,
    lola::vx::v3::SynapseWeightMatrix,
    lola::vx::v3::SynapseWeightRow>
    NonLeafNodeReadableContainerList;

// manually add all non-leaf node containers which are readable
template <typename T>
struct IsReadable<
    T,
    std::enable_if_t<hate::is_in_type_list<T, NonLeafNodeReadableContainerList>::value>>
    : std::true_type
{};

} // namespace haldls::vx::detail
