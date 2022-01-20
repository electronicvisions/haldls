#include "haldls/vx/readout.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/indent.h"
#include "hate/join.h"

namespace haldls::vx {

using halco::common::typed_array;
using namespace halco::hicann_dls::vx;

PadMultiplexerConfig::PadMultiplexerConfig() :
    m_cadc_v_ramp_mux(),
    m_cadc_v_ramp_mux_to_pad(false),
    m_capmem_i_out_mux(),
    m_capmem_i_out_mux_to_inter(false),
    m_capmem_inter_mux_to_pad(false),
    m_capmem_v_out_mux(),
    m_capmem_v_out_mux_to_inter(false),
    m_capmem_v_ref_mux(),
    m_capmem_v_ref_mux_to_inter(false),
    m_neuron_i_stim_mux(),
    m_neuron_i_stim_mux_to_pad(),
    m_cadc_debug_acausal_to_inter(false),
    m_cadc_debug_causal_to_inter(false),
    m_synin_debug_inhibitory_to_inter(false),
    m_synin_debug_excitatory_to_inter(false),
    m_synapse_inter_mux_to_pad(false),
    m_buffer_to_pad(),
    m_debug_to_pad(false)
{}

PadMultiplexerConfig::capmem_quadrant_type const& PadMultiplexerConfig::get_cadc_v_ramp_mux() const
{
	return m_cadc_v_ramp_mux;
}

void PadMultiplexerConfig::set_cadc_v_ramp_mux(capmem_quadrant_type const& value)
{
	m_cadc_v_ramp_mux = value;
}

bool PadMultiplexerConfig::get_cadc_v_ramp_mux_to_pad() const
{
	return m_cadc_v_ramp_mux_to_pad;
}

void PadMultiplexerConfig::set_cadc_v_ramp_mux_to_pad(bool const value)
{
	m_cadc_v_ramp_mux_to_pad = value;
}

PadMultiplexerConfig::capmem_quadrant_type const& PadMultiplexerConfig::get_capmem_i_out_mux() const
{
	return m_capmem_i_out_mux;
}

void PadMultiplexerConfig::set_capmem_i_out_mux(capmem_quadrant_type const& value)
{
	m_capmem_i_out_mux = value;
}

bool PadMultiplexerConfig::get_capmem_i_out_mux_to_capmem_intermediate_mux() const
{
	return m_capmem_i_out_mux_to_inter;
}

void PadMultiplexerConfig::set_capmem_i_out_mux_to_capmem_intermediate_mux(bool const value)
{
	m_capmem_i_out_mux_to_inter = value;
}

bool PadMultiplexerConfig::get_capmem_intermediate_mux_to_pad() const
{
	return m_capmem_inter_mux_to_pad;
}

void PadMultiplexerConfig::set_capmem_intermediate_mux_to_pad(bool const value)
{
	m_capmem_inter_mux_to_pad = value;
}

PadMultiplexerConfig::capmem_quadrant_type const& PadMultiplexerConfig::get_capmem_v_out_mux() const
{
	return m_capmem_v_out_mux;
}

void PadMultiplexerConfig::set_capmem_v_out_mux(capmem_quadrant_type const& value)
{
	m_capmem_v_out_mux = value;
}

bool PadMultiplexerConfig::get_capmem_v_out_mux_to_capmem_intermediate_mux() const
{
	return m_capmem_v_out_mux_to_inter;
}

void PadMultiplexerConfig::set_capmem_v_out_mux_to_capmem_intermediate_mux(bool const value)
{
	m_capmem_v_out_mux_to_inter = value;
}

PadMultiplexerConfig::capmem_quadrant_type const& PadMultiplexerConfig::get_capmem_v_ref_mux() const
{
	return m_capmem_v_ref_mux;
}

void PadMultiplexerConfig::set_capmem_v_ref_mux(capmem_quadrant_type const& value)
{
	m_capmem_v_ref_mux = value;
}

bool PadMultiplexerConfig::get_capmem_v_ref_mux_to_capmem_intermediate_mux() const
{
	return m_capmem_v_ref_mux_to_inter;
}

void PadMultiplexerConfig::set_capmem_v_ref_mux_to_capmem_intermediate_mux(bool const value)
{
	m_capmem_v_ref_mux_to_inter = value;
}

PadMultiplexerConfig::hemisphere_type const& PadMultiplexerConfig::get_neuron_i_stim_mux() const
{
	return m_neuron_i_stim_mux;
}

void PadMultiplexerConfig::set_neuron_i_stim_mux(hemisphere_type const& value)
{
	m_neuron_i_stim_mux = value;
}

bool PadMultiplexerConfig::get_neuron_i_stim_mux_to_pad() const
{
	return m_neuron_i_stim_mux_to_pad;
}

void PadMultiplexerConfig::set_neuron_i_stim_mux_to_pad(bool const value)
{
	m_neuron_i_stim_mux_to_pad = value;
}

bool PadMultiplexerConfig::get_cadc_debug_acausal_to_synapse_intermediate_mux() const
{
	return m_cadc_debug_acausal_to_inter;
}

void PadMultiplexerConfig::set_cadc_debug_acausal_to_synapse_intermediate_mux(bool const value)
{
	m_cadc_debug_acausal_to_inter = value;
}

bool PadMultiplexerConfig::get_cadc_debug_causal_to_synapse_intermediate_mux() const
{
	return m_cadc_debug_causal_to_inter;
}

void PadMultiplexerConfig::set_cadc_debug_causal_to_synapse_intermediate_mux(bool const value)
{
	m_cadc_debug_causal_to_inter = value;
}

bool PadMultiplexerConfig::get_synin_debug_inhibitory_to_synapse_intermediate_mux() const
{
	return m_synin_debug_inhibitory_to_inter;
}

void PadMultiplexerConfig::set_synin_debug_inhibitory_to_synapse_intermediate_mux(bool const value)
{
	m_synin_debug_inhibitory_to_inter = value;
}

bool PadMultiplexerConfig::get_synin_debug_excitatory_to_synapse_intermediate_mux() const
{
	return m_synin_debug_excitatory_to_inter;
}

void PadMultiplexerConfig::set_synin_debug_excitatory_to_synapse_intermediate_mux(bool const value)
{
	m_synin_debug_excitatory_to_inter = value;
}

bool PadMultiplexerConfig::get_synapse_intermediate_mux_to_pad() const
{
	return m_synapse_inter_mux_to_pad;
}

void PadMultiplexerConfig::set_synapse_intermediate_mux_to_pad(bool const value)
{
	m_synapse_inter_mux_to_pad = value;
}

PadMultiplexerConfig::buffer_type const& PadMultiplexerConfig::get_buffer_to_pad() const
{
	return m_buffer_to_pad;
}

void PadMultiplexerConfig::set_buffer_to_pad(buffer_type const& value)
{
	m_buffer_to_pad = value;
}

bool PadMultiplexerConfig::get_debug_to_pad() const
{
	return m_debug_to_pad;
}

void PadMultiplexerConfig::set_debug_to_pad(bool const value)
{
	m_debug_to_pad = value;
}

bool PadMultiplexerConfig::operator==(PadMultiplexerConfig const& other) const
{
	return (
	    m_cadc_v_ramp_mux == other.m_cadc_v_ramp_mux &&
	    m_cadc_v_ramp_mux_to_pad == other.m_cadc_v_ramp_mux_to_pad &&
	    m_capmem_i_out_mux == other.m_capmem_i_out_mux &&
	    m_capmem_i_out_mux_to_inter == other.m_capmem_i_out_mux_to_inter &&
	    m_capmem_inter_mux_to_pad == other.m_capmem_inter_mux_to_pad &&
	    m_capmem_v_out_mux == other.m_capmem_v_out_mux &&
	    m_capmem_v_out_mux_to_inter == other.m_capmem_v_out_mux_to_inter &&
	    m_capmem_v_ref_mux == other.m_capmem_v_ref_mux &&
	    m_capmem_v_ref_mux_to_inter == other.m_capmem_v_ref_mux_to_inter &&
	    m_neuron_i_stim_mux == other.m_neuron_i_stim_mux &&
	    m_neuron_i_stim_mux_to_pad == other.m_neuron_i_stim_mux_to_pad &&
	    m_cadc_debug_acausal_to_inter == other.m_cadc_debug_acausal_to_inter &&
	    m_cadc_debug_causal_to_inter == other.m_cadc_debug_causal_to_inter &&
	    m_synin_debug_inhibitory_to_inter == other.m_synin_debug_inhibitory_to_inter &&
	    m_synin_debug_excitatory_to_inter == other.m_synin_debug_excitatory_to_inter &&
	    m_synapse_inter_mux_to_pad == other.m_synapse_inter_mux_to_pad &&
	    m_buffer_to_pad == other.m_buffer_to_pad &&
	    m_debug_to_pad == other.m_debug_to_pad);
}

bool PadMultiplexerConfig::operator!=(PadMultiplexerConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PadMultiplexerConfig const& config)
{
	std::stringstream ss;
	ss << "PadMultiplexerConfig(\n" << std::boolalpha;
	ss << "\tcadc_v_ramp_mux:                                    \t[";
	hate::join(ss, config.m_cadc_v_ramp_mux.begin(), config.m_cadc_v_ramp_mux.end(), ", ");
	ss << "]\n";
	ss << "\tcadc_v_ramp_mux_to_pad:                             \t"
	   << config.m_cadc_v_ramp_mux_to_pad << "\n";
	ss << "\tcapmem_i_out_mux:                                   \t[";
	hate::join(ss, config.m_capmem_i_out_mux.begin(), config.m_capmem_i_out_mux.end(), ", ");
	ss << "]\n";
	ss << "\tcapmem_i_out_mux_to_capmem_intermediate_mux:        \t"
	   << config.m_capmem_i_out_mux_to_inter << "\n";
	ss << "\tcapmem_intermediate_mux_to_pad:                     \t"
	   << config.m_capmem_inter_mux_to_pad << "\n";
	ss << "\tcapmem_v_ref_mux:                                   \t[";
	hate::join(ss, config.m_capmem_v_ref_mux.begin(), config.m_capmem_v_ref_mux.end(), ", ");
	ss << "]\n";
	ss << "\tcapmem_v_ref_mux_to_capmem_intermediate_mux:        \t"
	   << config.m_capmem_v_ref_mux_to_inter << "\n";
	ss << "\tneuron_i_stim_mux: [";
	hate::join(ss, config.m_neuron_i_stim_mux.begin(), config.m_neuron_i_stim_mux.end(), ", ");
	ss << "]\n";
	ss << "\tneuron_i_stim_mux_to_pad: " << config.m_neuron_i_stim_mux_to_pad << "\n";
	ss << "\tcadc_debug_acausal_to_synapse_intermediate_mux:     \t"
	   << config.m_cadc_debug_acausal_to_inter << "\n";
	ss << "\tcadc_debug_causal_to_synapse_intermediate_mux:      \t"
	   << config.m_cadc_debug_causal_to_inter << "\n";
	ss << "\tsynin_debug_inhibitory_to_synapse_intermediate_mux: \t"
	   << config.m_synin_debug_inhibitory_to_inter << "\n";
	ss << "\tsynin_debug_excitatory_to_synapse_intermediate_mux: \t"
	   << config.m_synin_debug_excitatory_to_inter << "\n";
	ss << "\tsynapse_intermediate_mux_to_pad:                    \t"
	   << config.m_synapse_inter_mux_to_pad << "\n";
	ss << "\tbuffer_to_pad:                                      \t[";
	hate::join(ss, config.m_buffer_to_pad.begin(), config.m_buffer_to_pad.end(), ", ");
	ss << "]\n";
	ss << "\tdebug_to_pad:                                       \t" << config.m_debug_to_pad
	   << "\n)";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, PadMultiplexerConfig::config_size_in_words> PadMultiplexerConfig::addresses(
    coordinate_type const& coord)
{
	return {
	    AddressT(
	        madc_base_address + 14 + coord.toEnum() * PadMultiplexerConfig::config_size_in_words),
	    AddressT(
	        madc_base_address + 15 + coord.toEnum() * PadMultiplexerConfig::config_size_in_words)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, PadMultiplexerConfig::config_size_in_words>
    PadMultiplexerConfig::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PadMultiplexerConfig::config_size_in_words>
PadMultiplexerConfig::addresses(coordinate_type const& coord);

namespace {

struct PadMultiplexerConfigBitfield
{
	union
	{
		std::array<uint32_t, PadMultiplexerConfig::config_size_in_words> words;

		struct __attribute__((packed))
		{
			// clang-format off
			uint32_t cadc_v_ramp_out_mux_q3      : 1;  // 0  in base + {14, 16}
			uint32_t cadc_v_ramp_out_mux_q2      : 1;  // 1
			uint32_t cadc_v_ramp_out_mux_q1      : 1;  // 2
			uint32_t cadc_v_ramp_out_mux_q0      : 1;  // 3
			uint32_t cadc_v_ramp_mux_to_pad      : 1;  // 4
			uint32_t capmem_i_out_mux_q3         : 1;  // 5
			uint32_t capmem_i_out_mux_q2         : 1;  // 6
			uint32_t capmem_i_out_mux_q1         : 1;  // 7
			uint32_t capmem_i_out_mux_q0         : 1;  // 8
			uint32_t capmem_i_out_mux_to_inter   : 1;  // 9
			uint32_t capmem_inter_mux_to_pad     : 1;  // 10
			uint32_t capmem_v_out_mux_q3         : 1;  // 11
			uint32_t capmem_v_out_mux_q2         : 1;  // 12
			uint32_t capmem_v_out_mux_q1         : 1;  // 13
			uint32_t capmem_v_out_mux_q0         : 1;  // 14
			uint32_t capmem_v_out_mux_to_inter   : 1;  // 15
			uint32_t /* unused */                : 16; // 16...31
			uint32_t capmem_v_ref_mux_q3         : 1;  // 0  in base + {15, 17}
			uint32_t capmem_v_ref_mux_q2         : 1;  // 1
			uint32_t capmem_v_ref_mux_q1         : 1;  // 2
			uint32_t capmem_v_ref_mux_q0         : 1;  // 3
			uint32_t capmem_v_ref_mux_to_inter   : 1;  // 4
			uint32_t neuron_i_stim_mux_top       : 1;  // 5
			uint32_t neuron_i_stim_mux_bottom    : 1;  // 6
			uint32_t synapse_mux_causal          : 1;  // 7
			uint32_t synapse_mux_acausal         : 1;  // 8
			uint32_t synapse_mux_inhibitory      : 1;  // 9
			uint32_t synapse_mux_excitatory      : 1;  // 10
			uint32_t synapse_mux_to_pad          : 1;  // 11
			uint32_t buffer_to_pad_1             : 1;  // 12
			uint32_t buffer_to_pad_0             : 1;  // 13
			uint32_t debug_to_pad                : 1;  // 14
			uint32_t neuron_i_stim_mux_to_pad    : 1;  // 15
			uint32_t /* unused */                : 16; // 16...31
			// clang-format on
		} m;

		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	PadMultiplexerConfigBitfield() { u.words = {{0, 0}}; }

	PadMultiplexerConfigBitfield(
	    std::array<uint32_t, PadMultiplexerConfig::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0xffff;
		u.words[1] = data[1] & 0xffff;
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, PadMultiplexerConfig::config_size_in_words> PadMultiplexerConfig::encode() const
{
	PadMultiplexerConfigBitfield bitfield;

	bitfield.u.m.cadc_v_ramp_out_mux_q3 =
	    m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];
	bitfield.u.m.cadc_v_ramp_out_mux_q2 =
	    m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.cadc_v_ramp_out_mux_q1 =
	    m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.cadc_v_ramp_out_mux_q0 =
	    m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.cadc_v_ramp_mux_to_pad = m_cadc_v_ramp_mux_to_pad;
	bitfield.u.m.capmem_i_out_mux_q3 =
	    m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];
	bitfield.u.m.capmem_i_out_mux_q2 =
	    m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.capmem_i_out_mux_q1 =
	    m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.capmem_i_out_mux_q0 =
	    m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.capmem_i_out_mux_to_inter = m_capmem_i_out_mux_to_inter;
	bitfield.u.m.capmem_inter_mux_to_pad = m_capmem_inter_mux_to_pad;
	bitfield.u.m.capmem_v_out_mux_q3 =
	    m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];
	bitfield.u.m.capmem_v_out_mux_q2 =
	    m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.capmem_v_out_mux_q1 =
	    m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.capmem_v_out_mux_q0 =
	    m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.capmem_v_out_mux_to_inter = m_capmem_v_out_mux_to_inter;
	bitfield.u.m.capmem_v_ref_mux_q3 =
	    m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)];
	bitfield.u.m.capmem_v_ref_mux_q2 =
	    m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)];
	bitfield.u.m.capmem_v_ref_mux_q1 =
	    m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)];
	bitfield.u.m.capmem_v_ref_mux_q0 =
	    m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)];
	bitfield.u.m.capmem_v_ref_mux_to_inter = m_capmem_v_ref_mux_to_inter;
	bitfield.u.m.neuron_i_stim_mux_top =
	    m_neuron_i_stim_mux[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_i_stim_mux_bottom =
	    m_neuron_i_stim_mux[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_i_stim_mux_to_pad = m_neuron_i_stim_mux_to_pad;
	bitfield.u.m.synapse_mux_causal = m_cadc_debug_causal_to_inter;
	bitfield.u.m.synapse_mux_acausal = m_cadc_debug_acausal_to_inter;
	bitfield.u.m.synapse_mux_inhibitory = m_synin_debug_inhibitory_to_inter;
	bitfield.u.m.synapse_mux_excitatory = m_synin_debug_excitatory_to_inter;
	bitfield.u.m.synapse_mux_to_pad = m_synapse_inter_mux_to_pad;
	bitfield.u.m.buffer_to_pad_1 =
	    m_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)];
	bitfield.u.m.buffer_to_pad_0 =
	    m_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)];
	bitfield.u.m.debug_to_pad = m_debug_to_pad;

	std::array<WordT, PadMultiplexerConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, PadMultiplexerConfig::config_size_in_words>
    PadMultiplexerConfig::encode() const;
