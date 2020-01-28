#include "haldls/vx/pll.h"

#include <array>
#include <ostream>

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

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

HALDLS_VX_DEFAULT_OSTREAM_OP(ADPLL)

namespace {

struct ADPLLLowerBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t loop_filter_int  : 5;
			uint32_t loop_filter_prop : 5;
			uint32_t loop_div_n       : 5;
			uint32_t core_div_m0      : 5;
			uint32_t core_div_m1      : 5;
			uint32_t pre_div_p0       : 2;
			uint32_t pre_div_p1       : 2;
			uint32_t pre_div_p2       : 2;
			uint32_t /*unused*/       : 1;
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
			uint32_t tune                 : 12;
			uint32_t dco_power_switch     :  6;
			uint32_t open_lock            :  1;
			uint32_t enforce_lock         :  1;
			uint32_t pfd_select           :  1;
			uint32_t lock_window          :  1;
			uint32_t filter_shift         :  2;
			uint32_t disable_output_clock :  1;
			uint32_t /*unused*/           :  5;
			uint32_t enable               :  1;
			uint32_t use_external_config  :  1;
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

	if constexpr (std::is_same<WordT, fisch::vx::JTAGPLLRegister>::value) {
		fisch::vx::JTAGPLLRegister lower;
		lower.set(fisch::vx::JTAGPLLRegister::Value(lower_bitfield.u.raw));
		fisch::vx::JTAGPLLRegister upper;
		upper.set(fisch::vx::JTAGPLLRegister::Value(upper_bitfield.u.raw));
		return {lower, upper};
	} else {
		return {fisch::vx::OmnibusData(lower_bitfield.u.raw),
		        fisch::vx::OmnibusData(upper_bitfield.u.raw)};
	}
}
template SYMBOL_VISIBLE std::array<fisch::vx::JTAGPLLRegister, ADPLL::config_size_in_words>
ADPLL::encode() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChipOverJTAG, ADPLL::config_size_in_words>
ADPLL::encode() const;

template <>
void ADPLL::decode(std::array<fisch::vx::JTAGPLLRegister, config_size_in_words> const& /*data*/)
{}

template <typename WordT>
void ADPLL::decode(std::array<WordT, config_size_in_words> const& data)
{
	ADPLLLowerBitfield lower_bitfield;
	lower_bitfield.u.raw = data[0].get();

	ADPLLUpperBitfield upper_bitfield;
	upper_bitfield.u.raw = data[1].get();

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
    std::array<fisch::vx::JTAGPLLRegister, config_size_in_words> const& /*data*/);

template SYMBOL_VISIBLE void ADPLL::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, config_size_in_words> const& data);

template <typename Archive>
void ADPLL::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_loop_filter_int));
	ar(CEREAL_NVP(m_loop_filter_prop));
	ar(CEREAL_NVP(m_loop_div_n));
	ar(CEREAL_NVP(m_core_div_m0));
	ar(CEREAL_NVP(m_core_div_m1));
	ar(CEREAL_NVP(m_pre_div_p0));
	ar(CEREAL_NVP(m_pre_div_p1));
	ar(CEREAL_NVP(m_pre_div_p2));
	ar(CEREAL_NVP(m_tune));
	ar(CEREAL_NVP(m_dco_power_switch));
	ar(CEREAL_NVP(m_open_lock));
	ar(CEREAL_NVP(m_enforce_lock));
	ar(CEREAL_NVP(m_pfd_select));
	ar(CEREAL_NVP(m_lock_window));
	ar(CEREAL_NVP(m_filter_shift));
	ar(CEREAL_NVP(m_enable_output_clock));
	ar(CEREAL_NVP(m_enable));
	ar(CEREAL_NVP(m_use_external_config));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ADPLL)


PLLClockOutputBlock::ClockOutput::ClockOutput() :
    m_enable_output(true),
    m_enable_bypass(false),
    m_adpll(),
    m_adpll_output()
{}

