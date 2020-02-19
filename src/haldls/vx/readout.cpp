#include <cmath>
#include <numeric>
#include <utility>

#include "halco/common/typed_array.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"
#include "haldls/vx/readout.h"

#include "fisch/vx/jtag.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"

namespace haldls::vx {

using halco::common::typed_array;
using namespace halco::hicann_dls::vx;

ReadoutMuxConfig::ReadoutMuxConfig() :
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
    m_differential_to_pad(false)
{}

ReadoutMuxConfig::capmem_quadrant_type const& ReadoutMuxConfig::get_cadc_v_ramp_mux() const
{
	return m_cadc_v_ramp_mux;
}

void ReadoutMuxConfig::set_cadc_v_ramp_mux(capmem_quadrant_type const& value)
{
	m_cadc_v_ramp_mux = value;
}

bool ReadoutMuxConfig::get_cadc_v_ramp_mux_to_pad() const
{
	return m_cadc_v_ramp_mux_to_pad;
}

void ReadoutMuxConfig::set_cadc_v_ramp_mux_to_pad(bool const value)
{
	m_cadc_v_ramp_mux_to_pad = value;
}

ReadoutMuxConfig::capmem_quadrant_type const& ReadoutMuxConfig::get_capmem_i_out_mux() const
{
	return m_capmem_i_out_mux;
}

void ReadoutMuxConfig::set_capmem_i_out_mux(capmem_quadrant_type const& value)
{
	m_capmem_i_out_mux = value;
}

bool ReadoutMuxConfig::get_capmem_i_out_mux_to_capmem_intermediate_mux() const
{
	return m_capmem_i_out_mux_to_inter;
}

void ReadoutMuxConfig::set_capmem_i_out_mux_to_capmem_intermediate_mux(bool const value)
{
	m_capmem_i_out_mux_to_inter = value;
}

bool ReadoutMuxConfig::get_capmem_intermediate_mux_to_pad() const
{
	return m_capmem_inter_mux_to_pad;
}

void ReadoutMuxConfig::set_capmem_intermediate_mux_to_pad(bool const value)
{
	m_capmem_inter_mux_to_pad = value;
}

ReadoutMuxConfig::capmem_quadrant_type const& ReadoutMuxConfig::get_capmem_v_out_mux() const
{
	return m_capmem_v_out_mux;
}

void ReadoutMuxConfig::set_capmem_v_out_mux(capmem_quadrant_type const& value)
{
	m_capmem_v_out_mux = value;
}

bool ReadoutMuxConfig::get_capmem_v_out_mux_to_capmem_intermediate_mux() const
{
	return m_capmem_v_out_mux_to_inter;
}

void ReadoutMuxConfig::set_capmem_v_out_mux_to_capmem_intermediate_mux(bool const value)
{
	m_capmem_v_out_mux_to_inter = value;
}

ReadoutMuxConfig::capmem_quadrant_type const& ReadoutMuxConfig::get_capmem_v_ref_mux() const
{
	return m_capmem_v_ref_mux;
}

void ReadoutMuxConfig::set_capmem_v_ref_mux(capmem_quadrant_type const& value)
{
	m_capmem_v_ref_mux = value;
}

bool ReadoutMuxConfig::get_capmem_v_ref_mux_to_capmem_intermediate_mux() const
{
	return m_capmem_v_ref_mux_to_inter;
}

void ReadoutMuxConfig::set_capmem_v_ref_mux_to_capmem_intermediate_mux(bool const value)
{
	m_capmem_v_ref_mux_to_inter = value;
}

ReadoutMuxConfig::hemisphere_type const& ReadoutMuxConfig::get_neuron_i_stim_mux() const
{
	return m_neuron_i_stim_mux;
}

void ReadoutMuxConfig::set_neuron_i_stim_mux(hemisphere_type const& value)
{
	m_neuron_i_stim_mux = value;
}

bool ReadoutMuxConfig::get_neuron_i_stim_mux_to_pad() const
{
	return m_neuron_i_stim_mux_to_pad;
}

void ReadoutMuxConfig::set_neuron_i_stim_mux_to_pad(bool const value)
{
	m_neuron_i_stim_mux_to_pad = value;
}

bool ReadoutMuxConfig::get_cadc_debug_acausal_to_synapse_intermediate_mux() const
{
	return m_cadc_debug_acausal_to_inter;
}

void ReadoutMuxConfig::set_cadc_debug_acausal_to_synapse_intermediate_mux(bool const value)
{
	m_cadc_debug_acausal_to_inter = value;
}

bool ReadoutMuxConfig::get_cadc_debug_causal_to_synapse_intermediate_mux() const
{
	return m_cadc_debug_causal_to_inter;
}

void ReadoutMuxConfig::set_cadc_debug_causal_to_synapse_intermediate_mux(bool const value)
{
	m_cadc_debug_causal_to_inter = value;
}

bool ReadoutMuxConfig::get_synin_debug_inhibitory_to_synapse_intermediate_mux() const
{
	return m_synin_debug_inhibitory_to_inter;
}

void ReadoutMuxConfig::set_synin_debug_inhibitory_to_synapse_intermediate_mux(bool const value)
{
	m_synin_debug_inhibitory_to_inter = value;
}

bool ReadoutMuxConfig::get_synin_debug_excitatory_to_synapse_intermediate_mux() const
{
	return m_synin_debug_excitatory_to_inter;
}

void ReadoutMuxConfig::set_synin_debug_excitatory_to_synapse_intermediate_mux(bool const value)
{
	m_synin_debug_excitatory_to_inter = value;
}

bool ReadoutMuxConfig::get_synapse_intermediate_mux_to_pad() const
{
	return m_synapse_inter_mux_to_pad;
}

void ReadoutMuxConfig::set_synapse_intermediate_mux_to_pad(bool const value)
{
	m_synapse_inter_mux_to_pad = value;
}

ReadoutMuxConfig::buffer_type const& ReadoutMuxConfig::get_buffer_to_pad() const
{
	return m_buffer_to_pad;
}

void ReadoutMuxConfig::set_buffer_to_pad(buffer_type const& value)
{
	m_buffer_to_pad = value;
}

bool ReadoutMuxConfig::get_differential_to_pad() const
{
	return m_differential_to_pad;
}

void ReadoutMuxConfig::set_differential_to_pad(bool const value)
{
	m_differential_to_pad = value;
}

bool ReadoutMuxConfig::operator==(ReadoutMuxConfig const& other) const
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
	    m_differential_to_pad == other.m_differential_to_pad);
}

