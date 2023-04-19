#include "cereal/types/haldls/vx/block.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "haldls/vx/block.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PollingOmnibusBlockConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_address));
	ar(CEREAL_NVP(value.m_mask));
	ar(CEREAL_NVP(value.m_target));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PollingOmnibusBlock& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_expects_equality));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PollingOmnibusBlockConfig);
CEREAL_REGISTER_TYPE(haldls::vx::PollingOmnibusBlockConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PollingOmnibusBlockConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PollingOmnibusBlockConfig)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PollingOmnibusBlock);
CEREAL_REGISTER_TYPE(haldls::vx::PollingOmnibusBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::BlockUntil, haldls::vx::PollingOmnibusBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PollingOmnibusBlock)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_block)
