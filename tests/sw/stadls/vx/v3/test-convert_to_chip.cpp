#include <gtest/gtest.h>

#include "haldls/vx/v3/container.h"
#include "haldls/vx/v3/fpga.h"
#include "hate/type_index.h"
#include "hate/type_list.h"
#include "lola/vx/v3/chip.h"
#include "lola/vx/v3/container.h"
#include "stadls/vx/v3/decode.h"
#include "stadls/vx/v3/dumper.h"
#include <random>

TEST(convert_to_chip, General)
{
	std::mt19937 rng(1234);
	lola::vx::v3::Chip previous;
	stadls::vx::decode_random<lola::vx::v3::Chip>(rng, previous);

	{
		auto converted = stadls::vx::v3::convert_to_chip(stadls::vx::v3::DumperDone(), previous);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v3::PlaybackProgramBuilderDumper dumper;
		dumper.write(halco::hicann_dls::vx::v3::ChipOnDLS(), previous);
		auto dumperdone = dumper.done();
		auto converted = stadls::vx::v3::convert_to_chip(dumperdone);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v3::PlaybackProgramBuilderDumper dumper;
		// some Omnibus config, which is not part of the Chip
		dumper.write(
		    halco::hicann_dls::vx::v3::EventRecordingConfigOnFPGA(),
		    haldls::vx::v3::EventRecordingConfig());
		auto dumperdone = dumper.done();
		// expect warning
		auto converted = stadls::vx::v3::convert_to_chip(dumperdone);
		// chip should not have been touched
		EXPECT_EQ(converted, lola::vx::v3::Chip());
	}
}


/**
 * List of containers, which are not covered by lola Chip object either because they are
 * read-only, init-like, trigger-like, because their coordinates are not easily iterable (test
 * shortcoming) or because their content is represented in another haldls container (isolated
 * weight/label/calib bits synapse containers, single ppu memory words/bytes).
 */
typedef hate::type_list<
    haldls::vx::v3::InstructionTimeoutConfig,           // init
    haldls::vx::v3::DAC6573ChannelConfig,               // init
    haldls::vx::v3::AD5252ChannelConfig,                // init
    haldls::vx::v3::AD5252ChannelConfigPersistent,      // init
    haldls::vx::v3::TCA9554Config,                      // init
    haldls::vx::v3::TCA9554Inputs,                      // init
    haldls::vx::v3::VectorGeneratorFIFOWord,            // trigger
    haldls::vx::v3::VectorGeneratorTrigger,             // trigger
    haldls::vx::v3::VectorGeneratorNotificationAddress, // not covered
    haldls::vx::v3::VectorGeneratorLUTEntry,            // not covered
    haldls::vx::v3::VectorGeneratorControl,             // not covered
    haldls::vx::v3::INA219Config,                       // init
    haldls::vx::v3::INA219Status,                       // read
    haldls::vx::v3::NullPayloadReadable,                // read
    haldls::vx::v3::PollingOmnibusBlockConfig,          // trigger
    haldls::vx::v3::NeuronReset,                        // trigger
    haldls::vx::v3::SpikeCounterRead,                   // read
    haldls::vx::v3::SpikeCounterReset,                  // trigger
    haldls::vx::v3::HicannARQStatus,                    // read
    haldls::vx::v3::PhyStatus,                          // read
    haldls::vx::v3::PPUControlRegister,                 // trigger
    haldls::vx::v3::PPUStatusRegister,                  // read
    haldls::vx::v3::ResetChip,                          // init
    haldls::vx::v3::Timer,                              // trigger
    haldls::vx::v3::JTAGClockScaler,                    // init
    haldls::vx::v3::JTAGIdCode,                         // read
    haldls::vx::v3::ResetJTAGTap,                       // init
    haldls::vx::v3::ShiftRegister,                      // init
    haldls::vx::v3::DACChannel,                         // init
    haldls::vx::v3::DACControl,                         // init
    haldls::vx::v3::PADIEvent,                          // trigger
    haldls::vx::v3::ADPLL,                              // init
    haldls::vx::v3::PLLClockOutputBlock,                // init
    haldls::vx::v3::PLLSelfTest,                        // trigger
    haldls::vx::v3::PLLSelfTestStatus,                  // read
    haldls::vx::v3::PhyConfigFPGA,                      // init
    haldls::vx::v3::CommonPhyConfigFPGA,                // init
    haldls::vx::v3::PhyConfigChip,                      // init
    haldls::vx::v3::CommonPhyConfigChip,                // init
    haldls::vx::v3::PerfTest,                           // trigger
    haldls::vx::v3::PerfTestStatus,                     // read
    haldls::vx::v3::SystimeSyncBase,                    // init
    haldls::vx::v3::SynapseWeightQuad,                  // included in SynapseQuad
    haldls::vx::v3::SynapseLabelQuad,                   // included in SynapseQuad
    haldls::vx::v3::SynapseCorrelationCalibQuad,        // included in SynapseQuad
    haldls::vx::v3::CADCSampleQuad,                     // read
    haldls::vx::v3::SpikePack1ToChip,                   // trigger
    haldls::vx::v3::SpikePack2ToChip,                   // trigger
    haldls::vx::v3::SpikePack3ToChip,                   // trigger
    haldls::vx::v3::FPGADeviceDNA,                      // read
    haldls::vx::v3::EventRecordingConfig,               // trigger
    haldls::vx::v3::CrossbarInputDropCounter,           // read
    haldls::vx::v3::CrossbarOutputEventCounter,         // read
    haldls::vx::v3::MADCControl,                        // trigger
    haldls::vx::v3::CorrelationReset,                   // trigger
    haldls::vx::v3::NeuronResetQuad,                    // trigger
    haldls::vx::v3::BlockPostPulse,                     // trigger
    haldls::vx::v3::SystimeSync,                        // trigger
    haldls::vx::v3::PPUMemoryBlock,                     // coordinate not easily iterable
    haldls::vx::v3::PPUMemoryWord,                      // included in PPUMemory
    haldls::vx::v3::ExternalPPUMemoryByte,              // included in ExternalPPUMemory
    haldls::vx::v3::ExternalPPUMemoryQuad,              // included in ExternalPPUMemory
    lola::vx::v3::ExternalPPUMemoryBlock,               // coordinate not easily iterable
    lola::vx::v3::DACControlBlock,                      // init
    lola::vx::v3::DACChannelBlock,                      // init
    lola::vx::v3::CADCSampleRow,                        // read
    lola::vx::v3::CADCSamples,                          // read
    lola::vx::v3::SynapseWeightMatrix,                  // included in SynapseMatrix
    lola::vx::v3::SynapseLabelMatrix,                   // included in SynapseMatrix
    lola::vx::v3::SynapseCorrelationCalibMatrix,        // included in SynapseMatrix
    lola::vx::v3::SynapseWeightRow,                     // included in SynapseRow
    lola::vx::v3::SynapseLabelRow,                      // included in SynapseRow
    lola::vx::v3::SynapseCorrelationCalibRow,           // included in SynapseRow
    haldls::vx::v3::SynapseQuad,                        // included in SynapseRow
    lola::vx::v3::CorrelationResetRow>
    ContainersNotCoveredByChip;