template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    PadMultiplexerConfig::config_size_in_words>
PadMultiplexerConfig::encode() const;

template <typename WordT>
void PadMultiplexerConfig::decode(
    std::array<WordT, PadMultiplexerConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, PadMultiplexerConfig::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	PadMultiplexerConfigBitfield bitfield(raw_data);

	m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] =
	    bitfield.u.m.cadc_v_ramp_out_mux_q3;
	m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] =
	    bitfield.u.m.cadc_v_ramp_out_mux_q2;
	m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] =
	    bitfield.u.m.cadc_v_ramp_out_mux_q1;
	m_cadc_v_ramp_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] =
	    bitfield.u.m.cadc_v_ramp_out_mux_q0;
	m_cadc_v_ramp_mux_to_pad = bitfield.u.m.cadc_v_ramp_mux_to_pad;
	m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] =
	    bitfield.u.m.capmem_i_out_mux_q3;
	m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] =
	    bitfield.u.m.capmem_i_out_mux_q2;
	m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] =
	    bitfield.u.m.capmem_i_out_mux_q1;
	m_capmem_i_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] =
	    bitfield.u.m.capmem_i_out_mux_q0;
	m_capmem_i_out_mux_to_inter = bitfield.u.m.capmem_i_out_mux_to_inter;
	m_capmem_inter_mux_to_pad = bitfield.u.m.capmem_inter_mux_to_pad;
	m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] =
	    bitfield.u.m.capmem_v_out_mux_q3;
	m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] =
	    bitfield.u.m.capmem_v_out_mux_q2;
	m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] =
	    bitfield.u.m.capmem_v_out_mux_q1;
	m_capmem_v_out_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] =
	    bitfield.u.m.capmem_v_out_mux_q0;
	m_capmem_v_out_mux_to_inter = bitfield.u.m.capmem_v_out_mux_to_inter;
	m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(3)] =
	    bitfield.u.m.capmem_v_ref_mux_q3;
	m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(2)] =
	    bitfield.u.m.capmem_v_ref_mux_q2;
	m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(1)] =
	    bitfield.u.m.capmem_v_ref_mux_q1;
	m_capmem_v_ref_mux[halco::hicann_dls::vx::CapMemBlockOnDLS(0)] =
	    bitfield.u.m.capmem_v_ref_mux_q0;
	m_capmem_v_ref_mux_to_inter = bitfield.u.m.capmem_v_ref_mux_to_inter;
	m_neuron_i_stim_mux[halco::hicann_dls::vx::HemisphereOnDLS(0)] =
	    bitfield.u.m.neuron_i_stim_mux_top;
	m_neuron_i_stim_mux[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.neuron_i_stim_mux_bottom;
	m_neuron_i_stim_mux_to_pad = bitfield.u.m.neuron_i_stim_mux_to_pad;
	m_cadc_debug_causal_to_inter = bitfield.u.m.synapse_mux_causal;
	m_cadc_debug_acausal_to_inter = bitfield.u.m.synapse_mux_acausal;
	m_synin_debug_inhibitory_to_inter = bitfield.u.m.synapse_mux_inhibitory;
	m_synin_debug_excitatory_to_inter = bitfield.u.m.synapse_mux_excitatory;
	m_synapse_inter_mux_to_pad = bitfield.u.m.synapse_mux_to_pad;
	m_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)] =
	    bitfield.u.m.buffer_to_pad_1;
	m_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)] =
	    bitfield.u.m.buffer_to_pad_0;
	m_debug_to_pad = bitfield.u.m.debug_to_pad;
}

