#include "cereal/types/haldls/vx/pll.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/pll.h"

namespace cereal {

template <typename Archive>
void serialize(Archive& ar, haldls::vx::ADPLL& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_loop_filter_int));
	ar(CEREAL_NVP(value.m_loop_filter_prop));
	ar(CEREAL_NVP(value.m_loop_div_n));
	ar(CEREAL_NVP(value.m_core_div_m0));
	ar(CEREAL_NVP(value.m_core_div_m1));
	ar(CEREAL_NVP(value.m_pre_div_p0));
	ar(CEREAL_NVP(value.m_pre_div_p1));
	ar(CEREAL_NVP(value.m_pre_div_p2));
	ar(CEREAL_NVP(value.m_tune));
	ar(CEREAL_NVP(value.m_dco_power_switch));
	ar(CEREAL_NVP(value.m_open_lock));
	ar(CEREAL_NVP(value.m_enforce_lock));
	ar(CEREAL_NVP(value.m_pfd_select));
	ar(CEREAL_NVP(value.m_lock_window));
	ar(CEREAL_NVP(value.m_filter_shift));
	ar(CEREAL_NVP(value.m_enable_output_clock));
	ar(CEREAL_NVP(value.m_enable));
	ar(CEREAL_NVP(value.m_use_external_config));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::PLLSelfTest& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_clock_enable));
	ar(CEREAL_NVP(value.m_pre_scaler_p));
	ar(CEREAL_NVP(value.m_select_source));
	ar(CEREAL_NVP(value.m_check_range));
	ar(CEREAL_NVP(value.m_check_value));
}

template <typename Archive>
void serialize(Archive& ar, haldls::vx::PLLSelfTestStatus& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_success));
	ar(CEREAL_NVP(value.m_finished));
	ar(CEREAL_NVP(value.m_counter_value));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::ADPLL)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PLLSelfTest)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PLLSelfTestStatus)
CEREAL_REGISTER_TYPE(haldls::vx::ADPLL)
CEREAL_REGISTER_TYPE(haldls::vx::PLLSelfTest)
CEREAL_REGISTER_TYPE(haldls::vx::PLLSelfTestStatus)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::ADPLL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PLLSelfTest)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PLLSelfTestStatus)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_pll)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::ADPLL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PLLSelfTest)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PLLSelfTestStatus)
