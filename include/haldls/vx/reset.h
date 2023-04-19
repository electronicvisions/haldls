#pragma once
#include <array>
#include <iosfwd>

#include "halco/hicann-dls/vx/reset.h"
#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct ResetChip;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::ResetChip& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {
class ResetChip;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for setting the reset pin of the chip.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ResetChip
    : public ContainerBase<ResetChip>
{
public:
	typedef halco::hicann_dls::vx::ResetChipOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Construct chip reset with boolean value.
	 * On true, the chip reset is enabled.
	 * @param value Boolean value
	 */
	explicit ResetChip(bool value = false) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE;

	/**
	 * Set chip reset.
	 * On true, the chip reset is enabled.
	 * @param value Boolean value
	 */
	void set(bool value) SYMBOL_VISIBLE;

	bool operator==(ResetChip const& other) const SYMBOL_VISIBLE;
	bool operator!=(ResetChip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ResetChip const& config) SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::ResetChipOnDLS, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::ResetChipOnDLS, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::ResetChip, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::ResetChip, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend struct cereal::access;
	template <typename Archive>
	friend void ::cereal::serialize(Archive& ar, ResetChip& value, std::uint32_t const version);

	bool m_value;
};


namespace detail {

template <>
struct BackendContainerTrait<ResetChip>
    : public BackendContainerBase<ResetChip, fisch::vx::word_access_type::ResetChip>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
