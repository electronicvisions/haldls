#pragma once

#include <array>
#include <ostream>

#include "fisch/vx/jtag.h"

#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container writing JTAG clock-scaler value.
 */
class GENPYBIND(visible) JTAGClockScaler
{
public:
	typedef halco::hicann_dls::vx::JTAGClockScalerOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Clock-scaler value type. The JTAG clock scales with 1 / (value + 2). */
	struct GENPYBIND(inline_base("*")) Value : public fisch::vx::JTAGClockScaler::Value
	{
		explicit Value(uintmax_t const value = 0) : fisch::vx::JTAGClockScaler::Value(value) {}
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

	/** Number of words for read-write access. */
	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;

	/**
	 * Get word addresses for encoding a write or reading.
	 * @return Array of addresses
	 */
	std::array<halco::hicann_dls::vx::JTAGClockScalerOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Get container config encoded in writable words.
	 * @return Array of word values
	 */
	std::array<fisch::vx::JTAGClockScaler, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	/**
	 * Decode read words into container config.
	 * @param data Array of word values
	 */
	void decode(std::array<fisch::vx::JTAGClockScaler, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar);

	fisch::vx::JTAGClockScaler m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<JTAGClockScaler>
    : public BackendContainerBase<JTAGClockScaler, fisch::vx::JTAGClockScaler>
{};

} // namespace detail


/**
 * Container for resetting JTAG state-machine.
 */
class GENPYBIND(visible) ResetJTAGTap
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

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::ResetJTAGTapOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::ResetJTAGTap, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::ResetJTAGTap, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar);

	fisch::vx::ResetJTAGTap m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<ResetJTAGTap>
    : public BackendContainerBase<ResetJTAGTap, fisch::vx::ResetJTAGTap>
{};

} // namespace detail


/**
 * Container for reading the JTAG IDCODE.
 */
class GENPYBIND(visible) JTAGIdCode
{
public:
	typedef halco::hicann_dls::vx::JTAGIdCodeOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** JTAG IDCODE value type. */
	struct GENPYBIND(inline_base("*")) Value : public fisch::vx::JTAGIdCode::Value
	{
		explicit Value(value_type const value = 0) : fisch::vx::JTAGIdCode::Value(value) {}
	};

	/** Default constructor. */
	JTAGIdCode() SYMBOL_VISIBLE;

	/**
	 * Get JTAG IDCODE.
	 * @return Value
	 */
	Value get() const SYMBOL_VISIBLE;

	bool operator==(JTAGIdCode const& other) const SYMBOL_VISIBLE;
	bool operator!=(JTAGIdCode const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, JTAGIdCode const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::JTAGIdCodeOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::JTAGIdCode, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::JTAGIdCode, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar);

	fisch::vx::JTAGIdCode m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<JTAGIdCode>
    : public BackendContainerBase<JTAGIdCode, fisch::vx::JTAGIdCode>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
