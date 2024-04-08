#include "lola/vx/routing_crossbar.h"

#include "haldls/vx/container.tcc"
#include "hate/indent.h"
#include "lola/vx/hana.h"
#include <sstream>

namespace lola::vx {

Crossbar::Crossbar() : nodes(), outputs() {}

bool Crossbar::operator==(Crossbar const& other) const
{
	return equal(*this, other);
}

bool Crossbar::operator!=(Crossbar const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, Crossbar const& config)
{
	os << "Crossbar(\n";
	hate::IndentingOstream ios(os);
	ios << hate::Indentation("\t");
	for (auto const n : halco::common::iter_all<halco::hicann_dls::vx::CrossbarNodeOnDLS>()) {
		ios << n << ": " << config.nodes[n] << "\n";
	}
	ios << config.outputs;
	os << "\n)";
	return os;
}

} // namespace lola::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::Crossbar)
