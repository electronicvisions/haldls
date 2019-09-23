#pragma once

#include <algorithm>
#include <ostream>

#include "halco/hicann-dls/vx/coordinates.h"

#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusFPGA;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) HicannARQStatus
{
public:
	typedef halco::hicann_dls::vx::HicannARQStatusOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Number of words submitted to the ARQ from the Chip.
	 */
	struct GENPYBIND(inline_base("*")) ReadCount
	    : public halco::common::detail::BaseType<ReadCount, uint64_t>
	{
		constexpr explicit ReadCount(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : base_t(val)
		{}
	};

	/**
	 * Number of words submitted to the ARQ from the FPGA.
	 */
	struct GENPYBIND(inline_base("*")) WriteCount
	    : public halco::common::detail::BaseType<WriteCount, uint32_t>
	{
		constexpr explicit WriteCount(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : base_t(val)
		{}
	};

	/**
	 * Number of words sent from the ARQ from the Chip to the FPGA.
	 */
	struct GENPYBIND(inline_base("*")) RxCount
	    : public halco::common::detail::BaseType<RxCount, uint32_t>
	{
		constexpr explicit RxCount(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : base_t(val)
		{}
	};

	/**
	 * Number of words sent from the ARQ from the FPGA to the Chip.
	 */
	struct GENPYBIND(inline_base("*")) TxCount
	    : public halco::common::detail::BaseType<TxCount, uint32_t>
	{
		constexpr explicit TxCount(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : base_t(val)
		{}
	};

	HicannARQStatus() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(read_count))
	ReadCount get_read_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(read_count))
	void set_read_count(ReadCount value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(write_count))
	WriteCount get_write_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(write_count))
	void set_write_count(WriteCount value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(rx_count))
	RxCount get_rx_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(rx_count))
	void set_rx_count(RxCount value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(tx_count))
	TxCount get_tx_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(tx_count))
	void set_tx_count(TxCount value) SYMBOL_VISIBLE;

	bool operator==(HicannARQStatus const& other) const SYMBOL_VISIBLE;
	bool operator!=(HicannARQStatus const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 4;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::OmnibusFPGA, write_config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::OmnibusFPGA, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, HicannARQStatus const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	ReadCount m_read_count;
	WriteCount m_write_count;
	RxCount m_rx_count;
	TxCount m_tx_count;
};

namespace detail {

template <>
struct BackendContainerTrait<HicannARQStatus>
    : public BackendContainerBase<HicannARQStatus, fisch::vx::OmnibusFPGA>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::HicannARQStatus::ReadCount)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::HicannARQStatus::WriteCount)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::HicannARQStatus::RxCount)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::HicannARQStatus::TxCount)

} // namespace std
