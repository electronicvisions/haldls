#include "haldls/vx/i2c.h"

#include "fisch/vx/word_access/type/i2c.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/chip_carrier.h"
#include "halco/hicann-dls/vx/i2c.h"
#include "halco/hicann-dls/vx/jboa.h"
#include "halco/hicann-dls/vx/xboard.h"
#include "haldls/vx/container.tcc"
#include "hate/type_index.h"

namespace haldls::vx {

float TMP112Status::Temperature::toUncalibratedTemperature() const
{
	return static_cast<float>(value() / 16 /* 4bit right shift */) * 0.0625 /* LSB [K] */;
}

TMP112Status::TMP112Status() : m_temperature() {}

TMP112Status::Temperature TMP112Status::get_temperature() const
{
	return m_temperature;
}

void TMP112Status::set_temperature(Temperature const value)
{
	m_temperature = value;
}

bool TMP112Status::operator==(TMP112Status const& other) const
{
	return m_temperature == other.m_temperature;
}

bool TMP112Status::operator!=(TMP112Status const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, TMP112Status const& status)
{
	os << "TMP112Status(" << status.m_temperature << ")";
	return os;
}

std::array<halco::hicann_dls::vx::I2CTempRegisterOnBoard, TMP112Status::read_config_size_in_words>
TMP112Status::read_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::I2CTempRegisterOnBoard(coord.toEnum())};
}

std::array<halco::hicann_dls::vx::I2CTempRegisterOnBoard, TMP112Status::write_config_size_in_words>
TMP112Status::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::word_access_type::I2CTempRegister, TMP112Status::write_config_size_in_words>
TMP112Status::encode() const
{
	return {};
}

void TMP112Status::decode(std::array<
                          fisch::vx::word_access_type::I2CTempRegister,
                          TMP112Status::read_config_size_in_words> const& data)
{
	auto const temperature = data[0];

	bool const negative = temperature & 0x8000;
	auto const temperature_value = temperature & 0x7fff;
	if (!negative) {
		m_temperature = Temperature(temperature_value);
	} else {
		m_temperature = Temperature(-static_cast<uint16_t>((~(temperature_value - 1)) & 0x7fff));
	}
}

INA219Config::INA219Config() : m_bus_adc_mode(ADCMode::bits12), m_shunt_adc_mode(ADCMode::bits12) {}

INA219Config::ADCMode INA219Config::get_bus_adc_mode() const
{
	return m_bus_adc_mode;
}

void INA219Config::set_bus_adc_mode(ADCMode const value)
{
	m_bus_adc_mode = value;
}

INA219Config::ADCMode INA219Config::get_shunt_adc_mode() const
{
	return m_shunt_adc_mode;
}

void INA219Config::set_shunt_adc_mode(ADCMode const value)
{
	m_shunt_adc_mode = value;
}

bool INA219Config::operator==(INA219Config const& other) const
{
	return m_bus_adc_mode == other.m_bus_adc_mode && m_shunt_adc_mode == other.m_shunt_adc_mode;
}

bool INA219Config::operator!=(INA219Config const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, INA219Config::ADCMode const& mode)
{
	switch (mode) {
		case INA219Config::ADCMode::bits9: {
			os << "bits9";
			break;
		}
		case INA219Config::ADCMode::bits10: {
			os << "bits10";
			break;
		}
		case INA219Config::ADCMode::bits11: {
			os << "bits11";
			break;
		}
		case INA219Config::ADCMode::bits12: {
			os << "bits12";
			break;
		}
		case INA219Config::ADCMode::bits12_samples1: {
			os << "bits12_samples1";
			break;
		}
		case INA219Config::ADCMode::bits12_samples2: {
			os << "bits12_samples2";
			break;
		}
		case INA219Config::ADCMode::bits12_samples4: {
			os << "bits12_samples4";
			break;
		}
		case INA219Config::ADCMode::bits12_samples8: {
			os << "bits12_samples8";
			break;
		}
		case INA219Config::ADCMode::bits12_samples16: {
			os << "bits12_samples16";
			break;
		}
		case INA219Config::ADCMode::bits12_samples32: {
			os << "bits12_samples32";
			break;
		}
		case INA219Config::ADCMode::bits12_samples64: {
			os << "bits12_samples64";
			break;
		}
		case INA219Config::ADCMode::bits12_samples128: {
			os << "bits12_samples128";
			break;
		}
		default: {
			throw std::logic_error("Unknown ADCMode.");
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, INA219Config const& config)
{
	os << "INA219Config(" << config.m_bus_adc_mode << ", " << config.m_shunt_adc_mode << ")";
	return os;
}

std::array<halco::hicann_dls::vx::I2CINA219RwRegisterOnBoard, INA219Config::config_size_in_words>
INA219Config::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::I2CINA219RwRegisterOnBoard(
	    halco::hicann_dls::vx::I2CINA219RwRegisterOnINA219::config, coord.toINA219OnBoard())};
}

