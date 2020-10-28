#include "stadls/vx/v3/dumperdone.h"

#include "haldls/cerealization.tcc"

namespace stadls::vx::v3 {

bool DumperDone::operator==(DumperDone const& other) const
{
	return values == other.values;
}
bool DumperDone::operator!=(DumperDone const& other) const
{
	return !(*this == other);
}

} // namespace stadls::vx::v3

template std::string haldls::vx::to_json(stadls::vx::v3::DumperDone const&);
template std::string haldls::vx::to_portablebinary(stadls::vx::v3::DumperDone const&);
template void haldls::vx::from_json(stadls::vx::v3::DumperDone&, std::string const&);
template void haldls::vx::from_portablebinary(stadls::vx::v3::DumperDone&, std::string const&);

CEREAL_CLASS_VERSION(stadls::vx::v3::DumperDone, 4)
