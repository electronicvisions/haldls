#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/timer.h"

namespace haldls { namespace vx GENPYBIND_TAG_HALDLS_VX {

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

// The madc sample signal is active in one cycle at every 12-clock cycle (and can be extended via
// sample_duration_adjust, enable_madc_clock_scaling and madc_clock_scale_value). For one-channel
// ADC the sampling rate is 1/12th of the clock frequency (cf.
// https://gerrit.bioai.eu/gitweb?p=hicann-dls-private.git;a=blob_plain;f=hicann-dls/units/hx_madc/doc/MADC.pdf;hb=HEAD;js=1).
static constexpr GENPYBIND(visible) uint_fast8_t minimal_madc_clock_cycles_per_sample = 12;

} // namespace vx
} // namespace haldls
