#include <cassert>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <utility>

#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"
#include "hate/math.h"

#include "fisch/vx/jtag.h"

namespace haldls {
namespace vx {

using halco::common::typed_array;
using namespace halco::hicann_dls::vx;

CapMemCell::Value CapMemCell::get_value() const
{
	return m_value;
}

void CapMemCell::set_value(CapMemCell::Value const& value)
{
	m_value = value;
}

template <typename AddressT>
std::array<AddressT, CapMemCell::config_size_in_words> CapMemCell::addresses(
    coordinate_type const& cell) const
{
	static_assert(
	    halco::hicann_dls::vx::CapMemBlockOnDLS::size == capmem_sram_base_addresses.size(),
	    "Address base array size does not match coordinate size.");
	auto const base_address = capmem_sram_base_addresses.at(cell.toCapMemBlockOnDLS());
	auto constexpr column_stride = 32;
	return {{static_cast<AddressT>(
	    base_address +
	    cell.toCapMemCellOnCapMemBlock().toCapMemColumnOnCapMemBlock() * column_stride +
	    cell.toCapMemCellOnCapMemBlock().toCapMemRowOnCapMemBlock())}};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CapMemCell::config_size_in_words>
    CapMemCell::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& cell) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, CapMemCell::config_size_in_words>
    CapMemCell::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& cell) const;

template <typename WordT>
std::array<WordT, CapMemCell::config_size_in_words> CapMemCell::encode() const
{
	return {{static_cast<WordT>(fisch::vx::OmnibusData(get_value()))}};
}

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChipOverJTAG, CapMemCell::config_size_in_words>
CapMemCell::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, CapMemCell::config_size_in_words>
CapMemCell::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void CapMemCell::decode(std::array<WordT, CapMemCell::config_size_in_words> const& data)
{
	set_value(Value(data[0].get()));
}

template SYMBOL_VISIBLE void CapMemCell::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, CapMemCell::config_size_in_words> const& data);

template SYMBOL_VISIBLE void CapMemCell::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, CapMemCell::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, CapMemCell const& cell)
{
    using namespace hate::math;
	auto const w = static_cast<CapMemCell::Value::value_type>(cell.get_value());
	std::stringstream out;
	out << std::showbase << std::internal << std::setfill('0') << std::hex
	    << std::setw(round_up_integer_division(num_bits(CapMemCell::Value::max), 4)) << w;
	os << out.str() << std::endl;
	return print_words_for_each_backend(os, cell);
}

bool CapMemCell::operator==(CapMemCell const& other) const
{
	return m_value == other.get_value();
}

bool CapMemCell::operator!=(CapMemCell const& other) const
{
	return !(*this == other);
}

template <class Archive>
void CapMemCell::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CapMemCell)

CapMemBlock::CapMemBlock()
{
	for (auto column : halco::common::iter_all<CapMemColumnOnCapMemBlock>()) {
		for (auto row : halco::common::iter_all<CapMemRowOnCapMemBlock>()) {
			// TODO: needs to be replaced by default (shared) neuron parameters once we have them
			m_capmem_cells.at(CapMemCellOnCapMemBlock(column, row)) =
			    CapMemCell(CapMemCell::Value(0));
		}
	}
}

CapMemCell::Value CapMemBlock::get_cell(CapMemCellOnCapMemBlock const& coord) const
{
	return m_capmem_cells.at(coord).get_value();
}

void CapMemBlock::set_cell(CapMemCellOnCapMemBlock const& coord, CapMemCell::Value const& value)
{
	m_capmem_cells.at(coord).set_value(value);
}

std::ostream& operator<<(std::ostream& os, CapMemBlock const& block)
{
	for (auto it = block.m_capmem_cells.begin(); it != block.m_capmem_cells.end(); it++) {
		os << *it;
		if (not(std::next(it) == block.m_capmem_cells.end())) {
			os << std::endl;
		}
	}
	return os;
}

bool CapMemBlock::operator==(CapMemBlock const& other) const
{
	return m_capmem_cells == other.m_capmem_cells;
}

bool CapMemBlock::operator!=(CapMemBlock const& other) const
{
	return !(*this == other);
}

template <class Archive>
void CapMemBlock::cerealize(Archive& ar)
{
	ar(CEREAL_NVP(m_capmem_cells));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CapMemBlock)

} // namespace vx
} // namespace haldls
