#include <string>

#include "haldls/vx/neuron.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

#include "fisch/vx/jtag.h"

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace vx {

// TODO: Initialize with reasonable default values
NeuronBackendConfig::NeuronBackendConfig() :
    m_address_out(),
    m_reset_holdoff(),
    m_refractory_time(),
    m_post_overwrite(false),
    m_select_input_clock(),
    m_en_adapt_pulse(false),
    m_en_baesian_extension(false),
    m_en_neuron_slave(false),
    m_connect_fire_bottom(false),
    m_connect_fire_from_right(false),
    m_connect_fire_to_right(false),
    m_en_spike_out(false),
    m_en_neuron_master(false),
    m_en_0_baesian(false),
    m_en_1_baesian(false)
{}

void NeuronBackendConfig::set_address_out(AddressOut const val)
{
	m_address_out = val;
}

void NeuronBackendConfig::set_reset_holdoff(ResetHoldoff const val)
{
	m_reset_holdoff = val;
}

void NeuronBackendConfig::set_refractory_time(RefractoryTime const val)
{
	m_refractory_time = val;
}

void NeuronBackendConfig::set_post_overwrite(bool const val)
{
	m_post_overwrite = val;
}

void NeuronBackendConfig::set_select_input_clock(InputClock const val)
{
	m_select_input_clock = val;
}

void NeuronBackendConfig::set_enable_adaptation_pulse(bool const val)
{
	m_en_adapt_pulse = val;
}

void NeuronBackendConfig::set_enable_bayesian_extension(bool const val)
{
	m_en_baesian_extension = val;
}

void NeuronBackendConfig::set_enable_neuron_slave(bool const val)
{
	m_en_neuron_slave = val;
}

void NeuronBackendConfig::set_connect_fire_bottom(bool const val)
{
	m_connect_fire_bottom = val;
}

void NeuronBackendConfig::set_connect_fire_from_right(bool const val)
{
	m_connect_fire_from_right = val;
}

void NeuronBackendConfig::set_connect_fire_to_right(bool const val)
{
	m_connect_fire_to_right = val;
}

void NeuronBackendConfig::set_enable_spike_out(bool const val)
{
	m_en_spike_out = val;
}

void NeuronBackendConfig::set_enable_neuron_master(bool const val)
{
	m_en_neuron_master = val;
}

void NeuronBackendConfig::set_enable_bayesian_0(bool const val)
{
	m_en_0_baesian = val;
}

void NeuronBackendConfig::set_enable_bayesian_1(bool const val)
{
	m_en_1_baesian = val;
}

NeuronBackendConfig::AddressOut NeuronBackendConfig::get_address_out() const
{
	return m_address_out;
}

NeuronBackendConfig::ResetHoldoff NeuronBackendConfig::get_reset_holdoff() const
{
	return m_reset_holdoff;
}

NeuronBackendConfig::RefractoryTime NeuronBackendConfig::get_refractory_time() const
{
	return m_refractory_time;
}

bool NeuronBackendConfig::get_post_overwrite() const
{
	return m_post_overwrite;
}

NeuronBackendConfig::InputClock NeuronBackendConfig::get_select_input_clock() const
{
	return m_select_input_clock;
}

bool NeuronBackendConfig::get_enable_adaptation_pulse() const
{
	return m_en_adapt_pulse;
}

bool NeuronBackendConfig::get_enable_bayesian_extension() const
{
	return m_en_baesian_extension;
}

bool NeuronBackendConfig::get_enable_neuron_slave() const
{
	return m_en_neuron_slave;
}

bool NeuronBackendConfig::get_connect_fire_bottom() const
{
	return m_connect_fire_bottom;
}

bool NeuronBackendConfig::get_connect_fire_from_right() const
{
	return m_connect_fire_from_right;
}

bool NeuronBackendConfig::get_connect_fire_to_right() const
{
	return m_connect_fire_to_right;
}

bool NeuronBackendConfig::get_enable_spike_out() const
{
	return m_en_spike_out;
}

bool NeuronBackendConfig::get_enable_neuron_master() const
{
	return m_en_neuron_master;
}

bool NeuronBackendConfig::get_enable_bayesian_0() const
{
	return m_en_0_baesian;
}

bool NeuronBackendConfig::get_enable_bayesian_1() const
{
	return m_en_1_baesian;
}

struct NeuronBackendConfig::NeuronBackendConfigBitfield
{
	union
	{
		std::array<uint32_t, NeuronBackendConfig::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
		                                                                     // bits ; word
		uint32_t address_out_lsbs                                     :  6;  // 0-5  ; 0
		uint32_t reset_holdoff_1                                      :  2;  // 6-7  ; 0
		uint32_t                                                      : 24;  // 8-31 ; 0

		uint32_t refractory_time_1                                    :  4;  // 0-3  ; 1
		uint32_t address_out_msbs                                     :  2;  // 4-5  ; 1
		uint32_t reset_holdoff_2                                      :  2;  // 6-7  ; 1
		uint32_t                                                      : 24;  // 8-31 ; 1

		uint32_t post_overwrite                                       :  1;  // 0    ; 2
		uint32_t select_input_clock                                   :  1;  // 1    ; 2
		uint32_t refractory_time_2                                    :  4;  // 2-5  ; 2
		uint32_t en_adapt_pulse                                       :  1;  // 6    ; 2
		uint32_t en_baesian_extension                                 :  1;  // 7    ; 2
		uint32_t                                                      : 24;  // 8-31 ; 2

		uint32_t en_neuron_slave                                      :  1;  // 0    ; 3
		uint32_t connect_fire_bottom                                  :  1;  // 1    ; 3
		uint32_t connect_fire_from_right                              :  1;  // 2    ; 3
		uint32_t connect_fire_to_right                                :  1;  // 3    ; 3
		uint32_t en_spike_out                                         :  1;  // 4    ; 3
		uint32_t en_neuron_master                                     :  1;  // 5    ; 3
		uint32_t en_0_baesian                                         :  1;  // 6    ; 3
		uint32_t en_1_baesian                                         :  1;  // 7    ; 3
		uint32_t                                                      : 24;  // 8-31 ; 3
		} m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	NeuronBackendConfigBitfield() { u.words = {{0, 0, 0, 0}}; }

	NeuronBackendConfigBitfield(
	    std::array<uint32_t, NeuronBackendConfig::config_size_in_words> data)
	{
		u.words[0] = data[0];
		u.words[1] = data[1];
		u.words[2] = data[2];
		u.words[3] = data[3];
	}
};

template <typename AddressT>
std::array<AddressT, NeuronBackendConfig::config_size_in_words> NeuronBackendConfig::addresses(
    NeuronBackendConfig::coordinate_type const& neuron) const
{
	auto base_address = neuron_backend_left_sram_base_address;
	if (neuron.toNeuronBackendConfigBlockOnDLS() == 1) {
		base_address = neuron_backend_right_sram_base_address;
	}
	auto const neuron_coord = neuron.toNeuronBackendConfigOnNeuronBackendConfigBlock();
	std::array<AddressT, NeuronBackendConfig::config_size_in_words> data;
	size_t i = 0;
	std::generate(data.begin(), data.end(), [&]() {
		return static_cast<AddressT>(
		    base_address + neuron_coord * NeuronBackendConfig::config_size_in_words + i++);
	});
	return data;
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    NeuronBackendConfig::config_size_in_words>
NeuronBackendConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, NeuronBackendConfig::config_size_in_words>
    NeuronBackendConfig::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& cell) const;

NeuronBackendConfig::NeuronBackendConfigBitfield NeuronBackendConfig::to_bitfield() const
{
	NeuronBackendConfig::NeuronBackendConfigBitfield bitfield;
	bitfield.u.m.address_out_lsbs = (~m_address_out & 0b00111111); // bits are inverted
	bitfield.u.m.reset_holdoff_1 = m_reset_holdoff & 0b0011;
	bitfield.u.m.refractory_time_1 = (~m_refractory_time & 0b11110000) >> 4;
	bitfield.u.m.address_out_msbs = (~m_address_out & 0b11000000) >> 6; // bits are inverted
	bitfield.u.m.reset_holdoff_2 = (m_reset_holdoff & 0b1100) >> 2;
	bitfield.u.m.post_overwrite = m_post_overwrite;
	bitfield.u.m.select_input_clock = static_cast<uint32_t>(m_select_input_clock);
	bitfield.u.m.refractory_time_2 = (~m_refractory_time & 0b00001111);
	bitfield.u.m.en_adapt_pulse = m_en_adapt_pulse;
	bitfield.u.m.en_baesian_extension = m_en_baesian_extension;
	bitfield.u.m.en_neuron_slave = m_en_neuron_slave;
	bitfield.u.m.connect_fire_bottom = m_connect_fire_bottom;
	bitfield.u.m.connect_fire_from_right = m_connect_fire_from_right;
	bitfield.u.m.connect_fire_to_right = m_connect_fire_to_right;
	bitfield.u.m.en_spike_out = m_en_spike_out;
	bitfield.u.m.en_neuron_master = m_en_neuron_master;
	bitfield.u.m.en_0_baesian = m_en_0_baesian;
	bitfield.u.m.en_1_baesian = m_en_1_baesian;
	return bitfield;
}

void NeuronBackendConfig::from_bitfield(
    NeuronBackendConfig::NeuronBackendConfigBitfield const& bitfield)
{
	// bits of address out are inverted
	m_address_out = NeuronBackendConfig::AddressOut(
	    ((~(bitfield.u.m.address_out_lsbs) & 0b111111) |
	     (~(bitfield.u.m.address_out_msbs << 6) & 0b11000000)) &
	    0xff);
	m_reset_holdoff = NeuronBackendConfig::ResetHoldoff(
	    bitfield.u.m.reset_holdoff_1 | (bitfield.u.m.reset_holdoff_2 << 2));
	m_refractory_time = NeuronBackendConfig::RefractoryTime(
	    ((~(bitfield.u.m.refractory_time_2) & 0xf) |
	     (~(bitfield.u.m.refractory_time_1 << 4) & 0xf0)) &
	    0xff);
	m_post_overwrite = bitfield.u.m.post_overwrite;
	m_select_input_clock = NeuronBackendConfig::InputClock(bitfield.u.m.select_input_clock);
	m_en_adapt_pulse = bitfield.u.m.en_adapt_pulse;
	m_en_baesian_extension = bitfield.u.m.en_baesian_extension;
	m_en_neuron_slave = bitfield.u.m.en_neuron_slave;
	m_connect_fire_bottom = bitfield.u.m.connect_fire_bottom;
	m_connect_fire_from_right = bitfield.u.m.connect_fire_from_right;
	m_connect_fire_to_right = bitfield.u.m.connect_fire_to_right;
	m_en_spike_out = bitfield.u.m.en_spike_out;
	m_en_neuron_master = bitfield.u.m.en_neuron_master;
	m_en_0_baesian = bitfield.u.m.en_0_baesian;
	m_en_1_baesian = bitfield.u.m.en_1_baesian;
}

template <typename WordT>
std::array<WordT, NeuronBackendConfig::config_size_in_words> NeuronBackendConfig::encode() const
{
	auto bitfield = to_bitfield();
	std::array<WordT, NeuronBackendConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronBackendConfig::config_size_in_words>
    NeuronBackendConfig::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChip, NeuronBackendConfig::config_size_in_words>
    NeuronBackendConfig::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void NeuronBackendConfig::decode(
    std::array<WordT, NeuronBackendConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, NeuronBackendConfig::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	NeuronBackendConfigBitfield bitfield(raw_data);
	from_bitfield(bitfield);
}

template SYMBOL_VISIBLE void NeuronBackendConfig::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronBackendConfig::config_size_in_words> const&
        data);

template SYMBOL_VISIBLE void NeuronBackendConfig::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, NeuronBackendConfig::config_size_in_words> const& data);

