#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/timer.h"

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

inline static const GENPYBIND(visible) Timer::Value pll_and_omnibus_settling_duration =
    Timer::Value(Timer::Value::fpga_clock_cycles_per_us * 100);
// TODO: This value is empirically safe, but might be too long, measure this value, see Issue #3406
inline static const GENPYBIND(visible) Timer::Value xboard_dac_settling_duration =
    Timer::Value(Timer::Value::fpga_clock_cycles_per_us * 1000);
inline static const GENPYBIND(visible) Timer::Value chip_reset_high_duration = Timer::Value(10);
inline static const GENPYBIND(visible) Timer::Value chip_reset_low_duration = Timer::Value(100);
inline static const GENPYBIND(visible) Timer::Value reference_generator_reset_duration =
    Timer::Value(1000);
// Reference frequency (in Hz) for the PLL, which is created, depending on the setup, by a dedicated
// IC or FPGA. This frequency can in theory be altered but this is not implemented yet and is not
// planned to be implemented.
inline static const GENPYBIND(visible) double nominal_pll_f_reference = 5e7;

} // namespace haldls::vx
