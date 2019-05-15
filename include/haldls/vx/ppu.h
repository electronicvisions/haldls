#pragma once

#include <array>
#include <ostream>

#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

#include "haldls/cerealization.h"

namespace fisch::vx {
class OmnibusChipOverJTAG;
class OmnibusChip;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) PPUMemoryWord
{
public:
	typedef halco::hicann_dls::vx::PPUMemoryWordOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	PPUMemoryWord() SYMBOL_VISIBLE;
	explicit PPUMemoryWord(Value const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(value))
	void set_value(Value const& value) SYMBOL_VISIBLE;

	bool operator==(PPUMemoryWord const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUMemoryWord const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PPUMemoryWord const& pmw) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	Value m_value;
};

class GENPYBIND(visible) PPUControlRegister
{
public:
	typedef halco::hicann_dls::vx::PPUControlRegisterOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	PPUControlRegister() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(cache_controller_enable))
	bool get_cache_controller_enable() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(cache_controller_enable))
	void set_cache_controller_enable(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(inhibit_reset))
	bool get_inhibit_reset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(inhibit_reset))
	void set_inhibit_reset(bool const value) SYMBOL_VISIBLE;

	/* - force_clock_on: Forces the clock to be on, even if the ppu is sleeping
	 * - force_clock_off: Forces the clock to be off, which is weaker than
	 *   forcing the clock on
	 * None of these is needed in normal operation. Setting the inhibit_reset
	 * bit starts the execution. If the program finishes, the clock is stopped again. */
	GENPYBIND(getter_for(force_clock_on))
	bool get_force_clock_on() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(force_clock_on))
	void set_force_clock_on(bool const value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(force_clock_off))
	bool get_force_clock_off() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(force_clock_off))
	void set_force_clock_off(bool const value) SYMBOL_VISIBLE;

	bool operator==(PPUControlRegister const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUControlRegister const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	bool m_cache_controller_enable;
	bool m_inhibit_reset;
	bool m_force_clock_on;
	bool m_force_clock_off;
};

class GENPYBIND(visible) PPUStatusRegister
{
public:
	typedef halco::hicann_dls::vx::PPUStatusRegisterOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	PPUStatusRegister() SYMBOL_VISIBLE;

	// Read only property
	GENPYBIND(getter_for(sleep))
	bool get_sleep() const SYMBOL_VISIBLE;

	bool operator==(PPUStatusRegister const& other) const SYMBOL_VISIBLE;
	bool operator!=(PPUStatusRegister const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	bool m_sleep;
};

namespace detail {

template <>
struct BackendContainerTrait<PPUMemoryWord>
    : public BackendContainerBase<
          PPUMemoryWord,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

template <>
struct BackendContainerTrait<PPUStatusRegister>
    : public BackendContainerBase<
          PPUStatusRegister,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

template <>
struct BackendContainerTrait<PPUControlRegister>
    : public BackendContainerBase<
          PPUControlRegister,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PPUMemoryWord::Value)

} // namespace std