template SYMBOL_VISIBLE void PadMultiplexerConfig::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        PadMultiplexerConfig::config_size_in_words> const& words);

template SYMBOL_VISIBLE void PadMultiplexerConfig::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        PadMultiplexerConfig::config_size_in_words> const& words);

template <class Archive>
void PadMultiplexerConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_cadc_v_ramp_mux));
	ar(CEREAL_NVP(m_cadc_v_ramp_mux_to_pad));
	ar(CEREAL_NVP(m_capmem_i_out_mux));
	ar(CEREAL_NVP(m_capmem_i_out_mux_to_inter));
	ar(CEREAL_NVP(m_capmem_inter_mux_to_pad));
	ar(CEREAL_NVP(m_capmem_v_out_mux));
	ar(CEREAL_NVP(m_capmem_v_out_mux_to_inter));
	ar(CEREAL_NVP(m_capmem_v_ref_mux));
	ar(CEREAL_NVP(m_capmem_v_ref_mux_to_inter));
	ar(CEREAL_NVP(m_neuron_i_stim_mux));
	ar(CEREAL_NVP(m_neuron_i_stim_mux_to_pad));
	ar(CEREAL_NVP(m_cadc_debug_acausal_to_inter));
	ar(CEREAL_NVP(m_cadc_debug_causal_to_inter));
	ar(CEREAL_NVP(m_synin_debug_inhibitory_to_inter));
	ar(CEREAL_NVP(m_synin_debug_excitatory_to_inter));
	ar(CEREAL_NVP(m_synapse_inter_mux_to_pad));
	ar(CEREAL_NVP(m_buffer_to_pad));
	ar(CEREAL_NVP(m_debug_to_pad));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PadMultiplexerConfig)


