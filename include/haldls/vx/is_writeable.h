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

namespace halco::hicann_dls::vx::v3 {
struct Coordinates;
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
class DACChannelBlock;
class DACControlBlock;
} // namespace lola::vx

namespace lola::vx::v1 {
class AtomicNeuron;
class CADCSampleRow;
class CADCSamples;
class CorrelationResetRow;
using DACChannelBlock = lola::vx::DACChannelBlock;
using DACControlBlock = lola::vx::DACControlBlock;
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
class CorrelationResetRow;
using DACChannelBlock = lola::vx::DACChannelBlock;
using DACControlBlock = lola::vx::DACControlBlock;
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
class CorrelationResetRow;
using DACChannelBlock = lola::vx::DACChannelBlock;
using DACControlBlock = lola::vx::DACControlBlock;
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
    v1::PPUMemoryBlock,
    v1::PPUMemory,
    v1::CapMemBlock,
    lola::vx::v1::AtomicNeuron,
    lola::vx::v1::CorrelationResetRow,
    lola::vx::v1::DACChannelBlock,
    lola::vx::v1::DACControlBlock,
    lola::vx::v1::ExternalPPUMemoryBlock,
    lola::vx::v1::SynapseRow,
    lola::vx::v1::SynapseMatrix,
    lola::vx::v1::SynapseWeightMatrix,
    lola::vx::v1::SynapseLabelMatrix,
    lola::vx::v1::SynapseCorrelationCalibMatrix,
    lola::vx::v1::SynapseWeightRow,
    lola::vx::v1::SynapseLabelRow,
    lola::vx::v1::SynapseCorrelationCalibRow,
    v2::PPUMemoryBlock,
    v2::PPUMemory,
    v2::CapMemBlock,
    lola::vx::v2::AtomicNeuron,
    lola::vx::v2::CorrelationResetRow,
    lola::vx::v2::DACChannelBlock,
    lola::vx::v2::DACControlBlock,
    lola::vx::v2::ExternalPPUMemoryBlock,
    lola::vx::v2::SynapseRow,
    lola::vx::v2::SynapseMatrix,
    lola::vx::v2::SynapseWeightMatrix,
    lola::vx::v2::SynapseLabelMatrix,
    lola::vx::v2::SynapseCorrelationCalibMatrix,
    lola::vx::v2::SynapseWeightRow,
    lola::vx::v2::SynapseLabelRow,
    lola::vx::v2::SynapseCorrelationCalibRow,
    v3::PPUMemoryBlock,
    v3::PPUMemory,
    v3::CapMemBlock,
    lola::vx::v3::AtomicNeuron,
    lola::vx::v3::CorrelationResetRow,
    lola::vx::v3::DACChannelBlock,
    lola::vx::v3::DACControlBlock,
    lola::vx::v3::ExternalPPUMemoryBlock,
    lola::vx::v3::SynapseRow,
    lola::vx::v3::SynapseMatrix,
    lola::vx::v3::SynapseWeightMatrix,
    lola::vx::v3::SynapseLabelMatrix,
    lola::vx::v3::SynapseCorrelationCalibMatrix,
    lola::vx::v3::SynapseWeightRow,
    lola::vx::v3::SynapseLabelRow,
    lola::vx::v3::SynapseCorrelationCalibRow>
    NonLeafNodeWriteableContainerList;

// manually add all non-leaf node containers which are writeable
template <typename T>
struct IsWriteable<
    T,
    std::enable_if_t<hate::is_in_type_list<T, NonLeafNodeWriteableContainerList>::value>>
    : std::true_type
{};

} // namespace haldls::vx::detail
