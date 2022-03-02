#include "lola/vx/v2/chip.h"

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v2::Chip>
    : public BackendContainerBase<lola::vx::v2::Chip, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::Chip>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config, lola::vx::v2::Chip::coordinate_type const& coord, VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::v2;
		using namespace halco::common;

		visitor(coord, config);

		visit_preorder(config.memory_timing, MemoryTimingOnDLS(), visitor);
		visit_preorder(config.crossbar, CrossbarOnDLS(), visitor);

		for (auto const c : iter_all<BackgroundSpikeSourceOnDLS>()) {
			visit_preorder(config.background_spike_sources[c], c, visitor);
		}

		visit_preorder(config.capmem, CapMemOnDLS(), visitor);

		for (auto const c : iter_all<SynapseDriverBlockOnDLS>()) {
			visit_preorder(config.synapse_driver_blocks[c], c, visitor);
		}

		for (auto const c : iter_all<SynapseBlockOnDLS>()) {
			visit_preorder(config.synapse_blocks[c], c, visitor);
		}

		visit_preorder(config.neuron_block, NeuronBlockOnDLS(), visitor);
		for (auto const c : iter_all<CADCOnDLS>()) {
			visit_preorder(config.cadc_readout_chains[c], c, visitor);
		}

		visit_preorder(config.readout_chain, ReadoutChainOnDLS(), visitor);

		for (auto const c : iter_all<PPUMemoryOnDLS>()) {
			visit_preorder(config.ppu_memory[c], c, visitor);
		}

		visit_preorder(config.external_ppu_memory, ExternalPPUMemoryOnFPGA(), visitor);
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::Chip,
    memory_timing,
    crossbar,
    background_spike_sources,
    capmem,
    synapse_driver_blocks,
    synapse_blocks,
    neuron_block,
    cadc_readout_chains,
    readout_chain,
    ppu_memory,
    external_ppu_memory);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::Chip)
