#pragma once
#include <array>
#include <iosfwd>
#include <limits>

#include "fisch/vx/constants.h"
#include "fisch/vx/word_access/type/i2c.h"
#include "halco/common/geometry.h"
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/i2c.h"
#include "halco/hicann-dls/vx/ultra96.h"
#include "halco/hicann-dls/vx/xboard.h"
#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"
#ifndef __ppu__
#include "hxcomm/vx/target.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct INA219Config;
struct INA219Status;
struct TCA9554Inputs;
struct TCA9554Config;
struct AD5252ChannelConfig;
struct AD5252ChannelConfigPersistent;
struct DAC6573ChannelConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::INA219Config& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::INA219Status& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::TCA9554Inputs& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::TCA9554Config& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::AD5252ChannelConfig& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::AD5252ChannelConfigPersistent& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::DAC6573ChannelConfig& value, std::uint32_t const version);

} // namespace cereal
#endif

namespace halco::hicann_dls::vx {} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) INA219Config
    : public ContainerBase<INA219Config>
{
public:
	typedef halco::hicann_dls::vx::INA219ConfigOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

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
	std::array<fisch::vx::word_access_type::I2CINA219RwRegister, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<fisch::vx::word_access_type::I2CINA219RwRegister, config_size_in_words> const&
	        data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, INA219Config& value, std::uint32_t const version)
	    SYMBOL_VISIBLE;

	ADCMode m_bus_adc_mode;
	ADCMode m_shunt_adc_mode;
};

std::ostream& operator<<(std::ostream& os, INA219Config::ADCMode const& mode) SYMBOL_VISIBLE;

#ifndef __ppu__

#endif

namespace detail {

template <>
struct BackendContainerTrait<INA219Config>
    : public BackendContainerBase<INA219Config, fisch::vx::word_access_type::I2CINA219RwRegister>
{};

} // namespace detail


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) INA219Status
    : public ContainerBase<INA219Status>
{
public:
	typedef halco::hicann_dls::vx::INA219StatusOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

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
	std::array<fisch::vx::word_access_type::I2CINA219RoRegister, write_config_size_in_words>
	encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<
	            fisch::vx::word_access_type::I2CINA219RoRegister,
	            read_config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, INA219Status& value, std::uint32_t const version)
	    SYMBOL_VISIBLE;

	BusVoltage m_bus_voltage;
	bool m_bus_voltage_overflow;
	ShuntVoltage m_shunt_voltage;
};

#ifndef __ppu__

#endif

namespace detail {

template <>
struct BackendContainerTrait<INA219Status>
    : public BackendContainerBase<INA219Status, fisch::vx::word_access_type::I2CINA219RoRegister>
{};

} // namespace detail


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) TCA9554Inputs
    : public ContainerBase<TCA9554Inputs>
{
public:
	typedef halco::hicann_dls::vx::TCA9554InputsOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

	typedef GENPYBIND(opaque) halco::common::
	    typed_array<bool, halco::hicann_dls::vx::TCA9554ChannelOnBoard> ChannelsBooleanArray;

	/* Default constructor */
	TCA9554Inputs() SYMBOL_VISIBLE;

	/**
	 * Get input value for all channels.
	 * @return Array of channel input values
	 */
	GENPYBIND(getter_for(channel_input), return_value_policy(reference_internal))
	ChannelsBooleanArray const& get_channel_input() const SYMBOL_VISIBLE;

	/**
	 * Set input value for all channels.
	 * @param value Array of channel input values
	 */
	GENPYBIND(setter_for(channel_input))
	void set_channel_input(ChannelsBooleanArray const& value) SYMBOL_VISIBLE;

	bool operator==(TCA9554Inputs const& other) const SYMBOL_VISIBLE;
	bool operator!=(TCA9554Inputs const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, TCA9554Inputs const& config) SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static std::
	    array<halco::hicann_dls::vx::I2CTCA9554RoRegisterOnBoard, write_config_size_in_words>
	    write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::I2CTCA9554RoRegisterOnBoard, read_config_size_in_words>
	read_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::I2CTCA9554RoRegister, write_config_size_in_words>
	encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<
	            fisch::vx::word_access_type::I2CTCA9554RoRegister,
	            read_config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, TCA9554Inputs& value, std::uint32_t const version)
	    SYMBOL_VISIBLE;

	ChannelsBooleanArray m_input;
};

