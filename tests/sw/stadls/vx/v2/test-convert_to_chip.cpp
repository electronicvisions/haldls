#include <gtest/gtest.h>

#include "haldls/vx/v2/container.h"
#include "haldls/vx/v2/fpga.h"
#include "hate/type_index.h"
#include "hate/type_list.h"
#include "lola/vx/v2/chip.h"
#include "lola/vx/v2/container.h"
#include "stadls/vx/v2/decode.h"
#include "stadls/vx/v2/dumper.h"
#include <random>

TEST(convert_to_chip, General)
{
	std::mt19937 rng(1234);
	lola::vx::v2::Chip previous;
	stadls::vx::decode_random<lola::vx::v2::Chip>(rng, previous);

	{
		auto converted = stadls::vx::v2::convert_to_chip(stadls::vx::v2::DumperDone(), previous);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v2::PlaybackProgramBuilderDumper dumper;
		dumper.write(halco::hicann_dls::vx::v2::ChipOnDLS(), previous);
		auto dumperdone = dumper.done();
		auto converted = stadls::vx::v2::convert_to_chip(dumperdone);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v2::PlaybackProgramBuilderDumper dumper;
		// some Omnibus config, which is not part of the Chip
		dumper.write(
		    halco::hicann_dls::vx::v2::EventRecordingConfigOnFPGA(),
		    haldls::vx::v2::EventRecordingConfig());
		auto dumperdone = dumper.done();
		// expect warning
		auto converted = stadls::vx::v2::convert_to_chip(dumperdone);
		// chip should not have been touched
		EXPECT_EQ(converted, lola::vx::v2::Chip());
	}
}


/**
 * List of containers, which are not covered by lola Chip object either because they are
 * read-only, init-like, trigger-like, because their coordinates are not easily iterable (test
 * shortcoming) or because their content is represented in another haldls container (isolated
 * weight/label/calib bits synapse containers, single ppu memory words/bytes).
 */