bool PLLClockOutputBlock::ClockOutput::get_enable_output() const
{
	return m_enable_output;
}

void PLLClockOutputBlock::ClockOutput::set_enable_output(bool const value)
{
	m_enable_output = value;
}

bool PLLClockOutputBlock::ClockOutput::get_enable_bypass() const
{
	return m_enable_bypass;
}

void PLLClockOutputBlock::ClockOutput::set_enable_bypass(bool const value)
{
	m_enable_bypass = value;
}

halco::hicann_dls::vx::ADPLLOnDLS PLLClockOutputBlock::ClockOutput::get_select_adpll() const
{
	return m_adpll;
}

void PLLClockOutputBlock::ClockOutput::set_select_adpll(
    halco::hicann_dls::vx::ADPLLOnDLS const& value)
{
	m_adpll = value;
}

PLLClockOutputBlock::ClockOutput::ADPLLOutput
PLLClockOutputBlock::ClockOutput::get_select_adpll_output() const
{
	return m_adpll_output;
}

void PLLClockOutputBlock::ClockOutput::set_select_adpll_output(ADPLLOutput const value)
{
	m_adpll_output = value;
}

bool PLLClockOutputBlock::ClockOutput::operator==(ClockOutput const& other) const
{
	return (
	    (m_enable_output == other.m_enable_output) && (m_enable_bypass == other.m_enable_bypass) &&
	    (m_adpll == other.m_adpll) && (m_adpll_output == other.m_adpll_output));
}

bool PLLClockOutputBlock::ClockOutput::operator!=(ClockOutput const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(PLLClockOutputBlock)

template <typename Archive>
void PLLClockOutputBlock::ClockOutput::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_output));
	ar(CEREAL_NVP(m_enable_bypass));
	ar(CEREAL_NVP(m_adpll));
	ar(CEREAL_NVP(m_adpll_output));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PLLClockOutputBlock::ClockOutput)

PLLClockOutputBlock::PLLClockOutputBlock() : m_output()
{
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::serdes_ser_send_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(0));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::phy_ref_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(0));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::ppu_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(1));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::madc_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(0));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::serdes_ser_send_clk]
	    .set_select_adpll_output(ClockOutput::ADPLLOutput::core_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::phy_ref_clk].set_select_adpll_output(
	    ClockOutput::ADPLLOutput::core_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::ppu_clk].set_select_adpll_output(
	    ClockOutput::ADPLLOutput::core_1);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::madc_clk].set_select_adpll_output(
	    ClockOutput::ADPLLOutput::dco);
}

PLLClockOutputBlock::ClockOutput const& PLLClockOutputBlock::get_clock_output(
    halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord) const
{
	return m_output[coord];
}

void PLLClockOutputBlock::set_clock_output(
    halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord, ClockOutput const& config)
{
	m_output[coord] = config;
}

bool PLLClockOutputBlock::operator==(PLLClockOutputBlock const& other) const
{
	return m_output == other.m_output;
}

bool PLLClockOutputBlock::operator!=(PLLClockOutputBlock const& other) const
{
	return !(*this == other);
}

namespace {

struct PLLClockOutputBlockBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t enable_output_0       : 1;
			uint32_t enable_bypass_0       : 1;
			uint32_t select_adpll_0        : 1;
			uint32_t select_output_adpll_0 : 2;
			uint32_t unused_0              : 3;
			uint32_t enable_output_1       : 1;
			uint32_t enable_bypass_1       : 1;
			uint32_t select_adpll_1        : 1;
			uint32_t select_output_adpll_1 : 2;
			uint32_t unused_1              : 3;
			uint32_t enable_output_2       : 1;
			uint32_t enable_bypass_2       : 1;
			uint32_t select_adpll_2        : 1;
			uint32_t select_output_adpll_2 : 2;
			uint32_t unused_2              : 3;
			uint32_t enable_output_3       : 1;
			uint32_t enable_bypass_3       : 1;
			uint32_t select_adpll_3        : 1;
			uint32_t select_output_adpll_3 : 2;
			uint32_t unused_3              : 3;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PLLClockOutputBlockBitfield() { u.raw = 0u; }

	PLLClockOutputBlockBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

template <>
std::array<halco::hicann_dls::vx::JTAGPLLRegisterOnDLS, PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::JTAGPLLRegisterOnDLS(4)};
}

template <>
std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(pll_base_address + 4)};
}

template SYMBOL_VISIBLE std::array<
	halco::hicann_dls::vx::JTAGPLLRegisterOnDLS,
	PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/);

template <typename WordT>
std::array<WordT, PLLClockOutputBlock::config_size_in_words> PLLClockOutputBlock::encode() const
{
	PLLClockOutputBlockBitfield bitfield;
	bitfield.u.m.enable_output_0 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_enable_output();
	bitfield.u.m.enable_bypass_0 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_enable_bypass();
	bitfield.u.m.select_adpll_0 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_select_adpll();
	bitfield.u.m.select_output_adpll_0 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_select_adpll_output());
	bitfield.u.m.unused_0 = 0b100;
	bitfield.u.m.enable_output_1 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_enable_output();
	bitfield.u.m.enable_bypass_1 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_enable_bypass();
	bitfield.u.m.select_adpll_1 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_select_adpll();
	bitfield.u.m.select_output_adpll_1 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_select_adpll_output());
	bitfield.u.m.unused_1 = 0b100;

	bitfield.u.m.enable_output_2 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_enable_output();
	bitfield.u.m.enable_bypass_2 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_enable_bypass();
	bitfield.u.m.select_adpll_2 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_select_adpll();
	bitfield.u.m.select_output_adpll_2 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_select_adpll_output());
	bitfield.u.m.unused_2 = 0b100;

	bitfield.u.m.enable_output_3 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_enable_output();
	bitfield.u.m.enable_bypass_3 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_enable_bypass();
	bitfield.u.m.select_adpll_3 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_select_adpll();
	bitfield.u.m.select_output_adpll_3 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_select_adpll_output());
	bitfield.u.m.unused_3 = 0b100;

	if constexpr (std::is_same<WordT, fisch::vx::JTAGPLLRegister>::value) {
		fisch::vx::JTAGPLLRegister reg;
		reg.set(fisch::vx::JTAGPLLRegister::Value(bitfield.u.raw));
		return {reg};
	} else {
		return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
	}
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::JTAGPLLRegister, PLLClockOutputBlock::config_size_in_words>
    PLLClockOutputBlock::encode() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, PLLClockOutputBlock::config_size_in_words>
    PLLClockOutputBlock::encode() const;

template <>
void PLLClockOutputBlock::decode(
    std::array<fisch::vx::JTAGPLLRegister, config_size_in_words> const& /*data*/)
{}

template <typename WordT>
void PLLClockOutputBlock::decode(std::array<WordT, config_size_in_words> const& data)
{
	PLLClockOutputBlockBitfield bitfield;

	bitfield.u.raw = data[0].get();

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_enable_output(
	    bitfield.u.m.enable_output_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_0));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_select_adpll_output(
	    static_cast<ClockOutput::ADPLLOutput>(bitfield.u.m.select_output_adpll_0));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_enable_output(
	    bitfield.u.m.enable_output_1);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_1);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_1));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_select_adpll_output(
	    static_cast<ClockOutput::ADPLLOutput>(bitfield.u.m.select_output_adpll_1));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_enable_output(
	    bitfield.u.m.enable_output_2);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_2);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_2));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_select_adpll_output(
	    static_cast<ClockOutput::ADPLLOutput>(bitfield.u.m.select_output_adpll_2));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_enable_output(
	    bitfield.u.m.enable_output_3);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_3);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_3));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_select_adpll_output(
	    static_cast<ClockOutput::ADPLLOutput>(bitfield.u.m.select_output_adpll_3));
}

template SYMBOL_VISIBLE void PLLClockOutputBlock::decode(
    std::array<fisch::vx::JTAGPLLRegister, config_size_in_words> const& /*data*/);

