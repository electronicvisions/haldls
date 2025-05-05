#include <gtest/gtest.h>

#include "haldls/vx/v4/container.h"
#include "haldls/vx/v4/fpga.h"
#include "hate/type_index.h"
#include "hate/type_list.h"
#include "lola/vx/v4/chip.h"
#include "lola/vx/v4/container.h"
#include "stadls/vx/v4/decode.h"
#include "stadls/vx/v4/dumper.h"
#include <random>

TEST(convert_to_chip, General)
{
	std::mt19937 rng(1234);
	lola::vx::v4::Chip previous;
	stadls::vx::decode_random<lola::vx::v4::Chip>(rng, previous);

	{
		auto converted = stadls::vx::v4::convert_to_chip(stadls::vx::v4::DumperDone(), previous);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v4::PlaybackProgramBuilderDumper dumper;
		dumper.write(halco::hicann_dls::vx::v4::ChipOnDLS(), previous);
		auto dumperdone = dumper.done();
		auto converted = stadls::vx::v4::convert_to_chip(dumperdone);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v4::PlaybackProgramBuilderDumper dumper;
		// some Omnibus config, which is not part of the Chip
		dumper.write(
		    halco::hicann_dls::vx::v4::EventRecordingConfigOnFPGA(),
		    haldls::vx::v4::EventRecordingConfig());
		auto dumperdone = dumper.done();
		// expect warning
		auto converted = stadls::vx::v4::convert_to_chip(dumperdone);
		// chip should not have been touched
		EXPECT_EQ(converted, lola::vx::v4::Chip());
	}
}


/**
 * List of containers, which are not covered by lola Chip object either because they are
 * read-only, init-like, trigger-like, because their coordinates are not easily iterable (test
 * shortcoming) or because their content is represented in another haldls container (isolated
 * weight/label/calib bits synapse containers, single ppu memory words/bytes).
 */
typedef hate::type_list<
    haldls::vx::v4::InstructionTimeoutConfig,             // init
    haldls::vx::v4::SystimeCorrectionBarrierConfig,       // init
    haldls::vx::v4::DAC6573ChannelConfig,                 // init
    haldls::vx::v4::AD5252ChannelConfig,                  // init
    haldls::vx::v4::AD5252ChannelConfigPersistent,        // init
    haldls::vx::v4::TCA9554Config,                        // init
    haldls::vx::v4::TCA9554Inputs,                        // init
    haldls::vx::v4::VectorGeneratorFIFOWord,              // trigger
    haldls::vx::v4::VectorGeneratorTrigger,               // trigger
    haldls::vx::v4::VectorGeneratorNotificationAddress,   // not covered
    haldls::vx::v4::VectorGeneratorLUTEntry,              // not covered
    haldls::vx::v4::VectorGeneratorControl,               // not covered
    haldls::vx::v4::INA219Config,                         // init
    haldls::vx::v4::INA219Status,                         // read
    haldls::vx::v4::NullPayloadReadable,                  // read
    haldls::vx::v4::PollingOmnibusBlockConfig,            // trigger
    haldls::vx::v4::NeuronReset,                          // trigger
    haldls::vx::v4::SpikeCounterRead,                     // read
    haldls::vx::v4::SpikeCounterReset,                    // trigger
    haldls::vx::v4::HicannARQStatus,                      // read
    haldls::vx::v4::PhyStatus,                            // read
    haldls::vx::v4::PPUControlRegister,                   // trigger
    haldls::vx::v4::PPUStatusRegister,                    // read
    haldls::vx::v4::ResetChip,                            // init
    haldls::vx::v4::Timer,                                // trigger
    haldls::vx::v4::JTAGClockScaler,                      // init
    haldls::vx::v4::JTAGIdCode,                           // read
    haldls::vx::v4::ResetJTAGTap,                         // init
    haldls::vx::v4::ShiftRegister,                        // init
    haldls::vx::v4::DACChannel,                           // init
    haldls::vx::v4::DACControl,                           // init
    haldls::vx::v4::PADIEvent,                            // trigger
    haldls::vx::v4::ADPLL,                                // init
    haldls::vx::v4::PLLClockOutputBlock,                  // init
    haldls::vx::v4::PLLSelfTest,                          // trigger
    haldls::vx::v4::PLLSelfTestStatus,                    // read
    haldls::vx::v4::PhyConfigFPGA,                        // init
    haldls::vx::v4::CommonPhyConfigFPGA,                  // init
    haldls::vx::v4::PhyConfigChip,                        // init
    haldls::vx::v4::CommonPhyConfigChip,                  // init
    haldls::vx::v4::PerfTest,                             // trigger
    haldls::vx::v4::PerfTestStatus,                       // read
    haldls::vx::v4::SystimeSyncBase,                      // init
    haldls::vx::v4::SynapseWeightQuad,                    // included in SynapseQuad
    haldls::vx::v4::SynapseLabelQuad,                     // included in SynapseQuad
    haldls::vx::v4::SynapseCorrelationCalibQuad,          // included in SynapseQuad
    haldls::vx::v4::CADCSampleQuad,                       // read
    haldls::vx::v4::SpikePack1ToChip,                     // trigger
    haldls::vx::v4::SpikePack2ToChip,                     // trigger
    haldls::vx::v4::SpikePack3ToChip,                     // trigger
    haldls::vx::v4::FPGADeviceDNA,                        // read
    haldls::vx::v4::EventRecordingConfig,                 // trigger
    haldls::vx::v4::CrossbarInputDropCounter,             // read
    haldls::vx::v4::CrossbarOutputEventCounter,           // read
    haldls::vx::v4::GlobalMergerMatrixInputDropCounter,   // read
    haldls::vx::v4::GlobalMergerMatrixOutputEventCounter, // read
    haldls::vx::v4::GlobalMergerMatrixOutputConfig,       // FIXME
    haldls::vx::v4::GlobalMergerMatrixInputDropCounter,   // FIXME
    haldls::vx::v4::GlobalMergerMatrixOutputEventCounter, // FIXME
    haldls::vx::v4::GlobalMergerMatrixNode,               // FIXME
    haldls::vx::v4::NeuronLabelToGL1EventLUTEntry,        // FIXME
    haldls::vx::v4::MADCControl,                          // trigger
    haldls::vx::v4::CorrelationReset,                     // trigger
    haldls::vx::v4::NeuronResetQuad,                      // trigger
    haldls::vx::v4::BlockPostPulse,                       // trigger
    haldls::vx::v4::SystimeSync,                          // trigger
    haldls::vx::v4::PPUMemoryBlock,                       // coordinate not easily iterable
    haldls::vx::v4::PPUMemoryWord,                        // included in PPUMemory
    haldls::vx::v4::ExternalPPUMemoryByte,                // included in ExternalPPUMemory
    haldls::vx::v4::ExternalPPUMemoryQuad,                // included in ExternalPPUMemory
    haldls::vx::v4::ExternalPPUDRAMMemoryByte,            // logically not part of the chip
    haldls::vx::v4::ExternalPPUDRAMMemoryQuad,            // logically not part of the chip
    lola::vx::v4::ExternalPPUDRAMMemoryBlock,             // logically not part of the chip
    haldls::vx::v4::SpikeIOConfig,                        // logically not part of the chip
    haldls::vx::v4::SpikeIOInputRoute,                    // logically not part of the chip
    haldls::vx::v4::SpikeIOOutputRoute,                   // logically not part of the chip
    haldls::vx::v4::FPGASystimeSyncNumRetries,            // logically not part of the chip
    haldls::vx::v4::FPGASystimeSyncLastRTT,               // logically not part of the chip
    haldls::vx::v4::FPGASystimeSyncLastAsicSystime,       // logically not part of the chip
    haldls::vx::v4::FPGASystimeSyncActiveState,           // logically not part of the chip
    lola::vx::v4::LogicalNeuron,                          // FIXME
    lola::vx::v4::ExternalPPUMemoryBlock,                 // coordinate not easily iterable
    lola::vx::v4::DACControlBlock,                        // init
    lola::vx::v4::DACChannelBlock,                        // init
    lola::vx::v4::CADCSampleRow,                          // read
    lola::vx::v4::CADCSamples,                            // read
    lola::vx::v4::SynapseWeightMatrix,                    // included in SynapseMatrix
    lola::vx::v4::SynapseLabelMatrix,                     // included in SynapseMatrix
    lola::vx::v4::SynapseCorrelationCalibMatrix,          // included in SynapseMatrix
    lola::vx::v4::SynapseWeightRow,                       // included in SynapseRow
    lola::vx::v4::SynapseLabelRow,                        // included in SynapseRow
    lola::vx::v4::SynapseCorrelationCalibRow,             // included in SynapseRow
    haldls::vx::v4::SynapseQuad,                          // included in SynapseRow
    lola::vx::v4::CorrelationResetRow>
    ContainersNotCoveredByChip;

