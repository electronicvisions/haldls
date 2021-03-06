#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace halco::hicann_dls::vx {
class OmnibusAddress;
} // namespace halco::hicann_dls::vx

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
	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::Omnibus, write_config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Omnibus, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(FPGADeviceDNA)

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
	std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words> read_addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words> write_addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::Omnibus, write_config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Omnibus, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable_event_recording;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(EventRecordingConfig)

class GENPYBIND(visible) ExternalPPUMemoryByte
{
public:
	typedef halco::hicann_dls::vx::ExternalPPUMemoryByteOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast16_t, 0xff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) : rant_t(val) {}
	};

	typedef uint8_t raw_type;
	// verify that the underlying word size matches the value type size
	static_assert(
	    static_cast<raw_type>(-1) == Value::max, "raw_type size does not match Value type.");

	explicit ExternalPPUMemoryByte(Value value = Value()) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(value))
	void set_value(Value const& value) SYMBOL_VISIBLE;

	bool operator==(ExternalPPUMemoryByte const& other) const SYMBOL_VISIBLE;
	bool operator!=(ExternalPPUMemoryByte const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ExternalPPUMemoryByte const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::Omnibus, config_size_in_words> encode(coordinate_type const& coord) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    coordinate_type const& coord,
	    std::array<fisch::vx::Omnibus, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);

	Value m_value;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(ExternalPPUMemoryByte)

namespace detail {

template <>
struct BackendContainerTrait<FPGADeviceDNA>
    : public BackendContainerBase<FPGADeviceDNA, fisch::vx::Omnibus>
{};

template <>
struct BackendContainerTrait<EventRecordingConfig>
    : public BackendContainerBase<EventRecordingConfig, fisch::vx::Omnibus>
{};

template <>
struct BackendContainerTrait<ExternalPPUMemoryByte>
    : public BackendContainerBase<ExternalPPUMemoryByte, fisch::vx::Omnibus>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::FPGADeviceDNA::Value)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ExternalPPUMemoryByte::Value)

} // namespace std
