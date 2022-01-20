#include "haldls/vx/neuron.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"

namespace haldls::vx {

// TODO: Initialize with reasonable default values, see Issue #3368
template <typename Coordinates>
NeuronBackendConfig<Coordinates>::NeuronBackendConfig() :
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
    m_en_neuron_master(true),
    m_en_0_baesian(false),
    m_en_1_baesian(false)
{}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_address_out(AddressOut const val)
{
	m_address_out = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_reset_holdoff(ResetHoldoff const val)
{
	m_reset_holdoff = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_refractory_time(RefractoryTime const val)
{
	m_refractory_time = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_post_overwrite(bool const val)
{
	m_post_overwrite = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_select_input_clock(InputClock const val)
{
	m_select_input_clock = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_enable_adaptation_pulse(bool const val)
{
	m_en_adapt_pulse = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_enable_bayesian_extension(bool const val)
{
	m_en_baesian_extension = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_enable_neuron_slave(bool const val)
{
	m_en_neuron_slave = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_connect_fire_bottom(bool const val)
{
	m_connect_fire_bottom = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_connect_fire_from_right(bool const val)
{
	m_connect_fire_from_right = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_connect_fire_to_right(bool const val)
{
	m_connect_fire_to_right = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_enable_spike_out(bool const val)
{
	m_en_spike_out = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_enable_neuron_master(bool const val)
{
	m_en_neuron_master = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_enable_bayesian_0(bool const val)
{
	m_en_0_baesian = val;
}

template <typename Coordinates>
void NeuronBackendConfig<Coordinates>::set_enable_bayesian_1(bool const val)
{
	m_en_1_baesian = val;
}

template <typename Coordinates>
NeuronBackendConfig<Coordinates>::AddressOut NeuronBackendConfig<Coordinates>::get_address_out()
    const
{
	return m_address_out;
}

template <typename Coordinates>
NeuronBackendConfig<Coordinates>::ResetHoldoff NeuronBackendConfig<Coordinates>::get_reset_holdoff()
    const
{
	return m_reset_holdoff;
}

template <typename Coordinates>
NeuronBackendConfig<Coordinates>::RefractoryTime
NeuronBackendConfig<Coordinates>::get_refractory_time() const
{
	return m_refractory_time;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_post_overwrite() const
{
	return m_post_overwrite;
}

template <typename Coordinates>
NeuronBackendConfig<Coordinates>::InputClock
NeuronBackendConfig<Coordinates>::get_select_input_clock() const
{
	return m_select_input_clock;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_enable_adaptation_pulse() const
{
	return m_en_adapt_pulse;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_enable_bayesian_extension() const
{
	return m_en_baesian_extension;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_enable_neuron_slave() const
{
	return m_en_neuron_slave;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_connect_fire_bottom() const
{
	return m_connect_fire_bottom;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_connect_fire_from_right() const
{
	return m_connect_fire_from_right;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_connect_fire_to_right() const
{
	return m_connect_fire_to_right;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_enable_spike_out() const
{
	return m_en_spike_out;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_enable_neuron_master() const
{
	return m_en_neuron_master;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_enable_bayesian_0() const
{
	return m_en_0_baesian;
}

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::get_enable_bayesian_1() const
{
	return m_en_1_baesian;
}

namespace {

template <typename Coordinates>
struct NeuronBackendConfigBitfield
{
	union
	{
		std::array<uint32_t, NeuronBackendConfig<Coordinates>::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
			                                                                     // bits ; word
			uint32_t address_out_msbs                                     :  6;  // 0-5  ; 0
			uint32_t reset_holdoff_1                                      :  2;  // 6-7  ; 0
			uint32_t                                                      : 24;  // 8-31 ; 0

			uint32_t refractory_time_1                                    :  4;  // 0-3  ; 1
			uint32_t address_out_lsbs                                     :  2;  // 4-5  ; 1
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

	NeuronBackendConfigBitfield()
	{
		u.words = {{0, 0, 0, 0}};
	}

	NeuronBackendConfigBitfield(
	    std::array<uint32_t, NeuronBackendConfig<Coordinates>::config_size_in_words> data)
	{
		u.words[0] = data[0];
		u.words[1] = data[1];
		u.words[2] = data[2];
		u.words[3] = data[3];
	}
};

} // anonymous namespace

template <typename Coordinates>
template <typename AddressT>
std::array<AddressT, NeuronBackendConfig<Coordinates>::config_size_in_words>
NeuronBackendConfig<Coordinates>::addresses(
    NeuronBackendConfig<Coordinates>::coordinate_type const& neuron)
{
	auto base_address = neuron_backend_west_sram_base_address;
	if (neuron.toNeuronBackendConfigBlockOnDLS() == 1) {
		base_address = neuron_backend_east_sram_base_address;
	}
	auto const neuron_coord = neuron.toNeuronBackendConfigOnNeuronBackendConfigBlock();
	std::array<AddressT, NeuronBackendConfig<Coordinates>::config_size_in_words> data;
	size_t i = 0;
	std::generate(data.begin(), data.end(), [&]() {
		return static_cast<AddressT>(
		    base_address + neuron_coord * NeuronBackendConfig<Coordinates>::config_size_in_words +
		    i++);
	});
	return data;
}

template <typename Coordinates>
template <typename WordT>
std::array<WordT, NeuronBackendConfig<Coordinates>::config_size_in_words>
NeuronBackendConfig<Coordinates>::encode() const
{
	NeuronBackendConfigBitfield<Coordinates> bitfield;
	bitfield.u.m.address_out_lsbs = (~m_address_out & 0b00000011); // bits are inverted
	bitfield.u.m.reset_holdoff_1 = m_reset_holdoff & 0b0011;
	bitfield.u.m.refractory_time_1 = (~m_refractory_time & 0b11110000) >> 4;
	bitfield.u.m.address_out_msbs = (~m_address_out & 0b11111100) >> 2; // bits are inverted
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
	std::array<WordT, NeuronBackendConfig<Coordinates>::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template <typename Coordinates>
template <typename WordT>
void NeuronBackendConfig<Coordinates>::decode(
    std::array<WordT, NeuronBackendConfig<Coordinates>::config_size_in_words> const& data)
{
	std::array<uint32_t, NeuronBackendConfig<Coordinates>::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	NeuronBackendConfigBitfield<Coordinates> bitfield(raw_data);
	// bits of address out are inverted
	m_address_out = NeuronBackendConfig<Coordinates>::AddressOut(
	    ((~(bitfield.u.m.address_out_lsbs) & 0b00000011) |
	     (~(bitfield.u.m.address_out_msbs << 2) & 0b11111100)) &
	    0xff);
	m_reset_holdoff = NeuronBackendConfig<Coordinates>::ResetHoldoff(
	    bitfield.u.m.reset_holdoff_1 | (bitfield.u.m.reset_holdoff_2 << 2));
	m_refractory_time = NeuronBackendConfig<Coordinates>::RefractoryTime(
	    ((~(bitfield.u.m.refractory_time_2) & 0xf) |
	     (~(bitfield.u.m.refractory_time_1 << 4) & 0xf0)) &
	    0xff);
	m_post_overwrite = bitfield.u.m.post_overwrite;
	m_select_input_clock =
	    NeuronBackendConfig<Coordinates>::InputClock(bitfield.u.m.select_input_clock);
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

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::operator==(NeuronBackendConfig const& other) const
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

template <typename Coordinates>
bool NeuronBackendConfig<Coordinates>::operator!=(NeuronBackendConfig const& other) const
{
	return !(*this == other);
}

template <typename Coordinates>
template <class Archive>
void NeuronBackendConfig<Coordinates>::serialize(Archive& ar, std::uint32_t const)
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

#define NEURON_BACKEND_CONFIG_UNROLL(Coordinates)                                                  \
	template class NeuronBackendConfig<Coordinates>;                                               \
                                                                                                   \
	template std::array<                                                                           \
	    fisch::vx::word_access_type::OmnibusChipOverJTAG,                                          \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>()   \
	    const;                                                                                     \
                                                                                                   \
	template std::array<                                                                           \
	    fisch::vx::word_access_type::Omnibus,                                                      \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::encode<fisch::vx::word_access_type::Omnibus>() const;        \
                                                                                                   \
	template void                                                                                  \
	NeuronBackendConfig<Coordinates>::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(    \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::OmnibusChipOverJTAG,                                      \
	        NeuronBackendConfig<Coordinates>::config_size_in_words> const& data);                  \
                                                                                                   \
	template void NeuronBackendConfig<Coordinates>::decode<fisch::vx::word_access_type::Omnibus>(  \
	    std::array<                                                                                \
	        fisch::vx::word_access_type::Omnibus,                                                  \
	        NeuronBackendConfig<Coordinates>::config_size_in_words> const& data);                  \
                                                                                                   \
	template std::array<                                                                           \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::addresses<                                                   \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(coordinate_type const& cell);           \
                                                                                                   \
	template std::array<                                                                           \
	    halco::hicann_dls::vx::OmnibusAddress,                                                     \
	    NeuronBackendConfig<Coordinates>::config_size_in_words>                                    \
	NeuronBackendConfig<Coordinates>::addresses<halco::hicann_dls::vx::OmnibusAddress>(            \
	    coordinate_type const& cell);                                                              \
                                                                                                   \
	EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(NeuronBackendConfig<Coordinates>)

} // namespace haldls::vx
