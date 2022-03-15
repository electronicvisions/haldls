#include <gtest/gtest.h>

#include "haldls/vx/v1/container.h"
#include "haldls/vx/v1/fpga.h"
#include "hate/type_index.h"
#include "hate/type_list.h"
#include "lola/vx/v1/chip.h"
#include "lola/vx/v1/container.h"
#include "stadls/vx/v1/decode.h"
#include "stadls/vx/v1/dumper.h"
#include <random>

TEST(convert_to_chip, General)
{
	std::mt19937 rng(1234);
	lola::vx::v1::Chip previous;
	stadls::vx::decode_random<lola::vx::v1::Chip>(rng, previous);

	{
		auto converted = stadls::vx::v1::convert_to_chip(stadls::vx::v1::DumperDone(), previous);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v1::PlaybackProgramBuilderDumper dumper;
		dumper.write(halco::hicann_dls::vx::v1::ChipOnDLS(), previous);
		auto dumperdone = dumper.done();
		auto converted = stadls::vx::v1::convert_to_chip(dumperdone);
		EXPECT_EQ(converted, previous);
	}
	{
		stadls::vx::v1::PlaybackProgramBuilderDumper dumper;
		// some Omnibus config, which is not part of the Chip
		dumper.write(
		    halco::hicann_dls::vx::v1::EventRecordingConfigOnFPGA(),
		    haldls::vx::v1::EventRecordingConfig());
		auto dumperdone = dumper.done();
		// expect warning
		auto converted = stadls::vx::v1::convert_to_chip(dumperdone);
		// chip should not have been touched
		EXPECT_EQ(converted, lola::vx::v1::Chip());
	}
}


/**
 * List of containers, which are not covered by lola Chip object either because they are
 * read-only, init-like, trigger-like, because their coordinates are not easily iterable (test
 * shortcoming) or because their content is represented in another haldls container (isolated
 * weight/label/calib bits synapse containers, single ppu memory words/bytes).
 */
