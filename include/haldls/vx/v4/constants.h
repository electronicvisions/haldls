#pragma once
#include "haldls/vx/constants.h"
#include "haldls/vx/genpybind.h"

namespace haldls::vx {
namespace v4 GENPYBIND_TAG_HALDLS_VX_V4 {

inline static const GENPYBIND(visible) Timer::Value pll_and_omnibus_settling_duration =
    haldls::vx::pll_and_omnibus_settling_duration;
inline static const GENPYBIND(visible) Timer::Value xboard_dac_settling_duration =
    haldls::vx::xboard_dac_settling_duration;
inline static const GENPYBIND(visible) Timer::Value chip_reset_high_duration =
    haldls::vx::chip_reset_high_duration;
inline static const GENPYBIND(visible) Timer::Value chip_reset_low_duration =
    haldls::vx::chip_reset_low_duration;
inline static const GENPYBIND(visible) Timer::Value reference_generator_reset_duration =
    haldls::vx::reference_generator_reset_duration;

} // namespace v4
} // namespace haldls::vx
