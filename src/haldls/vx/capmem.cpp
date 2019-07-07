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

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"

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
void CapMemCell::serialize(Archive& ar)
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
void CapMemBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_capmem_cells));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CapMemBlock)


CapMemBlockConfig::CapMemBlockConfig() :
    m_enable_capmem(true),
    m_debug_readout_enable(false),
    m_debug_capmem_coord(),
    m_debug_v_ref_select(CapMemBlockConfig::VRefSelect::disabled),
    m_debug_i_out_select(CapMemBlockConfig::IOutSelect::disabled),
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
    m_sub_counter(8),
    m_pause_counter(8096),
    m_pulse_a(4),
    m_pulse_b(7),
    m_boost_a(0),
    m_boost_b(0)
{}

bool CapMemBlockConfig::get_enable_capmem() const
{
	return m_enable_capmem;
}

void CapMemBlockConfig::set_enable_capmem(bool const value)
{
	m_enable_capmem = value;
}

bool CapMemBlockConfig::get_debug_readout_enable() const
{
	return m_debug_readout_enable;
}

void CapMemBlockConfig::set_debug_readout_enable(bool const value)
{
	m_debug_readout_enable = value;
}

halco::hicann_dls::vx::CapMemCellOnCapMemBlock CapMemBlockConfig::get_debug_capmem_coord() const
{
	return m_debug_capmem_coord;
}

void CapMemBlockConfig::set_debug_capmem_coord(
    halco::hicann_dls::vx::CapMemCellOnCapMemBlock const& value)
{
	m_debug_capmem_coord = value;
}

CapMemBlockConfig::VRefSelect CapMemBlockConfig::get_debug_v_ref_select() const
{
	return m_debug_v_ref_select;
}

void CapMemBlockConfig::set_debug_v_ref_select(CapMemBlockConfig::VRefSelect const& value)
{
	m_debug_v_ref_select = value;
}

CapMemBlockConfig::IOutSelect CapMemBlockConfig::get_debug_i_out_select() const
{
	return m_debug_i_out_select;
}

void CapMemBlockConfig::set_debug_i_out_select(CapMemBlockConfig::IOutSelect const& value)
{
	m_debug_i_out_select = value;
}

CapMemBlockConfig::OutAmpBias CapMemBlockConfig::get_debug_out_amp_bias() const
{
	return m_debug_out_amp_bias;
}

void CapMemBlockConfig::set_debug_out_amp_bias(CapMemBlockConfig::OutAmpBias const& value)
{
	m_debug_out_amp_bias = value;
}

CapMemBlockConfig::SourceFollowerBias CapMemBlockConfig::get_debug_source_follower_bias() const
{
	return m_debug_source_follower_bias;
}

void CapMemBlockConfig::set_debug_source_follower_bias(
    CapMemBlockConfig::SourceFollowerBias const& value)
{
	m_debug_source_follower_bias = value;
}

CapMemBlockConfig::LevelShifterBias CapMemBlockConfig::get_debug_level_shifter_bias() const
{
	return m_debug_level_shifter_bias;
}

void CapMemBlockConfig::set_debug_level_shifter_bias(
    CapMemBlockConfig::LevelShifterBias const& value)
{
	m_debug_level_shifter_bias = value;
}

CapMemBlockConfig::VGlobalBias CapMemBlockConfig::get_v_global_bias() const
{
	return m_v_global_bias;
}

void CapMemBlockConfig::set_v_global_bias(CapMemBlockConfig::VGlobalBias const& value)
{
	m_v_global_bias = value;
}

CapMemBlockConfig::CurrentCellRes CapMemBlockConfig::get_current_cell_res() const
{
	return m_current_cell_res;
}

void CapMemBlockConfig::set_current_cell_res(CapMemBlockConfig::CurrentCellRes const& value)
{
	m_current_cell_res = value;
}

CapMemBlockConfig::BoostFactor CapMemBlockConfig::get_boost_factor() const
{
	return m_boost_factor;
}

void CapMemBlockConfig::set_boost_factor(CapMemBlockConfig::BoostFactor const& value)
{
	m_boost_factor = value;
}

bool CapMemBlockConfig::get_enable_boost() const
{
	return m_enable_boost;
}

