#include "stadls/vx/v3/dumper.h"

#include "haldls/cerealization.tcc"
#include "stadls/vx/dumper.tcc"

namespace stadls::vx {

template class stadls::vx::detail::Dumper<stadls::vx::v3::DumperDone>;

template std::ostream& stadls::vx::detail::operator<<(
    std::ostream&, stadls::vx::detail::Dumper<stadls::vx::v3::DumperDone> const&);

#define PLAYBACK_CONTAINER(_Name, Type)                                                            \
	template void v3::Dumper::write<Type>(                                                         \
	    typename Type::coordinate_type const& coord, Type const& config);                          \
	template PlaybackProgram::ContainerTicket<Type> v3::Dumper::read<Type>(                        \
	    typename Type::coordinate_type const& coord);
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v3/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v3/container.def"

} // namespace stadls::vx

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v3::Dumper)
CEREAL_CLASS_VERSION(stadls::vx::v3::Dumper, 3)
