#include "haldls/vx/background.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls {
namespace vx {

BackgroundSpikeSource::BackgroundSpikeSource() :
    m_enable(false),
    m_enable_random(false),
    m_period(),
    m_rate(),
    m_seed(),
    m_mask(),
    m_neuron_label()
{}

BackgroundSpikeSource::Period BackgroundSpikeSource::get_period() const
{
	return m_period;
}

void BackgroundSpikeSource::set_period(BackgroundSpikeSource::Period const& period)
{
	m_period = period;
}

BackgroundSpikeSource::Rate BackgroundSpikeSource::get_rate() const
{
	return m_rate;
}

void BackgroundSpikeSource::set_rate(BackgroundSpikeSource::Rate const& rate)
{
	m_rate = rate;
}

BackgroundSpikeSource::Seed BackgroundSpikeSource::get_seed() const
{
	return m_seed;
}

void BackgroundSpikeSource::set_seed(BackgroundSpikeSource::Seed const& seed)
{
	m_seed = seed;
}

BackgroundSpikeSource::Mask BackgroundSpikeSource::get_mask() const
{
	return m_mask;
}

void BackgroundSpikeSource::set_mask(BackgroundSpikeSource::Mask const& mask)
{
	m_mask = mask;
}

halco::hicann_dls::vx::NeuronLabel BackgroundSpikeSource::get_neuron_label() const
{
	return m_neuron_label;
}

void BackgroundSpikeSource::set_neuron_label(halco::hicann_dls::vx::NeuronLabel const& value)
{
	m_neuron_label = value;
}

bool BackgroundSpikeSource::get_enable() const
{
	return m_enable;
}

void BackgroundSpikeSource::set_enable(bool const value)
{
	m_enable = value;
}

bool BackgroundSpikeSource::get_enable_random() const
{
	return m_enable_random;
}

void BackgroundSpikeSource::set_enable_random(bool const value)
{
	m_enable_random = value;
}

bool BackgroundSpikeSource::operator==(BackgroundSpikeSource const& other) const
{
	return (m_enable == other.m_enable) && (m_enable_random == other.m_enable_random) &&
	       (m_period == other.m_period) && (m_rate == other.m_rate) && (m_seed == other.m_seed) &&
	       (m_mask == other.m_mask) && (m_neuron_label == other.m_neuron_label);
}

bool BackgroundSpikeSource::operator!=(BackgroundSpikeSource const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, BackgroundSpikeSource const& config)
{
	std::stringstream ss;
	ss << "BackgroundSpikeSource(enable: " << std::boolalpha << config.m_enable
	   << ", enable_random: " << config.m_enable_random << ", " << config.m_period << ", "
	   << config.m_rate << ", " << config.m_seed << ", " << config.m_mask << ", "
	   << config.m_neuron_label << ")";
	return (os << ss.str());
}
#endif

template <typename AddressT>
std::array<AddressT, BackgroundSpikeSource::config_size_in_words> BackgroundSpikeSource::addresses(
    coordinate_type const& coord)
{
	uint32_t const base = background_spike_source_base_address + (coord * config_size_in_words);
	// first word at end so that on enabling, all configuration is already present
	// otherwise the seed is not applied FIXME: make issue
	return {AddressT(base + 1), AddressT(base + 2), AddressT(base)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    BackgroundSpikeSource::config_size_in_words>
BackgroundSpikeSource::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, BackgroundSpikeSource::config_size_in_words>
    BackgroundSpikeSource::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);

namespace {

struct BackgroundSpikeSourceBitfield
{
	union
	{
		std::array<uint32_t, BackgroundSpikeSource::config_size_in_words> raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t enable        :  1;) \
			(uint32_t enable_random :  1;) \
			(uint32_t /* unused */  : 14;) \
			(uint32_t period        : 16;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t seed          : 32;)
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t mask          :  8;) \
			(uint32_t rate          :  8;) \
			(uint32_t neuron_label  : 14;) \
			(uint32_t /* unused */  :  2;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	BackgroundSpikeSourceBitfield() { u.raw = {0u, 0u, 0u}; }

	BackgroundSpikeSourceBitfield(
	    std::array<uint32_t, BackgroundSpikeSource::config_size_in_words> data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename WordT>
std::array<WordT, BackgroundSpikeSource::config_size_in_words> BackgroundSpikeSource::encode() const
{
	BackgroundSpikeSourceBitfield bitfield;
	bitfield.u.m.enable = m_enable;
	bitfield.u.m.enable_random = m_enable_random;
	bitfield.u.m.period = m_period;
	bitfield.u.m.rate = m_rate;
	bitfield.u.m.seed = m_seed;
	bitfield.u.m.mask = m_mask;
	bitfield.u.m.neuron_label = m_neuron_label;

	return {WordT(bitfield.u.raw[1]), WordT(bitfield.u.raw[2]), WordT(bitfield.u.raw[0])};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    BackgroundSpikeSource::config_size_in_words>
BackgroundSpikeSource::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, BackgroundSpikeSource::config_size_in_words>
    BackgroundSpikeSource::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void BackgroundSpikeSource::decode(
    std::array<WordT, BackgroundSpikeSource::config_size_in_words> const& data)
{
	BackgroundSpikeSourceBitfield bitfield;
	bitfield.u.raw[0] = data.at(2);
	bitfield.u.raw[1] = data.at(0);
	bitfield.u.raw[2] = data.at(1);

	m_enable = bitfield.u.m.enable;
	m_enable_random = bitfield.u.m.enable_random;
	m_period = Period(bitfield.u.m.period);
	m_rate = Rate(bitfield.u.m.rate);
	m_seed = Seed(bitfield.u.m.seed);
	m_mask = Mask(bitfield.u.m.mask);
	m_neuron_label = halco::hicann_dls::vx::NeuronLabel(bitfield.u.m.neuron_label);
}

template SYMBOL_VISIBLE void
BackgroundSpikeSource::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        BackgroundSpikeSource::config_size_in_words> const& data);

template SYMBOL_VISIBLE void BackgroundSpikeSource::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        BackgroundSpikeSource::config_size_in_words> const& data);

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::BackgroundSpikeSource)
