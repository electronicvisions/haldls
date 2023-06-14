#include "cereal/types/haldls/vx/arq.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/arq.h"

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::HicannARQStatus& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_read_count));
	ar(CEREAL_NVP(value.m_write_count));
	ar(CEREAL_NVP(value.m_rx_count));
	ar(CEREAL_NVP(value.m_tx_count));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::HicannARQStatus);
CEREAL_REGISTER_TYPE(haldls::vx::HicannARQStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::HicannARQStatus)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_arq)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::HicannARQStatus)
