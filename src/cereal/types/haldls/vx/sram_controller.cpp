#include "cereal/types/haldls/vx/sram_controller.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/sram_controller.h"

namespace cereal {

template <typename Archive>
void serialize(Archive& ar, haldls::vx::detail::SRAMTimingConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_read_delay));
	ar(CEREAL_NVP(value.m_address_setup_time));
	ar(CEREAL_NVP(value.m_enable_width));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::detail::SRAMTimingConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_sram_controller)
