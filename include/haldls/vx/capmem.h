#pragma once

#include <array>
#include <cstdint>
#include <ostream>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"

#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class OmnibusChip;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) CapMemCell
{
public:
	typedef halco::hicann_dls::vx::CapMemCellOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast16_t, 1023, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	CapMemCell() : CapMemCell(CapMemCell::Value(0)) {}
	SYMBOL_VISIBLE
	explicit CapMemCell(Value const& value) : m_value(value) {}
	SYMBOL_VISIBLE

	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(value))
	void set_value(Value const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& cell) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CapMemCell const& cell) SYMBOL_VISIBLE;

	bool operator==(CapMemCell const& other) const SYMBOL_VISIBLE;
	bool operator!=(CapMemCell const& other) const SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	Value m_value;
};

class GENPYBIND(visible) CapMemBlock
{
public:
	typedef halco::hicann_dls::vx::CapMemBlockOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	CapMemBlock() SYMBOL_VISIBLE;

	CapMemCell::Value get_cell(halco::hicann_dls::vx::CapMemCellOnCapMemBlock const& cell) const
	    SYMBOL_VISIBLE;
	void set_cell(
	    halco::hicann_dls::vx::CapMemCellOnCapMemBlock const& cell,
	    CapMemCell::Value const& value) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CapMemBlock const& block) SYMBOL_VISIBLE;

	bool operator==(CapMemBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(CapMemBlock const& other) const SYMBOL_VISIBLE;

private:
	friend detail::VisitPreorderImpl<CapMemBlock>;
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<CapMemCell, halco::hicann_dls::vx::CapMemCellOnCapMemBlock>
	    m_capmem_cells;
};


namespace detail {

template <>
struct BackendContainerTrait<CapMemBlock>
    : public BackendContainerBase<
          CapMemBlock,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

template <>
struct BackendContainerTrait<CapMemCell>
    : public BackendContainerBase<
          CapMemCell,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

template <>
struct VisitPreorderImpl<CapMemBlock>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config, CapMemBlock::coordinate_type const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const cell : iter_all<CapMemCellOnCapMemBlock>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			visit_preorder(config.m_capmem_cells[cell], CapMemCellOnDLS(cell, coord), visitor);
		}
	}
};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::CapMemCell::Value)

} // namespace std
