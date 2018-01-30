#pragma once


#include "halco/common/genpybind.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/optional.h"
#include "haldls/common/visibility.h"
#include "haldls/v2/common.h"

namespace haldls {
namespace v2 GENPYBIND(tag(haldls_v2)) {
namespace detail {

template <ocp_address_type::value_type Address>
class FlyspiRWRegister
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	constexpr explicit FlyspiRWRegister(ocp_word_type::value_type const value = 0) HALDLS_VISIBLE
		: m_value(value){};

	// accessor
	ocp_word_type::value_type get_value() const HALDLS_VISIBLE;
	void set_value(ocp_word_type::value_type const value) HALDLS_VISIBLE;

	// comparison
	bool operator==(FlyspiRWRegister<Address> const& other) const HALDLS_VISIBLE;
	bool operator!=(FlyspiRWRegister<Address> const& other) const HALDLS_VISIBLE;

	// encode/decode
	std::array<ocp_address_type, 1> write_addresses(coordinate_type const& /*unique*/) const
		HALDLS_VISIBLE;
	std::array<ocp_address_type, 1> read_addresses(coordinate_type const& /*unique*/) const
		HALDLS_VISIBLE;
	std::array<ocp_word_type, 1> encode(coordinate_type const& /*unique*/) const HALDLS_VISIBLE;
	void decode(std::array<ocp_word_type, 1> const& data) HALDLS_VISIBLE;

private:
	ocp_word_type::value_type m_value;
};

template <ocp_address_type::value_type Address>
class FlyspiRORegister
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	constexpr explicit FlyspiRORegister() HALDLS_VISIBLE : m_value(){};

	// accessor
	common::optional<ocp_word_type::value_type> get_value() const HALDLS_VISIBLE;

	// comparison
	bool operator==(FlyspiRORegister<Address> const& other) const HALDLS_VISIBLE;
	bool operator!=(FlyspiRORegister<Address> const& other) const HALDLS_VISIBLE;

	// encode/decode
	std::array<ocp_address_type, 0> write_addresses(coordinate_type const& /*unique*/) const
		HALDLS_VISIBLE;
	std::array<ocp_address_type, 1> read_addresses(coordinate_type const& /*unique*/) const
		HALDLS_VISIBLE;
	std::array<ocp_word_type, 0> encode(coordinate_type const& /*unique*/) const HALDLS_VISIBLE;
	void decode(std::array<ocp_word_type, 1> const& data) HALDLS_VISIBLE;

private:
	common::optional<ocp_word_type::value_type> m_value;
};

} // namespace detail

// TODO: Python wrapping of the following typedefs
typedef detail::FlyspiRWRegister<0x8002> FlyspiProgramAddress;
typedef detail::FlyspiRWRegister<0x8003> FlyspiProgramSize;
typedef detail::FlyspiRWRegister<0x8004> FlyspiResultAddress;
typedef detail::FlyspiRORegister<0x8005> FlyspiResultSize;
typedef detail::FlyspiRORegister<0x8006> FlyspiState;

class GENPYBIND(visible) FlyspiControl
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	FlyspiControl() HALDLS_VISIBLE;

	// accessors
	bool get_program_loopback() const HALDLS_VISIBLE;
	void set_program_loopback(bool const value) HALDLS_VISIBLE;
	bool get_memory_loopback() const HALDLS_VISIBLE;
	void set_memory_loopback(bool const value) HALDLS_VISIBLE;
	bool get_execute() const HALDLS_VISIBLE;
	void set_execute(bool const value) HALDLS_VISIBLE;

	// comparison
	bool operator==(FlyspiControl const& other) const HALDLS_VISIBLE;
	bool operator!=(FlyspiControl const& other) const HALDLS_VISIBLE;

	// encode/decode
	static size_t const constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<ocp_address_type, config_size_in_words> addresses(
		coordinate_type const& /*unique*/) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, config_size_in_words> encode(
		coordinate_type const& /*unique*/) const HALDLS_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, config_size_in_words> const& data) HALDLS_VISIBLE
		GENPYBIND(hidden);

private:
	bool m_program_loopback;
	bool m_memory_loopback;
	bool m_execute;
};