ReadoutSourceSelection::SourceMultiplexer::SourceMultiplexer() :
    m_debug_plus(false),
    m_debug_minus(false),
    m_current_dac(false),
    m_synin_debug_inhibitory(false),
    m_synin_debug_excitatory(false),
    m_cadc_debug_causal(false),
    m_cadc_debug_acausal(false),
    m_synapse_driver_debug(),
    m_neuron_odd(),
    m_neuron_even()
{}

bool ReadoutSourceSelection::SourceMultiplexer::get_debug_plus() const
{
	return m_debug_plus;
}

void ReadoutSourceSelection::SourceMultiplexer::set_debug_plus(bool const value)
{
	m_debug_plus = value;
}

bool ReadoutSourceSelection::SourceMultiplexer::get_debug_minus() const
{
	return m_debug_minus;
}

void ReadoutSourceSelection::SourceMultiplexer::set_debug_minus(bool const value)
{
	m_debug_minus = value;
}

bool ReadoutSourceSelection::SourceMultiplexer::get_current_dac() const
{
	return m_current_dac;
}

void ReadoutSourceSelection::SourceMultiplexer::set_current_dac(bool const value)
{
	m_current_dac = value;
}

bool ReadoutSourceSelection::SourceMultiplexer::get_synin_debug_inhibitory() const
{
	return m_synin_debug_inhibitory;
}

