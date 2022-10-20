#include "haldls/vx/fpga.h"

#include <iomanip>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/quad.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"
#include "hate/math.h"

namespace haldls {
namespace vx {

FPGADeviceDNA::FPGADeviceDNA(Value const value) : m_value(value) {}

FPGADeviceDNA::Value FPGADeviceDNA::get_value() const
{
	return m_value;
}

void FPGADeviceDNA::set_value(Value const value)
{
	m_value = value;
}

bool FPGADeviceDNA::operator==(FPGADeviceDNA const& other) const
{
	return m_value == other.m_value;
}

bool FPGADeviceDNA::operator!=(FPGADeviceDNA const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, FPGADeviceDNA const& config)
{
	os << "FPGADeviceDNA(" << config.m_value << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, FPGADeviceDNA::read_config_size_in_words>
FPGADeviceDNA::read_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(fpga_device_dna_base_address),
	        halco::hicann_dls::vx::OmnibusAddress(fpga_device_dna_base_address + 1)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, FPGADeviceDNA::write_config_size_in_words>
FPGADeviceDNA::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

namespace {

struct FPGADeviceDNABitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct {
			uint64_t low : 32;
			uint64_t high : 25;
			uint64_t /* unused */ : 7;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FPGADeviceDNABitfield() { u.raw = 0ul; }

	FPGADeviceDNABitfield(uint64_t data) { u.raw = data; }
};

} // namespace

std::array<fisch::vx::word_access_type::Omnibus, FPGADeviceDNA::write_config_size_in_words>
FPGADeviceDNA::encode() const
{
	return {};
}

void FPGADeviceDNA::decode(std::array<
                           fisch::vx::word_access_type::Omnibus,
                           FPGADeviceDNA::read_config_size_in_words> const& data)
{
	FPGADeviceDNABitfield bitfield;
	bitfield.u.m.low = data[1];
	bitfield.u.m.high = data[0];

	m_value = Value(bitfield.u.raw);
}

template <class Archive>
void FPGADeviceDNA::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(FPGADeviceDNA)

EventRecordingConfig::EventRecordingConfig() : m_enable_event_recording(false) {}

bool EventRecordingConfig::get_enable_event_recording() const
{
	return m_enable_event_recording;
}

void EventRecordingConfig::set_enable_event_recording(bool const value)
{
	m_enable_event_recording = value;
}

bool EventRecordingConfig::operator==(EventRecordingConfig const& other) const
{
	return m_enable_event_recording == other.m_enable_event_recording;
}

bool EventRecordingConfig::operator!=(EventRecordingConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, EventRecordingConfig const& config)
{
	std::stringstream ss;
	ss << "EventRecordingConfig(enable_event_recording: " << std::boolalpha
	   << config.m_enable_event_recording << ")";
	return (os << ss.str());
}

std::array<halco::hicann_dls::vx::OmnibusAddress, EventRecordingConfig::read_config_size_in_words>
EventRecordingConfig::read_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(event_recording_config_base_address)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, EventRecordingConfig::write_config_size_in_words>
EventRecordingConfig::write_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(event_recording_config_base_address)};
}

namespace {

struct EventRecordingConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t enable_event_recording : 1;
			uint32_t /* unused */           : 31;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	EventRecordingConfigBitfield() { u.raw = 0ul; }

	EventRecordingConfigBitfield(uint64_t data) { u.raw = data; }
};

} // namespace

void EventRecordingConfig::decode(std::array<
                                  fisch::vx::word_access_type::Omnibus,
                                  EventRecordingConfig::read_config_size_in_words> const& data)
{
	EventRecordingConfigBitfield bitfield;
	bitfield.u.m.enable_event_recording = data[0];

	m_enable_event_recording = bitfield.u.m.enable_event_recording;
}

std::array<fisch::vx::word_access_type::Omnibus, EventRecordingConfig::write_config_size_in_words>
EventRecordingConfig::encode() const
{
	EventRecordingConfigBitfield bitfield;
	bitfield.u.m.enable_event_recording = m_enable_event_recording;

	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw)};
}

template <class Archive>
void EventRecordingConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_event_recording));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(EventRecordingConfig)


