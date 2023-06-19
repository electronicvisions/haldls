#pragma once
#include "cereal/types/haldls/cereal.h"
#include <cereal/types/polymorphic.hpp>

namespace haldls::vx {

struct TMP112Status;
struct INA219Config;
struct INA219Status;
struct TCA9554Inputs;
struct TCA9554Config;
struct AD5252ChannelConfig;
struct AD5252ChannelConfigPersistent;
struct DAC6573ChannelConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::TMP112Status& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::INA219Config& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::INA219Status& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::TCA9554Inputs& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::TCA9554Config& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::AD5252ChannelConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::AD5252ChannelConfigPersistent& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::DAC6573ChannelConfig& value, std::uint32_t const version);

} // namespace cereal

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::TMP112Status)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::INA219Config)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::INA219Status)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::TCA9554Inputs)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::TCA9554Config)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::AD5252ChannelConfig)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::AD5252ChannelConfigPersistent)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::DAC6573ChannelConfig)

CEREAL_FORCE_DYNAMIC_INIT(haldls_vx_i2c)