void ReadoutSourceSelection::SourceMultiplexer::set_synin_debug_inhibitory(bool const value)
{
	m_synin_debug_inhibitory = value;
}

bool ReadoutSourceSelection::SourceMultiplexer::get_synin_debug_excitatory() const
{
	return m_synin_debug_excitatory;
}

void ReadoutSourceSelection::SourceMultiplexer::set_synin_debug_excitatory(bool const value)
{
	m_synin_debug_excitatory = value;
}

bool ReadoutSourceSelection::SourceMultiplexer::get_cadc_debug_causal() const
{
	return m_cadc_debug_causal;
}

void ReadoutSourceSelection::SourceMultiplexer::set_cadc_debug_causal(bool const value)
{
	m_cadc_debug_causal = value;
}

bool ReadoutSourceSelection::SourceMultiplexer::get_cadc_debug_acausal() const
{
	return m_cadc_debug_acausal;
}

void ReadoutSourceSelection::SourceMultiplexer::set_cadc_debug_acausal(bool const value)
{
	m_cadc_debug_acausal = value;
}

ReadoutSourceSelection::SourceMultiplexer::hemisphere_type const&
ReadoutSourceSelection::SourceMultiplexer::get_synapse_driver_debug() const
{
	return m_synapse_driver_debug;
}

