#include "haldls/vx/spi.h"

#include "fisch/vx/word_access/type/spi.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/spi.h"
#include "haldls/vx/container.tcc"
#include "hate/join.h"

namespace haldls {
namespace vx {

ShiftRegister::ShiftRegister() :
    m_mux_1(AnalogReadoutMux1Input::off),
    m_mux_2(AnalogReadoutMux2Input::off),
    m_mux_3(AnalogReadoutMux3Input::off),
    m_enable_i_ref_board(true),
    m_enable_measure_i_ref(false),
    m_enable_dac_to_readout_0(false),
    m_enable_dac_to_readout_1(false),
    m_enable_led(),
    m_enable_adc_power_down(false),
    m_enable_adc_reset(false),
    m_enable_vdd()
{
	m_enable_led.fill(false);
	m_enable_vdd.fill(true);
}

bool ShiftRegister::get_enable_i_ref_board() const
{
	return m_enable_i_ref_board;
}

void ShiftRegister::set_enable_i_ref_board(bool const value)
{
	m_enable_i_ref_board = value;
}

bool ShiftRegister::get_enable_measure_i_ref() const
{
	return m_enable_measure_i_ref;
}

void ShiftRegister::set_enable_measure_i_ref(bool const value)
{
	m_enable_measure_i_ref = value;
}

ShiftRegister::AnalogReadoutMux1Input ShiftRegister::get_select_analog_readout_mux_1_input() const
{
	return m_mux_1;
}

void ShiftRegister::set_select_analog_readout_mux_1_input(AnalogReadoutMux1Input const value)
{
	m_mux_1 = value;
}

ShiftRegister::AnalogReadoutMux2Input ShiftRegister::get_select_analog_readout_mux_2_input() const
{
	return m_mux_2;
}

void ShiftRegister::set_select_analog_readout_mux_2_input(AnalogReadoutMux2Input const value)
{
	m_mux_2 = value;
}

ShiftRegister::AnalogReadoutMux3Input ShiftRegister::get_select_analog_readout_mux_3_input() const
{
	return m_mux_3;
}

void ShiftRegister::set_select_analog_readout_mux_3_input(AnalogReadoutMux3Input const value)
{
	m_mux_3 = value;
}

bool ShiftRegister::get_enable_dac_to_readout_0() const
{
	return m_enable_dac_to_readout_0;
}

void ShiftRegister::set_enable_dac_to_readout_0(bool const value)
{
	m_enable_dac_to_readout_0 = value;
}

bool ShiftRegister::get_enable_dac_to_readout_1() const
{
	return m_enable_dac_to_readout_1;
}

void ShiftRegister::set_enable_dac_to_readout_1(bool const value)
{
	m_enable_dac_to_readout_1 = value;
}

bool ShiftRegister::get_enable_led(halco::hicann_dls::vx::LEDOnBoard const& coord) const
{
	return m_enable_led[coord];
}

void ShiftRegister::set_enable_led(halco::hicann_dls::vx::LEDOnBoard const& coord, bool const value)
{
	m_enable_led[coord] = value;
}

bool ShiftRegister::get_enable_adc_power_down() const
{
	return m_enable_adc_power_down;
}

void ShiftRegister::set_enable_adc_power_down(bool const value)
{
	m_enable_adc_power_down = value;
}

bool ShiftRegister::get_enable_adc_reset() const
{
	return m_enable_adc_reset;
}

void ShiftRegister::set_enable_adc_reset(bool const value)
{
	m_enable_adc_reset = value;
}

bool ShiftRegister::get_enable_vdd(halco::hicann_dls::vx::VDDOnBoard const& coord) const
{
	return m_enable_vdd[coord];
}

void ShiftRegister::set_enable_vdd(halco::hicann_dls::vx::VDDOnBoard const& coord, bool const value)
{
	m_enable_vdd[coord] = value;
}

bool ShiftRegister::operator==(ShiftRegister const& other) const
{
	return m_mux_1 == other.m_mux_1 && m_mux_2 == other.m_mux_2 && m_mux_3 == other.m_mux_3 &&
	       m_enable_i_ref_board == other.m_enable_i_ref_board &&
	       m_enable_measure_i_ref == other.m_enable_measure_i_ref &&
	       m_enable_dac_to_readout_0 == other.m_enable_dac_to_readout_0 &&
	       m_enable_dac_to_readout_1 == other.m_enable_dac_to_readout_1 &&
	       m_enable_led == other.m_enable_led &&
	       m_enable_adc_power_down == other.m_enable_adc_power_down &&
	       m_enable_adc_reset == other.m_enable_adc_reset && m_enable_vdd == other.m_enable_vdd;
}

bool ShiftRegister::operator!=(ShiftRegister const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ShiftRegister::AnalogReadoutMux1Input const& config)
{
	switch (config) {
		case ShiftRegister::AnalogReadoutMux1Input::off: {
			os << "off";
			break;
		}
		case ShiftRegister::AnalogReadoutMux1Input::readout_chain_0: {
			os << "readout_chain_0";
			break;
		}
		case ShiftRegister::AnalogReadoutMux1Input::readout_chain_1: {
			os << "readout_chain_1";
			break;
		}
		case ShiftRegister::AnalogReadoutMux1Input::mux_rfu_0: {
			os << "mux_rfu_0";
			break;
		}
		default: {
			throw std::logic_error("Unknown AnalogReadoutMux1Input.");
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, ShiftRegister::AnalogReadoutMux2Input const& config)
{
	switch (config) {
		case ShiftRegister::AnalogReadoutMux2Input::off: {
			os << "off";
			break;
		}
		case ShiftRegister::AnalogReadoutMux2Input::v_reset: {
			os << "v_reset";
			break;
		}
		case ShiftRegister::AnalogReadoutMux2Input::vdd_res_meas: {
			os << "vdd_res_meas";
			break;
		}
		case ShiftRegister::AnalogReadoutMux2Input::mux_dac_25: {
			os << "mux_dac_25";
			break;
		}
		default: {
			throw std::logic_error("Unknown AnalogReadoutMux2Input.");
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, ShiftRegister::AnalogReadoutMux3Input const& config)
{
	switch (config) {
		case ShiftRegister::AnalogReadoutMux3Input::off: {
			os << "off";
			break;
		}
		case ShiftRegister::AnalogReadoutMux3Input::i_ref: {
			os << "i_ref";
			break;
		}
		case ShiftRegister::AnalogReadoutMux3Input::mux_rfu_1: {
			os << "mux_rfu_1";
			break;
		}
		case ShiftRegister::AnalogReadoutMux3Input::mux_rfu_2: {
			os << "mux_rfu_2";
			break;
		}
		default: {
			throw std::logic_error("Unknown AnalogReadoutMux3Input.");
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, ShiftRegister const& config)
{
	std::stringstream ss;
	ss << "ShiftRegister(\n";
	ss << "\tmux_1:                   \t" << config.m_mux_1 << "\n";
	ss << "\tmux_2:                   \t" << config.m_mux_2 << "\n";
	ss << "\tmux_3:                   \t" << config.m_mux_3 << "\n";
	ss << "\tenable_i_ref_board:      \t" << std::boolalpha << config.m_enable_i_ref_board << "\n";
	ss << "\tenable_measure_i_ref:    \t" << config.m_enable_measure_i_ref << "\n";
	ss << "\tenable_dac_to_readout_0: \t" << config.m_enable_dac_to_readout_0 << "\n";
	ss << "\tenable_dac_to_readout_1: \t" << config.m_enable_dac_to_readout_1 << "\n";
	ss << "\tenable_led:              \t[";
	ss << hate::join(config.m_enable_led.begin(), config.m_enable_led.end(), ", ");
	ss << "]\n";
	ss << "\tenable_adc_power_down:   \t" << config.m_enable_adc_power_down << "\n";
	ss << "\tenable_adc_reset:        \t" << config.m_enable_adc_reset << "\n";
	ss << "\tenable_vdd:              \t[";
	ss << hate::join(config.m_enable_vdd.begin(), config.m_enable_vdd.end(), ", ");
	ss << "]\n)";
	return (os << ss.str());
}

std::
    array<halco::hicann_dls::vx::SPIShiftRegisterOnBoard, ShiftRegister::write_config_size_in_words>
    ShiftRegister::write_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::SPIShiftRegisterOnBoard()};
}

std::array<halco::hicann_dls::vx::SPIShiftRegisterOnBoard, ShiftRegister::read_config_size_in_words>
ShiftRegister::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

namespace {

struct ShiftRegisterBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t mux_1            :  2;
			uint32_t mux_2            :  2;
			uint32_t mux_3            :  2;
			uint32_t enable_i_ref_board   :  1;
			uint32_t enable_measure_i_ref :  1;
			uint32_t enable_dac_to_readout_0    :  1;
			uint32_t enable_dac_to_readout_1    :  1;
			uint32_t disable_led_1              :  1;
			uint32_t disable_led_2              :  1;
			uint32_t disable_led_3              :  1;
			uint32_t enable_adc_power_down :  1;
			uint32_t enable_adc_reset      :  1;
			uint32_t disable_led_4              :  1;
			uint32_t enable_vdd25digital : 1;
			uint32_t enable_vdd12digital : 1;
			uint32_t enable_vdd25analog : 1;
			uint32_t enable_vdd12analog : 1;
			uint32_t enable_vdd12madc : 1;
			uint32_t enable_vdd12pll : 1;
			uint32_t disable_led_7 : 1;
			uint32_t disable_led_8 : 1;
			uint32_t /* unused */        : 8;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ShiftRegisterBitfield() { u.raw = 0u; }

	ShiftRegisterBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

std::array<fisch::vx::word_access_type::SPIShiftRegister, ShiftRegister::write_config_size_in_words>
ShiftRegister::encode() const
{
	ShiftRegisterBitfield bitfield;
	bitfield.u.m.mux_1 = static_cast<uint32_t>(m_mux_1);
	bitfield.u.m.mux_2 = static_cast<uint32_t>(m_mux_2);
	bitfield.u.m.mux_3 = static_cast<uint32_t>(m_mux_3);
	bitfield.u.m.enable_i_ref_board = m_enable_i_ref_board;
	bitfield.u.m.enable_measure_i_ref = m_enable_measure_i_ref;
	bitfield.u.m.enable_dac_to_readout_0 = m_enable_dac_to_readout_0;
	bitfield.u.m.enable_dac_to_readout_1 = m_enable_dac_to_readout_1;
	bitfield.u.m.disable_led_1 = !m_enable_led[halco::hicann_dls::vx::LEDOnBoard::led_1];
	bitfield.u.m.disable_led_2 = !m_enable_led[halco::hicann_dls::vx::LEDOnBoard::led_2];
	bitfield.u.m.disable_led_3 = !m_enable_led[halco::hicann_dls::vx::LEDOnBoard::led_3];
	bitfield.u.m.enable_adc_power_down = m_enable_adc_power_down;
	bitfield.u.m.enable_adc_reset = m_enable_adc_reset;
	bitfield.u.m.disable_led_4 = !m_enable_led[halco::hicann_dls::vx::LEDOnBoard::led_4];
	bitfield.u.m.enable_vdd25digital =
	    m_enable_vdd[halco::hicann_dls::vx::VDDOnBoard::vdd25_digital];
	bitfield.u.m.enable_vdd12digital =
	    m_enable_vdd[halco::hicann_dls::vx::VDDOnBoard::vdd12_digital];
	bitfield.u.m.enable_vdd25analog = m_enable_vdd[halco::hicann_dls::vx::VDDOnBoard::vdd25_analog];
	bitfield.u.m.enable_vdd12analog = m_enable_vdd[halco::hicann_dls::vx::VDDOnBoard::vdd12_analog];
	bitfield.u.m.enable_vdd12madc = m_enable_vdd[halco::hicann_dls::vx::VDDOnBoard::vdd12_madc];
	bitfield.u.m.enable_vdd12pll = m_enable_vdd[halco::hicann_dls::vx::VDDOnBoard::vdd12_pll];
	bitfield.u.m.disable_led_7 = !m_enable_led[halco::hicann_dls::vx::LEDOnBoard::led_7];
	bitfield.u.m.disable_led_8 = !m_enable_led[halco::hicann_dls::vx::LEDOnBoard::led_8];

	return {fisch::vx::word_access_type::SPIShiftRegister(bitfield.u.raw)};
}

void ShiftRegister::decode(std::array<
                           fisch::vx::word_access_type::SPIShiftRegister,
                           ShiftRegister::read_config_size_in_words> const& /*data*/)
{}


DACChannel::Value DACChannel::get_value() const
{
	return m_value;
}

void DACChannel::set_value(Value const value)
{
	m_value = value;
}

bool DACChannel::operator==(DACChannel const& other) const
{
	return m_value == other.m_value;
}

bool DACChannel::operator!=(DACChannel const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, DACChannel const& config)
{
	os << "DACChannel(" << config.m_value << ")";
	return os;
}

std::array<halco::hicann_dls::vx::SPIDACDataRegisterOnBoard, DACChannel::write_config_size_in_words>
DACChannel::write_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::SPIDACDataRegisterOnBoard(coord.toEnum())};
}

std::array<halco::hicann_dls::vx::SPIDACDataRegisterOnBoard, DACChannel::read_config_size_in_words>
DACChannel::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::word_access_type::SPIDACDataRegister, DACChannel::write_config_size_in_words>
DACChannel::encode() const
{
	return {fisch::vx::word_access_type::SPIDACDataRegister(m_value)};
}

void DACChannel::decode(std::array<
                        fisch::vx::word_access_type::SPIDACDataRegister,
                        DACChannel::read_config_size_in_words> const& /*data*/)
{}


DACControl::DACControl() : m_enable_channel()
{
	m_enable_channel.fill(true);
}

bool DACControl::get_enable_channel(halco::hicann_dls::vx::DACChannelOnDAC const& channel) const
{
	return m_enable_channel[channel];
}

void DACControl::set_enable_channel(
    halco::hicann_dls::vx::DACChannelOnDAC const& channel, bool const value)
{
	m_enable_channel[channel] = value;
}

bool DACControl::operator==(DACControl const& other) const
{
	return m_enable_channel == other.m_enable_channel;
}

bool DACControl::operator!=(DACControl const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, DACControl const& config)
{
	std::stringstream ss;
	ss << "DACControl(enable_channel: [";
	ss << std::boolalpha;
	ss << hate::join(config.m_enable_channel.begin(), config.m_enable_channel.end(), ", ");
	ss << "])";
	return (os << ss.str());
}

std::array<
    halco::hicann_dls::vx::SPIDACControlRegisterOnBoard,
    DACControl::write_config_size_in_words>
DACControl::write_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::SPIDACControlRegisterOnBoard(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::gain_reference, coord),
	        halco::hicann_dls::vx::SPIDACControlRegisterOnBoard(
	            halco::hicann_dls::vx::SPIDACControlRegisterOnDAC::power_down, coord)};
}

std::array<
    halco::hicann_dls::vx::SPIDACControlRegisterOnBoard,
    DACControl::read_config_size_in_words>
DACControl::read_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<
    fisch::vx::word_access_type::SPIDACControlRegister,
    DACControl::write_config_size_in_words>
DACControl::encode() const
{
	auto gain_reference = fisch::vx::word_access_type::SPIDACControlRegister(0b001100);

	uint32_t power_down_bits = 0;
	for (auto channel : halco::common::iter_all<halco::hicann_dls::vx::DACChannelOnDAC>()) {
		power_down_bits |= (static_cast<uint32_t>(!m_enable_channel[channel]) << channel.toEnum());
	}
	auto power_down = fisch::vx::word_access_type::SPIDACControlRegister(power_down_bits);

	return {
	    fisch::vx::word_access_type::SPIDACControlRegister(gain_reference),
	    fisch::vx::word_access_type::SPIDACControlRegister(power_down)};
}

void DACControl::decode(std::array<
                        fisch::vx::word_access_type::SPIDACControlRegister,
                        DACControl::read_config_size_in_words> const& /*data*/)
{}

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ShiftRegister)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::DACChannel)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::DACControl)