namespace {

struct INA219ConfigBitfield
{
	union
	{
		uint16_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint16_t mode : 3;
			uint16_t shunt_adc_mode : 4;
			uint16_t bus_adc_mode : 4;
			uint16_t shunt_voltage_range : 2;
			uint16_t bus_voltage_range : 1;
			uint16_t /* unused */ : 1;
			uint16_t reset        : 1;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	INA219ConfigBitfield()
	{
		u.raw = 0u;
	}

	INA219ConfigBitfield(uint16_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<fisch::vx::word_access_type::I2CINA219RwRegister, INA219Config::config_size_in_words>
INA219Config::encode() const
{
	INA219ConfigBitfield bitfield;
	bitfield.u.m.mode = 0b111 /* bus and shunt continuous */;
	bitfield.u.m.shunt_adc_mode = static_cast<uint16_t>(m_shunt_adc_mode);
	bitfield.u.m.bus_adc_mode = static_cast<uint16_t>(m_bus_adc_mode);
	bitfield.u.m.shunt_voltage_range = 0b00 /* +-40mV */;
	bitfield.u.m.bus_voltage_range = 0b0 /* 16V */;
	bitfield.u.m.reset = 0;

	return {fisch::vx::word_access_type::I2CINA219RwRegister(bitfield.u.raw)};
}

void INA219Config::decode(std::array<
                          fisch::vx::word_access_type::I2CINA219RwRegister,
                          INA219Config::config_size_in_words> const& data)
{
	INA219ConfigBitfield bitfield(data[0]);
	m_shunt_adc_mode = ADCMode(bitfield.u.m.shunt_adc_mode);
	m_bus_adc_mode = ADCMode(bitfield.u.m.bus_adc_mode);
}


float INA219Status::BusVoltage::toUncalibratedVoltage() const
{
	return static_cast<float>(value() * 4 /* 4mV/LSB */) / 1000. /* to V */;
}

float INA219Status::ShuntVoltage::toUncalibratedVoltage() const
{
	return static_cast<float>(value() * 10 /* 10uV/LSB */) / 1000000. /* to V */;
}

float INA219Status::UncalibratedPower::calculate() const
{
	return uncalibrated_bus_voltage * uncalibrated_shunt_voltage / shunt_resistance;
}

std::ostream& operator<<(std::ostream& os, INA219Status::UncalibratedPower const& data)
{
	os << "UncalibratedPower(" << data.calculate() << " W)";
	return os;
}

INA219Status::INA219Status() : m_bus_voltage(), m_bus_voltage_overflow(false), m_shunt_voltage() {}

INA219Status::BusVoltage INA219Status::get_bus_voltage() const
{
	return m_bus_voltage;
}

void INA219Status::set_bus_voltage(BusVoltage const value)
{
	m_bus_voltage = value;
}

INA219Status::ShuntVoltage INA219Status::get_shunt_voltage() const
{
	return m_shunt_voltage;
}

void INA219Status::set_shunt_voltage(ShuntVoltage const value)
{
	m_shunt_voltage = value;
}

bool INA219Status::get_bus_voltage_overflow() const
{
	return m_bus_voltage_overflow;
}

void INA219Status::set_bus_voltage_overflow(bool const value)
{
	m_bus_voltage_overflow = value;
}

INA219Status::UncalibratedPower INA219Status::toUncalibratedPower() const
{
	return UncalibratedPower{.uncalibrated_bus_voltage = m_bus_voltage.toUncalibratedVoltage(),
	                         .uncalibrated_shunt_voltage = m_shunt_voltage.toUncalibratedVoltage()};
}

bool INA219Status::operator==(INA219Status const& other) const
{
	return m_bus_voltage == other.m_bus_voltage && m_shunt_voltage == other.m_shunt_voltage &&
	       m_bus_voltage_overflow == other.m_bus_voltage_overflow;
}

bool INA219Status::operator!=(INA219Status const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, INA219Status const& status)
{
	std::stringstream ss;
	ss << "bus_voltage_overflow: " << std::boolalpha << status.m_bus_voltage_overflow;
	os << "INA219Status(" << status.m_bus_voltage << ", " << ss.str() << ", "
	   << status.m_shunt_voltage << ")";
	return os;
}

std::array<
    halco::hicann_dls::vx::I2CINA219RoRegisterOnBoard,
    INA219Status::read_config_size_in_words>
INA219Status::read_addresses(coordinate_type const& coord)
{
	return {
	    halco::hicann_dls::vx::I2CINA219RoRegisterOnBoard(
	        halco::hicann_dls::vx::I2CINA219RoRegisterOnINA219::bus, coord.toINA219OnBoard()),
	    halco::hicann_dls::vx::I2CINA219RoRegisterOnBoard(
	        halco::hicann_dls::vx::I2CINA219RoRegisterOnINA219::shunt, coord.toINA219OnBoard())};
}

std::array<
    halco::hicann_dls::vx::I2CINA219RoRegisterOnBoard,
    INA219Status::write_config_size_in_words>
INA219Status::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<
    fisch::vx::word_access_type::I2CINA219RoRegister,
    INA219Status::write_config_size_in_words>
INA219Status::encode() const
{
	return {};
}

void INA219Status::decode(std::array<
                          fisch::vx::word_access_type::I2CINA219RoRegister,
                          INA219Status::read_config_size_in_words> const& data)
{
	auto const bus = data[0];
	auto const shunt = data[1];

	m_bus_voltage_overflow = bus & 0b1;
	m_bus_voltage = BusVoltage(bus >> 3);

	bool const shunt_sign = shunt & 0x8000;
	auto const shunt_value = shunt & 0xfff;
	if (!shunt_sign) {
		m_shunt_voltage = ShuntVoltage(shunt_value);
	} else {
		m_shunt_voltage = ShuntVoltage(-static_cast<uint16_t>((~(shunt_value - 1)) & 0xfff));
	}
}


namespace {

template <typename TypedArray, typename CoordinateType>
struct TypedBoolArrayToUint8Conversion
{
	uint8_t raw;

	TypedBoolArrayToUint8Conversion(uint8_t const& data = 0u) : raw(data) {}
	TypedBoolArrayToUint8Conversion(TypedArray const& array)
	{
		from_array(array);
	}

	void from_array(TypedArray const& arr)
	{
		raw = 0u;
		for (auto coord : halco::common::iter_all<CoordinateType>()) {
			raw |= static_cast<bool>(arr[coord]) << coord.toEnum();
		}
	}

	TypedArray to_array() const
	{
		TypedArray arr;
		for (auto coord : halco::common::iter_all<CoordinateType>()) {
			uint8_t const i = coord.toEnum();
			arr[coord] = static_cast<typename TypedArray::value_type>((bool) (raw & (1 << i)));
		}
		return arr;
	}
};

} // namespace

TCA9554Inputs::TCA9554Inputs() : m_input() {}

TCA9554Inputs::ChannelsBooleanArray const& TCA9554Inputs::get_channel_input() const
{
	return m_input;
}

void TCA9554Inputs::set_channel_input(const ChannelsBooleanArray& value)
{
	m_input = value;
}

bool TCA9554Inputs::operator==(TCA9554Inputs const& other) const
{
	return m_input == other.m_input;
}

bool TCA9554Inputs::operator!=(TCA9554Inputs const& other) const
{
	return !(*this == other);
}

std::array<
    halco::hicann_dls::vx::I2CTCA9554RoRegisterOnBoard,
    TCA9554Inputs::write_config_size_in_words>
TCA9554Inputs::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

std::array<
    halco::hicann_dls::vx::I2CTCA9554RoRegisterOnBoard,
    TCA9554Inputs::read_config_size_in_words>
TCA9554Inputs::read_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::I2CTCA9554RoRegisterOnBoard(
	    halco::hicann_dls::vx::I2CTCA9554RoRegisterOnTCA9554::inputs, coord.toTCA9554OnBoard())};
}

std::array<
    fisch::vx::word_access_type::I2CTCA9554RoRegister,
    TCA9554Inputs::write_config_size_in_words>
TCA9554Inputs::encode() const
{
	return {};
}

void TCA9554Inputs::decode(std::array<
                           fisch::vx::word_access_type::I2CTCA9554RoRegister,
                           TCA9554Inputs::read_config_size_in_words> const& data)
{
	m_input = TypedBoolArrayToUint8Conversion<
	              ChannelsBooleanArray, halco::hicann_dls::vx::TCA9554ChannelOnBoard>(data[0])
	              .to_array();
}

std::ostream& operator<<(std::ostream& os, TCA9554Inputs const& config)
{
	std::stringstream ss;
	for (bool val : config.m_input) {
		ss << val;
	}

	return (os << hate::name<TCA9554Inputs>() << "(inputs: " << ss.str() << ")");
}


TCA9554Config::TCA9554Config() : m_output(), m_polarity(), m_mode()
{
	m_output.fill(false);
	m_polarity.fill(ChannelPolarity::normal);
	m_mode.fill(ChannelMode::output);
}

TCA9554Config::ChannelsBooleanArray const& TCA9554Config::get_channel_output() const
{
	return m_output;
}

void TCA9554Config::set_channel_output(const ChannelsBooleanArray& value)
{
	m_output = value;
}

TCA9554Config::ChannelsPolarityArray const& TCA9554Config::get_channel_polarity() const
{
	return m_polarity;
}

void TCA9554Config::set_channel_polarity(const ChannelsPolarityArray& value)
{
	m_polarity = value;
}

TCA9554Config::ChannelsModeArray const& TCA9554Config::get_channel_mode() const
{
	return m_mode;
}

void TCA9554Config::set_channel_mode(const ChannelsModeArray& value)
{
	m_mode = value;
}

bool TCA9554Config::operator==(TCA9554Config const& other) const
{
	return m_polarity == other.m_polarity && m_mode == other.m_mode && m_output == other.m_output;
}

bool TCA9554Config::operator!=(TCA9554Config const& other) const
{
	return !(*this == other);
}

std::array<halco::hicann_dls::vx::I2CTCA9554RwRegisterOnBoard, TCA9554Config::config_size_in_words>
TCA9554Config::addresses(coordinate_type const& coord)
{
	halco::hicann_dls::vx::TCA9554OnBoard const base_coord = coord.toTCA9554OnBoard();

	return {halco::hicann_dls::vx::I2CTCA9554RwRegisterOnBoard(
	            halco::hicann_dls::vx::I2CTCA9554RwRegisterOnTCA9554::outputs, base_coord),
	        halco::hicann_dls::vx::I2CTCA9554RwRegisterOnBoard(
	            halco::hicann_dls::vx::I2CTCA9554RwRegisterOnTCA9554::polarity, base_coord),
	        halco::hicann_dls::vx::I2CTCA9554RwRegisterOnBoard(
	            halco::hicann_dls::vx::I2CTCA9554RwRegisterOnTCA9554::config, base_coord)};
}


std::array<fisch::vx::word_access_type::I2CTCA9554RwRegister, TCA9554Config::config_size_in_words>
TCA9554Config::encode() const
{
	return {
	    fisch::vx::word_access_type::I2CTCA9554RwRegister(
	        TypedBoolArrayToUint8Conversion<
	            ChannelsBooleanArray, halco::hicann_dls::vx::TCA9554ChannelOnBoard>(m_output)
	            .raw),
	    fisch::vx::word_access_type::I2CTCA9554RwRegister(
	        TypedBoolArrayToUint8Conversion<
	            ChannelsPolarityArray, halco::hicann_dls::vx::TCA9554ChannelOnBoard>(m_polarity)
	            .raw),
	    fisch::vx::word_access_type::I2CTCA9554RwRegister(
	        TypedBoolArrayToUint8Conversion<
	            ChannelsModeArray, halco::hicann_dls::vx::TCA9554ChannelOnBoard>(m_mode)
	            .raw)};
}

void TCA9554Config::decode(std::array<
                           fisch::vx::word_access_type::I2CTCA9554RwRegister,
                           TCA9554Config::config_size_in_words> const& data)
{
	m_output = TypedBoolArrayToUint8Conversion<
	               ChannelsBooleanArray, halco::hicann_dls::vx::TCA9554ChannelOnBoard>(data[0])
	               .to_array();
	m_polarity = TypedBoolArrayToUint8Conversion<
	                 ChannelsPolarityArray, halco::hicann_dls::vx::TCA9554ChannelOnBoard>(data[1])
	                 .to_array();
	m_mode = TypedBoolArrayToUint8Conversion<
	             ChannelsModeArray, halco::hicann_dls::vx::TCA9554ChannelOnBoard>(data[2])
	             .to_array();
}

std::ostream& operator<<(std::ostream& os, TCA9554Config const& config)
{
	std::stringstream ss_mode, ss_polarity, ss_output;

	for (auto coord : halco::common::iter_all<halco::hicann_dls::vx::TCA9554ChannelOnBoard>()) {
		bool const is_last = coord == halco::hicann_dls::vx::TCA9554ChannelOnBoard::max;

		ss_mode << (config.m_mode[coord] == TCA9554Config::ChannelMode::input ? "input   "
		                                                                      : "output  ")
		        << (is_last ? "" : " ");
		ss_polarity << (config.m_polarity[coord] == TCA9554Config::ChannelPolarity::normal
		                    ? "normal  "
		                    : "inverted")
		            << (is_last ? "" : " ");
		ss_output << (config.m_output[coord] == true ? "active  " : "inactive")
		          << (is_last ? "" : " ");
	}

	return os << hate::name<TCA9554Config>() << "(\n"
	          << "\tmode:     \t" << ss_mode.str() << "\n"
	          << "\tpolarity: \t" << ss_polarity.str() << "\n"
	          << "\toutput:   \t" << ss_output.str() << "\n)";
}


AD5252ChannelConfig::AD5252ChannelConfig() : m_value() {}

AD5252ChannelConfig::WiperSetting AD5252ChannelConfig::get_value() const
{
	return m_value;
}

void AD5252ChannelConfig::set_value(WiperSetting const value)
{
	m_value = value;
}

bool AD5252ChannelConfig::operator==(AD5252ChannelConfig const& other) const
{
	return m_value == other.m_value;
}

bool AD5252ChannelConfig::operator!=(AD5252ChannelConfig const& other) const
{
	return !(*this == other);
}

std::array<
    halco::hicann_dls::vx::I2CAD5252RwRegisterOnBoard,
    AD5252ChannelConfig::config_size_in_words>
AD5252ChannelConfig::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::I2CAD5252RwRegisterOnBoard(
	    halco::hicann_dls::vx::I2CAD5252RwRegisterOnAD5252Channel::rdac_volatile,
	    coord.toAD5252ChannelOnBoard())};
}
std::array<
    fisch::vx::word_access_type::I2CAD5252RwRegister,
    AD5252ChannelConfig::config_size_in_words>
