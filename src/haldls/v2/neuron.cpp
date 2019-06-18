#include "haldls/v2/neuron.h"

#include "halco/common/cerealization_geometry.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace v2 {

CommonNeuronConfig::CommonNeuronConfig()
	: m_digital_out(false),
	  m_post_correlation_signal_length(0),
	  m_external_correlation_signal(false),
	  m_inhibit_spike_comparator(false)
{}

bool CommonNeuronConfig::get_enable_digital_out() const
{
	return m_digital_out;
}

void CommonNeuronConfig::set_enable_digital_out(bool const value)
{
	m_digital_out = value;
}

CommonNeuronConfig::PostCorrelationSignalLength
CommonNeuronConfig::get_post_correlation_signal_length() const
{
	return m_post_correlation_signal_length;
}

void CommonNeuronConfig::set_post_correlation_signal_length(
	CommonNeuronConfig::PostCorrelationSignalLength const& value)
{
	m_post_correlation_signal_length = value;
}

bool CommonNeuronConfig::get_enable_external_post_correlation_signal() const
{
	return m_external_correlation_signal;
}

void CommonNeuronConfig::set_enable_external_post_correlation_signal(bool const value)
{
	m_external_correlation_signal = value;
}

bool CommonNeuronConfig::get_inhibit_spike_comparator() const
{
	return m_inhibit_spike_comparator;
}

void CommonNeuronConfig::set_inhibit_spike_comparator(bool const value)
{
	m_inhibit_spike_comparator = value;
}

bool CommonNeuronConfig::operator==(CommonNeuronConfig const& b) const
{
	return m_digital_out == b.get_enable_digital_out() &&
		   m_post_correlation_signal_length == b.get_post_correlation_signal_length() &&
		   m_external_correlation_signal == b.get_enable_external_post_correlation_signal() &&
		   m_inhibit_spike_comparator == b.get_inhibit_spike_comparator();
}

bool CommonNeuronConfig::operator!=(CommonNeuronConfig const& b) const
{
	return !(*this == b);
}

namespace {

struct CommonNeuronConfigBitfield {
	union {
		hardware_word_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type digital_out                    :  1; // 0
			hardware_word_type post_correlation_signal_length :  4; // 1 - 4
			hardware_word_type external_correlation_signal    :  1; // 5
			hardware_word_type inhibit_spike_comparator       :  1; // 6
			hardware_word_type                                : 25;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CommonNeuronConfigBitfield() {
		u.raw = 0u;
	}

	CommonNeuronConfigBitfield(hardware_word_type data) {
		u.raw = data & 0b0111'1111ul;
	}
};

} // namespace

std::array<hardware_address_type, CommonNeuronConfig::config_size_in_words> CommonNeuronConfig::addresses(CommonNeuronConfig::coordinate_type const&) const
{
	return {{0x1a000000}};
}

std::array<hardware_word_type, CommonNeuronConfig::config_size_in_words> CommonNeuronConfig::encode() const
{
	CommonNeuronConfigBitfield bitfield;
	bitfield.u.m.digital_out = m_digital_out;
	bitfield.u.m.post_correlation_signal_length = m_post_correlation_signal_length;
	bitfield.u.m.external_correlation_signal = m_external_correlation_signal;
	bitfield.u.m.inhibit_spike_comparator = m_inhibit_spike_comparator;
	return {{bitfield.u.raw}};
}

void CommonNeuronConfig::decode(std::array<hardware_word_type, CommonNeuronConfig::config_size_in_words> const& data)
{
	CommonNeuronConfigBitfield bitfield(data[0]);

	m_digital_out = bitfield.u.m.digital_out;
	m_post_correlation_signal_length =
		PostCorrelationSignalLength(bitfield.u.m.post_correlation_signal_length);
	m_external_correlation_signal = bitfield.u.m.external_correlation_signal;
	m_inhibit_spike_comparator = bitfield.u.m.inhibit_spike_comparator;
}

template <class Archive>
void CommonNeuronConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_digital_out));
	ar(CEREAL_NVP(m_post_correlation_signal_length));
	ar(CEREAL_NVP(m_external_correlation_signal));
	ar(CEREAL_NVP(m_inhibit_spike_comparator));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CommonNeuronConfig)