typedef hate::type_list<
    haldls::vx::v2::InstructionTimeoutConfig,                // init
    haldls::vx::v2::DAC6573ChannelConfig,                    // init
    haldls::vx::v2::AD5252ChannelConfig,                     // init
    haldls::vx::v2::AD5252ChannelConfigPersistent,           // init
    haldls::vx::v2::TCA9554Config,                           // init
    haldls::vx::v2::TCA9554Inputs,                           // init
    haldls::vx::v2::VectorGeneratorFIFOWord,                 // trigger
    haldls::vx::v2::VectorGeneratorTrigger,                  // trigger
    haldls::vx::v2::VectorGeneratorNotificationAddress,      // not covered
    haldls::vx::v2::VectorGeneratorLUTEntry,                 // not covered
    haldls::vx::v2::VectorGeneratorControl,                  // not covered
    haldls::vx::v2::INA219Config,                            // init
    haldls::vx::v2::INA219Status,                            // read
    haldls::vx::v2::NullPayloadReadable,                     // read
    haldls::vx::v2::PollingOmnibusBlockConfig,               // trigger
    haldls::vx::v2::NeuronReset,                             // trigger
    haldls::vx::v2::SpikeCounterRead,                        // read
    haldls::vx::v2::SpikeCounterReset,                       // trigger
    haldls::vx::v2::HicannARQStatus,                         // read
    haldls::vx::v2::PhyStatus,                               // read
    haldls::vx::v2::PPUControlRegister,                      // trigger
    haldls::vx::v2::PPUStatusRegister,                       // read
    haldls::vx::v2::ResetChip,                               // init
    haldls::vx::v2::Timer,                                   // trigger
    haldls::vx::v2::JTAGClockScaler,                         // init
    haldls::vx::v2::JTAGIdCode,                              // read
    haldls::vx::v2::ResetJTAGTap,                            // init
    haldls::vx::v2::ShiftRegister,                           // init
    haldls::vx::v2::DACChannel,                              // init
    haldls::vx::v2::DACControl,                              // init
    haldls::vx::v2::PADIEvent,                               // trigger
    haldls::vx::v2::ADPLL,                                   // init
    haldls::vx::v2::PLLClockOutputBlock,                     // init
    haldls::vx::v2::PLLSelfTest,                             // trigger
    haldls::vx::v2::PLLSelfTestStatus,                       // read
    haldls::vx::v2::PhyConfigFPGA,                           // init
    haldls::vx::v2::CommonPhyConfigFPGA,                     // init
    haldls::vx::v2::PhyConfigChip,                           // init
    haldls::vx::v2::CommonPhyConfigChip,                     // init
    haldls::vx::v2::PerfTest,                                // trigger
    haldls::vx::v2::PerfTestStatus,                          // read
    haldls::vx::v2::SystimeSyncBase,                         // init
    haldls::vx::v2::SynapseWeightQuad,                       // included in SynapseQuad
    haldls::vx::v2::SynapseLabelQuad,                        // included in SynapseQuad
    haldls::vx::v2::SynapseCorrelationCalibQuad,             // included in SynapseQuad
    haldls::vx::v2::CADCSampleQuad,                          // read
    haldls::vx::v2::SpikePack1ToChip,                        // trigger
    haldls::vx::v2::SpikePack2ToChip,                        // trigger
    haldls::vx::v2::SpikePack3ToChip,                        // trigger
    haldls::vx::v2::FPGADeviceDNA,                           // read
    haldls::vx::v2::EventRecordingConfig,                    // trigger
    haldls::vx::v2::CrossbarInputDropCounter,                // read
    haldls::vx::v2::CrossbarOutputEventCounter,              // read
    haldls::vx::v2::MADCControl,                             // trigger
    haldls::vx::v2::CorrelationReset,                        // trigger
    haldls::vx::v2::NeuronResetQuad,                         // trigger
    haldls::vx::v2::BlockPostPulse,                          // trigger
    haldls::vx::v2::SystimeSync,                             // trigger
    haldls::vx::v2::PPUMemoryBlock,                          // coordinate not easily iterable
    haldls::vx::v2::PPUMemoryWord,                           // included in PPUMemory
    haldls::vx::v2::ExternalPPUMemoryByte,                   // included in ExternalPPUMemory
    haldls::vx::v2::ExternalPPUMemoryQuad,                   // included in ExternalPPUMemory
    haldls::vx::v2::SpikeIOConfig,                           // logically not part of the chip
    haldls::vx::v2::SpikeIOInputRoute,                       // logically not part of the chip
    haldls::vx::v2::SpikeIOOutputRoute,                      // logically not part of the chip
    haldls::vx::v2::EventSwitchSource,                       // logically not part of the chip
    haldls::vx::v2::EventSwitchReadout,                      // logically not part of the chip
    haldls::vx::v2::EventSwitchConfig,                       // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommBucketTriggerConfig,      // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommBucketDestinationConfig,  // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommBucketNumPktsSent,        // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommBucketCounterReset,       // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommRouterLookupConfig,       // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommRouterConfig,             // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommRouterEventLossDisabled,  // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommRouterEventLossMisconf,   // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommRouterEventsRouted,       // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommRouterCounterReset,       // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommDecoderNumEventsReceived, // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommDecoderCounterReset,      // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommTimestampDelayConfig,     // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommTimestampDelayNumEventsReceived, // logically not part of the
                                                                    // chip
    haldls::vx::v2::ExtollSpikeCommTimestampDelayEventLossFull,    // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommTimestampDelayEventLossExpired, // logically not part of the chip
    haldls::vx::v2::ExtollSpikeCommTimestampDelayCounterReset,     // logically not part of the chip
    haldls::vx::v2::ExtollBarrierTriggerReached,                   // logically not part of the chip
    haldls::vx::v2::ExtollBarrierReleased,                         // logically not part of the chip
    haldls::vx::v2::ExtollBarrierConfig,                           // logically not part of the chip
    haldls::vx::v2::ExtollInterruptControl,                        // logically not part of the chip
    haldls::vx::v2::ExtollInterruptConfig,                         // logically not part of the chip
    haldls::vx::v2::ExtollBarrierInterruptInportErrorCount,        // logically not part of the chip
    haldls::vx::v2::ExtollBarrierInterruptInportCounterReset,      // logically not part of the chip
    haldls::vx::v2::FPGASystimeSyncNumRetries,                     // logically not part of the chip
    haldls::vx::v2::FPGASystimeSyncLastRTT,                        // logically not part of the chip
    haldls::vx::v2::FPGASystimeSyncLastAsicSystime,                // logically not part of the chip
    haldls::vx::v2::FPGASystimeSyncActiveState,                    // logically not part of the chip
    lola::vx::v2::LogicalNeuron,                                   // FIXME
    lola::vx::v2::ExternalPPUMemoryBlock,                          // coordinate not easily iterable
    lola::vx::v2::DACControlBlock,                                 // init
    lola::vx::v2::DACChannelBlock,                                 // init
    lola::vx::v2::CADCSampleRow,                                   // read
    lola::vx::v2::CADCSamples,                                     // read
    lola::vx::v2::SynapseWeightMatrix,                             // included in SynapseMatrix
    lola::vx::v2::SynapseLabelMatrix,                              // included in SynapseMatrix
    lola::vx::v2::SynapseCorrelationCalibMatrix,                   // included in SynapseMatrix
    lola::vx::v2::SynapseWeightRow,                                // included in SynapseRow
    lola::vx::v2::SynapseLabelRow,                                 // included in SynapseRow
    lola::vx::v2::SynapseCorrelationCalibRow,                      // included in SynapseRow
    haldls::vx::v2::SynapseQuad,                                   // included in SynapseRow
    haldls::vx::v2::CapMemCell,                                    // FIXME
    lola::vx::v2::CorrelationResetRow>
    ContainersNotCoveredByChip;

template <typename Type>
std::enable_if_t<!hate::is_in_type_list<Type, ContainersNotCoveredByChip>::value>
test_ChipCoverage()
{
	static_assert(!hate::is_in_type_list<Type, ContainersNotCoveredByChip>::value);
	std::mt19937 rng(1234);
	lola::vx::v2::Chip chip_default;
	for (auto const coord : halco::common::iter_all<typename Type::coordinate_type>()) {
		stadls::vx::v2::PlaybackProgramBuilderDumper dumper;
		Type config =
		    haldls::vx::detail::coordinate_to_container<typename Type::coordinate_type, Type>(
		        coord);
		auto config_default = config;
		while (config == config_default) {
			stadls::vx::decode_random<Type>(rng, config);
		}
		dumper.write(coord, config);
		auto const dumperdone = dumper.done();
		auto chip_converted = stadls::vx::v2::convert_to_chip(dumperdone);
		EXPECT_NE(chip_converted, chip_default) << coord;
	}
}

template <typename Type>
std::enable_if_t<hate::is_in_type_list<Type, ContainersNotCoveredByChip>::value> test_ChipCoverage()
{
	GTEST_SKIP();
}

/**
 * Ensure coverage of lola Chip object over haldls containers.
 */
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	TEST(Name, lola_vx_Chip_Coverage)                                                              \
	{                                                                                              \
		test_ChipCoverage<Type>();                                                                 \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"
