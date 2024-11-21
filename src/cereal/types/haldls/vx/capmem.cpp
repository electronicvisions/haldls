#include "cereal/types/haldls/vx/capmem.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/capmem.h"

namespace cereal {


template <class Archive>
void serialize(Archive& ar, haldls::vx::CapMemSRAMTimingConfig& value, std::uint32_t const)
{
	ar(cereal::base_class<haldls::vx::detail::SRAMTimingConfig>(&value));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CapMemSRAMTimingConfig)
CEREAL_REGISTER_TYPE(haldls::vx::CapMemSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CapMemSRAMTimingConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_capmem)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CapMemSRAMTimingConfig)