#include "haldls/vx/neuron.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"

namespace haldls {
namespace vx {

// TODO: Initialize with reasonable default values, see Issue #3367
CommonNeuronBackendConfig::CommonNeuronBackendConfig() :
    m_en_event_regs(true),
    m_force_reset(false),
    m_en_clocks(true),
    m_clock_scale_slow(CommonNeuronBackendConfig::ClockScale()),
    m_clock_scale_fast(CommonNeuronBackendConfig::ClockScale()),
    m_sample_pos_edge(),
    m_clock_scale_adapt_pulse(CommonNeuronBackendConfig::ClockScale()),
    m_clock_scale_post_pulse(CommonNeuronBackendConfig::ClockScale()),
    m_wait_global_post_pulse(CommonNeuronBackendConfig::WaitGlobalPostPulse()),
    m_wait_spike_counter_reset(CommonNeuronBackendConfig::WaitSpikeCounterReset()),
    m_wait_spike_counter_read(CommonNeuronBackendConfig::WaitSpikeCounterRead()),
    m_wait_fire_neuron(CommonNeuronBackendConfig::WaitFireNeuron())
{
	m_sample_pos_edge.fill(true);
}

void CommonNeuronBackendConfig::set_enable_event_registers(bool const val)
{
	m_en_event_regs = val;
}

void CommonNeuronBackendConfig::set_force_reset(bool const val)
{
	m_force_reset = val;
}

void CommonNeuronBackendConfig::set_enable_clocks(bool const val)
{
	m_en_clocks = val;
}

void CommonNeuronBackendConfig::set_clock_scale_slow(ClockScale const val)
{
	m_clock_scale_slow = val;
}

void CommonNeuronBackendConfig::set_clock_scale_fast(ClockScale const val)
{
	m_clock_scale_fast = val;
}

void CommonNeuronBackendConfig::set_sample_positive_edge(
    halco::hicann_dls::vx::NeuronEventOutputOnNeuronBackendBlock const coord, bool const val)
{
	m_sample_pos_edge[coord] = val;
}

void CommonNeuronBackendConfig::set_clock_scale_adaptation_pulse(ClockScale const val)
{
	m_clock_scale_adapt_pulse = val;
}

void CommonNeuronBackendConfig::set_clock_scale_post_pulse(ClockScale const val)
{
	m_clock_scale_post_pulse = val;
}

void CommonNeuronBackendConfig::set_wait_global_post_pulse(WaitGlobalPostPulse const val)
{
	m_wait_global_post_pulse = val;
}

void CommonNeuronBackendConfig::set_wait_spike_counter_reset(WaitSpikeCounterReset const val)
{
	m_wait_spike_counter_reset = val;
}

void CommonNeuronBackendConfig::set_wait_spike_counter_read(WaitSpikeCounterRead const val)
{
	m_wait_spike_counter_read = val;
}

void CommonNeuronBackendConfig::set_wait_fire_neuron(WaitFireNeuron const val)
{
	m_wait_fire_neuron = val;
}

bool CommonNeuronBackendConfig::get_enable_event_registers() const
{
	return m_en_event_regs;
}

bool CommonNeuronBackendConfig::get_force_reset() const
{
	return m_force_reset;
}

bool CommonNeuronBackendConfig::get_enable_clocks() const
{
	return m_en_clocks;
}

CommonNeuronBackendConfig::ClockScale CommonNeuronBackendConfig::get_clock_scale_slow() const
{
	return m_clock_scale_slow;
}

CommonNeuronBackendConfig::ClockScale CommonNeuronBackendConfig::get_clock_scale_fast() const
{
	return m_clock_scale_fast;
}

bool CommonNeuronBackendConfig::get_sample_positive_edge(
    halco::hicann_dls::vx::NeuronEventOutputOnNeuronBackendBlock const coord) const
{
	return m_sample_pos_edge[coord];
}

CommonNeuronBackendConfig::ClockScale CommonNeuronBackendConfig::get_clock_scale_adaptation_pulse()
    const
{
	return m_clock_scale_adapt_pulse;
}

CommonNeuronBackendConfig::ClockScale CommonNeuronBackendConfig::get_clock_scale_post_pulse() const
{
	return m_clock_scale_post_pulse;
}

CommonNeuronBackendConfig::WaitGlobalPostPulse CommonNeuronBackendConfig::get_wait_global_post_pulse() const
{
	return m_wait_global_post_pulse;
}

CommonNeuronBackendConfig::WaitSpikeCounterReset CommonNeuronBackendConfig::get_wait_spike_counter_reset() const
{
	return m_wait_spike_counter_reset;
}

CommonNeuronBackendConfig::WaitSpikeCounterRead CommonNeuronBackendConfig::get_wait_spike_counter_read() const
{
	return m_wait_spike_counter_read;
}

CommonNeuronBackendConfig::WaitFireNeuron CommonNeuronBackendConfig::get_wait_fire_neuron() const
{
	return m_wait_fire_neuron;
}

namespace {

struct CommonNeuronBackendConfigBitfield
{
	union
	{
		std::array<uint32_t, CommonNeuronBackendConfig::config_size_in_words> words;
		// clang-format off
		struct __attribute__((packed)) {
			                                        /* bits  ; word */
#define WORD \
			(uint32_t en_event_regs            : 1; /* 0     ; 0    */ ) \
			(uint32_t force_reset              : 1; /* 1     ; 0    */ ) \
			(uint32_t en_clocks                : 1; /* 2     ; 0    */ ) \
			(uint32_t                          : 1; /* 3     ; 0    */ ) \
			(uint32_t clock_scale_slow         : 4; /* 4-7   ; 0    */ ) \
			(uint32_t clock_scale_fast         : 4; /* 8-11  ; 0    */ ) \
			(uint32_t sample_pos_edge          : 4; /* 12-15 ; 0    */ ) \
			(uint32_t clock_scale_adapt_pulse  : 4; /* 16-19 ; 0    */ ) \
			(uint32_t clock_scale_post_pulse   : 4; /* 20-23 ; 0    */ ) \
			(uint32_t                          : 8; /* 24-31 ; 0    */ )
			EXPAND_WORD(WORD)
#undef WORD
#define WORD \
			(uint32_t wait_global_post_pulse   : 8; /* 0-7   ; 1    */ ) \
			(uint32_t wait_spike_counter_reset : 8; /* 8-15  ; 1    */ ) \
			(uint32_t wait_spike_counter_read  : 8; /* 16-23 ; 1    */ ) \
			(uint32_t wait_fire_neuron         : 8; /* 23-31 ; 1    */ )
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	CommonNeuronBackendConfigBitfield() { u.words = {{0, 0}}; }

