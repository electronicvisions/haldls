#pragma once
#include <array>
#include <limits>
#include <ostream>

#include "fisch/vx/constants.h"
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/i2c.h"
#include "halco/hicann-dls/vx/xboard.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"

namespace cereal {
class access;
} // namespace cereal

namespace halco::hicann_dls::vx {} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) INA219Config
{
public:
	typedef halco::hicann_dls::vx::INA219ConfigOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};

	/** ADC resolution and number of samples. */
	enum class ADCMode
	{
		bits9 = 0,
		bits10 = 1,
		bits11 = 2,
		bits12 = 3,
		bits12_samples1 = 4,
		bits12_samples2 = 5,
		bits12_samples4 = 6,
		bits12_samples8 = 7,
		bits12_samples16 = 8,
		bits12_samples32 = 9,
		bits12_samples64 = 10,
		bits12_samples128 = 11
	};

	/** Default constructor. */
	INA219Config() SYMBOL_VISIBLE;

	/**
	 * Get bus ADC mode value.
	 * @return Value
	 */
	GENPYBIND(getter_for(bus_adc_mode))
	ADCMode get_bus_adc_mode() const SYMBOL_VISIBLE;

	/**
	 * Set bus ADC mode value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(bus_adc_mode))
	void set_bus_adc_mode(ADCMode value) SYMBOL_VISIBLE;

	/**
	 * Get shunt ADC mode value.
	 * @return Value
	 */
	GENPYBIND(getter_for(shunt_adc_mode))
	ADCMode get_shunt_adc_mode() const SYMBOL_VISIBLE;

	/**
	 * Set shunt ADC mode value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(shunt_adc_mode))
	void set_shunt_adc_mode(ADCMode value) SYMBOL_VISIBLE;

	bool operator==(INA219Config const& other) const SYMBOL_VISIBLE;
	bool operator!=(INA219Config const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, INA219Config const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::I2CINA219RwRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::I2CINA219RwRegister, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::I2CINA219RwRegister, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	ADCMode m_bus_adc_mode;
	ADCMode m_shunt_adc_mode;
};

namespace detail {

template <>
struct BackendContainerTrait<INA219Config>
    : public BackendContainerBase<INA219Config, fisch::vx::I2CINA219RwRegister>
{};

} // namespace detail


class GENPYBIND(visible) INA219Status
{
public:
	typedef halco::hicann_dls::vx::INA219StatusOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};

	/**
	 * Bus voltage with linear conversion and LSB = 4mV.
	 */
	struct GENPYBIND(inline_base("*")) BusVoltage
	    : public halco::common::detail::RantWrapper<BusVoltage, uint_fast16_t, 4000, 0>
	{
		constexpr explicit BusVoltage(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}

		/**
		 * Convert to uncalibrated voltage [V] (i.e. ideal conversion).
		 */
		float toUncalibratedVoltage() const SYMBOL_VISIBLE;
	};

	/**
	 * Shunt voltage with linear conversion and LSB = 10uV.
	 */
	struct GENPYBIND(inline_base("*")) ShuntVoltage
	    : public halco::common::detail::RantWrapper<ShuntVoltage, int16_t, 4000, -4000>
	{
		constexpr explicit ShuntVoltage(intmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}

		/**
		 * Convert to uncalibrated voltage [V] (i.e. ideal conversion).
		 */
		float toUncalibratedVoltage() const SYMBOL_VISIBLE;
	};

	/**
	 * Uncalibrated power [W].
	 */
	struct GENPYBIND(visible) UncalibratedPower
	{
		/**
		 * Uncalibrated bus voltage [V].
		 */
		float uncalibrated_bus_voltage;

		/**
		 * Uncalibrated shunt voltage [V].
		 */
		float uncalibrated_shunt_voltage;

		/*
		 * Shunt resistance [Ohm].
		 */
		float shunt_resistance = 0.027;

		float calculate() const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, UncalibratedPower const& data)
		    SYMBOL_VISIBLE;
	};

	/** Default constructor. */
	INA219Status() SYMBOL_VISIBLE;

	/**
	 * Get bus voltage value.
	 * @return Value
	 */
	GENPYBIND(getter_for(bus_voltage))
	BusVoltage get_bus_voltage() const SYMBOL_VISIBLE;

	/**
	 * Set bus voltage value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(bus_voltage))
	void set_bus_voltage(BusVoltage value) SYMBOL_VISIBLE;

	/**
	 * Get bus voltage overflow value.
	 * @return Value
	 */
	GENPYBIND(getter_for(bus_voltage_overflow))
	bool get_bus_voltage_overflow() const SYMBOL_VISIBLE;

	/**
	 * Set bus voltage overflow value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(bus_voltage_overflow))
	void set_bus_voltage_overflow(bool value) SYMBOL_VISIBLE;

	/**
	 * Get shunt voltage value.
	 * @return Value
	 */
	GENPYBIND(getter_for(shunt_voltage))
	ShuntVoltage get_shunt_voltage() const SYMBOL_VISIBLE;

	/**
	 * Set shunt voltage value.
	 * @param value Value
	 */
	GENPYBIND(setter_for(shunt_voltage))
	void set_shunt_voltage(ShuntVoltage value) SYMBOL_VISIBLE;

	/**
	 * Convert bus and shunt voltage to uncalibrated power value.
	 * The voltages are assumed to be correct, the shunt resistance defaults to the expected value
	 * of 0.027 Ohm.
	 */
	UncalibratedPower toUncalibratedPower() const SYMBOL_VISIBLE;

	bool operator==(INA219Status const& other) const SYMBOL_VISIBLE;
	bool operator!=(INA219Status const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, INA219Status const& config) SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 2;
	static std::array<halco::hicann_dls::vx::I2CINA219RoRegisterOnBoard, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::I2CINA219RoRegisterOnBoard, read_config_size_in_words>
	read_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::I2CINA219RoRegister, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::I2CINA219RoRegister, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	BusVoltage m_bus_voltage;
	bool m_bus_voltage_overflow;
	ShuntVoltage m_shunt_voltage;
};

namespace detail {

template <>
struct BackendContainerTrait<INA219Status>
    : public BackendContainerBase<INA219Status, fisch::vx::I2CINA219RoRegister>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
