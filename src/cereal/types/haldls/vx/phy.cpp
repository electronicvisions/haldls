#include "cereal/types/haldls/vx/phy.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/vx/phy.h"

namespace cereal {

template <typename Archive>
void serialize(Archive& ar, haldls::vx::detail::PhyConfigBase& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_bit_slip));
	ar(CEREAL_NVP(value.m_manual_delay));
	ar(CEREAL_NVP(value.m_enable_delay_cell_measurement));
	ar(CEREAL_NVP(value.m_enable_initialization_master_mode));
	ar(CEREAL_NVP(value.m_enable_manual_tx_data_valid_for_init));
	ar(CEREAL_NVP(value.m_enable_force_lvds_power_up));
	ar(CEREAL_NVP(value.m_enable_force_start));
	ar(CEREAL_NVP(value.m_enable_manual_training_mode));
	ar(CEREAL_NVP(value.m_enable_ber_loopback));
	ar(CEREAL_NVP(value.m_vbias));
	ar(CEREAL_NVP(value.m_debug_outputs));
	ar(CEREAL_NVP(value.m_enable_transmission_without_idle_pattern));
	ar(CEREAL_NVP(value.m_enable_clock_pre_alignment));
	ar(CEREAL_NVP(value.m_enable_des_recal));
	ar(CEREAL_NVP(value.m_enable_loopback_en));
	ar(CEREAL_NVP(value.m_enable_auto_init));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::PhyConfigFPGA& value, std::uint32_t const)
{
	ar(cereal::base_class<haldls::vx::detail::PhyConfigBase>(&value));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::PhyConfigChip& value, std::uint32_t const)
{
	ar(cereal::base_class<haldls::vx::detail::PhyConfigBase>(&value));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::CommonPhyConfigFPGA& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_phy));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::CommonPhyConfigChip& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_phy));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::PhyStatus& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_crc_error_count));
	ar(CEREAL_NVP(value.m_online_time));
	ar(CEREAL_NVP(value.m_rx_dropped_count));
	ar(CEREAL_NVP(value.m_rx_count));
	ar(CEREAL_NVP(value.m_tx_count));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::detail::PhyConfigBase)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PhyConfigFPGA)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PhyConfigChip)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonPhyConfigFPGA)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::CommonPhyConfigChip)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PhyStatus)
CEREAL_REGISTER_TYPE(haldls::vx::PhyConfigFPGA)
CEREAL_REGISTER_TYPE(haldls::vx::PhyConfigChip)
CEREAL_REGISTER_TYPE(haldls::vx::CommonPhyConfigFPGA)
CEREAL_REGISTER_TYPE(haldls::vx::CommonPhyConfigChip)
CEREAL_REGISTER_TYPE(haldls::vx::PhyStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PhyConfigFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PhyConfigChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CommonPhyConfigFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::CommonPhyConfigChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PhyStatus)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_phy)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PhyConfigFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PhyConfigChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CommonPhyConfigFPGA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::CommonPhyConfigChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PhyStatus)
