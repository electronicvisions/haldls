#include "lola/vx/routing_table.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

namespace lola {
namespace vx {

OutputRoutingTable::OutputRoutingTable() {}

bool OutputRoutingTable::operator==(OutputRoutingTable const& other) const
{
	return equal(*this, other);
}

bool OutputRoutingTable::operator!=(OutputRoutingTable const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, OutputRoutingTable const& config)
{
	return print(os, config);
}

InputRoutingTable::InputRoutingTable() {}

bool InputRoutingTable::operator==(InputRoutingTable const& other) const
{
	return equal(*this, other);
}

bool InputRoutingTable::operator!=(InputRoutingTable const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, InputRoutingTable const& config)
{
	return print(os, config);
}

} // namespace vx
} // namespace lola

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::OutputRoutingTable)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::InputRoutingTable)
