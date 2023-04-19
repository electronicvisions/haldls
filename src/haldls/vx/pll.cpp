#include "haldls/vx/pll.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/bitfield.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/traits.h"
#include "hate/indent.h"
#include "hate/join.h"

namespace haldls {
namespace vx {

ADPLL::ADPLL() :
    m_loop_filter_int(),
    m_loop_filter_prop(),
    m_loop_div_n(),
    m_core_div_m0(),
    m_core_div_m1(),
    m_pre_div_p0(),
    m_pre_div_p1(),
    m_pre_div_p2(),
    m_tune(),
    m_dco_power_switch(),
    m_open_lock(false),
    m_enforce_lock(true),
    m_pfd_select(false),
    m_lock_window(false),
    m_filter_shift(),
    m_enable_output_clock(true),
    m_enable(true),
    m_use_external_config(true)
{}

ADPLL::LoopFilterInt ADPLL::get_loop_filter_int() const
{
	return m_loop_filter_int;
}

void ADPLL::set_loop_filter_int(LoopFilterInt const value)
{
	m_loop_filter_int = value;
}

ADPLL::LoopFilterProp ADPLL::get_loop_filter_prop() const
{
	return m_loop_filter_prop;
}

void ADPLL::set_loop_filter_prop(LoopFilterProp const value)
{
	m_loop_filter_prop = value;
}

ADPLL::LoopDivN ADPLL::get_loop_div_n() const
{
	return m_loop_div_n;
}

void ADPLL::set_loop_div_n(LoopDivN const value)
{
	m_loop_div_n = value;
}

ADPLL::CoreDivM0 ADPLL::get_core_div_m0() const
{
	return m_core_div_m0;
}

void ADPLL::set_core_div_m0(CoreDivM0 const value)
{
	m_core_div_m0 = value;
}

ADPLL::CoreDivM1 ADPLL::get_core_div_m1() const
{
	return m_core_div_m1;
}

void ADPLL::set_core_div_m1(CoreDivM1 const value)
{
	m_core_div_m1 = value;
}

ADPLL::PreDivP0 ADPLL::get_pre_div_p0() const
{
	return m_pre_div_p0;
}

void ADPLL::set_pre_div_p0(PreDivP0 const value)
{
	m_pre_div_p0 = value;
}

ADPLL::PreDivP1 ADPLL::get_pre_div_p1() const
{
	return m_pre_div_p1;
}

void ADPLL::set_pre_div_p1(PreDivP1 const value)
{
	m_pre_div_p1 = value;
}

ADPLL::PreDivP2 ADPLL::get_pre_div_p2() const
{
	return m_pre_div_p2;
}

void ADPLL::set_pre_div_p2(PreDivP2 const value)
{
	m_pre_div_p2 = value;
}

ADPLL::Tune ADPLL::get_tune() const
{
	return m_tune;
}

void ADPLL::set_tune(Tune const value)
{
	m_tune = value;
}

ADPLL::DcoPowerSwitch ADPLL::get_dco_power_switch() const
{
	return m_dco_power_switch;
}

void ADPLL::set_dco_power_switch(DcoPowerSwitch const value)
{
	m_dco_power_switch = value;
}

bool ADPLL::get_open_lock() const
{
	return m_open_lock;
}

void ADPLL::set_open_lock(bool const value)
{
	m_open_lock = value;
}

bool ADPLL::get_enforce_lock() const
{
	return m_enforce_lock;
}

void ADPLL::set_enforce_lock(bool const value)
{
	m_enforce_lock = value;
}

bool ADPLL::get_pfd_select() const
{
	return m_pfd_select;
}

void ADPLL::set_pfd_select(bool const value)
{
	m_pfd_select = value;
}

bool ADPLL::get_lock_window() const
{
	return m_lock_window;
}

void ADPLL::set_lock_window(bool const value)
{
	m_lock_window = value;
}

ADPLL::FilterShift ADPLL::get_filter_shift() const
{
	return m_filter_shift;
}

void ADPLL::set_filter_shift(FilterShift const value)
{
	m_filter_shift = value;
}

bool ADPLL::get_enable_output_clock() const
{
	return m_enable_output_clock;
}

void ADPLL::set_enable_output_clock(bool const value)
{
	m_enable_output_clock = value;
}

bool ADPLL::get_enable() const
{
	return m_enable;
}

void ADPLL::set_enable(bool const value)
{
	m_enable = value;
}

bool ADPLL::get_use_external_config() const
{
	return m_use_external_config;
}

void ADPLL::set_use_external_config(bool const value)
{
	m_use_external_config = value;
}

double ADPLL::calculate_output_frequency(
    Output const& output, double const& f_ref /* PLL_f_reference*/) const
{
	double frequency_dco = m_pre_div_p0 * m_loop_div_n * f_ref;
	switch (output) {
		case Output::dco:
			return frequency_dco / m_pre_div_p2;
		case Output::core_0:
			return frequency_dco / (m_pre_div_p1 * m_core_div_m0);
		case Output::core_1:
			return frequency_dco / (m_pre_div_p1 * m_core_div_m1);
		default:
#ifndef __ppu__
			throw std::runtime_error("Unsupported ADPLL output was chosen.");
#else
			exit(1);
#endif
	}
}

bool ADPLL::operator==(ADPLL const& other) const
{
	return (
	    m_loop_filter_int == other.m_loop_filter_int &&
	    m_loop_filter_prop == other.m_loop_filter_prop && m_loop_div_n == other.m_loop_div_n &&
	    m_core_div_m0 == other.m_core_div_m0 && m_core_div_m1 == other.m_core_div_m1 &&
	    m_pre_div_p0 == other.m_pre_div_p0 && m_pre_div_p1 == other.m_pre_div_p1 &&
	    m_pre_div_p2 == other.m_pre_div_p2 && m_tune == other.m_tune &&
	    m_enable == other.m_enable && m_use_external_config == other.m_use_external_config &&
	    m_dco_power_switch == other.m_dco_power_switch && m_open_lock == other.m_open_lock &&
	    m_enforce_lock == other.m_enforce_lock && m_pfd_select == other.m_pfd_select &&
	    m_lock_window == other.m_lock_window && m_filter_shift == other.m_filter_shift &&
	    m_enable_output_clock == other.m_enable_output_clock);
}

bool ADPLL::operator!=(ADPLL const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ADPLL const& config)
{
	std::stringstream ss;
	ss << "ADPLL(\n" << std::boolalpha;
	ss << "\tloop_filter_int:     \t" << config.m_loop_filter_int << "\n";
	ss << "\tloop_filter_prop:    \t" << config.m_loop_filter_prop << "\n";
	ss << "\tloop_div_n:          \t" << config.m_loop_div_n << "\n";
	ss << "\tcore_div_m0:         \t" << config.m_core_div_m0 << "\n";
	ss << "\tcore_div_m1:         \t" << config.m_core_div_m1 << "\n";
	ss << "\tpre_div_p0:          \t" << config.m_pre_div_p0 << "\n";
	ss << "\tpre_div_p1:          \t" << config.m_pre_div_p1 << "\n";
	ss << "\tpre_div_p2:          \t" << config.m_pre_div_p2 << "\n";
	ss << "\ttune:                \t" << config.m_tune << "\n";
	ss << "\tfilter_shift:        \t" << config.m_filter_shift << "\n";
	ss << "\tenable:              \t" << config.m_enable << "\n";
	ss << "\tuse_external_config: \t" << config.m_use_external_config << "\n";
	ss << "\tdco_power_switch:    \t" << config.m_dco_power_switch << "\n";
	ss << "\topen_lock:           \t" << config.m_open_lock << "\n";
	ss << "\tenforce_lock:        \t" << config.m_enforce_lock << "\n";
	ss << "\tpfd_select:          \t" << config.m_pfd_select << "\n";
	ss << "\tlock_window:         \t" << config.m_lock_window << "\n";
	ss << "\tenable_output_clock: \t" << config.m_enable_output_clock << "\n)";
	return (os << ss.str());
}

std::ostream& operator<<(std::ostream& os, ADPLL::Output const& config)
{
	switch (config) {
		case ADPLL::Output::core_0: {
			os << "core_0";
			break;
		}
		case ADPLL::Output::core_1: {
			os << "core_1";
			break;
		}
		case ADPLL::Output::dco: {
			os << "dco";
			break;
		}
		default: {
#ifndef __ppu__
			throw std::logic_error("Unknown ADPLLOutput.");
#else
			exit(1);
#endif
		}
	}
	return os;
}

namespace {

struct ADPLLLowerBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define BITFIELD \
			(uint32_t loop_filter_int  : 5;) \
			(uint32_t loop_filter_prop : 5;) \
			(uint32_t loop_div_n       : 5;) \
			(uint32_t core_div_m0      : 5;) \
			(uint32_t core_div_m1      : 5;) \
			(uint32_t pre_div_p0       : 2;) \
			(uint32_t pre_div_p1       : 2;) \
			(uint32_t pre_div_p2       : 2;) \
			(uint32_t /*unused*/       : 1;)
			EXPAND_BITFIELD_ELEMENTS(BITFIELD)
#undef BITFIELD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ADPLLLowerBitfield() { u.raw = 0u; }

