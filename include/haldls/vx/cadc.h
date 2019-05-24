#pragma once
#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChip;
class OmnibusChipOverJTAG;
} // namespace fisch::vx

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
		constexpr explicit ResetWait(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	/**
	 * Dead time in cycles after linear ramp increase to wait for signal propagation to every
	 * channel to prevent cutting off leaf channel ramps too early.
	 */
	struct GENPYBIND(inline_base("*")) DeadTime
	    : public halco::common::detail::RantWrapper<DeadTime, uint_fast16_t, 255, 0>
	{
		constexpr explicit DeadTime(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
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

	bool operator==(CADCConfig const& other) const GENPYBIND(hidden) SYMBOL_VISIBLE;
	bool operator!=(CADCConfig const& other) const GENPYBIND(hidden) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCConfig const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

protected:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	bool m_enable;
	ResetWait m_reset_wait;
	DeadTime m_dead_time;
};

namespace detail {

template <>
struct BackendContainerTrait<CADCConfig>
    : public BackendContainerBase<
          CADCConfig,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
