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
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename lola::vx::SinglechipFPGA::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename lola::vx::SinglechipFPGA::coordinate_type> const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::MultichipJboaLeafFPGA, output_routing_table, input_routing_table);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SinglechipFPGA);