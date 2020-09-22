#include "stadls/vx/v2/dumperdone.h"

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.tcc"

#include <cereal/types/utility.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>

namespace stadls::vx::v2 {

template <typename Archive>
void DumperDone::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(values));
}

bool DumperDone::operator==(DumperDone const& other) const
{
	return values == other.values;
}
bool DumperDone::operator!=(DumperDone const& other) const
{
	return !(*this == other);
}

} // namespace stadls::vx::v2

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v2::DumperDone)
CEREAL_CLASS_VERSION(stadls::vx::v2::DumperDone, 0)
