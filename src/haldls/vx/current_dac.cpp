#include "haldls/vx/current_dac.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/bitfield.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/bitset.h"
#include "hate/indent.h"
#include "hate/join.h"
#include "hate/math.h"

namespace haldls::vx {

using halco::common::typed_array;
using namespace halco::hicann_dls::vx;

CurrentDAC::CurrentDAC() :
    m_current(0),
    m_enable_current(false),
    m_sign(Sign::source),
    m_connect_neuron({false, false}),
    m_connect_synapse({false, false})
{}

CurrentDAC::Current CurrentDAC::get_current() const
{
	return m_current;
}

void CurrentDAC::set_current(Current const value)
{
	m_current = value;
}

bool CurrentDAC::get_enable_current() const
{
	return m_enable_current;
}

void CurrentDAC::set_enable_current(bool const value)
{
	m_enable_current = value;
}

CurrentDAC::Sign CurrentDAC::get_sign() const
{
	return m_sign;
}

void CurrentDAC::set_sign(Sign const value)
{
	m_sign = value;
}

CurrentDAC::HemisphereType CurrentDAC::get_connect_neuron() const
{
	return m_connect_neuron;
}

void CurrentDAC::set_connect_neuron(HemisphereType const value)
{
	m_connect_neuron = value;
}

CurrentDAC::SynapseTargetType CurrentDAC::get_connect_synapse() const
{
	return m_connect_synapse;
}

void CurrentDAC::set_connect_synapse(SynapseTargetType const value)
{
	m_connect_synapse = value;
}

bool CurrentDAC::operator==(CurrentDAC const& other) const
{
	return (
	    m_current == other.m_current && m_enable_current == other.m_enable_current &&
	    m_sign == other.m_sign && m_connect_neuron == other.m_connect_neuron &&
	    m_connect_synapse == other.m_connect_synapse);
}

bool CurrentDAC::operator!=(CurrentDAC const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CurrentDAC::Sign const& config)
{
	switch (config) {
		case CurrentDAC::Sign::source: {
			os << "source";
			break;
		}
		case CurrentDAC::Sign::sink: {
			os << "sink";
			break;
		}
		default: {
#ifndef __ppu__
			throw std::logic_error("Unknown Sign.");
#else
			exit(1);
#endif
		}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, CurrentDAC const& config)
{
	std::stringstream ss;
	ss << "CurrentDAC(\n" << std::boolalpha;
	// clang-format off
	ss << "\tcurrent:          \t" << config.m_current << "\n"
	   << "\tenable_current:   \t" << config.m_enable_current << "\n"
	   << "\tsign:             \t" << config.m_sign << "\n"
	   << "\tconnect_neuron:   \t[top: "
	   << config.m_connect_neuron[halco::hicann_dls::vx::HemisphereOnDLS::top]
	   << ", bottom: "
	   << config.m_connect_neuron[halco::hicann_dls::vx::HemisphereOnDLS::bottom]
	   << "]\n)"
	   << "\tconnect_synapse:  \t[inhibitory: "
	   << config.m_connect_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::inhibitory]
	   << ", excitatory: "
	   << config.m_connect_synapse[halco::hicann_dls::vx::SynapticInputOnNeuron::excitatory]
	   << "]\n)"
	   << "\n)";
	// clang-format on
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, CurrentDAC::config_size_in_words> CurrentDAC::addresses(
    coordinate_type const& /*coord*/)
{
	return {AddressT(madc_base_address + 11)};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CurrentDAC::config_size_in_words>
    CurrentDAC::addresses(coordinate_type const& /*coord*/);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CurrentDAC::config_size_in_words>
    CurrentDAC::addresses(coordinate_type const& /*coord*/);

namespace {

struct CurrentDACBitfield
{
	union
	{
		std::array<uint32_t, CurrentDAC::config_size_in_words> words;

		struct __attribute__((packed))
		{
			// clang-format off
#define BITFIELD \
			(uint32_t binary_code             :  6; /* 0-5     */ ) \
			(uint32_t direction               :  1; /* 6       */ ) \
			(uint32_t enable_current          :  1; /* 7       */ ) \
			(uint32_t thermometer_code        :  3; /* 8-10    */ ) \
			(uint32_t mux_i_stim_top          :  1; /* 11      */ ) \
			(uint32_t mux_i_stim_bottom       :  1; /* 12      */ ) \
			(uint32_t mux_syntest_inhibitory  :  1; /* 13      */ ) \
			(uint32_t mux_syntest_excitatory  :  1; /* 14      */ ) \
			(uint32_t /* unused */            : 17; /* 15...31 */ )
			EXPAND_BITFIELD_ELEMENTS(BITFIELD)
#undef BITFIELD
			// clang-format on
		} m;

		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	CurrentDACBitfield()
	{
		u.words = {{0}};
	}

	CurrentDACBitfield(std::array<uint32_t, CurrentDAC::config_size_in_words> data)
	{
		u.words[0] = data[0];
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, CurrentDAC::config_size_in_words> CurrentDAC::encode() const
{
	CurrentDACBitfield bitfield;

	/* split 8 bit current value into thermometer and binary code.
	 * 2 MSB correspond to thermometer code and 6 LSB to binary code*/
	bitfield.u.m.binary_code = m_current & 0x3f;
	size_t const num_bits_to_set = m_current >> 6;
	bitfield.u.m.thermometer_code = hate::fill_bits<uint32_t>(num_bits_to_set);

	bitfield.u.m.direction = static_cast<uint32_t>(m_sign);
	bitfield.u.m.enable_current = m_enable_current;
	bitfield.u.m.mux_i_stim_top = m_connect_neuron[HemisphereOnDLS(0)];
	bitfield.u.m.mux_i_stim_bottom = m_connect_neuron[HemisphereOnDLS(1)];
	bitfield.u.m.mux_syntest_inhibitory = m_connect_synapse[SynapticInputOnNeuron(0)];
	bitfield.u.m.mux_syntest_excitatory = m_connect_synapse[SynapticInputOnNeuron(1)];

	std::array<WordT, CurrentDAC::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CurrentDAC::config_size_in_words>
    CurrentDAC::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, CurrentDAC::config_size_in_words>
    CurrentDAC::encode() const;

template <typename WordT>
void CurrentDAC::decode(std::array<WordT, CurrentDAC::config_size_in_words> const& data)
{
	std::array<uint32_t, CurrentDAC::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	CurrentDACBitfield bitfield(raw_data);

	/* convert binary and thermometer encoding into 8 bit current value.
	 * 2 MSB correspond to thermometer code and 6 LSB to binary code*/
	uint32_t value = bitfield.u.m.binary_code;
	std::bitset<3> thermo = bitfield.u.m.thermometer_code;
	if (thermo.test(2)) {
		value += 3 << 6;
	} else if (thermo.test(1)) {
		value += 2 << 6;
	} else if (thermo.test(0)) {
		value += 1 << 6;
	}
	m_current = Current(value);

	m_enable_current = bitfield.u.m.enable_current;
	m_sign = Sign(bitfield.u.m.direction);
	m_connect_neuron[HemisphereOnDLS(0)] = bitfield.u.m.mux_i_stim_top;
	m_connect_neuron[HemisphereOnDLS(1)] = bitfield.u.m.mux_i_stim_bottom;
	m_connect_synapse[SynapticInputOnNeuron(0)] = bitfield.u.m.mux_syntest_inhibitory;
	m_connect_synapse[SynapticInputOnNeuron(1)] = bitfield.u.m.mux_syntest_excitatory;
}

template SYMBOL_VISIBLE void CurrentDAC::decode(
    std::array<fisch::vx::word_access_type::Omnibus, CurrentDAC::config_size_in_words> const&
        words);

template SYMBOL_VISIBLE void CurrentDAC::decode(std::array<
                                                fisch::vx::word_access_type::OmnibusChipOverJTAG,
                                                CurrentDAC::config_size_in_words> const& words);

} // namespace haldls::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CurrentDAC)
