#include "haldls/container/v2/board.h"

#include "halco/common/iter_all.h"

using namespace halco::hicann_dls::v2;
using namespace halco::common;

namespace haldls {
namespace container {
namespace v2 {

namespace {

constexpr std::size_t number_of_parameters = 15;

const std::array<std::pair<DACOnBoard, DAC::Channel>, number_of_parameters> dac_channel_lookup{{
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(1)}, // cadc_ramp_01
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(0)}, // cadc_ramp_bias
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(2)}, // cadc_ramp_slope
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(3)}, // cadc_v_bias
	{DACOnBoard::DAC_25_DECIVOLT, DAC::Channel(6)}, // capmem_i_buf_bias
	{DACOnBoard::DAC_25_DECIVOLT, DAC::Channel(1)}, // capmem_i_offset
	{DACOnBoard::DAC_25_DECIVOLT, DAC::Channel(7)}, // capmem_i_ref
	{DACOnBoard::DAC_25_DECIVOLT, DAC::Channel(2)}, // general_purpose_0
	{DACOnBoard::DAC_25_DECIVOLT, DAC::Channel(3)}, // general_purpose_1
	{DACOnBoard::DAC_25_DECIVOLT, DAC::Channel(5)}, // syn_corout_bias
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(7)}, // syn_v_bias
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(4)}, // syn_v_dd_res_meas
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(6)}, // syn_v_ramp
	{DACOnBoard::DAC_25_DECIVOLT, DAC::Channel(4)}, // syn_v_reset
	{DACOnBoard::DAC_12_DECIVOLT, DAC::Channel(5)}  // syn_v_store
}};

const std::array<DAC::Value, number_of_parameters> dac_default_values{{
	/* cadc_ramp_01      */ DAC::Value(1365), // sensible default (stable across boards)
	/* cadc_ramp_bias    */ DAC::Value(0),    // specific to board (sensitive)
	/* cadc_ramp_slope   */ DAC::Value(0),    // (even more sensitive)
	/* cadc_v_bias       */ DAC::Value(2389), // sensible default (stable across boards)
	/* capmem_i_buf_bias */ DAC::Value(3000), // sensible default (stable across boards)
	/* capmem_i_offset   */ DAC::Value(0),    // sensible default (stable across boards)
	/* capmem_i_ref      */ DAC::Value(3906), // sensible default (better: tune for each board)
	/* general_purpose_0 */ DAC::Value(0),
	/* general_purpose_1 */ DAC::Value(0),
	/* syn_corout_bias   */ DAC::Value(573),  // sensible default (stable across boards)
	/* syn_v_bias        */ DAC::Value(1500), // ?
	/* syn_v_dd_res_meas */ DAC::Value(4095), // sensible default (stable across boards)
	/* syn_v_ramp        */ DAC::Value(4095), // STDP time constant -> min
	/* syn_v_reset       */ DAC::Value(4095), // reset of corr. accum. trace to zero
	/* syn_v_store       */ DAC::Value(4095)  // STDP amplitude -> 0
}};

} // namespace

FPGAConfig::FPGAConfig()
	: m_reset_to_dls(true), // note that this effects a partial reset of the chip configuration
	  m_soft_reset(false),
	  m_tg_ctrl(0),
	  m_enable_spike_router(false),
	  m_i_phase_sel(false),
	  m_o_phase_sel(false),
	  m_train(false),
	  m_txrx_en(true),
	  m_en_lvds_rx(true),
	  m_analog_power_en(true),
	  m_loopback_to_dls(false)
{}

void FPGAConfig::set_enable_spike_router(bool const value) {
	m_enable_spike_router = value;
}

bool FPGAConfig::get_enable_spike_router() const {
	return m_enable_spike_router;
}

bool FPGAConfig::operator==(FPGAConfig const& /*other*/) const
{
	return true;
}

bool FPGAConfig::operator!=(FPGAConfig const& other) const
{
	return !(*this == other);
}

