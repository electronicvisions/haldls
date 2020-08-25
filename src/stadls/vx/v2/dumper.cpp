#include "stadls/vx/v2/dumper.h"

#include "haldls/cerealization.tcc"
#include "stadls/vx/dumper.tcc"

namespace stadls::vx {

template class stadls::vx::detail::Dumper<stadls::vx::v2::DumperDone>;

template std::ostream& stadls::vx::detail::operator<<(
    std::ostream&, stadls::vx::detail::Dumper<stadls::vx::v2::DumperDone> const&);

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

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(stadls::vx::v2::Dumper)
CEREAL_CLASS_VERSION(stadls::vx::v2::Dumper, 1)