void ReadoutSourceSelection::SourceMultiplexer::set_synapse_driver_debug(
    hemisphere_type const& value)
{
	m_synapse_driver_debug = value;
}

ReadoutSourceSelection::SourceMultiplexer::hemisphere_type const&
ReadoutSourceSelection::SourceMultiplexer::get_neuron_odd() const
{
	return m_neuron_odd;
}

void ReadoutSourceSelection::SourceMultiplexer::set_neuron_odd(hemisphere_type const& value)
{
	m_neuron_odd = value;
}

ReadoutSourceSelection::SourceMultiplexer::hemisphere_type const&
ReadoutSourceSelection::SourceMultiplexer::get_neuron_even() const
{
	return m_neuron_even;
}

void ReadoutSourceSelection::SourceMultiplexer::set_neuron_even(hemisphere_type const& value)
{
	m_neuron_even = value;
}

bool ReadoutSourceSelection::SourceMultiplexer::operator==(
    ReadoutSourceSelection::SourceMultiplexer const& other) const
{
	return (
	    m_debug_plus == other.m_debug_plus && m_debug_minus == other.m_debug_minus &&
	    m_synin_debug_inhibitory == other.m_synin_debug_inhibitory &&
	    m_synin_debug_excitatory == other.m_synin_debug_excitatory &&
	    m_cadc_debug_causal == other.m_cadc_debug_causal &&
	    m_cadc_debug_acausal == other.m_cadc_debug_acausal &&
	    m_synapse_driver_debug == other.m_synapse_driver_debug &&
	    m_neuron_odd == other.m_neuron_odd && m_neuron_even == other.m_neuron_even);
}

bool ReadoutSourceSelection::SourceMultiplexer::operator!=(
    ReadoutSourceSelection::SourceMultiplexer const& other) const
{
	return !(*this == other);
}

template <class Archive>
void ReadoutSourceSelection::SourceMultiplexer::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_debug_plus));
	ar(CEREAL_NVP(m_debug_minus));
	ar(CEREAL_NVP(m_current_dac));
	ar(CEREAL_NVP(m_synin_debug_inhibitory));
	ar(CEREAL_NVP(m_synin_debug_excitatory));
	ar(CEREAL_NVP(m_cadc_debug_causal));
	ar(CEREAL_NVP(m_cadc_debug_acausal));
	ar(CEREAL_NVP(m_synapse_driver_debug));
	ar(CEREAL_NVP(m_neuron_odd));
	ar(CEREAL_NVP(m_neuron_even));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ReadoutSourceSelection::SourceMultiplexer)


ReadoutSourceSelection::ReadoutSourceSelection() : m_buffers(), m_enable_buffer_to_pad() {}

ReadoutSourceSelection::SourceMultiplexer ReadoutSourceSelection::get_buffer(
    halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection const& buffer) const
{
	return m_buffers.at(buffer);
}

void ReadoutSourceSelection::set_buffer(
    halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection const& buffer,
    ReadoutSourceSelection::SourceMultiplexer const& value)
{
	m_buffers.at(buffer) = value;
}

ReadoutSourceSelection::source_multiplexer_type const&
ReadoutSourceSelection::get_enable_buffer_to_pad() const
{
	return m_enable_buffer_to_pad;
}

void ReadoutSourceSelection::set_enable_buffer_to_pad(
    ReadoutSourceSelection::source_multiplexer_type const& value)
{
	m_enable_buffer_to_pad = value;
}

