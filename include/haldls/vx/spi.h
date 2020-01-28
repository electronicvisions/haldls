#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class SPIShiftRegister;
class SPIDACDataRegister;
class SPIDACControlRegister;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for configuration of the 24-bit wide shift register controlling six VDD switches,
 * the selection of the ADC source, ADC power down and reset pins, six LEDs, routing two
 * DAC channels to a differential debug line and the CapMem reference current connection.
 * Information about the routing can be found in the xBoard wiki under
 * https://brainscales-r.kip.uni-heidelberg.de/projects/symap2ic/wiki/xboard.
 */
class GENPYBIND(visible) ShiftRegister : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::ShiftRegisterOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;

	enum class AnalogReadoutMux1Input
	{
		off,
		readout_chain_0,
		readout_chain_1,
		mux_rfu_0
	};

	enum class AnalogReadoutMux2Input
	{
		off,
		v_reset,
		vdd_res_meas,
		mux_dac_25
	};

	enum class AnalogReadoutMux3Input
	{
		off,
		i_ref,
		mux_rfu_1,
		mux_rfu_2
	};

	/** Default constructor. */
	ShiftRegister() SYMBOL_VISIBLE;

	/**
	 * Get enable value for connecting the DAC generating the CapMem reference current to the chip
	 * carrier board.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_i_ref_board))
	bool get_enable_i_ref_board() const SYMBOL_VISIBLE;

	/**
	 * Set enable value for connecting the DAC generating the CapMem reference current to the chip
	 * carrier board.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_i_ref_board))
	void set_enable_i_ref_board(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable value for connecting the CapMem reference current to ground via a 2.2MOhm
	 * resistor for a current measurement with the ADC.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_measure_i_ref))
	bool get_enable_measure_i_ref() const SYMBOL_VISIBLE;

	/**
	 * Set enable value for connecting the CapMem reference current to ground via a 2.2MOhm
	 * resistor for a current measurement with the ADC.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_measure_i_ref))
	void set_enable_measure_i_ref(bool value) SYMBOL_VISIBLE;

	/**
	 * Get the source selection for the first analog readout MUX.
	 * @return Selected input
	 */
	GENPYBIND(getter_for(select_analog_readout_mux_1_input))
	AnalogReadoutMux1Input get_select_analog_readout_mux_1_input() const SYMBOL_VISIBLE;

	/**
	 * Set the source selection for the first analog readout MUX.
	 * @param value Input to be selected
	 */
	GENPYBIND(setter_for(select_analog_readout_mux_1_input))
	void set_select_analog_readout_mux_1_input(AnalogReadoutMux1Input value) SYMBOL_VISIBLE;

	/**
	 * Get the source selection for the second analog readout MUX.
	 * @return Selected input
	 */
	GENPYBIND(getter_for(select_analog_readout_mux_2_input))
	AnalogReadoutMux2Input get_select_analog_readout_mux_2_input() const SYMBOL_VISIBLE;

	/**
	 * Set the source selection for the second analog readout MUX.
	 * @param value Input to be selected
	 */
	GENPYBIND(setter_for(select_analog_readout_mux_2_input))
	void set_select_analog_readout_mux_2_input(AnalogReadoutMux2Input value) SYMBOL_VISIBLE;

	/**
	 * Get the source selection for the third analog readout MUX.
	 * @return Selected input
	 */
	GENPYBIND(getter_for(select_analog_readout_mux_3_input))
	AnalogReadoutMux3Input get_select_analog_readout_mux_3_input() const SYMBOL_VISIBLE;

	/**
	 * Set the source selection for the third analog readout MUX.
	 * @param value Input to be selected
	 */
	GENPYBIND(setter_for(select_analog_readout_mux_3_input))
	void set_select_analog_readout_mux_3_input(AnalogReadoutMux3Input value) SYMBOL_VISIBLE;

	/**
	 * Get enable value for the DAC-powered Readout0 debug line.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_dac_to_readout_0))
	bool get_enable_dac_to_readout_0() const SYMBOL_VISIBLE;

	/**
	 * Set enable value for the DAC-powered Readout0 debug line.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_dac_to_readout_0))
	void set_enable_dac_to_readout_0(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable value for the DAC-powered Readout1 debug line.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_dac_to_readout_1))
	bool get_enable_dac_to_readout_1() const SYMBOL_VISIBLE;

	/**
	 * Set enable value for the DAC-powered Readout0 debug line.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_dac_to_readout_1))
	void set_enable_dac_to_readout_1(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable value for the specified LED.
	 * @param coord Coordinate of LED to get enable value for
	 * @return Boolean value
	 */
	bool get_enable_led(halco::hicann_dls::vx::LEDOnBoard const& coord) const SYMBOL_VISIBLE;

	/**
	 * Get enable value for the specified LED.
	 * @param coord Coordinate of LED to set enable value for
	 * @param value Boolean value
	 */
	void set_enable_led(halco::hicann_dls::vx::LEDOnBoard const& coord, bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable value of the ADC power-down pin. The behavior can be controlled in the ADC
	 * container.
	 * @see ADC
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_adc_power_down))
	bool get_enable_adc_power_down() const SYMBOL_VISIBLE;

	/**
	 * Get enable value of the ADC power-down pin. The behavior can be controlled in the ADC
	 * container.
	 * @see ADC
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_adc_power_down))
	void set_enable_adc_power_down(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable value of the ADC reset pin.
	 * @see ADC
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_adc_reset))
	bool get_enable_adc_reset() const SYMBOL_VISIBLE;

	/**
	 * Set enable value of the ADC reset pin.
	 * @see ADC
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_adc_reset))
	void set_enable_adc_reset(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable value for the specified VDD line.
	 * @param coord Coordinate of VDD line to get enable value for
	 * @return Boolean value
	 */
	bool get_enable_vdd(halco::hicann_dls::vx::VDDOnBoard const& coord) const SYMBOL_VISIBLE;

	/**
	 * Set enable value for the specified VDD line.
	 * @param coord Coordinate of VDD line to set enable value for
	 * @param value Boolean value
	 */
	void set_enable_vdd(halco::hicann_dls::vx::VDDOnBoard const& coord, bool value) SYMBOL_VISIBLE;

	bool operator==(ShiftRegister const& other) const SYMBOL_VISIBLE;
	bool operator!=(ShiftRegister const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ShiftRegister const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::SPIShiftRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::SPIShiftRegister, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::SPIShiftRegister, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	AnalogReadoutMux1Input m_mux_1;
	AnalogReadoutMux2Input m_mux_2;
	AnalogReadoutMux3Input m_mux_3;
	bool m_enable_i_ref_board;
	bool m_enable_measure_i_ref;
	bool m_enable_dac_to_readout_0;
	bool m_enable_dac_to_readout_1;
	halco::common::typed_array<bool, halco::hicann_dls::vx::LEDOnBoard> m_enable_led;
	bool m_enable_adc_power_down;
	bool m_enable_adc_reset;
	halco::common::typed_array<bool, halco::hicann_dls::vx::VDDOnBoard> m_enable_vdd;
};

