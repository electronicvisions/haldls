#include "haldls/v2/register.h"

namespace haldls {
namespace v2 {

namespace detail {

template <ocp_address_type::value_type Address>
ocp_word_type::value_type FlyspiRWRegister<Address>::get_value() const
{
	return m_value;
}

template <ocp_address_type::value_type Address>
void FlyspiRWRegister<Address>::set_value(ocp_word_type::value_type const value)
{
	m_value = value;
}

// comparison
template <ocp_address_type::value_type Address>
bool FlyspiRWRegister<Address>::operator==(FlyspiRWRegister<Address> const& other) const
{
	return m_value == other.get_value();
}

template <ocp_address_type::value_type Address>
bool FlyspiRWRegister<Address>::operator!=(FlyspiRWRegister<Address> const& other) const
{
	return !(*this == other);
}

// encode/decode
template <ocp_address_type::value_type Address>
std::array<ocp_address_type, 1> FlyspiRWRegister<Address>::write_addresses(
	coordinate_type const& /*unique*/) const
{
	return {{Address}};
}

template <ocp_address_type::value_type Address>
std::array<ocp_address_type, 1> FlyspiRWRegister<Address>::read_addresses(
	coordinate_type const& /*unique*/) const
{
	return {{Address}};
}

template <ocp_address_type::value_type Address>
std::array<ocp_word_type, 1> FlyspiRWRegister<Address>::encode(
	coordinate_type const& /*unique*/) const
{
	return {{m_value}};
}

template <ocp_address_type::value_type Address>
void FlyspiRWRegister<Address>::decode(std::array<ocp_word_type, 1> const& data)
{
	m_value = data[0].value;
}

template <ocp_address_type::value_type Address>
hate::optional<ocp_word_type::value_type> FlyspiRORegister<Address>::get_value() const
{
	return m_value;
}

template <ocp_address_type::value_type Address>
bool FlyspiRORegister<Address>::operator==(FlyspiRORegister<Address> const& other) const
{
	return m_value == other.get_value();
}

template <ocp_address_type::value_type Address>
bool FlyspiRORegister<Address>::operator!=(FlyspiRORegister<Address> const& other) const
{
	return !(*this == other);
}

template <ocp_address_type::value_type Address>
std::array<ocp_address_type, 0> FlyspiRORegister<Address>::write_addresses(
	coordinate_type const& /*unique*/) const
{
	return {{}};
}

template <ocp_address_type::value_type Address>
std::array<ocp_address_type, 1> FlyspiRORegister<Address>::read_addresses(
	coordinate_type const& /*unique*/) const
{
	return {{Address}};
}

template <ocp_address_type::value_type Address>
std::array<ocp_word_type, 0> FlyspiRORegister<Address>::encode(
	coordinate_type const& /*unique*/) const
{
	return {{}};
}

template <ocp_address_type::value_type Address>
void FlyspiRORegister<Address>::decode(std::array<ocp_word_type, 1> const& data)
{
	m_value = data[0].value;
}

} // namespace detail

template class detail::FlyspiRWRegister<0x8002>;
template class detail::FlyspiRWRegister<0x8003>;
template class detail::FlyspiRWRegister<0x8004>;
template class detail::FlyspiRORegister<0x8005>;
template class detail::FlyspiRORegister<0x8006>;

FlyspiControl::FlyspiControl()
	: m_program_loopback(false), m_memory_loopback(false), m_execute(false)
{}

bool FlyspiControl::get_program_loopback() const
{
	return m_program_loopback;
}

void FlyspiControl::set_program_loopback(bool const value)
{
	m_program_loopback = value;
}

bool FlyspiControl::get_memory_loopback() const
{
	return m_memory_loopback;
}

void FlyspiControl::set_memory_loopback(bool const value)
{
	m_memory_loopback = value;
}
bool FlyspiControl::get_execute() const
{
	return m_execute;
}

void FlyspiControl::set_execute(bool const value)
{
	m_execute = value;
}

bool FlyspiControl::operator==(FlyspiControl const& other) const
{
	// clang-format off
	return (
		m_program_loopback == other.get_program_loopback() &&
		m_memory_loopback == other.get_memory_loopback() &&
		m_execute == other.get_execute());
	// clang-format on
}

bool FlyspiControl::operator!=(FlyspiControl const& other) const
{
	return !(*this == other);
}

namespace {

struct FlyspiControlBitfield
{
	union
	{
		ocp_word_type::value_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			ocp_word_type::value_type program_loopback :  1; // 0
			ocp_word_type::value_type execute          :  1; // 1
			ocp_word_type::value_type memory_loopback  :  1; // 2
			ocp_word_type::value_type                  : 29; // 3-31
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FlyspiControlBitfield() { u.raw = 0u; }

