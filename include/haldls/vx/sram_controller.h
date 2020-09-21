#pragma once
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
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

namespace detail GENPYBIND_MODULE {

/**
 * Configuration of full-custom SRAM timing.
 *
 *                         enable width
 *                         |---------------|
 * address _____/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_________
 * enable  ________________/¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\_________
 *                                       ^
 *              |----------|-------------|
 *                address    read delay
 *               setup time
 */
// NOTE: This is not a complete container to be used directly but rather a base class for
// specialized containers for the different uses of the same controller type.
class GENPYBIND(visible) SRAMTimingConfig
{
public:
	typedef std::true_type is_leaf_node;

	/**
	 * Wait time after enable signal pull for cells to drive their value until read.
	 */
	struct GENPYBIND(inline_base("*")) ReadDelay
	    : public halco::common::detail::
	          RantWrapper<ReadDelay, uint_fast16_t, hate::math::pow(2, 8) - 1, 0>
	{
		constexpr explicit ReadDelay(uintmax_t const val = 120) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Wait time for address value propagation after enable signal pull.
	 */
	struct GENPYBIND(inline_base("*")) AddressSetupTime
	    : public halco::common::detail::
	          RantWrapper<AddressSetupTime, uint_fast16_t, hate::math::pow(2, 4) - 1, 0>
	{
		constexpr explicit AddressSetupTime(uintmax_t const val = 11)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Duration of enable signal pull.
	 */
	struct GENPYBIND(inline_base("*")) EnableWidth
	    : public halco::common::detail::
	          RantWrapper<EnableWidth, uint_fast16_t, hate::math::pow(2, 4) - 1, 0>
	{
		constexpr explicit EnableWidth(uintmax_t const val = 11) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	SRAMTimingConfig() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(read_delay))
	ReadDelay get_read_delay() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(read_delay))
	void set_read_delay(ReadDelay value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(address_setup_time))
	AddressSetupTime get_address_setup_time() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(address_setup_time))
	void set_address_setup_time(AddressSetupTime value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_width))
	EnableWidth get_enable_width() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_width))
	void set_enable_width(EnableWidth value) SYMBOL_VISIBLE;

	bool operator==(SRAMTimingConfig const& other) const GENPYBIND(hidden) SYMBOL_VISIBLE;
	bool operator!=(SRAMTimingConfig const& other) const GENPYBIND(hidden) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t) SYMBOL_VISIBLE;

	ReadDelay m_read_delay;
	AddressSetupTime m_address_setup_time;
	EnableWidth m_enable_width;
};

} // namespace detail

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(detail::SRAMTimingConfig)

} // namespace vx
} // namespace haldls
