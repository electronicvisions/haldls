#include "stadls/vx/v2/dumper.h"

namespace stadls::vx {

namespace v2 {

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

} // namespace v2

template class stadls::vx::detail::Dumper<stadls::vx::v2::DumperDone>;

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template void v2::Dumper::write<Type>(                                                         \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template PlaybackProgram::ContainerTicket<Type> v2::Dumper::read<Type>(                        \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"

} // namespace stadls::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v2::DumperDone)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v2::Dumper)
CEREAL_CLASS_VERSION(stadls::vx::v2::DumperDone, 0)
CEREAL_CLASS_VERSION(stadls::vx::v2::Dumper, 0)