bool ReadoutSourceSelection::operator==(ReadoutSourceSelection const& other) const
{
	return (m_buffers == other.m_buffers && m_enable_buffer_to_pad == other.m_enable_buffer_to_pad);
}

bool ReadoutSourceSelection::operator!=(ReadoutSourceSelection const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ReadoutSourceSelection::SourceMultiplexer const& config)
{
	std::stringstream ss;
	ss << "SourceMultiplexer(\n" << std::boolalpha;
	ss << "\tdebug_plus:             \t" << config.m_debug_plus << "\n";
	ss << "\tdebug_minus:            \t" << config.m_debug_minus << "\n";
	ss << "\tcurrent_dac:            \t" << config.m_current_dac << "\n";
	ss << "\tsynin_debug_inhibitory: \t" << config.m_synin_debug_inhibitory << "\n";
	ss << "\tsynin_debug_excitatory: \t" << config.m_synin_debug_excitatory << "\n";
	ss << "\tcadc_debug_causal:      \t" << config.m_cadc_debug_causal << "\n";
	ss << "\tcadc_debug_acausal:     \t" << config.m_cadc_debug_acausal << "\n";
	ss << "\tsynapse_driver_debug:   \t[";
	hate::join(
	    ss, config.m_synapse_driver_debug.begin(), config.m_synapse_driver_debug.end(), ", ");
	ss << "]\n";
	ss << "\tneuron_odd:             \t[";
	hate::join(ss, config.m_neuron_odd.begin(), config.m_neuron_odd.end(), ", ");
	ss << "]\n";
	ss << "\tneuron_even:            \t[";
	hate::join(ss, config.m_neuron_even.begin(), config.m_neuron_even.end(), ", ");
	ss << "]\n)";
	return (os << ss.str());
}

std::ostream& operator<<(std::ostream& os, ReadoutSourceSelection const& config)
{
	std::stringstream ss;
	ss << "ReadoutSourceSelection(\n" << std::boolalpha;
	ss << hate::indent(hate::join_string(config.m_buffers, "\n"), "\t");
	ss << "\n\tenable_buffer_to_pad: [";
	hate::join(
	    ss, config.m_enable_buffer_to_pad.begin(), config.m_enable_buffer_to_pad.end(), ", ");
	ss << "]\n)";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, ReadoutSourceSelection::config_size_in_words>
ReadoutSourceSelection::addresses(coordinate_type const& /* coord */)
{
	return {AddressT(madc_base_address + 12), AddressT(madc_base_address + 13)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, ReadoutSourceSelection::config_size_in_words>
    ReadoutSourceSelection::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    ReadoutSourceSelection::config_size_in_words>
ReadoutSourceSelection::addresses(coordinate_type const& coord);

namespace {

struct ReadoutSourceSelectionBitfield
{
	union
	{
		std::array<uint32_t, ReadoutSourceSelection::config_size_in_words> words;

		struct __attribute__((packed))
		{
			// clang-format off
			uint32_t debug_plus_0           : 1;  // 0  in base + 12
			uint32_t debug_minus_0          : 1;  // 1
			uint32_t idac_i_out_0           : 1;  // 2
			uint32_t synin_debug_inh_0      : 1;  // 3
			uint32_t synin_debug_exc_0      : 1;  // 4
			uint32_t cadc_test_causal_0     : 1;  // 5
			uint32_t cadc_test_acausal_0    : 1;  // 6
			uint32_t syndrv_debug_top_0     : 1;  // 7
			uint32_t syndrv_debug_bottom_0  : 1;  // 8
			uint32_t neuron_top_odd_0       : 1;  // 9
			uint32_t neuron_top_even_0      : 1;  // 10
			uint32_t neuron_bottom_odd_0    : 1;  // 11
			uint32_t neuron_bottom_even_0   : 1;  // 12
			uint32_t /* unused */           : 19; // 13...31
			uint32_t debug_plus_1           : 1;  // 0  in base + 13
			uint32_t debug_minus_1          : 1;  // 1
			uint32_t idac_i_out_1           : 1;  // 2
			uint32_t synin_debug_inh_1      : 1;  // 3
			uint32_t synin_debug_exc_1      : 1;  // 4
			uint32_t cadc_test_causal_1     : 1;  // 5
			uint32_t cadc_test_acausal_1    : 1;  // 6
			uint32_t syndrv_debug_top_1     : 1;  // 7
			uint32_t syndrv_debug_bottom_1  : 1;  // 8
			uint32_t neuron_top_odd_1       : 1;  // 9
			uint32_t neuron_top_even_1      : 1;  // 10
			uint32_t neuron_bottom_odd_1    : 1;  // 11
			uint32_t neuron_bottom_even_1   : 1;  // 12
			uint32_t enable_buffer_to_pad_1 : 1;  // 13
			uint32_t enable_buffer_to_pad_0 : 1;  // 14
			uint32_t /* unused */           : 17; // 15...31
			// clang-format on
		} m;

		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	ReadoutSourceSelectionBitfield() { u.words = {{0, 0}}; }

	ReadoutSourceSelectionBitfield(
	    std::array<uint32_t, ReadoutSourceSelection::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0b0001111111111111;
		u.words[1] = data[1] & 0b0111111111111111;
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, ReadoutSourceSelection::config_size_in_words> ReadoutSourceSelection::encode()
    const
{
	ReadoutSourceSelectionBitfield bitfield;

	bitfield.u.m.debug_plus_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)].m_debug_plus;
	bitfield.u.m.debug_minus_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_debug_minus;
	bitfield.u.m.idac_i_out_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_current_dac;
	bitfield.u.m.synin_debug_inh_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_synin_debug_inhibitory;
	bitfield.u.m.synin_debug_exc_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_synin_debug_excitatory;
	bitfield.u.m.cadc_test_causal_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_cadc_debug_causal;
	bitfield.u.m.cadc_test_acausal_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_cadc_debug_acausal;
	bitfield.u.m.syndrv_debug_top_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.syndrv_debug_bottom_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_top_odd_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_top_even_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_bottom_odd_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_bottom_even_0 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)];

	bitfield.u.m.debug_plus_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)].m_debug_plus;
	bitfield.u.m.debug_minus_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_debug_minus;
	bitfield.u.m.idac_i_out_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_current_dac;
	bitfield.u.m.synin_debug_inh_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_synin_debug_inhibitory;
	bitfield.u.m.synin_debug_exc_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_synin_debug_excitatory;
	bitfield.u.m.cadc_test_causal_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_cadc_debug_causal;
	bitfield.u.m.cadc_test_acausal_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_cadc_debug_acausal;
	bitfield.u.m.syndrv_debug_top_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.syndrv_debug_bottom_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_top_odd_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_top_even_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_bottom_odd_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_bottom_even_1 =
	    m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)];

	bitfield.u.m.enable_buffer_to_pad_0 =
	    m_enable_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)];
	bitfield.u.m.enable_buffer_to_pad_1 =
	    m_enable_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)];

	std::array<WordT, ReadoutSourceSelection::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, ReadoutSourceSelection::config_size_in_words>
    ReadoutSourceSelection::encode() const;