bool NeuronBackendConfig::operator==(NeuronBackendConfig const& other) const
{
	return (
	    m_address_out == other.get_address_out() && m_reset_holdoff == other.get_reset_holdoff() &&
	    m_refractory_time == other.get_refractory_time() &&
	    m_post_overwrite == other.get_post_overwrite() &&
	    m_select_input_clock == other.get_select_input_clock() &&
	    m_en_adapt_pulse == other.get_enable_adaptation_pulse() &&
	    m_en_baesian_extension == other.get_enable_bayesian_extension() &&
	    m_en_neuron_slave == other.get_enable_neuron_slave() &&
	    m_connect_fire_bottom == other.get_connect_fire_bottom() &&
	    m_connect_fire_from_right == other.get_connect_fire_from_right() &&
	    m_connect_fire_to_right == other.get_connect_fire_to_right() &&
	    m_en_spike_out == other.get_enable_spike_out() &&
	    m_en_neuron_master == other.get_enable_neuron_master() &&
	    m_en_0_baesian == other.get_enable_bayesian_0() &&
	    m_en_1_baesian == other.get_enable_bayesian_1());
}

bool NeuronBackendConfig::operator!=(NeuronBackendConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, NeuronBackendConfig const& config)
{
	// TODO add descriptions
	print_words_for_each_backend<NeuronBackendConfig>(os, config);
	// clang-format off
	os << "NAME\t\t\tVALUE\tDESCRIPTION" << std::endl
	<< std::boolalpha
	<< "address_out\t\t" << std::to_string(config.m_address_out) << "\t?" << std::endl
	<< "reset_holdoff\t" << std::to_string(config.m_reset_holdoff) << "\t?" << std::endl
	<< "refractory_time\t\t\t" << std::to_string(config.m_refractory_time) << "\t?" << std::endl
	<< "post_overwrite\t\t" << config.m_post_overwrite << "\t?" << std::endl
	<< "select_input_clock\t" << std::to_string(config.m_select_input_clock) << "\t?" << std::endl
	<< "en_adapt_pulse\t\t" << config.m_en_adapt_pulse << "\t?" << std::endl
	<< "en_baesian_extension\t" << config.m_en_baesian_extension << "\t?" << std::endl
	<< "en_neuron_slave\t\t" << config.m_en_neuron_slave << "\t?" << std::endl
	<< "connect_fire_bottom\t" << config.m_connect_fire_bottom << "\t?" << std::endl
	<< "connect_fire_from_right\t" << config.m_connect_fire_from_right << "\t?" << std::endl
	<< "connect_fire_to_right\t" << config.m_connect_fire_to_right << "\t?" << std::endl
	<< "en_spike_out\t\t" << config.m_en_spike_out << "\t?" << std::endl
	<< "en_neuron_master\t" << config.m_en_neuron_master << "\t?" << std::endl
	<< "en_0_baesian\t\t" << config.m_en_0_baesian << "\t?" << std::endl
	<< "en_1_baesian\t\t" << config.m_en_1_baesian << "\t?";
	// clang-format on
	return os;
}

