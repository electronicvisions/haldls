#pragma once
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "lola/vx/routing_crossbar.h"
#include <boost/hana/adapt_struct.hpp>
#include "lola/vx/cerealization.h"

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::Crossbar>
    : public BackendContainerBase<lola::vx::Crossbar, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::Crossbar>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
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
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::Crossbar)