	ADPLLLowerBitfield(uint32_t data) { u.raw = data; }
};

struct ADPLLUpperBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define BITFIELD \
			(uint32_t tune                 : 12;) \
			(uint32_t dco_power_switch     :  6;) \
			(uint32_t open_lock            :  1;) \
			(uint32_t enforce_lock         :  1;) \
			(uint32_t pfd_select           :  1;) \
			(uint32_t lock_window          :  1;) \
			(uint32_t filter_shift         :  2;) \
			(uint32_t disable_output_clock :  1;) \
			(uint32_t /*unused*/           :  5;) \
			(uint32_t enable               :  1;) \
			(uint32_t use_external_config  :  1;)
			EXPAND_BITFIELD_ELEMENTS(BITFIELD)
#undef BITFIELD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ADPLLUpperBitfield() { u.raw = 0u; }

	ADPLLUpperBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <>
std::array<halco::hicann_dls::vx::JTAGPLLRegisterOnDLS, ADPLL::config_size_in_words>
ADPLL::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::JTAGPLLRegisterOnDLS(0 + coord.toEnum() * 2),
	        halco::hicann_dls::vx::JTAGPLLRegisterOnDLS(1 + coord.toEnum() * 2)};
}

template <>
std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, ADPLL::config_size_in_words>
ADPLL::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(pll_base_address + 0 + coord.toEnum() * 2),
	        halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(pll_base_address + 1 + coord.toEnum() * 2)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::JTAGPLLRegisterOnDLS, ADPLL::config_size_in_words>
    ADPLL::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, ADPLL::config_size_in_words>
    ADPLL::addresses(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, ADPLL::config_size_in_words> ADPLL::encode() const
{
	ADPLLLowerBitfield lower_bitfield;
	lower_bitfield.u.m.loop_filter_int = m_loop_filter_int;
	lower_bitfield.u.m.loop_filter_prop = m_loop_filter_prop;
	lower_bitfield.u.m.loop_div_n = m_loop_div_n;
	lower_bitfield.u.m.core_div_m0 = m_core_div_m0;
	lower_bitfield.u.m.core_div_m1 = m_core_div_m1;
	lower_bitfield.u.m.pre_div_p0 = m_pre_div_p0 - 1;
	lower_bitfield.u.m.pre_div_p1 = m_pre_div_p1 - 1;
	lower_bitfield.u.m.pre_div_p2 = m_pre_div_p2 - 1;

	ADPLLUpperBitfield upper_bitfield;
	upper_bitfield.u.m.tune = m_tune;
	upper_bitfield.u.m.dco_power_switch = m_dco_power_switch;
	upper_bitfield.u.m.open_lock = m_open_lock;
	upper_bitfield.u.m.enforce_lock = m_enforce_lock;
	upper_bitfield.u.m.pfd_select = m_pfd_select;
	upper_bitfield.u.m.lock_window = m_lock_window;
	upper_bitfield.u.m.filter_shift = m_filter_shift;
	upper_bitfield.u.m.disable_output_clock = !m_enable_output_clock;
	upper_bitfield.u.m.enable = m_enable;
	upper_bitfield.u.m.use_external_config = m_use_external_config;

	return {WordT(lower_bitfield.u.raw), WordT(upper_bitfield.u.raw)};
}
template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::JTAGPLLRegister, ADPLL::config_size_in_words>
    ADPLL::encode() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, ADPLL::config_size_in_words>
    ADPLL::encode() const;

template <>
void ADPLL::decode(
    std::array<fisch::vx::word_access_type::JTAGPLLRegister, config_size_in_words> const& /*data*/)
{}

template <typename WordT>
void ADPLL::decode(std::array<WordT, config_size_in_words> const& data)
{
	ADPLLLowerBitfield lower_bitfield;
	lower_bitfield.u.raw = data[0];

	ADPLLUpperBitfield upper_bitfield;
	upper_bitfield.u.raw = data[1];

	m_loop_filter_int = LoopFilterInt(lower_bitfield.u.m.loop_filter_int);
	m_loop_filter_prop = LoopFilterProp(lower_bitfield.u.m.loop_filter_prop);
	m_loop_div_n = LoopDivN(lower_bitfield.u.m.loop_div_n);
	m_core_div_m0 = CoreDivM0(lower_bitfield.u.m.core_div_m0);
	m_core_div_m1 = CoreDivM1(lower_bitfield.u.m.core_div_m1);
	m_pre_div_p0 = PreDivP0(lower_bitfield.u.m.pre_div_p0 + 1);
	m_pre_div_p1 = PreDivP1(lower_bitfield.u.m.pre_div_p1 + 1);
	m_pre_div_p2 = PreDivP2(lower_bitfield.u.m.pre_div_p2 + 1);

	m_tune = Tune(upper_bitfield.u.m.tune);
	m_dco_power_switch = DcoPowerSwitch(upper_bitfield.u.m.dco_power_switch);
	m_open_lock = upper_bitfield.u.m.open_lock;
	m_enforce_lock = upper_bitfield.u.m.enforce_lock;
	m_pfd_select = upper_bitfield.u.m.pfd_select;
	m_lock_window = upper_bitfield.u.m.lock_window;
	m_filter_shift = FilterShift(upper_bitfield.u.m.filter_shift);
	m_enable_output_clock = !upper_bitfield.u.m.disable_output_clock;
	m_enable = upper_bitfield.u.m.enable;
	m_use_external_config = upper_bitfield.u.m.use_external_config;
}

template SYMBOL_VISIBLE void ADPLL::decode(
    std::array<fisch::vx::word_access_type::JTAGPLLRegister, config_size_in_words> const& /*data*/);

template SYMBOL_VISIBLE void ADPLL::decode(
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, config_size_in_words> const& data);


PLLSelfTest::PLLSelfTest() :
    m_clock_enable(false),
    m_pre_scaler_p(),
    m_select_source(),
    m_check_range(),
    m_check_value()
{}

bool PLLSelfTest::get_clock_enable() const
{
	return m_clock_enable;
}

void PLLSelfTest::set_clock_enable(bool const value)
{
	m_clock_enable = value;
}

PLLSelfTest::PreScalerP PLLSelfTest::get_pre_scaler_p() const
{
	return m_pre_scaler_p;
}

void PLLSelfTest::set_pre_scaler_p(PreScalerP const value)
{
	m_pre_scaler_p = value;
}

halco::hicann_dls::vx::PLLClockOutputOnDLS PLLSelfTest::get_select_source() const
{
	return m_select_source;
}

void PLLSelfTest::set_select_source(halco::hicann_dls::vx::PLLClockOutputOnDLS const value)
{
	m_select_source = value;
}

PLLSelfTest::CheckRange PLLSelfTest::get_check_range() const
{
	return m_check_range;
}

void PLLSelfTest::set_check_range(CheckRange const value)
{
	m_check_range = value;
}

PLLSelfTest::CheckValue PLLSelfTest::get_check_value() const
{
	return m_check_value;
}

void PLLSelfTest::set_check_value(CheckValue const value)
{
	m_check_value = value;
}

bool PLLSelfTest::operator==(PLLSelfTest const& other) const
{
	return (
	    m_clock_enable == other.m_clock_enable && m_pre_scaler_p == other.m_pre_scaler_p &&
	    m_select_source == other.m_select_source && m_check_range == other.m_check_range &&
	    m_check_value == other.m_check_value);
}

bool PLLSelfTest::operator!=(PLLSelfTest const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PLLSelfTest const& config)
{
	std::stringstream ss;
	ss << "PLLSelfTest(clock_enable: " << std::boolalpha << config.m_clock_enable << ", "
	   << config.m_pre_scaler_p << ", " << config.m_select_source << ", " << config.m_check_range
	   << ", " << config.m_check_value << ")";
	return (os << ss.str());
}

namespace {

struct PLLSelfTestBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define BITFIELD \
			(uint32_t clock_enable    :  1;) \
			(uint32_t pre_scaler_p    :  4;) \
			(uint32_t select_source   :  2;) \
			(uint32_t /*unused*/      :  1;) \
			(uint32_t check_range     :  4;) \
			(uint32_t check_value     : 20;)
			EXPAND_BITFIELD_ELEMENTS(BITFIELD)
#undef BITFIELD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PLLSelfTestBitfield() { u.raw = 0u; }

	PLLSelfTestBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <>
std::array<halco::hicann_dls::vx::JTAGPLLRegisterOnDLS, PLLSelfTest::config_size_in_words>
PLLSelfTest::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::JTAGPLLRegisterOnDLS(5)};
}

