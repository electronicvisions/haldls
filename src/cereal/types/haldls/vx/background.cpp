#include "cereal/types/haldls/vx/background.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/background.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::BackgroundSpikeSource& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable));
	ar(CEREAL_NVP(value.m_enable_random));
	ar(CEREAL_NVP(value.m_period));
	ar(CEREAL_NVP(value.m_rate));
	ar(CEREAL_NVP(value.m_seed));
	ar(CEREAL_NVP(value.m_mask));
	ar(CEREAL_NVP(value.m_neuron_label));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::BackgroundSpikeSource);
CEREAL_REGISTER_TYPE(haldls::vx::BackgroundSpikeSource)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::BackgroundSpikeSource)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_background)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::BackgroundSpikeSource)
