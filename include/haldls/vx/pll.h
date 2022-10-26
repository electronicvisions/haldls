#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/pll.h"
#include "haldls/cerealization.h"
#include "haldls/vx/constants.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace halco::hicann_dls::vx {
struct JTAGPLLRegisterOnDLS;
struct OmnibusChipOverJTAGAddress;
} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for configuration of an ADPLL (All-Digital Phased-Locked-Loop) clock generator.
 * The PLL features two ADPLLs with three clock outputs each. The clock outputs are routed to the
 * PLLClockOutputBlock with four clock output ports.
 */
class GENPYBIND(visible) ADPLL
{
public:
	/** Outputs of the ADPLL. */
	enum class Output
	{
		core_0,
		core_1,
		dco
	};

	typedef halco::hicann_dls::vx::ADPLLOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Integral part of the PID controller for the DCO. */
	struct GENPYBIND(inline_base("*")) LoopFilterInt
	    : public halco::common::detail::RantWrapper<LoopFilterInt, uint_fast16_t, 31, 1>
	{
		constexpr explicit LoopFilterInt(uintmax_t const val = 2) : rant_t(val) {}
	};

	/** Proportional part of the PID controller for the DCO. */
	struct GENPYBIND(inline_base("*")) LoopFilterProp
	    : public halco::common::detail::RantWrapper<LoopFilterProp, uint_fast16_t, 31, 1>
	{
		constexpr explicit LoopFilterProp(uintmax_t const val = 8) : rant_t(val) {}
	};

	/**
	 * Divider to set the frequency of the DCO f_dco (Together with PreDivP0).
	 * f_dco = P0 * N * f_ref.
	 */
	struct GENPYBIND(inline_base("*")) LoopDivN
	    : public halco::common::detail::RantWrapper<LoopDivN, uint_fast16_t, 31, 1>
	{
		constexpr explicit LoopDivN(uintmax_t const val = 10) : rant_t(val) {}
	};

	/**
	 * Divider to set the output frequency f_clk_core0 (Together with PreDivP1).
	 * f_clk_core0 = f_dco / (P1 * M0).
	 */
	struct GENPYBIND(inline_base("*")) CoreDivM0
	    : public halco::common::detail::RantWrapper<CoreDivM0, uint_fast16_t, 31, 1>
	{
		constexpr explicit CoreDivM0(uintmax_t const val = 1) : rant_t(val) {}
	};

	/**
	 * Divider to set the output frequency f_clk_core1 (Together with PreDivP1).
	 * f_clk_core1 = f_dco / (P1 * M1).
	 */
	struct GENPYBIND(inline_base("*")) CoreDivM1
	    : public halco::common::detail::RantWrapper<CoreDivM1, uint_fast16_t, 31, 1>
	{
		constexpr explicit CoreDivM1(uintmax_t const val = 2) : rant_t(val) {}
	};

	/**
	 * Divider to set the frequency of the DCO f_dco (Together with LoopDivN).
	 * f_dco = P0 * N * f_ref.
	 */
	struct GENPYBIND(inline_base("*")) PreDivP0
	    : public halco::common::detail::RantWrapper<PreDivP0, uint_fast16_t, 4, 2>
	{
		constexpr explicit PreDivP0(uintmax_t const val = 2) : rant_t(val) {}
	};

	/**
	 * Divider for f_clk_core0 and f_clk_core1.
	 * For f_clk_core0: CoreDivM0 for f_clk_core0 = f_dco / (P1 * M0).
	 * For f_clk_core1: CoreDivM1 for f_clk_core1 = f_dco / (P1 * M1).
	 */
	struct GENPYBIND(inline_base("*")) PreDivP1
	    : public halco::common::detail::RantWrapper<PreDivP1, uint_fast16_t, 4, 2>
	{
		constexpr explicit PreDivP1(uintmax_t const val = 2) : rant_t(val) {}
	};

	/**
	 * Divider to set the output frequency f_clk_dco.
	 * f_clk_dco = f_dco / P2.
	 */
	struct GENPYBIND(inline_base("*")) PreDivP2
	    : public halco::common::detail::RantWrapper<PreDivP2, uint_fast16_t, 4, 2>
	{
		constexpr explicit PreDivP2(uintmax_t const val = 2) : rant_t(val) {}
	};