AD5252ChannelConfig::encode() const
{
	return {fisch::vx::word_access_type::I2CAD5252RwRegister(m_value)};
}

void AD5252ChannelConfig::decode(std::array<
                                 fisch::vx::word_access_type::I2CAD5252RwRegister,
                                 AD5252ChannelConfig::config_size_in_words> const& data)
{
	m_value = WiperSetting(data[0]);
}

std::ostream& operator<<(std::ostream& os, AD5252ChannelConfig const& config)
{
	return os << hate::name<AD5252ChannelConfig>() << "(value: " << config.m_value << ")";
}


AD5252ChannelConfigPersistent::AD5252ChannelConfigPersistent() : m_value() {}

AD5252ChannelConfigPersistent::WiperSetting AD5252ChannelConfigPersistent::get_value() const
{
	return m_value;
}

void AD5252ChannelConfigPersistent::set_value(WiperSetting const value)
{
	m_value = value;
}

bool AD5252ChannelConfigPersistent::operator==(AD5252ChannelConfigPersistent const& other) const
{
	return m_value == other.m_value;
}

bool AD5252ChannelConfigPersistent::operator!=(AD5252ChannelConfigPersistent const& other) const
{
	return !(*this == other);
}

std::array<
    halco::hicann_dls::vx::I2CAD5252RwRegisterOnBoard,
    AD5252ChannelConfigPersistent::config_size_in_words>