InstructionTimeoutConfig::InstructionTimeoutConfig() :
    m_value(Value::fpga_clock_cycles_per_us * 10000)
{}

InstructionTimeoutConfig::Value InstructionTimeoutConfig::get_value() const
{
	return m_value;
}

void InstructionTimeoutConfig::set_value(Value const value)
{
	m_value = value;
}

bool InstructionTimeoutConfig::operator==(InstructionTimeoutConfig const& other) const
{
	return m_value == other.m_value;
}

bool InstructionTimeoutConfig::operator!=(InstructionTimeoutConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, InstructionTimeoutConfig const& config)
{
	os << "InstructionTimeoutConfig(" << config.m_value << ")";
	return os;
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    InstructionTimeoutConfig::read_config_size_in_words>
InstructionTimeoutConfig::read_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(instruction_timeout_config_base_address)};
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    InstructionTimeoutConfig::write_config_size_in_words>
InstructionTimeoutConfig::write_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(instruction_timeout_config_base_address)};
}

void InstructionTimeoutConfig::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        InstructionTimeoutConfig::read_config_size_in_words> const& data)
{
	m_value = Value(data[0].word.value());
}

std::array<
    fisch::vx::word_access_type::Omnibus,
    InstructionTimeoutConfig::write_config_size_in_words>
InstructionTimeoutConfig::encode() const
{
	return {fisch::vx::word_access_type::Omnibus(m_value)};
}

template <class Archive>
void InstructionTimeoutConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(InstructionTimeoutConfig)


ExternalPPUMemoryByte::Value ExternalPPUMemoryByte::get_value() const
{
	return m_value;
}

void ExternalPPUMemoryByte::set_value(ExternalPPUMemoryByte::Value const& value)
{
	m_value = value;
}

bool ExternalPPUMemoryByte::operator==(ExternalPPUMemoryByte const& other) const
{
	return m_value == other.m_value;
}

bool ExternalPPUMemoryByte::operator!=(ExternalPPUMemoryByte const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExternalPPUMemoryByte const& config)
{
	using namespace hate::math;
	auto const w = static_cast<uintmax_t>(config.m_value);
	std::stringstream out;
	out << "ExternalPPUMemoryByte(";
	out << "0x" << std::setfill('0') << std::hex
	    << std::setw(round_up_integer_division(num_bits(ExternalPPUMemoryByte::Value::max), 4))
	    << w;
	os << out.str() << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, ExternalPPUMemoryByte::config_size_in_words>
ExternalPPUMemoryByte::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    external_ppu_memory_base_address + (coord.toEnum() / sizeof(uint32_t)))};
}

std::array<fisch::vx::word_access_type::Omnibus, ExternalPPUMemoryByte::config_size_in_words>
ExternalPPUMemoryByte::encode(coordinate_type const& coord) const
{
	auto const byte_in_word = (sizeof(uint32_t) - 1) - (coord.toEnum() % sizeof(uint32_t));
	uint32_t const raw_value = static_cast<uint32_t>(m_value) << (byte_in_word * CHAR_BIT);
	fisch::vx::word_access_type::Omnibus::ByteEnables byte_enables{};
	byte_enables[byte_in_word] = true;
	return {fisch::vx::word_access_type::Omnibus(raw_value, byte_enables)};
}

void ExternalPPUMemoryByte::decode(
    coordinate_type const& coord,
    std::array<
        fisch::vx::word_access_type::Omnibus,
        ExternalPPUMemoryByte::config_size_in_words> const& data)
{
	auto const byte_in_word = (sizeof(uint32_t) - 1) - (coord.toEnum() % sizeof(uint32_t));
	m_value = Value((data[0] >> (byte_in_word * CHAR_BIT)) & 0xff);
}

template <typename Archive>
void ExternalPPUMemoryByte::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExternalPPUMemoryByte)

ExternalPPUMemoryQuad::ExternalPPUMemoryQuad() : m_quad(), m_enables({true, true, true, true}) {}

ExternalPPUMemoryQuad::Quad const& ExternalPPUMemoryQuad::get_quad() const
{
	return m_quad;
}

