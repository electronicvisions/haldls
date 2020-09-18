#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "haldls/vx/common.h"
#include "haldls/vx/routing_crossbar.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"
#include "lola/vx/genpybind.h"

namespace cereal {
class access;
} // namespace cereal

namespace lola::vx GENPYBIND_TAG_LOLA_VX {

/**
 * Container for configuration of the routing crossbar.
 */
class GENPYBIND(visible) Crossbar
{
public:
	typedef halco::hicann_dls::vx::CrossbarOnDLS coordinate_type;
	typedef std::false_type has_local_data;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware};

	/** Default constructor. */
	Crossbar() SYMBOL_VISIBLE;

	typedef halco::common::
	    typed_array<haldls::vx::CrossbarNode, halco::hicann_dls::vx::CrossbarNodeOnDLS>
	        Nodes GENPYBIND(opaque);

	/** Crossbar node configuration. */
	Nodes nodes;

	haldls::vx::CrossbarOutputConfig outputs;

	bool operator==(Crossbar const& other) const SYMBOL_VISIBLE;
	bool operator!=(Crossbar const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Crossbar const& block) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::Crossbar>;
};

} // namespace lola::vx

#include "lola/vx/routing_crossbar.tcc"