AD5252ChannelConfigPersistent::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::I2CAD5252RwRegisterOnBoard(
	    halco::hicann_dls::vx::I2CAD5252RwRegisterOnAD5252Channel::eemem_persistent,
	    coord.toAD5252ChannelOnBoard())};
}
std::array<
    fisch::vx::word_access_type::I2CAD5252RwRegister,
    AD5252ChannelConfigPersistent::config_size_in_words>
AD5252ChannelConfigPersistent::encode() const
{
	return {fisch::vx::word_access_type::I2CAD5252RwRegister(m_value)};
}

void AD5252ChannelConfigPersistent::decode(
    std::array<
        fisch::vx::word_access_type::I2CAD5252RwRegister,
        AD5252ChannelConfigPersistent::config_size_in_words> const& data)
{
	m_value = WiperSetting(data[0]);
}

std::ostream& operator<<(std::ostream& os, AD5252ChannelConfigPersistent const& config)
{
	return os << hate::name<AD5252ChannelConfigPersistent>() << "(value: " << config.m_value << ")";
}


DAC6573ChannelConfig::Value DAC6573ChannelConfig::get_value() const
{
	return m_value;
}

void DAC6573ChannelConfig::set_value(Value value)
{
	m_value = value;
}

bool DAC6573ChannelConfig::operator==(DAC6573ChannelConfig const& other) const
{
	return other.m_value == m_value;
}

