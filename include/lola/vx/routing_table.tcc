#pragma once

#include "lola/vx/routing_table.h"

#include "halco/common/iter_all.h"
#include "haldls/vx/traits.h"
#include <boost/hana/adapt_struct.hpp>


namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::OutputRoutingTable>
    : public BackendContainerBase<
          lola::vx::OutputRoutingTable,
          fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::OutputRoutingTable>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename lola::vx::OutputRoutingTable::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		for (auto const entry_coordinate : halco::common::iter_all<
		         typename haldls::vx::OutputRoutingTableEntry::coordinate_type>()) {
			visit_preorder(config.entries[entry_coordinate], entry_coordinate, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename lola::vx::OutputRoutingTable::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		for (auto const entry_coordinate : halco::common::iter_all<
		         typename haldls::vx::OutputRoutingTableEntry::coordinate_type>()) {
			hate::Empty<haldls::vx::OutputRoutingTableEntry> empty_entry;
			visit_preorder(empty_entry, entry_coordinate, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename lola::vx::OutputRoutingTable::coordinate_type> const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		for (auto const entry_coordinate : halco::common::iter_all<
		         typename haldls::vx::OutputRoutingTableEntry::coordinate_type>()) {
			visit_preorder(config.entries[entry_coordinate], entry_coordinate, visitor);
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::InputRoutingTable>
    : public BackendContainerBase<lola::vx::InputRoutingTable, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct VisitPreorderImpl<lola::vx::InputRoutingTable>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename lola::vx::InputRoutingTable::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		for (auto const entry_coordinate : halco::common::iter_all<
		         typename haldls::vx::InputRoutingTableEntry::coordinate_type>()) {
			visit_preorder(config.entries[entry_coordinate], entry_coordinate, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename lola::vx::InputRoutingTable::coordinate_type const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		for (auto const entry_coordinate : halco::common::iter_all<
		         typename haldls::vx::InputRoutingTableEntry::coordinate_type>()) {
			hate::Empty<haldls::vx::InputRoutingTableEntry> empty_entry;
			visit_preorder(empty_entry, entry_coordinate, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename lola::vx::InputRoutingTable::coordinate_type> const& coordinate,
	    VisitorT&& visitor)
	{
		visitor(coordinate, config);

		for (auto const entry_coordinate : halco::common::iter_all<
		         typename haldls::vx::InputRoutingTableEntry::coordinate_type>()) {
			visit_preorder(config.entries[entry_coordinate], entry_coordinate, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::OutputRoutingTable, entries);
BOOST_HANA_ADAPT_STRUCT(lola::vx::InputRoutingTable, entries);