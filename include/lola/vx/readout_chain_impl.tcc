#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/readout_chain.h"

#include "halco/common/iter_all.h"
#include "haldls/cerealization.tcc"
#include "hate/indent.h"
#include "hate/join.h"
#include "lola/vx/cerealization.tcc"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 1
#define CHIP_REVISION_STR v1
#elif CHIP_REVISION == 2
#define CHIP_REVISION_STR v2
#elif CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

std::ostream& operator<<(std::ostream& os, ReadoutChain::InputMux const& config)
{
	os << "InputMux(\n";
	for (auto mux : halco::common::iter_all<
	         halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection>()) {
		std::stringstream ss;
		ss << mux << ":\n";
		ss << config[mux];
		os << hate::indent(ss.str(), "\t") << "\n";
	}
	os << "\n";
	os << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::BufferToPad const& config)
{
	os << "BufferToPad(\n";
	for (auto mux : halco::common::iter_all<
	         halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection>()) {
		std::stringstream ss;
		ss << mux << ":\n";
		ss << config[mux];
		os << hate::indent(ss.str(), "\t") << "\n";
	}
	os << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::PadMux const& config)
{
	os << "PadMux(\n";
	for (auto mux : halco::common::iter_all<halco::hicann_dls::vx::PadMultiplexerConfigOnDLS>()) {
		std::stringstream ss;
		ss << mux << ":\n";
		ss << config[mux];
		os << hate::indent(ss.str(), "\t") << "\n";
	}
	os << "\n";
	os << ")";
	return os;
}

ReadoutChain::BufferToPadT::BufferToPadT() : enable(false), amp_i_bias() {}

