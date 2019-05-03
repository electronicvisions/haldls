#pragma once

#include <array>
#include <ostream>

#include "fisch/vx/reset.h"

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
 * Container for enabling highspeed perf test.
 * Words transfering an increasing counter value are sent.
 * The received word count is expected to be equal to the sent count and all received words are
 * expected to be in order.
 */
class GENPYBIND(visible) PerfTest
{
public:
	typedef halco::hicann_dls::vx::PerfTestOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	explicit PerfTest(bool value = false) SYMBOL_VISIBLE;

	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	bool operator==(PerfTest const& other) const SYMBOL_VISIBLE;
	bool operator!=(PerfTest const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PerfTest const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::OmnibusFPGA, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::OmnibusFPGA, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void cerealize(Archive& ar);

	bool m_enable;
};

namespace detail {

template <>
struct BackendContainerTrait<PerfTest>
    : public BackendContainerBase<PerfTest, fisch::vx::OmnibusFPGA>
{};

} // namespace detail


/**
 * Container for perf test result readout.
 */
class GENPYBIND(visible) PerfTestStatus
{
public:
	typedef halco::hicann_dls::vx::PerfTestStatusOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Number of words sent successfully. */
	struct GENPYBIND(inline_base("*")) Sent
	    : public halco::common::detail::RantWrapper<Sent, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit Sent(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	/** Number of words received. */
	struct GENPYBIND(inline_base("*")) Received
	    : public halco::common::detail::RantWrapper<Received, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit Received(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	/** Number of words received in order. */
	struct GENPYBIND(inline_base("*")) InOrder
	    : public halco::common::detail::RantWrapper<InOrder, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit InOrder(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
	};

	PerfTestStatus() SYMBOL_VISIBLE;

	GENPYBIND(setter_for(sent))
	void set_sent(Sent value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(sent))
	Sent get_sent() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(received))
	void set_received(Received value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(received))
	Received get_received() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(in_order))
	void set_in_order(InOrder value) SYMBOL_VISIBLE;
	GENPYBIND(getter_for(in_order))
	InOrder get_in_order() const SYMBOL_VISIBLE;

	bool operator==(PerfTestStatus const& other) const SYMBOL_VISIBLE;
	bool operator!=(PerfTestStatus const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PerfTestStatus const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, config_size_in_words> addresses(
	    coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::OmnibusFPGA, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::OmnibusFPGA, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void cerealize(Archive& ar);

	Sent m_sent;
	Received m_received;
	InOrder m_in_order;
};

namespace detail {

template <>
struct BackendContainerTrait<PerfTestStatus>
    : public BackendContainerBase<PerfTestStatus, fisch::vx::OmnibusFPGA>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PerfTestStatus::Sent)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PerfTestStatus::Received)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PerfTestStatus::InOrder)

} // namespace std