auto FPGAConfig::addresses(coordinate_type const& /*unique*/) const
	-> std::array<ocp_address_type, config_size_in_words>
{
	static std::uint32_t constexpr base_address = 0x8000;
	return {{base_address + 0x20}};
}

namespace {

struct FPGAConfigBitfield {
	union {
		ocp_word_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type loopback_to_dls                :  1; //  0
			hardware_word_type analog_power_en                :  1; //  1
			hardware_word_type en_lvds_rx                     :  1; //  2
			hardware_word_type txrx_en                        :  1; //  3
			hardware_word_type train                          :  1; //  4
			hardware_word_type o_phase_sel                    :  1; //  5
			hardware_word_type i_phase_sel                    :  1; //  6
			hardware_word_type enable_spike_router            :  1; //  7
			hardware_word_type tg_ctrl                        :  6; //  8
			hardware_word_type                                : 16; // 14-29
			hardware_word_type soft_reset                     :  1; // 30
			hardware_word_type reset_to_dls                   :  1; // 31
		} m;
		// clang-format on
		static_assert(
			sizeof(raw) == sizeof(m) && sizeof(raw) == sizeof(std::uint32_t),
			"sizes of union types should match");
	} u;

	FPGAConfigBitfield() {
		u.raw = {0u};
	}

	FPGAConfigBitfield(hardware_word_type data) {
		u.raw = {data & 0xC0003FFFul};
	}
};

} // namespace

auto FPGAConfig::encode() const -> std::array<ocp_word_type, config_size_in_words>
{
	FPGAConfigBitfield bitfield;

	bitfield.u.m.loopback_to_dls = m_loopback_to_dls;
	bitfield.u.m.analog_power_en = m_analog_power_en;
	bitfield.u.m.en_lvds_rx = m_en_lvds_rx;
	bitfield.u.m.txrx_en = m_txrx_en;
	bitfield.u.m.train = m_train;
	bitfield.u.m.o_phase_sel = m_o_phase_sel;
	bitfield.u.m.i_phase_sel = m_i_phase_sel;
	bitfield.u.m.enable_spike_router = m_enable_spike_router;
	bitfield.u.m.tg_ctrl = m_tg_ctrl.to_ulong();
	bitfield.u.m.soft_reset = m_soft_reset;
	bitfield.u.m.reset_to_dls = m_reset_to_dls;

	return {{bitfield.u.raw}};
}

SpikeRouter::SpikeRouter()
	: m_squeeze_mode_enabled(false),
	  m_squeeze_mode_address(),
	  m_squeeze_mode_delay(),
	  m_address_by_neuron(),
	  m_target_rows_by_neuron()
{}

void SpikeRouter::reset()
{
	SynapseBlock::Synapse::Address const zero{0};
	m_squeeze_mode_enabled = false;
	m_squeeze_mode_address = zero;
	m_squeeze_mode_delay = Delay(0);
	for (auto neuron : iter_all<NeuronOnDLS>()) {
		m_address_by_neuron[neuron] = zero;
		m_target_rows_by_neuron[neuron].reset();
	}
}

void SpikeRouter::enable_squeeze_mode(
	SynapseBlock::Synapse::Address const& address, Delay const& delay)
{
	reset();
	m_squeeze_mode_enabled = true;
	m_squeeze_mode_address = address;
	m_squeeze_mode_delay = delay;
}

void SpikeRouter::set_neuron_route(
	halco::hicann_dls::v2::NeuronOnDLS const& neuron,
	SynapseBlock::Synapse::Address const& address,
	target_rows_type const& target_rows)
{
	if (m_squeeze_mode_enabled)
		throw std::runtime_error("cannot set neuron routes in squeeze mode");

	m_address_by_neuron[neuron] = address;
	m_target_rows_by_neuron[neuron] = target_rows;
}