	CommonNeuronBackendConfigBitfield(
	    std::array<uint32_t, CommonNeuronBackendConfig::config_size_in_words> data)
	{
		u.words[0] = data[0];
		u.words[1] = data[1];
	}
};

} // anonymous namespace

template <typename AddressT>
std::array<AddressT, CommonNeuronBackendConfig::config_size_in_words>
CommonNeuronBackendConfig::addresses(CommonNeuronBackendConfig::coordinate_type const& backend)
{
	auto base_address = neuron_backend_west_register_base_address;
	if (backend == 1) {
		base_address = neuron_backend_east_register_base_address;
	}
	std::array<AddressT, CommonNeuronBackendConfig::config_size_in_words> data;
	size_t i = 0;
	std::generate(
	    data.begin(), data.end(), [&]() { return static_cast<AddressT>(base_address + i++); });
	return data;
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CommonNeuronBackendConfig::config_size_in_words>
CommonNeuronBackendConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusAddress, CommonNeuronBackendConfig::config_size_in_words>
    CommonNeuronBackendConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename WordT>
std::array<WordT, CommonNeuronBackendConfig::config_size_in_words>
CommonNeuronBackendConfig::encode() const
{
	using namespace halco::hicann_dls::vx;
	CommonNeuronBackendConfigBitfield bitfield;
	bitfield.u.m.en_event_regs = m_en_event_regs;
	bitfield.u.m.force_reset = m_force_reset;
	bitfield.u.m.en_clocks = m_en_clocks;
	bitfield.u.m.clock_scale_slow = static_cast<uint32_t>(m_clock_scale_slow);
	bitfield.u.m.clock_scale_fast = static_cast<uint32_t>(m_clock_scale_fast);
	bitfield.u.m.sample_pos_edge = static_cast<uint32_t>(
	    m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(0)] |
	    m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(1)] << 1 |
	    m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(2)] << 2 |
	    m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(3)] << 3);
	bitfield.u.m.clock_scale_adapt_pulse = static_cast<uint32_t>(m_clock_scale_adapt_pulse);
	bitfield.u.m.clock_scale_post_pulse = static_cast<uint32_t>(m_clock_scale_post_pulse);
	bitfield.u.m.wait_global_post_pulse = static_cast<uint32_t>(m_wait_global_post_pulse);
	bitfield.u.m.wait_spike_counter_reset = static_cast<uint32_t>(m_wait_spike_counter_reset);
	bitfield.u.m.wait_spike_counter_read = static_cast<uint32_t>(m_wait_spike_counter_read);
	bitfield.u.m.wait_fire_neuron = static_cast<uint32_t>(m_wait_fire_neuron);
	std::array<WordT, CommonNeuronBackendConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(w); });
	return data;
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    CommonNeuronBackendConfig::config_size_in_words>
CommonNeuronBackendConfig::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::Omnibus, CommonNeuronBackendConfig::config_size_in_words>
    CommonNeuronBackendConfig::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void CommonNeuronBackendConfig::decode(
    std::array<WordT, CommonNeuronBackendConfig::config_size_in_words> const& data)
{
	using namespace halco::hicann_dls::vx;
	std::array<uint32_t, CommonNeuronBackendConfig::config_size_in_words> raw_data;
	std::transform(data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w; });
	CommonNeuronBackendConfigBitfield bitfield(raw_data);
	m_en_event_regs = bitfield.u.m.en_event_regs;
	m_force_reset = bitfield.u.m.force_reset;
	m_en_clocks = bitfield.u.m.en_clocks;
	m_clock_scale_slow = CommonNeuronBackendConfig::ClockScale(bitfield.u.m.clock_scale_slow);
	m_clock_scale_fast = CommonNeuronBackendConfig::ClockScale(bitfield.u.m.clock_scale_fast);
	m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(0)] =
	    bitfield.u.m.sample_pos_edge & 0b1;
	m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(1)] =
	    bitfield.u.m.sample_pos_edge & 0b10;
	m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(2)] =
	    bitfield.u.m.sample_pos_edge & 0b100;
	m_sample_pos_edge[NeuronEventOutputOnNeuronBackendBlock(3)] =
	    bitfield.u.m.sample_pos_edge & 0b1000;
	m_clock_scale_adapt_pulse =
	    CommonNeuronBackendConfig::ClockScale(bitfield.u.m.clock_scale_adapt_pulse);
	m_clock_scale_post_pulse =
	    CommonNeuronBackendConfig::ClockScale(bitfield.u.m.clock_scale_post_pulse);
	m_wait_global_post_pulse =
	    CommonNeuronBackendConfig::WaitGlobalPostPulse(bitfield.u.m.wait_global_post_pulse);
	m_wait_spike_counter_reset =
	    CommonNeuronBackendConfig::WaitSpikeCounterReset(bitfield.u.m.wait_spike_counter_reset);
	m_wait_spike_counter_read =
	    CommonNeuronBackendConfig::WaitSpikeCounterRead(bitfield.u.m.wait_spike_counter_read);
	m_wait_fire_neuron =
	    CommonNeuronBackendConfig::WaitFireNeuron(bitfield.u.m.wait_fire_neuron);
}