template <class Archive>
void NeuronBackendConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_address_out));
	ar(CEREAL_NVP(m_reset_holdoff));
	ar(CEREAL_NVP(m_refractory_time));
	ar(CEREAL_NVP(m_post_overwrite));
	ar(CEREAL_NVP(m_select_input_clock));
	ar(CEREAL_NVP(m_en_adapt_pulse));
	ar(CEREAL_NVP(m_en_baesian_extension));
	ar(CEREAL_NVP(m_en_neuron_slave));
	ar(CEREAL_NVP(m_connect_fire_bottom));
	ar(CEREAL_NVP(m_connect_fire_from_right));
	ar(CEREAL_NVP(m_connect_fire_to_right));
	ar(CEREAL_NVP(m_en_spike_out));
	ar(CEREAL_NVP(m_en_neuron_master));
	ar(CEREAL_NVP(m_en_0_baesian));
	ar(CEREAL_NVP(m_en_1_baesian));
}

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
    m_en_thresh_comp(false),
    m_en_synin_inh(false),
    m_en_synin_exc(false),
    m_en_byp_inh(false),
    m_en_byp_exc(false),
    m_en_mem_off(false),
    m_en_cap_merge(false),
    m_mem_cap_size(0),
    m_invert_adapt_a(false),
    m_invert_adapt_b(false),
    m_en_adapt(false),
    m_en_adapt_cap(false),
    m_exp_weight(0),
    m_en_exp(false),
    m_en_read_vw(false),
    m_en_unbuf_access(false),
    m_en_readout_amp(false),
    m_readout_select(NeuronConfig::ReadoutSource::membrane),
    m_en_readout(false),
    m_en_reset_deg(false),
    m_en_reset_div(false),
    m_en_reset_mul(false),
    m_en_leak_deg(false),
    m_en_leak_div(false),
    m_en_leak_mul(false)
{}

