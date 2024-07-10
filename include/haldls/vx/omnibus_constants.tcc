constexpr uint32_t ppu_top_subtree_address{0x0200'0000};
constexpr uint32_t ppu_bottom_subtree_address{0x0300'0000};
constexpr uint32_t top_ppu_base_address{0x0080'0000 | ppu_top_subtree_address};
constexpr uint32_t bottom_ppu_base_address{0x0080'0000 | ppu_bottom_subtree_address};
constexpr uint32_t ppu_control_register_address_mask{0x0020'0000};
constexpr uint32_t ppu_status_register_address_mask{0x0020'0001};
constexpr uint32_t anncore_center_base_address{0x0010'0000};

constexpr uint32_t synram_ctrl_bottom_base_address{ppu_bottom_subtree_address};
constexpr uint32_t synram_ctrl_top_base_address{ppu_top_subtree_address};
constexpr uint32_t synram_synacc_bottom_base_address{(bottom_ppu_base_address | (1 << 22))};
constexpr uint32_t synram_synacc_top_base_address{(top_ppu_base_address | (1 << 22))};

constexpr uint32_t synram_synapse_bottom_base_address{synram_synacc_bottom_base_address +
                                                      0x000f'0000};
constexpr uint32_t synram_synapse_top_base_address{synram_synacc_top_base_address + 0x000f'0000};

constexpr uint32_t synram_synapse_even_2msb_bottom_base_address{synram_synacc_bottom_base_address +
                                                                0x0001'0000};
constexpr uint32_t synram_synapse_even_2msb_top_base_address{synram_synacc_top_base_address +
                                                             0x0001'0000};
constexpr uint32_t synram_synapse_odd_2msb_bottom_base_address{synram_synacc_bottom_base_address +
                                                               0x0002'0000};
constexpr uint32_t synram_synapse_odd_2msb_top_base_address{synram_synacc_top_base_address +
                                                            0x0002'0000};

constexpr uint32_t synram_synapse_weight_bottom_base_address{synram_synacc_bottom_base_address};
constexpr uint32_t synram_synapse_weight_top_base_address{synram_synacc_top_base_address};
constexpr uint32_t synram_synapse_label_bottom_base_address{synram_synacc_bottom_base_address +
                                                            0x4'0000};
constexpr uint32_t synram_synapse_label_top_base_address{synram_synacc_top_base_address + 0x4'0000};

constexpr uint32_t synram_causal_cadc_bottom_base_address{synram_synacc_bottom_base_address +
                                                          0x0008'0000};
constexpr uint32_t synram_causal_cadc_top_base_address{synram_synacc_top_base_address +
                                                       0x0008'0000};
constexpr uint32_t synram_acausal_cadc_bottom_base_address{synram_synacc_bottom_base_address +
                                                           0x000c'0000};
constexpr uint32_t synram_acausal_cadc_top_base_address{synram_synacc_top_base_address +
                                                        0x000c'0000};

constexpr std::array<std::array<uint32_t, 2>, 2> synram_cadc_base_addresses{
    {{synram_causal_cadc_top_base_address, synram_acausal_cadc_top_base_address},
     {synram_causal_cadc_bottom_base_address, synram_acausal_cadc_bottom_base_address}}};

constexpr uint32_t cadc_buffer_enable_mask = 0x0020'0000;
constexpr uint32_t correlation_config_offset = 0x0020'0000;
constexpr std::array<uint32_t, 2> correlation_config_base_addresses = {
    ppu_top_subtree_address + correlation_config_offset,
    ppu_bottom_subtree_address + correlation_config_offset};

constexpr uint32_t phy_on_chip_base_address{0x0004'0000};
constexpr uint32_t pll_base_address{0x0008'0000};
constexpr uint32_t systime_base_address{0x0};

constexpr uint32_t capmem_nw_sram_base_address{0x140000};
constexpr uint32_t capmem_ne_sram_base_address{0x148000};
constexpr uint32_t capmem_sw_sram_base_address{0x150000};
constexpr uint32_t capmem_se_sram_base_address{0x158000};
constexpr std::array<uint32_t, 4> capmem_sram_base_addresses = {
    capmem_nw_sram_base_address, capmem_ne_sram_base_address, capmem_sw_sram_base_address,
    capmem_se_sram_base_address};

constexpr uint32_t capmem_nw_config_base_address{0x144000};
constexpr uint32_t capmem_ne_config_base_address{0x14c000};
constexpr uint32_t capmem_sw_config_base_address{0x154000};
constexpr uint32_t capmem_se_config_base_address{0x15c000};
constexpr std::array<uint32_t, 4> capmem_config_base_addresses = {
    capmem_nw_config_base_address, capmem_ne_config_base_address, capmem_sw_config_base_address,
    capmem_se_config_base_address};

constexpr uint32_t neuron_nw_sram_base_address{0x16'0000};
constexpr uint32_t neuron_ne_sram_base_address{0x16'8000};
constexpr uint32_t neuron_sw_sram_base_address{0x17'0000};
constexpr uint32_t neuron_se_sram_base_address{0x17'8000};
constexpr std::array<uint32_t, 4> neuron_sram_base_addresses = {
    neuron_nw_sram_base_address, neuron_ne_sram_base_address, neuron_sw_sram_base_address,
    neuron_se_sram_base_address};

constexpr uint32_t neuron_nw_sram_timing_base_address{neuron_nw_sram_base_address | 0x402};
constexpr uint32_t neuron_ne_sram_timing_base_address{neuron_ne_sram_base_address | 0x402};
constexpr uint32_t neuron_sw_sram_timing_base_address{neuron_sw_sram_base_address | 0x402};
constexpr uint32_t neuron_se_sram_timing_base_address{neuron_se_sram_base_address | 0x402};
constexpr std::array<uint32_t, 4> neuron_sram_timing_base_addresses = {
    neuron_nw_sram_timing_base_address, neuron_ne_sram_timing_base_address,
    neuron_sw_sram_timing_base_address, neuron_se_sram_timing_base_address};

constexpr uint32_t synapse_driver_top_sram_base_address{0x11'0400};
constexpr uint32_t synapse_driver_bottom_sram_base_address{0x11'8400};
constexpr std::array<uint32_t, 2> synapse_driver_sram_base_addresses = {
    synapse_driver_top_sram_base_address, synapse_driver_bottom_sram_base_address};

constexpr uint32_t synapse_driver_top_sram_timing_base_address{
    synapse_driver_top_sram_base_address | 0x202};
constexpr uint32_t synapse_driver_bottom_sram_timing_base_address{
    synapse_driver_bottom_sram_base_address | 0x202};
constexpr std::array<uint32_t, 2> synapse_driver_sram_timing_base_addresses = {
    synapse_driver_top_sram_timing_base_address, synapse_driver_bottom_sram_timing_base_address};

constexpr uint32_t padi_top_base_address{0x11'0000};
constexpr uint32_t padi_bottom_base_address{0x11'8000};
constexpr std::array<uint32_t, 2> padi_base_addresses = {
    padi_top_base_address, padi_bottom_base_address};

constexpr uint32_t cadc_top_base_address(ppu_top_subtree_address | 0x0040'0000);
constexpr uint32_t cadc_top_busreg_address(cadc_top_base_address | 0x0000'1000);
constexpr uint32_t cadc_top_sram_west_base_address(cadc_top_base_address | 0x0000'0800);
constexpr uint32_t cadc_top_sram_east_base_address(cadc_top_base_address);
constexpr uint32_t cadc_top_sram_timing_west_base_address(cadc_top_sram_west_base_address | 0x102);
constexpr uint32_t cadc_top_sram_timing_east_base_address(cadc_top_sram_east_base_address | 0x102);

constexpr uint32_t cadc_bottom_base_address(ppu_bottom_subtree_address | 0x0040'0000);
constexpr uint32_t cadc_bottom_busreg_address(cadc_bottom_base_address | 0x0000'1000);
constexpr uint32_t cadc_bottom_sram_west_base_address(cadc_bottom_base_address | 0x0000'0800);
constexpr uint32_t cadc_bottom_sram_east_base_address(cadc_bottom_base_address);
constexpr uint32_t cadc_bottom_sram_timing_west_base_address(
    cadc_bottom_sram_west_base_address | 0x102);
constexpr uint32_t cadc_bottom_sram_timing_east_base_address(
    cadc_bottom_sram_east_base_address | 0x102);

constexpr std::array<uint32_t, 2> cadc_busreg_addresses = {cadc_top_busreg_address,
                                                           cadc_bottom_busreg_address};

constexpr std::array<uint32_t, 4> cadc_sram_base_addresses = {
    cadc_top_sram_west_base_address, cadc_top_sram_east_base_address,
    cadc_bottom_sram_west_base_address, cadc_bottom_sram_east_base_address};

constexpr std::array<uint32_t, 4> cadc_offset_sram_timing_config_base_addresses = {
    cadc_top_sram_timing_west_base_address, cadc_top_sram_timing_east_base_address,
    cadc_bottom_sram_timing_west_base_address, cadc_bottom_sram_timing_east_base_address};

constexpr uint32_t correlation_reset_base_top(ppu_top_subtree_address | 0x00c9'0000);
constexpr uint32_t correlation_reset_base_bottom(ppu_bottom_subtree_address | 0x00c9'0000);

constexpr std::array<uint32_t, 2> correlation_reset_base_addresses = {
    correlation_reset_base_top, correlation_reset_base_bottom};

constexpr uint32_t neuron_backend_west_sram_base_address{0x1a'1000};
constexpr uint32_t neuron_backend_east_sram_base_address{0x1a'9000};
constexpr uint32_t neuron_backend_west_sram_timing_base_address{
    neuron_backend_west_sram_base_address | 0x402};
constexpr uint32_t neuron_backend_east_sram_timing_base_address{
    neuron_backend_east_sram_base_address | 0x402};

constexpr uint32_t neuron_backend_west_register_base_address{0x1a'1800};
constexpr uint32_t neuron_backend_east_register_base_address{0x1a'9800};

constexpr uint32_t neuron_reset_left_sram_base_address{0x12'0000 | 1ul << 13};
constexpr uint32_t neuron_reset_right_sram_base_address{0x12'8000 | 1ul << 13};

constexpr std::array<uint32_t, 2> neuron_backend_sram_timing_base_addresses = {
    neuron_backend_west_sram_timing_base_address, neuron_backend_east_sram_timing_base_address};

constexpr std::array<uint32_t, 2> neuron_reset_sram_base_addresses = {
    neuron_reset_left_sram_base_address, neuron_reset_right_sram_base_address};

constexpr uint32_t neuron_post_pulse_left_address{0x12'2400};
constexpr uint32_t neuron_post_pulse_right_address{0x12'a400};

constexpr std::array<uint32_t, 2> neuron_post_pulse_addresses = {neuron_post_pulse_left_address,
                                                                 neuron_post_pulse_right_address};

constexpr uint32_t spike_counter_read_left_sram_base_address{neuron_reset_left_sram_base_address |
                                                             0x800};
constexpr uint32_t spike_counter_read_right_sram_base_address{neuron_reset_right_sram_base_address |
                                                              0x800};

constexpr std::array<uint32_t, 2> spike_counter_read_sram_base_addresses = {
    spike_counter_read_left_sram_base_address, spike_counter_read_right_sram_base_address};

constexpr uint32_t spike_counter_reset_left_sram_base_address{neuron_reset_left_sram_base_address |
                                                              0xc00};
constexpr uint32_t spike_counter_reset_right_sram_base_address{
    neuron_reset_right_sram_base_address | 0xc00};

constexpr std::array<uint32_t, 2> spike_counter_reset_sram_base_addresses = {
    spike_counter_reset_left_sram_base_address, spike_counter_reset_right_sram_base_address};

// cf. omnibus_top bus splits
// split(31, num_in_flight(l2_pkg::omnibus_sync_depth))
// 	slave(asic) # same!
constexpr uint32_t fpga_omnibus_mask{0x8000'0000};
// 	split(27, num_in_flight(8))
// 		slave(ppumem)
constexpr uint32_t external_ppu_memory_base_address{0x0000'0000 | fpga_omnibus_mask};
constexpr uint32_t external_ppu_dram_memory_base_address{
    0x0400'0000 | external_ppu_memory_base_address};
// 		split(15, num_in_flight(8))
// 			split(14)
// 				split(13)
// 					slave(executor)
constexpr uint32_t executor_omnibus_mask{
    0x0800'0000 | external_ppu_memory_base_address};
constexpr uint32_t fpga_device_dna_base_address{0x3 | executor_omnibus_mask};
constexpr uint32_t event_recording_config_base_address{0x5 | executor_omnibus_mask};
constexpr uint32_t instruction_timeout_config_base_address{0x9 | executor_omnibus_mask};
constexpr uint32_t systime_correction_barrier_config{0xb | executor_omnibus_mask};
// 					slave(systime)
constexpr uint32_t systime_omnibus_mask{0x0000'2000 | executor_omnibus_mask};
constexpr uint32_t systime_num_tries_address{0x0 | systime_omnibus_mask};
constexpr uint32_t systime_last_rtt_address{0x1 | systime_omnibus_mask};
constexpr uint32_t systime_last_asic_systime_address{0x3 | systime_omnibus_mask};
constexpr uint32_t systime_active_state_address{0x5 | systime_omnibus_mask};
// 				split(13)
// 					slave(ut)
constexpr uint32_t ut_omnibus_mask{0x0000'4000 | executor_omnibus_mask};
// 					split(12)
// 						slave(phy)
constexpr uint32_t phy_omnibus_mask{0x0000'2000 | ut_omnibus_mask};
// 						split(11, num_in_flight(1))
// 							slave(spimaster)
constexpr uint32_t spimaster_omnibus_mask{0x0000'1000 | phy_omnibus_mask}; // unused for now
// 							split(10, num_in_flight(1))
// 								slave(i2cmaster)
constexpr uint32_t i2cmaster_omnibus_mask{0x0000'0800 | spimaster_omnibus_mask}; // unused for now
// 								slave(evswitch)
constexpr uint32_t evswitch_omnibus_mask{0x0000'0400 | i2cmaster_omnibus_mask};
constexpr uint32_t evswitch_to_exec_source_address{0x0 | evswitch_omnibus_mask};
constexpr uint32_t evswitch_to_asic_source_address{0x1 | evswitch_omnibus_mask};
constexpr uint32_t evswitch_to_external_source_address{0x2 | evswitch_omnibus_mask};
constexpr uint32_t evswitch_config{0x3 | evswitch_omnibus_mask};
constexpr uint32_t evswitch_systime_offset{0x4 | evswitch_omnibus_mask};
constexpr uint32_t evswitch_global_systime{0x6 | evswitch_omnibus_mask};
constexpr uint32_t evswitch_last_global_systime{0x8 | evswitch_omnibus_mask};
// 			split(14, num_in_flight(8))
// 				slave(perftest)
constexpr uint32_t perftest_omnibus_mask{
    0x0000'8000 | 0x0800'0000 | external_ppu_memory_base_address};
// 				slave(l2)
constexpr uint32_t l2_omnibus_mask{0x0000'4000 | perftest_omnibus_mask};
constexpr uint32_t hicann_arq_status_base_address{/*2 x NUM_PHY*/ 0x0000'0010 | l2_omnibus_mask};
constexpr uint32_t vector_generator_top_base_address{0x0400'0000 | fpga_omnibus_mask};
constexpr uint32_t vector_generator_bottom_base_address{0x0400'1000 | fpga_omnibus_mask};
constexpr std::array<uint32_t, 2> vector_generator_base_addresses = {
    vector_generator_top_base_address, vector_generator_bottom_base_address};

constexpr uint32_t spl1_fabric_base_address{0x0013'0000};
constexpr uint32_t crossbar_out_mux_base_address{spl1_fabric_base_address};
constexpr uint32_t background_spike_source_base_address{spl1_fabric_base_address + 1};
constexpr uint32_t crossbar_node_base_address{background_spike_source_base_address +
                                              3 /* config size */ *
                                                  8 /* # background spike sources */};
constexpr uint32_t crossbar_input_drop_counter_base_address{crossbar_node_base_address + (8 * 3) + (4 * 9) +
                                                            (8 * 2)};
constexpr uint32_t crossbar_output_event_counter_base_address{
    crossbar_input_drop_counter_base_address + 20};

constexpr uint32_t madc_base_address{0x000c'0000};

constexpr uint32_t fpga_spikeio_mask{executor_omnibus_mask | 1u << 17};
constexpr uint32_t fpga_spikeio_output_routing_base_address{fpga_spikeio_mask};
constexpr uint32_t fpga_spikeio_input_routing_base_address{fpga_spikeio_mask | 1u << 16};
constexpr uint32_t fpga_spikeio_config_base_address{fpga_spikeio_mask | 1u << 16 | 1u << 15};
