#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/routing_crossbar.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/routing_crossbar.h"
#include "hate/visibility.h"
#ifndef __ppu__
#include "hxcomm/vx/target.h"
#endif
#include "lola/vx/genpybind.h"

namespace cereal {
struct access;
} // namespace cereal

namespace lola { namespace vx GENPYBIND_TAG_LOLA_VX {

/**
 * Container for configuration of the routing crossbar.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) Crossbar
    : public haldls::vx::ContainerBase<Crossbar>
{
public:
	typedef halco::hicann_dls::vx::CrossbarOnDLS coordinate_type;
	typedef std::false_type has_local_data;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware};
#endif

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

} // namespace vx
} // namespace lola

#include "lola/vx/routing_crossbar.tcc"
