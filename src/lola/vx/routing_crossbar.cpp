#include "lola/vx/routing_crossbar.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

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
	return print(os, config);
}

} // namespace lola::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::Crossbar)