template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    ReadoutSourceSelection::config_size_in_words>
ReadoutSourceSelection::encode() const;

template <typename WordT>
void ReadoutSourceSelection::decode(
    std::array<WordT, ReadoutSourceSelection::config_size_in_words> const& data)
{
	std::array<uint32_t, ReadoutSourceSelection::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	ReadoutSourceSelectionBitfield bitfield(raw_data);

	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)].m_debug_plus =
	    bitfield.u.m.debug_plus_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)].m_debug_minus =
	    bitfield.u.m.debug_minus_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)].m_current_dac =
	    bitfield.u.m.idac_i_out_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_synin_debug_inhibitory = bitfield.u.m.synin_debug_inh_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_synin_debug_excitatory = bitfield.u.m.synin_debug_exc_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_cadc_debug_causal = bitfield.u.m.cadc_test_causal_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_cadc_debug_acausal = bitfield.u.m.cadc_test_acausal_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)] =
	    bitfield.u.m.syndrv_debug_top_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.syndrv_debug_bottom_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_odd_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_even_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)] = bitfield.u.m.neuron_bottom_odd_0;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.neuron_bottom_even_0;

	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)].m_debug_plus =
	    bitfield.u.m.debug_plus_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)].m_debug_minus =
	    bitfield.u.m.debug_minus_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)].m_current_dac =
	    bitfield.u.m.idac_i_out_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_synin_debug_inhibitory = bitfield.u.m.synin_debug_inh_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_synin_debug_excitatory = bitfield.u.m.synin_debug_exc_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_cadc_debug_causal = bitfield.u.m.cadc_test_causal_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_cadc_debug_acausal = bitfield.u.m.cadc_test_acausal_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)] =
	    bitfield.u.m.syndrv_debug_top_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.syndrv_debug_bottom_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_odd_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_even_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)] = bitfield.u.m.neuron_bottom_odd_1;
	m_buffers[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.neuron_bottom_even_1;

	m_enable_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(0)] =
	    bitfield.u.m.enable_buffer_to_pad_0;
	m_enable_buffer_to_pad[halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection(1)] =
	    bitfield.u.m.enable_buffer_to_pad_1;
}

template SYMBOL_VISIBLE void ReadoutSourceSelection::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        ReadoutSourceSelection::config_size_in_words> const& words);

template SYMBOL_VISIBLE void ReadoutSourceSelection::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        ReadoutSourceSelection::config_size_in_words> const& words);

template <class Archive>
void ReadoutSourceSelection::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_buffers));
	ar(CEREAL_NVP(m_enable_buffer_to_pad));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ReadoutSourceSelection)

} // namespace haldls::vx

CEREAL_CLASS_VERSION(haldls::vx::PadMultiplexerConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ReadoutSourceSelection::SourceMultiplexer, 0)
CEREAL_CLASS_VERSION(haldls::vx::ReadoutSourceSelection, 0)