	FlyspiControlBitfield(ocp_word_type data) { u.raw = data.value; }
};

} // namespace

std::array<ocp_address_type, FlyspiControl::config_size_in_words> FlyspiControl::addresses(
	FlyspiControl::coordinate_type const& /*unique*/) const
{
	return {{0x8000}};
}

std::array<ocp_word_type, FlyspiControl::config_size_in_words> FlyspiControl::encode(
	FlyspiControl::coordinate_type const& /*unique*/) const
{
	FlyspiControlBitfield bitfield;
	bitfield.u.m.memory_loopback = m_memory_loopback;
	bitfield.u.m.program_loopback = m_program_loopback;
	bitfield.u.m.execute = m_execute;
	return {{bitfield.u.raw}};
}

void FlyspiControl::decode(
	std::array<ocp_word_type, FlyspiControl::config_size_in_words> const& data)
{
	FlyspiControlBitfield bitfield(data[0]);
	m_memory_loopback = bitfield.u.m.memory_loopback;
	m_program_loopback = bitfield.u.m.program_loopback;
	m_execute = bitfield.u.m.execute;
}

FlyspiConfig::FlyspiConfig()
	: m_dls_reset(false),
	  m_soft_reset(false),
	  m_tg_control(TgControl(0)),
	  m_spike_router(false),
	  m_i_phase_select(false),
	  m_o_phase_select(false),
	  m_train(false),
	  m_transceiver(true),
	  m_lvds(true),
	  m_analog_power(true),
	  m_dls_loopback(false)
{}

bool FlyspiConfig::get_dls_reset() const
{
	return m_dls_reset;
}

void FlyspiConfig::set_dls_reset(bool const value)
{
	m_dls_reset = value;
}

bool FlyspiConfig::get_soft_reset() const
{
	return m_soft_reset;
}

void FlyspiConfig::set_soft_reset(bool const value)
{
	m_soft_reset = value;
}

FlyspiConfig::TgControl FlyspiConfig::get_tg_control() const
{
	return m_tg_control;
}

void FlyspiConfig::set_tg_control(FlyspiConfig::TgControl const value)
{
	m_tg_control = value;
}

bool FlyspiConfig::get_enable_spike_router() const
{
	return m_spike_router;
}

void FlyspiConfig::set_enable_spike_router(bool const value)
{
	m_spike_router = value;
}

bool FlyspiConfig::get_i_phase_select() const
{
	return m_i_phase_select;
}

void FlyspiConfig::set_i_phase_select(bool const value)
{
	m_i_phase_select = value;
}

bool FlyspiConfig::get_o_phase_select() const
{
	return m_o_phase_select;
}

void FlyspiConfig::set_o_phase_select(bool const value)
{
	m_o_phase_select = value;
}

bool FlyspiConfig::get_enable_train() const
{
	return m_train;
}

void FlyspiConfig::set_enable_train(bool const value)
{
	m_train = value;
}

bool FlyspiConfig::get_enable_transceiver() const
{
	return m_transceiver;
}

void FlyspiConfig::set_enable_transceiver(bool const value)
{
	m_transceiver = value;
}

bool FlyspiConfig::get_enable_lvds() const
{
	return m_lvds;
}

void FlyspiConfig::set_enable_lvds(bool const value)
{
	m_lvds = value;
}

bool FlyspiConfig::get_enable_analog_power() const
{
	return m_analog_power;
}

void FlyspiConfig::set_enable_analog_power(bool const value)
{
	m_analog_power = value;
}

bool FlyspiConfig::get_enable_dls_loopback() const
{
	return m_dls_loopback;
}

void FlyspiConfig::set_enable_dls_loopback(bool const value)
{
	m_dls_loopback = value;
}

bool FlyspiConfig::operator==(FlyspiConfig const& other) const
{
	// clang-format off
	return (
		m_dls_reset == other.get_dls_reset() &&
		m_soft_reset == other.get_soft_reset() &&
		m_tg_control == other.get_tg_control() &&
		m_spike_router == other.get_enable_spike_router() &&
		m_i_phase_select == other.get_i_phase_select() &&
		m_o_phase_select == other.get_o_phase_select() &&
		m_train == other.get_enable_train() &&
		m_transceiver == other.get_enable_transceiver() &&
		m_lvds == other.get_enable_lvds() &&
		m_analog_power == other.get_enable_analog_power() &&
		m_dls_loopback == other.get_enable_dls_loopback());
	// clang-format on
}

bool FlyspiConfig::operator!=(FlyspiConfig const& other) const
{
	return !(*this == other);
}

namespace {

struct FlyspiConfigBitfield
{
	union
	{
		ocp_word_type::value_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			ocp_word_type::value_type dls_loopback   :  1; // 0
			ocp_word_type::value_type analog_power   :  1; // 1
			ocp_word_type::value_type lvds           :  1; // 2
			ocp_word_type::value_type transceiver    :  1; // 3
			ocp_word_type::value_type train          :  1; // 4
			ocp_word_type::value_type o_phase_select :  1; // 5
			ocp_word_type::value_type i_phase_select :  1; // 6
			ocp_word_type::value_type spike_router   :  1; // 7
			ocp_word_type::value_type tg_control     :  6; // 8-13
			ocp_word_type::value_type                : 16; // 14-29
			ocp_word_type::value_type soft_reset     :  1; // 30
			ocp_word_type::value_type dls_reset      :  1; // 31
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FlyspiConfigBitfield() { u.raw = 0u; }

