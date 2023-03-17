#pragma once
#include <array>
#include <iosfwd>

#include "fisch/vx/word_access/type/timer.h"
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/timing.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for resetting and reading the FPGA playback timer.
 */
class GENPYBIND(visible) Timer
{
public:
	typedef halco::hicann_dls::vx::TimerOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::BaseType<Value, uint32_t>
	{
		constexpr explicit Value(uintmax_t value = 0) GENPYBIND(implicit_conversion) : base_t(value)
		{}

		static const Value fpga_clock_cycles_per_us SYMBOL_VISIBLE;
	};

	/**
	 * Construct on-FPGA-timer with value.
	 * @param value Value to construct with
	 */
	explicit Timer(Value value = Value()) SYMBOL_VISIBLE;

	/**
	 * Set timer value.
	 * @param value Value to set
	 */
	void set(Value value) SYMBOL_VISIBLE;

	/**
	 * Get timer value.
	 * @return Value value
	 */
	Value get() const SYMBOL_VISIBLE;

	bool operator==(Timer const& other) const SYMBOL_VISIBLE;
	bool operator!=(Timer const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Timer const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::TimerOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Timer, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Timer, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	Value m_value;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(Timer)

namespace detail {

template <>
struct BackendContainerTrait<Timer>
    : public BackendContainerBase<Timer, fisch::vx::word_access_type::Timer>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::Timer::Value)

} // namespace std
