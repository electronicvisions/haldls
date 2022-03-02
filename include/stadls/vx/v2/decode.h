#pragma once
#define CHIP_REVISION 2
#include "stadls/vx/decode.h"
#undef CHIP_REVISION

#include "lola/vx/v2/chip.h"

namespace stadls::vx {

// specializations

template <>
inline void decode_random(std::mt19937& gen, lola::vx::v2::Chip& config)
{
	using namespace halco::hicann_dls::vx::v2;
	using namespace halco::common;

	decode_random(gen, config.memory_timing);
	decode_random(gen, config.crossbar);

	for (auto const c : iter_all<BackgroundSpikeSourceOnDLS>()) {
		decode_random(gen, config.background_spike_sources[c]);
	}

	decode_random(gen, config.capmem);

	for (auto const c : iter_all<SynapseDriverBlockOnDLS>()) {
		decode_random(gen, config.synapse_driver_blocks[c]);
	}

	for (auto const c : iter_all<SynapseBlockOnDLS>()) {
		decode_random(gen, config.synapse_blocks[c]);
	}

	decode_random(gen, config.neuron_block);
	for (auto const c : iter_all<CADCOnDLS>()) {
		decode_random(gen, config.cadc_readout_chains[c]);
	}

	decode_random(gen, config.readout_chain);

	for (auto const c : iter_all<PPUMemoryOnDLS>()) {
		decode_random(gen, config.ppu_memory[c]);
	}

	decode_random(gen, config.external_ppu_memory);
}

} // namespace stadls::vx
