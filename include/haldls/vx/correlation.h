#pragma once
#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/capmem.h"
#include "halco/hicann-dls/vx/correlation.h"
#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct CommonCorrelationConfig;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::CommonCorrelationConfig& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx::word_access_type {
struct Omnibus;
struct OmnibusChipOverJTAG;
} // namespace fisch::vx::word_access_type

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Correlation reset and readout timing configuration container.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) CommonCorrelationConfig
    : public ContainerBase<CommonCorrelationConfig>
{
public:
	typedef std::true_type is_leaf_node;
	typedef halco::hicann_dls::vx::CommonCorrelationConfigOnDLS coordinate_type;

	/**
	 * Wait time in PPU clock cycles between connecting the outputs of a synapse
	 * to the CADC inputs and starting the CADC read.
	 */
	struct GENPYBIND(inline_base("*")) SenseDelay
	    : public halco::common::detail::
	          RantWrapper<SenseDelay, uint_fast16_t, hate::math::pow(2, 12) - 1, 0>
	{
		constexpr explicit SenseDelay(uintmax_t const val = 1) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Duration of correlation reset for synapses (+3 ppu clock cycles).
	 * Determines how long the measurement capacitor is connected to the reset voltage.
	 */
	struct GENPYBIND(inline_base("*")) ResetDuration
	    : public halco::common::detail::
	          RantWrapper<ResetDuration, uint_fast16_t, hate::math::pow(2, 12) - 1, 0>
	{
		constexpr explicit ResetDuration(uintmax_t const val = 200) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Wait time in PPU clock cycles after disabling the correlation column reset enable.
	 */
	struct GENPYBIND(inline_base("*")) ResetFallTime
	    : public halco::common::detail::
	          RantWrapper<ResetFallTime, uint_fast16_t, hate::math::pow(2, 12) - 1, 0>
	{
		constexpr explicit ResetFallTime(uintmax_t const val = 5) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Switch between normal and auto-calibrating correlation reset. */
	enum class ResetMode : uint_fast8_t
	{
		normal = 0b0,
		auto_calibrating = 0b1
	};

	/** Define the source of the CADC ramp offset voltages. */
	typedef halco::common::typed_array<
	    halco::hicann_dls::vx::CapMemBlockOnHemisphere,
	    halco::hicann_dls::vx::CapMemBlockOnHemisphere>
	    RampOffsets GENPYBIND(opaque(false));

	/** Default constructor. */
	CommonCorrelationConfig() SYMBOL_VISIBLE;

	/**
	 * Wait time in PPU clock cycles between connecting the outputs of a synapse
	 * to the CADC inputs and starting the CADC read.
	 */
	GENPYBIND(getter_for(sense_delay))
	SenseDelay get_sense_delay() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(sense_delay))
	void set_sense_delay(SenseDelay value) SYMBOL_VISIBLE;

	/**
	 * Duration of correlation reset for synapses (+3 ppu clock cycles).
	 * Determines how long the measurement capacitor is connected to the reset voltage.
	 */
	GENPYBIND(getter_for(reset_duration))
	ResetDuration get_reset_duration() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(reset_duration))
	void set_reset_duration(ResetDuration value) SYMBOL_VISIBLE;

	/**
	 * Wait time in PPU clock cycles after disabling the correlation column reset enable.
	 */
	GENPYBIND(getter_for(reset_fall_time))
	ResetFallTime get_reset_fall_time() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(reset_fall_time))
	void set_reset_fall_time(ResetFallTime value) SYMBOL_VISIBLE;

	/**
	 * Reset mode for correlation storage capacitors.
	 * Defaults to normal mode.
	 */
	GENPYBIND(getter_for(reset_mode))
	ResetMode get_reset_mode() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(reset_mode))
	void set_reset_mode(ResetMode value) SYMBOL_VISIBLE;

	/**
	 * Ramp offset voltages for the CADCs.
	 * The analog parameter v_offset can be switched between the left and right
	 * quadrants. This allows using a low voltage for measurements that is shared
	 * for both CADC ramps, and a different high voltage during auto-calibrating reset.
	 */
	GENPYBIND(getter_for(cadc_v_offset_assignment), return_value_policy(reference))
	RampOffsets const& get_cadc_v_offset_assignment() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cadc_v_offset_assignment))
	void set_cadc_v_offset_assignment(RampOffsets const& value) SYMBOL_VISIBLE;

	bool operator==(CommonCorrelationConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonCorrelationConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CommonCorrelationConfig const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 4;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar, CommonCorrelationConfig& value, std::uint32_t const version) SYMBOL_VISIBLE;

	SenseDelay m_sense_delay;
	ResetDuration m_reset_duration;
	ResetFallTime m_reset_fall_time;
	ResetMode m_reset_mode;
	RampOffsets m_cadc_v_offset_assignment;
};

std::ostream& operator<<(std::ostream& os, CommonCorrelationConfig::ResetMode const& config)
    SYMBOL_VISIBLE;

} // namespace vx
} // namespace haldls

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<CommonCorrelationConfig>
    : public BackendContainerBase<
          haldls::vx::CommonCorrelationConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace haldls::vx::detail
