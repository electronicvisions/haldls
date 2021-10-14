#include "haldls/vx/fpga.h"

#include <iomanip>

#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/quad.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"
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

std::array<fisch::vx::Omnibus, FPGADeviceDNA::write_config_size_in_words> FPGADeviceDNA::encode()
    const
{
	return {};
}

void FPGADeviceDNA::decode(
    std::array<fisch::vx::Omnibus, FPGADeviceDNA::read_config_size_in_words> const& data)
{
	FPGADeviceDNABitfield bitfield;
	bitfield.u.m.low = data[1].get();
	bitfield.u.m.high = data[0].get();

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
EventRecordingConfig::read_addresses(coordinate_type const& /*coord*/) const
{
	return {halco::hicann_dls::vx::OmnibusAddress(event_recording_config_base_address)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, EventRecordingConfig::write_config_size_in_words>
EventRecordingConfig::write_addresses(coordinate_type const& /*coord*/) const
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

void EventRecordingConfig::decode(
    std::array<fisch::vx::Omnibus, EventRecordingConfig::read_config_size_in_words> const& data)
{
	EventRecordingConfigBitfield bitfield;
	bitfield.u.m.enable_event_recording = data[0].get();

	m_enable_event_recording = bitfield.u.m.enable_event_recording;
}

std::array<fisch::vx::Omnibus, EventRecordingConfig::write_config_size_in_words>
EventRecordingConfig::encode() const
{
	EventRecordingConfigBitfield bitfield;
	bitfield.u.m.enable_event_recording = m_enable_event_recording;

	return {fisch::vx::Omnibus(fisch::vx::Omnibus::Value(static_cast<uint32_t>(bitfield.u.raw)))};
}

template <class Archive>
void EventRecordingConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_event_recording));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(EventRecordingConfig)


ExternalPPUMemoryByte::ExternalPPUMemoryByte(ExternalPPUMemoryByte::Value const value) :
    m_value(value)
{}

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

std::array<fisch::vx::Omnibus, ExternalPPUMemoryByte::config_size_in_words>
ExternalPPUMemoryByte::encode(coordinate_type const& coord) const
{
	auto const byte_in_word = (sizeof(uint32_t) - 1) - (coord.toEnum() % sizeof(uint32_t));
	uint32_t const raw_value = static_cast<uint32_t>(m_value) << (byte_in_word * CHAR_BIT);
	fisch::vx::Omnibus::Value::ByteEnables byte_enables{};
	byte_enables[byte_in_word] = true;
	return {fisch::vx::Omnibus(fisch::vx::Omnibus::Value(raw_value, byte_enables))};
}

void ExternalPPUMemoryByte::decode(
    coordinate_type const& coord,
    std::array<fisch::vx::Omnibus, ExternalPPUMemoryByte::config_size_in_words> const& data)
{
	auto const byte_in_word = (sizeof(uint32_t) - 1) - (coord.toEnum() % sizeof(uint32_t));
	m_value = Value((data[0].get() >> (byte_in_word * CHAR_BIT)) & 0xff);
}

template <typename Archive>
void ExternalPPUMemoryByte::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_value));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(ExternalPPUMemoryByte)

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::FPGADeviceDNA, 0)
CEREAL_CLASS_VERSION(haldls::vx::EventRecordingConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::ExternalPPUMemoryByte, 0)