void CapMemBlockConfig::set_enable_boost(bool const value)
{
	m_enable_boost = value;
}

bool CapMemBlockConfig::get_enable_autoboost() const
{
	return m_enable_autoboost;
}

void CapMemBlockConfig::set_enable_autoboost(bool const value)
{
	m_enable_autoboost = value;
}

CapMemBlockConfig::PrescalePause CapMemBlockConfig::get_prescale_pause() const
{
	return m_prescale_pause;
}

void CapMemBlockConfig::set_prescale_pause(CapMemBlockConfig::PrescalePause const& value)
{
	m_prescale_pause = value;
}

CapMemBlockConfig::PrescaleRamp CapMemBlockConfig::get_prescale_ramp() const
{
	return m_prescale_ramp;
}

void CapMemBlockConfig::set_prescale_ramp(CapMemBlockConfig::PrescaleRamp const& value)
{
	m_prescale_ramp = value;
}

CapMemBlockConfig::SubCounter CapMemBlockConfig::get_sub_counter() const
{
	return m_sub_counter;
}

void CapMemBlockConfig::set_sub_counter(CapMemBlockConfig::SubCounter const& value)
{
	m_sub_counter = value;
}

CapMemBlockConfig::PauseCounter CapMemBlockConfig::get_pause_counter() const
{
	return m_pause_counter;
}

void CapMemBlockConfig::set_pause_counter(CapMemBlockConfig::PauseCounter const& value)
{
	m_pause_counter = value;
}

CapMemBlockConfig::PulseA CapMemBlockConfig::get_pulse_a() const
{
	return m_pulse_a;
}

void CapMemBlockConfig::set_pulse_a(CapMemBlockConfig::PulseA const& value)
{
	m_pulse_a = value;
}

CapMemBlockConfig::PulseB CapMemBlockConfig::get_pulse_b() const
{
	return m_pulse_b;
}

void CapMemBlockConfig::set_pulse_b(CapMemBlockConfig::PulseB const& value)
{
	m_pulse_b = value;
}

CapMemBlockConfig::BoostA CapMemBlockConfig::get_boost_a() const
{
	return m_boost_a;
}

void CapMemBlockConfig::set_boost_a(CapMemBlockConfig::BoostA const& value)
{
	m_boost_a = value;
}

CapMemBlockConfig::BoostB CapMemBlockConfig::get_boost_b() const
{
	return m_boost_b;
}

void CapMemBlockConfig::set_boost_b(CapMemBlockConfig::BoostB const& value)
{
	m_boost_b = value;
}

bool CapMemBlockConfig::operator==(CapMemBlockConfig const& other) const
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

bool CapMemBlockConfig::operator!=(CapMemBlockConfig const& other) const
{
	return !(*this == other);
}

namespace {

struct CapMemBlockConfigBitfield
{
	typedef std::array<uint32_t, CapMemBlockConfig::config_size_in_words> array_type;

	union
	{
		array_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t hotbit_capmem_row          : 32;

			uint32_t capmem_column              : 32;

			uint32_t debug_v_ref_select         : 32;

			uint32_t v_global_bias              :  4;
			uint32_t debug_level_shifter_bias   :  4;
			uint32_t debug_source_follower_bias :  4;
			uint32_t debug_out_amp_bias         :  4;
			uint32_t                            : 16;

			uint32_t current_cell_res           : 32;

			uint32_t debug_i_out_select         : 32;

			uint32_t sub_counter                : 16;
			uint32_t enable_capmem              :  1;
			uint32_t enable_boost               :  1;
			uint32_t enable_autoboost           :  1;
			uint32_t                            :  1;
			uint32_t prescale_ramp              :  4;
			uint32_t prescale_pause             :  4;
			uint32_t boost_factor               :  4;

			uint32_t pulse_b                    : 16;
			uint32_t pulse_a                    : 16;

			uint32_t boost_b                    : 16;
			uint32_t boost_a                    : 16;