class GENPYBIND(visible) FlyspiConfig
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) TgControl
		: public halco::common::detail::RantWrapper<TgControl, uint_fast16_t, 63, 0>
	{
		constexpr explicit TgControl(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	FlyspiConfig() HALDLS_VISIBLE;

	// accessors
	bool get_dls_reset() const HALDLS_VISIBLE;
	void set_dls_reset(bool const value) HALDLS_VISIBLE;
	bool get_soft_reset() const HALDLS_VISIBLE;
	void set_soft_reset(bool const value) HALDLS_VISIBLE;
	// dstoe 2018-01-09 tg_control may be unused. Should be verified in the fpga code.
	TgControl get_tg_control() const HALDLS_VISIBLE;
	void set_tg_control(TgControl const value) HALDLS_VISIBLE;
	bool get_enable_spike_router() const HALDLS_VISIBLE;
	void set_enable_spike_router(bool const value) HALDLS_VISIBLE;
	bool get_i_phase_select() const HALDLS_VISIBLE;
	void set_i_phase_select(bool const value) HALDLS_VISIBLE;
	bool get_o_phase_select() const HALDLS_VISIBLE;
	void set_o_phase_select(bool const value) HALDLS_VISIBLE;
	bool get_enable_train() const HALDLS_VISIBLE;
	void set_enable_train(bool const value) HALDLS_VISIBLE;
	bool get_enable_transceiver() const HALDLS_VISIBLE;
	void set_enable_transceiver(bool const value) HALDLS_VISIBLE;
	bool get_enable_lvds() const HALDLS_VISIBLE;
	void set_enable_lvds(bool const value) HALDLS_VISIBLE;
	bool get_enable_analog_power() const HALDLS_VISIBLE;
	void set_enable_analog_power(bool const value) HALDLS_VISIBLE;
	bool get_enable_dls_loopback() const HALDLS_VISIBLE;
	void set_enable_dls_loopback(bool const value) HALDLS_VISIBLE;

	// comparison
	bool operator==(FlyspiConfig const& other) const HALDLS_VISIBLE;
	bool operator!=(FlyspiConfig const& other) const HALDLS_VISIBLE;

	// encode/decode
	static size_t const constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<ocp_address_type, config_size_in_words> addresses(
		coordinate_type const& /*unique*/) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, config_size_in_words> encode(coordinate_type const& /*unique*/) const
		HALDLS_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, config_size_in_words> const& data) HALDLS_VISIBLE
		GENPYBIND(hidden);

private:
	bool m_dls_reset;
	bool m_soft_reset;
	TgControl m_tg_control;
	bool m_spike_router;
	bool m_i_phase_select;
	bool m_o_phase_select;
	bool m_train;
	bool m_transceiver;
	bool m_lvds;
	bool m_analog_power;
	bool m_dls_loopback;
};

class GENPYBIND(visible) FlyspiException
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	FlyspiException() HALDLS_VISIBLE;

	// read only accessors
	common::optional<bool> get_result_read_error() const HALDLS_VISIBLE;
	common::optional<bool> get_result_read_overflow() const HALDLS_VISIBLE;
	common::optional<bool> get_result_write_error() const HALDLS_VISIBLE;
	common::optional<bool> get_result_write_underrun() const HALDLS_VISIBLE;
	common::optional<bool> get_playback_read_error() const HALDLS_VISIBLE;
	common::optional<bool> get_playback_read_overflow() const HALDLS_VISIBLE;
	common::optional<bool> get_playback_write_error() const HALDLS_VISIBLE;
	common::optional<bool> get_playback_write_underrun() const HALDLS_VISIBLE;
	common::optional<bool> get_program_exception() const HALDLS_VISIBLE;
	common::optional<bool> get_serdes_overflow() const HALDLS_VISIBLE;
	common::optional<bool> get_serdes_pll_unlocked() const HALDLS_VISIBLE;
	common::optional<bool> get_serdes_race() const HALDLS_VISIBLE;
	common::optional<bool> get_encode_overflow() const HALDLS_VISIBLE;

	// comparison
	bool operator==(FlyspiException const& other) const HALDLS_VISIBLE;
	bool operator!=(FlyspiException const& other) const HALDLS_VISIBLE;

	// encode/decode
	static size_t const constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t const constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	std::array<ocp_address_type, write_config_size_in_words> write_addresses(
		coordinate_type const& /*unique*/) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<ocp_address_type, read_config_size_in_words> read_addresses(
		coordinate_type const& /*unique*/) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, write_config_size_in_words> encode(
		coordinate_type const& /*unique*/) const HALDLS_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, read_config_size_in_words> const& data) HALDLS_VISIBLE
		GENPYBIND(hidden);

private:
	common::optional<bool> m_result_read_error;
	common::optional<bool> m_result_read_overflow;
	common::optional<bool> m_result_write_error;
	common::optional<bool> m_result_write_underrun;
	common::optional<bool> m_playback_read_error;
	common::optional<bool> m_playback_read_overflow;
	common::optional<bool> m_playback_write_error;
	common::optional<bool> m_playback_write_underrun;
	common::optional<bool> m_program_exception;
	common::optional<bool> m_serdes_overflow;
	common::optional<bool> m_serdes_pll_unlocked;
	common::optional<bool> m_serdes_race;
	common::optional<bool> m_encode_overflow;
};

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::FlyspiConfig::TgControl)

} // namespace std