typedef hate::type_list<
    haldls::vx::v1::DAC6573ChannelConfig,               // init
    haldls::vx::v1::AD5252ChannelConfig,                // init
    haldls::vx::v1::AD5252ChannelConfigPersistent,      // init
    haldls::vx::v1::TCA9554Config,                      // init
    haldls::vx::v1::TCA9554Inputs,                      // init
    haldls::vx::v1::VectorGeneratorFIFOWord,            // trigger
    haldls::vx::v1::VectorGeneratorTrigger,             // trigger
    haldls::vx::v1::VectorGeneratorNotificationAddress, // not covered
    haldls::vx::v1::VectorGeneratorLUTEntry,            // not covered
    haldls::vx::v1::VectorGeneratorControl,             // not covered
    haldls::vx::v1::INA219Config,                       // init
    haldls::vx::v1::INA219Status,                       // read
    haldls::vx::v1::NullPayloadReadable,                // read
    haldls::vx::v1::PollingOmnibusBlockConfig,          // trigger
    haldls::vx::v1::NeuronReset,                        // trigger
    haldls::vx::v1::SpikeCounterRead,                   // read
    haldls::vx::v1::SpikeCounterReset,                  // trigger
    haldls::vx::v1::HicannARQStatus,                    // read
    haldls::vx::v1::PhyStatus,                          // read
    haldls::vx::v1::PPUControlRegister,                 // trigger
    haldls::vx::v1::PPUStatusRegister,                  // read
    haldls::vx::v1::ResetChip,                          // init
    haldls::vx::v1::Timer,                              // trigger
    haldls::vx::v1::JTAGClockScaler,                    // init
    haldls::vx::v1::JTAGIdCode,                         // read
    haldls::vx::v1::ResetJTAGTap,                       // init
    haldls::vx::v1::ShiftRegister,                      // init
    haldls::vx::v1::DACChannel,                         // init
    haldls::vx::v1::DACControl,                         // init
    haldls::vx::v1::PADIEvent,                          // trigger
    haldls::vx::v1::ADPLL,                              // init
    haldls::vx::v1::PLLClockOutputBlock,                // init
    haldls::vx::v1::PLLSelfTest,                        // trigger
    haldls::vx::v1::PLLSelfTestStatus,                  // read
    haldls::vx::v1::PhyConfigFPGA,                      // init
    haldls::vx::v1::CommonPhyConfigFPGA,                // init
    haldls::vx::v1::PhyConfigChip,                      // init
    haldls::vx::v1::CommonPhyConfigChip,                // init
    haldls::vx::v1::PerfTest,                           // trigger
    haldls::vx::v1::PerfTestStatus,                     // read
    haldls::vx::v1::SystimeSyncBase,                    // init
    haldls::vx::v1::SynapseWeightQuad,                  // included in SynapseQuad
    haldls::vx::v1::SynapseLabelQuad,                   // included in SynapseQuad
    haldls::vx::v1::SynapseCorrelationCalibQuad,        // included in SynapseQuad
    haldls::vx::v1::CADCSampleQuad,                     // read
    haldls::vx::v1::SpikePack1ToChip,                   // trigger
    haldls::vx::v1::SpikePack2ToChip,                   // trigger
    haldls::vx::v1::SpikePack3ToChip,                   // trigger
    haldls::vx::v1::FPGADeviceDNA,                      // read
    haldls::vx::v1::EventRecordingConfig,               // trigger
    haldls::vx::v1::CrossbarInputDropCounter,           // read
    haldls::vx::v1::CrossbarOutputEventCounter,         // read
    haldls::vx::v1::MADCControl,                        // trigger
    haldls::vx::v1::CorrelationReset,                   // trigger
    haldls::vx::v1::NeuronResetQuad,                    // trigger
    haldls::vx::v1::BlockPostPulse,                     // trigger
    haldls::vx::v1::SystimeSync,                        // trigger
    haldls::vx::v1::PPUMemoryBlock,                     // coordinate not easily iterable
    haldls::vx::v1::PPUMemoryWord,                      // included in PPUMemory
    haldls::vx::v1::ExternalPPUMemoryByte,              // included in ExternalPPUMemory
    haldls::vx::v1::ExternalPPUMemoryQuad,              // included in ExternalPPUMemory
    lola::vx::v1::ExternalPPUMemoryBlock,               // coordinate not easily iterable
    lola::vx::v1::DACControlBlock,                      // init
    lola::vx::v1::DACChannelBlock,                      // init
    lola::vx::v1::CADCSampleRow,                        // read
    lola::vx::v1::CADCSamples,                          // read
    lola::vx::v1::SynapseWeightMatrix,                  // included in SynapseMatrix
    lola::vx::v1::SynapseLabelMatrix,                   // included in SynapseMatrix
    lola::vx::v1::SynapseCorrelationCalibMatrix,        // included in SynapseMatrix
    lola::vx::v1::SynapseWeightRow,                     // included in SynapseRow
    lola::vx::v1::SynapseLabelRow,                      // included in SynapseRow
    lola::vx::v1::SynapseCorrelationCalibRow,           // included in SynapseRow
    haldls::vx::v1::SynapseQuad,                        // included in SynapseRow
    lola::vx::v1::CorrelationResetRow>
    ContainersNotCoveredByChip;

template <typename Type>
void apply_fixes(Type& config)
{
	if constexpr (std::is_same_v<Type, haldls::vx::v1::NeuronConfig>) {
		// lola::vx::v1::AtomicNeuron can only represent this state
		config.set_enable_adaptation_readout(
		    config.get_readout_source() == haldls::vx::v1::NeuronConfig::ReadoutSource::adaptation);
	}
}

/**
 * Ensure coverage of lola Chip object over haldls containers.
 */
#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	TEST(Name, lola_vx_Chip_Coverage)                                                              \
	{                                                                                              \
		if constexpr (!hate::is_in_type_list<Type, ContainersNotCoveredByChip>::value) {           \
			std::mt19937 rng(1234);                                                                \
			lola::vx::v1::Chip chip_default;                                                       \
			for (auto const coord : halco::common::iter_all<Type::coordinate_type>()) {            \
				stadls::vx::v1::PlaybackProgramBuilderDumper dumper;                               \
				Type config =                                                                      \
				    haldls::vx::detail::coordinate_to_container<Type::coordinate_type, Type>(      \
				        coord);                                                                    \
				auto config_default = config;                                                      \
				while (config == config_default) {                                                 \
					stadls::vx::decode_random<Type>(rng, config);                                  \
				}                                                                                  \
				apply_fixes(config);                                                               \
				dumper.write(coord, config);                                                       \
				auto const dumperdone = dumper.done();                                             \
				auto chip_converted = stadls::vx::v1::convert_to_chip(dumperdone);                 \
				EXPECT_NE(chip_converted, chip_default) << coord;                                  \
			}                                                                                      \
		}                                                                                          \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"