bool ReadoutChain::BufferToPadT::operator==(BufferToPadT const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::BufferToPadT::operator!=(BufferToPadT const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::BufferToPadT const& config)
{
	std::stringstream ss;
	ss << "BufferToPadT(\n"
	   << "\tenable:     " << std::boolalpha << config.enable << "\n"
	   << "\tamp_i_bias: " << config.amp_i_bias << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::DynamicMux::DynamicMux() :
    enable_amplifiers(true), initially_selected_input(), input_select_length(), i_bias()
{}

bool ReadoutChain::DynamicMux::operator==(DynamicMux const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::DynamicMux::operator!=(DynamicMux const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::DynamicMux const& config)
{
	std::stringstream ss;
	ss << "DynamicMux(\n"
	   << "\tenable_amplifiers:        " << std::boolalpha << config.enable_amplifiers << "\n"
	   << "\tinitially_selected_input: " << config.initially_selected_input << "\n"
	   << "\tinput_select_length:      " << config.input_select_length << "\n"
	   << "\ti_bias:                   " << config.i_bias << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::PseudoDifferentialConverter::PseudoDifferentialConverter() :
    enable_reference({false, false}), v_ref(), buffer_bias()
{}

bool ReadoutChain::PseudoDifferentialConverter::operator==(
    PseudoDifferentialConverter const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::PseudoDifferentialConverter::operator!=(
    PseudoDifferentialConverter const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::PseudoDifferentialConverter const& config)
{
	std::stringstream ss;
	ss << "PseudoDifferentialConverter(\n";
	ss << "\tenable_reference: [";
	hate::join(ss, config.enable_reference.begin(), config.enable_reference.end(), ", ");
	ss << "]\n"
	   << "\tv_ref:            " << config.v_ref << "\n"
	   << "\tbuffer_bias:      " << config.buffer_bias << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::MADCInputCrossbar::MADCInputCrossbar() :
    connect_source_measure_unit(false),
    connect_dynamic_mux(true),
    connect_pad_mux(false),
    connect_madc_preamp(true)
{}

bool ReadoutChain::MADCInputCrossbar::operator==(MADCInputCrossbar const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADCInputCrossbar::operator!=(MADCInputCrossbar const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADCInputCrossbar const& config)
{
	std::stringstream ss;
	ss << "MADCInputCrossbar(\n"
	   << "\tconnect_source_measure_unit: " << std::boolalpha << config.connect_source_measure_unit
	   << "\n"
	   << "\tconnect_dynamic_mux:         " << config.connect_dynamic_mux << "\n"
	   << "\tconnect_pad_mux:             " << config.connect_pad_mux << "\n"
	   << "\tconnect_madc_preamp:         " << config.connect_madc_preamp << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::MADCPreamp::MADCPreamp() :
    gain_cap_size(), sampling_window_start(3), sampling_window_end(8), v_ref(), i_bias()
{}

bool ReadoutChain::MADCPreamp::operator==(MADCPreamp const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADCPreamp::operator!=(MADCPreamp const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADCPreamp const& config)
{
	std::stringstream ss;
	ss << "MADCPreamp(\n"
	   << "\tgain_cap_size:         " << std::boolalpha << config.gain_cap_size << "\n"
	   << "\tsampling_window_start: " << config.sampling_window_start << "\n"
	   << "\tsampling_window_end:   " << config.sampling_window_end << "\n"
	   << "\tv_ref:                 " << config.v_ref << "\n"
	   << "\ti_bias:                " << config.i_bias << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::MADCDebugCrossbar::MADCDebugCrossbar() :
    connect_preamp_to_madc(true), connect_pads_to_madc(false), connect_preamp_to_pads(false)
{}

bool ReadoutChain::MADCDebugCrossbar::operator==(MADCDebugCrossbar const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADCDebugCrossbar::operator!=(MADCDebugCrossbar const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADCDebugCrossbar const& config)
{
	std::stringstream ss;
	ss << "MADCDebugCrossbar(\n"
	   << "\tconnect_preamp_to_madc: " << std::boolalpha << config.connect_preamp_to_madc << "\n"
	   << "\tconnect_pads_to_madc:   " << config.connect_pads_to_madc << "\n"
	   << "\tconnect_preamp_to_pads: " << config.connect_preamp_to_pads << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::MADC::MADC() :
    sample_duration_adjust(SampleDurationAdjust()),
    enable_sar_reset_on_fall(false),
    sar_reset_wait(),
    sar_reset_length(),
    powerup_wait_value(),
    conversion_cycles_offset(),
    enable_calibration(false),
    calibration_wait_value(),
    number_of_samples(),
    sample_on_positive_edge(true),
    enable_dummy_data(false),
    enable_madc_clock_scaling(false),
    clock_scale_value(),
    in_500na()
{}

bool ReadoutChain::MADC::operator==(MADC const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADC::operator!=(MADC const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADC const& config)
{
	std::stringstream ss;
	ss << "MADC(\n"
	   << "\tsample_duration_adjust:    " << std::boolalpha << config.sample_duration_adjust << "\n"
	   << "\tenable_sar_reset_on_fall:  " << config.enable_sar_reset_on_fall << "\n"
	   << "\tsar_reset_wait:            " << config.sar_reset_wait << "\n"
	   << "\tsar_reset_length:          " << config.sar_reset_length << "\n"
	   << "\tpowerup_wait_value:        " << config.powerup_wait_value << "\n"
	   << "\tconversion_cycles_offset:  " << config.conversion_cycles_offset << "\n"
	   << "\tenable_calibration:        " << config.enable_calibration << "\n"
	   << "\tcalibration_wait_value:    " << config.calibration_wait_value << "\n"
	   << "\tnumber_of_samples:         " << config.number_of_samples << "\n"
	   << "\tsample_on_positive_edge:   " << config.sample_on_positive_edge << "\n"
	   << "\tenable_dummy_data:         " << config.enable_dummy_data << "\n"
	   << "\tenable_madc_clock_scaling: " << config.enable_madc_clock_scaling << "\n"
	   << "\tclock_scale_value:         " << config.clock_scale_value << "\n"
	   << "\tin_500na:                  " << config.in_500na << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::SourceMeasureUnit::SourceMeasureUnit() :
    sampling_window_start(9),
    sampling_window_end(14),
    connect_neuron_stimulus({false, false}),
    connect_synapse_debug({false, false}),
    test_voltage(),
    buffer_i_bias(),
    amp_v_ref(),
    amp_i_bias()
{}

bool ReadoutChain::SourceMeasureUnit::operator==(SourceMeasureUnit const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::SourceMeasureUnit::operator!=(SourceMeasureUnit const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::SourceMeasureUnit const& config)
{
	std::stringstream ss;
	ss << "SourceMeasureUnit(\n"
	   << "\tsampling_window_start:   " << std::boolalpha << config.sampling_window_start << "\n"
	   << "\tsampling_window_end:     " << config.sampling_window_end << "\n";
	ss << "\tconnect_neuron_stimulus: [";
	hate::join(
	    ss, config.connect_neuron_stimulus.begin(), config.connect_neuron_stimulus.end(), ", ");
	ss << "]\n";
	ss << "\tconnect_synapse_debug:   [";
	hate::join(ss, config.connect_synapse_debug.begin(), config.connect_synapse_debug.end(), ", ");
	ss << "]\n";
	ss << "\ttest_voltage:            " << config.test_voltage << "\n"
	   << "\tbuffer_i_bias:           " << config.buffer_i_bias << "\n"
	   << "\tamp_v_ref:               " << config.amp_v_ref << "\n"
	   << "\tamp_i_bias:              " << config.amp_i_bias << "\n"
	   << "\tamp_i_bias:              " << config.amp_i_bias << "\n)";
	os << ss.str();
	return os;
}

ReadoutChain::CurrentDAC::CurrentDAC() :
    enable_current(false),
    current(),
    sign(),
    connect_neuron_stimulus({false, false}),
    connect_synapse_debug({false, false}),
    i_bias(),
    i_bias_casc()
{}

bool ReadoutChain::CurrentDAC::operator==(CurrentDAC const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::CurrentDAC::operator!=(CurrentDAC const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::CurrentDAC const& config)
{
	std::stringstream ss;
	ss << "CurrentDAC(\n"
	   << "\tenable_current:          " << std::boolalpha << config.enable_current << "\n"
	   << "\tcurrent:                 " << config.current << "\n"
	   << "\tsign:                    " << config.sign << "\n"
	   << "\tconnect_neuron_stimulus: [";
	hate::join(
	    ss, config.connect_neuron_stimulus.begin(), config.connect_neuron_stimulus.end(), ", ");
	ss << "]\n";
	ss << "\tconnect_synapse_debug:   [";
	hate::join(ss, config.connect_synapse_debug.begin(), config.connect_synapse_debug.end(), ", ");
	ss << "]\n"
	   << "\ti_bias:                  " << config.i_bias << "\n"
	   << "\ti_bias_casc:             " << config.i_bias_casc << "\n)";
	os << ss.str();
	return os;
}

bool ReadoutChain::operator==(ReadoutChain const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::operator!=(ReadoutChain const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain const& config)
{
	os << "ReadoutChain(\n"
	   << config.input_mux << "\n"
	   << config.buffer_to_pad << "\n"
	   << config.dynamic_mux << "\n"
	   << config.pseudo_diff_converter << "\n"
	   << config.madc_input_crossbar << "\n"
	   << config.madc_preamp << "\n"
	   << config.madc_debug_crossbar << "\n"
	   << config.madc << "\n"
	   << config.pad_mux << "\n"
	   << config.source_measure_unit << "\n"
	   << config.current_dac << "\n"
	   << ")";
	return os;
}

} // lola::vx::CHIP_REVISION

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::BufferToPadT)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::DynamicMux)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::PseudoDifferentialConverter)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCInputCrossbar)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCPreamp)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::MADCDebugCrossbar)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::MADC)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(
    lola::vx::CHIP_REVISION_STR::ReadoutChain::SourceMeasureUnit)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain::CurrentDAC)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::CHIP_REVISION_STR::ReadoutChain)
