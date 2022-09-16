#pragma once
#include <array>
#include <iosfwd>

#include "halco/hicann-dls/vx/timing.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

#ifndef __ppu__
#include "haldls/cerealization.h"
#endif

namespace fisch::vx {
class Omnibus;
class OmnibusChipOverJTAG;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for configuring the initial counter value of the systime counter in the chip and in the
 * FPGA after the next systime syncronization operation.
 */
class GENPYBIND(visible) SystimeSyncBase
{
public:
	typedef halco::hicann_dls::vx::SystimeSyncBaseOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint64_t, 0x7ffffffffff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	explicit SystimeSyncBase(Value const value = Value()) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(value))
	void set_value(Value const& value) SYMBOL_VISIBLE;

	bool operator==(SystimeSyncBase const& other) const SYMBOL_VISIBLE;
	bool operator!=(SystimeSyncBase const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SystimeSyncBase const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	Value m_value;
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(SystimeSyncBase)
#endif

namespace detail {

template <>
struct BackendContainerTrait<SystimeSyncBase>
    : public BackendContainerBase<
          SystimeSyncBase,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail


/**
 * Container for syncronization of chip and FPGA systime.
 * After syncronization the FPGA will annotate responses with systime information by sending
 * additional timing messages.
 */
class GENPYBIND(visible) SystimeSync
{
public:
	typedef halco::hicann_dls::vx::SystimeSyncOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	SystimeSync(bool do_sync = true) SYMBOL_VISIBLE;

	/**
	 * Get systime sync enable value.
	 * On true, systime sync is triggered, on false only a systime update response is emitted.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(do_sync))
	bool get_do_sync() const SYMBOL_VISIBLE;

	/**
	 * Set systime sync enable value.
	 * On true, systime sync is triggered, on false only a systime update response is emitted.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(do_sync))
	void set_do_sync(bool value) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SystimeSync const& systime_sync)
	    SYMBOL_VISIBLE;

	bool operator==(SystimeSync const& other) const SYMBOL_VISIBLE;
	bool operator!=(SystimeSync const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<coordinate_type, read_config_size_in_words> read_addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden) SYMBOL_VISIBLE;
	static std::array<coordinate_type, write_config_size_in_words> write_addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden) SYMBOL_VISIBLE;
	std::array<fisch::vx::word_access_type::SystimeSync, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden) SYMBOL_VISIBLE;
	void decode(
	    std::array<fisch::vx::word_access_type::SystimeSync, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	bool m_do_sync;

	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);
};

#ifndef __ppu__
EXTERN_INSTANTIATE_CEREAL_SERIALIZE(SystimeSync)
#endif

namespace detail {

template <>
struct BackendContainerTrait<SystimeSync>
    : public BackendContainerBase<SystimeSync, fisch::vx::word_access_type::SystimeSync>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::SystimeSyncBase::Value)

} // namespace std
