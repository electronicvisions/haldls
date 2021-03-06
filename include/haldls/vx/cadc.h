#pragma once
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/cadc.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/sram_controller.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"

namespace fisch::vx {
class Omnibus;
class OmnibusChipOverJTAG;
} // namespace fisch::vx

namespace halco::hicann_dls::vx {
class OmnibusAddress;
} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * CADC container with global digital config for a vertical half.
 */
class GENPYBIND(visible) CADCConfig
{
public:
	typedef std::true_type is_leaf_node;
	typedef halco::hicann_dls::vx::CADCConfigOnDLS coordinate_type;

	/**
	 * Wait time in cycles after ramp drop to compensate for analog low-pass filtering of ramp
	 * signal and preventing the drop of the last ramp to cause interference in the next linear
	 * ramp increase.
	 */
	struct GENPYBIND(inline_base("*")) ResetWait
	    : public halco::common::detail::RantWrapper<ResetWait, uint_fast16_t, 255, 0>
	{
		constexpr explicit ResetWait(uintmax_t const val = 100) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Dead time in cycles after linear ramp increase to wait for signal propagation to every
	 * channel to prevent cutting off leaf channel ramps too early.
	 */
	struct GENPYBIND(inline_base("*")) DeadTime
	    : public halco::common::detail::RantWrapper<DeadTime, uint_fast16_t, 255, 0>
	{
		constexpr explicit DeadTime(uintmax_t const val = 20) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	CADCConfig() SYMBOL_VISIBLE;

	/**
	 * Get enable value for the CADC.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set enable value for the CADC.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * Get ResetWait value common for both CADC in a vertical half.
	 * @return ResetWait value
	 */
	GENPYBIND(getter_for(reset_wait))
	ResetWait get_reset_wait() const SYMBOL_VISIBLE;

	/**
	 * Set ResetWait value common for both CADC in a vertical half.
	 * @param value ResetWait value
	 */
	GENPYBIND(setter_for(reset_wait))
	void set_reset_wait(ResetWait value) SYMBOL_VISIBLE;

	/**
	 * Get DeadTime value common for both CADC in a vertical half.
	 * @return DeadTime value
	 */
	GENPYBIND(getter_for(dead_time))
	DeadTime get_dead_time() const SYMBOL_VISIBLE;

	/**
	 * Set DeadTime value common for both CADC in a vertical half.
	 * @param value DeadTime value
	 */
	GENPYBIND(setter_for(dead_time))
	void set_dead_time(DeadTime value) SYMBOL_VISIBLE;

	bool operator==(CADCConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCConfig const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

protected:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable;
	ResetWait m_reset_wait;
	DeadTime m_dead_time;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CADCConfig)

namespace detail {

template <>
struct BackendContainerTrait<CADCConfig>
    : public BackendContainerBase<CADCConfig, fisch::vx::Omnibus, fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

template <typename Coordinates>
class CADCChannelConfig;

template <typename Coordinates>
std::ostream& operator<<(std::ostream& os, CADCChannelConfig<Coordinates> const& config);

/**
 * CADC container with channel-local digital offset config.
 */
template <typename Coordinates>
class SYMBOL_VISIBLE CADCChannelConfig
{
public:
	typedef std::true_type is_leaf_node;
	typedef typename Coordinates::CADCChannelConfigOnDLS coordinate_type;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware};

	/**
	 * Offset value to add to measurement.
	 */
	struct GENPYBIND(inline_base("*")) Offset
	    : public halco::common::detail::RantWrapper<Offset, int_fast16_t, 127, -128>
	{
		typedef halco::common::detail::RantWrapper<Offset, int_fast16_t, 127, -128> rant_t;
		constexpr explicit Offset(intmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	CADCChannelConfig();

	/**
	 * Get Offset value.
	 * @return Offset value
	 */
	GENPYBIND(getter_for(offset))
	Offset get_offset() const;

	/**
	 * Set Offset value.
	 * @param value Offset value
	 */
	GENPYBIND(setter_for(offset))
	void set_offset(Offset value);

	bool operator==(CADCChannelConfig const& other) const;
	bool operator!=(CADCChannelConfig const& other) const;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<<>(
	    std::ostream& os, CADCChannelConfig<Coordinates> const& config);

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

protected:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	Offset m_offset;
};

#define CADC_EXTERN_TEMPLATE(Coordinates)                                                          \
	extern template class SYMBOL_VISIBLE CADCChannelConfig<Coordinates>;                           \
	extern template SYMBOL_VISIBLE GENPYBIND(hidden) std::ostream& operator<<<Coordinates>(        \
	    std::ostream& os, CADCChannelConfig<Coordinates> const& value);                            \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    CADCChannelConfig<Coordinates>::config_size_in_words>                                      \
	CADCChannelConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusAddress,                                                     \
	    CADCChannelConfig<Coordinates>::config_size_in_words>                                      \
	CADCChannelConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
                                                                                                   \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    fisch::vx::OmnibusChipOverJTAG, CADCChannelConfig<Coordinates>::config_size_in_words>      \
	CADCChannelConfig<Coordinates>::encode() const;                                                \
	extern template SYMBOL_VISIBLE                                                                 \
	    std::array<fisch::vx::Omnibus, CADCChannelConfig<Coordinates>::config_size_in_words>       \
	    CADCChannelConfig<Coordinates>::encode() const;                                            \
                                                                                                   \
	extern template SYMBOL_VISIBLE void CADCChannelConfig<Coordinates>::decode(                    \
	    std::array<                                                                                \
	        fisch::vx::OmnibusChipOverJTAG,                                                        \
	        CADCChannelConfig<Coordinates>::config_size_in_words> const& data);                    \
	extern template SYMBOL_VISIBLE void CADCChannelConfig<Coordinates>::decode(                    \
	    std::array<                                                                                \
	        fisch::vx::Omnibus, CADCChannelConfig<Coordinates>::config_size_in_words> const&       \
	        data);

namespace detail {

template <typename Coordinates>
struct BackendContainerTrait<CADCChannelConfig<Coordinates>>
    : public BackendContainerBase<
          CADCChannelConfig<Coordinates>,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


// TODO: Switch to CRTP pattern when https://github.com/kljohann/genpybind/issues/28 is solved
class GENPYBIND(visible) CADCOffsetSRAMTimingConfig : public detail::SRAMTimingConfig
{
public:
	typedef halco::hicann_dls::vx::CADCOffsetSRAMTimingConfigOnDLS coordinate_type;

	bool operator==(CADCOffsetSRAMTimingConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCOffsetSRAMTimingConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCOffsetSRAMTimingConfig const& config)
	    SYMBOL_VISIBLE;

	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CADCOffsetSRAMTimingConfig)

namespace detail {

template <>
struct BackendContainerTrait<CADCOffsetSRAMTimingConfig>
    : public BackendContainerBase<
          CADCOffsetSRAMTimingConfig,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
