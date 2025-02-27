#pragma once
#include "halco/hicann-dls/vx/v4/chip.h"
#include "haldls/vx/container.h"
#include "haldls/vx/v4/background.h"
#include "haldls/vx/v4/ppu.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include "lola/vx/v4/cadc.h"
#include "lola/vx/v4/capmem.h"
#include "lola/vx/v4/memory_timing.h"
#include "lola/vx/v4/neuron.h"
#include "lola/vx/v4/ppu.h"
#include "lola/vx/v4/readout_chain.h"
#include "lola/vx/v4/routing_crossbar.h"
#include "lola/vx/v4/synapse_block.h"
#include "lola/vx/v4/synapse_driver.h"
#include <boost/hana/adapt_struct.hpp>

namespace lola::vx { namespace v4 GENPYBIND_TAG_LOLA_VX_V4 {

/**
 * Complete static configuration of one chip.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) Chip
    : public haldls::vx::ContainerBase<Chip>
{
public:
	typedef halco::hicann_dls::vx::ChipOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	Chip() = default;

	/**
	 * Timing of memory cells, SynRAM, CADC offsets, analog and digital neuron and synapse driver
	 * configuration.
	 */
	MemoryTiming memory_timing;

	/**
	 * Event routing crossbar configuration.
	 */
	Crossbar crossbar;

	typedef halco::common::typed_array<
	    haldls::vx::v4::BackgroundSpikeSource,
	    halco::hicann_dls::vx::v4::BackgroundSpikeSourceOnDLS>
	    BackgroundSpikeSources GENPYBIND(opaque(false));
	/**
	 * Background spike sources.
	 */
	BackgroundSpikeSources background_spike_sources;

	/**
	 * CapMem configuration and unused cells, all cells associated with other entities, e.g. neurons
	 * are placed at their respective containers.
	 */
	CapMem capmem;

	typedef halco::common::
	    typed_array<SynapseDriverBlock, halco::hicann_dls::vx::v4::SynapseDriverBlockOnDLS>
	        SynapseDriverBlocks GENPYBIND(opaque(false));
	/**
	 * Blocks of synapse drivers, one per hemisphere containing hemisphere-global configuration and
	 * all individual synapse driver configurations.
	 */
	SynapseDriverBlocks synapse_driver_blocks;

	typedef halco::common::typed_array<SynapseBlock, halco::hicann_dls::vx::v4::SynapseBlockOnDLS>
	    SynapseBlocks GENPYBIND(opaque(false));
	/**
	 * Blocks of synapses, one per hemisphere containing hemisphere-global configuration and all
	 * individual synapse parameters.
	 */
	SynapseBlocks synapse_blocks;

	/**
	 * Block on neurons containing all atomic neuron circuit configurations, chip-global neuron
	 * backend parameters and synaptic input switches to the synapse blocks.
	 */
	NeuronBlock neuron_block;

	typedef halco::common::typed_array<CADCReadoutChain, halco::hicann_dls::vx::v4::CADCOnDLS>
	    CADCReadoutChains GENPYBIND(opaque(false));
	/**
	 * CADC readout chains, one per hemisphere containing CADC and correlation sensor configuration
	 * as well as the switches to the synapses or neurons.
	 */
	CADCReadoutChains cadc_readout_chains;

	/**
	 * Readout chain containing MADC configuration and analog signal routing.
	 */
	ReadoutChain readout_chain;

	typedef halco::common::
	    typed_array<haldls::vx::v4::PPUMemory, halco::hicann_dls::vx::v4::PPUMemoryOnDLS>
	        PPUMemory GENPYBIND(opaque(false));
	/**
	 * PPU SRAM memory, one per PPU.
	 */
	PPUMemory ppu_memory;

	/**
	 * Chip-external global PPU memory.
	 */
	ExternalPPUMemory external_ppu_memory;

	bool operator==(Chip const& other) const SYMBOL_VISIBLE;
	bool operator!=(Chip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Chip const& config) SYMBOL_VISIBLE;

	/**
	 * Default instance for neuron bypass.
	 */
	static const SYMBOL_VISIBLE Chip default_neuron_bypass;

private:
	friend haldls::vx::detail::VisitPreorderImpl<Chip>;
};

} // namespace v4
} // namespace lola::vx

#include "lola/vx/v4/chip.tcc"
