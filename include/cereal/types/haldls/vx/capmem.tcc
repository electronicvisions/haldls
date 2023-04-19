#pragma once
#include "cereal/types/haldls/cereal.h"
#include "cereal/types/haldls/vx/capmem.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/cerealization_typed_heap_array.h"
#include "haldls/vx/capmem.h"
#include <cereal/types/variant.hpp>

namespace cereal {

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CapMemCell<Coordinates>& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP_("m_value", value.m_value));
}

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CapMemBlock<Coordinates>& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(value.m_capmem_cells));
}

template <typename Archive, typename Coordinates>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar,
    haldls::vx::CapMemBlockConfig<Coordinates>& value,
    std::uint32_t const /* version */)
{
	ar(CEREAL_NVP(value.m_enable_capmem));
	ar(CEREAL_NVP(value.m_debug_readout_enable));
	ar(CEREAL_NVP(value.m_debug_capmem_coord));
	ar(CEREAL_NVP(value.m_debug_v_ref_select));
	ar(CEREAL_NVP(value.m_debug_i_out_select));
	ar(CEREAL_NVP(value.m_debug_out_amp_bias));
	ar(CEREAL_NVP(value.m_debug_source_follower_bias));
	ar(CEREAL_NVP(value.m_debug_level_shifter_bias));
	ar(CEREAL_NVP(value.m_v_global_bias));
	ar(CEREAL_NVP(value.m_current_cell_res));
	ar(CEREAL_NVP(value.m_enable_boost));
	ar(CEREAL_NVP(value.m_boost_factor));
	ar(CEREAL_NVP(value.m_enable_autoboost));
	ar(CEREAL_NVP(value.m_prescale_pause));
	ar(CEREAL_NVP(value.m_prescale_ramp));
	ar(CEREAL_NVP(value.m_sub_counter));
	ar(CEREAL_NVP(value.m_pause_counter));
	ar(CEREAL_NVP(value.m_pulse_a));
	ar(CEREAL_NVP(value.m_pulse_b));
	ar(CEREAL_NVP(value.m_boost_a));
	ar(CEREAL_NVP(value.m_boost_b));
}

} // namespace cereal