bool SpikeRouter::operator==(SpikeRouter const& other) const
{
	return (
		m_squeeze_mode_enabled == other.m_squeeze_mode_enabled &&
		m_squeeze_mode_address == other.m_squeeze_mode_address &&
		m_squeeze_mode_delay == other.m_squeeze_mode_delay &&
		m_address_by_neuron == other.m_address_by_neuron &&
		m_target_rows_by_neuron == other.m_target_rows_by_neuron);
}

bool SpikeRouter::operator!=(SpikeRouter const& other) const
{
	return !(*this == other);
}

auto SpikeRouter::addresses(coordinate_type const& /*unique*/) const
	-> std::array<ocp_address_type, config_size_in_words>
{
	static std::uint32_t constexpr base_address = 0x8000;
	static std::uint32_t constexpr control_address = base_address + 0x30;
	static std::uint32_t constexpr data_address = base_address + 0x31;

	// We need to alternate between data and control register for every row...
	std::array<ocp_address_type, config_size_in_words> result;
	for (size_t ii = 0; ii < config_size_in_words / 2; ++ii) {
		result[2 * ii].value = data_address;
		result[2 * ii + 1].value = control_address;
	}

	// ...and set the control register to the appropriate final values.
	result[config_size_in_words - 1].value = control_address;
	return result;
}

auto SpikeRouter::encode() const
	-> std::array<ocp_word_type, config_size_in_words>
{
	std::array<ocp_word_type, config_size_in_words> result;

	for (auto neuron : iter_all<NeuronOnDLS>()) {
		size_t const ii = neuron;
		std::bitset<32> data = m_target_rows_by_neuron.at(neuron);
		std::bitset<32> control;
		// Alternatively:
		//     std::bitset<32> control = control_final;
		//     control.reset(11); // disable squeeze/bypass mode
		control |= static_cast<std::uint32_t>(neuron) << 6; // write address
		control |= static_cast<std::uint32_t>(m_address_by_neuron[neuron]);

		result[2 * ii].value = data.to_ulong();
		result[2 * ii + 1].value = control.to_ulong();
	}

	std::bitset<32> control_final;
	control_final |= static_cast<std::uint32_t>(m_squeeze_mode_delay) << 18;
	control_final |= static_cast<std::uint32_t>(m_squeeze_mode_address) << 12;
	control_final.set(11, m_squeeze_mode_enabled);

	result[config_size_in_words - 1].value = control_final.to_ulong();
	return result;
}

Board::Board() : m_fpga(), m_spike_router(), m_dacs() {
	for (size_t ii = 0; ii < number_of_parameters; ++ii) {
		set_parameter(Parameter(ii), dac_default_values[ii]);
	}
}

void Board::set_parameter(Parameter const& parameter, DAC::Value const& value)
{
	DACOnBoard dac;
	DAC::Channel channel;
	std::tie(dac, channel) = dac_channel_lookup[static_cast<std::uint_fast16_t>(parameter)];
	m_dacs[dac].set(channel, value);
}

DAC::Value Board::get_parameter(Parameter const& parameter) const
{
	DACOnBoard dac;
	DAC::Channel channel;
	std::tie(dac, channel) = dac_channel_lookup[static_cast<std::uint_fast16_t>(parameter)];
	return m_dacs[dac].get(channel);
}

FPGAConfig Board::get_fpga_config() const
{
	return m_fpga;
}

void Board::set_fpga_config(FPGAConfig const& config)
{
	m_fpga = config;
}

SpikeRouter Board::get_spike_router() const
{
	return m_spike_router;
}
void Board::set_spike_router(SpikeRouter const& config)
{
	m_spike_router = config;
}

bool Board::operator==(Board const& other) const
{
	// clang-format off
	return (
		m_fpga == other.m_fpga &&
		m_spike_router == other.m_spike_router &&
		m_dacs == other.m_dacs);
	// clang-format on
}

bool Board::operator!=(Board const& other) const
{
	return !(*this == other);
}

} // namespace v2
} // namespace container
} // namespace haldls
