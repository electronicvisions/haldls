#pragma once


#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/optional.h"
#include "hate/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/genpybind.h"
#include "haldls/v2/synapse.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {
namespace detail {

template <ocp_address_type::value_type Address>
class FlyspiRWRegister
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	constexpr explicit FlyspiRWRegister(ocp_word_type::value_type const value = 0) SYMBOL_VISIBLE
		: m_value(value){};

	// accessor
	ocp_word_type::value_type get_value() const SYMBOL_VISIBLE;
	void set_value(ocp_word_type::value_type const value) SYMBOL_VISIBLE;

	// comparison
	bool operator==(FlyspiRWRegister<Address> const& other) const SYMBOL_VISIBLE;
	bool operator!=(FlyspiRWRegister<Address> const& other) const SYMBOL_VISIBLE;

	// encode/decode
	std::array<ocp_address_type, 1> write_addresses(coordinate_type const& /*unique*/) const
		SYMBOL_VISIBLE;
	std::array<ocp_address_type, 1> read_addresses(coordinate_type const& /*unique*/) const
		SYMBOL_VISIBLE;
	std::array<ocp_word_type, 1> encode(coordinate_type const& /*unique*/) const SYMBOL_VISIBLE;
	void decode(std::array<ocp_word_type, 1> const& data) SYMBOL_VISIBLE;

private:
	ocp_word_type::value_type m_value;
};

template <ocp_address_type::value_type Address>
class FlyspiRORegister
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	constexpr explicit FlyspiRORegister() SYMBOL_VISIBLE : m_value(){};

	// accessor
	hate::optional<ocp_word_type::value_type> get_value() const SYMBOL_VISIBLE;

	// comparison
	bool operator==(FlyspiRORegister<Address> const& other) const SYMBOL_VISIBLE;
	bool operator!=(FlyspiRORegister<Address> const& other) const SYMBOL_VISIBLE;

	// encode/decode
	std::array<ocp_address_type, 0> write_addresses(coordinate_type const& /*unique*/) const
		SYMBOL_VISIBLE;
	std::array<ocp_address_type, 1> read_addresses(coordinate_type const& /*unique*/) const
		SYMBOL_VISIBLE;
	std::array<ocp_word_type, 0> encode(coordinate_type const& /*unique*/) const SYMBOL_VISIBLE;
	void decode(std::array<ocp_word_type, 1> const& data) SYMBOL_VISIBLE;

private:
	hate::optional<ocp_word_type::value_type> m_value;
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

	FlyspiControl() SYMBOL_VISIBLE;

	// accessors
	bool get_program_loopback() const SYMBOL_VISIBLE;
	void set_program_loopback(bool const value) SYMBOL_VISIBLE;
	bool get_memory_loopback() const SYMBOL_VISIBLE;
	void set_memory_loopback(bool const value) SYMBOL_VISIBLE;
	bool get_execute() const SYMBOL_VISIBLE;
	void set_execute(bool const value) SYMBOL_VISIBLE;

	// comparison
	bool operator==(FlyspiControl const& other) const SYMBOL_VISIBLE;
	bool operator!=(FlyspiControl const& other) const SYMBOL_VISIBLE;

	// encode/decode
	static size_t const constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<ocp_address_type, config_size_in_words> addresses(
		coordinate_type const& /*unique*/) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, config_size_in_words> encode(
		coordinate_type const& /*unique*/) const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE
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
		constexpr explicit TgControl(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	FlyspiConfig() SYMBOL_VISIBLE;

	// accessors
	bool get_dls_reset() const SYMBOL_VISIBLE;
	void set_dls_reset(bool const value) SYMBOL_VISIBLE;
	bool get_soft_reset() const SYMBOL_VISIBLE;
	void set_soft_reset(bool const value) SYMBOL_VISIBLE;
	// dstoe 2018-01-09 tg_control may be unused. Should be verified in the fpga code.
	TgControl get_tg_control() const SYMBOL_VISIBLE;
	void set_tg_control(TgControl const value) SYMBOL_VISIBLE;
	bool get_enable_spike_router() const SYMBOL_VISIBLE;
	void set_enable_spike_router(bool const value) SYMBOL_VISIBLE;
	bool get_i_phase_select() const SYMBOL_VISIBLE;
	void set_i_phase_select(bool const value) SYMBOL_VISIBLE;
	bool get_o_phase_select() const SYMBOL_VISIBLE;
	void set_o_phase_select(bool const value) SYMBOL_VISIBLE;
	bool get_enable_train() const SYMBOL_VISIBLE;
	void set_enable_train(bool const value) SYMBOL_VISIBLE;
	bool get_enable_transceiver() const SYMBOL_VISIBLE;
	void set_enable_transceiver(bool const value) SYMBOL_VISIBLE;
	bool get_enable_lvds() const SYMBOL_VISIBLE;
	void set_enable_lvds(bool const value) SYMBOL_VISIBLE;
	bool get_enable_analog_power() const SYMBOL_VISIBLE;
	void set_enable_analog_power(bool const value) SYMBOL_VISIBLE;
	bool get_enable_dls_loopback() const SYMBOL_VISIBLE;
	void set_enable_dls_loopback(bool const value) SYMBOL_VISIBLE;

	// comparison
	bool operator==(FlyspiConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(FlyspiConfig const& other) const SYMBOL_VISIBLE;

	// encode/decode
	static size_t const constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<ocp_address_type, config_size_in_words> addresses(
		coordinate_type const& /*unique*/) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, config_size_in_words> encode(coordinate_type const& /*unique*/) const
		SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE
		GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

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

	FlyspiException() SYMBOL_VISIBLE;

	// read only accessors
	hate::optional<bool> get_result_read_error() const SYMBOL_VISIBLE;
	hate::optional<bool> get_result_read_overflow() const SYMBOL_VISIBLE;
	hate::optional<bool> get_result_write_error() const SYMBOL_VISIBLE;
	hate::optional<bool> get_result_write_underrun() const SYMBOL_VISIBLE;
	hate::optional<bool> get_playback_read_error() const SYMBOL_VISIBLE;
	hate::optional<bool> get_playback_read_overflow() const SYMBOL_VISIBLE;
	hate::optional<bool> get_playback_write_error() const SYMBOL_VISIBLE;
	hate::optional<bool> get_playback_write_underrun() const SYMBOL_VISIBLE;
	hate::optional<bool> get_program_exception() const SYMBOL_VISIBLE;
	hate::optional<bool> get_serdes_overflow() const SYMBOL_VISIBLE;
	hate::optional<bool> get_serdes_pll_unlocked() const SYMBOL_VISIBLE;
	hate::optional<bool> get_serdes_race() const SYMBOL_VISIBLE;
	hate::optional<bool> get_encode_overflow() const SYMBOL_VISIBLE;

	hate::optional<bool> check() const SYMBOL_VISIBLE;

	friend std::ostream& operator<<(std::ostream& os, FlyspiException const& a) SYMBOL_VISIBLE;

	// comparison
	bool operator==(FlyspiException const& other) const SYMBOL_VISIBLE;
	bool operator!=(FlyspiException const& other) const SYMBOL_VISIBLE;

	// encode/decode
	static size_t const constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t const constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	std::array<ocp_address_type, write_config_size_in_words> write_addresses(
		coordinate_type const& /*unique*/) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<ocp_address_type, read_config_size_in_words> read_addresses(
		coordinate_type const& /*unique*/) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, write_config_size_in_words> encode(
		coordinate_type const& /*unique*/) const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, read_config_size_in_words> const& data) SYMBOL_VISIBLE
		GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	hate::optional<bool> m_result_read_error;
	hate::optional<bool> m_result_read_overflow;
	hate::optional<bool> m_result_write_error;
	hate::optional<bool> m_result_write_underrun;
	hate::optional<bool> m_playback_read_error;
	hate::optional<bool> m_playback_read_overflow;
	hate::optional<bool> m_playback_write_error;
	hate::optional<bool> m_playback_write_underrun;
	hate::optional<bool> m_program_exception;
	hate::optional<bool> m_serdes_overflow;
	hate::optional<bool> m_serdes_pll_unlocked;
	hate::optional<bool> m_serdes_race;
	hate::optional<bool> m_encode_overflow;
};

