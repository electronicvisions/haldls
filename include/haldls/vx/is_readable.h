#pragma once
#include "fisch/vx/traits.h"
#include "haldls/vx/container.h"
#include "hate/type_list.h"
#include "lola/vx/container.h"

namespace haldls::vx::detail {

typedef hate::type_list<
    ADPLL,
    PLLSelfTest,
    PLLClockOutputBlock,
    SpikeCounterRead,
    HicannARQStatus,
    PhyStatus,
    PPUMemoryWord,
    PPUMemoryBlock,
    PPUControlRegister,
    PPUStatusRegister,
    JTAGIdCode,
    CapMemCell,
    CapMemBlock,
    CommonNeuronBackendConfig,
    CommonPhyConfigFPGA,
    PLLSelfTestStatus,
    PerfTest,
    PerfTestStatus,
    SystimeSyncBase,
    CommonSynramConfig,
    SynapseQuad,
    ColumnCorrelationQuad,
    ColumnCurrentQuad,
    CADCConfig,
    CADCChannelConfig,
    CADCSampleQuad,
    FPGADeviceDNA,
    EventRecordingConfig,
    ReferenceGeneratorConfig,
    PadMultiplexerConfig,
    ReadoutSourceSelection,
    lola::vx::CADCSampleRow,
    lola::vx::SynapseMatrix,
    lola::vx::SynapseRow,
    MADCConfig>
    ReadableContainerList;

typedef hate::type_list<
    CapMemBlockConfig,
    CommonPADIBusConfig,
    BackgroundSpikeSource,
    CrossbarOutputConfig,
    CrossbarNode,
    CrossbarInputDropCounter,
    CrossbarOutputEventCounter>
    OnlyWithSimulationReadbackReadableContainerList;

typedef hate::
    type_list<NeuronConfig, NeuronBackendConfig, PhyConfigFPGA, PhyConfigChip, SynapseDriverConfig>
        OnlyWithHardwareReadableContainerList;

/**
 * Get if container type is readable on hardware.
 * @tparam ContainerT Type of container
 * @tparam BackendT Type of backend
 * @return Boolean value
 */
template <typename ContainerT, typename BackendT>
constexpr bool is_hardware_readable()
{
	if constexpr (!fisch::vx::IsReadable<BackendT>::value) {
		return false;
	}
	if constexpr (hate::is_in_type_list<ContainerT, ReadableContainerList>::value) {
		return true;
	}
	if constexpr (hate::is_in_type_list<ContainerT, OnlyWithHardwareReadableContainerList>::value) {
		return true;
	}
	return false;
}

/**
 * Get if container type is readable in simulation.
 * @tparam ContainerT Type of container
 * @tparam BackendT Type of backend
 * @return Boolean value
 */
template <typename ContainerT, typename BackendT>
constexpr bool is_simulation_readable()
{
	if constexpr (!fisch::vx::IsReadable<BackendT>::value) {
		return false;
	}
	if constexpr (hate::is_in_type_list<ContainerT, ReadableContainerList>::value) {
		return true;
	}
	if constexpr (hate::is_in_type_list<
	                  ContainerT, OnlyWithSimulationReadbackReadableContainerList>::value) {
		return true;
	}
	return false;
}

} // namespace haldls::vx::detail
