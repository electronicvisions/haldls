constexpr uint64_t pulse_comm_top_rf_mask = 0x0020'0000ull;

constexpr uint64_t tx_accum_bucket_rf_offset = 0x0000ull;
constexpr uint64_t tx_accum_bucket_rf_size = 0x0040ull;

constexpr uint64_t tx_accum_bucket_rf_base_addr{pulse_comm_top_rf_mask +
    tx_accum_bucket_rf_offset};

constexpr uint64_t tx_accum_bucket_rf_0{tx_accum_bucket_rf_base_addr +
    0 * tx_accum_bucket_rf_size};

constexpr uint64_t tx_accum_bucket_rf_1{tx_accum_bucket_rf_base_addr +
    1 * tx_accum_bucket_rf_size};

constexpr uint64_t tx_accum_bucket_rf_2{tx_accum_bucket_rf_base_addr +
    2 * tx_accum_bucket_rf_size};

constexpr uint64_t tx_accum_bucket_rf_3{tx_accum_bucket_rf_base_addr +
    3 * tx_accum_bucket_rf_size};

constexpr uint64_t tx_accum_bucket_rf_4{tx_accum_bucket_rf_base_addr +
    4 * tx_accum_bucket_rf_size};

constexpr uint64_t tx_accum_bucket_rf_5{tx_accum_bucket_rf_base_addr +
    5 * tx_accum_bucket_rf_size};

constexpr uint64_t tx_accum_bucket_rf_6{tx_accum_bucket_rf_base_addr +
    6 * tx_accum_bucket_rf_size};

constexpr uint64_t tx_accum_bucket_rf_7{tx_accum_bucket_rf_base_addr +
    7 * tx_accum_bucket_rf_size};

constexpr std::array<uint64_t, 8> tx_accum_bucket_rf_addresses = {
    tx_accum_bucket_rf_0, tx_accum_bucket_rf_1, tx_accum_bucket_rf_2, tx_accum_bucket_rf_3,
    tx_accum_bucket_rf_4, tx_accum_bucket_rf_5, tx_accum_bucket_rf_6, tx_accum_bucket_rf_7};

enum class BucketRegs {
    TRIGGER = 0x0,
    DESTINATION = 0x8,
    PKTS_SENT = 0x10,
    EVTS_RCVD_0 = 0x18,
    EVTS_RCVD_1 = 0x20,
    CNT_RES = 0x28
};


constexpr uint64_t tx_accum_router_rf_offset = 0x0004'0000ull;
constexpr uint64_t tx_accum_router_rf_size = 0x0004'0000ull;

constexpr uint64_t tx_accum_router_rf_addr{pulse_comm_top_rf_mask +
    tx_accum_router_rf_offset};

constexpr uint64_t tx_accum_router_rf_0{tx_accum_router_rf_addr +
    0 * tx_accum_router_rf_size};

constexpr uint64_t tx_accum_router_rf_1{tx_accum_router_rf_addr +
    1 * tx_accum_router_rf_size};

constexpr std::array<uint64_t, 2> tx_accum_router_rf_addresses = {
    tx_accum_router_rf_0, tx_accum_router_rf_1};

