#include "haldls/container/v2/capmem.h"

#include <utility>

#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"

namespace haldls {
namespace container {
namespace v2 {

using halco::common::typed_array;
using namespace halco::hicann_dls::v2;

static const typed_array<uint_fast16_t, CapMemRowOnDLS> default_neuron_parameters{{
	300,  //  0: v_leak
	600,  //  1: v_treshold
	690,  //  2: v_exc_syn_input_reference
	690,  //  3: v_inh_syn_input_reference
	0,    //  4: unused
	0,    //  5: unused
	0,    //  6: unused
	0,    //  7: unused
	0,    //  8: unused
	650,  //  9: i_bias_spike_comparator
	130,  // 10: i_spike_comparator_delay
	1022, // 11: i_bias_leak_main
	1022, // 12: i_bias_leak_sd
	0,    // 13: i_bias_readout_buffer
	300,  // 14: i_refractory_time
	0,    // 15: i_bias_exc_syn_input_main
	0,    // 16: i_bias_exc_syn_input_sd
	500,  // 17: i_bias_exc_syn_input_resistor
	0,    // 18: i_bias_exc_syn_input_offset
	500,  // 19: i_bias_inh_syn_input_resistor
	0,    // 20: i_bias_inh_syn_input_main
	0,    // 21: unused
	0,    // 22: i_bias_inh_syn_input_sd
	0     // 23: i_bias_inh_syn_input_offset
}};

static const typed_array<uint_fast16_t, CapMemRowOnDLS> default_shared_neuron_parameters{{
    100, // e_reset
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0,   // unused
    0    // unused
}};

CapMemCell::CapMemCell()
	: CapMemCell(CapMemCell::Value(0))
{
}

CapMemCell::CapMemCell(CapMemCell::Value const& value) : m_value(value) {}

CapMemCell::Value CapMemCell::get_value() const
{
	return m_value;
}

void CapMemCell::set_value(CapMemCell::Value const& value)
{
	m_value = value;
}

bool CapMemCell::operator==(CapMemCell const& other) const
{
	return m_value == other.get_value();
}

bool CapMemCell::operator!=(CapMemCell const& other) const
{
	return !(*this == other);
}

CapMem::CapMem()
{
	for (auto column : halco::common::iter_all<CapMemColumnOnDLS>()) {
		for (auto row : halco::common::iter_all<CapMemRowOnDLS>()) {
			if (column.isShared()) {
				m_capmem_cells.at(CapMemCellOnDLS(column, row)) =
				    CapMemCell(CapMemCell::Value(default_shared_neuron_parameters.at(row)));
			} else {
				m_capmem_cells.at(CapMemCellOnDLS(column, row)) =
				    CapMemCell(CapMemCell::Value(default_neuron_parameters.at(row)));
			}
		}
	}
}

CapMemCell CapMem::get(CapMemCellOnDLS const& coord) const
{
	return m_capmem_cells.at(coord);
}

void CapMem::set(CapMemCellOnDLS const& coord, CapMemCell const& value)
{
	m_capmem_cells.at(coord) = value;
}

CapMemCell CapMem::get(
	NeuronOnDLS const& neuron, NeuronParameter const& neuron_parameter) const
{
	return m_capmem_cells.at(CapMemCellOnDLS(neuron, neuron_parameter));
}

void CapMem::set(
	NeuronOnDLS const& neuron,
	NeuronParameter const& neuron_parameter,
	CapMemCell const& value)
{
	m_capmem_cells.at(CapMemCellOnDLS(neuron, neuron_parameter)) = value;
}

CapMemCell CapMem::get(CommonNeuronParameter const& common_parameter) const
{
	return m_capmem_cells.at(CapMemCellOnDLS(common_parameter));
}

void CapMem::set(CommonNeuronParameter const& common_parameter, CapMemCell const& value)
{
	m_capmem_cells.at(CapMemCellOnDLS(common_parameter)) = value;
}

bool CapMem::operator==(CapMem const& other) const
{
	return m_capmem_cells == other.m_capmem_cells;
}

bool CapMem::operator!=(CapMem const& other) const
{
	return !(*this == other);
}

CommonCapMemConfig::CommonCapMemConfig()
	: m_enable_capmem(false),
	  m_debug_readout_enable(false),
	  m_debug_capmem_coord(CapMemCellOnDLS()),
	  m_debug_v_ref_select(CommonCapMemConfig::VRefSelect::disabled),
	  m_debug_i_out_select(CommonCapMemConfig::IOutSelect::disabled),
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
{
}

bool CommonCapMemConfig::get_enable_capmem() const
{
	return m_enable_capmem;
}

void CommonCapMemConfig::set_enable_capmem(bool const value)
{
	m_enable_capmem = value;
}

bool CommonCapMemConfig::get_debug_readout_enable() const
{
	return m_debug_readout_enable;
}

void CommonCapMemConfig::set_debug_readout_enable(bool const value)
{
	m_debug_readout_enable = value;
}

CapMemCellOnDLS CommonCapMemConfig::get_debug_capmem_coord() const
{
	return m_debug_capmem_coord;
}

void CommonCapMemConfig::set_debug_capmem_coord(CapMemCellOnDLS const& value)
{
	m_debug_capmem_coord = value;
}

CommonCapMemConfig::VRefSelect CommonCapMemConfig::get_debug_v_ref_select() const
{
	return m_debug_v_ref_select;
}

void CommonCapMemConfig::set_debug_v_ref_select(CommonCapMemConfig::VRefSelect const& value)
{
	m_debug_v_ref_select = value;
}

CommonCapMemConfig::IOutSelect CommonCapMemConfig::get_debug_i_out_select() const
{
	return m_debug_i_out_select;
}

void CommonCapMemConfig::set_debug_i_out_select(CommonCapMemConfig::IOutSelect const& value)
{
	m_debug_i_out_select = value;
}

CommonCapMemConfig::OutAmpBias CommonCapMemConfig::get_debug_out_amp_bias() const
{
	return m_debug_out_amp_bias;
}

void CommonCapMemConfig::set_debug_out_amp_bias(CommonCapMemConfig::OutAmpBias const& value)
{
	m_debug_out_amp_bias = value;
}

CommonCapMemConfig::SourceFollowerBias CommonCapMemConfig::get_debug_source_follower_bias() const
{
	return m_debug_source_follower_bias;
}

void CommonCapMemConfig::set_debug_source_follower_bias(CommonCapMemConfig::SourceFollowerBias const& value)
{
	m_debug_source_follower_bias = value;
}

CommonCapMemConfig::LevelShifterBias CommonCapMemConfig::get_debug_level_shifter_bias() const
{
	return m_debug_level_shifter_bias;
}

void CommonCapMemConfig::set_debug_level_shifter_bias(CommonCapMemConfig::LevelShifterBias const& value)
{
	m_debug_level_shifter_bias = value;
}

CommonCapMemConfig::VGlobalBias CommonCapMemConfig::get_v_global_bias() const
{
	return m_v_global_bias;
}

void CommonCapMemConfig::set_v_global_bias(CommonCapMemConfig::VGlobalBias const& value)
{
	m_v_global_bias = value;
}

CommonCapMemConfig::CurrentCellRes CommonCapMemConfig::get_current_cell_res() const
{
	return m_current_cell_res;
}

void CommonCapMemConfig::set_current_cell_res(CommonCapMemConfig::CurrentCellRes const& value)
{
	m_current_cell_res = value;
}

CommonCapMemConfig::BoostFactor CommonCapMemConfig::get_boost_factor() const
{
	return m_boost_factor;
}

void CommonCapMemConfig::set_boost_factor(CommonCapMemConfig::BoostFactor const& value)
{
	m_boost_factor = value;
}

bool CommonCapMemConfig::get_enable_boost() const
{
	return m_enable_boost;
}

void CommonCapMemConfig::set_enable_boost(bool const value)
{
	m_enable_boost = value;
}

bool CommonCapMemConfig::get_enable_autoboost() const
{
	return m_enable_autoboost;
}

void CommonCapMemConfig::set_enable_autoboost(bool const value)
{
	m_enable_autoboost = value;
}

CommonCapMemConfig::PrescalePause CommonCapMemConfig::get_prescale_pause() const
{
	return m_prescale_pause;
}

void CommonCapMemConfig::set_prescale_pause(CommonCapMemConfig::PrescalePause const& value)
{
	m_prescale_pause = value;
}

CommonCapMemConfig::PrescaleRamp CommonCapMemConfig::get_prescale_ramp() const
{
	return m_prescale_ramp;
}

void CommonCapMemConfig::set_prescale_ramp(CommonCapMemConfig::PrescaleRamp const& value)
{
	m_prescale_ramp = value;
}

CommonCapMemConfig::SubCounter CommonCapMemConfig::get_sub_counter() const
{
	return m_sub_counter;
}

void CommonCapMemConfig::set_sub_counter(CommonCapMemConfig::SubCounter const& value)
{
	m_sub_counter = value;
}

CommonCapMemConfig::PauseCounter CommonCapMemConfig::get_pause_counter() const
{
	return m_pause_counter;
}

void CommonCapMemConfig::set_pause_counter(CommonCapMemConfig::PauseCounter const& value)
{
	m_pause_counter = value;
}

CommonCapMemConfig::PulseA CommonCapMemConfig::get_pulse_a() const
{
	return m_pulse_a;
}

void CommonCapMemConfig::set_pulse_a(CommonCapMemConfig::PulseA const& value)
{
	m_pulse_a = value;
}

CommonCapMemConfig::PulseB CommonCapMemConfig::get_pulse_b() const
{
	return m_pulse_b;
}

void CommonCapMemConfig::set_pulse_b(CommonCapMemConfig::PulseB const& value)
{
	m_pulse_b = value;
}

CommonCapMemConfig::BoostA CommonCapMemConfig::get_boost_a() const
{
	return m_boost_a;
}

void CommonCapMemConfig::set_boost_a(CommonCapMemConfig::BoostA const& value)
{
	m_boost_a = value;
}

CommonCapMemConfig::BoostB CommonCapMemConfig::get_boost_b() const
{
	return m_boost_b;
}

void CommonCapMemConfig::set_boost_b(CommonCapMemConfig::BoostB const& value)
{
	m_boost_b = value;
}

bool CommonCapMemConfig::operator==(CommonCapMemConfig const& other) const
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

bool CommonCapMemConfig::operator!=(CommonCapMemConfig const& other) const
{
	return !(*this == other);
}

} // namespace v2
} // namespace container
} // namespace haldls
