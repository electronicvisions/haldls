#include "lola/vx/v3/chip.h"

#include "hate/empty.h"

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v3::Chip>
    : public BackendContainerBase<lola::vx::v3::Chip, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::Chip>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    lola::vx::v3::Chip::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::v3;
		using namespace halco::common;

		visitor(coord, config);

		hate::Empty<lola::vx::v3::Crossbar> crossbar;
		visit_preorder(crossbar, CrossbarOnDLS(), visitor);

		for (auto const c : iter_all<BackgroundSpikeSourceOnDLS>()) {
			hate::Empty<haldls::vx::v3::BackgroundSpikeSource> background_spike_source;
			visit_preorder(background_spike_source, c, visitor);
		}

		hate::Empty<lola::vx::v3::CapMem> capmem;
		visit_preorder(capmem, CapMemOnDLS(), visitor);

		for (auto const c : iter_all<SynapseDriverBlockOnDLS>()) {
			hate::Empty<lola::vx::v3::SynapseDriverBlock> synapse_driver_block;
			visit_preorder(synapse_driver_block, c, visitor);
		}

		for (auto const c : iter_all<SynapseBlockOnDLS>()) {
			hate::Empty<lola::vx::v3::SynapseBlock> synapse_block;
			visit_preorder(synapse_block, c, visitor);
		}

		hate::Empty<lola::vx::v3::NeuronBlock> neuron_block;
		visit_preorder(neuron_block, NeuronBlockOnDLS(), visitor);
		for (auto const c : iter_all<CADCOnDLS>()) {
			hate::Empty<lola::vx::v3::CADCReadoutChain> cadc_readout_chain;
			visit_preorder(cadc_readout_chain, c, visitor);
		}

		hate::Empty<lola::vx::v3::ReadoutChain> readout_chain;
		visit_preorder(readout_chain, ReadoutChainOnDLS(), visitor);

		for (auto const c : iter_all<PPUMemoryOnDLS>()) {
			hate::Empty<haldls::vx::v3::PPUMemory> ppu_memory;
			visit_preorder(ppu_memory, c, visitor);
		}

		hate::Empty<lola::vx::v3::ExternalPPUMemory> external_ppu_memory;
		visit_preorder(external_ppu_memory, ExternalPPUMemoryOnFPGA(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<halco::hicann_dls::vx::ChipOnDLS> const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::v3;
		using namespace halco::common;

		visitor(coord, config);

		visit_preorder(config.crossbar, hate::Empty<CrossbarOnDLS>{}, visitor);

		for (auto const c : iter_all<BackgroundSpikeSourceOnDLS>()) {
			visit_preorder(
			    config.background_spike_sources[c], hate::Empty<BackgroundSpikeSourceOnDLS>{},
			    visitor);
		}

		visit_preorder(config.capmem, hate::Empty<CapMemOnDLS>{}, visitor);

		for (auto const c : iter_all<SynapseDriverBlockOnDLS>()) {
			visit_preorder(
			    config.synapse_driver_blocks[c], hate::Empty<SynapseDriverBlockOnDLS>{}, visitor);
		}

		for (auto const c : iter_all<SynapseBlockOnDLS>()) {
			visit_preorder(config.synapse_blocks[c], hate::Empty<SynapseBlockOnDLS>{}, visitor);
		}

		visit_preorder(config.neuron_block, hate::Empty<NeuronBlockOnDLS>{}, visitor);
		for (auto const c : iter_all<CADCOnDLS>()) {
			visit_preorder(config.cadc_readout_chains[c], hate::Empty<CADCOnDLS>{}, visitor);
		}

		visit_preorder(config.readout_chain, hate::Empty<ReadoutChainOnDLS>{}, visitor);

		for (auto const c : iter_all<PPUMemoryOnDLS>()) {
			visit_preorder(config.ppu_memory[c], hate::Empty<PPUMemoryOnDLS>{}, visitor);
		}

		visit_preorder(config.external_ppu_memory, hate::Empty<ExternalPPUMemoryOnFPGA>{}, visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config, halco::hicann_dls::vx::ChipOnDLS const& coord, VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::v3;
		using namespace halco::common;

		visitor(coord, config);

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
    lola::vx::v3::Chip,
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
