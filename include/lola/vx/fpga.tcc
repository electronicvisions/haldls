#pragma once
#include "lola/vx/fpga.h"

#include "halco/common/iter_all.h"
#include "haldls/vx/traits.h"
#include <boost/hana/adapt_struct.hpp>


namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::MultichipJboaLeafFPGA>
    : public BackendContainerBase<
          lola::vx::MultichipJboaLeafFPGA,
          fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::MultichipJboaLeafFPGA>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename lola::vx::MultichipJboaLeafFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		visit_preorder(
		    config.output_routing_table, lola::vx::OutputRoutingTable::coordinate_type(), visitor);

		visit_preorder(
		    config.input_routing_table, lola::vx::InputRoutingTable::coordinate_type(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename lola::vx::MultichipJboaLeafFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);


		hate::Empty<lola::vx::OutputRoutingTable> empty_output;
		visit_preorder(empty_output, lola::vx::OutputRoutingTable::coordinate_type(), visitor);

		hate::Empty<lola::vx::InputRoutingTable> empty_input;
		visit_preorder(empty_input, lola::vx::InputRoutingTable::coordinate_type(), visitor);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename lola::vx::MultichipJboaLeafFPGA::coordinate_type> const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);


		visit_preorder(
		    config.output_routing_table, lola::vx::OutputRoutingTable::coordinate_type(), visitor);

		visit_preorder(
		    config.input_routing_table, lola::vx::InputRoutingTable::coordinate_type(), visitor);
	}
};

template <>
struct BackendContainerTrait<lola::vx::SinglechipFPGA>
    : public BackendContainerBase<lola::vx::SinglechipFPGA, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::SinglechipFPGA>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename lola::vx::SinglechipFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		visit_preorder(
		    config.spikeio_config, halco::hicann_dls::vx::SpikeIOConfigOnFPGA{}, visitor);

		for (auto const c :
		     halco::common::iter_all<halco::hicann_dls::vx::SpikeIOInputRouteOnFPGA>()) {
			visit_preorder(config.spikeio_input_routes[c], c, visitor);
		}

		for (auto const c :
		     halco::common::iter_all<halco::hicann_dls::vx::SpikeIOOutputRouteOnFPGA>()) {
			visit_preorder(config.spikeio_output_routes[c], c, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename lola::vx::SinglechipFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		hate::Empty<haldls::vx::SpikeIOConfig> empty_cfg;
		visit_preorder(empty_cfg, halco::hicann_dls::vx::SpikeIOConfigOnFPGA{}, visitor);

		for (auto const c :
		     halco::common::iter_all<halco::hicann_dls::vx::SpikeIOInputRouteOnFPGA>()) {
			hate::Empty<haldls::vx::SpikeIOInputRoute> empty_in;
			visit_preorder(empty_in, c, visitor);
		}

		for (auto const c :
		     halco::common::iter_all<halco::hicann_dls::vx::SpikeIOOutputRouteOnFPGA>()) {
			hate::Empty<haldls::vx::SpikeIOOutputRoute> empty_out;
			visit_preorder(empty_out, c, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename lola::vx::SinglechipFPGA::coordinate_type> const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		visit_preorder(
		    config.spikeio_config, halco::hicann_dls::vx::SpikeIOConfigOnFPGA{}, visitor);

		for (auto const c :
		     halco::common::iter_all<halco::hicann_dls::vx::SpikeIOInputRouteOnFPGA>()) {
			visit_preorder(config.spikeio_input_routes[c], c, visitor);
		}

		for (auto const c :
		     halco::common::iter_all<halco::hicann_dls::vx::SpikeIOOutputRouteOnFPGA>()) {
			visit_preorder(config.spikeio_output_routes[c], c, visitor);
		}
		// TODO all 3 overloads needed for sio? real coordintes yes; emtpy? idk ; emtpy coord? idk
		// lets keep for now and mirror jboa visit later
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::MultichipJboaLeafFPGA, output_routing_table, input_routing_table);
BOOST_HANA_ADAPT_STRUCT(
    lola::vx::SinglechipFPGA, spikeio_config, spikeio_input_routes, spikeio_output_routes);