			uint32_t pause_counter              : 32;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CapMemBlockConfigBitfield() { u.raw = {{0}}; }
	CapMemBlockConfigBitfield(array_type const& data) { u.raw = data; }
};

} // namespace

template <typename AddressT>
std::array<AddressT, CapMemBlockConfig::config_size_in_words> CapMemBlockConfig::addresses(
    coordinate_type const& coord) const
{
	uint32_t const base_address = capmem_config_base_addresses.at(coord.value());
	std::array<AddressT, CapMemBlockConfig::config_size_in_words> result;
	size_t i = 0;
	std::generate(result.begin(), result.end(), [&i, base_address]() {
		return AddressT(base_address + i++);
	});
	return result;
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CapMemBlockConfig::config_size_in_words>
CapMemBlockConfig::addresses(coordinate_type const& coord) const;
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, CapMemBlockConfig::config_size_in_words>
    CapMemBlockConfig::addresses(coordinate_type const& coord) const;

template <typename WordT>
std::array<WordT, CapMemBlockConfig::config_size_in_words> CapMemBlockConfig::encode() const
{
	CapMemBlockConfigBitfield bitfield;
	if (m_debug_readout_enable) {
		auto capmem_coord = m_debug_capmem_coord;
		bitfield.u.m.hotbit_capmem_row = 1u
		                                 << (halco::hicann_dls::vx::CapMemRowOnCapMemBlock::max -
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
		return WordT(fisch::vx::OmnibusData(m));
	});

	return ret;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, CapMemBlockConfig::config_size_in_words>
    CapMemBlockConfig::encode() const;
template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, CapMemBlockConfig::config_size_in_words>
CapMemBlockConfig::encode() const;

template <typename WordT>
void CapMemBlockConfig::decode(
    std::array<WordT, CapMemBlockConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, config_size_in_words> words;
	std::transform(data.cbegin(), data.cend(), words.begin(), [](auto m) { return m.get(); });

	CapMemBlockConfigBitfield bitfield(words);
	if (bitfield.u.m.hotbit_capmem_row) {
		m_debug_readout_enable = true;
		uint32_t hotbit = bitfield.u.m.hotbit_capmem_row;
		size_t position = 0;
		assert(hotbit != 0);
		while (hotbit >>= 1)
			++position;
		m_debug_capmem_coord = CapMemCellOnCapMemBlock(
		    CapMemColumnOnCapMemBlock(bitfield.u.m.capmem_column),
		    CapMemRowOnCapMemBlock(CapMemRowOnCapMemBlock::max - position));
	} else {
		m_debug_readout_enable = false;
		m_debug_capmem_coord = CapMemCellOnDLS();
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

template SYMBOL_VISIBLE void CapMemBlockConfig::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, CapMemBlockConfig::config_size_in_words> const&
        data);
template SYMBOL_VISIBLE void CapMemBlockConfig::decode(
    std::array<fisch::vx::OmnibusChip, CapMemBlockConfig::config_size_in_words> const& data);

template <class Archive>
void CapMemBlockConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_capmem));
	ar(CEREAL_NVP(m_debug_readout_enable));
	ar(CEREAL_NVP(m_debug_capmem_coord));
	ar(CEREAL_NVP(m_debug_v_ref_select));
	ar(CEREAL_NVP(m_debug_i_out_select));
	ar(CEREAL_NVP(m_debug_out_amp_bias));
	ar(CEREAL_NVP(m_debug_source_follower_bias));
	ar(CEREAL_NVP(m_debug_level_shifter_bias));
	ar(CEREAL_NVP(m_v_global_bias));
	ar(CEREAL_NVP(m_current_cell_res));
	ar(CEREAL_NVP(m_enable_boost));
	ar(CEREAL_NVP(m_boost_factor));
	ar(CEREAL_NVP(m_enable_autoboost));
	ar(CEREAL_NVP(m_prescale_pause));
	ar(CEREAL_NVP(m_prescale_ramp));
	ar(CEREAL_NVP(m_sub_counter));
	ar(CEREAL_NVP(m_pause_counter));
	ar(CEREAL_NVP(m_pulse_a));
	ar(CEREAL_NVP(m_pulse_b));
	ar(CEREAL_NVP(m_boost_a));
	ar(CEREAL_NVP(m_boost_b));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CapMemBlockConfig)


} // namespace vx
} // namespace haldls
