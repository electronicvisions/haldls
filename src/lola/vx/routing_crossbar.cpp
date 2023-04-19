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
	std::stringstream ss;
	for (auto const n : halco::common::iter_all<halco::hicann_dls::vx::CrossbarNodeOnDLS>()) {
		ss << n << ": " << config.nodes[n] << "\n";
	}
	ss << config.outputs;
	os << hate::indent(ss.str(), "\t");
	os << "\n)";
	return os;
}

} // namespace lola::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::Crossbar)
