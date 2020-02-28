#pragma once
#include <array>
#include <ostream>

#include "halco/hicann-dls/vx/madc.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class OmnibusChip;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for MADC control state machine.
 * Flow chart of state machine:
 * https://chat.bioai.eu/visions/pl/s4o4mq36xtyeigtzkw9kgfihcc
 * FIXME: Add more explanatory description (issue #3471)
 */
class GENPYBIND(visible) MADCControl
{
public:
	typedef halco::hicann_dls::vx::MADCControlOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	MADCControl() SYMBOL_VISIBLE;

	// accessors

	/** Enable switched capacitor amplifier for current measurements. */
	GENPYBIND(getter_for(enable_iconv_amplifier))
	bool get_enable_iconv_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_iconv_amplifier))
	void set_enable_iconv_amplifier(bool const value) SYMBOL_VISIBLE;

	/** Enable switched capacitor pre-amplifier for voltage measurements. */
	GENPYBIND(getter_for(enable_pre_amplifier))
	bool get_enable_pre_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_pre_amplifier))
	void set_enable_pre_amplifier(bool const value) SYMBOL_VISIBLE;

	/** Let the MADC sample continuously. */
	GENPYBIND(getter_for(enable_continuous_sampling))
	bool get_enable_continuous_sampling() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_continuous_sampling))
	void set_enable_continuous_sampling(bool const value) SYMBOL_VISIBLE;

	/** Let the MADC power down after it finished sampling. */
	GENPYBIND(getter_for(enable_power_down_after_sampling))
	bool get_enable_power_down_after_sampling() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_power_down_after_sampling))
	void set_enable_power_down_after_sampling(bool const value) SYMBOL_VISIBLE;

	/** Stop a currently active recording. */
	GENPYBIND(getter_for(stop_recording))
	bool get_stop_recording() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(stop_recording))
	void set_stop_recording(bool const value) SYMBOL_VISIBLE;

	/** Start a recording. */
	GENPYBIND(getter_for(start_recording))
	bool get_start_recording() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(start_recording))
	void set_start_recording(bool const value) SYMBOL_VISIBLE;

	/** Wake up the MADC. */
	GENPYBIND(getter_for(wake_up))
	bool get_wake_up() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wake_up))
	void set_wake_up(bool const value) SYMBOL_VISIBLE;

	bool operator==(MADCControl const& other) const SYMBOL_VISIBLE;
	bool operator!=(MADCControl const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MADCControl const& event) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	bool m_enable_iconv_amplifier;
	bool m_enable_pre_amplifier;
	bool m_enable_continuous_sampling;
	bool m_enable_power_down_after_sampling;
	bool m_stop_recording;
	bool m_start_recording;
	bool m_wake_up;
};

namespace detail {

template <>
struct BackendContainerTrait<MADCControl>
    : public BackendContainerBase<
          MADCControl,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