void ExternalPPUMemoryQuad::set_quad(ExternalPPUMemoryQuad::Quad const& quad)
{
	m_quad = quad;
}

ExternalPPUMemoryQuad::Enables const& ExternalPPUMemoryQuad::get_enables() const
{
	return m_enables;
}

void ExternalPPUMemoryQuad::set_enables(ExternalPPUMemoryQuad::Enables const& enables)
{
	m_enables = enables;
}

bool ExternalPPUMemoryQuad::operator==(ExternalPPUMemoryQuad const& other) const
{
	return (m_quad == other.m_quad) && (m_enables == other.m_enables);
}

bool ExternalPPUMemoryQuad::operator!=(ExternalPPUMemoryQuad const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExternalPPUMemoryQuad const& config)
{
	using namespace hate::math;
	std::stringstream out;
	out << "ExternalPPUMemoryQuad(\n";
	out << "\t quad: [" << hate::join_string(config.m_quad.begin(), config.m_quad.end(), ", ")
	    << "]\n";
	out << "\t enables: ["
	    << hate::join_string(config.m_enables.begin(), config.m_enables.end(), ", ") << "]\n";
	os << out.str() << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, ExternalPPUMemoryQuad::config_size_in_words>
ExternalPPUMemoryQuad::addresses(coordinate_type const& coord)
{
	return {
	    halco::hicann_dls::vx::OmnibusAddress(external_ppu_memory_base_address + coord.toEnum())};
}

namespace {

struct ExternalPPUMemoryQuadBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t byte_3 : 8;
			uint32_t byte_2 : 8;
			uint32_t byte_1 : 8;
			uint32_t byte_0 : 8;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	ExternalPPUMemoryQuadBitfield()
	{
		u.raw = 0ul;
	}

	ExternalPPUMemoryQuadBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<fisch::vx::word_access_type::Omnibus, ExternalPPUMemoryQuad::config_size_in_words>
ExternalPPUMemoryQuad::encode() const
{
	static_assert(
	    std::tuple_size<fisch::vx::word_access_type::Omnibus::ByteEnables>::value ==
	    std::tuple_size<Enables>::value);
	fisch::vx::word_access_type::Omnibus::ByteEnables byte_enables;
	std::copy(m_enables.begin(), m_enables.end(), byte_enables.rbegin());

	ExternalPPUMemoryQuadBitfield bitfield;
	bitfield.u.m.byte_0 = m_quad[halco::hicann_dls::vx::EntryOnQuad(0)];
	bitfield.u.m.byte_1 = m_quad[halco::hicann_dls::vx::EntryOnQuad(1)];
	bitfield.u.m.byte_2 = m_quad[halco::hicann_dls::vx::EntryOnQuad(2)];
	bitfield.u.m.byte_3 = m_quad[halco::hicann_dls::vx::EntryOnQuad(3)];
	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw, byte_enables)};
}

void ExternalPPUMemoryQuad::decode(std::array<
                                   fisch::vx::word_access_type::Omnibus,
                                   ExternalPPUMemoryQuad::config_size_in_words> const& data)
{
	static_assert(
	    std::tuple_size<fisch::vx::word_access_type::Omnibus::ByteEnables>::value ==
	    std::tuple_size<Enables>::value);
	std::copy(data[0].byte_enables.begin(), data[0].byte_enables.end(), m_enables.rbegin());
	ExternalPPUMemoryQuadBitfield bitfield(data[0]);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(0)] = Value(bitfield.u.m.byte_0);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(1)] = Value(bitfield.u.m.byte_1);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(2)] = Value(bitfield.u.m.byte_2);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(3)] = Value(bitfield.u.m.byte_3);
}

template <typename Archive>
void ExternalPPUMemoryQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_quad));
	ar(CEREAL_NVP(m_enables));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExternalPPUMemoryQuad)

SpikeIOConfig::SpikeIOConfig() :
    m_data_rate_scaler(DataRateScaler()),
    m_enable_tx(false),
    m_enable_rx(false),
    m_enable_internal_loopback(false)
{}

SpikeIOConfig::DataRateScaler const& SpikeIOConfig::get_data_rate_scaler() const
{
	return m_data_rate_scaler;
}