NeuronDigitalConfig::NeuronDigitalConfig()
	: m_synapse_input_exc(false),
	  m_synapse_input_inh(false),
	  m_leak_high_conductance(false),
	  m_leak(true),
	  m_bigcap(true),
	  m_smallcap(true),
	  m_fire_out_mode(FireOutMode::disabled),
	  m_mux_readout_mode(MuxReadoutMode::v_mem),
	  m_unbuffered_readout(false),
	  m_buffered_readout(false)
{}

bool NeuronDigitalConfig::get_enable_synapse_input_excitatory() const
{
	return m_synapse_input_exc;
}

void NeuronDigitalConfig::set_enable_synapse_input_excitatory(bool const value)
{
	m_synapse_input_exc = value;
}

bool NeuronDigitalConfig::get_enable_synapse_input_inhibitory() const
{
	return m_synapse_input_inh;
}

void NeuronDigitalConfig::set_enable_synapse_input_inhibitory(bool const value)
{
	m_synapse_input_inh = value;
}

bool NeuronDigitalConfig::get_enable_high_conductance_leak() const
{
	return m_leak_high_conductance;
}

void NeuronDigitalConfig::set_enable_high_conductance_leak(bool const value)
{
	m_leak_high_conductance = value;
}

bool NeuronDigitalConfig::get_enable_leak() const
{
	return m_leak;
}

void NeuronDigitalConfig::set_enable_leak(bool const value)
{
	m_leak = value;
}

bool NeuronDigitalConfig::get_enable_bigcap() const
{
	return m_bigcap;
}

void NeuronDigitalConfig::set_enable_bigcap(bool const value)
{
	m_bigcap = value;
}

bool NeuronDigitalConfig::get_enable_smallcap() const
{
	return m_smallcap;
}

void NeuronDigitalConfig::set_enable_smallcap(bool const value)
{
	m_smallcap = value;
}

NeuronDigitalConfig::FireOutMode NeuronDigitalConfig::get_fire_out_mode() const
{
	return m_fire_out_mode;
}

void NeuronDigitalConfig::set_fire_out_mode(NeuronDigitalConfig::FireOutMode const value)
{
	m_fire_out_mode = value;
}

NeuronDigitalConfig::MuxReadoutMode NeuronDigitalConfig::get_mux_readout_mode() const
{
	return m_mux_readout_mode;
}

void NeuronDigitalConfig::set_mux_readout_mode(
	NeuronDigitalConfig::MuxReadoutMode const value)
{
	m_mux_readout_mode = value;
}

bool NeuronDigitalConfig::get_enable_unbuffered_readout() const
{
	return m_unbuffered_readout;
}

void NeuronDigitalConfig::set_enable_unbuffered_readout(bool value)
{
	m_unbuffered_readout = value;
}

bool NeuronDigitalConfig::get_enable_buffered_readout() const
{
	return m_buffered_readout;
}

void NeuronDigitalConfig::set_enable_buffered_readout(
	bool value, hate::Passkey<Chip, PlaybackProgram> const& /*passkey*/)
{
	m_buffered_readout = value;
}

bool NeuronDigitalConfig::operator==(NeuronDigitalConfig const& other) const
{
	// clang-format off
	return (
		m_synapse_input_exc == other.get_enable_synapse_input_excitatory() &&
		m_synapse_input_inh == other.get_enable_synapse_input_inhibitory() &&
		m_leak_high_conductance == other.get_enable_high_conductance_leak() &&
		m_leak == other.get_enable_leak() &&
		m_bigcap == other.get_enable_bigcap() &&
		m_smallcap == other.get_enable_smallcap() &&
		m_fire_out_mode == other.get_fire_out_mode() &&
		m_mux_readout_mode == other.get_mux_readout_mode() &&
		m_unbuffered_readout == other.get_enable_unbuffered_readout() &&
		m_buffered_readout == other.get_enable_buffered_readout());
	// clang-format on
}

bool NeuronDigitalConfig::operator!=(NeuronDigitalConfig const& other) const
{
	return !(*this == other);
}