template <>
std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PLLSelfTest::config_size_in_words>
PLLSelfTest::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(pll_base_address + 5)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PLLSelfTest::config_size_in_words>
    PLLSelfTest::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::JTAGPLLRegisterOnDLS, PLLSelfTest::config_size_in_words>
    PLLSelfTest::addresses(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, PLLSelfTest::config_size_in_words> PLLSelfTest::encode() const
{
	PLLSelfTestBitfield bitfield;
	bitfield.u.m.clock_enable = m_clock_enable;
	bitfield.u.m.pre_scaler_p = m_pre_scaler_p;
	bitfield.u.m.select_source = m_select_source;
	bitfield.u.m.check_range = m_check_range;
	bitfield.u.m.check_value = m_check_value;
	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::JTAGPLLRegister, PLLSelfTest::config_size_in_words>
    PLLSelfTest::encode() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, PLLSelfTest::config_size_in_words>
    PLLSelfTest::encode() const;

template <typename WordT>
void PLLSelfTest::decode(std::array<WordT, config_size_in_words> const& data)
{
	if constexpr (!std::is_same<WordT, fisch::vx::word_access_type::JTAGPLLRegister>::value) {
		PLLSelfTestBitfield bitfield;
		bitfield.u.raw = data[0];

		m_clock_enable = bitfield.u.m.clock_enable;
		m_pre_scaler_p = PreScalerP(bitfield.u.m.pre_scaler_p);
		m_select_source = halco::hicann_dls::vx::PLLClockOutputOnDLS(bitfield.u.m.select_source);
		m_check_range = CheckRange(bitfield.u.m.check_range);
		m_check_value = CheckValue(bitfield.u.m.check_value);
	}
}

template SYMBOL_VISIBLE void PLLSelfTest::decode(
    std::array<fisch::vx::word_access_type::JTAGPLLRegister, config_size_in_words> const& /*data*/);

template SYMBOL_VISIBLE void PLLSelfTest::decode(std::array<
                                                 fisch::vx::word_access_type::OmnibusChipOverJTAG,
                                                 config_size_in_words> const& /*data*/);


PLLSelfTestStatus::PLLSelfTestStatus() : m_success(false), m_finished(false), m_counter_value() {}

bool PLLSelfTestStatus::get_success() const
{
	return m_success;
}

bool PLLSelfTestStatus::get_finished() const
{
	return m_finished;
}

PLLSelfTestStatus::CounterValue PLLSelfTestStatus::get_counter_value() const
{
	return m_counter_value;
}

bool PLLSelfTestStatus::operator==(PLLSelfTestStatus const& other) const
{
	return (
	    m_success == other.m_success && m_finished == other.m_finished &&
	    m_counter_value == other.m_counter_value);
}

bool PLLSelfTestStatus::operator!=(PLLSelfTestStatus const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PLLSelfTestStatus const& config)
{
	std::stringstream ss;
	ss << "PLLSelfTestStatus(" << std::boolalpha << config.m_counter_value
	   << ", finished: " << config.m_finished << ", success: " << config.m_success << ")";
	return (os << ss.str());
}

namespace {

struct PLLSelfTestStatusBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define BITFIELD \
			(uint32_t counter_value : 20;) \
			(uint32_t finished      :  1;) \
			(uint32_t success       :  1;) \
			(uint32_t /*unused*/    : 10;)
			EXPAND_BITFIELD_ELEMENTS(BITFIELD)
#undef BITFIELD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PLLSelfTestStatusBitfield() { u.raw = 0u; }

	PLLSelfTestStatusBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <>
std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLSelfTestStatus::read_config_size_in_words>
PLLSelfTestStatus::read_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(pll_base_address + 7)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLSelfTestStatus::read_config_size_in_words>
PLLSelfTestStatus::read_addresses(coordinate_type const& coord);

template <>
std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLSelfTestStatus::write_config_size_in_words>
PLLSelfTestStatus::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLSelfTestStatus::write_config_size_in_words>
PLLSelfTestStatus::write_addresses(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, PLLSelfTestStatus::write_config_size_in_words> PLLSelfTestStatus::encode() const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    PLLSelfTestStatus::write_config_size_in_words>
PLLSelfTestStatus::encode() const;

template <typename WordT>
void PLLSelfTestStatus::decode(std::array<WordT, read_config_size_in_words> const& data)
{
	PLLSelfTestStatusBitfield bitfield;
	bitfield.u.raw = data[0];

	m_finished = bitfield.u.m.finished;
	m_success = bitfield.u.m.success;
	m_counter_value = CounterValue(bitfield.u.m.counter_value);
}

template SYMBOL_VISIBLE void PLLSelfTestStatus::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        read_config_size_in_words> const& /*data*/);

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ADPLL)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PLLSelfTest)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::PLLSelfTestStatus)
