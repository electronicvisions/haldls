#include "cereal/types/haldls/vx/synapse_driver.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/synapse_driver.h"

namespace cereal {

template <typename Archive>
void serialize(Archive& ar, haldls::vx::SynapseDriverSRAMTimingConfig& value, std::uint32_t const)
{
	ar(cereal::base_class<haldls::vx::detail::SRAMTimingConfig>(&value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::SynapseDriverConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_en_receiver));
	ar(CEREAL_NVP(value.m_row_address_compare_mask));
	ar(CEREAL_NVP(value.m_en_address_out));
	ar(CEREAL_NVP(value.m_utilization));
	ar(CEREAL_NVP(value.m_offset));
	ar(CEREAL_NVP(value.m_en_hagen_dac));
	ar(CEREAL_NVP(value.m_recovery));
	ar(CEREAL_NVP(value.m_row_mode_bottom));
	ar(CEREAL_NVP(value.m_row_mode_top));
	ar(CEREAL_NVP(value.m_hagen_dac_offset));
	ar(CEREAL_NVP(value.m_select_target_voltages));
	ar(CEREAL_NVP(value.m_en_readout));
	ar(CEREAL_NVP(value.m_en_renewing));
	ar(CEREAL_NVP(value.m_en_hagen_modulation));
	ar(CEREAL_NVP(value.m_en_stp));
	ar(CEREAL_NVP(value.m_en_charge_sharing));
	ar(CEREAL_NVP(value.m_en_recovery));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseDriverSRAMTimingConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SynapseDriverConfig)
CEREAL_REGISTER_TYPE(haldls::vx::SynapseDriverSRAMTimingConfig)
CEREAL_REGISTER_TYPE(haldls::vx::SynapseDriverConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::SynapseDriverSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SynapseDriverConfig)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_synapse_driver)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::SynapseDriverSRAMTimingConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SynapseDriverConfig)
