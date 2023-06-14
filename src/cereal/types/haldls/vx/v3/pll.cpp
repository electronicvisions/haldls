#include "cereal/types/haldls/vx/v3/pll.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"

namespace haldls::vx::v3 {

template <typename Archive>
void PLLClockOutputBlock::ClockOutput::serialize(Archive& ar, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(m_enable_output));
	ar(CEREAL_NVP(m_enable_bypass));
	ar(CEREAL_NVP(m_adpll));
	ar(CEREAL_NVP(m_adpll_output));
}

} // namespace haldls::vx::v3

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::v3::PLLClockOutputBlock& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(value.m_output));
	ar(CEREAL_NVP(value.m_switch_spl1_to_madc));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v3::PLLClockOutputBlock::ClockOutput);

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v3::PLLClockOutputBlock);
CEREAL_REGISTER_TYPE(haldls::vx::v3::PLLClockOutputBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v3::PLLClockOutputBlock)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_v3_pll)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v3::PLLClockOutputBlock)