struct NeuronConfig::NeuronConfigBitfield
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
        uint32_t                                                      : 26;  // 6-31 ; 2

        uint32_t en_read_vw                                           :  1;  // 0    ; 3
        uint32_t                                                      :  2;  // 1-2  ; 3
        uint32_t en_unbuf_access                                      :  1;  // 3    ; 3
        uint32_t en_readout_amp                                       :  1;  // 4    ; 3
        uint32_t readout_select                                       :  2;  // 5-6  ; 3
        uint32_t en_readout                                           :  1;  // 7    ; 3
        uint32_t                                                      : 24;  // 8-31 ; 3

        uint32_t invert_adapt_a                                       :  1;  // 0    ; 4
        uint32_t invert_adapt_b                                       :  1;  // 1    ; 4
        uint32_t en_adapt                                             :  1;  // 2    ; 4
        uint32_t en_adapt_cap                                         :  1;  // 3    ; 4
        uint32_t exp_weight                                           :  3;  // 4-6  ; 4
        uint32_t en_exp                                               :  1;  // 7    ; 4
        uint32_t                                                      : 24;  // 8-31 ; 4

        uint32_t en_mem_off                                           :  1;  // 0    ; 5
        uint32_t en_cap_merge                                         :  1;  // 1    ; 5
        uint32_t mem_cap_size                                         :  6;  // 2-7  ; 5
        uint32_t                                                      : 24;  // 8-31 ; 5

        uint32_t                                                      :  2;  // 0-1  ; 6
        uint32_t en_fire                                              :  1;  // 2    ; 6
        uint32_t en_thresh_comp                                       :  1;  // 3    ; 6
        uint32_t en_synin_inh                                         :  1;  // 4    ; 6
        uint32_t en_synin_exc                                         :  1;  // 5    ; 6
        uint32_t en_byp_inh                                           :  1;  // 6    ; 6
        uint32_t en_byp_exc                                           :  1;  // 7    ; 6
        uint32_t                                                      : 24;  // 8-31 ; 6

        uint32_t en_comp_cond_div                                     :  1;  // 0    ; 7
        uint32_t en_comp_cond_mul                                     :  1;  // 1    ; 7
        uint32_t connect_soma                                         :  1;  // 2    ; 7
        uint32_t connect_membrane_right                               :  1;  // 3    ; 7
        uint32_t en_comp_cond                                         :  1;  // 4    ; 7
        uint32_t connect_bottom                                       :  1;  // 5    ; 7
        uint32_t connect_somata                                       :  1;  // 6    ; 7
        uint32_t                                                      : 25;  // 7-31 ; 7
        } m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	NeuronConfigBitfield() { u.words = {{0, 0, 0, 0, 0, 0}}; }

	NeuronConfigBitfield(std::array<uint32_t, NeuronConfig::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0b0011'1111u;
		u.words[1] = data[1] & 0b1111'1001u;
		u.words[2] = data[2] & 0b1111'1111u;
		u.words[3] = data[3] & 0b1111'1111u;
		u.words[4] = data[4] & 0b1111'1111u;
		u.words[5] = data[5] & 0b0111'1111u;
	}
};

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

