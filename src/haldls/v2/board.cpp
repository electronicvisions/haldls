#include "haldls/v2/board.h"

#include "halco/common/cerealization_typed_array.h"
#include "halco/common/iter_all.h"
#include "haldls/cerealization.h"

using namespace halco::hicann_dls::v2;
using namespace halco::common;

namespace haldls {
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

Board::Board() : m_flyspi_config(), m_flyspi_exception(), m_spike_router(), m_dacs()
{
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

FlyspiConfig const& Board::get_flyspi_config() const
{
	return m_flyspi_config;
}

void Board::set_flyspi_config(FlyspiConfig const& config)
{
	m_flyspi_config = config;
}

SpikeRouter const& Board::get_spike_router() const
{
	return m_spike_router;
}
void Board::set_spike_router(SpikeRouter const& config)
{
	m_spike_router = config;
}

FlyspiException Board::get_flyspi_exception() const
{
	return m_flyspi_exception;
}

bool Board::operator==(Board const& other) const
{
	// clang-format off
	return (
		m_flyspi_config == other.m_flyspi_config &&
		m_flyspi_exception == other.m_flyspi_exception &&
		m_spike_router == other.m_spike_router &&
		m_dacs == other.m_dacs);
	// clang-format on
}

bool Board::operator!=(Board const& other) const
{
	return !(*this == other);
}

template <class Archive>
void Board::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_flyspi_config));
	ar(CEREAL_NVP(m_flyspi_exception));
	ar(CEREAL_NVP(m_spike_router));
	ar(CEREAL_NVP(m_dacs));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(Board)

} // namespace v2
} // namespace haldls
