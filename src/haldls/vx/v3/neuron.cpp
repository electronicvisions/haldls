#include "haldls/vx/v3/neuron.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/neuron.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls::vx {
NEURON_BACKEND_CONFIG_UNROLL(halco::hicann_dls::vx::v3::Coordinates)
} // namespace haldls::vx

namespace haldls::vx::v3 {

// TODO: Initialize with reasonable default values
NeuronConfig::NeuronConfig() :
    m_en_comp_cond_div(false),
    m_en_comp_cond_mul(false),
    m_connect_soma(false),
    m_connect_membrane_right(false),
    m_en_comp_cond(false),
    m_connect_bottom(false),
    m_connect_somata(false),
    m_en_fire(false),
    m_en_strong_fire(false),
    m_en_thresh_comp(false),
    m_en_synin_inh(false),
    m_en_synin_exc(false),
    m_en_synin_exc_small_cap(true),
    m_en_synin_inh_small_cap(true),
    m_en_synin_exc_high_res(false),
    m_en_synin_inh_high_res(false),
    m_en_byp_inh(false),
    m_en_byp_exc(false),
    m_en_mem_off(false),
    m_invert_current(false),
    m_mem_cap_size(0),
    m_invert_adapt_a(false),
    m_invert_adapt_b(false),
    m_en_adapt(false),
    m_en_exp(false),
    m_en_unbuf_access(false),
    m_en_readout_amp(false),
    m_readout_select(NeuronConfig::ReadoutSource::membrane),
    m_en_readout(false),
    m_en_reset_deg(false),
    m_en_reset_div(false),
    m_en_reset_mul(false),
    m_en_leak_deg(false),
    m_en_leak_div(false),
    m_en_leak_mul(false),
    m_en_pause(false)
{}

namespace {

struct NeuronConfigBitfield
{
	union
	{
		std::array<uint32_t, NeuronConfig::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
			                                                                     // bits ; word
			uint32_t en_reset_deg                                         :  1;  // 0    ; 2
			uint32_t en_reset_div                                         :  1;  // 1    ; 2
			uint32_t en_reset_mul                                         :  1;  // 2    ; 2
			uint32_t en_leak_deg                                          :  1;  // 3    ; 2
			uint32_t en_leak_div                                          :  1;  // 4    ; 2
			uint32_t en_leak_mul                                          :  1;  // 5    ; 2
			uint32_t en_strong_fire                                       :  1;  // 6    ; 2
			uint32_t                                                      : 25;  // 7-31 ; 2

			uint32_t                                                      :  1;  // 0    ; 3
			uint32_t en_exp                                               :  1;  // 1    ; 3
			uint32_t                                                      :  1;  // 2    ; 3
			uint32_t en_unbuf_access                                      :  1;  // 3    ; 3
			uint32_t en_readout_amp                                       :  1;  // 4    ; 3
			uint32_t readout_select                                       :  2;  // 5-6  ; 3
			uint32_t en_readout                                           :  1;  // 7    ; 3
			uint32_t                                                      : 24;  // 8-31 ; 3

			uint32_t invert_adapt_b                                       :  1;  // 0    ; 4
			uint32_t invert_adapt_a                                       :  1;  // 1    ; 4
			uint32_t en_adapt                                             :  1;  // 2    ; 4
			uint32_t                                                      :  1;  // 3    ; 4
			uint32_t en_synin_inh_small_cap                               :  1;  // 4    ; 4
			uint32_t en_synin_exc_small_cap                               :  1;  // 5    ; 4
			uint32_t en_synin_inh_high_res                                :  1;  // 6    ; 4
			uint32_t en_synin_exc_high_res                                :  1;  // 7    ; 4
			uint32_t                                                      : 24;  // 8-31 ; 4

			uint32_t en_mem_off                                           :  1;  // 0    ; 5
			uint32_t                                                      :  1;  // 1    ; 5
			uint32_t mem_cap_size                                         :  6;  // 2-7  ; 5
			uint32_t                                                      : 24;  // 8-31 ; 5