bool ReadoutMuxConfig::operator!=(ReadoutMuxConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ReadoutMuxConfig const& config)
{
	return print_words_for_each_backend(os, config);
}

template <typename AddressT>
std::array<AddressT, ReadoutMuxConfig::config_size_in_words> ReadoutMuxConfig::addresses(
    coordinate_type const& coord) const
{
	return {
	    AddressT(madc_base_address + 14 + coord.toEnum() * ReadoutMuxConfig::config_size_in_words),
	    AddressT(madc_base_address + 15 + coord.toEnum() * ReadoutMuxConfig::config_size_in_words)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, ReadoutMuxConfig::config_size_in_words>
    ReadoutMuxConfig::addresses(coordinate_type const& coord) const;

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, ReadoutMuxConfig::config_size_in_words>
    ReadoutMuxConfig::addresses(coordinate_type const& coord) const;

namespace {

struct ReadoutMuxConfigBitfield
{
	union
	{
		std::array<uint32_t, ReadoutMuxConfig::config_size_in_words> words;

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
			uint32_t differential_to_pad         : 1;  // 14
			uint32_t neuron_i_stim_mux_to_pad    : 1;  // 15
			uint32_t /* unused */                : 16; // 16...31
			// clang-format on
		} m;

		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	ReadoutMuxConfigBitfield() { u.words = {{0, 0}}; }

	ReadoutMuxConfigBitfield(std::array<uint32_t, ReadoutMuxConfig::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0xffff;
		u.words[1] = data[1] & 0xffff;
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, ReadoutMuxConfig::config_size_in_words> ReadoutMuxConfig::encode() const
{
	ReadoutMuxConfigBitfield bitfield;

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
	    m_buffer_to_pad[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)];
	bitfield.u.m.buffer_to_pad_0 =
	    m_buffer_to_pad[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)];
	bitfield.u.m.differential_to_pad = m_differential_to_pad;

	std::array<WordT, ReadoutMuxConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, ReadoutMuxConfig::config_size_in_words>
ReadoutMuxConfig::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, ReadoutMuxConfig::config_size_in_words>
    ReadoutMuxConfig::encode() const;

template <typename WordT>
void ReadoutMuxConfig::decode(std::array<WordT, ReadoutMuxConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, ReadoutMuxConfig::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	ReadoutMuxConfigBitfield bitfield(raw_data);

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
	m_buffer_to_pad[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)] =
	    bitfield.u.m.buffer_to_pad_1;
	m_buffer_to_pad[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)] =
	    bitfield.u.m.buffer_to_pad_0;
	m_differential_to_pad = bitfield.u.m.differential_to_pad;
}

template SYMBOL_VISIBLE void ReadoutMuxConfig::decode(
    std::array<fisch::vx::OmnibusChip, ReadoutMuxConfig::config_size_in_words> const& words);

template SYMBOL_VISIBLE void ReadoutMuxConfig::decode(
    std::array<fisch::vx::OmnibusChipOverJTAG, ReadoutMuxConfig::config_size_in_words> const&
        words);

template <class Archive>
void ReadoutMuxConfig::serialize(Archive& ar)
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
	ar(CEREAL_NVP(m_differential_to_pad));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ReadoutMuxConfig)


ReadoutBufferConfigBlock::ReadoutBufferConfig::ReadoutBufferConfig() :
    m_differential_signal(false),
    m_differential_reference(false),
    m_current_dac(false),
    m_synin_debug_inhibitory(false),
    m_synin_debug_excitatory(false),
    m_cadc_debug_causal(false),
    m_cadc_debug_acausal(false),
    m_synapse_driver_debug(),
    m_neuron_odd(),
    m_neuron_even(),
    m_enable_buffer(false)
{}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_differential_signal() const
{
	return m_differential_signal;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_differential_signal(bool const value)
{
	m_differential_signal = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_differential_reference() const
{
	return m_differential_reference;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_differential_reference(bool const value)
{
	m_differential_reference = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_current_dac() const
{
	return m_current_dac;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_current_dac(bool const value)
{
	m_current_dac = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_synin_debug_inhibitory() const
{
	return m_synin_debug_inhibitory;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_synin_debug_inhibitory(bool const value)
{
	m_synin_debug_inhibitory = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_synin_debug_excitatory() const
{
	return m_synin_debug_excitatory;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_synin_debug_excitatory(bool const value)
{
	m_synin_debug_excitatory = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_cadc_debug_causal() const
{
	return m_cadc_debug_causal;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_cadc_debug_causal(bool const value)
{
	m_cadc_debug_causal = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_cadc_debug_acausal() const
{
	return m_cadc_debug_acausal;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_cadc_debug_acausal(bool const value)
{
	m_cadc_debug_acausal = value;
}

ReadoutBufferConfigBlock::ReadoutBufferConfig::hemisphere_type const&
ReadoutBufferConfigBlock::ReadoutBufferConfig::get_synapse_driver_debug() const
{
	return m_synapse_driver_debug;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_synapse_driver_debug(
    hemisphere_type const& value)
{
	m_synapse_driver_debug = value;
}

ReadoutBufferConfigBlock::ReadoutBufferConfig::hemisphere_type const&
ReadoutBufferConfigBlock::ReadoutBufferConfig::get_neuron_odd() const
{
	return m_neuron_odd;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_neuron_odd(hemisphere_type const& value)
{
	m_neuron_odd = value;
}

ReadoutBufferConfigBlock::ReadoutBufferConfig::hemisphere_type const&
ReadoutBufferConfigBlock::ReadoutBufferConfig::get_neuron_even() const
{
	return m_neuron_even;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_neuron_even(hemisphere_type const& value)
{
	m_neuron_even = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::get_enable_buffer() const
{
	return m_enable_buffer;
}

void ReadoutBufferConfigBlock::ReadoutBufferConfig::set_enable_buffer(bool const value)
{
	m_enable_buffer = value;
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::operator==(
    ReadoutBufferConfigBlock::ReadoutBufferConfig const& other) const
{
	return (
	    m_differential_signal == other.m_differential_signal &&
	    m_differential_reference == other.m_differential_reference &&
	    m_synin_debug_inhibitory == other.m_synin_debug_inhibitory &&
	    m_synin_debug_excitatory == other.m_synin_debug_excitatory &&
	    m_cadc_debug_causal == other.m_cadc_debug_causal &&
	    m_cadc_debug_acausal == other.m_cadc_debug_acausal &&
	    m_synapse_driver_debug == other.m_synapse_driver_debug &&
	    m_neuron_odd == other.m_neuron_odd && m_neuron_even == other.m_neuron_even &&
	    m_enable_buffer == other.m_enable_buffer);
}

bool ReadoutBufferConfigBlock::ReadoutBufferConfig::operator!=(
    ReadoutBufferConfigBlock::ReadoutBufferConfig const& other) const
{
	return !(*this == other);
}

template <class Archive>
void ReadoutBufferConfigBlock::ReadoutBufferConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_differential_signal));
	ar(CEREAL_NVP(m_differential_reference));
	ar(CEREAL_NVP(m_current_dac));
	ar(CEREAL_NVP(m_synin_debug_inhibitory));
	ar(CEREAL_NVP(m_synin_debug_excitatory));
	ar(CEREAL_NVP(m_cadc_debug_causal));
	ar(CEREAL_NVP(m_cadc_debug_acausal));
	ar(CEREAL_NVP(m_synapse_driver_debug));
	ar(CEREAL_NVP(m_neuron_odd));
	ar(CEREAL_NVP(m_neuron_even));
	ar(CEREAL_NVP(m_enable_buffer));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ReadoutBufferConfigBlock::ReadoutBufferConfig)


ReadoutBufferConfigBlock::ReadoutBufferConfigBlock() : m_buffers() {}

ReadoutBufferConfigBlock::ReadoutBufferConfig ReadoutBufferConfigBlock::get_buffer(
    halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock const& buffer) const
{
	return m_buffers.at(buffer);
}

void ReadoutBufferConfigBlock::set_buffer(
    halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock const& buffer,
    ReadoutBufferConfigBlock::ReadoutBufferConfig const& value)
{
	m_buffers.at(buffer) = value;
}

bool ReadoutBufferConfigBlock::operator==(ReadoutBufferConfigBlock const& other) const
{
	return m_buffers == other.m_buffers;
}

bool ReadoutBufferConfigBlock::operator!=(ReadoutBufferConfigBlock const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ReadoutBufferConfigBlock const& config)
{
	return print_words_for_each_backend(os, config);
}

template <typename AddressT>
std::array<AddressT, ReadoutBufferConfigBlock::config_size_in_words>
ReadoutBufferConfigBlock::addresses(coordinate_type const& /* coord */) const
{
	return {AddressT(madc_base_address + 12), AddressT(madc_base_address + 13)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipAddress, ReadoutBufferConfigBlock::config_size_in_words>
    ReadoutBufferConfigBlock::addresses(coordinate_type const& coord) const;

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    ReadoutBufferConfigBlock::config_size_in_words>
ReadoutBufferConfigBlock::addresses(coordinate_type const& coord) const;

namespace {

struct ReadoutBufferConfigBlockBitfield
{
	union
	{
		std::array<uint32_t, ReadoutBufferConfigBlock::config_size_in_words> words;

		struct __attribute__((packed))
		{
			// clang-format off
			uint32_t differential_plus_0    : 1;  // 0  in base + 12
			uint32_t differential_minus_0   : 1;  // 1
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
			uint32_t differential_plus_1    : 1;  // 0  in base + 13
			uint32_t differential_minus_1   : 1;  // 1
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
			uint32_t enable_buffer_1        : 1;  // 13
			uint32_t enable_buffer_0        : 1;  // 14
			uint32_t /* unused */           : 17; // 15...31
			// clang-format on
		} m;

		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	ReadoutBufferConfigBlockBitfield() { u.words = {{0, 0}}; }

	ReadoutBufferConfigBlockBitfield(
	    std::array<uint32_t, ReadoutBufferConfigBlock::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0b0001111111111111;
		u.words[1] = data[1] & 0b0111111111111111;
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, ReadoutBufferConfigBlock::config_size_in_words> ReadoutBufferConfigBlock::encode()
    const
{
	ReadoutBufferConfigBlockBitfield bitfield;

	bitfield.u.m.differential_plus_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_differential_signal;
	bitfield.u.m.differential_minus_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_differential_reference;
	bitfield.u.m.idac_i_out_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_current_dac;
	bitfield.u.m.synin_debug_inh_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_synin_debug_inhibitory;
	bitfield.u.m.synin_debug_exc_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_synin_debug_excitatory;
	bitfield.u.m.cadc_test_causal_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_cadc_debug_causal;
	bitfield.u.m.cadc_test_acausal_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_cadc_debug_acausal;
	bitfield.u.m.syndrv_debug_top_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.syndrv_debug_bottom_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_top_odd_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_top_even_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_bottom_odd_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_bottom_even_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)];

	bitfield.u.m.differential_plus_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_differential_signal;
	bitfield.u.m.differential_minus_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_differential_reference;
	bitfield.u.m.idac_i_out_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_current_dac;
	bitfield.u.m.synin_debug_inh_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_synin_debug_inhibitory;
	bitfield.u.m.synin_debug_exc_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_synin_debug_excitatory;
	bitfield.u.m.cadc_test_causal_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_cadc_debug_causal;
	bitfield.u.m.cadc_test_acausal_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_cadc_debug_acausal;
	bitfield.u.m.syndrv_debug_top_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.syndrv_debug_bottom_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_top_odd_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_top_even_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)];
	bitfield.u.m.neuron_bottom_odd_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)];
	bitfield.u.m.neuron_bottom_even_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)];

	bitfield.u.m.enable_buffer_0 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	        .m_enable_buffer;
	bitfield.u.m.enable_buffer_1 =
	    m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	        .m_enable_buffer;

	std::array<WordT, ReadoutBufferConfigBlock::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, ReadoutBufferConfigBlock::config_size_in_words>
    ReadoutBufferConfigBlock::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, ReadoutBufferConfigBlock::config_size_in_words>
    ReadoutBufferConfigBlock::encode() const;

template <typename WordT>
void ReadoutBufferConfigBlock::decode(
    std::array<WordT, ReadoutBufferConfigBlock::config_size_in_words> const& data)
{
	std::array<uint32_t, ReadoutBufferConfigBlock::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	ReadoutBufferConfigBlockBitfield bitfield(raw_data);

	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_differential_signal = bitfield.u.m.differential_plus_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_differential_reference = bitfield.u.m.differential_minus_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_current_dac = bitfield.u.m.idac_i_out_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_synin_debug_inhibitory = bitfield.u.m.synin_debug_inh_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_synin_debug_excitatory = bitfield.u.m.synin_debug_exc_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_cadc_debug_causal = bitfield.u.m.cadc_test_causal_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_cadc_debug_acausal = bitfield.u.m.cadc_test_acausal_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)] =
	    bitfield.u.m.syndrv_debug_top_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.syndrv_debug_bottom_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_odd_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_even_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)] = bitfield.u.m.neuron_bottom_odd_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.neuron_bottom_even_0;

	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_differential_signal = bitfield.u.m.differential_plus_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_differential_reference = bitfield.u.m.differential_minus_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_current_dac = bitfield.u.m.idac_i_out_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_synin_debug_inhibitory = bitfield.u.m.synin_debug_inh_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_synin_debug_excitatory = bitfield.u.m.synin_debug_exc_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_cadc_debug_causal = bitfield.u.m.cadc_test_causal_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_cadc_debug_acausal = bitfield.u.m.cadc_test_acausal_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(0)] =
	    bitfield.u.m.syndrv_debug_top_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_synapse_driver_debug[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.syndrv_debug_bottom_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_odd_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(0)] = bitfield.u.m.neuron_top_even_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_neuron_odd[halco::hicann_dls::vx::HemisphereOnDLS(1)] = bitfield.u.m.neuron_bottom_odd_1;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_neuron_even[halco::hicann_dls::vx::HemisphereOnDLS(1)] =
	    bitfield.u.m.neuron_bottom_even_1;

	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(0)]
	    .m_enable_buffer = bitfield.u.m.enable_buffer_0;
	m_buffers[halco::hicann_dls::vx::ReadoutBufferConfigOnReadoutBufferConfigBlock(1)]
	    .m_enable_buffer = bitfield.u.m.enable_buffer_1;
}

template SYMBOL_VISIBLE void ReadoutBufferConfigBlock::decode(
    std::array<fisch::vx::OmnibusChip, ReadoutBufferConfigBlock::config_size_in_words> const&
        words);

template SYMBOL_VISIBLE void ReadoutBufferConfigBlock::decode(
    std::array<
        fisch::vx::OmnibusChipOverJTAG,
        ReadoutBufferConfigBlock::config_size_in_words> const& words);

template <class Archive>
void ReadoutBufferConfigBlock::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_buffers));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ReadoutBufferConfigBlock)

} // namespace haldls::vx
