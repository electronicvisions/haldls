#include "cereal/types/haldls/vx/perftest.h"

#include "cereal/types/haldls/cereal.tcc"
#include "halco/common/cerealization_geometry.h"
#include "haldls/vx/perftest.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::PerfTest& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::PerfTestStatus& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_sent));
	ar(CEREAL_NVP(value.m_received));
	ar(CEREAL_NVP(value.m_in_order));
	ar(CEREAL_NVP(value.m_error_word));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PerfTest)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PerfTestStatus)
CEREAL_REGISTER_TYPE(haldls::vx::PerfTest)
CEREAL_REGISTER_TYPE(haldls::vx::PerfTestStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PerfTest)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PerfTestStatus)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_perftest)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PerfTest)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PerfTestStatus)
