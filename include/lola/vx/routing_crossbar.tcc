#pragma once
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "lola/vx/routing_crossbar.h"
#include <boost/hana/adapt_struct.hpp>

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::Crossbar>
    : public BackendContainerBase<lola::vx::Crossbar, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::Crossbar>
{
	template <typename VisitorT>
	static void call(
	    hate::Empty<lola::vx::Crossbar> const& config,
	    lola::vx::Crossbar::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;
		using namespace haldls::vx;

		visitor(coord, config);

		hate::Empty<CrossbarOutputConfig> crossbar_output_config;
		visit_preorder(crossbar_output_config, CrossbarOutputConfigOnDLS(), visitor);

		for (auto n : iter_all<CrossbarNodeOnDLS>()) {
			hate::Empty<CrossbarNode> crossbar_node;
			visit_preorder(crossbar_node, n, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::Crossbar::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		visit_preorder(config.outputs, hate::Empty<CrossbarOutputConfigOnDLS>{}, visitor);

		for (auto n : iter_all<CrossbarNodeOnDLS>()) {
			visit_preorder(config.nodes[n], hate::Empty<CrossbarNodeOnDLS>{}, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config, lola::vx::Crossbar::coordinate_type const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		visit_preorder(config.outputs, CrossbarOutputConfigOnDLS(), visitor);

		for (auto n : iter_all<CrossbarNodeOnDLS>()) {
			visit_preorder(config.nodes[n], n, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::Crossbar, nodes, outputs);
