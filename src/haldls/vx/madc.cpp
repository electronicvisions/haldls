#include "haldls/vx/madc.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/constants.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"

#ifndef __ppu__
#include <log4cxx/logger.h>
#endif

namespace haldls {
namespace vx {


MADCControl::MADCControl() :
    m_enable_iconv_amplifier(false),
    m_enable_pre_amplifier(false),
    m_enable_continuous_sampling(false),
    m_enable_power_down_after_sampling(false),
    m_stop_recording(false),
    m_start_recording(false),
    m_wake_up(false)
{}

bool MADCControl::get_enable_iconv_amplifier() const
{
	return m_enable_iconv_amplifier;
}

void MADCControl::set_enable_iconv_amplifier(bool const value)
{
	m_enable_iconv_amplifier = value;
}

bool MADCControl::get_enable_pre_amplifier() const
{
	return m_enable_pre_amplifier;
}

void MADCControl::set_enable_pre_amplifier(bool const value)
{
	m_enable_pre_amplifier = value;
}

bool MADCControl::get_enable_continuous_sampling() const
{
	return m_enable_continuous_sampling;
}

void MADCControl::set_enable_continuous_sampling(bool const value)
{
	m_enable_continuous_sampling = value;
}

bool MADCControl::get_enable_power_down_after_sampling() const
{
	return m_enable_power_down_after_sampling;
}

void MADCControl::set_enable_power_down_after_sampling(bool const value)
{
	m_enable_power_down_after_sampling = value;
}

bool MADCControl::get_stop_recording() const
{
	return m_stop_recording;
}

void MADCControl::set_stop_recording(bool const value)
{
	m_stop_recording = value;
}

bool MADCControl::get_start_recording() const
{
	return m_start_recording;
}

void MADCControl::set_start_recording(bool const value)
{
	m_start_recording = value;
}

bool MADCControl::get_wake_up() const
{
	return m_wake_up;
}

void MADCControl::set_wake_up(bool const value)
{
	m_wake_up = value;
}

template <typename AddressT>
std::array<AddressT, MADCControl::write_config_size_in_words> SYMBOL_VISIBLE
MADCControl::write_addresses(coordinate_type const& /* coord */)
{
	return {AddressT(madc_base_address)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    MADCControl::write_config_size_in_words>
MADCControl::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, MADCControl::write_config_size_in_words>
    MADCControl::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);


template <typename AddressT>
std::array<AddressT, MADCControl::read_config_size_in_words> MADCControl::read_addresses(
    coordinate_type const& /* coord */)
{
	return {};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, MADCControl::read_config_size_in_words>
    MADCControl::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, MADCControl::read_config_size_in_words>
    MADCControl::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

namespace {

struct MADCControlBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t wake_up                           :  1; /*       0 */ ) \
			(uint32_t start_recording                   :  1; /*       1 */ ) \
			(uint32_t stop_recording                    :  1; /*       2 */ ) \
			(uint32_t enable_power_down_after_sampling  :  1; /*       3 */ ) \
			(uint32_t enable_continuous_sampling        :  1; /*       4 */ ) \
			(uint32_t enable_pre_amplifier              :  1; /*       5 */ ) \
			(uint32_t enable_iconv_amplifier            :  1; /*       6 */ ) \
			(uint32_t                                   : 25; /* 31 -  7 */ )
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "Sizes of union types should match.");
	} u;

	MADCControlBitfield() { u.raw = 0u; }

