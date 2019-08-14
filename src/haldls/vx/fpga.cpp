#include "haldls/vx/fpga.h"

#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

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
	os << config.m_value << std::endl;
	return print_words_for_each_backend(os, config);
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, FPGADeviceDNA::read_config_size_in_words>
FPGADeviceDNA::read_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusFPGAAddress(fpga_device_dna_base_address),
	        halco::hicann_dls::vx::OmnibusFPGAAddress(fpga_device_dna_base_address + 1)};
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, FPGADeviceDNA::write_config_size_in_words>
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

std::array<fisch::vx::OmnibusFPGA, FPGADeviceDNA::write_config_size_in_words>
FPGADeviceDNA::encode() const
{
	return {};
}

void FPGADeviceDNA::decode(
    std::array<fisch::vx::OmnibusFPGA, FPGADeviceDNA::read_config_size_in_words> const& data)
{
	FPGADeviceDNABitfield bitfield;
	bitfield.u.m.low = data[1].get();
	bitfield.u.m.high = data[0].get();

	m_value = Value(bitfield.u.raw);
}

template <class Archive>
void FPGADeviceDNA::serialize(Archive& ar)
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
	os << config.m_enable_event_recording << std::endl;
	return print_words_for_each_backend(os, config);
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, EventRecordingConfig::read_config_size_in_words>
EventRecordingConfig::read_addresses(coordinate_type const& /*coord*/) const
{
	return {halco::hicann_dls::vx::OmnibusFPGAAddress(event_recording_config_base_address)};
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, EventRecordingConfig::write_config_size_in_words>
EventRecordingConfig::write_addresses(coordinate_type const& /*coord*/) const
{
	return {halco::hicann_dls::vx::OmnibusFPGAAddress(event_recording_config_base_address)};
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
    std::array<fisch::vx::OmnibusFPGA, EventRecordingConfig::read_config_size_in_words> const& data)
{
	EventRecordingConfigBitfield bitfield;
	bitfield.u.m.enable_event_recording = data[0].get();

	m_enable_event_recording = bitfield.u.m.enable_event_recording;
}

std::array<fisch::vx::OmnibusFPGA, EventRecordingConfig::write_config_size_in_words> EventRecordingConfig::encode() const
{
	EventRecordingConfigBitfield bitfield;
	bitfield.u.m.enable_event_recording = m_enable_event_recording;

	return {fisch::vx::OmnibusFPGA(fisch::vx::OmnibusFPGA::Value(static_cast<uint32_t>(bitfield.u.raw)))};
}

template <class Archive>
void EventRecordingConfig::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_event_recording));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(EventRecordingConfig)

} // namespace vx
} // namespace haldls