namespace {

struct NeuronDigitalConfigBitfield {
	union {
		hardware_word_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			hardware_word_type                         :  1; // 0
			hardware_word_type fire_out_mode           :  2; // 1, 2
			hardware_word_type                         :  1; // 3
			hardware_word_type synapse_input_exc       :  1; // 4
			hardware_word_type synapse_input_inh       :  1; // 5
			hardware_word_type leak_high_conductance   :  1; // 6
			hardware_word_type unbuffered_readout      :  1; // 7
			hardware_word_type buffered_readout        :  1; // 8
			hardware_word_type                         :  1; // 9
			hardware_word_type leak                    :  1; // 10
			hardware_word_type                         :  1; // 11
			hardware_word_type                         :  1; // 12
			hardware_word_type                         :  1; // 13
			hardware_word_type mux_readout_mode        :  2; // 14, 15
			hardware_word_type bigcap                  :  1; // 16
			hardware_word_type smallcap                :  1; // 17
			hardware_word_type                         : 14; // 18-32
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	NeuronDigitalConfigBitfield() {
		u.raw = 0u;
	}

	NeuronDigitalConfigBitfield(hardware_word_type data) {
		u.raw = data & 0b0011'1100'0101'1111'0110ul;
	}
};

} // namespace

std::array<hardware_address_type, NeuronDigitalConfig::config_size_in_words> NeuronDigitalConfig::addresses(
	coordinate_type const& neuron) const
{
	hardware_word_type constexpr base_address = 0x1a000001;
	return {{static_cast<hardware_address_type>(base_address + neuron)}};
}

std::array<hardware_word_type, NeuronDigitalConfig::config_size_in_words> NeuronDigitalConfig::encode() const
{
	NeuronDigitalConfigBitfield bitfield;
	bitfield.u.m.synapse_input_exc = m_synapse_input_exc;
	bitfield.u.m.synapse_input_inh = m_synapse_input_inh;
	bitfield.u.m.leak_high_conductance = m_leak_high_conductance;
	bitfield.u.m.leak = m_leak;
	bitfield.u.m.bigcap = m_bigcap;
	bitfield.u.m.smallcap = m_smallcap;
	bitfield.u.m.fire_out_mode = static_cast<hardware_word_type>(m_fire_out_mode);
	bitfield.u.m.mux_readout_mode = static_cast<hardware_word_type>(m_mux_readout_mode);
	bitfield.u.m.unbuffered_readout = m_unbuffered_readout;
	bitfield.u.m.buffered_readout = m_buffered_readout;
	return {{bitfield.u.raw}};
}

void NeuronDigitalConfig::decode(std::array<hardware_word_type, NeuronDigitalConfig::config_size_in_words> const& data)
{
	NeuronDigitalConfigBitfield bitfield(data[0]);
	m_synapse_input_exc = bitfield.u.m.synapse_input_exc;
	m_synapse_input_inh = bitfield.u.m.synapse_input_inh;
	m_leak_high_conductance = bitfield.u.m.leak_high_conductance;
	m_leak = bitfield.u.m.leak;
	m_bigcap = bitfield.u.m.bigcap;
	m_smallcap = bitfield.u.m.smallcap;
	m_fire_out_mode = FireOutMode(bitfield.u.m.fire_out_mode);
	m_mux_readout_mode = MuxReadoutMode(bitfield.u.m.mux_readout_mode);
	m_unbuffered_readout = bitfield.u.m.unbuffered_readout;
	m_buffered_readout = bitfield.u.m.buffered_readout;
}


template <class Archive>
void NeuronDigitalConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_synapse_input_exc));
	ar(CEREAL_NVP(m_synapse_input_inh));
	ar(CEREAL_NVP(m_leak_high_conductance));
	ar(CEREAL_NVP(m_leak));
	ar(CEREAL_NVP(m_bigcap));
	ar(CEREAL_NVP(m_smallcap));
	ar(CEREAL_NVP(m_fire_out_mode));
	ar(CEREAL_NVP(m_mux_readout_mode));
	ar(CEREAL_NVP(m_unbuffered_readout));
	ar(CEREAL_NVP(m_buffered_readout));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(NeuronDigitalConfig)

} // namespace v2
} // namespace haldls