	FlyspiConfigBitfield(ocp_word_type data) { u.raw = data.value; }
};

} // namespace

std::array<ocp_address_type, FlyspiConfig::config_size_in_words> FlyspiConfig::addresses(
	FlyspiConfig::coordinate_type const& /*unique*/) const
{
	return {{0x8020}};
}

std::array<ocp_word_type, FlyspiConfig::config_size_in_words> FlyspiConfig::encode(
	FlyspiConfig::coordinate_type const& /*unique*/) const
{
	FlyspiConfigBitfield bitfield;
	bitfield.u.m.dls_reset = m_dls_reset;
	bitfield.u.m.soft_reset = m_soft_reset;
	bitfield.u.m.tg_control = m_tg_control.value();
	bitfield.u.m.spike_router = m_spike_router;
	bitfield.u.m.i_phase_select = m_i_phase_select;
	bitfield.u.m.o_phase_select = m_o_phase_select;
	bitfield.u.m.train = m_train;
	bitfield.u.m.transceiver = m_transceiver;
	bitfield.u.m.lvds = m_lvds;
	bitfield.u.m.analog_power = m_analog_power;
	bitfield.u.m.dls_loopback = m_dls_loopback;

	return {{bitfield.u.raw}};
}

void FlyspiConfig::decode(std::array<ocp_word_type, FlyspiConfig::config_size_in_words> const& data)
{
	FlyspiConfigBitfield bitfield(data[0]);

	m_dls_reset = bitfield.u.m.dls_reset;
	m_soft_reset = bitfield.u.m.soft_reset;
	m_tg_control = TgControl(bitfield.u.m.tg_control);
	m_spike_router = bitfield.u.m.spike_router;
	m_i_phase_select = bitfield.u.m.i_phase_select;
	m_o_phase_select = bitfield.u.m.o_phase_select;
	m_train = bitfield.u.m.train;
	m_transceiver = bitfield.u.m.transceiver;
	m_lvds = bitfield.u.m.lvds;
	m_analog_power = bitfield.u.m.analog_power;
	m_dls_loopback = bitfield.u.m.dls_loopback;
}

FlyspiException::FlyspiException()
	: m_result_read_error(),
	  m_result_read_overflow(),
	  m_result_write_error(),
	  m_result_write_underrun(),
	  m_playback_read_error(),
	  m_playback_read_overflow(),
	  m_playback_write_error(),
	  m_playback_write_underrun(),
	  m_program_exception(),
	  m_serdes_overflow(),
	  m_serdes_pll_unlocked(),
	  m_serdes_race(),
	  m_encode_overflow()
{}

hate::optional<bool> FlyspiException::get_result_read_error() const
{
	return m_result_read_error;
}

hate::optional<bool> FlyspiException::get_result_read_overflow() const
{
	return m_result_read_overflow;
}

hate::optional<bool> FlyspiException::get_result_write_error() const
{
	return m_result_write_error;
}

hate::optional<bool> FlyspiException::get_result_write_underrun() const
{
	return m_result_write_underrun;
}

hate::optional<bool> FlyspiException::get_playback_read_error() const
{
	return m_playback_read_error;
}

hate::optional<bool> FlyspiException::get_playback_read_overflow() const
{
	return m_playback_read_overflow;
}

hate::optional<bool> FlyspiException::get_playback_write_error() const
{
	return m_playback_write_error;
}

hate::optional<bool> FlyspiException::get_playback_write_underrun() const
{
	return m_playback_write_underrun;
}

hate::optional<bool> FlyspiException::get_program_exception() const
{
	return m_program_exception;
}

hate::optional<bool> FlyspiException::get_serdes_overflow() const
{
	return m_serdes_overflow;
}

hate::optional<bool> FlyspiException::get_serdes_pll_unlocked() const
{
	return m_serdes_pll_unlocked;
}

hate::optional<bool> FlyspiException::get_serdes_race() const
{
	return m_serdes_race;
}

hate::optional<bool> FlyspiException::get_encode_overflow() const
{
	return m_encode_overflow;
}

hate::optional<bool> FlyspiException::check() const
{
	hate::optional<bool> ret;
	// only set value if all registers have a value
	if (m_result_read_error && m_result_read_overflow && m_result_write_error &&
	    m_result_write_underrun && m_playback_read_error && m_playback_read_overflow &&
	    m_playback_write_error && m_playback_write_underrun && m_program_exception &&
	    m_serdes_overflow && m_serdes_pll_unlocked && m_serdes_race && m_encode_overflow) {
		ret =
		    !(m_result_read_error.value() || m_result_read_overflow.value() ||
		      m_result_write_error.value() || m_result_write_underrun.value() ||
		      m_playback_read_error.value() || m_playback_read_overflow.value() ||
		      m_playback_write_error.value() || m_playback_write_underrun.value() ||
		      m_program_exception.value() || m_serdes_overflow.value() ||
		      m_serdes_pll_unlocked.value() || m_serdes_race.value() || m_encode_overflow.value());
	}
	return ret;
}

#define PRINT_EXCEPTION(VALUE)                                                            \
	if (VALUE()) {                                                                       \
		os << #VALUE << ": " << *VALUE() << std::endl;                                   \
	}

std::ostream& operator<<(std::ostream& os, FlyspiException const& a)
{
	os << "FlyspiException: " << std::endl;
	PRINT_EXCEPTION(a.get_result_read_error)
	PRINT_EXCEPTION(a.get_result_read_overflow)
	PRINT_EXCEPTION(a.get_result_write_error)
	PRINT_EXCEPTION(a.get_result_write_underrun)
	PRINT_EXCEPTION(a.get_playback_read_error)
	PRINT_EXCEPTION(a.get_playback_read_overflow)
	PRINT_EXCEPTION(a.get_playback_write_error)
	PRINT_EXCEPTION(a.get_playback_write_underrun)
	PRINT_EXCEPTION(a.get_program_exception)
	PRINT_EXCEPTION(a.get_serdes_overflow)
	PRINT_EXCEPTION(a.get_serdes_pll_unlocked)
	PRINT_EXCEPTION(a.get_serdes_race)
	PRINT_EXCEPTION(a.get_encode_overflow)
	return os;
}
#undef PRINT_EXCEPTION

bool FlyspiException::operator==(FlyspiException const& other) const
{
	// clang-format off
	return (
		m_result_read_error == other.m_result_read_error &&
		m_result_read_overflow == other.m_result_read_overflow &&
		m_result_write_error == other.m_result_write_error &&
		m_result_write_underrun == other.m_result_write_underrun &&
		m_playback_read_error == other.m_playback_read_error &&
		m_playback_read_overflow == other.m_playback_read_overflow &&
		m_playback_write_error == other.m_playback_write_error &&
		m_playback_write_underrun == other.m_playback_write_underrun &&
		m_program_exception == other.m_program_exception &&
		m_serdes_overflow == other.m_serdes_overflow &&
		m_serdes_pll_unlocked == other.m_serdes_pll_unlocked &&
		m_serdes_race == other.m_serdes_race &&
		m_encode_overflow == other.m_encode_overflow);
	// clang-format on
}

bool FlyspiException::operator!=(FlyspiException const& other) const
{
	return !(*this == other);
}

namespace {

struct FlyspiExceptionBitfield
{
	union
	{
		ocp_word_type::value_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			ocp_word_type::value_type result_read_error       :  1; // 0
			ocp_word_type::value_type result_read_overflow    :  1; // 1
			ocp_word_type::value_type result_write_error      :  1; // 2
			ocp_word_type::value_type result_write_underrun   :  1; // 3
			ocp_word_type::value_type playback_read_error     :  1; // 4
			ocp_word_type::value_type playback_read_overflow  :  1; // 5
			ocp_word_type::value_type playback_write_error    :  1; // 6
			ocp_word_type::value_type playback_write_underrun :  1; // 7
			ocp_word_type::value_type program_exception       :  1; // 8
			ocp_word_type::value_type                         :  7; // 9-15
			ocp_word_type::value_type serdes_overflow         :  1; // 16
			ocp_word_type::value_type serdes_pll_unlocked     :  1; // 17
			ocp_word_type::value_type serdes_race             :  1; // 18
			ocp_word_type::value_type                         :  1; // 19
			ocp_word_type::value_type encode_overflow         :  1; // 20
			ocp_word_type::value_type                         : 11; // 21-31
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FlyspiExceptionBitfield() { u.raw = 0u; }