bool DAC6573ChannelConfig::operator!=(DAC6573ChannelConfig const& other) const
{
	return !(*this == other);
}

std::array<
    halco::hicann_dls::vx::I2CDAC6573RwRegisterOnBoard,
    DAC6573ChannelConfig::config_size_in_words>
DAC6573ChannelConfig::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::I2CDAC6573RwRegisterOnBoard(
	    coord.toDAC6573ChannelOnDAC6573(), coord.toDAC6573OnBoard())};
}

std::array<
    fisch::vx::word_access_type::I2CDAC6573RwRegister,
    DAC6573ChannelConfig::config_size_in_words>
DAC6573ChannelConfig::encode() const
{
	return {fisch::vx::word_access_type::I2CDAC6573RwRegister(m_value)};
}

void DAC6573ChannelConfig::decode(
    std::array<fisch::vx::word_access_type::I2CDAC6573RwRegister, config_size_in_words> const& data)
{
	m_value = Value(data[0]);
}

std::ostream& operator<<(std::ostream& os, DAC6573ChannelConfig const& config)
{
	return os << hate::name<DAC6573ChannelConfig>() << "(value: " << config.m_value << ")";
}

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::TMP112Status)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::INA219Config)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::INA219Status)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::TCA9554Inputs)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::TCA9554Config)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::AD5252ChannelConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::AD5252ChannelConfigPersistent)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::DAC6573ChannelConfig)