template SYMBOL_VISIBLE void
CommonNeuronBackendConfig::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        CommonNeuronBackendConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void
CommonNeuronBackendConfig::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        CommonNeuronBackendConfig::config_size_in_words> const& data);

bool CommonNeuronBackendConfig::operator==(CommonNeuronBackendConfig const& other) const
{
	return (
	    m_en_event_regs == other.get_enable_event_registers() &&
	    m_force_reset == other.get_force_reset() && m_en_clocks == other.get_enable_clocks() &&
	    m_clock_scale_slow == other.get_clock_scale_slow() &&
	    m_clock_scale_fast == other.get_clock_scale_fast() &&
	    m_sample_pos_edge == other.m_sample_pos_edge &&
	    m_clock_scale_adapt_pulse == other.get_clock_scale_adaptation_pulse() &&
	    m_clock_scale_post_pulse == other.get_clock_scale_post_pulse() &&
	    m_wait_global_post_pulse == other.get_wait_global_post_pulse() &&
	    m_wait_spike_counter_reset == other.get_wait_spike_counter_reset() &&
	    m_wait_spike_counter_read == other.get_wait_spike_counter_read() &&
	    m_wait_fire_neuron == other.get_wait_fire_neuron());
}

bool CommonNeuronBackendConfig::operator!=(CommonNeuronBackendConfig const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, CommonNeuronBackendConfig const& config)
{
	std::stringstream ss;
	ss << "CommonNeuronBackendConfig(\n" << std::boolalpha;
	// clang-format off
	ss << "\tenable_event_registers:       \t" << config.m_en_event_regs << "\n"
	   << "\tforce_reset:                  \t" << config.m_force_reset << "\n"
	   << "\tenable_clocks:                \t" << config.m_en_clocks << "\n"
	   << "\tclock_scale_slow:             \t" << config.m_clock_scale_slow << "\n"
	   << "\tclock_scale_fast:             \t" << config.m_clock_scale_fast << "\n"
	   << "\tsample_positive_edge:         \t[" << hate::join(config.m_sample_pos_edge, ", ") << "]\n"
	   << "\tclock_scale_adaptation_pulse: \t" << config.m_clock_scale_adapt_pulse << "\n"
	   << "\tclock_scale_post_pulse:       \t" << config.m_clock_scale_post_pulse << "\n"
	   << "\twait_global_post_pulse:       \t" << config.m_wait_global_post_pulse << "\n"
	   << "\twait_spike_counter_reset:     \t" << config.m_wait_spike_counter_reset << "\n"
	   << "\twait_spike_counter_read       \t" << config.m_wait_spike_counter_read << "\n"
	   << "\twait_fire_neuron:             \t" << config.m_wait_fire_neuron << "\n)";
	// clang-format on
	return (os << ss.str());
}
#endif


