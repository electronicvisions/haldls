#include "halco/common/iter_all.h"
#include <boost/hana/adapt_struct.hpp>

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::CapMem>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::CapMem,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::CapMem>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::CHIP_REVISION_STR::CapMem::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		for (auto block : halco::common::iter_all<CapMemBlockConfigOnDLS>()) {
			visit_preorder(config.blocks[block], hate::Empty<CapMemBlockConfigOnDLS>{}, visitor);
		}

		visit_preorder(
		    config.synapse_bias_selection, hate::Empty<SynapseBiasSelectionOnDLS>{}, visitor);
		visit_preorder(
		    config.reference_generator, hate::Empty<ReferenceGeneratorConfigOnDLS>{}, visitor);

		for (auto cell : halco::common::iter_all<UnusedCapMemCellOnDLS>()) {
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_config(config.unused_cells[cell]);
			visit_preorder(cell_config, hate::Empty<CapMemCellOnDLS>{}, visitor);
			if constexpr (!std::is_same_v<ContainerT, lola::vx::CHIP_REVISION_STR::CapMem const>) {
				config.unused_cells[cell] = cell_config.get_value();
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    lola::vx::CHIP_REVISION_STR::CapMem::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		for (auto block : halco::common::iter_all<CapMemBlockConfigOnDLS>()) {
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemBlockConfig> config;
			visit_preorder(config, block, visitor);
		}

		hate::Empty<haldls::vx::CHIP_REVISION_STR::SynapseBiasSelection> synapse_bias_selection;
		visit_preorder(synapse_bias_selection, SynapseBiasSelectionOnDLS(), visitor);
		hate::Empty<haldls::vx::CHIP_REVISION_STR::ReferenceGeneratorConfig>
		    reference_generator_config;
		visit_preorder(reference_generator_config, ReferenceGeneratorConfigOnDLS(), visitor);

		for (auto cell : halco::common::iter_all<UnusedCapMemCellOnDLS>()) {
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_config;
			visit_preorder(cell_config, cell.toCapMemCellOnDLS(), visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::CHIP_REVISION_STR::CapMem::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		for (auto block : halco::common::iter_all<CapMemBlockConfigOnDLS>()) {
			visit_preorder(config.blocks[block], block, visitor);
		}

		visit_preorder(config.synapse_bias_selection, SynapseBiasSelectionOnDLS(), visitor);
		visit_preorder(config.reference_generator, ReferenceGeneratorConfigOnDLS(), visitor);

		for (auto cell : halco::common::iter_all<UnusedCapMemCellOnDLS>()) {
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_config(config.unused_cells[cell]);
			visit_preorder(cell_config, cell.toCapMemCellOnDLS(), visitor);
			if constexpr (!std::is_same_v<ContainerT, lola::vx::CHIP_REVISION_STR::CapMem const>) {
				config.unused_cells[cell] = cell_config.get_value();
			}
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::CapMem,
    blocks,
    synapse_bias_selection,
    reference_generator,
    unused_cells);
