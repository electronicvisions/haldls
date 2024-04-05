#include "haldls/vx/capmem.h"

#include <cassert>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <utility>

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"
#include "hate/math.h"

namespace haldls {
namespace vx {

using halco::common::typed_array;

template <typename Coordinates>
typename CapMemCell<Coordinates>::value_type CapMemCell<Coordinates>::get_value() const
{
	return m_value;
}

template <typename Coordinates>
void CapMemCell<Coordinates>::set_value(value_type const& value)
{
	m_value = value;
}

template <typename Coordinates>
template <typename AddressT>
std::array<AddressT, CapMemCell<Coordinates>::config_size_in_words>
CapMemCell<Coordinates>::addresses(coordinate_type const& cell)
{
	static_assert(
	    Coordinates::CapMemBlockOnDLS::size == capmem_sram_base_addresses.size(),
	    "Address base array size does not match coordinate size.");
	auto const base_address = capmem_sram_base_addresses.at(cell.toCapMemBlockOnDLS());
	auto constexpr column_stride = 32;
	return {{static_cast<AddressT>(
	    base_address +
	    cell.toCapMemCellOnCapMemBlock().toCapMemColumnOnCapMemBlock() * column_stride +
	    cell.toCapMemCellOnCapMemBlock().toCapMemRowOnCapMemBlock())}};
}

template <typename Coordinates>
template <typename WordT>
std::array<WordT, CapMemCell<Coordinates>::config_size_in_words> CapMemCell<Coordinates>::encode()
    const
{
	if (auto const ptr = std::get_if<DisableRefresh>(&m_value)) {
		return {WordT(*ptr)};
	} else {
		return {WordT(std::get<Value>(m_value))};
	}
}

template <typename Coordinates>
template <typename WordT>
void CapMemCell<Coordinates>::decode(
    std::array<WordT, CapMemCell<Coordinates>::config_size_in_words> const& data)
{
	auto value = data[0] & DisableRefresh::max;
	if (value == DisableRefresh()) {
		m_value = DisableRefresh();
	} else {
		m_value = Value(value);
	}
}

template <typename Coordinates>
bool CapMemCell<Coordinates>::operator==(CapMemCell<Coordinates> const& other) const
{
	return m_value == other.get_value();
}

template <typename Coordinates>
bool CapMemCell<Coordinates>::operator!=(CapMemCell<Coordinates> const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
#endif

#define CAPMEM_CELL_UNROLL_PPU(Coordinates)                                                        \
	template class CapMemCell<Coordinates>;                                                        \
                                                                                                   \
	std::ostream& operator<<(std::ostream& os, CapMemCell<Coordinates>::value_type const& value)   \
	{                                                                                              \
		std::visit([&](auto const& v) { os << v; }, value);                                        \
		return os;                                                                                 \
	}                                                                                              \
                                                                                                   \
	template SYMBOL_VISIBLE std::array<                                                            \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    CapMemCell<Coordinates>::config_size_in_words>                                             \
	CapMemCell<Coordinates>::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(         \
	    coordinate_type const& cell);                                                              \
                                                                                                   \
	template SYMBOL_VISIBLE std::array<                                                            \
	    halco::hicann_dls::vx::OmnibusAddress, CapMemCell<Coordinates>::config_size_in_words>      \
	CapMemCell<Coordinates>::addresses<halco::hicann_dls::vx::OmnibusAddress>(                     \
	    coordinate_type const& cell);                                                              \
                                                                                                   \
	template SYMBOL_VISIBLE std::array<                                                            \
	    fisch::vx::word_access_type::OmnibusChipOverJTAG,                                          \
	    CapMemCell<Coordinates>::config_size_in_words>                                             \
	CapMemCell<Coordinates>::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;     \
                                                                                                   \
	template SYMBOL_VISIBLE std::array<                                                            \
	    fisch::vx::word_access_type::Omnibus, CapMemCell<Coordinates>::config_size_in_words>       \
	CapMemCell<Coordinates>::encode<fisch::vx::word_access_type::Omnibus>() const;                 \
                                                                                                   \
	template SYMBOL_VISIBLE void                                                                   \
	CapMemCell<Coordinates>::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(             \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::OmnibusChipOverJTAG,                                      \
	        CapMemCell<Coordinates>::config_size_in_words> const& data);                           \
                                                                                                   \
	template SYMBOL_VISIBLE void                                                                   \
	CapMemCell<Coordinates>::decode<fisch::vx::word_access_type::Omnibus>(                         \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::Omnibus,                                                  \
	        CapMemCell<Coordinates>::config_size_in_words> const& data);

#ifndef __ppu__
#define CAPMEM_CELL_UNROLL(Coordinates) CAPMEM_CELL_UNROLL_PPU(Coordinates)
#endif

template <typename Coordinates>
CapMemBlock<Coordinates>::CapMemBlock()
{
	for (auto column : halco::common::iter_all<typename Coordinates::CapMemColumnOnCapMemBlock>()) {
		for (auto row : halco::common::iter_all<typename Coordinates::CapMemRowOnCapMemBlock>()) {
			// TODO: needs to be replaced by default (shared) neuron parameters once we have them
			m_capmem_cells.at(typename Coordinates::CapMemCellOnCapMemBlock(column, row)) =
			    CapMemCell<Coordinates>(typename CapMemCell<Coordinates>::Value(0));
		}
	}
}

template <typename Coordinates>
typename CapMemCell<Coordinates>::value_type CapMemBlock<Coordinates>::get_cell(
    typename Coordinates::CapMemCellOnCapMemBlock const& coord) const
{
	return m_capmem_cells.at(coord).get_value();
}

template <typename Coordinates>
void CapMemBlock<Coordinates>::set_cell(
    typename Coordinates::CapMemCellOnCapMemBlock const& coord,
    typename CapMemCell<Coordinates>::value_type const& value)
{
	m_capmem_cells.at(coord).set_value(value);
}

template <typename Coordinates>
bool CapMemBlock<Coordinates>::operator==(CapMemBlock<Coordinates> const& other) const
{
	return m_capmem_cells == other.m_capmem_cells;
}

template <typename Coordinates>
bool CapMemBlock<Coordinates>::operator!=(CapMemBlock<Coordinates> const& other) const
{
	return !(*this == other);
}

template <typename Coordinates>
std::ostream& operator<<(std::ostream& os, CapMemBlock<Coordinates> const& block)
{
	os << hate::join(block.m_capmem_cells, "\n") << std::flush;
	return os;
}

#define CAPMEM_BLOCK_UNROLL_PPU(Coordinates) template class CapMemBlock<Coordinates>;

#ifndef __ppu__
#define CAPMEM_BLOCK_UNROLL(Coordinates) CAPMEM_BLOCK_UNROLL_PPU(Coordinates)
#endif

template <typename Coordinates>
CapMemBlockConfig<Coordinates>::CapMemBlockConfig() :
    m_enable_capmem(true),
    m_debug_readout_enable(false),
    m_debug_capmem_coord(),
    m_debug_v_ref_select(VRefSelect::disabled),
    m_debug_i_out_select(IOutSelect::disabled),
    m_debug_out_amp_bias(7),
    m_debug_source_follower_bias(3),
    m_debug_level_shifter_bias(3),
    m_v_global_bias(7),
    m_current_cell_res(8),
    m_enable_boost(false),
    m_boost_factor(0),
    m_enable_autoboost(false),
    m_prescale_pause(3),
    m_prescale_ramp(3),
    m_sub_counter(16),
    m_pause_counter(8096),
    m_pulse_a(4),
    m_pulse_b(7),
    m_boost_a(0),
    m_boost_b(0)
{}

template <typename Coordinates>
bool CapMemBlockConfig<Coordinates>::get_enable_capmem() const
{
	return m_enable_capmem;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_enable_capmem(bool const value)
{
	m_enable_capmem = value;
}

template <typename Coordinates>
bool CapMemBlockConfig<Coordinates>::get_debug_readout_enable() const
{
	return m_debug_readout_enable;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_debug_readout_enable(bool const value)
{
	m_debug_readout_enable = value;
}

template <typename Coordinates>
typename Coordinates::CapMemCellOnCapMemBlock
CapMemBlockConfig<Coordinates>::get_debug_capmem_coord() const
{
	return m_debug_capmem_coord;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_debug_capmem_coord(
    typename Coordinates::CapMemCellOnCapMemBlock const& value)
{
	m_debug_capmem_coord = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::VRefSelect
CapMemBlockConfig<Coordinates>::get_debug_v_ref_select() const
{
	return m_debug_v_ref_select;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_debug_v_ref_select(VRefSelect const& value)
{
	m_debug_v_ref_select = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::IOutSelect
CapMemBlockConfig<Coordinates>::get_debug_i_out_select() const
{
	return m_debug_i_out_select;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_debug_i_out_select(IOutSelect const& value)
{
	m_debug_i_out_select = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::OutAmpBias
CapMemBlockConfig<Coordinates>::get_debug_out_amp_bias() const
{
	return m_debug_out_amp_bias;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_debug_out_amp_bias(OutAmpBias const& value)
{
	m_debug_out_amp_bias = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::SourceFollowerBias
CapMemBlockConfig<Coordinates>::get_debug_source_follower_bias() const
{
	return m_debug_source_follower_bias;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_debug_source_follower_bias(SourceFollowerBias const& value)
{
	m_debug_source_follower_bias = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::LevelShifterBias
CapMemBlockConfig<Coordinates>::get_debug_level_shifter_bias() const
{
	return m_debug_level_shifter_bias;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_debug_level_shifter_bias(LevelShifterBias const& value)
{
	m_debug_level_shifter_bias = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::VGlobalBias
CapMemBlockConfig<Coordinates>::get_v_global_bias() const
{
	return m_v_global_bias;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_v_global_bias(VGlobalBias const& value)
{
	m_v_global_bias = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::CurrentCellRes
CapMemBlockConfig<Coordinates>::get_current_cell_res() const
{
	return m_current_cell_res;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_current_cell_res(CurrentCellRes const& value)
{
	m_current_cell_res = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::BoostFactor
CapMemBlockConfig<Coordinates>::get_boost_factor() const
{
	return m_boost_factor;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_boost_factor(BoostFactor const& value)
{
	m_boost_factor = value;
}

template <typename Coordinates>
bool CapMemBlockConfig<Coordinates>::get_enable_boost() const
{
	return m_enable_boost;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_enable_boost(bool const value)
{
	m_enable_boost = value;
}

template <typename Coordinates>
bool CapMemBlockConfig<Coordinates>::get_enable_autoboost() const
{
	return m_enable_autoboost;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_enable_autoboost(bool const value)
{
	m_enable_autoboost = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::PrescalePause
CapMemBlockConfig<Coordinates>::get_prescale_pause() const
{
	return m_prescale_pause;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_prescale_pause(PrescalePause const& value)
{
	m_prescale_pause = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::PrescaleRamp
CapMemBlockConfig<Coordinates>::get_prescale_ramp() const
{
	return m_prescale_ramp;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_prescale_ramp(PrescaleRamp const& value)
{
	m_prescale_ramp = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::SubCounter
CapMemBlockConfig<Coordinates>::get_sub_counter() const
{
	return m_sub_counter;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_sub_counter(SubCounter const& value)
{
	m_sub_counter = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::PauseCounter
CapMemBlockConfig<Coordinates>::get_pause_counter() const
{
	return m_pause_counter;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_pause_counter(PauseCounter const& value)
{
	m_pause_counter = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::PulseA CapMemBlockConfig<Coordinates>::get_pulse_a() const
{
	return m_pulse_a;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_pulse_a(PulseA const& value)
{
	m_pulse_a = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::PulseB CapMemBlockConfig<Coordinates>::get_pulse_b() const
{
	return m_pulse_b;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_pulse_b(PulseB const& value)
{
	m_pulse_b = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::BoostA CapMemBlockConfig<Coordinates>::get_boost_a() const
{
	return m_boost_a;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_boost_a(BoostA const& value)
{
	m_boost_a = value;
}

template <typename Coordinates>
typename CapMemBlockConfig<Coordinates>::BoostB CapMemBlockConfig<Coordinates>::get_boost_b() const
{
	return m_boost_b;
}

template <typename Coordinates>
void CapMemBlockConfig<Coordinates>::set_boost_b(BoostB const& value)
{
	m_boost_b = value;
}

template <typename Coordinates>
bool CapMemBlockConfig<Coordinates>::operator==(CapMemBlockConfig<Coordinates> const& other) const
{
	// clang-format off
	return (
		m_enable_capmem == other.get_enable_capmem() &&
		m_debug_readout_enable == other.get_debug_readout_enable() &&
		m_debug_capmem_coord == other.get_debug_capmem_coord() &&
		m_debug_v_ref_select == other.get_debug_v_ref_select() &&
		m_debug_i_out_select == other.get_debug_i_out_select() &&
		m_debug_out_amp_bias == other.get_debug_out_amp_bias() &&
		m_debug_source_follower_bias == other.get_debug_source_follower_bias() &&
		m_debug_level_shifter_bias == other.get_debug_level_shifter_bias() &&
		m_v_global_bias == other.get_v_global_bias() &&
		m_current_cell_res == other.get_current_cell_res() &&
		m_boost_factor == other.get_boost_factor() &&
		m_enable_boost == other.get_enable_boost() &&
		m_enable_autoboost == other.get_enable_autoboost() &&
		m_prescale_pause == other.get_prescale_pause() &&
		m_prescale_ramp == other.get_prescale_ramp() &&
		m_sub_counter == other.get_sub_counter() &&
		m_pause_counter == other.get_pause_counter() &&
		m_pulse_a == other.get_pulse_a() &&
		m_pulse_b == other.get_pulse_b() &&
		m_boost_a == other.get_boost_a() &&
		m_boost_b == other.get_boost_b());
	// clang-format on
}

template <typename Coordinates>
bool CapMemBlockConfig<Coordinates>::operator!=(CapMemBlockConfig<Coordinates> const& other) const
{
	return !(*this == other);
}


namespace {

template <typename Coordinates>
struct CapMemBlockConfigBitfield
{
	typedef std::array<uint32_t, CapMemBlockConfig<Coordinates>::config_size_in_words> array_type;

	union
	{
		array_type raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t hotbit_capmem_row          : 24;) \
			(uint32_t /* unused */               :  8;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t capmem_column              : 32;) \
			                                            \
			(uint32_t debug_v_ref_select         :  2;) \
			(uint32_t /* unused */               : 30;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t v_global_bias              :  4;) \
			(uint32_t debug_level_shifter_bias   :  4;) \
			(uint32_t debug_source_follower_bias :  4;) \
			(uint32_t debug_out_amp_bias         :  4;) \
			(uint32_t                            : 16;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t current_cell_res           :  6;) \
			(uint32_t /* unused */               : 26;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t debug_i_out_select         :  2;) \
			(uint32_t /* unused */               : 30;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t sub_counter                : 16;) \
			(uint32_t enable_capmem              :  1;) \
			(uint32_t enable_boost               :  1;) \
			(uint32_t enable_autoboost           :  1;) \
			(uint32_t                            :  1;) \
			(uint32_t prescale_ramp              :  4;) \
			(uint32_t prescale_pause             :  4;) \
			(uint32_t boost_factor               :  4;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t pulse_b                    : 16;) \
			(uint32_t pulse_a                    : 16;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t boost_b                    : 16;) \
			(uint32_t boost_a                    : 16;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t pause_counter              : 32;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CapMemBlockConfigBitfield()
	{
		u.raw = {{0}};
	}
	CapMemBlockConfigBitfield(array_type const& data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename Coordinates>
template <typename AddressT>
std::array<AddressT, CapMemBlockConfig<Coordinates>::config_size_in_words>
CapMemBlockConfig<Coordinates>::addresses(coordinate_type const& coord)
{
	uint32_t const base_address = capmem_config_base_addresses.at(coord.value());
	std::array<AddressT, CapMemBlockConfig<Coordinates>::config_size_in_words> result;
	size_t i = 0;
	std::generate(result.begin(), result.end(), [&i, base_address]() {
		return AddressT(base_address + i++);
	});
	return result;
}

template <typename Coordinates>
template <typename WordT>
std::array<WordT, CapMemBlockConfig<Coordinates>::config_size_in_words>
CapMemBlockConfig<Coordinates>::encode() const
{
	CapMemBlockConfigBitfield<Coordinates> bitfield;
	if (m_debug_readout_enable) {
		auto capmem_coord = m_debug_capmem_coord;
		bitfield.u.m.hotbit_capmem_row = 1u
		                                 << (Coordinates::CapMemRowOnCapMemBlock::max -
		                                     capmem_coord.toCapMemRowOnCapMemBlock());
		bitfield.u.m.capmem_column = capmem_coord.toCapMemColumnOnCapMemBlock();
	} else {
		bitfield.u.m.hotbit_capmem_row = 0;
		bitfield.u.m.capmem_column = 0;
	}
	bitfield.u.m.debug_v_ref_select = static_cast<uint32_t>(m_debug_v_ref_select);
	bitfield.u.m.debug_out_amp_bias = m_debug_out_amp_bias.value();
	bitfield.u.m.debug_source_follower_bias = m_debug_source_follower_bias.value();
	bitfield.u.m.debug_level_shifter_bias = m_debug_level_shifter_bias.value();
	bitfield.u.m.v_global_bias = m_v_global_bias.value();
	bitfield.u.m.current_cell_res = m_current_cell_res.value();
	bitfield.u.m.debug_i_out_select = static_cast<uint32_t>(m_debug_i_out_select);
	bitfield.u.m.boost_factor = m_boost_factor.value();
	bitfield.u.m.prescale_pause = m_prescale_pause.value();
	bitfield.u.m.prescale_ramp = m_prescale_ramp.value();
	bitfield.u.m.enable_autoboost = m_enable_autoboost;
	bitfield.u.m.enable_boost = m_enable_boost;
	bitfield.u.m.enable_capmem = m_enable_capmem;
	bitfield.u.m.sub_counter = m_sub_counter.value();
	bitfield.u.m.pulse_a = m_pulse_a.value();
	bitfield.u.m.pulse_b = m_pulse_b.value();
	bitfield.u.m.boost_a = m_boost_a.value();
	bitfield.u.m.boost_b = m_boost_b.value();
	bitfield.u.m.pause_counter = m_pause_counter.value();

	std::array<WordT, config_size_in_words> ret;
	std::transform(bitfield.u.raw.cbegin(), bitfield.u.raw.cend(), ret.begin(), [](auto m) {
		return WordT(m);
	});

	return ret;
}

template <typename Coordinates>
template <typename WordT>
void CapMemBlockConfig<Coordinates>::decode(
    std::array<WordT, CapMemBlockConfig<Coordinates>::config_size_in_words> const& data)
{
	std::array<uint32_t, config_size_in_words> words;
	std::transform(data.cbegin(), data.cend(), words.begin(), [](auto m) { return m; });

	CapMemBlockConfigBitfield<Coordinates> bitfield(words);
	if (bitfield.u.m.hotbit_capmem_row) {
		m_debug_readout_enable = true;
		uint32_t hotbit = bitfield.u.m.hotbit_capmem_row;
		size_t position = 0;
		assert(hotbit != 0);
		while (hotbit >>= 1)
			++position;
		m_debug_capmem_coord = typename Coordinates::CapMemCellOnCapMemBlock(
		    typename Coordinates::CapMemColumnOnCapMemBlock(bitfield.u.m.capmem_column),
		    typename Coordinates::CapMemRowOnCapMemBlock(
		        Coordinates::CapMemRowOnCapMemBlock::max - position));
	} else {
		m_debug_readout_enable = false;
		m_debug_capmem_coord = typename Coordinates::CapMemCellOnCapMemBlock();
	}
	m_debug_v_ref_select = VRefSelect(bitfield.u.m.debug_v_ref_select);
	m_debug_out_amp_bias = OutAmpBias(bitfield.u.m.debug_out_amp_bias);
	m_debug_source_follower_bias = SourceFollowerBias(bitfield.u.m.debug_source_follower_bias);
	m_debug_level_shifter_bias = LevelShifterBias(bitfield.u.m.debug_level_shifter_bias);
	m_v_global_bias = VGlobalBias(bitfield.u.m.v_global_bias);
	m_current_cell_res = CurrentCellRes(bitfield.u.m.current_cell_res);
	m_debug_i_out_select = IOutSelect(bitfield.u.m.debug_i_out_select);
	m_boost_factor = BoostFactor(bitfield.u.m.boost_factor);
	m_prescale_pause = PrescalePause(bitfield.u.m.prescale_pause);
	m_prescale_ramp = PrescaleRamp(bitfield.u.m.prescale_ramp);
	m_enable_autoboost = bitfield.u.m.enable_autoboost;
	m_enable_boost = bitfield.u.m.enable_boost;
	m_enable_capmem = bitfield.u.m.enable_capmem;
	m_sub_counter = SubCounter(bitfield.u.m.sub_counter);
	m_pulse_a = PulseA(bitfield.u.m.pulse_a);
	m_pulse_b = PulseB(bitfield.u.m.pulse_b);
	m_boost_a = BoostA(bitfield.u.m.boost_a);
	m_boost_b = BoostB(bitfield.u.m.boost_b);
	m_pause_counter = PauseCounter(bitfield.u.m.pause_counter);
}

std::ostream& operator<<(std::ostream& os, CapMemBlockConfigVRefSelect const& config)
{
	switch (config) {
		case CapMemBlockConfigVRefSelect::disabled: {
			os << "disabled";
			break;
		}
		case CapMemBlockConfigVRefSelect::v_ref_v: {
			os << "v_ref_v";
			break;
		}
		case CapMemBlockConfigVRefSelect::v_ref_i: {
			os << "v_ref_i";
			break;
		}
		default: {
#ifndef __ppu__
			throw std::logic_error("Unknown v_ref_select.");
#else
			exit(1);
#endif
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, CapMemBlockConfigIOutSelect const& config)
{
	switch (config) {
		case CapMemBlockConfigIOutSelect::disabled: {
			os << "disabled";
			break;
		}
		case CapMemBlockConfigIOutSelect::i_out_mux: {
			os << "i_out_mux";
			break;
		}
		case CapMemBlockConfigIOutSelect::i_out_ramp: {
			os << "i_out_ramp";
			break;
		}
		default: {
#ifndef __ppu__
			throw std::logic_error("Unknown i_out_select.");
#else
			exit(1);
#endif
		}
	}
	return os;
}

#define CAPMEM_BLOCK_CONFIG_UNROLL_PPU(Coordinates)                                                \
	template class CapMemBlockConfig<Coordinates>;                                                 \
	template SYMBOL_VISIBLE std::array<                                                            \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    CapMemBlockConfig<Coordinates>::config_size_in_words>                                      \
	CapMemBlockConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
	template SYMBOL_VISIBLE std::array<                                                            \
	    halco::hicann_dls::vx::OmnibusAddress,                                                     \
	    CapMemBlockConfig<Coordinates>::config_size_in_words>                                      \
	CapMemBlockConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
                                                                                                   \
	template SYMBOL_VISIBLE std::array<                                                            \
	    fisch::vx::word_access_type::OmnibusChipOverJTAG,                                          \
	    CapMemBlockConfig<Coordinates>::config_size_in_words>                                      \
	CapMemBlockConfig<Coordinates>::encode() const;                                                \
	template SYMBOL_VISIBLE std::array<                                                            \
	    fisch::vx::word_access_type::Omnibus,                                                      \
	    CapMemBlockConfig<Coordinates>::config_size_in_words>                                      \
	CapMemBlockConfig<Coordinates>::encode() const;                                                \
                                                                                                   \
	template SYMBOL_VISIBLE void CapMemBlockConfig<Coordinates>::decode(                           \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::OmnibusChipOverJTAG,                                      \
	        CapMemBlockConfig<Coordinates>::config_size_in_words> const& data);                    \
	template SYMBOL_VISIBLE void CapMemBlockConfig<Coordinates>::decode(                           \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::Omnibus,                                                  \
	        CapMemBlockConfig<Coordinates>::config_size_in_words> const& data);

#ifndef __ppu
#define CAPMEM_BLOCK_CONFIG_UNROLL(Coordinates) CAPMEM_BLOCK_CONFIG_UNROLL_PPU(Coordinates)
#endif

} // namespace vx
} // namespace haldls
