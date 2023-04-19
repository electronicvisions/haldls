#include "cereal/types/haldls/vx/barrier.h"

#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/barrier.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, haldls::vx::Barrier& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_omnibus));
	ar(CEREAL_NVP(value.m_enable_jtag));
	ar(CEREAL_NVP(value.m_enable_systime));
	ar(CEREAL_NVP(value.m_enable_multi_fpga));
	ar(CEREAL_NVP(value.m_enable_systime_correction));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::Barrier);
CEREAL_REGISTER_TYPE(haldls::vx::Barrier)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::BlockUntil, haldls::vx::Barrier)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::Barrier)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_barrier)
