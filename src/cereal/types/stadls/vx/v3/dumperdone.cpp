#include "cereal/types/stadls/vx/v3/dumperdone.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/haldls/vx/ppu.h"
#include "stadls/vx/v3/dumperdone.h"
#include <cereal/types/memory.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, stadls::vx::v3::DumperDone& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP_("values", value.values));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(stadls::vx::v3::DumperDone);

CEREAL_REGISTER_DYNAMIC_INIT(stadls_vx_v3_dumperdone)
