#pragma once
#include <array>
#include <iosfwd>

#include "fisch/vx/word_access/type/reset.h"
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/highspeed_link.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace halco::hicann_dls::vx {
struct OmnibusAddress;
} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for enabling highspeed perf test.
 * Words transfering an increasing counter value are sent.
 * The received word count is expected to be equal to the sent count and all received words are
 * expected to be in order.
 * The test execution does not block playback execution.
 */
class GENPYBIND(visible) PerfTest
{
public:
	typedef halco::hicann_dls::vx::PerfTestOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Construct perf test with enable value.
	 * @param value Enable value of test execution
	 */
	explicit PerfTest(bool value = false) SYMBOL_VISIBLE;

	/**
	 * Set enable value of test execution.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable value of test execution.
	 * @return value Boolean value
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	bool operator==(PerfTest const& other) const SYMBOL_VISIBLE;
	bool operator!=(PerfTest const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PerfTest const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	bool m_enable;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PerfTest)

namespace detail {

template <>
struct BackendContainerTrait<PerfTest>
    : public BackendContainerBase<PerfTest, fisch::vx::word_access_type::Omnibus>
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
		constexpr explicit Sent(uintmax_t const val = 0) : rant_t(val) {}
	};

	/** Number of words received. */
	struct GENPYBIND(inline_base("*")) Received
	    : public halco::common::detail::RantWrapper<Received, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit Received(uintmax_t const val = 0) : rant_t(val) {}
	};

	/** Number of words received in order. */
	struct GENPYBIND(inline_base("*")) InOrder
	    : public halco::common::detail::RantWrapper<InOrder, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit InOrder(uintmax_t const val = 0) : rant_t(val) {}
	};

	/** First non-consecutive payload. */
	struct GENPYBIND(inline_base("*")) ErrorWord
	    : public halco::common::detail::RantWrapper<ErrorWord, uint_fast32_t, 0xffffffff, 0>
	{
		constexpr explicit ErrorWord(uintmax_t const val = 0) : rant_t(val) {}
	};

	/** Default constructor. */
	PerfTestStatus() SYMBOL_VISIBLE;

	/**
	 * Set number of successfully sent words.
	 * @param value Sent counter value
	 */
	GENPYBIND(setter_for(sent))
	void set_sent(Sent value) SYMBOL_VISIBLE;

	/**
	 * Get number of successfully sent words.
	 * @return Sent counter value
	 */
	GENPYBIND(getter_for(sent))
	Sent get_sent() const SYMBOL_VISIBLE;

	/**
	 * Set number of successfully received words.
	 * @param value Received counter value
	 */
	GENPYBIND(setter_for(received))
	void set_received(Received value) SYMBOL_VISIBLE;

	/**
	 * Get number of successfully received words.
	 * @return Received counter value
	 */
	GENPYBIND(getter_for(received))
	Received get_received() const SYMBOL_VISIBLE;

	/**
	 * Set number of words received in order.
	 * @param value InOrder counter value
	 */
	GENPYBIND(setter_for(in_order))
	void set_in_order(InOrder value) SYMBOL_VISIBLE;

	/**
	 * Get number of words received in order.
	 * @return InOrder counter value
	 */
	GENPYBIND(getter_for(in_order))
	InOrder get_in_order() const SYMBOL_VISIBLE;

	/**
	 * Set index of first non-consecutive word.
	 * @param value ErrorWord index value
	 */
	GENPYBIND(setter_for(error_word))
	void set_error_word(ErrorWord value) SYMBOL_VISIBLE;

	/**
	 * Get index of first non-consecutive word.
	 * @return ErrorWord index value
	 */
	GENPYBIND(getter_for(error_word))
	ErrorWord get_error_word() const SYMBOL_VISIBLE;

	bool operator==(PerfTestStatus const& other) const SYMBOL_VISIBLE;
	bool operator!=(PerfTestStatus const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PerfTestStatus const& config) SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 4;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	Sent m_sent;
	Received m_received;
	InOrder m_in_order;
	ErrorWord m_error_word;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PerfTestStatus)

namespace detail {

template <>
struct BackendContainerTrait<PerfTestStatus>
    : public BackendContainerBase<PerfTestStatus, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PerfTestStatus::Sent)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PerfTestStatus::Received)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PerfTestStatus::InOrder)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::PerfTestStatus::ErrorWord)

} // namespace std
