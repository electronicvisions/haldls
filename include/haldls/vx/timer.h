#pragma once

#include <array>
#include <ostream>

#include "fisch/vx/timer.h"

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
 * Container for resetting the FPGA playback timer.
 */
class GENPYBIND(visible) Timer
{
public:
	typedef halco::hicann_dls::vx::TimerOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value : public fisch::vx::Timer::Value
	{
		explicit Value(value_type const value = 0) GENPYBIND(implicit_conversion) :
		    fisch::vx::Timer::Value(value)
		{}
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

	bool operator==(Timer const& other) const SYMBOL_VISIBLE;
	bool operator!=(Timer const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Timer const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::TimerOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::Timer, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Timer, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar);

	fisch::vx::Timer m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<Timer> : public BackendContainerBase<Timer, fisch::vx::Timer>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