	MADCControlBitfield(uint32_t data) { u.raw = data; }
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, MADCControl::write_config_size_in_words> MADCControl::encode() const
{
	MADCControlBitfield bitfield;

	bitfield.u.m.enable_iconv_amplifier = m_enable_iconv_amplifier;
	bitfield.u.m.enable_pre_amplifier = m_enable_pre_amplifier;
	bitfield.u.m.enable_continuous_sampling = m_enable_continuous_sampling;
	bitfield.u.m.enable_power_down_after_sampling = m_enable_power_down_after_sampling;
	bitfield.u.m.stop_recording = m_stop_recording;
	bitfield.u.m.start_recording = m_start_recording;
	bitfield.u.m.wake_up = m_wake_up;

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, MADCControl::write_config_size_in_words>
    MADCControl::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, MADCControl::write_config_size_in_words>
    MADCControl::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void MADCControl::decode(
    std::array<WordT, MADCControl::read_config_size_in_words> const& /* data */)
{}

template SYMBOL_VISIBLE void MADCControl::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        MADCControl::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void MADCControl::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, MADCControl::read_config_size_in_words> const&
        data);

std::ostream& operator<<(std::ostream& os, MADCControl const& config)
{
	std::stringstream ss;
	ss << "MADCControl(\n";
	// clang-format off
	ss << std::boolalpha
	   << "\tenable_iconv_amplifier:           \t" << config.m_enable_iconv_amplifier << "\n"
	   << "\tenable_pre_amplifier:             \t" << config.m_enable_pre_amplifier << "\n"
	   << "\tenable_continuous_sampling:       \t" << config.m_enable_continuous_sampling << "\n"
	   << "\tenable_power_down_after_sampling: \t" << config.m_enable_power_down_after_sampling << "\n"
	   << "\tstop_recording:                   \t" << config.m_stop_recording << "\n"
	   << "\tstart_recording:                  \t" << config.m_start_recording << "\n"
	   << "\twake_up:                          \t" << config.m_wake_up << "\n)";
	// clang-format on
	return (os << ss.str());
}

bool MADCControl::operator==(MADCControl const& other) const
{
	return (
	    m_enable_iconv_amplifier == other.get_enable_iconv_amplifier() &&
	    m_enable_pre_amplifier == other.get_enable_pre_amplifier() &&
	    m_enable_continuous_sampling == other.get_enable_continuous_sampling() &&
	    m_enable_power_down_after_sampling == other.get_enable_power_down_after_sampling() &&
	    m_stop_recording == other.get_stop_recording() &&
	    m_start_recording == other.get_start_recording() && m_wake_up == other.get_wake_up());
}

bool MADCControl::operator!=(MADCControl const& other) const
{
	return !(*this == other);
}


MADCConfig::MADCConfig() :
    m_active_mux_initially_selected_input(
        halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection()),
    m_active_mux_input_select_length(ActiveMuxInputSelectLength()),
    m_sample_duration_adjust(SampleDurationAdjust()),
    m_enable_sar_reset_on_fall(false),
    m_sar_reset_wait(SARResetWait()),
    m_sar_reset_length(SARResetLength()),
    m_powerup_wait_value(PowerupWaitValue()),
    m_conversion_cycles_offset(ConversionCyclesOffset()),
    m_calibration_wait_value(CalibrationWaitValue()),
    m_enable_calibration(false),
    m_number_of_samples(NumberOfSamples()),
    m_preamp_sampling_window_start(SamplingWindowTiming(3)),
    m_preamp_sampling_window_end(SamplingWindowTiming(8)),
    m_iconv_sampling_window_start(SamplingWindowTiming(9)),
    m_iconv_sampling_window_end(SamplingWindowTiming(14)),
    m_sample_on_positive_edge(true),
    m_enable_dummy_data(false),
    m_connect_preamp_to_madc(true),
    m_connect_pads_to_madc(false),
    m_connect_preamp_to_pads(false),
    m_preamp_gain_capacitor_size(PreampGainCapacitorSize()),
    m_enable_madc_clock_scaling(false),
    m_madc_clock_scale_value(MADCClockScaleValue()),
    m_enable_active_mux_amplifiers(true),
    m_enable_pseudo_differential_reference(hemisphere_type()),
    m_signal_selection_connect_current_meter(false),
    m_signal_selection_connect_active_mux(true),
    m_signal_selection_connect_debug(false),
    m_signal_selection_connect_preamp(true),
    m_enable_iconv_amplifier(false),
    m_connect_iconv_neuron(hemisphere_type()),
    m_connect_iconv_synapse(synapse_target_type())
{}

MADCConfig::input_selection_type MADCConfig::MADCConfig::get_active_mux_initially_selected_input()
    const
{
	return m_active_mux_initially_selected_input;
}

void MADCConfig::set_active_mux_initially_selected_input(input_selection_type const& value)
{
	m_active_mux_initially_selected_input = value;
}

MADCConfig::ActiveMuxInputSelectLength MADCConfig::get_active_mux_input_select_length() const
{
	return m_active_mux_input_select_length;
}

void MADCConfig::set_active_mux_input_select_length(ActiveMuxInputSelectLength const& value)
{
	m_active_mux_input_select_length = value;
}

MADCConfig::SampleDurationAdjust MADCConfig::get_sample_duration_adjust() const
{
	return m_sample_duration_adjust;
}

void MADCConfig::set_sample_duration_adjust(SampleDurationAdjust const& value)
{
	m_sample_duration_adjust = value;
}

bool MADCConfig::get_enable_sar_reset_on_fall() const
{
	return m_enable_sar_reset_on_fall;
}

void MADCConfig::set_enable_sar_reset_on_fall(bool const value)
{
	m_enable_sar_reset_on_fall = value;
}

MADCConfig::SARResetWait MADCConfig::get_sar_reset_wait() const
{
	return m_sar_reset_wait;
}

void MADCConfig::set_sar_reset_wait(SARResetWait const& value)
{
	m_sar_reset_wait = value;
}

MADCConfig::SARResetLength MADCConfig::get_sar_reset_length() const
{
	return m_sar_reset_length;
}

void MADCConfig::set_sar_reset_length(SARResetLength const& value)
{
	m_sar_reset_length = value;
}

MADCConfig::PowerupWaitValue MADCConfig::get_powerup_wait_value() const
{
	return m_powerup_wait_value;
}

void MADCConfig::set_powerup_wait_value(PowerupWaitValue const& value)
{
	m_powerup_wait_value = value;
}

MADCConfig::ConversionCyclesOffset MADCConfig::get_conversion_cycles_offset() const
{
	return m_conversion_cycles_offset;
}

void MADCConfig::set_conversion_cycles_offset(ConversionCyclesOffset const& value)
{
	m_conversion_cycles_offset = value;
}

MADCConfig::CalibrationWaitValue MADCConfig::get_calibration_wait_value() const
{
	return m_calibration_wait_value;
}

void MADCConfig::set_calibration_wait_value(CalibrationWaitValue const& value)
{
	m_calibration_wait_value = value;
}

bool MADCConfig::get_enable_calibration() const
{
	return m_enable_calibration;
}

void MADCConfig::set_enable_calibration(bool const value)
{
	m_enable_calibration = value;
}

MADCConfig::NumberOfSamples MADCConfig::get_number_of_samples() const
{
	return m_number_of_samples;
}

void MADCConfig::set_number_of_samples(NumberOfSamples const& value)
{
	m_number_of_samples = value;
}

MADCConfig::SamplingWindowTiming MADCConfig::get_preamp_sampling_window_start() const
{
	return m_preamp_sampling_window_start;
}

void MADCConfig::set_preamp_sampling_window_start(SamplingWindowTiming const& value)
{
	m_preamp_sampling_window_start = value;
}

MADCConfig::SamplingWindowTiming MADCConfig::get_preamp_sampling_window_end() const
{
	return m_preamp_sampling_window_end;
}

void MADCConfig::set_preamp_sampling_window_end(SamplingWindowTiming const& value)
{
	m_preamp_sampling_window_end = value;
}

MADCConfig::SamplingWindowTiming MADCConfig::get_iconv_sampling_window_start() const
{
	return m_iconv_sampling_window_start;
}

void MADCConfig::set_iconv_sampling_window_start(SamplingWindowTiming const& value)
{
	m_iconv_sampling_window_start = value;
}

MADCConfig::SamplingWindowTiming MADCConfig::get_iconv_sampling_window_end() const
{
	return m_iconv_sampling_window_end;
}

void MADCConfig::set_iconv_sampling_window_end(SamplingWindowTiming const& value)
{
	m_iconv_sampling_window_end = value;
}

bool MADCConfig::get_sample_on_positive_edge() const
{
	return m_sample_on_positive_edge;
}

void MADCConfig::set_sample_on_positive_edge(bool const value)
{
	m_sample_on_positive_edge = value;
}

bool MADCConfig::get_enable_dummy_data() const
{
	return m_enable_dummy_data;
}

void MADCConfig::set_enable_dummy_data(bool const value)
{
	m_enable_dummy_data = value;
}

bool MADCConfig::get_connect_preamp_to_madc() const
{
	return m_connect_preamp_to_madc;
}

void MADCConfig::set_connect_preamp_to_madc(bool const value)
{
	m_connect_preamp_to_madc = value;
}

bool MADCConfig::get_connect_pads_to_madc() const
{
	return m_connect_pads_to_madc;
}

void MADCConfig::set_connect_pads_to_madc(bool const value)
{
	m_connect_pads_to_madc = value;
}

bool MADCConfig::get_connect_preamp_to_pads() const
{
	return m_connect_preamp_to_pads;
}

void MADCConfig::set_connect_preamp_to_pads(bool const value)
{
	m_connect_preamp_to_pads = value;
}

MADCConfig::PreampGainCapacitorSize MADCConfig::get_preamp_gain_capacitor_size() const
{
	return m_preamp_gain_capacitor_size;
}

void MADCConfig::set_preamp_gain_capacitor_size(PreampGainCapacitorSize const& value)
{
	m_preamp_gain_capacitor_size = value;
}

bool MADCConfig::get_enable_madc_clock_scaling() const
{
	return m_enable_madc_clock_scaling;
}

void MADCConfig::set_enable_madc_clock_scaling(bool const value)
{
	m_enable_madc_clock_scaling = value;
}

MADCConfig::MADCClockScaleValue MADCConfig::get_madc_clock_scale_value() const
{
	return m_madc_clock_scale_value;
}

void MADCConfig::set_madc_clock_scale_value(MADCClockScaleValue const& value)
{
	m_madc_clock_scale_value = value;
}

bool MADCConfig::get_enable_active_mux_amplifiers() const
{
	return m_enable_active_mux_amplifiers;
}

void MADCConfig::set_enable_active_mux_amplifiers(bool const value)
{
	m_enable_active_mux_amplifiers = value;
}

MADCConfig::hemisphere_type const& MADCConfig::get_enable_pseudo_differential_reference() const
{
	return m_enable_pseudo_differential_reference;
}

void MADCConfig::set_enable_pseudo_differential_reference(hemisphere_type const& value)
{
	m_enable_pseudo_differential_reference = value;
}

bool MADCConfig::get_signal_selection_connect_current_meter() const
{
	return m_signal_selection_connect_current_meter;
}

void MADCConfig::set_signal_selection_connect_current_meter(bool const value)
{
	m_signal_selection_connect_current_meter = value;
}

bool MADCConfig::get_signal_selection_connect_active_mux() const
{
	return m_signal_selection_connect_active_mux;
}

void MADCConfig::set_signal_selection_connect_active_mux(bool const value)
{
	m_signal_selection_connect_active_mux = value;
}

bool MADCConfig::get_signal_selection_connect_debug() const
{
	return m_signal_selection_connect_debug;
}

void MADCConfig::set_signal_selection_connect_debug(bool const value)
{
	m_signal_selection_connect_debug = value;
}

bool MADCConfig::get_signal_selection_connect_preamp() const
{
	return m_signal_selection_connect_preamp;
}

void MADCConfig::set_signal_selection_connect_preamp(bool const value)
{
	m_signal_selection_connect_preamp = value;
}

bool MADCConfig::get_enable_iconv_amplifier() const
{
	return m_enable_iconv_amplifier;
}

void MADCConfig::set_enable_iconv_amplifier(bool const value)
{
	m_enable_iconv_amplifier = value;
}

MADCConfig::hemisphere_type const& MADCConfig::get_connect_iconv_neuron() const
{
	return m_connect_iconv_neuron;
}

void MADCConfig::set_connect_iconv_neuron(hemisphere_type const& value)
{
	m_connect_iconv_neuron = value;
}

MADCConfig::synapse_target_type const& MADCConfig::get_connect_iconv_synapse() const
{
	return m_connect_iconv_synapse;
}

void MADCConfig::set_connect_iconv_synapse(synapse_target_type const& value)
{
	m_connect_iconv_synapse = value;
}

double MADCConfig::calculate_sample_rate(double const& madc_base_frequency) const
{
	double const res =
	    madc_base_frequency / (m_sample_duration_adjust + minimal_madc_clock_cycles_per_sample);
	if (m_enable_madc_clock_scaling)
		return res / (2 * (m_madc_clock_scale_value + 1));
	else
		return res;
}

template <typename AddressT>
std::array<AddressT, MADCConfig::config_size_in_words> SYMBOL_VISIBLE
MADCConfig::addresses(coordinate_type const& /* coord */)
{
	std::array<AddressT, MADCConfig::config_size_in_words> addresses;
	size_t i = 1;

	// we start with an offset of 1 since the first word is wrapped by MADCControl
	std::generate(addresses.begin(), addresses.end(), [&]() {
		return static_cast<AddressT>(madc_base_address + i++);
	});
	return addresses;
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, MADCConfig::config_size_in_words>
    MADCConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, MADCConfig::config_size_in_words>
    MADCConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& coord);

namespace {

struct MADCConfigBitfield
{
	union
	{
		std::array<uint32_t, MADCConfig::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			/* word 0 */                                                           \
			(uint32_t                                        :  3; /*  2 -  0 */ ) \
			(uint32_t active_mux_initially_selected_input    :  1; /*       3 */ ) \
			(uint32_t active_mux_input_select_length         :  4; /*  7 -  4 */ ) \
			(uint32_t sample_duration_adjust                 :  3; /* 10 -  8 */ ) \
			(uint32_t enable_sar_reset_on_fall               :  1; /*      11 */ ) \
			(uint32_t sar_reset_wait                         :  2; /* 13 - 12 */ ) \
			(uint32_t sar_reset_length                       :  2; /* 15 - 14 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 1 */                                                           \
			(uint32_t powerup_wait_value                     : 10; /*  9 -  0 */ ) \
			(uint32_t                                        :  2; /* 11 - 10 */ ) \
			(uint32_t conversion_cycles_offset               :  4; /* 15 - 12 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 2 */                                                           \
			(uint32_t calibration_wait_value                 : 10; /*       0 */ ) \
			(uint32_t                                        :  5; /* 14 - 10 */ ) \
			(uint32_t enable_calibration                     :  1; /*      15 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 3 */                                                           \
			(uint32_t number_of_samples                      : 16; /* 15 -  0 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 4 */                                                           \
			(uint32_t preamp_sampling_window_start           :  5; /*  4 -  0 */ ) \
			(uint32_t preamp_sampling_window_end             :  5; /*  9 -  5 */ ) \
			(uint32_t                                        :  6; /* 15 - 10 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 5 */                                                           \
			(uint32_t iconv_sampling_window_start            :  5; /*  4 -  0 */ ) \
			(uint32_t iconv_sampling_window_end              :  5; /*  9 -  5 */ ) \
			(uint32_t                                        :  6; /* 15 - 10 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 6 */                                                           \
			(uint32_t sample_on_positive_edge                :  1; /*       0 */ ) \
			(uint32_t enable_dummy_data                      :  1; /*       1 */ ) \
			(uint32_t                                        : 14; /* 15 -  2 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 7 */                                                           \
			(uint32_t connect_preamp_to_madc                 :  1; /*       0 */ ) \
			(uint32_t connect_pads_to_madc                   :  1; /*       1 */ ) \
			(uint32_t connect_preamp_to_pads                 :  1; /*       2 */ ) \
			(uint32_t preamp_gain_capacitor_size             :  5; /*       3 */ ) \
			(uint32_t                                        :  8; /* 15 -  8 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 8 */                                                           \
			(uint32_t enable_madc_clock_scaling              :  1; /*       0 */ ) \
			(uint32_t madc_clock_scale_value                 : 12; /*       1 */ ) \
			(uint32_t                                        :  3; /* 15 - 13 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			/* word 9 */                                                           \
			(uint32_t enable_active_mux_amplifiers           :  1; /*       0 */ ) \
			(uint32_t enable_pseudo_differential_reference_n :  1; /*       1 */ ) \
			(uint32_t enable_pseudo_differential_reference_s :  1; /*       2 */ ) \
			(uint32_t signal_selection_connect_current_meter :  1; /*       3 */ ) \
			(uint32_t signal_selection_connect_active_mux    :  1; /*       4 */ ) \
			(uint32_t signal_selection_connect_debug         :  1; /*       5 */ ) \
			(uint32_t signal_selection_connect_preamp        :  1; /*       6 */ ) \
			(uint32_t enable_iconv_amplifier                 :  1; /*       7 */ ) \
			(uint32_t connect_iconv_neuron_n                 :  1; /*       8 */ ) \
			(uint32_t connect_iconv_neuron_s                 :  1; /*       9 */ ) \
			(uint32_t connect_iconv_synapse_i                :  1; /*      10 */ ) \
			(uint32_t connect_iconv_synapse_x                :  1; /*      11 */ ) \
			(uint32_t                                        :  4; /* 15 - 12 */ ) \
			(uint32_t                                        : 16; /* 31 - 16 */ )
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	MADCConfigBitfield()
	{
		u.words = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
	}

	MADCConfigBitfield(std::array<uint32_t, MADCConfig::config_size_in_words> const& data)
	{
		u.words = data;
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, MADCConfig::config_size_in_words> MADCConfig::encode() const
{
	MADCConfigBitfield bitfield;

	bitfield.u.m.active_mux_initially_selected_input = m_active_mux_initially_selected_input;
#ifndef __ppu__
	if (m_active_mux_input_select_length.value() > 1) {
		auto logger = log4cxx::Logger::getLogger("haldls.MADCConfig");
		LOG4CXX_WARN(
		    logger, "encode(): Active mux input select length > 1, channel assignment lags one "
		            "sample and can't be reliably used due to possible event loss.");
	}
#endif
	bitfield.u.m.active_mux_input_select_length = m_active_mux_input_select_length;
	bitfield.u.m.sample_duration_adjust = m_sample_duration_adjust;
	bitfield.u.m.enable_sar_reset_on_fall = m_enable_sar_reset_on_fall;
	bitfield.u.m.sar_reset_wait = m_sar_reset_wait;
	bitfield.u.m.sar_reset_length = m_sar_reset_length;
	bitfield.u.m.powerup_wait_value = m_powerup_wait_value;
	bitfield.u.m.conversion_cycles_offset = m_conversion_cycles_offset;
	bitfield.u.m.calibration_wait_value = m_calibration_wait_value;
	bitfield.u.m.enable_calibration = m_enable_calibration;
	bitfield.u.m.number_of_samples = m_number_of_samples;
	bitfield.u.m.preamp_sampling_window_start = m_preamp_sampling_window_start;
	bitfield.u.m.preamp_sampling_window_end = m_preamp_sampling_window_end;
	bitfield.u.m.iconv_sampling_window_start = m_iconv_sampling_window_start;
	bitfield.u.m.iconv_sampling_window_end = m_iconv_sampling_window_end;
	bitfield.u.m.sample_on_positive_edge = m_sample_on_positive_edge;
	bitfield.u.m.enable_dummy_data = m_enable_dummy_data;
	bitfield.u.m.connect_preamp_to_madc = m_connect_preamp_to_madc;
	bitfield.u.m.connect_pads_to_madc = m_connect_pads_to_madc;
	bitfield.u.m.connect_preamp_to_pads = m_connect_preamp_to_pads;
	bitfield.u.m.preamp_gain_capacitor_size = m_preamp_gain_capacitor_size;
	bitfield.u.m.enable_madc_clock_scaling = m_enable_madc_clock_scaling;
	bitfield.u.m.madc_clock_scale_value = m_madc_clock_scale_value;
	bitfield.u.m.enable_active_mux_amplifiers = m_enable_active_mux_amplifiers;
	bitfield.u.m.enable_pseudo_differential_reference_n =
	    m_enable_pseudo_differential_reference[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.enable_pseudo_differential_reference_s =
	    m_enable_pseudo_differential_reference[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.signal_selection_connect_current_meter = m_signal_selection_connect_current_meter;
	bitfield.u.m.signal_selection_connect_active_mux = m_signal_selection_connect_active_mux;
	bitfield.u.m.signal_selection_connect_debug = m_signal_selection_connect_debug;
	bitfield.u.m.signal_selection_connect_preamp = m_signal_selection_connect_preamp;
	bitfield.u.m.enable_iconv_amplifier = m_enable_iconv_amplifier;
	bitfield.u.m.connect_iconv_neuron_n =
	    m_connect_iconv_neuron[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.connect_iconv_neuron_s =
	    m_connect_iconv_neuron[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.connect_iconv_synapse_i =
	    m_connect_iconv_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::inhibitory];
	bitfield.u.m.connect_iconv_synapse_x =
	    m_connect_iconv_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::excitatory];

	std::array<WordT, MADCConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, MADCConfig::config_size_in_words>
    MADCConfig::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, MADCConfig::config_size_in_words>
    MADCConfig::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void MADCConfig::decode(std::array<WordT, MADCConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, MADCConfig::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	MADCConfigBitfield bitfield(raw_data);

	m_active_mux_initially_selected_input =
	    halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(
	        bitfield.u.m.active_mux_initially_selected_input);
	m_active_mux_input_select_length =
	    ActiveMuxInputSelectLength(bitfield.u.m.active_mux_input_select_length);
	m_sample_duration_adjust = SampleDurationAdjust(bitfield.u.m.sample_duration_adjust);
	m_enable_sar_reset_on_fall = bitfield.u.m.enable_sar_reset_on_fall;
	m_sar_reset_wait = SARResetWait(bitfield.u.m.sar_reset_wait);
	m_sar_reset_length = SARResetLength(bitfield.u.m.sar_reset_length);
	m_powerup_wait_value = PowerupWaitValue(bitfield.u.m.powerup_wait_value);
	m_conversion_cycles_offset = ConversionCyclesOffset(bitfield.u.m.conversion_cycles_offset);
	m_calibration_wait_value = CalibrationWaitValue(bitfield.u.m.calibration_wait_value);
	m_enable_calibration = bitfield.u.m.enable_calibration;
	m_number_of_samples = NumberOfSamples(bitfield.u.m.number_of_samples);
	m_preamp_sampling_window_start =
	    SamplingWindowTiming(bitfield.u.m.preamp_sampling_window_start);
	m_preamp_sampling_window_end = SamplingWindowTiming(bitfield.u.m.preamp_sampling_window_end);
	m_iconv_sampling_window_start = SamplingWindowTiming(bitfield.u.m.iconv_sampling_window_start);
	m_iconv_sampling_window_end = SamplingWindowTiming(bitfield.u.m.iconv_sampling_window_end);
	m_sample_on_positive_edge = bitfield.u.m.sample_on_positive_edge;
	m_enable_dummy_data = bitfield.u.m.enable_dummy_data;
	m_connect_preamp_to_madc = bitfield.u.m.connect_preamp_to_madc;
	m_connect_pads_to_madc = bitfield.u.m.connect_pads_to_madc;
	m_connect_preamp_to_pads = bitfield.u.m.connect_preamp_to_pads;
	m_preamp_gain_capacitor_size = PreampGainCapacitorSize(bitfield.u.m.preamp_gain_capacitor_size);
	m_enable_madc_clock_scaling = bitfield.u.m.enable_madc_clock_scaling;
	m_madc_clock_scale_value = MADCClockScaleValue(bitfield.u.m.madc_clock_scale_value);
	m_enable_active_mux_amplifiers = bitfield.u.m.enable_active_mux_amplifiers;
	m_enable_pseudo_differential_reference[halco::hicann_dls::vx::HemisphereOnDLS(0)] =
	    bitfield.u.m.enable_pseudo_differential_reference_n;
	m_enable_pseudo_differential_reference[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.enable_pseudo_differential_reference_s;
	m_signal_selection_connect_current_meter = bitfield.u.m.signal_selection_connect_current_meter;
	m_signal_selection_connect_active_mux = bitfield.u.m.signal_selection_connect_active_mux;
	m_signal_selection_connect_debug = bitfield.u.m.signal_selection_connect_debug;
	m_signal_selection_connect_preamp = bitfield.u.m.signal_selection_connect_preamp;
	m_enable_iconv_amplifier = bitfield.u.m.enable_iconv_amplifier;
	m_connect_iconv_neuron[halco::hicann_dls::vx::HemisphereOnDLS(0)] =
	    bitfield.u.m.connect_iconv_neuron_n;
	m_connect_iconv_neuron[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.connect_iconv_neuron_s;
	m_connect_iconv_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::inhibitory] =
	    bitfield.u.m.connect_iconv_synapse_i;
	m_connect_iconv_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::excitatory] =
	    bitfield.u.m.connect_iconv_synapse_x;
}

template SYMBOL_VISIBLE void MADCConfig::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        MADCConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void MADCConfig::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, MADCConfig::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, MADCConfig const& config)
{
	std::stringstream ss;
	ss << "MADCConfig(\n" << std::boolalpha;
	// clang-format off
	ss << "\tactive_mux_initially_selected_input:   \t" << config.m_active_mux_initially_selected_input << "\n"
	   << "\tactive_mux_input_select_length:        \t" << config.m_active_mux_input_select_length << "\n"
	   << "\tsample_duration_adjust:                \t" << config.m_sample_duration_adjust << "\n"
	   << "\tenable_sar_reset_on_fall:              \t" << config.m_enable_sar_reset_on_fall << "\n"
	   << "\tsar_reset_wait:                        \t" << config.m_sar_reset_wait << "\n"
	   << "\tsar_reset_length:                      \t" << config.m_sar_reset_length << "\n"
	   << "\tpowerup_wait_value:                    \t" << config.m_powerup_wait_value << "\n"
	   << "\tconversion_cycles_offset:              \t" << config.m_conversion_cycles_offset << "\n"
	   << "\tcalibration_wait_value:                \t" << config.m_calibration_wait_value << "\n"
	   << "\tenable_calibration:                    \t" << config.m_enable_calibration << "\n"
	   << "\tnumber_of_samples:                     \t" << config.m_number_of_samples << "\n"
	   << "\tpreamp_sampling_window_start:          \t" << config.m_preamp_sampling_window_start << "\n"
	   << "\tpreamp_sampling_window_end:            \t" << config.m_preamp_sampling_window_end << "\n"
	   << "\ticonv_sampling_window_start:           \t" << config.m_iconv_sampling_window_start << "\n"
	   << "\ticonv_sampling_window_end:             \t" << config.m_iconv_sampling_window_end << "\n"
	   << "\tsample_on_positive_edge:               \t" << config.m_sample_on_positive_edge << "\n"
	   << "\tenable_dummy_data:                     \t" << config.m_enable_dummy_data << "\n"
	   << "\tconnect_preamp_to_madc:                \t" << config.m_connect_preamp_to_madc << "\n"
	   << "\tconnect_pads_to_madc:                  \t" << config.m_connect_pads_to_madc << "\n"
	   << "\tconnect_preamp_to_pads:                \t" << config.m_connect_preamp_to_pads << "\n"
	   << "\tpreamp_gain_capacitor_size:            \t" << config.m_preamp_gain_capacitor_size << "\n"
	   << "\tenable_madc_clock_scaling:             \t" << config.m_enable_madc_clock_scaling << "\n"
	   << "\tmadc_clock_scale_value:                \t" << config.m_madc_clock_scale_value << "\n"
	   << "\tenable_active_mux_amplifiers:          \t" << config.m_enable_active_mux_amplifiers << "\n"
	   << "\tenable_pseudo_differential_reference:  \t[";
	ss << hate::join(config.m_enable_pseudo_differential_reference.begin(), config.m_enable_pseudo_differential_reference.end(), ", ");
	ss << "]\n"
	   << "\tsignal_selection_connect_current_meter:\t" << config.m_signal_selection_connect_current_meter << "\n"
	   << "\tsignal_selection_connect_active_mux:   \t" << config.m_signal_selection_connect_active_mux << "\n"
	   << "\tsignal_selection_connect_debug:        \t" << config.m_signal_selection_connect_debug << "\n"
	   << "\tsignal_selection_connect_preamp:       \t" << config.m_signal_selection_connect_preamp << "\n"
	   << "\tenable_iconv_amplifier:                \t" << config.m_enable_iconv_amplifier << "\n"
	   << "\tconnect_iconv_neuron                   \t[";
	ss << hate::join(config.m_connect_iconv_neuron.begin(), config.m_connect_iconv_neuron.end(), ", ");
	ss << "]\n"
	   << "\tconnect_iconv_synapse:                 \t[inhibitory: "
	   << config.m_connect_iconv_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::inhibitory]
	   << ", excitatory: "
	   << config.m_connect_iconv_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::excitatory]
	   << "]\n)";
	// clang-format on

	return (os << ss.str());
}

bool MADCConfig::operator==(MADCConfig const& other) const
{
	return (
	    m_active_mux_initially_selected_input == other.m_active_mux_initially_selected_input &&
	    m_active_mux_input_select_length == other.m_active_mux_input_select_length &&
	    m_sample_duration_adjust == other.m_sample_duration_adjust &&
	    m_enable_sar_reset_on_fall == other.m_enable_sar_reset_on_fall &&
	    m_sar_reset_wait == other.m_sar_reset_wait &&
	    m_sar_reset_length == other.m_sar_reset_length &&
	    m_powerup_wait_value == other.m_powerup_wait_value &&
	    m_conversion_cycles_offset == other.m_conversion_cycles_offset &&
	    m_calibration_wait_value == other.m_calibration_wait_value &&
	    m_enable_calibration == other.m_enable_calibration &&
	    m_number_of_samples == other.m_number_of_samples &&
	    m_preamp_sampling_window_start == other.m_preamp_sampling_window_start &&
	    m_preamp_sampling_window_end == other.m_preamp_sampling_window_end &&
	    m_iconv_sampling_window_start == other.m_iconv_sampling_window_start &&
	    m_iconv_sampling_window_end == other.m_iconv_sampling_window_end &&
	    m_sample_on_positive_edge == other.m_sample_on_positive_edge &&
	    m_enable_dummy_data == other.m_enable_dummy_data &&
	    m_connect_preamp_to_madc == other.m_connect_preamp_to_madc &&
	    m_connect_pads_to_madc == other.m_connect_pads_to_madc &&
	    m_connect_preamp_to_pads == other.m_connect_preamp_to_pads &&
	    m_preamp_gain_capacitor_size == other.m_preamp_gain_capacitor_size &&
	    m_enable_madc_clock_scaling == other.m_enable_madc_clock_scaling &&
	    m_madc_clock_scale_value == other.m_madc_clock_scale_value &&
	    m_enable_active_mux_amplifiers == other.m_enable_active_mux_amplifiers &&
	    m_enable_pseudo_differential_reference == other.m_enable_pseudo_differential_reference &&
	    m_signal_selection_connect_current_meter ==
	        other.m_signal_selection_connect_current_meter &&
	    m_signal_selection_connect_active_mux == other.m_signal_selection_connect_active_mux &&
	    m_signal_selection_connect_debug == other.m_signal_selection_connect_debug &&
	    m_signal_selection_connect_preamp == other.m_signal_selection_connect_preamp &&
	    m_enable_iconv_amplifier == other.m_enable_iconv_amplifier &&
	    m_connect_iconv_neuron == other.m_connect_iconv_neuron &&
	    m_connect_iconv_synapse == other.m_connect_iconv_synapse);
}

bool MADCConfig::operator!=(MADCConfig const& other) const
{
	return !(*this == other);
}

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::MADCControl)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::MADCConfig)
