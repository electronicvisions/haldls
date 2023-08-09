#include "cereal/types/haldls/vx/v4/capmem.h"

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/haldls/vx/capmem.tcc"
#include "halco/hicann-dls/vx/v4/coordinates.h"
#include "haldls/vx/v4/capmem.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v4::ReferenceGeneratorConfig& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(value.m_capmem_amplifier));
	ar(CEREAL_NVP(value.m_enable_internal_reference));
	ar(CEREAL_NVP(value.m_enable_reference_output));
	ar(CEREAL_NVP(value.m_enable_reference_input));
	ar(CEREAL_NVP(value.m_capmem_amplifier));
	ar(CEREAL_NVP(value.m_capmem_offset));
	ar(CEREAL_NVP(value.m_capmem_slope));
	ar(CEREAL_NVP(value.m_reference_control));
	ar(CEREAL_NVP(value.m_resistor_control));
	ar(CEREAL_NVP(value.m_enable_reset));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::CapMemCell);
CEREAL_REGISTER_TYPE(haldls::vx::v4::CapMemCell)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v4::CapMemCell)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::CapMemBlock);
CEREAL_REGISTER_TYPE(haldls::vx::v4::CapMemBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v4::CapMemBlock)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::CapMemBlockConfig);
CEREAL_REGISTER_TYPE(haldls::vx::v4::CapMemBlockConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v4::CapMemBlockConfig)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::ReferenceGeneratorConfig);
CEREAL_REGISTER_TYPE(haldls::vx::v4::ReferenceGeneratorConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::v4::ReferenceGeneratorConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_v4_capmem)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v4::CapMemCell)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v4::CapMemBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v4::CapMemBlockConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::v4::ReferenceGeneratorConfig)
