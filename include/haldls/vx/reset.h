#pragma once
#include <array>
#include <ostream>

#include "fisch/vx/reset.h"
#include "halco/hicann-dls/vx/reset.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Container for setting the reset pin of the chip.
 */
class GENPYBIND(visible) ResetChip
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

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::ResetChipOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::ResetChip, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::ResetChip, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar);

	fisch::vx::ResetChip m_value;
};

namespace detail {

template <>
struct BackendContainerTrait<ResetChip>
    : public BackendContainerBase<ResetChip, fisch::vx::ResetChip>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
