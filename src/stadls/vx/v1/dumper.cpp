#include "stadls/vx/v1/dumper.h"

#include "haldls/cerealization.tcc"
#include "stadls/vx/dumper.tcc"

namespace stadls::vx {

template class stadls::vx::detail::Dumper<stadls::vx::v1::DumperDone>;

template std::ostream& stadls::vx::detail::operator<<(
    std::ostream&, stadls::vx::detail::Dumper<stadls::vx::v1::DumperDone> const&);

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

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v1::Dumper)
CEREAL_CLASS_VERSION(stadls::vx::v1::Dumper, 2)