template <typename Type>
std::enable_if_t<!hate::is_in_type_list<Type, ContainersNotCoveredByChip>::value>
test_ChipCoverage()
{
	using namespace halco::hicann_dls::vx::v4;
	using namespace haldls::vx::v4;
	std::mt19937 rng(1234);
	lola::vx::v4::Chip chip_default;
	if constexpr (std::is_same<Type, CapMemCell>::value) {
		for (auto const block : halco::common::iter_all<CapMemBlockOnDLS>()) {
			std::vector<CapMemColumnOnCapMemBlock> const column_to_test{
			    CapMemColumnOnCapMemBlock(0), CapMemColumnOnCapMemBlock(128),
			    CapMemColumnOnCapMemBlock(129)};
			for (auto const column : column_to_test) {
				for (auto const row : halco::common::iter_all<CapMemRowOnCapMemBlock>()) {
					CapMemCellOnDLS coord(CapMemCellOnCapMemBlock(column, row), block);
					stadls::vx::v4::PlaybackProgramBuilderDumper dumper;
					CapMemCell config = haldls::vx::detail::coordinate_to_container<
					    CapMemCell::coordinate_type, CapMemCell>(coord);
					auto config_default = config;
					while (config == config_default) {
						stadls::vx::decode_random<CapMemCell>(rng, config);
					}
					dumper.write(coord, config);
					auto const dumperdone = dumper.done();
					auto chip_converted = stadls::vx::v4::convert_to_chip(dumperdone);
					EXPECT_NE(chip_converted, chip_default) << coord;
				}
			}
		}
	} else {
		for (auto const coord : halco::common::iter_all<typename Type::coordinate_type>()) {
			stadls::vx::v4::PlaybackProgramBuilderDumper dumper;
			Type config =
			    haldls::vx::detail::coordinate_to_container<typename Type::coordinate_type, Type>(
			        coord);
			auto config_default = config;
			while (config == config_default) {
				stadls::vx::decode_random<Type>(rng, config);
			}
			dumper.write(coord, config);
			auto const dumperdone = dumper.done();
			auto chip_converted = stadls::vx::v4::convert_to_chip(dumperdone);
			EXPECT_NE(chip_converted, chip_default) << coord;
		}
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
#include "haldls/vx/v4/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v4/container.def"