#ifndef __ppu__

#endif

namespace detail {

template <>
struct BackendContainerTrait<TCA9554Inputs>
    : public BackendContainerBase<TCA9554Inputs, fisch::vx::word_access_type::I2CTCA9554RoRegister>
{};

} // namespace detail

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) TCA9554Config
    : public ContainerBase<TCA9554Config>
{
public:
	typedef halco::hicann_dls::vx::TCA9554ConfigOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

	enum class ChannelPolarity : bool
	{
		normal = false,
		inverted = true
	};

	enum class ChannelMode : bool
	{
		output = false,
		input = true
	};

	typedef GENPYBIND(opaque(false)) halco::common::
	    typed_array<bool, halco::hicann_dls::vx::TCA9554ChannelOnBoard> ChannelsBooleanArray;
	typedef GENPYBIND(opaque) halco::common::typed_array<
	    ChannelPolarity,
	    halco::hicann_dls::vx::TCA9554ChannelOnBoard> ChannelsPolarityArray;
	typedef GENPYBIND(opaque) halco::common::
	    typed_array<ChannelMode, halco::hicann_dls::vx::TCA9554ChannelOnBoard> ChannelsModeArray;

	/** Default constructor. */
	TCA9554Config() SYMBOL_VISIBLE;

	/**
	 * Get output value for all channels.
	 * @return Array of output values
	 **/
	GENPYBIND(getter_for(channel_output), return_value_policy(reference_internal))
	ChannelsBooleanArray const& get_channel_output() const SYMBOL_VISIBLE;

	/**
	 * Set output value for all channels.
	 * @param value Array of output values
	 **/
	GENPYBIND(setter_for(channel_output))
	void set_channel_output(ChannelsBooleanArray const& value) SYMBOL_VISIBLE;

	/**
	 * Get polarity inversion mode of all channels.
	 * @return Array of polarity inversion modes
	 **/
	GENPYBIND(getter_for(channel_polarity), return_value_policy(reference_internal))
	ChannelsPolarityArray const& get_channel_polarity() const SYMBOL_VISIBLE;

	/**
	 * Set polarity inversion mode of all channels.
	 * @param value Array of polarity inversion modes
	 **/
	GENPYBIND(setter_for(channel_polarity))
	void set_channel_polarity(ChannelsPolarityArray const& value) SYMBOL_VISIBLE;

	/**
	 * Get input/output mode for all channels.
	 * @return Array of channel input/output modes
	 **/
	GENPYBIND(getter_for(channel_mode), return_value_policy(reference_internal))
	ChannelsModeArray const& get_channel_mode() const SYMBOL_VISIBLE;

	/**
	 * Set input/output mode for all channels.
	 * @param value Array of channel input/output modes
	 **/
	GENPYBIND(setter_for(channel_mode))
	void set_channel_mode(ChannelsModeArray const& value) SYMBOL_VISIBLE;

	bool operator==(TCA9554Config const& other) const SYMBOL_VISIBLE;
	bool operator!=(TCA9554Config const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, TCA9554Config const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	static std::array<halco::hicann_dls::vx::I2CTCA9554RwRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::I2CTCA9554RwRegister, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<fisch::vx::word_access_type::I2CTCA9554RwRegister, config_size_in_words> const&
	        data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, TCA9554Config& value, std::uint32_t const version)
	    SYMBOL_VISIBLE;

	ChannelsBooleanArray m_output;
	ChannelsPolarityArray m_polarity;
	ChannelsModeArray m_mode;
};

#ifndef __ppu__

#endif

