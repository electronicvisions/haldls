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

class GENPYBIND(visible) JTAGClockScaler
{
public:
	typedef halco::hicann_dls::vx::JTAGOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value : public fisch::vx::JTAGClockScaler::Value
	{
		explicit Value(uintmax_t const value = 0) : fisch::vx::JTAGClockScaler::Value(value) {}
	};

	JTAGClockScaler() SYMBOL_VISIBLE;
	explicit JTAGClockScaler(Value value) SYMBOL_VISIBLE;

	void set(Value value) SYMBOL_VISIBLE;

	bool operator==(JTAGClockScaler const& other) const SYMBOL_VISIBLE;
	bool operator!=(JTAGClockScaler const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::JTAGOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::JTAGClockScaler, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::JTAGClockScaler, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void cerealize(Archive& ar);

	fisch::vx::JTAGClockScaler m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<JTAGClockScaler>
    : public BackendContainerBase<JTAGClockScaler, fisch::vx::JTAGClockScaler>
{};

} // namespace detail


class GENPYBIND(visible) ResetJTAGTap
{
public:
	typedef halco::hicann_dls::vx::JTAGOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	ResetJTAGTap() SYMBOL_VISIBLE;

	bool operator==(ResetJTAGTap const& other) const SYMBOL_VISIBLE;
	bool operator!=(ResetJTAGTap const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::JTAGOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::ResetJTAGTap, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::ResetJTAGTap, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void cerealize(Archive& ar);

	fisch::vx::ResetJTAGTap m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<ResetJTAGTap>
    : public BackendContainerBase<ResetJTAGTap, fisch::vx::ResetJTAGTap>
{};

} // namespace detail


class GENPYBIND(visible) JTAGIdCode
{
public:
	typedef halco::hicann_dls::vx::JTAGOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value : public fisch::vx::JTAGIdCode::Value
	{
		explicit Value(value_type const value = 0) : fisch::vx::JTAGIdCode::Value(value) {}
	};

	JTAGIdCode() SYMBOL_VISIBLE;

	Value get() const SYMBOL_VISIBLE;

	bool operator==(JTAGIdCode const& other) const SYMBOL_VISIBLE;
	bool operator!=(JTAGIdCode const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::JTAGOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::JTAGIdCode, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::JTAGIdCode, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void cerealize(Archive& ar);

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
