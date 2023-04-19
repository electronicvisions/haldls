#include "cereal/types/haldls/vx/spi.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/spi.h"

namespace cereal {


template <typename Archive>
void serialize(Archive& ar, haldls::vx::ShiftRegister& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_mux_1));
	ar(CEREAL_NVP(value.m_mux_2));
	ar(CEREAL_NVP(value.m_mux_3));
	ar(CEREAL_NVP(value.m_enable_i_ref_board));
	ar(CEREAL_NVP(value.m_enable_measure_i_ref));
	ar(CEREAL_NVP(value.m_enable_dac_to_readout_0));
	ar(CEREAL_NVP(value.m_enable_dac_to_readout_1));
	ar(CEREAL_NVP(value.m_enable_led));
	ar(CEREAL_NVP(value.m_enable_adc_power_down));
	ar(CEREAL_NVP(value.m_enable_adc_reset));
	ar(CEREAL_NVP(value.m_enable_vdd));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::DACChannel& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::DACControl& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_channel));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ShiftRegister)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::DACChannel)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::DACControl)
CEREAL_REGISTER_TYPE(haldls::vx::ShiftRegister)
CEREAL_REGISTER_TYPE(haldls::vx::DACChannel)
CEREAL_REGISTER_TYPE(haldls::vx::DACControl)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ShiftRegister)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::DACChannel)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::DACControl)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_spi)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ShiftRegister)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::DACChannel)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::DACControl)