/**
 * Ensure coverage of lola Chip object over haldls containers.
 */
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	TEST(Name, lola_vx_Chip_Coverage)                                                              \
	{                                                                                              \
		using namespace halco::hicann_dls::vx::v3;                                                 \
		using namespace haldls::vx::v3;                                                            \
		if constexpr (!hate::is_in_type_list<Type, ContainersNotCoveredByChip>::value) {           \
			std::mt19937 rng(1234);                                                                \
			lola::vx::v3::Chip chip_default;                                                       \
			if constexpr (std::is_same<Type, CapMemCell>::value) {                                 \
				for (auto const block : halco::common::iter_all<CapMemBlockOnDLS>()) {             \
					std::vector<CapMemColumnOnCapMemBlock> const column_to_test{                   \
					    CapMemColumnOnCapMemBlock(0), CapMemColumnOnCapMemBlock(128),              \
					    CapMemColumnOnCapMemBlock(129)};                                           \
					for (auto const column : column_to_test) {                                     \
						for (auto const row : halco::common::iter_all<CapMemRowOnCapMemBlock>()) { \
							CapMemCellOnDLS coord(CapMemCellOnCapMemBlock(column, row), block);    \
							stadls::vx::v3::PlaybackProgramBuilderDumper dumper;                   \
							CapMemCell config = haldls::vx::detail::coordinate_to_container<       \
							    CapMemCell::coordinate_type, CapMemCell>(coord);                   \
							auto config_default = config;                                          \
							while (config == config_default) {                                     \
								stadls::vx::decode_random<CapMemCell>(rng, config);                \
							}                                                                      \
							dumper.write(coord, config);                                           \
							auto const dumperdone = dumper.done();                                 \
							auto chip_converted = stadls::vx::v3::convert_to_chip(dumperdone);     \
							EXPECT_NE(chip_converted, chip_default) << coord;                      \
						}                                                                          \
					}                                                                              \
				}                                                                                  \
			} else {                                                                               \
				for (auto const coord : halco::common::iter_all<Type::coordinate_type>()) {        \
					stadls::vx::v3::PlaybackProgramBuilderDumper dumper;                           \
					Type config =                                                                  \
					    haldls::vx::detail::coordinate_to_container<Type::coordinate_type, Type>(  \
					        coord);                                                                \
					auto config_default = config;                                                  \
					while (config == config_default) {                                             \
						stadls::vx::decode_random<Type>(rng, config);                              \
					}                                                                              \
					dumper.write(coord, config);                                                   \
					auto const dumperdone = dumper.done();                                         \
					auto chip_converted = stadls::vx::v3::convert_to_chip(dumperdone);             \
					EXPECT_NE(chip_converted, chip_default) << coord;                              \
				}                                                                                  \
			}                                                                                      \
		}                                                                                          \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v3/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v3/container.def"
