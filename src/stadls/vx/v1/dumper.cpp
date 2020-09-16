#include "stadls/vx/v1/dumper.h"

namespace stadls::vx {

namespace v1 {

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

} // namespace v1

template class stadls::vx::detail::Dumper<stadls::vx::v1::DumperDone>;

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template void v1::Dumper::write<Type>(                                                         \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template PlaybackProgram::ContainerTicket<Type> v1::Dumper::read<Type>(                        \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v1/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v1/container.def"

} // namespace stadls::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::DumperDone)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::Dumper)
CEREAL_CLASS_VERSION(stadls::vx::v1::DumperDone, 0)
CEREAL_CLASS_VERSION(stadls::vx::v1::Dumper, 0)
