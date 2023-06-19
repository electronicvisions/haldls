#include "cereal/types/haldls/vx/i2c.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/i2c.h"

namespace cereal {

template <typename Archive>
void serialize(Archive& ar, haldls::vx::TMP112Status& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_temperature));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::INA219Config& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_bus_adc_mode));
	ar(CEREAL_NVP(value.m_shunt_adc_mode));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::INA219Status& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_bus_voltage));
	ar(CEREAL_NVP(value.m_shunt_voltage));
	ar(CEREAL_NVP(value.m_bus_voltage_overflow));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::TCA9554Inputs& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_input));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::TCA9554Config& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_output));
	ar(CEREAL_NVP(value.m_polarity));
	ar(CEREAL_NVP(value.m_mode));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::AD5252ChannelConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::AD5252ChannelConfigPersistent& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <typename Archive>
void serialize(
    Archive& ar, haldls::vx::DAC6573ChannelConfig& value, std::uint32_t const /*version*/)
{
	ar(CEREAL_NVP(value.m_value));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::TMP112Status)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::INA219Config)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::INA219Status)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::TCA9554Inputs)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::TCA9554Config)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::AD5252ChannelConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::AD5252ChannelConfigPersistent)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::DAC6573ChannelConfig)
CEREAL_REGISTER_TYPE(haldls::vx::TMP112Status)
CEREAL_REGISTER_TYPE(haldls::vx::INA219Config)
CEREAL_REGISTER_TYPE(haldls::vx::INA219Status)
CEREAL_REGISTER_TYPE(haldls::vx::TCA9554Inputs)
CEREAL_REGISTER_TYPE(haldls::vx::TCA9554Config)
CEREAL_REGISTER_TYPE(haldls::vx::AD5252ChannelConfig)
CEREAL_REGISTER_TYPE(haldls::vx::AD5252ChannelConfigPersistent)
CEREAL_REGISTER_TYPE(haldls::vx::DAC6573ChannelConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::TMP112Status)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::INA219Config)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::INA219Status)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::TCA9554Inputs)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::TCA9554Config)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::AD5252ChannelConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::AD5252ChannelConfigPersistent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::DAC6573ChannelConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_i2c)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::TMP112Status)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::INA219Config)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::INA219Status)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::TCA9554Inputs)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::TCA9554Config)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::AD5252ChannelConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::AD5252ChannelConfigPersistent)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::DAC6573ChannelConfig)
