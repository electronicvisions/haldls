#include "cereal/types/haldls/vx/correlation.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/correlation.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonCorrelationConfig& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(value.m_sense_delay));
	ar(CEREAL_NVP(value.m_reset_duration));
	ar(CEREAL_NVP(value.m_reset_fall_time));
	ar(CEREAL_NVP(value.m_reset_mode));
	ar(CEREAL_NVP(value.m_cadc_v_offset_assignment));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonCorrelationConfig);
CEREAL_REGISTER_TYPE(haldls::vx::CommonCorrelationConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CommonCorrelationConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_correlation)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CommonCorrelationConfig)
