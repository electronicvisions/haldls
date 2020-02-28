#pragma once
#include <array>
#include <ostream>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for reading out the unique identifier of the FPGA.
 */
class GENPYBIND(visible) FPGADeviceDNA
{
public:
	typedef halco::hicann_dls::vx::FPGADeviceDNAOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Identifier value type. */
	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint64_t, 0x1ff'ffff'ffff'ffffull, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Construct FPGA identifier with value.
	 * @param value Value to construct with
	 */
	explicit FPGADeviceDNA(Value value = Value()) SYMBOL_VISIBLE;

	/**
	 * Get value.
	 * @return Value
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	/**
	 * Set value.
	 * @param value Value to set
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	bool operator==(FPGADeviceDNA const& other) const SYMBOL_VISIBLE;
	bool operator!=(FPGADeviceDNA const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, FPGADeviceDNA const& config) SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 2;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::OmnibusFPGA, write_config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::OmnibusFPGA, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	Value m_value;
};

/**
 * Container for the event (spikes, MADC samples) recording configuration register.
 */
class GENPYBIND(visible) EventRecordingConfig
{
public:
	typedef halco::hicann_dls::vx::EventRecordingConfigOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	explicit EventRecordingConfig() SYMBOL_VISIBLE;

	/**
	 * Get enable event recording.
	 * @return bool
	 */
	GENPYBIND(getter_for(enable_event_recording))
	bool get_enable_event_recording() const SYMBOL_VISIBLE;

	/**
	 * Set enable_event_recording.
	 * @param value bool to set
	 */
	GENPYBIND(setter_for(enable_event_recording))
	void set_enable_event_recording(bool value) SYMBOL_VISIBLE;

	bool operator==(EventRecordingConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(EventRecordingConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, EventRecordingConfig const& config) SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, read_config_size_in_words> read_addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::OmnibusFPGA, write_config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::OmnibusFPGA, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	bool m_enable_event_recording;
};

namespace detail {

template <>
struct BackendContainerTrait<FPGADeviceDNA>
    : public BackendContainerBase<FPGADeviceDNA, fisch::vx::OmnibusFPGA>
{};

template <>
struct BackendContainerTrait<EventRecordingConfig>
    : public BackendContainerBase<EventRecordingConfig, fisch::vx::OmnibusFPGA>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::FPGADeviceDNA::Value)

} // namespace std
