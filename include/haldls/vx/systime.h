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
class OmnibusChip;
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
		constexpr explicit Value(uintmax_t const val = 0) SYMBOL_VISIBLE : rant_t(val) {}
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

namespace detail {

template <>
struct BackendContainerTrait<SystimeSyncBase>
    : public BackendContainerBase<
          SystimeSyncBase,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::SystimeSyncBase::Value)

} // namespace std
