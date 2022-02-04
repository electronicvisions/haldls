constexpr uint64_t pulse_comm_top_rf_mask = 0x0020'0000ull;

constexpr uint64_t tx_accum_bucket_rf_offset = 0x0000ull;
constexpr uint64_t tx_accum_bucket_rf_size = 0x0020ull;

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

enum class BucketRegs {TRIGGER = 0x0, DESTINATION = 0x8, PKTS_SENT = 0x10, CNT_RES = 0x18};


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