			uint32_t invert_current                                       :  1;  // 0    ; 6
			uint32_t                                                      :  1;  // 1    ; 6
			uint32_t en_fire                                              :  1;  // 2    ; 6
			uint32_t en_thresh_comp                                       :  1;  // 3    ; 6
			uint32_t en_synin_inh                                         :  1;  // 4    ; 6
			uint32_t en_synin_exc                                         :  1;  // 5    ; 6
			uint32_t en_byp_inh                                           :  1;  // 6    ; 6
			uint32_t en_byp_exc                                           :  1;  // 7    ; 6
			uint32_t                                                      : 24;  // 8-31 ; 6

			uint32_t connect_somata                                       :  1;  // 0    ; 7
			uint32_t connect_bottom                                       :  1;  // 1    ; 7
			uint32_t en_comp_cond                                         :  1;  // 2    ; 7
			uint32_t connect_membrane_right                               :  1;  // 3    ; 7
			uint32_t connect_soma                                         :  1;  // 4    ; 7
			uint32_t en_comp_cond_mul                                     :  1;  // 5    ; 7
			uint32_t en_comp_cond_div                                     :  1;  // 6    ; 7
			uint32_t en_pause                                             :  1;  // 7    ; 7
			uint32_t                                                      : 24;  // 8-31 ; 7
		} m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	NeuronConfigBitfield()
	{
		u.words = {{0, 0, 0, 0, 0, 0}};
	}