void SpikeIOConfig::set_data_rate_scaler(SpikeIOConfig::DataRateScaler const value)
{
	m_data_rate_scaler = value;
}

bool SpikeIOConfig::get_enable_tx() const
{
	return m_enable_tx;
}

void SpikeIOConfig::set_enable_tx(bool const value)
{
	m_enable_tx = value;
}

bool SpikeIOConfig::get_enable_rx() const
{
	return m_enable_rx;
}

void SpikeIOConfig::set_enable_rx(bool const value)
{
	m_enable_rx = value;
}

bool SpikeIOConfig::get_enable_internal_loopback() const
{
	return m_enable_internal_loopback;
}

void SpikeIOConfig::set_enable_internal_loopback(bool const value)
{
	m_enable_internal_loopback = value;
}

bool SpikeIOConfig::operator==(SpikeIOConfig const& other) const
{
	return (
	    (m_data_rate_scaler == other.m_data_rate_scaler) && (m_enable_tx == other.m_enable_tx) &&
	    (m_enable_rx == other.m_enable_rx) &&
	    (m_enable_internal_loopback == other.m_enable_internal_loopback));
}

bool SpikeIOConfig::operator!=(SpikeIOConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SpikeIOConfig const& config)
{
	std::stringstream ss;
	ss << "SpikeIOConfig(" << std::endl;
	ss << "\tdata_rate_scaler:         " << config.get_data_rate_scaler() << std::endl;
	ss << "\tenable_tx:                " << config.get_enable_tx() << std::endl;
	ss << "\tenable_rx:                " << config.get_enable_rx() << std::endl;
	ss << "\tenable_internal_loopback: " << config.get_enable_internal_loopback() << std::endl;
	ss << ")" << std::endl;
	return (os << ss.str());
}

std::array<halco::hicann_dls::vx::OmnibusAddress, SpikeIOConfig::config_size_in_words>
SpikeIOConfig::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(fpga_spikeio_config_base_address)};
}

namespace {

struct SpikeIOConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t data_rate_scaler         : 16;  // 00 - 15
			uint32_t enable_tx                : 1;   // 16
			uint32_t enable_rx                : 1;   // 17
			uint32_t enable_internal_loopback : 1;   // 18
			uint32_t /* unused */             : 13;  // 19 - 31
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SpikeIOConfigBitfield()
	{
		u.raw = 0ul;
	}

	SpikeIOConfigBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<fisch::vx::word_access_type::Omnibus, SpikeIOConfig::config_size_in_words>
SpikeIOConfig::encode() const
{
	SpikeIOConfigBitfield bitfield;
	bitfield.u.m.data_rate_scaler = m_data_rate_scaler;
	bitfield.u.m.enable_tx = m_enable_tx;
	bitfield.u.m.enable_rx = m_enable_rx;
	bitfield.u.m.enable_internal_loopback = m_enable_internal_loopback;
	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw)};
}

void SpikeIOConfig::decode(
    std::array<fisch::vx::word_access_type::Omnibus, SpikeIOConfig::config_size_in_words> const&
        data)
{
	SpikeIOConfigBitfield bitfield(data[0]);
	m_data_rate_scaler = DataRateScaler(bitfield.u.m.data_rate_scaler);
	m_enable_tx = bitfield.u.m.enable_tx;
	m_enable_rx = bitfield.u.m.enable_rx;
	m_enable_internal_loopback = bitfield.u.m.enable_internal_loopback;
}

template <typename Archive>
void SpikeIOConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_data_rate_scaler));
	ar(CEREAL_NVP(m_enable_tx));
	ar(CEREAL_NVP(m_enable_rx));
	ar(CEREAL_NVP(m_enable_internal_loopback));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SpikeIOConfig)

SpikeIOInputRoute::SpikeIOInputRoute() : m_target(SILENT) {}
SpikeIOInputRoute::SpikeIOInputRoute(halco::hicann_dls::vx::SpikeLabel label) : m_target(label) {}

halco::hicann_dls::vx::SpikeLabel const& SpikeIOInputRoute::get_target() const
{
	return m_target;
}