bool NeuronConfig::get_enable_capacitor_merge() const
{
	return m_en_cap_merge;
}
void NeuronConfig::set_enable_capacitor_merge(bool const value)
{
	m_en_cap_merge = value;
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

bool NeuronConfig::get_enable_adaptation_capacitor() const
{
	return m_en_adapt_cap;
}

void NeuronConfig::set_enable_adaptation_capacitor(bool const value)
{
	m_en_adapt_cap = value;
}

NeuronConfig::ExponentialTermStrength NeuronConfig::get_exponential_term_strength() const
{
	return m_exp_weight;
}
void NeuronConfig::set_exponential_term_strength(NeuronConfig::ExponentialTermStrength const value)
{
	m_exp_weight = value;
}

bool NeuronConfig::get_enable_exponential() const
{
	return m_en_exp;
}
void NeuronConfig::set_enable_exponential(bool const value)
{
	m_en_exp = value;
}

bool NeuronConfig::get_enable_adaptation_readout() const
{
	return m_en_read_vw;
}
void NeuronConfig::set_enable_adaptation_readout(bool const value)
{
	m_en_read_vw = value;
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

template <typename AddressT>
std::array<AddressT, NeuronConfig::config_size_in_words> NeuronConfig::addresses(
    NeuronConfig::coordinate_type const& neuron) const
{
	assert(neuron.toNeuronConfigBlockOnDLS() < neuron_sram_base_addresses.size());
	auto const base_address = neuron_sram_base_addresses.at(neuron.toNeuronConfigBlockOnDLS());
	auto const neuron_coord = neuron.toNeuronConfigOnNeuronConfigBlock();
	std::array<AddressT, NeuronConfig::config_size_in_words> data;
	size_t i = 0;
        // offset of 2 words in address due to them being unused
	std::generate(data.begin(), data.end(), [&]() {
		return static_cast<AddressT>(
		    base_address + 2 + neuron_coord * 8 + i++);
	});
	return data;
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, NeuronConfig::config_size_in_words>
    NeuronConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& cell) const;

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipAddress, NeuronConfig::config_size_in_words>
    NeuronConfig::addresses<halco::hicann_dls::vx::OmnibusChipAddress>(
        coordinate_type const& cell) const;


NeuronConfig::NeuronConfigBitfield NeuronConfig::to_bitfield() const
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
	bitfield.u.m.en_thresh_comp = m_en_thresh_comp;
	bitfield.u.m.en_synin_inh = m_en_synin_inh;
	bitfield.u.m.en_synin_exc = m_en_synin_exc;
	bitfield.u.m.en_byp_inh = m_en_byp_inh;
	bitfield.u.m.en_byp_exc = m_en_byp_exc;
	bitfield.u.m.en_mem_off = m_en_mem_off;
	bitfield.u.m.en_cap_merge = m_en_cap_merge;
	bitfield.u.m.mem_cap_size = static_cast<uint32_t>(m_mem_cap_size);
	bitfield.u.m.invert_adapt_a = m_invert_adapt_a;
	bitfield.u.m.invert_adapt_b = m_invert_adapt_b;
	bitfield.u.m.en_adapt = m_en_adapt;
	bitfield.u.m.en_adapt_cap = m_en_adapt_cap;
	bitfield.u.m.exp_weight = static_cast<uint32_t>(m_exp_weight);
	bitfield.u.m.en_exp = m_en_exp;
	bitfield.u.m.en_read_vw = m_en_read_vw;
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
	return bitfield;
}

void NeuronConfig::from_bitfield(NeuronConfig::NeuronConfigBitfield const& bitfield)
{
	m_en_comp_cond_div = bitfield.u.m.en_comp_cond_div;
	m_en_comp_cond_mul = bitfield.u.m.en_comp_cond_mul;
	m_connect_soma = bitfield.u.m.connect_soma;
	m_connect_membrane_right = bitfield.u.m.connect_membrane_right;
	m_en_comp_cond = bitfield.u.m.en_comp_cond;
	m_connect_bottom = bitfield.u.m.connect_bottom;
	m_connect_somata = bitfield.u.m.connect_somata;
	m_en_fire = bitfield.u.m.en_fire;
	m_en_thresh_comp = bitfield.u.m.en_thresh_comp;
	m_en_synin_inh = bitfield.u.m.en_synin_inh;
	m_en_synin_exc = bitfield.u.m.en_synin_exc;
	m_en_byp_inh = bitfield.u.m.en_byp_inh;
	m_en_byp_exc = bitfield.u.m.en_byp_exc;
	m_en_mem_off = bitfield.u.m.en_mem_off;
	m_en_cap_merge = bitfield.u.m.en_cap_merge;
	m_mem_cap_size = MembraneCapacitorSize(bitfield.u.m.mem_cap_size);
	m_invert_adapt_a = bitfield.u.m.invert_adapt_a;
	m_invert_adapt_b = bitfield.u.m.invert_adapt_b;
	m_en_adapt = bitfield.u.m.en_adapt;
	m_en_adapt_cap = bitfield.u.m.en_adapt_cap;
	m_exp_weight = ExponentialTermStrength(bitfield.u.m.exp_weight);
	m_en_exp = bitfield.u.m.en_exp;
	m_en_read_vw = bitfield.u.m.en_read_vw;
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
}

template <typename WordT>
std::array<WordT, NeuronConfig::config_size_in_words> NeuronConfig::encode() const
{
	auto bitfield = to_bitfield();
	std::array<WordT, NeuronConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronConfig::config_size_in_words>
    NeuronConfig::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::OmnibusChip, NeuronConfig::config_size_in_words>
NeuronConfig::encode<fisch::vx::OmnibusChip>() const;

template <typename WordT>
void NeuronConfig::decode(std::array<WordT, NeuronConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, NeuronConfig::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	NeuronConfigBitfield bitfield(raw_data);
	from_bitfield(bitfield);
}

template SYMBOL_VISIBLE void NeuronConfig::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, NeuronConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void NeuronConfig::decode<fisch::vx::OmnibusChip>(
    std::array<fisch::vx::OmnibusChip, NeuronConfig::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, NeuronConfig const& config)
{
	print_words_for_each_backend<NeuronConfig>(os, config);
	// clang-format off
    os << "NAME\t\t\tVALUE\tDESCRIPTION" << std::endl
    << std::boolalpha
    << "en_comp_cond_div\t" << config.m_en_comp_cond_div << "\tdivide inter-compartment conductance bias by 4" << std::endl
    << "en_comp_cond_mul\t" << config.m_en_comp_cond_mul << "\tmultiply inter-compartment conductance bias by 4" << std::endl
    << "connect_soma\t\t" << config.m_connect_soma << "\tconnect local membrane to soma" << std::endl
    << "connect_membrane_right\t\t" << config.m_connect_membrane_right << "\tconnect local membrane to membrane on right" << std::endl
    << "en_comp_cond\t\t" << config.m_en_comp_cond << "\tenable inter-compartment conductance" << std::endl
    << "connect_bottom\t\t" << config.m_connect_bottom << "\tconnect local membrane to bottom membrane" << std::endl
    << "connect_somata\t\t" << config.m_connect_somata << "\tconnect soma to soma on the right" << std::endl
    << "en_fire\t\t\t" << config.m_en_fire << "\tenable fire output of neuron (also gates bypass circuits)" << std::endl
    << "en_thresh_comp\t\t" << config.m_en_thresh_comp << "\tenable threshold comparator" << std::endl
    << "en_synin_inh\t\t" << config.m_en_synin_inh << "\tenable inh. synaptic input" << std::endl
    << "en_synin_exc\t\t" << config.m_en_synin_exc << "\tenable exc. synaptic input" << std::endl
    << "en_byp_inh\t\t" << config.m_en_byp_inh << "\tenable inh. bypass circuit" << std::endl
    << "en_byp_exc\t\t" << config.m_en_byp_exc << "\tenable exc. bypass circuit" << std::endl
    << "en_mem_off\t\t" << config.m_en_mem_off << "\tenable membrane offset current (can also be used for stimulus/step current)" << std::endl
    << "en_cap_merge\t\t" << config.m_en_cap_merge << "\tenable merging of membrane and adaptation capacitances" << std::endl
    << "mem_cap_size\t\t" << std::to_string(config.m_mem_cap_size) << "\tconfigure membrane size" << std::endl
    << "invert_adapt_a\t\t" << config.m_invert_adapt_a << "\tflip the sign of a" << std::endl
    << "invert_adapt_b\t\t" << config.m_invert_adapt_b << "\tflip the sign of b" << std::endl
    << "en_adapt\t\t" << config.m_en_adapt << "\tenable adaptation" << std::endl
    << "en_adapt_cap\t\t" << config.m_en_adapt_cap << "\tenable adaptation capacitance" << std::endl
    << "exp_weight\t\t" << std::to_string(config.m_exp_weight) << "\tstrength of exponential term" << std::endl
    << "en_exp\t\t\t" << config.m_en_exp << "\tenable exponential term" << std::endl
    << "en_read_vw\t\t" << config.m_en_read_vw << "\tenable readout of adaptation voltage (user must also configure readout_select!)" << std::endl
    << "en_unbuf_access\t\t" << config.m_en_unbuf_access << "\tenable direct, unbuffered access to membrane" << std::endl
    << "en_readout_amp\t\t" << config.m_en_readout_amp << "\tenable readout amplifier" << std::endl
    << "readout_select\t\t" << std::to_string(static_cast<uint_fast8_t>(config.m_readout_select)) << "\tselect readout source (0: membrane, 1: exc. synin, 2: inh. synin, 3: adaptation)" << std::endl
    << "en_readout\t\t" << config.m_en_readout << "\tenable readout" << std::endl
    << "en_reset_deg\t\t" << config.m_en_reset_deg << "\tenable source degeneration of leak/reset OTA in reset mode" << std::endl
    << "en_reset_div\t\t" << config.m_en_reset_div << "\tenable division (8x) of conductance in reset mode" << std::endl
    << "en_reset_mul\t\t" << config.m_en_reset_mul << "\tenable multiplication (8x) of conductance in reset mode" << std::endl
    << "en_leak_deg\t\t" << config.m_en_leak_deg << "\tenable source degeneration of leak/reset OTA in leak mode" << std::endl
    << "en_leak_div\t\t" << config.m_en_leak_div << "\tenable division (8x) of conductance in leak mode" << std::endl
    << "en_leak_mul\t\t" << config.m_en_leak_mul << "\tenable multiplication (8x) of conductance in leak mode";
	// clang-format on
	return os;
}

bool NeuronConfig::operator==(NeuronConfig const& other) const
{
	// clang-format off
    return (
        m_en_comp_cond_div  == other.get_enable_divide_multicomp_conductance_bias() &&
        m_en_comp_cond_mul  == other.get_enable_multiply_multicomp_conductance_bias() &&
        m_connect_soma  == other.get_connect_soma() &&
        m_connect_membrane_right  == other.get_connect_membrane_right() &&
        m_en_comp_cond  == other.get_enable_multicomp_conductance() &&
        m_connect_bottom  == other.get_connect_bottom() &&
        m_connect_somata  == other.get_connect_soma_right() &&
        m_en_fire  == other.get_enable_fire() &&
        m_en_thresh_comp  == other.get_enable_threshold_comparator() &&
        m_en_synin_inh  == other.get_enable_synaptic_input_inhibitory() &&
        m_en_synin_exc  == other.get_enable_synaptic_input_excitatory() &&
        m_en_byp_inh  == other.get_enable_bypass_inhibitory() &&
        m_en_byp_exc  == other.get_enable_bypass_excitatory() &&
        m_en_mem_off  == other.get_enable_membrane_offset() &&
        m_en_cap_merge  == other.get_enable_capacitor_merge() &&
        m_mem_cap_size  == other.get_membrane_capacitor_size() &&
        m_invert_adapt_a  == other.get_invert_adaptation_a() &&
        m_invert_adapt_b  == other.get_invert_adaptation_b() &&
        m_en_adapt  == other.get_enable_adaptation() &&
        m_en_adapt_cap  == other.get_enable_adaptation_capacitor() &&
        m_exp_weight  == other.get_exponential_term_strength() &&
        m_en_exp  == other.get_enable_exponential() &&
        m_en_read_vw  == other.get_enable_adaptation_readout() &&
        m_en_unbuf_access  == other.get_enable_unbuffered_access() &&
        m_en_readout_amp  == other.get_enable_readout_amplifier() &&
        m_readout_select  == other.get_readout_source() &&
        m_en_readout  == other.get_enable_readout() &&
        m_en_reset_deg  == other.get_enable_reset_degeneration() &&
        m_en_reset_div  == other.get_enable_reset_division() &&
        m_en_reset_mul  == other.get_enable_reset_multiplication() &&
        m_en_leak_deg  == other.get_enable_leak_degeneration() &&
        m_en_leak_div  == other.get_enable_leak_division() &&
        m_en_leak_mul  == other.get_enable_leak_multiplication()
    );
	// clang-format on
}

bool NeuronConfig::operator!=(NeuronConfig const& other) const
{
	return !(*this == other);
}

template <class Archive>
void NeuronConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_en_comp_cond_div));
	ar(CEREAL_NVP(m_en_comp_cond_mul));
	ar(CEREAL_NVP(m_connect_soma));
	ar(CEREAL_NVP(m_connect_membrane_right));
	ar(CEREAL_NVP(m_en_comp_cond));
	ar(CEREAL_NVP(m_connect_bottom));
	ar(CEREAL_NVP(m_connect_somata));
	ar(CEREAL_NVP(m_en_fire));
	ar(CEREAL_NVP(m_en_thresh_comp));
	ar(CEREAL_NVP(m_en_synin_inh));
	ar(CEREAL_NVP(m_en_synin_exc));
	ar(CEREAL_NVP(m_en_byp_inh));
	ar(CEREAL_NVP(m_en_byp_exc));
	ar(CEREAL_NVP(m_en_mem_off));
	ar(CEREAL_NVP(m_en_cap_merge));
	ar(CEREAL_NVP(m_mem_cap_size));
	ar(CEREAL_NVP(m_invert_adapt_a));
	ar(CEREAL_NVP(m_invert_adapt_b));
	ar(CEREAL_NVP(m_en_adapt));
	ar(CEREAL_NVP(m_en_adapt_cap));
	ar(CEREAL_NVP(m_exp_weight));
	ar(CEREAL_NVP(m_en_exp));
	ar(CEREAL_NVP(m_en_read_vw));
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
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(NeuronConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(NeuronBackendConfig)

} // namespace vx
} // namespace haldls
