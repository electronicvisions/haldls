#pragma once
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/correlation.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"

namespace fisch::vx {
class Omnibus;
class OmnibusChipOverJTAG;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Correlation reset and readout timing configuration container.
 */
class GENPYBIND(visible) CommonCorrelationConfig
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
	 * Set true to use "auto" mode, set false to use legacy mode.
	 * Defaults to legacy mode, auto mode may not work.
	 */
	GENPYBIND(getter_for(reset_mode))
	bool get_reset_mode() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(reset_mode))
	void set_reset_mode(bool value) SYMBOL_VISIBLE;

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
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	SenseDelay m_sense_delay;
	ResetDuration m_reset_duration;
	ResetFallTime m_reset_fall_time;
	bool m_reset_mode;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CommonCorrelationConfig)

namespace detail {

template <>
struct BackendContainerTrait<CommonCorrelationConfig>
    : public BackendContainerBase<
          CommonCorrelationConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