void SpikeIOInputRoute::set_target(halco::hicann_dls::vx::SpikeLabel const value)
{
	m_target = value;
}

bool SpikeIOInputRoute::operator==(SpikeIOInputRoute const& other) const
{
	return (m_target == other.m_target);
}

bool SpikeIOInputRoute::operator!=(SpikeIOInputRoute const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SpikeIOInputRoute const& config)
{
	std::stringstream ss;
	ss << "SpikeIOInputRoute(" << std::endl;
	ss << "\ttarget: " << config.get_target() << std::endl;
	ss << ")" << std::endl;
	return (os << ss.str());
}

std::array<halco::hicann_dls::vx::OmnibusAddress, SpikeIOInputRoute::config_size_in_words>
SpikeIOInputRoute::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    fpga_spikeio_input_routing_base_address + coord.toEnum())};
}

std::array<fisch::vx::word_access_type::Omnibus, SpikeIOInputRoute::config_size_in_words>
SpikeIOInputRoute::encode() const
{
	return {fisch::vx::word_access_type::Omnibus(
	    m_target & ((1u << hate::math::num_bits(halco::hicann_dls::vx::SpikeLabel::max)) - 1))};
}

void SpikeIOInputRoute::decode(
    std::array<fisch::vx::word_access_type::Omnibus, SpikeIOInputRoute::config_size_in_words> const&
        data)
{
	m_target = halco::hicann_dls::vx::SpikeLabel(
	    data[0] & ((1u << hate::math::num_bits(halco::hicann_dls::vx::SpikeLabel::max)) - 1));
}

template <typename Archive>
void SpikeIOInputRoute::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_target));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SpikeIOInputRoute)

SpikeIOOutputRoute::SpikeIOOutputRoute() : m_target(SILENT) {}
SpikeIOOutputRoute::SpikeIOOutputRoute(halco::hicann_dls::vx::SpikeIOAddress serial_address) :
    m_target(serial_address)
{}

halco::hicann_dls::vx::SpikeIOAddress const& SpikeIOOutputRoute::get_target() const
{
	return m_target;
}

void SpikeIOOutputRoute::set_target(halco::hicann_dls::vx::SpikeIOAddress const value)
{
	m_target = value;
}

bool SpikeIOOutputRoute::operator==(SpikeIOOutputRoute const& other) const
{
	return (m_target == other.m_target);
}

bool SpikeIOOutputRoute::operator!=(SpikeIOOutputRoute const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SpikeIOOutputRoute const& config)
{
	std::stringstream ss;
	ss << "SpikeIOOutputRoute(" << std::endl;
	ss << "\ttarget: " << config.get_target() << std::endl;
	ss << ")" << std::endl;
	return (os << ss.str());
}

std::array<halco::hicann_dls::vx::OmnibusAddress, SpikeIOOutputRoute::config_size_in_words>
SpikeIOOutputRoute::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    fpga_spikeio_output_routing_base_address + coord.toEnum())};
}

std::array<fisch::vx::word_access_type::Omnibus, SpikeIOOutputRoute::config_size_in_words>
SpikeIOOutputRoute::encode() const
{
	return {fisch::vx::word_access_type::Omnibus(
	    m_target & ((1u << hate::math::num_bits(halco::hicann_dls::vx::SpikeIOAddress::max)) - 1))};
}

void SpikeIOOutputRoute::decode(std::array<
                                fisch::vx::word_access_type::Omnibus,
                                SpikeIOOutputRoute::config_size_in_words> const& data)
{
	m_target = halco::hicann_dls::vx::SpikeIOAddress(
	    data[0] & ((1u << hate::math::num_bits(halco::hicann_dls::vx::SpikeIOAddress::max)) - 1));
}

template <typename Archive>
void SpikeIOOutputRoute::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_target));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SpikeIOOutputRoute)

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::FPGADeviceDNA, 0)
CEREAL_CLASS_VERSION(haldls::vx::EventRecordingConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::InstructionTimeoutConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExternalPPUMemoryByte, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExternalPPUMemoryQuad, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikeIOConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikeIOInputRoute, 0)
CEREAL_CLASS_VERSION(haldls::vx::SpikeIOOutputRoute, 0)