class GENPYBIND(visible) SpikeRouter
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	SpikeRouter() SYMBOL_VISIBLE;

	struct GENPYBIND(inline_base("*")) Delay
		: public halco::common::detail::RantWrapper<Delay, uint_fast16_t, 0x4000 - 1, 0>
	{
		constexpr explicit Delay(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	/// \brief Disable routing of spikes altogether (default).
	void reset() SYMBOL_VISIBLE;

	/// \brief Enable "squeeze" mode, where every spike received in a specified interval
	///        is combined into a single packet with a fixed spike address.
	/// \note In the verilog implementation this mode is called "bypass" mode.  It is
	///       implemented by or-ing the spike vector (target synapse drivers) of all
	///       received spikes and assumes a fixed mapping from neurons to synapse drivers,
	///       e.g. spike events of neuron 0, 1, 2, … will be sent to synapse drivers 0,
	///       1, 2, ….
	/// After a specified delay, a single spike with a fixed address will be emitted (as a
	/// single packet) to all synapse drivers, whose corresponding neuron spiked at least
	/// once in that interval.
	void enable_squeeze_mode(
		SynapseBlock::Synapse::Address const& address, Delay const& delay) SYMBOL_VISIBLE;

	typedef std::bitset<halco::hicann_dls::v2::SynapseDriverOnDLS::size> target_rows_type;

	void set_neuron_route(
		halco::hicann_dls::v2::NeuronOnDLS const& neuron,
		SynapseBlock::Synapse::Address const& address,
		target_rows_type const& target_rows) SYMBOL_VISIBLE;

	bool operator==(SpikeRouter const& other) const SYMBOL_VISIBLE;
	bool operator!=(SpikeRouter const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) =
		2 * halco::hicann_dls::v2::NeuronOnDLS::size + 1;
	std::array<ocp_address_type, config_size_in_words> write_addresses(
		coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<ocp_address_type, 0> read_addresses(coordinate_type const& unique) const
		SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, 0> const& words) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	bool m_squeeze_mode_enabled;
	SynapseBlock::Synapse::Address m_squeeze_mode_address;
	Delay m_squeeze_mode_delay;

	halco::common::typed_array<SynapseBlock::Synapse::Address, halco::hicann_dls::v2::NeuronOnDLS>
		m_address_by_neuron;
	halco::common::typed_array<target_rows_type, halco::hicann_dls::v2::NeuronOnDLS>
		m_target_rows_by_neuron;
}; // SpikeRouter

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::FlyspiConfig::TgControl)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SpikeRouter::Delay)

} // namespace std