namespace detail {

template <>
struct BackendContainerTrait<ShiftRegister>
    : public BackendContainerBase<ShiftRegister, fisch::vx::SPIShiftRegister>
{};

} // namespace detail


/**
 * Container for individual configuration of the value of a DAC channel of the xBoard DACs.
 */
class GENPYBIND(visible) DACChannel : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::DACChannelOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast32_t, 0xfff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Construct DAC channel with value.
	 * @param value Value to construct with
	 */
	DACChannel(Value const& value = Value()) : m_value(value) {}

	/**
	 * Set DAC channel value.
	 * @param value Value to set
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	/**
	 * Get DAC channel value.
	 * @return Value to get
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	bool operator==(DACChannel const& other) const SYMBOL_VISIBLE;
	bool operator!=(DACChannel const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, DACChannel const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::SPIDACDataRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::SPIDACDataRegister, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::SPIDACDataRegister, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	Value m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<DACChannel>
    : public BackendContainerBase<DACChannel, fisch::vx::SPIDACDataRegister>
{};

} // namespace detail


/**
 * Container for enabling DAC channels of a xBoard DAC.
 */
class GENPYBIND(visible) DACControl : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::DACOnBoard coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor. */
	DACControl() SYMBOL_VISIBLE;

	/**
	 * Set DAC channel enable value.
	 * @param channel Channel to set value for
	 * @param value Boolean value to set
	 */
	void set_enable_channel(halco::hicann_dls::vx::DACChannelOnDAC const& channel, bool value)
	    SYMBOL_VISIBLE;

	/**
	 * Get DAC channel enable value.
	 * @param channel Channel to get value for
	 * @return Boolean value
	 */
	bool get_enable_channel(halco::hicann_dls::vx::DACChannelOnDAC const& channel) const
	    SYMBOL_VISIBLE;

	bool operator==(DACControl const& other) const SYMBOL_VISIBLE;
	bool operator!=(DACControl const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, DACControl const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	static std::array<halco::hicann_dls::vx::SPIDACControlRegisterOnBoard, config_size_in_words>
	addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::SPIDACControlRegister, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::SPIDACControlRegister, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<bool, halco::hicann_dls::vx::DACChannelOnDAC> m_enable_channel;
};

namespace detail {

template <>
struct BackendContainerTrait<DACControl>
    : public BackendContainerBase<DACControl, fisch::vx::SPIDACControlRegister>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::DACChannel::Value)

} // namespace std