template SYMBOL_VISIBLE void PLLClockOutputBlock::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, config_size_in_words> const& /*data*/);

template <typename Archive>
void PLLClockOutputBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_output));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PLLClockOutputBlock)


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

HALDLS_VX_DEFAULT_OSTREAM_OP(PLLSelfTest)

namespace {

struct PLLSelfTestBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t clock_enable    : 1;
			uint32_t pre_scaler_p    : 4;
			uint32_t select_source   : 2;
			uint32_t /*unused*/      : 1;
			uint32_t check_range     : 4;
			uint32_t check_value     : 20;
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
	if constexpr (std::is_same<WordT, fisch::vx::JTAGPLLRegister>::value) {
		fisch::vx::JTAGPLLRegister reg;
		reg.set(fisch::vx::JTAGPLLRegister::Value(bitfield.u.raw));
		return {reg};
	} else {
		return {WordT(fisch::vx::OmnibusData(bitfield.u.raw))};
	}
}

template SYMBOL_VISIBLE std::array<fisch::vx::JTAGPLLRegister, PLLSelfTest::config_size_in_words>
PLLSelfTest::encode() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, PLLSelfTest::config_size_in_words>
    PLLSelfTest::encode() const;

template <typename WordT>
void PLLSelfTest::decode(std::array<WordT, config_size_in_words> const& data)
{
	if constexpr (!std::is_same<WordT, fisch::vx::JTAGPLLRegister>::value) {
		PLLSelfTestBitfield bitfield;
		bitfield.u.raw = data[0].get();

		m_clock_enable = bitfield.u.m.clock_enable;
		m_pre_scaler_p = PreScalerP(bitfield.u.m.pre_scaler_p);
		m_select_source = halco::hicann_dls::vx::PLLClockOutputOnDLS(bitfield.u.m.select_source);
		m_check_range = CheckRange(bitfield.u.m.check_range);
		m_check_value = CheckValue(bitfield.u.m.check_value);
	}
}

template SYMBOL_VISIBLE void PLLSelfTest::decode(
    std::array<fisch::vx::JTAGPLLRegister, config_size_in_words> const& /*data*/);

template SYMBOL_VISIBLE void PLLSelfTest::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, config_size_in_words> const& /*data*/);

template <typename Archive>
void PLLSelfTest::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_clock_enable));
	ar(CEREAL_NVP(m_pre_scaler_p));
	ar(CEREAL_NVP(m_select_source));
	ar(CEREAL_NVP(m_check_range));
	ar(CEREAL_NVP(m_check_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PLLSelfTest)


PLLSelfTestStatus::PLLSelfTestStatus() : m_success(false), m_counter_value() {}

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
	return (m_success == other.m_success && m_counter_value == other.m_counter_value);
}

bool PLLSelfTestStatus::operator!=(PLLSelfTestStatus const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(PLLSelfTestStatus)

namespace {

struct PLLSelfTestStatusBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t counter_value : 20;
			uint32_t finished      : 1;
			uint32_t success       : 1;
			uint32_t /*unused*/    : 10;
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

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, PLLSelfTestStatus::write_config_size_in_words>
    PLLSelfTestStatus::encode() const;

template <typename WordT>
void PLLSelfTestStatus::decode(std::array<WordT, read_config_size_in_words> const& data)
{
	PLLSelfTestStatusBitfield bitfield;
	bitfield.u.raw = data[0].get();

	m_finished = bitfield.u.m.finished;
	m_success = bitfield.u.m.success;
	m_counter_value = CounterValue(bitfield.u.m.counter_value);
}

template SYMBOL_VISIBLE void PLLSelfTestStatus::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, read_config_size_in_words> const& /*data*/);

template <typename Archive>
void PLLSelfTestStatus::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_success));
	ar(CEREAL_NVP(m_finished));
	ar(CEREAL_NVP(m_counter_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PLLSelfTestStatus)

} // namespace vx
} // namespace haldls
