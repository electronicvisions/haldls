#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/common/typed_heap_array.h"
#include "halco/hicann-dls/vx/routing_table.h"

#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/routing_table_entry.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include <cereal/macros.hpp>

namespace fisch::vx {
class Omnibus;
} // namespace fisch::vx

namespace lola {
namespace vx GENPYBIND_TAG_LOLA_VX {

/**
 * Routing table for outgoing events.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) OutputRoutingTable
    : public haldls::vx::ContainerBase<OutputRoutingTable>
{
public:
	typedef halco::hicann_dls::vx::OutputRoutingTableOnFPGA coordinate_type;
	typedef std::false_type has_local_data;

	/** Default constructor. */
	OutputRoutingTable() SYMBOL_VISIBLE;

	bool operator==(OutputRoutingTable const& other) const SYMBOL_VISIBLE;
	bool operator!=(OutputRoutingTable const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, OutputRoutingTable const& config)
	    SYMBOL_VISIBLE;

	typedef halco::common::typed_heap_array<
	    haldls::vx::OutputRoutingTableEntry,
	    typename haldls::vx::OutputRoutingTableEntry::coordinate_type>
	    Entries GENPYBIND(opaque(false));
	Entries entries;

private:
	friend haldls::vx::detail::VisitPreorderImpl<OutputRoutingTable>;
};


/**
 * Routing table for incoming events.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) InputRoutingTable
    : public haldls::vx::ContainerBase<InputRoutingTable>
{
public:
	typedef halco::hicann_dls::vx::InputRoutingTableOnFPGA coordinate_type;
	typedef std::false_type has_local_data;

	/** Default constructor. */
	InputRoutingTable() SYMBOL_VISIBLE;

	bool operator==(InputRoutingTable const& other) const SYMBOL_VISIBLE;
	bool operator!=(InputRoutingTable const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, InputRoutingTable const& config)
	    SYMBOL_VISIBLE;

	typedef halco::common::typed_heap_array<
	    haldls::vx::InputRoutingTableEntry,
	    typename haldls::vx::InputRoutingTableEntry::coordinate_type>
	    Entries GENPYBIND(opaque(false));
	Entries entries;

private:
	friend haldls::vx::detail::VisitPreorderImpl<InputRoutingTable>;
};

} // namespace vx
} // namespace lola

#include "lola/vx/routing_table.tcc"
