#include "cereal/types/haldls/vx/cadc.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/cadc.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CADCChannelConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_offset));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, haldls::vx::CADCConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable));
	ar(CEREAL_NVP(value.m_reset_wait));
	ar(CEREAL_NVP(value.m_dead_time));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CADCOffsetSRAMTimingConfig& value, std::uint32_t const)
{
	ar(cereal::base_class<haldls::vx::detail::SRAMTimingConfig>(&value));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CADCSampleQuad& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_samples));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CADCChannelConfig);
CEREAL_REGISTER_TYPE(haldls::vx::CADCChannelConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CADCChannelConfig)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CADCConfig);
CEREAL_REGISTER_TYPE(haldls::vx::CADCConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CADCConfig)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CADCOffsetSRAMTimingConfig);
CEREAL_REGISTER_TYPE(haldls::vx::CADCOffsetSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CADCOffsetSRAMTimingConfig)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CADCSampleQuad);
CEREAL_REGISTER_TYPE(haldls::vx::CADCSampleQuad)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CADCSampleQuad)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_cadc)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CADCChannelConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CADCConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CADCOffsetSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CADCSampleQuad)