	/** Tune value of the DCO. */
	struct GENPYBIND(inline_base("*")) Tune
	    : public halco::common::detail::RantWrapper<Tune, uint_fast16_t, 4095, 0>
	{
		constexpr explicit Tune(uintmax_t const val = 512) : rant_t(val) {}
	};

	/** Number of activated PMOS header power switches during DCO operation. */
	struct GENPYBIND(inline_base("*")) DcoPowerSwitch
	    : public halco::common::detail::RantWrapper<DcoPowerSwitch, uint_fast16_t, 63, 0>
	{
		constexpr explicit DcoPowerSwitch(uintmax_t const val = 63) : rant_t(val) {}
	};

	/** Loop filter gain boost by 2^n during lock-in, leads to lock time reduction. */
	struct GENPYBIND(inline_base("*")) FilterShift
	    : public halco::common::detail::RantWrapper<FilterShift, uint_fast16_t, 3, 0>
	{
		constexpr explicit FilterShift(uintmax_t const val = 3) : rant_t(val) {}
	};

	/** Default construct ADPLL. */
	ADPLL() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(loop_filter_int))
	LoopFilterInt get_loop_filter_int() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(loop_filter_int))
	void set_loop_filter_int(LoopFilterInt value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(loop_filter_prop))
	LoopFilterProp get_loop_filter_prop() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(loop_filter_prop))
	void set_loop_filter_prop(LoopFilterProp value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(loop_div_n))
	LoopDivN get_loop_div_n() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(loop_div_n))
	void set_loop_div_n(LoopDivN value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(core_div_m0))
	CoreDivM0 get_core_div_m0() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(core_div_m0))
	void set_core_div_m0(CoreDivM0 value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(core_div_m1))
	CoreDivM1 get_core_div_m1() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(core_div_m1))
	void set_core_div_m1(CoreDivM1 value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(pre_div_p0))
	PreDivP0 get_pre_div_p0() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(pre_div_p0))
	void set_pre_div_p0(PreDivP0 value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(pre_div_p1))
	PreDivP1 get_pre_div_p1() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(pre_div_p1))
	void set_pre_div_p1(PreDivP1 value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(pre_div_p2))
	PreDivP2 get_pre_div_p2() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(pre_div_p2))
	void set_pre_div_p2(PreDivP2 value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(tune))
	Tune get_tune() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(tune))
	void set_tune(Tune value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(dco_power_switch))
	DcoPowerSwitch get_dco_power_switch() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(dco_power_switch))
	void set_dco_power_switch(DcoPowerSwitch value) SYMBOL_VISIBLE;

	/**
	 * Get whether open loop mode of the DCO is enabled.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(open_lock))
	bool get_open_lock() const SYMBOL_VISIBLE;

	/**
	 * Set enable open loop mode of the DCO.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(open_lock))
	void set_open_lock(bool value) SYMBOL_VISIBLE;

	/**
	 * Get whether the PLL signal is enforced to be locked.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enforce_lock))
	bool get_enforce_lock() const SYMBOL_VISIBLE;

	/**
	 * Set to enforce a locked PLL signal.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(enforce_lock))
	void set_enforce_lock(bool value) SYMBOL_VISIBLE;

	/**
	 * Get whether the pfd bit is set (with/without synchronizer).
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(pfd_select))
	bool get_pfd_select() const SYMBOL_VISIBLE;

	/**
	 * Set the pfd bit (with/without synchronizer).
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(pfd_select))
	void set_pfd_select(bool value) SYMBOL_VISIBLE;

	/**
	 * Get the lock detection timing window (false: long, true: short).
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(lock_window))
	bool get_lock_window() const SYMBOL_VISIBLE;

	/**
	 * Set the lock detection timing window (false: long, true: short).
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(lock_window))
	void set_lock_window(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(filter_shift))
	FilterShift get_filter_shift() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(filter_shift))
	void set_filter_shift(FilterShift value) SYMBOL_VISIBLE;

	/**
	 * Get whether output clock is enabled.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_output_clock))
	bool get_enable_output_clock() const SYMBOL_VISIBLE;

	/**
	 * Set whether to enable output clock.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(enable_output_clock))
	void set_enable_output_clock(bool value) SYMBOL_VISIBLE;

	/**
	 * Get whether the ADPLL is enabled.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set to enable the ADPLL.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * TODO: What exactly gets this?
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(use_external_config))
	bool get_use_external_config() const SYMBOL_VISIBLE;

	/**
	 * TODO: What exactly sets this?.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(use_external_config))
	void set_use_external_config(bool value) SYMBOL_VISIBLE;

	/**
	 * Calculate the frequency for the different outputs on the ADPLL. Frequencies are returned in
	 * Hz.
	 * @param output Ouput for which the frequency shoud be returned
	 * @param f_ref Nominal reference frequency the PLL uses as input
	 * @return Frequency in Hz at specified output
	 */
	double calculate_output_frequency(
	    Output const& output, double const& f_ref = nominal_pll_f_reference) const SYMBOL_VISIBLE;

	bool operator==(ADPLL const& other) const SYMBOL_VISIBLE;
	bool operator!=(ADPLL const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ADPLL const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	LoopFilterInt m_loop_filter_int;
	LoopFilterProp m_loop_filter_prop;
	LoopDivN m_loop_div_n;
	CoreDivM0 m_core_div_m0;
	CoreDivM1 m_core_div_m1;
	PreDivP0 m_pre_div_p0;
	PreDivP1 m_pre_div_p1;
	PreDivP2 m_pre_div_p2;
	Tune m_tune;
	DcoPowerSwitch m_dco_power_switch;
	bool m_open_lock;
	bool m_enforce_lock;
	bool m_pfd_select;
	bool m_lock_window;
	FilterShift m_filter_shift;
	bool m_enable_output_clock;
	bool m_enable;
	bool m_use_external_config;
};