	NeuronConfigBitfield(std::array<uint32_t, NeuronConfig::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0b0111'1111u;
		u.words[1] = data[1] & 0b1111'1011u;
		u.words[2] = data[2] & 0b1111'0111u;
		u.words[3] = data[3] & 0b1111'1111u;
		u.words[4] = data[4] & 0b1111'1101u;
		u.words[5] = data[5] & 0b1111'1111u;
	}
};

} // anonymous namespace

bool NeuronConfig::get_enable_divide_multicomp_conductance_bias() const
{
	return m_en_comp_cond_div;
}
void NeuronConfig::set_enable_divide_multicomp_conductance_bias(bool const value)
{
	m_en_comp_cond_div = value;
}

bool NeuronConfig::get_enable_multiply_multicomp_conductance_bias() const
{
	return m_en_comp_cond_mul;
}
void NeuronConfig::set_enable_multiply_multicomp_conductance_bias(bool const value)
{
	m_en_comp_cond_mul = value;
}

bool NeuronConfig::get_connect_soma() const
{
	return m_connect_soma;
}
void NeuronConfig::set_connect_soma(bool const value)
{
	m_connect_soma = value;
}

bool NeuronConfig::get_connect_membrane_right() const
{
	return m_connect_membrane_right;
}
void NeuronConfig::set_connect_membrane_right(bool const value)
{
	m_connect_membrane_right = value;
}

bool NeuronConfig::get_enable_multicomp_conductance() const
{
	return m_en_comp_cond;
}
void NeuronConfig::set_enable_multicomp_conductance(bool const value)
{
	m_en_comp_cond = value;
}

bool NeuronConfig::get_connect_bottom() const
{
	return m_connect_bottom;
}
void NeuronConfig::set_connect_bottom(bool const value)
{
	m_connect_bottom = value;
}

bool NeuronConfig::get_connect_soma_right() const
{
	return m_connect_somata;
}
void NeuronConfig::set_connect_soma_right(bool const value)
{
	m_connect_somata = value;
}

bool NeuronConfig::get_enable_fire() const
{
	return m_en_fire;
}
void NeuronConfig::set_enable_fire(bool const value)
{
	m_en_fire = value;
}

bool NeuronConfig::get_enable_strong_fire() const
{
	return m_en_strong_fire;
}

void NeuronConfig::set_enable_strong_fire(bool const value)
{
	m_en_strong_fire = value;
}

bool NeuronConfig::get_enable_threshold_comparator() const
{
	return m_en_thresh_comp;
}
void NeuronConfig::set_enable_threshold_comparator(bool const value)
{
	m_en_thresh_comp = value;
}

bool NeuronConfig::get_enable_synaptic_input_excitatory() const
{
	return m_en_synin_exc;
}

void NeuronConfig::set_enable_synaptic_input_excitatory(bool const value)
{
	m_en_synin_exc = value;
}
bool NeuronConfig::get_enable_synaptic_input_inhibitory() const
{
	return m_en_synin_inh;
}
void NeuronConfig::set_enable_synaptic_input_inhibitory(bool const value)
{
	m_en_synin_inh = value;
}

bool NeuronConfig::get_enable_synaptic_input_excitatory_small_capacitor() const
{
	return m_en_synin_exc_small_cap;
}

void NeuronConfig::set_enable_synaptic_input_excitatory_small_capacitor(bool const value)
{
	m_en_synin_exc_small_cap = value;
}

bool NeuronConfig::get_enable_synaptic_input_inhibitory_small_capacitor() const
{
	return m_en_synin_inh_small_cap;
}

void NeuronConfig::set_enable_synaptic_input_inhibitory_small_capacitor(bool const value)
{
	m_en_synin_inh_small_cap = value;
}

bool NeuronConfig::get_enable_synaptic_input_excitatory_high_resistance() const
{
	return m_en_synin_exc_high_res;
}

void NeuronConfig::set_enable_synaptic_input_excitatory_high_resistance(bool const value)
{
	m_en_synin_exc_high_res = value;
}

bool NeuronConfig::get_enable_synaptic_input_inhibitory_high_resistance() const
{
	return m_en_synin_inh_high_res;
}

void NeuronConfig::set_enable_synaptic_input_inhibitory_high_resistance(bool const value)
{
	m_en_synin_inh_high_res = value;
}

bool NeuronConfig::get_enable_bypass_excitatory() const
{
	return m_en_byp_exc;
}
void NeuronConfig::set_enable_bypass_excitatory(bool const value)
{
	m_en_byp_exc = value;
}

bool NeuronConfig::get_enable_bypass_inhibitory() const
{
	return m_en_byp_inh;
}
void NeuronConfig::set_enable_bypass_inhibitory(bool const value)
{
	m_en_byp_inh = value;
}

bool NeuronConfig::get_enable_membrane_offset() const
{
	return m_en_mem_off;
}
void NeuronConfig::set_enable_membrane_offset(bool const value)
{
	m_en_mem_off = value;
}

bool NeuronConfig::get_invert_membrane_offset() const
{
	return m_invert_current;
}

void NeuronConfig::set_invert_membrane_offset(bool const value)
{
	m_invert_current = value;
}

NeuronConfig::MembraneCapacitorSize NeuronConfig::get_membrane_capacitor_size() const
{
	return m_mem_cap_size;
}
void NeuronConfig::set_membrane_capacitor_size(NeuronConfig::MembraneCapacitorSize const value)
{
	m_mem_cap_size = value;
}

bool NeuronConfig::get_invert_adaptation_a() const
{
	return m_invert_adapt_a;
}
void NeuronConfig::set_invert_adaptation_a(bool const value)
{
	m_invert_adapt_a = value;
}

bool NeuronConfig::get_invert_adaptation_b() const
{
	return m_invert_adapt_b;
}
void NeuronConfig::set_invert_adaptation_b(bool const value)
{
	m_invert_adapt_b = value;
}

bool NeuronConfig::get_enable_adaptation() const
{
	return m_en_adapt;
}
void NeuronConfig::set_enable_adaptation(bool const value)
{
	m_en_adapt = value;
}

bool NeuronConfig::get_enable_exponential() const
{
	return m_en_exp;
}
void NeuronConfig::set_enable_exponential(bool const value)
{
	m_en_exp = value;
}

bool NeuronConfig::get_enable_unbuffered_access() const
{
	return m_en_unbuf_access;
}
void NeuronConfig::set_enable_unbuffered_access(bool const value)
{
	m_en_unbuf_access = value;
}

bool NeuronConfig::get_enable_readout_amplifier() const
{
	return m_en_readout_amp;
}
void NeuronConfig::set_enable_readout_amplifier(bool const value)
{
	m_en_readout_amp = value;
}

NeuronConfig::ReadoutSource NeuronConfig::get_readout_source() const
{
	return m_readout_select;
}
void NeuronConfig::set_readout_source(NeuronConfig::ReadoutSource const value)
{
	m_readout_select = value;
}

bool NeuronConfig::get_enable_readout() const
{
	return m_en_readout;
}
void NeuronConfig::set_enable_readout(bool const value)
{
	m_en_readout = value;
}

bool NeuronConfig::get_enable_reset_degeneration() const
{
	return m_en_reset_deg;
}
void NeuronConfig::set_enable_reset_degeneration(bool const value)
{
	m_en_reset_deg = value;
}

bool NeuronConfig::get_enable_reset_division() const
{
	return m_en_reset_div;
}
void NeuronConfig::set_enable_reset_division(bool const value)
{
	m_en_reset_div = value;
}

bool NeuronConfig::get_enable_reset_multiplication() const
{
	return m_en_reset_mul;
}
void NeuronConfig::set_enable_reset_multiplication(bool const value)
{
	m_en_reset_mul = value;
}

bool NeuronConfig::get_enable_leak_degeneration() const
{
	return m_en_leak_deg;
}
void NeuronConfig::set_enable_leak_degeneration(bool const value)
{
	m_en_leak_deg = value;
}

bool NeuronConfig::get_enable_leak_division() const
{
	return m_en_leak_div;
}
void NeuronConfig::set_enable_leak_division(bool const value)
{
	m_en_leak_div = value;
}

bool NeuronConfig::get_enable_leak_multiplication() const
{
	return m_en_leak_mul;
}
void NeuronConfig::set_enable_leak_multiplication(bool const value)
{
	m_en_leak_mul = value;
}

bool NeuronConfig::get_enable_pause() const
{
	return m_en_pause;
}
void NeuronConfig::set_enable_pause(bool const value)
{
	m_en_pause = value;
}

template <typename AddressT>
std::array<AddressT, NeuronConfig::config_size_in_words> NeuronConfig::addresses(
    NeuronConfig::coordinate_type const& neuron)
{
	assert(neuron.toNeuronConfigBlockOnDLS() < neuron_sram_base_addresses.size());
	auto const base_address = neuron_sram_base_addresses.at(neuron.toNeuronConfigBlockOnDLS());
	auto const neuron_coord = neuron.toNeuronConfigOnNeuronConfigBlock();
	std::array<AddressT, NeuronConfig::config_size_in_words> data;
	size_t i = 0;
	// offset of 2 words in address due to them being unused
	std::generate(data.begin(), data.end(), [&]() {
		return static_cast<AddressT>(base_address + 2 + neuron_coord * 8 + i++);
	});
	return data;
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, NeuronConfig::config_size_in_words>
    NeuronConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, NeuronConfig::config_size_in_words>
    NeuronConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& cell);

template <typename WordT>
std::array<WordT, NeuronConfig::config_size_in_words> NeuronConfig::encode() const
{
	NeuronConfigBitfield bitfield;
	bitfield.u.m.en_comp_cond_div = m_en_comp_cond_div;
	bitfield.u.m.en_comp_cond_mul = m_en_comp_cond_mul;
	bitfield.u.m.connect_soma = m_connect_soma;
	bitfield.u.m.connect_membrane_right = m_connect_membrane_right;
	bitfield.u.m.en_comp_cond = m_en_comp_cond;
	bitfield.u.m.connect_bottom = m_connect_bottom;
	bitfield.u.m.connect_somata = m_connect_somata;
	bitfield.u.m.en_fire = m_en_fire;
	bitfield.u.m.en_strong_fire = m_en_strong_fire;
	bitfield.u.m.en_thresh_comp = m_en_thresh_comp;
	bitfield.u.m.en_synin_inh = m_en_synin_inh;
	bitfield.u.m.en_synin_exc = m_en_synin_exc;
	bitfield.u.m.en_synin_inh_small_cap = m_en_synin_inh_small_cap;
	bitfield.u.m.en_synin_exc_small_cap = m_en_synin_exc_small_cap;
	bitfield.u.m.en_synin_inh_high_res = m_en_synin_inh_high_res;
	bitfield.u.m.en_synin_exc_high_res = m_en_synin_exc_high_res;
	bitfield.u.m.en_byp_inh = m_en_byp_inh;
	bitfield.u.m.en_byp_exc = m_en_byp_exc;
	bitfield.u.m.en_mem_off = m_en_mem_off;
	bitfield.u.m.invert_current = m_invert_current;
	bitfield.u.m.mem_cap_size = static_cast<uint32_t>(m_mem_cap_size);
	bitfield.u.m.invert_adapt_a = m_invert_adapt_a;
	bitfield.u.m.invert_adapt_b = m_invert_adapt_b;
	bitfield.u.m.en_adapt = m_en_adapt;
	bitfield.u.m.en_exp = m_en_exp;
	bitfield.u.m.en_unbuf_access = m_en_unbuf_access;
	bitfield.u.m.en_readout_amp = m_en_readout_amp;
	bitfield.u.m.readout_select = static_cast<uint32_t>(m_readout_select);
	bitfield.u.m.en_readout = m_en_readout;
	bitfield.u.m.en_reset_deg = m_en_reset_deg;
	bitfield.u.m.en_reset_div = m_en_reset_div;
	bitfield.u.m.en_reset_mul = m_en_reset_mul;
	bitfield.u.m.en_leak_deg = m_en_leak_deg;
	bitfield.u.m.en_leak_div = m_en_leak_div;
	bitfield.u.m.en_leak_mul = m_en_leak_mul;
	bitfield.u.m.en_pause = m_en_pause;
	std::array<WordT, NeuronConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronConfig::config_size_in_words>
    NeuronConfig::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, NeuronConfig::config_size_in_words>
NeuronConfig::encode<fisch::vx::Omnibus>() const;

template <typename WordT>
void NeuronConfig::decode(std::array<WordT, NeuronConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, NeuronConfig::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	NeuronConfigBitfield bitfield(raw_data);
	m_en_comp_cond_div = bitfield.u.m.en_comp_cond_div;
	m_en_comp_cond_mul = bitfield.u.m.en_comp_cond_mul;
	m_connect_soma = bitfield.u.m.connect_soma;
	m_connect_membrane_right = bitfield.u.m.connect_membrane_right;
	m_en_comp_cond = bitfield.u.m.en_comp_cond;
	m_connect_bottom = bitfield.u.m.connect_bottom;
	m_connect_somata = bitfield.u.m.connect_somata;
	m_en_fire = bitfield.u.m.en_fire;
	m_en_strong_fire = bitfield.u.m.en_strong_fire;
	m_en_thresh_comp = bitfield.u.m.en_thresh_comp;
	m_en_synin_inh = bitfield.u.m.en_synin_inh;
	m_en_synin_exc = bitfield.u.m.en_synin_exc;
	m_en_synin_inh_small_cap = bitfield.u.m.en_synin_inh_small_cap;
	m_en_synin_exc_small_cap = bitfield.u.m.en_synin_exc_small_cap;
	m_en_synin_inh_high_res = bitfield.u.m.en_synin_inh_high_res;
	m_en_synin_exc_high_res = bitfield.u.m.en_synin_exc_high_res;
	m_en_byp_inh = bitfield.u.m.en_byp_inh;
	m_en_byp_exc = bitfield.u.m.en_byp_exc;
	m_en_mem_off = bitfield.u.m.en_mem_off;
	m_invert_current = bitfield.u.m.invert_current;
	m_mem_cap_size = MembraneCapacitorSize(bitfield.u.m.mem_cap_size);
	m_invert_adapt_a = bitfield.u.m.invert_adapt_a;
	m_invert_adapt_b = bitfield.u.m.invert_adapt_b;
	m_en_adapt = bitfield.u.m.en_adapt;
	m_en_exp = bitfield.u.m.en_exp;
	m_en_unbuf_access = bitfield.u.m.en_unbuf_access;
	m_en_readout_amp = bitfield.u.m.en_readout_amp;
	m_readout_select = ReadoutSource(bitfield.u.m.readout_select);
	m_en_readout = bitfield.u.m.en_readout;
	m_en_reset_deg = bitfield.u.m.en_reset_deg;
	m_en_reset_div = bitfield.u.m.en_reset_div;
	m_en_reset_mul = bitfield.u.m.en_reset_mul;
	m_en_leak_deg = bitfield.u.m.en_leak_deg;
	m_en_leak_div = bitfield.u.m.en_leak_div;
	m_en_leak_mul = bitfield.u.m.en_leak_mul;
	m_en_pause = bitfield.u.m.en_pause;
}

template SYMBOL_VISIBLE void NeuronConfig::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void NeuronConfig::decode<fisch::vx::Omnibus>(
    std::array<fisch::vx::Omnibus, NeuronConfig::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, NeuronConfig::ReadoutSource const& config)
{
	switch (config) {
		case NeuronConfig::ReadoutSource::membrane: {
			os << "membrane";
			break;
		}
		case NeuronConfig::ReadoutSource::exc_synin: {
			os << "exc_synin";
			break;
		}
		case NeuronConfig::ReadoutSource::inh_synin: {
			os << "inh_synin";
			break;
		}
		case NeuronConfig::ReadoutSource::adaptation: {
			os << "adaptation";
			break;
		}
		default: {
			throw std::logic_error("Unknown ReadoutSource.");
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, NeuronConfig const& config)
{
	std::stringstream ss;
	ss << "NeuronConfig(\n" << std::boolalpha;
	// clang-format off
	ss << "\tenable_divide_multicomp_conductance_bias:         \t" << config.m_en_comp_cond_div << "\n"
	   << "\tenable_multiply_multicomp_conductance_bias:       \t" << config.m_en_comp_cond_mul << "\n"
	   << "\tconnect_soma:                                     \t" << config.m_connect_soma << "\n"
	   << "\tconnect_membrane_right:                           \t" << config.m_connect_membrane_right << "\n"
	   << "\tenable_multicomp_conductance:                     \t" << config.m_en_comp_cond << "\n"
	   << "\tconnect_bottom:                                   \t" << config.m_connect_bottom << "\n"
	   << "\tconnect_somata:                                   \t" << config.m_connect_somata << "\n"
	   << "\tenable_fire:                                      \t" << config.m_en_fire << "\n"
	   << "\tenable_strong_fire:                               \t" << config.m_en_strong_fire << "\n"
	   << "\tenable_threshold_comparator:                      \t" << config.m_en_thresh_comp << "\n"
	   << "\tenable_synaptic_input_inhibitory:                 \t" << config.m_en_synin_inh << "\n"
	   << "\tenable_synaptic_input_excitatory:                 \t" << config.m_en_synin_exc << "\n"
	   << "\tenable_synaptic_input_inhibitory_small_capacitor: \t" << config.m_en_synin_inh_small_cap << "\n"
	   << "\tenable_synaptic_input_excitatory_small_capacitor: \t" << config.m_en_synin_exc_small_cap << "\n"
	   << "\tenable_synaptic_input_inhibitory_high_resistance: \t" << config.m_en_synin_inh_high_res << "\n"
	   << "\tenable_synaptic_input_excitatory_high_resistance: \t" << config.m_en_synin_exc_high_res << "\n"
	   << "\tenable_bypass_inhibitory:                         \t" << config.m_en_byp_inh << "\n"
	   << "\tenable_bypass_excitatory:                         \t" << config.m_en_byp_exc << "\n"
	   << "\tenable_membrane_offset:                           \t" << config.m_en_mem_off << "\n"
	   << "\tinvert_membrane_offset:                           \t" << config.m_invert_current << "\n"
	   << "\tmembrane_capacitor_size:                          \t" << config.m_mem_cap_size << "\n"
	   << "\tinvert_adaptation_a:                              \t" << config.m_invert_adapt_a << "\n"
	   << "\tinvert_adaptation_b:                              \t" << config.m_invert_adapt_b << "\n"
	   << "\tenable_adaptation:                                \t" << config.m_en_adapt << "\n"
	   << "\tenable_exponential:                               \t" << config.m_en_exp << "\n"
	   << "\tenable_unbuffered_access:                         \t" << config.m_en_unbuf_access << "\n"
	   << "\tenable_readout_amplifier:                         \t" << config.m_en_readout_amp << "\n"
	   << "\treadout_source                                    \t" << config.m_readout_select << "\n"
	   << "\tenable_readout                                    \t" << config.m_en_readout << "\n"
	   << "\tenable_reset_degeneration:                        \t" << config.m_en_reset_deg << "\n"
	   << "\tenable_reset_division:                            \t" << config.m_en_reset_div << "\n"
	   << "\tenable_reset_multiplication:                      \t" << config.m_en_reset_mul << "\n"
	   << "\tenable_leak_degeneration:                         \t" << config.m_en_leak_deg << "\n"
	   << "\tenable_leak_division:                             \t" << config.m_en_leak_div << "\n"
	   << "\tenable_leak_multiplication:                       \t" << config.m_en_leak_mul << "\n"
	   << "\tenable_pause:                                     \t" << config.m_en_pause << "\n)";
	// clang-format on
	return (os << ss.str());
}

bool NeuronConfig::operator==(NeuronConfig const& other) const
{
	return (
	    m_en_comp_cond_div == other.get_enable_divide_multicomp_conductance_bias() &&
	    m_en_comp_cond_mul == other.get_enable_multiply_multicomp_conductance_bias() &&
	    m_connect_soma == other.get_connect_soma() &&
	    m_connect_membrane_right == other.get_connect_membrane_right() &&
	    m_en_comp_cond == other.get_enable_multicomp_conductance() &&
	    m_connect_bottom == other.get_connect_bottom() &&
	    m_connect_somata == other.get_connect_soma_right() &&
	    m_en_fire == other.get_enable_fire() &&
	    m_en_strong_fire == other.get_enable_strong_fire() &&
	    m_en_thresh_comp == other.get_enable_threshold_comparator() &&
	    m_en_synin_inh == other.get_enable_synaptic_input_inhibitory() &&
	    m_en_synin_exc == other.get_enable_synaptic_input_excitatory() &&
	    m_en_synin_inh_small_cap == other.get_enable_synaptic_input_inhibitory_small_capacitor() &&
	    m_en_synin_exc_small_cap == other.get_enable_synaptic_input_excitatory_small_capacitor() &&
	    m_en_synin_inh_high_res == other.get_enable_synaptic_input_inhibitory_high_resistance() &&
	    m_en_synin_exc_high_res == other.get_enable_synaptic_input_excitatory_high_resistance() &&
	    m_en_byp_inh == other.get_enable_bypass_inhibitory() &&
	    m_en_byp_exc == other.get_enable_bypass_excitatory() &&
	    m_en_mem_off == other.get_enable_membrane_offset() &&
	    m_invert_current == other.get_invert_membrane_offset() &&
	    m_mem_cap_size == other.get_membrane_capacitor_size() &&
	    m_invert_adapt_a == other.get_invert_adaptation_a() &&
	    m_invert_adapt_b == other.get_invert_adaptation_b() &&
	    m_en_adapt == other.get_enable_adaptation() && m_en_exp == other.get_enable_exponential() &&
	    m_en_unbuf_access == other.get_enable_unbuffered_access() &&
	    m_en_readout_amp == other.get_enable_readout_amplifier() &&
	    m_readout_select == other.get_readout_source() &&
	    m_en_readout == other.get_enable_readout() &&
	    m_en_reset_deg == other.get_enable_reset_degeneration() &&
	    m_en_reset_div == other.get_enable_reset_division() &&
	    m_en_reset_mul == other.get_enable_reset_multiplication() &&
	    m_en_leak_deg == other.get_enable_leak_degeneration() &&
	    m_en_leak_div == other.get_enable_leak_division() &&
	    m_en_leak_mul == other.get_enable_leak_multiplication() &&
	    m_en_pause == other.get_enable_pause());
}

bool NeuronConfig::operator!=(NeuronConfig const& other) const
{
	return !(*this == other);
}

template <class Archive>
void NeuronConfig::serialize(Archive& ar, std::uint32_t const version)
{
	ar(CEREAL_NVP(m_en_comp_cond_div));
	ar(CEREAL_NVP(m_en_comp_cond_mul));
	ar(CEREAL_NVP(m_connect_soma));
	ar(CEREAL_NVP(m_connect_membrane_right));
	ar(CEREAL_NVP(m_en_comp_cond));
	ar(CEREAL_NVP(m_connect_bottom));
	ar(CEREAL_NVP(m_connect_somata));
	ar(CEREAL_NVP(m_en_fire));
	ar(CEREAL_NVP(m_en_strong_fire));
	ar(CEREAL_NVP(m_en_thresh_comp));
	ar(CEREAL_NVP(m_en_synin_inh));
	ar(CEREAL_NVP(m_en_synin_exc));
	ar(CEREAL_NVP(m_en_synin_inh_small_cap));
	ar(CEREAL_NVP(m_en_synin_exc_small_cap));
	ar(CEREAL_NVP(m_en_synin_inh_high_res));
	ar(CEREAL_NVP(m_en_synin_exc_high_res));
	ar(CEREAL_NVP(m_en_byp_inh));
	ar(CEREAL_NVP(m_en_byp_exc));
	ar(CEREAL_NVP(m_en_mem_off));
	ar(CEREAL_NVP(m_invert_current));
	if (version < 1) {
		bool m_en_cap_merge = false;
		ar(CEREAL_NVP(m_en_cap_merge));
	}
	ar(CEREAL_NVP(m_mem_cap_size));
	ar(CEREAL_NVP(m_invert_adapt_a));
	ar(CEREAL_NVP(m_invert_adapt_b));
	ar(CEREAL_NVP(m_en_adapt));
	ar(CEREAL_NVP(m_en_exp));
	if (version < 2) {
		bool m_en_read_vw = false;
		ar(CEREAL_NVP(m_en_read_vw));
	}
	ar(CEREAL_NVP(m_en_unbuf_access));
	ar(CEREAL_NVP(m_en_readout_amp));
	ar(CEREAL_NVP(m_readout_select));
	ar(CEREAL_NVP(m_en_readout));
	ar(CEREAL_NVP(m_en_reset_deg));
	ar(CEREAL_NVP(m_en_reset_div));
	ar(CEREAL_NVP(m_en_reset_mul));
	ar(CEREAL_NVP(m_en_leak_deg));
	ar(CEREAL_NVP(m_en_leak_div));
	ar(CEREAL_NVP(m_en_leak_mul));
	ar(CEREAL_NVP(m_en_pause));
}

NeuronResetQuad::NeuronResetQuad() {}

template <typename AddressT>
std::array<AddressT, NeuronResetQuad::read_config_size_in_words> NeuronResetQuad::read_addresses(
    NeuronResetQuad::coordinate_type const& /* neuron */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    NeuronResetQuad::read_config_size_in_words>
NeuronResetQuad::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, NeuronResetQuad::read_config_size_in_words>
    NeuronResetQuad::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename AddressT>
std::array<AddressT, NeuronResetQuad::write_config_size_in_words> NeuronResetQuad::write_addresses(
    NeuronResetQuad::coordinate_type const& neuron)
{
	auto const base_address = correlation_reset_base_addresses.at(neuron.toSynramOnDLS());
	int row_offset = halco::hicann_dls::vx::v3::SynapseRowOnSynram::size *
	                 halco::hicann_dls::vx::v3::SynapseQuadColumnOnDLS::size;
	int quad_offset = neuron.toSynapseQuadColumnOnDLS();
	return {AddressT(base_address + row_offset + quad_offset)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    NeuronResetQuad::write_config_size_in_words>
NeuronResetQuad::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, NeuronResetQuad::write_config_size_in_words>
    NeuronResetQuad::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename WordT>
std::array<WordT, NeuronResetQuad::write_config_size_in_words> NeuronResetQuad::encode() const
{
	// Value does not matter
	return {WordT()};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronResetQuad::write_config_size_in_words>
    NeuronResetQuad::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, NeuronResetQuad::write_config_size_in_words>
NeuronResetQuad::encode<fisch::vx::Omnibus>() const;

template <typename WordT>
void NeuronResetQuad::decode(std::array<WordT, NeuronResetQuad::read_config_size_in_words> const&)
{}

template SYMBOL_VISIBLE void NeuronResetQuad::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronResetQuad::read_config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void NeuronResetQuad::decode<fisch::vx::Omnibus>(
    std::array<fisch::vx::Omnibus, NeuronResetQuad::read_config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, NeuronResetQuad const&)
{
	os << "NeuronResetQuad()";
	return os;
}

bool NeuronResetQuad::operator==(NeuronResetQuad const&) const
{
	return true;
}

bool NeuronResetQuad::operator!=(NeuronResetQuad const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void NeuronResetQuad::serialize(Archive&, std::uint32_t const)
{}

} // namespace haldls::vx::v3

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v3::NeuronConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v3::NeuronResetQuad)
CEREAL_CLASS_VERSION(haldls::vx::v3::NeuronBackendConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::v3::NeuronConfig, 2)
CEREAL_CLASS_VERSION(haldls::vx::v3::NeuronResetQuad, 0)