namespace detail {

template <>
struct BackendContainerTrait<TCA9554Config>
    : public BackendContainerBase<TCA9554Config, fisch::vx::word_access_type::I2CTCA9554RwRegister>
{};

} // namespace detail


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) AD5252ChannelConfig
    : public ContainerBase<AD5252ChannelConfig>
{
public:
	typedef halco::hicann_dls::vx::AD5252ChannelConfigOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

	struct GENPYBIND(inline_base("*")) WiperSetting
	    : public halco::common::detail::RantWrapper<
	          WiperSetting,
	          uint_fast16_t,
	          std::numeric_limits<uint8_t>::max(),
	          std::numeric_limits<uint8_t>::min()>
	{
		constexpr explicit WiperSetting(uintmax_t const val = 0x90) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	AD5252ChannelConfig() SYMBOL_VISIBLE;

	/**
	 * Get wiper setting for digital potentiometer.
	 * @return Wiper setting
	 */
	GENPYBIND(getter_for(value))
	WiperSetting get_value() const SYMBOL_VISIBLE;

	/**
	 * Set wiper setting for digital potentiometer.
	 * @param value The wiper setting.
	 */
	GENPYBIND(setter_for(value))
	void set_value(WiperSetting value) SYMBOL_VISIBLE;

	bool operator==(AD5252ChannelConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(AD5252ChannelConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, AD5252ChannelConfig const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::I2CAD5252RwRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::I2CAD5252RwRegister, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<fisch::vx::word_access_type::I2CAD5252RwRegister, config_size_in_words> const&
	        data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar, AD5252ChannelConfig& value, std::uint32_t const version) SYMBOL_VISIBLE;

	WiperSetting m_value;
};

#ifndef __ppu__

#endif

namespace detail {

template <>
struct BackendContainerTrait<AD5252ChannelConfig>
    : public BackendContainerBase<
          AD5252ChannelConfig,
          fisch::vx::word_access_type::I2CAD5252RwRegister>
{};

} // namespace detail


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) AD5252ChannelConfigPersistent
    : public ContainerBase<AD5252ChannelConfigPersistent>
{
public:
	typedef halco::hicann_dls::vx::AD5252ChannelConfigPersistentOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

	struct GENPYBIND(inline_base("*")) WiperSetting
	    : public halco::common::detail::RantWrapper<
	          WiperSetting,
	          uint_fast16_t,
	          std::numeric_limits<uint8_t>::max(),
	          std::numeric_limits<uint8_t>::min()>
	{
		constexpr explicit WiperSetting(uintmax_t const val = 0x90) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};


	/** Default constructor. */
	AD5252ChannelConfigPersistent() SYMBOL_VISIBLE;

	/**
	 * Get wiper setting for digital potentiometer.
	 * @return Wiper setting
	 */
	GENPYBIND(getter_for(value))
	WiperSetting get_value() const SYMBOL_VISIBLE;

	/**
	 * Set wiper setting for digital potentiometer.
	 * @param value The wiper setting.
	 */
	GENPYBIND(setter_for(value))
	void set_value(WiperSetting value) SYMBOL_VISIBLE;

	bool operator==(AD5252ChannelConfigPersistent const& other) const SYMBOL_VISIBLE;
	bool operator!=(AD5252ChannelConfigPersistent const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, AD5252ChannelConfigPersistent const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::I2CAD5252RwRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::I2CAD5252RwRegister, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<fisch::vx::word_access_type::I2CAD5252RwRegister, config_size_in_words> const&
	        data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar,
	    AD5252ChannelConfigPersistent& value,
	    std::uint32_t const version) SYMBOL_VISIBLE;

	WiperSetting m_value;
};

#ifndef __ppu__

#endif

namespace detail {

template <>
struct BackendContainerTrait<AD5252ChannelConfigPersistent>
    : public BackendContainerBase<
          AD5252ChannelConfigPersistent,
          fisch::vx::word_access_type::I2CAD5252RwRegister>
{};

} // namespace detail


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) DAC6573ChannelConfig
    : public ContainerBase<DAC6573ChannelConfig>
{
public:
	typedef halco::hicann_dls::vx::DAC6573ChannelOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation};
#endif

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<
	          Value,
	          uint_fast32_t,
	          fisch::vx::dac6573_value_max,
	          fisch::vx::dac_value_min>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	DAC6573ChannelConfig(Value const& value = Value()) : m_value(value) {}

	/**
	 * Get DAC channel value.
	 * @return Value
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	/**
	 * Set DAC channel value.
	 * @param value Value to set
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	bool operator==(DAC6573ChannelConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(DAC6573ChannelConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, DAC6573ChannelConfig const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::I2CDAC6573RwRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::I2CDAC6573RwRegister, config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<fisch::vx::word_access_type::I2CDAC6573RwRegister, config_size_in_words> const&
	        data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar, DAC6573ChannelConfig& value, std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value;
};

#ifndef __ppu__

#endif

namespace detail {

template <>
struct BackendContainerTrait<DAC6573ChannelConfig>
    : public BackendContainerBase<
          DAC6573ChannelConfig,
          fisch::vx::word_access_type::I2CDAC6573RwRegister>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::DAC6573ChannelConfig::Value)

}