std::ostream& operator<<(std::ostream&, ADPLL::Output const&) SYMBOL_VISIBLE;

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ADPLL)

namespace detail {

/**
 * The JTAGPLLRegister backend only provides write functionality, but is safe with respect to
 * changing the ADPLL config. The OmnibusChipOverJTAG access provides read and write functionality
 * but is unsafe if the omnibus clock is altered and thus reinitializes itself. During
 * reinitialization-time, OmnibusChipOverJTAG requests are dropped.
 */
template <>
struct BackendContainerTrait<ADPLL>
    : public BackendContainerBase<
          ADPLL,
          fisch::vx::word_access_type::JTAGPLLRegister,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container for configuration and triggering of the PLL internal self test.
 */
class GENPYBIND(visible) PLLSelfTest
{
public:
	typedef halco::hicann_dls::vx::PLLSelfTestOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Selects counting window of the self test.
	 * Counts within 2^(p + 2) reference cycles.
	 */
	struct GENPYBIND(inline_base("*")) PreScalerP
	    : public halco::common::detail::RantWrapper<PreScalerP, uint_fast16_t, 15, 0>
	{
		constexpr explicit PreScalerP(uintmax_t const val = 0) : rant_t(val) {}
	};

	/**
	 * Acceptance range for the internal counter compared to the expected counter.
	 * Decides to let the test PASS or FAIL.
	 */
	struct GENPYBIND(inline_base("*")) CheckRange
	    : public halco::common::detail::RantWrapper<CheckRange, uint_fast16_t, 15, 0>
	{
		constexpr explicit CheckRange(uintmax_t const val = 0) : rant_t(val) {}
	};

	/**
	 * Expected counter value within the 2^(p + 2) reference cycles.
	 * counter_value = (f_clk / f ref) * 2^(p + 2)
	 */
	struct GENPYBIND(inline_base("*")) CheckValue
	    : public halco::common::detail::
	          RantWrapper<CheckValue, uint_fast16_t, 1048575 /*2^20-1*/, 0>
	{
		constexpr explicit CheckValue(uintmax_t const val = 0) : rant_t(val) {}
	};

	/** Default construct PLL self test. */
	PLLSelfTest() SYMBOL_VISIBLE;

