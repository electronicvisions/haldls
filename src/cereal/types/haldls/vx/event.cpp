#include "cereal/types/haldls/vx/event.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/event.h"
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikePack1ToChip& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_impl));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikePack2ToChip& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_impl));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikePack3ToChip& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_impl));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::SpikeFromChip& value, std::uint32_t const)
{
	ar(CEREAL_NVP_("m_label", value.label));
	ar(CEREAL_NVP_("m_fpga_time", value.fpga_time));
	ar(CEREAL_NVP_("chip_time", value.chip_time));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::MADCSampleFromChip& value, std::uint32_t const)
{
	ar(CEREAL_NVP_("m_value", value.value));
	ar(CEREAL_NVP_("m_channel", value.channel));
	ar(CEREAL_NVP_("m_fpga_time", value.fpga_time));
	ar(CEREAL_NVP_("m_chip_time", value.chip_time));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::HighspeedLinkNotification& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_phy));
	ar(CEREAL_NVP(value.m_link_up));
	ar(CEREAL_NVP(value.m_decode_error));
	ar(CEREAL_NVP(value.m_crc_error));
	ar(CEREAL_NVP(value.m_crc_recover));
	ar(CEREAL_NVP(value.m_check_error));
	ar(CEREAL_NVP(value.m_fpga_time));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikePack1ToChip);
CEREAL_REGISTER_TYPE(haldls::vx::SpikePack1ToChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikePack1ToChip)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikePack2ToChip);
CEREAL_REGISTER_TYPE(haldls::vx::SpikePack2ToChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikePack2ToChip)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikePack3ToChip);
CEREAL_REGISTER_TYPE(haldls::vx::SpikePack3ToChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::SpikePack3ToChip)

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::SpikeFromChip);
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::MADCSampleFromChip);
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::HighspeedLinkNotification);

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_event)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikePack1ToChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikePack2ToChip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::SpikePack3ToChip)