NeuronReset::NeuronReset() {}

template <typename AddressT>
std::array<AddressT, NeuronReset::read_config_size_in_words> NeuronReset::read_addresses(
    NeuronReset::coordinate_type const& /* neuron */)
{
	return {};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, NeuronReset::read_config_size_in_words>
    NeuronReset::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
        coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, NeuronReset::read_config_size_in_words>
    NeuronReset::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(coordinate_type const& cell);

template <typename AddressT>
std::array<AddressT, NeuronReset::write_config_size_in_words> NeuronReset::write_addresses(
    NeuronReset::coordinate_type const& neuron)
{
	auto const base_address = neuron_reset_sram_base_addresses.at(neuron.toNeuronResetBlockOnDLS());
	auto const neuron_coord = neuron.toNeuronResetOnNeuronResetBlock();

	// Multiply the neuron coordinate with the backend config size to get the true address
	return {AddressT(base_address + neuron_coord * 4)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    NeuronReset::write_config_size_in_words>
NeuronReset::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, NeuronReset::write_config_size_in_words>
    NeuronReset::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename WordT>
std::array<WordT, NeuronReset::write_config_size_in_words> NeuronReset::encode() const
{
	// Value does not matter
	return {WordT()};
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, NeuronReset::write_config_size_in_words>
    NeuronReset::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, NeuronReset::write_config_size_in_words>
    NeuronReset::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void NeuronReset::decode(std::array<WordT, NeuronReset::read_config_size_in_words> const&)
{}

template SYMBOL_VISIBLE void NeuronReset::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        NeuronReset::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void NeuronReset::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<fisch::vx::word_access_type::Omnibus, NeuronReset::read_config_size_in_words> const&
        data);

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, NeuronReset const&)
{
	os << "NeuronReset()";
	return os;
}
#endif

bool NeuronReset::operator==(NeuronReset const&) const
{
	return true;
}

bool NeuronReset::operator!=(NeuronReset const& other) const
{
	return !(*this == other);
}


BlockPostPulse::BlockPostPulse() {}

template <typename AddressT>
std::array<AddressT, BlockPostPulse::read_config_size_in_words> BlockPostPulse::read_addresses(
    BlockPostPulse::coordinate_type const& /* neuron */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    BlockPostPulse::read_config_size_in_words>
BlockPostPulse::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, BlockPostPulse::read_config_size_in_words>
    BlockPostPulse::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename AddressT>
std::array<AddressT, BlockPostPulse::write_config_size_in_words> BlockPostPulse::write_addresses(
    BlockPostPulse::coordinate_type const& block)
{
	return {AddressT(neuron_post_pulse_addresses.at(block.toEnum()))};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    BlockPostPulse::write_config_size_in_words>
BlockPostPulse::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& block);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, BlockPostPulse::write_config_size_in_words>
    BlockPostPulse::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& block);

template <typename WordT>
std::array<WordT, BlockPostPulse::write_config_size_in_words> BlockPostPulse::encode() const
{
	// Value does not matter
	return {WordT()};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    BlockPostPulse::write_config_size_in_words>
BlockPostPulse::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, BlockPostPulse::write_config_size_in_words>
    BlockPostPulse::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void BlockPostPulse::decode(std::array<WordT, BlockPostPulse::read_config_size_in_words> const&)
{}

template SYMBOL_VISIBLE void
BlockPostPulse::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        BlockPostPulse::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void BlockPostPulse::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        BlockPostPulse::read_config_size_in_words> const& data);

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, BlockPostPulse const&)
{
	os << "BlockPostPulse()";
	return os;
}
#endif

bool BlockPostPulse::operator==(BlockPostPulse const&) const
{
	return true;
}

bool BlockPostPulse::operator!=(BlockPostPulse const& other) const
{
	return !(*this == other);
}


SpikeCounterRead::Count SpikeCounterRead::get_count() const
{
	return m_count;
}

void SpikeCounterRead::set_count(SpikeCounterRead::Count const cnt)
{
	m_count = cnt;
}

bool SpikeCounterRead::get_overflow() const
{
	return m_overflow;
}

void SpikeCounterRead::set_overflow(bool const ovrflw)
{
	m_overflow = ovrflw;
}

template <typename AddressT>
std::array<AddressT, SpikeCounterRead::read_config_size_in_words> SpikeCounterRead::read_addresses(
    SpikeCounterRead::coordinate_type const& neuron)
{
	auto const base_address =
	    spike_counter_read_sram_base_addresses[neuron.toSpikeCounterReadBlockOnDLS()];
	auto const neuron_coord = neuron.toSpikeCounterReadOnSpikeCounterReadBlock();

	// Multiply the neuron coordinate with the backend config size to get the true address
	return {AddressT(base_address + neuron_coord * 4)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SpikeCounterRead::read_config_size_in_words>
SpikeCounterRead::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SpikeCounterRead::read_config_size_in_words>
    SpikeCounterRead::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename AddressT>
std::array<AddressT, SpikeCounterRead::write_config_size_in_words>
SpikeCounterRead::write_addresses(SpikeCounterRead::coordinate_type const& /* neuron */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SpikeCounterRead::write_config_size_in_words>
SpikeCounterRead::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SpikeCounterRead::write_config_size_in_words>
    SpikeCounterRead::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename WordT>
std::array<WordT, SpikeCounterRead::write_config_size_in_words> SpikeCounterRead::encode() const
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    SpikeCounterRead::write_config_size_in_words>
SpikeCounterRead::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, SpikeCounterRead::write_config_size_in_words>
    SpikeCounterRead::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void SpikeCounterRead::decode(
    std::array<WordT, SpikeCounterRead::read_config_size_in_words> const& data)
{
	m_count = Count(data[0] & 0xFF);
	m_overflow = data[0] & 0x100;
}

template SYMBOL_VISIBLE void
SpikeCounterRead::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        SpikeCounterRead::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void SpikeCounterRead::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        SpikeCounterRead::read_config_size_in_words> const& data);

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, SpikeCounterRead const& config)
{
	std::stringstream ss;
	ss << std::boolalpha << config.m_overflow;
	os << "SpikeCounterRead(" << config.m_count << ", overflow: " << ss.str() << ")";
	return os;
}
#endif

bool SpikeCounterRead::operator==(SpikeCounterRead const& other) const
{
	return (m_count == other.get_count() && m_overflow == other.get_overflow());
}

bool SpikeCounterRead::operator!=(SpikeCounterRead const& other) const
{
	return !(*this == other);
}


SpikeCounterReset::SpikeCounterReset() {}

template <typename AddressT>
std::array<AddressT, SpikeCounterReset::read_config_size_in_words>
SpikeCounterReset::read_addresses(SpikeCounterReset::coordinate_type const& /* neuron */)
{
	return {};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SpikeCounterReset::read_config_size_in_words>
SpikeCounterReset::read_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SpikeCounterReset::read_config_size_in_words>
    SpikeCounterReset::read_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename AddressT>
std::array<AddressT, SpikeCounterReset::write_config_size_in_words>
SpikeCounterReset::write_addresses(SpikeCounterReset::coordinate_type const& neuron)
{
	auto const base_address =
	    spike_counter_reset_sram_base_addresses.at(neuron.toSpikeCounterResetBlockOnDLS());

	auto const neuron_coord = neuron.toSpikeCounterResetOnSpikeCounterResetBlock();

	// Multiply the neuron coordinate with the backend config size to get the true address
	return {AddressT(base_address + neuron_coord * 4)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SpikeCounterReset::write_config_size_in_words>
SpikeCounterReset::write_addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& cell);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SpikeCounterReset::write_config_size_in_words>
    SpikeCounterReset::write_addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& cell);

template <typename WordT>
std::array<WordT, SpikeCounterReset::write_config_size_in_words> SpikeCounterReset::encode() const
{
	// Value does not matter
	return {WordT()};
}

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    SpikeCounterReset::write_config_size_in_words>
SpikeCounterReset::encode<fisch::vx::word_access_type::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, SpikeCounterReset::write_config_size_in_words>
    SpikeCounterReset::encode<fisch::vx::word_access_type::Omnibus>() const;

template <typename WordT>
void SpikeCounterReset::decode(
    std::array<WordT, SpikeCounterReset::read_config_size_in_words> const&)
{}

template SYMBOL_VISIBLE void
SpikeCounterReset::decode<fisch::vx::word_access_type::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        SpikeCounterReset::read_config_size_in_words> const& data);

template SYMBOL_VISIBLE void SpikeCounterReset::decode<fisch::vx::word_access_type::Omnibus>(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        SpikeCounterReset::read_config_size_in_words> const& data);

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, SpikeCounterReset const&)
{
	os << "SpikeCounterReset()";
	return os;
}
#endif

bool SpikeCounterReset::operator==(SpikeCounterReset const&) const
{
	return true;
}

bool SpikeCounterReset::operator!=(SpikeCounterReset const& other) const
{
	return !(*this == other);
}


bool NeuronSRAMTimingConfig::operator==(NeuronSRAMTimingConfig const& other) const
{
	return static_cast<SRAMTimingConfig>(*this) == static_cast<SRAMTimingConfig>(other);
}

bool NeuronSRAMTimingConfig::operator!=(NeuronSRAMTimingConfig const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, NeuronSRAMTimingConfig const& config)
{
	os << "NeuronSRAMTimingConfig(" << config.get_read_delay() << ", "
	   << config.get_address_setup_time() << ", " << config.get_write_width() << ")";
	return os;
}
#endif

template <typename AddressT>
std::array<AddressT, NeuronSRAMTimingConfig::config_size_in_words>
NeuronSRAMTimingConfig::addresses(coordinate_type const& coord)
{
	return {AddressT(neuron_sram_timing_base_addresses.at(coord.toEnum())),
	        AddressT(neuron_sram_timing_base_addresses.at(coord.toEnum()) + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    NeuronSRAMTimingConfig::config_size_in_words>
NeuronSRAMTimingConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, NeuronSRAMTimingConfig::config_size_in_words>
    NeuronSRAMTimingConfig::addresses(coordinate_type const& coord);


bool NeuronBackendSRAMTimingConfig::operator==(NeuronBackendSRAMTimingConfig const& other) const
{
	return static_cast<SRAMTimingConfig>(*this) == static_cast<SRAMTimingConfig>(other);
}

bool NeuronBackendSRAMTimingConfig::operator!=(NeuronBackendSRAMTimingConfig const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
std::ostream& operator<<(std::ostream& os, NeuronBackendSRAMTimingConfig const& config)
{
	os << "NeuronBackendSRAMTimingConfig(" << config.get_read_delay() << ", "
	   << config.get_address_setup_time() << ", " << config.get_write_width() << ")";
	return os;
}
#endif

template <typename AddressT>
std::array<AddressT, NeuronBackendSRAMTimingConfig::config_size_in_words>
NeuronBackendSRAMTimingConfig::addresses(coordinate_type const& coord)
{
	return {AddressT(neuron_backend_sram_timing_base_addresses.at(coord.toEnum())),
	        AddressT(neuron_backend_sram_timing_base_addresses.at(coord.toEnum()) + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    NeuronBackendSRAMTimingConfig::config_size_in_words>
NeuronBackendSRAMTimingConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    NeuronBackendSRAMTimingConfig::config_size_in_words>
NeuronBackendSRAMTimingConfig::addresses(coordinate_type const& coord);

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::NeuronSRAMTimingConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::NeuronBackendSRAMTimingConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CommonNeuronBackendConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::NeuronReset)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::BlockPostPulse)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikeCounterRead)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikeCounterReset)