	FlyspiExceptionBitfield(ocp_word_type data) { u.raw = data.value; }
};

} // namespace

std::array<ocp_address_type, FlyspiException::write_config_size_in_words>
FlyspiException::write_addresses(FlyspiControl::coordinate_type const& /*unique*/) const
{
	return {{}}; // RO register
}

std::array<ocp_address_type, FlyspiException::read_config_size_in_words>
FlyspiException::read_addresses(FlyspiControl::coordinate_type const& /*unique*/) const
{
	return {{0x8001}};
}
std::array<ocp_word_type, FlyspiException::write_config_size_in_words> FlyspiException::encode(
	FlyspiControl::coordinate_type const& /*unique*/) const
{
	return {{}}; // RO register
}

void FlyspiException::decode(
	std::array<ocp_word_type, FlyspiException::read_config_size_in_words> const& data)
{
	FlyspiExceptionBitfield bitfield(data[0]);
	m_result_read_error = static_cast<bool>(bitfield.u.m.result_read_error);
	m_result_read_overflow = static_cast<bool>(bitfield.u.m.result_read_overflow);
	m_result_write_error = static_cast<bool>(bitfield.u.m.result_write_error);
	m_result_write_underrun = static_cast<bool>(bitfield.u.m.result_write_underrun);
	m_playback_read_error = static_cast<bool>(bitfield.u.m.playback_read_error);
	m_playback_read_overflow = static_cast<bool>(bitfield.u.m.playback_read_overflow);
	m_playback_write_error = static_cast<bool>(bitfield.u.m.playback_write_error);
	m_playback_write_underrun = static_cast<bool>(bitfield.u.m.playback_write_underrun);
	m_program_exception = static_cast<bool>(bitfield.u.m.program_exception);
	m_serdes_overflow = static_cast<bool>(bitfield.u.m.serdes_overflow);
	m_serdes_pll_unlocked = static_cast<bool>(bitfield.u.m.serdes_pll_unlocked);
	m_serdes_race = static_cast<bool>(bitfield.u.m.serdes_race);
	m_encode_overflow = static_cast<bool>(bitfield.u.m.encode_overflow);
}


} // namespace v2
} // namespace haldls