	/**
	 * Get wheter the clock is enabled for the self-test.
	 * Before executing a valid test, this has to be written to be false.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(clock_enable))
	bool get_clock_enable() const SYMBOL_VISIBLE;

	/**
	 * Set to enable the clock for the self-test.
	 * Before executing a valid test, this has to be written to be false.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(clock_enable))
	void set_clock_enable(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(pre_scaler_p))
	PreScalerP get_pre_scaler_p() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(pre_scaler_p))
	void set_pre_scaler_p(PreScalerP value) SYMBOL_VISIBLE;

	/**
	 * Get selected source output to test.
	 * @return Selected PLLClockOutputOnDLS
	 */
	GENPYBIND(getter_for(select_source))
	halco::hicann_dls::vx::PLLClockOutputOnDLS get_select_source() const SYMBOL_VISIBLE;

	/**
	 * Set selected source output to test.
	 * @param To be selected PLLClockOutputOnDLS
	 */
	GENPYBIND(setter_for(select_source))
	void set_select_source(halco::hicann_dls::vx::PLLClockOutputOnDLS value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(check_range))
	CheckRange get_check_range() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(check_range))
	void set_check_range(CheckRange value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(check_value))
	CheckValue get_check_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(check_value))
	void set_check_value(CheckValue value) SYMBOL_VISIBLE;

	bool operator==(PLLSelfTest const& other) const SYMBOL_VISIBLE;
	bool operator!=(PLLSelfTest const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PLLSelfTest const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_clock_enable;
	PreScalerP m_pre_scaler_p;
	halco::hicann_dls::vx::PLLClockOutputOnDLS m_select_source;
	CheckRange m_check_range;
	CheckValue m_check_value;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PLLSelfTest)

namespace detail {

template <>
struct BackendContainerTrait<PLLSelfTest>
    : public BackendContainerBase<
          PLLSelfTest,
          fisch::vx::word_access_type::JTAGPLLRegister,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container of PLL self-test status data.
 */
class GENPYBIND(visible) PLLSelfTestStatus
{
public:
	typedef halco::hicann_dls::vx::PLLSelfTestStatusOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Measured counter value in previous self-test execution.
	 */
	struct GENPYBIND(inline_base("*")) CounterValue
	    : public halco::common::detail::
	          RantWrapper<CounterValue, uint_fast16_t, 1048575 /*2^20-1*/, 0>
	{
		constexpr explicit CounterValue(uintmax_t const val = 0) : rant_t(val) {}
	};

	/**
	 * Default construct a PLL self-test status.
	 */
	PLLSelfTestStatus() SYMBOL_VISIBLE;

	/**
	 * Get if self-test execution was successful.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(success))
	bool get_success() const SYMBOL_VISIBLE;

	/**
	 * Get if self-test exectution finished before this containers' read.
	 * This flag only produces reliable information on the first validly
	 * finished self test after power cycle.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(finished))
	bool get_finished() const SYMBOL_VISIBLE;

	/**
	 * Get self-test clock period counter value.
	 * @return Counter value
	 */
	GENPYBIND(getter_for(counter_value))
	CounterValue get_counter_value() const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PLLSelfTestStatus const& config)
	    SYMBOL_VISIBLE;

	bool operator==(PLLSelfTestStatus const& other) const SYMBOL_VISIBLE;
	bool operator!=(PLLSelfTestStatus const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_success;
	bool m_finished;
	CounterValue m_counter_value;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PLLSelfTestStatus)

namespace detail {

template <>
struct BackendContainerTrait<PLLSelfTestStatus>
    : public BackendContainerBase<
          PLLSelfTestStatus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::LoopFilterInt)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::LoopFilterProp)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::LoopDivN)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::CoreDivM0)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::CoreDivM1)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::PreDivP0)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::PreDivP1)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::PreDivP2)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::Tune)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::DcoPowerSwitch)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ADPLL::FilterShift)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PLLSelfTest::PreScalerP)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PLLSelfTest::CheckRange)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PLLSelfTest::CheckValue)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PLLSelfTestStatus::CounterValue)

} // namespace std
