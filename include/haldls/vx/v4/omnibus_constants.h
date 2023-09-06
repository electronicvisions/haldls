#pragma once
#include <array>

namespace haldls::vx::v4 {

#include "haldls/vx/omnibus_constants.tcc"

constexpr uint32_t global_merger_matrix_base_address{pll_base_address + 0x0002'0000};
constexpr uint32_t global_merger_matrix_out_mux_base_address{global_merger_matrix_base_address};
constexpr uint32_t global_merger_matrix_node_base_address{global_merger_matrix_base_address + 1};
constexpr uint32_t neuron_label_to_gl1_index_lut_entry_base_address{
    global_merger_matrix_node_base_address + 54 * 2};
constexpr uint32_t global_merger_matrix_input_config_to_crossbar_base_address{
    neuron_label_to_gl1_index_lut_entry_base_address + 8 * 64};
constexpr uint32_t global_merger_matrix_input_drop_counter_base_address{
    global_merger_matrix_input_config_to_crossbar_base_address + 4};
constexpr uint32_t global_merger_matrix_output_event_counter_base_address{
    global_merger_matrix_input_drop_counter_base_address + 4};

} // namespace haldls::vx::v4
