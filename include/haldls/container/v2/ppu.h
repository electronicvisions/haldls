#pragma once

#include <array>
#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"

namespace haldls {
namespace container {
namespace v2 {

class PPUMemoryWord
{
public:
	struct Value
		: public halco::common::detail::RantWrapper<Value, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	PPUMemoryWord() HALDLS_VISIBLE;
	explicit PPUMemoryWord(Value const& value) HALDLS_VISIBLE;

	Value get_value() const HALDLS_VISIBLE;
	void set_value(Value const& value) HALDLS_VISIBLE;

	bool operator==(PPUMemoryWord const& other) const HALDLS_VISIBLE;
	bool operator!=(PPUMemoryWord const& other) const HALDLS_VISIBLE;

private:
	Value m_value;
};

class PPUMemory
{
public:
	typedef std::array<PPUMemoryWord, halco::hicann_dls::v2::PPUMemoryWordOnDLS::size>
		words_type;

	PPUMemory() HALDLS_VISIBLE;
	explicit PPUMemory(words_type const& words) HALDLS_VISIBLE;

	words_type get_words() const HALDLS_VISIBLE;
	void set_words(words_type const& words) HALDLS_VISIBLE;

	PPUMemoryWord get_word(halco::hicann_dls::v2::PPUMemoryWordOnDLS const& pos) const HALDLS_VISIBLE;
	void set_word(
		halco::hicann_dls::v2::PPUMemoryWordOnDLS const& pos, PPUMemoryWord const& word) HALDLS_VISIBLE;

	bool operator==(PPUMemory const& other) const HALDLS_VISIBLE;
	bool operator!=(PPUMemory const& other) const HALDLS_VISIBLE;

private:
	words_type m_words;
};

class PPUControlRegister
{
public:
	PPUControlRegister() HALDLS_VISIBLE;

	bool get_inhibit_reset() const HALDLS_VISIBLE;
	void set_inhibit_reset(bool const value) HALDLS_VISIBLE;

	/* - force_clock_on: Forces the clock to be on, even if the ppu is sleeping
	 * - force_clock_off: Forces the clock to be off, which is weaker than
	 *   forcing the clock on
	 * None of these is needed in normal operation. Setting the inhibit_reset
	 * bit starts the execution. If the program finishes, the clock is stopped again. */
	bool get_force_clock_on() const HALDLS_VISIBLE;
	void set_force_clock_on(bool const value) HALDLS_VISIBLE;
	bool get_force_clock_off() const HALDLS_VISIBLE;
	void set_force_clock_off(bool const value) HALDLS_VISIBLE;

	bool operator==(PPUControlRegister const& other) const HALDLS_VISIBLE;
	bool operator!=(PPUControlRegister const& other) const HALDLS_VISIBLE;

private:
	bool m_inhibit_reset;
	bool m_force_clock_on;
	bool m_force_clock_off;
};

class PPUStatusRegister
{
public:
	PPUStatusRegister() HALDLS_VISIBLE;

	// Read only property
	bool get_sleep() const HALDLS_VISIBLE;

	bool operator==(PPUStatusRegister const& other) const HALDLS_VISIBLE;
	bool operator!=(PPUStatusRegister const& other) const HALDLS_VISIBLE;

private:
	bool m_sleep;
};

} // namespace v2
} // namespace container
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::PPUMemoryWord::Value)

} // namespace std
