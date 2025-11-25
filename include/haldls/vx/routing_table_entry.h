#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/event.h"
#include "halco/hicann-dls/vx/routing_table_entry.h"

#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct OutputRoutingTableEntry;
struct InputRoutingTableEntry;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::OutputRoutingTableEntry& value, std::uint32_t const version);

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::InputRoutingTableEntry& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Routing table entry for outgoing events.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) OutputRoutingTableEntry
    : public ContainerBase<OutputRoutingTableEntry>
{
public:
	typedef halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Inter-chip-routing label.
	 */
	struct GENPYBIND(inline_base("*")) Label
	    : public halco::common::detail::
	          RantWrapper<Label, uint_fast64_t, hate::math::pow(2, 15) - 1, 0>
	{
		constexpr explicit Label(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{
		}
	};

	/** Default constructor. */
	OutputRoutingTableEntry() SYMBOL_VISIBLE;

	/**
	 * Get enable value.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set enable value.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * Get right label of routing table entry.
	 * @return Right label of routing table entry.
	 */
	GENPYBIND(getter_for(label))
	OutputRoutingTableEntry::Label get_label() const SYMBOL_VISIBLE;

	/**
	 * Set right label of routing table entry.
	 * @param value Right label of routing table entry.
	 */
	GENPYBIND(setter_for(label))
	void set_label(OutputRoutingTableEntry::Label const& value) SYMBOL_VISIBLE;

	bool operator==(OutputRoutingTableEntry const& other) const SYMBOL_VISIBLE;
	bool operator!=(OutputRoutingTableEntry const& other) const SYMBOL_VISIBLE;

	/**
	 * Return adresses to write to with omnibus.
	 * @param coordinate Coordinate to write to.
	 */
	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coordinate)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Encode currenct config in words and return them.
	 * @return Words describing current config.
	 */
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Decode given config and set it.
	 * @param data Config to write in words.
	 */
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, OutputRoutingTableEntry const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, OutputRoutingTableEntry& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable;
	Label m_label;
};


namespace detail {

template <>
struct BackendContainerTrait<OutputRoutingTableEntry>
    : public BackendContainerBase<OutputRoutingTableEntry, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail


/**
 * Routing table entry for incoming events.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) InputRoutingTableEntry
    : public ContainerBase<InputRoutingTableEntry>
{
public:
	typedef halco::hicann_dls::vx::InputRoutingTableEntryOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * On-Chip label for extern source.
	 */
	using Label = halco::hicann_dls::vx::SpikeLabel;

	/** Default constructor. */
	InputRoutingTableEntry() SYMBOL_VISIBLE;

	/**
	 * Get enable value.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set enable value.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * Get right label of routing table entry.
	 * @return Right label of routing table entry.
	 */
	GENPYBIND(getter_for(label))
	InputRoutingTableEntry::Label get_label() const SYMBOL_VISIBLE;

	/**
	 * Set right label of routing table entry.
	 * @param value Right label of routing table entry.
	 */
	GENPYBIND(setter_for(label))
	void set_label(InputRoutingTableEntry::Label const& value) SYMBOL_VISIBLE;

	bool operator==(InputRoutingTableEntry const& other) const SYMBOL_VISIBLE;
	bool operator!=(InputRoutingTableEntry const& other) const SYMBOL_VISIBLE;

	/**
	 * Return adresses to write to with omnibus.
	 * @param coordinate Coordinate to write to.
	 */
	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coordinate)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Encode currenct config in words and return them.
	 * @return Words describing current config.
	 */
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	/**
	 * Decode given config and set it.
	 * @param data Config to write in words.
	 */
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, InputRoutingTableEntry const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, InputRoutingTableEntry& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable;
	Label m_label;
};


namespace detail {

template <>
struct BackendContainerTrait<InputRoutingTableEntry>
    : public BackendContainerBase<InputRoutingTableEntry, fisch::vx::word_access_type::Omnibus>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