enum class RouterRegs {SEND_LOOKUP = 0x0, CONFIG = 0x2'0000, EVT_LOSS_DISABLED = 0x2'0008,
                       EVT_LOSS_MISCONF = 0x2'0010, EVTS_ROUTED = 0x2'0018, CNT_RES = 0x2'0020};


constexpr uint64_t rx_decoder_rf_offset = 0x10'0000ull;
constexpr uint64_t rx_decoder_rf_size = 0x0040ull;

constexpr uint64_t rx_decoder_rf_addr{pulse_comm_top_rf_mask +
    rx_decoder_rf_offset};

enum DecoderRegs {EVTS_RCVD_0 = 0x0, EVTS_RCVD_1 = 0x8, CNT_RES = 0x10};

constexpr std::array<uint64_t, 2> rx_decoder_rf_evts_rcvd_addresses = {
    DecoderRegs::EVTS_RCVD_0, DecoderRegs::EVTS_RCVD_1};


constexpr uint64_t rx_timestamp_delay_buffer_rf_offset = 0x10'0040ull;
constexpr uint64_t rx_timestamp_delay_buffer_rf_size = 0x0040ull;

constexpr uint64_t rx_timestamp_delay_buffer_rf_addr{pulse_comm_top_rf_mask +
    rx_timestamp_delay_buffer_rf_offset};

constexpr uint64_t rx_timestamp_delay_buffer_rf_0{rx_timestamp_delay_buffer_rf_addr +
    0 * rx_timestamp_delay_buffer_rf_size};

constexpr uint64_t rx_timestamp_delay_buffer_rf_1{rx_timestamp_delay_buffer_rf_addr +
    1 * rx_timestamp_delay_buffer_rf_size};

constexpr std::array<uint64_t, 2> rx_timestamp_delay_buffer_rf_addresses = {
    rx_timestamp_delay_buffer_rf_0, rx_timestamp_delay_buffer_rf_1};

enum class TimestampDelayRegs {CONFIG = 0x0, EVTS_RCVD = 0x8, EVT_LOSS_FULL = 0x10,
                               EVT_LOSS_EXPIRED = 0x18, CNT_RES = 0x20};


constexpr uint64_t fpga_barrier_top_rf_mask = 0x6'0000ull;
constexpr uint64_t tourmalet_barrier_top_rf_mask = 0x44'0000ull;

constexpr uint64_t barrier_control_rf_offset = 0x0000ull;
constexpr uint64_t barrier_control_rf_size = 0x1000ull;

constexpr uint64_t barrier_control_rf_base_offset{barrier_control_rf_offset};

constexpr uint64_t barrier_control_rf_0{barrier_control_rf_base_offset +
    0 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_1{barrier_control_rf_base_offset +
    1 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_2{barrier_control_rf_base_offset +
    2 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_3{barrier_control_rf_base_offset +
    3 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_4{barrier_control_rf_base_offset +
    4 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_5{barrier_control_rf_base_offset +
    5 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_6{barrier_control_rf_base_offset +
    6 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_7{barrier_control_rf_base_offset +
    7 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_8{barrier_control_rf_base_offset +
    8 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_9{barrier_control_rf_base_offset +
    9 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_10{barrier_control_rf_base_offset +
    10 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_11{barrier_control_rf_base_offset +
    11 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_12{barrier_control_rf_base_offset +
    12 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_13{barrier_control_rf_base_offset +
    13 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_14{barrier_control_rf_base_offset +
    14 * barrier_control_rf_size};

constexpr uint64_t barrier_control_rf_15{barrier_control_rf_base_offset +
    15 * barrier_control_rf_size};

constexpr std::array<uint64_t, 16> barrier_control_rf_offsets = {
    barrier_control_rf_0, barrier_control_rf_1, barrier_control_rf_2, barrier_control_rf_3,
    barrier_control_rf_4, barrier_control_rf_5, barrier_control_rf_6, barrier_control_rf_7,
    barrier_control_rf_8, barrier_control_rf_9, barrier_control_rf_10, barrier_control_rf_11,
    barrier_control_rf_12, barrier_control_rf_13, barrier_control_rf_14, barrier_control_rf_15};

enum class BarrierControlRegs {CONTROL = 0x0};


constexpr uint64_t interrupt_control_rf_offset = 0x1'0000ull;
constexpr uint64_t interrupt_control_rf_size = 0x1000ull;

constexpr uint64_t interrupt_control_rf_base_offset{interrupt_control_rf_offset};

constexpr uint64_t interrupt_control_rf_0{interrupt_control_rf_base_offset +
    0 * interrupt_control_rf_size};

constexpr uint64_t interrupt_control_rf_1{interrupt_control_rf_base_offset +
    1 * interrupt_control_rf_size};

constexpr uint64_t interrupt_control_rf_2{interrupt_control_rf_base_offset +
    2 * interrupt_control_rf_size};

constexpr uint64_t interrupt_control_rf_3{interrupt_control_rf_base_offset +
    3 * interrupt_control_rf_size};

constexpr std::array<uint64_t, 4> interrupt_control_rf_offsets = {
    interrupt_control_rf_0, interrupt_control_rf_1, interrupt_control_rf_2, interrupt_control_rf_3};

enum class InterruptControlRegs {CONTROL = 0x0};


constexpr uint64_t barrier_config_rf_offset = 0x1'4000ull;
constexpr uint64_t barrier_config_rf_size = 0x0008ull;

constexpr uint64_t barrier_config_rf_base_offset{barrier_config_rf_offset};

constexpr uint64_t barrier_config_rf_0{barrier_config_rf_base_offset +
    0 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_1{barrier_config_rf_base_offset +
    1 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_2{barrier_config_rf_base_offset +
    2 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_3{barrier_config_rf_base_offset +
    3 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_4{barrier_config_rf_base_offset +
    4 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_5{barrier_config_rf_base_offset +
    5 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_6{barrier_config_rf_base_offset +
    6 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_7{barrier_config_rf_base_offset +
    7 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_8{barrier_config_rf_base_offset +
    8 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_9{barrier_config_rf_base_offset +
    9 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_10{barrier_config_rf_base_offset +
    10 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_11{barrier_config_rf_base_offset +
    11 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_12{barrier_config_rf_base_offset +
    12 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_13{barrier_config_rf_base_offset +
    13 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_14{barrier_config_rf_base_offset +
    14 * barrier_config_rf_size};

constexpr uint64_t barrier_config_rf_15{barrier_config_rf_base_offset +
    15 * barrier_config_rf_size};

constexpr std::array<uint64_t, 16> barrier_config_rf_offsets = {
    barrier_config_rf_0, barrier_config_rf_1, barrier_config_rf_2, barrier_config_rf_3,
    barrier_config_rf_4, barrier_config_rf_5, barrier_config_rf_6, barrier_config_rf_7,
    barrier_config_rf_8, barrier_config_rf_9, barrier_config_rf_10, barrier_config_rf_11,
    barrier_config_rf_12, barrier_config_rf_13, barrier_config_rf_14, barrier_config_rf_15};

enum class BarrierConfigRegs {CONFIG = 0x0};


constexpr uint64_t interrupt_config_rf_offset = 0x1'4080ull;
constexpr uint64_t interrupt_config_rf_size = 0x0008ull;

constexpr uint64_t interrupt_config_rf_base_offset{interrupt_config_rf_offset};

constexpr uint64_t interrupt_config_rf_0{interrupt_config_rf_base_offset +
    0 * interrupt_config_rf_size};

constexpr uint64_t interrupt_config_rf_1{interrupt_config_rf_base_offset +
    1 * interrupt_config_rf_size};

constexpr uint64_t interrupt_config_rf_2{interrupt_config_rf_base_offset +
    2 * interrupt_config_rf_size};

constexpr uint64_t interrupt_config_rf_3{interrupt_config_rf_base_offset +
    3 * interrupt_config_rf_size};

constexpr std::array<uint64_t, 4> interrupt_config_rf_offsets = {
    interrupt_config_rf_0, interrupt_config_rf_1, interrupt_config_rf_2, interrupt_config_rf_3};

enum class InterruptConfigRegs {CONFIG = 0x0};


constexpr uint64_t barrier_interrupt_inport_rf_offset = 0x1'4100ull;

enum class BarrierInterruptInportRegs {ERROR_CNT = 0x0, CNT_RES = 0x700};
