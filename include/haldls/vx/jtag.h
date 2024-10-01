#pragma once
#include <array>
#include <iosfwd>

#include "fisch/vx/constants.h"
#include "fisch/vx/word_access/type/jtag.h"
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/jtag.h"
#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct JTAGClockScaler;
struct ResetJTAGTap;
struct JTAGIdCode;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::JTAGClockScaler& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ResetJTAGTap& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::JTAGIdCode& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {
class JTAGClockScaler;
class JTAGIdCode;
class ResetJTAGTap;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container writing JTAG clock-scaler value.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) JTAGClockScaler
    : public ContainerBase<JTAGClockScaler>
{
public:
	typedef halco::hicann_dls::vx::JTAGClockScalerOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Clock-scaler value type. The JTAG clock scales with 1 / (value + 2). */
	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<
	          Value,
	          uint_fast16_t,
	          fisch::vx::jtag_clock_scaler_max,
	          fisch::vx::jtag_clock_scaler_min>
	{
		constexpr explicit Value(uintmax_t const value = 6) GENPYBIND(implicit_conversion) :
		    rant_t(value)
		{}
	};

	/**
	 * Construct scaler with value.
	 * @param value Value to construct scaler with
	 */
	explicit JTAGClockScaler(Value value = Value()) SYMBOL_VISIBLE;

	/**
	 * Set clock-scaler value.
	 * @param value Clock-scaler value to set
	 */
	void set(Value value) SYMBOL_VISIBLE;

	bool operator==(JTAGClockScaler const& other) const SYMBOL_VISIBLE;
	bool operator!=(JTAGClockScaler const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, JTAGClockScaler const& config) SYMBOL_VISIBLE;

	/** Number of words for write access. */
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;

	/** Number of words for read access. */
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;

	/**
	 * Get word addresses for encoding a write.
	 * @return Array of addresses
	 */
	static std::array<halco::hicann_dls::vx::JTAGClockScalerOnDLS, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Get word addresses for encoding a read.
	 * @return Array of addresses
	 */
	static std::array<halco::hicann_dls::vx::JTAGClockScalerOnDLS, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Get container config encoded in writable words.
	 * @return Array of word values
	 */
	std::array<fisch::vx::word_access_type::JTAGClockScaler, write_config_size_in_words> encode()
	    const SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Decode read words into container config.
	 * @param data Array of word values
	 */
	void decode(
	    std::array<fisch::vx::word_access_type::JTAGClockScaler, read_config_size_in_words> const&
	        data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(
	    Archive& ar, JTAGClockScaler& value, std::uint32_t const version);

	Value m_value;
};


namespace detail {

template <>
struct BackendContainerTrait<JTAGClockScaler>
    : public BackendContainerBase<JTAGClockScaler, fisch::vx::word_access_type::JTAGClockScaler>
{};

} // namespace detail


/**
 * Container for resetting JTAG state-machine.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ResetJTAGTap
    : public ContainerBase<ResetJTAGTap>
{
public:
	typedef halco::hicann_dls::vx::ResetJTAGTapOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor. */
	ResetJTAGTap() SYMBOL_VISIBLE;

	bool operator==(ResetJTAGTap const& other) const SYMBOL_VISIBLE;
	bool operator!=(ResetJTAGTap const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ResetJTAGTap const& config) SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::ResetJTAGTapOnDLS, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ResetJTAGTapOnDLS, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::ResetJTAGTap, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<fisch::vx::word_access_type::ResetJTAGTap, read_config_size_in_words> const&
	        data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, ResetJTAGTap& value, std::uint32_t const version);
};


namespace detail {

template <>
struct BackendContainerTrait<ResetJTAGTap>
    : public BackendContainerBase<ResetJTAGTap, fisch::vx::word_access_type::ResetJTAGTap>
{};

} // namespace detail


/**
 * Container for reading the JTAG IDCODE.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) JTAGIdCode
    : public ContainerBase<JTAGIdCode>
{
public:
	typedef halco::hicann_dls::vx::JTAGIdCodeOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Hardware revision number, starting from 0.
	 */
	struct GENPYBIND(inline_base("*")) Version
	    // Parameter is specified to be 4bit in JTAG spec.
	    : public halco::common::detail::RantWrapper<Version, uint_fast8_t, 15, 0>
	{
		constexpr explicit Version(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Device identification number.
	 */
	struct GENPYBIND(inline_base("*")) PartNumber
	    // Parameter is specified to be 16bit in JTAG spec.
	    : public halco::common::detail::RantWrapper<PartNumber, uint_fast16_t, 65535, 0>
	{
		constexpr explicit PartNumber(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 *  JEDEC Manufacturer ID code, Heidelberg University has '0x057'.
	 */
	struct GENPYBIND(inline_base("*")) ManufacturerId
	    // Parameter is specified to be 11bit in JTAG spec.
	    : public halco::common::detail::RantWrapper<ManufacturerId, uint_fast16_t, 2047, 0>
	{
		constexpr explicit ManufacturerId(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/** Default constructor. */
	JTAGIdCode() SYMBOL_VISIBLE;

	/**
	 * Get hardware revision number, starting from 0.
	 */
	GENPYBIND(getter_for(version))
	Version get_version() const SYMBOL_VISIBLE;

	/**
	 * Get device identification number.
	 */
	GENPYBIND(getter_for(part_number))
	PartNumber get_part_number() const SYMBOL_VISIBLE;

	/**
	 * Get JEDEC Manufacturer ID code, Heidelberg University has '0x057'.
	 */
	GENPYBIND(getter_for(manufacturer_id))
	ManufacturerId get_manufacturer_id() const SYMBOL_VISIBLE;

	/**
	 * Set hardware revision number, starting from 0.
	 */
	GENPYBIND(setter_for(version))
	void set_version(Version value) SYMBOL_VISIBLE;

	/**
	 * Set device identification number.
	 */
	GENPYBIND(setter_for(part_number))
	void set_part_number(PartNumber value) SYMBOL_VISIBLE;

	/**
	 * Set JEDEC Manufacturer ID code, Heidelberg University has '0x057'.
	 */
	GENPYBIND(setter_for(manufacturer_id))
	void set_manufacturer_id(ManufacturerId value) SYMBOL_VISIBLE;

	bool operator==(JTAGIdCode const& other) const SYMBOL_VISIBLE;
	bool operator!=(JTAGIdCode const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, JTAGIdCode const& config) SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::JTAGIdCodeOnDLS, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::JTAGIdCodeOnDLS, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::JTAGIdCode, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(
	    std::array<fisch::vx::word_access_type::JTAGIdCode, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, JTAGIdCode& value, std::uint32_t const version);

	Version m_version;
	PartNumber m_part_number;
	ManufacturerId m_manufacturer_id;
};


namespace detail {

template <>
struct BackendContainerTrait<JTAGIdCode>
    : public BackendContainerBase<JTAGIdCode, fisch::vx::word_access_type::JTAGIdCode>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
