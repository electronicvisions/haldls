#include "haldls/vx/i2c.h"

#include "fisch/vx/i2c.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/i2c.h"
#include "haldls/cerealization.tcc"

namespace haldls::vx {

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

std::array<fisch::vx::I2CINA219RwRegister, INA219Config::config_size_in_words>
INA219Config::encode() const
{
	INA219ConfigBitfield bitfield;
	bitfield.u.m.mode = 0b111 /* bus and shunt continuous */;
	bitfield.u.m.shunt_adc_mode = static_cast<uint16_t>(m_shunt_adc_mode);
	bitfield.u.m.bus_adc_mode = static_cast<uint16_t>(m_bus_adc_mode);
	bitfield.u.m.shunt_voltage_range = 0b00 /* +-40mV */;
	bitfield.u.m.bus_voltage_range = 0b0 /* 16V */;
	bitfield.u.m.reset = 0;

	return {fisch::vx::I2CINA219RwRegister(fisch::vx::I2CINA219RwRegister::Value(bitfield.u.raw))};
}

void INA219Config::decode(
    std::array<fisch::vx::I2CINA219RwRegister, INA219Config::config_size_in_words> const& data)
{
	INA219ConfigBitfield bitfield(data[0].get());
	m_shunt_adc_mode = ADCMode(bitfield.u.m.shunt_adc_mode);
	m_bus_adc_mode = ADCMode(bitfield.u.m.bus_adc_mode);
}

template <typename Archive>
void INA219Config::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_bus_adc_mode));
	ar(CEREAL_NVP(m_shunt_adc_mode));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(INA219Config)


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

std::array<fisch::vx::I2CINA219RoRegister, INA219Status::write_config_size_in_words>
INA219Status::encode() const
{
	return {};
}

void INA219Status::decode(
    std::array<fisch::vx::I2CINA219RoRegister, INA219Status::read_config_size_in_words> const& data)
{
	auto const bus = data[0].get();
	auto const shunt = data[1].get();

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

template <typename Archive>
void INA219Status::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_bus_voltage));
	ar(CEREAL_NVP(m_shunt_voltage));
	ar(CEREAL_NVP(m_bus_voltage_overflow));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(INA219Status)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::INA219Config, 0)
CEREAL_CLASS_VERSION(haldls::vx::INA219Status, 0)
