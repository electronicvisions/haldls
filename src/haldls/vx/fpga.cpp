#include "haldls/vx/fpga.h"

#include <iomanip>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/quad.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"
#include "hate/math.h"

namespace haldls {
namespace vx {


FPGASystimeSyncNumRetries::FPGASystimeSyncNumRetries(Value const value) : m_value(value) {}

FPGASystimeSyncNumRetries::Value FPGASystimeSyncNumRetries::get_value() const
{
	return m_value;
}

void FPGASystimeSyncNumRetries::set_value(Value const value)
{
	m_value = value;
}

bool FPGASystimeSyncNumRetries::operator==(FPGASystimeSyncNumRetries const& other) const
{
	return m_value == other.m_value;
}

bool FPGASystimeSyncNumRetries::operator!=(FPGASystimeSyncNumRetries const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, FPGASystimeSyncNumRetries const& config)
{
	os << "FPGASystimeSyncNumRetries(" << config.m_value << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, FPGASystimeSyncNumRetries::config_size_in_words>
FPGASystimeSyncNumRetries::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(systime_num_tries_address)};
}

namespace {

struct FPGASystimeSyncNumRetriesBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct {
			uint32_t value      :  4;
			uint32_t /*unused*/ : 28;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FPGASystimeSyncNumRetriesBitfield()
	{
		u.raw = 0ul;
	}

	FPGASystimeSyncNumRetriesBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<fisch::vx::word_access_type::Omnibus, FPGASystimeSyncNumRetries::config_size_in_words>
FPGASystimeSyncNumRetries::encode() const
{
	FPGASystimeSyncNumRetriesBitfield bitfield;
	bitfield.u.m.value = m_value;

	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw)};
}

void FPGASystimeSyncNumRetries::decode(std::array<
                                       fisch::vx::word_access_type::Omnibus,
                                       FPGASystimeSyncNumRetries::config_size_in_words> const& data)
{
	FPGASystimeSyncNumRetriesBitfield bitfield;
	bitfield.u.m.value = data[0];

	m_value = Value(bitfield.u.raw);
}


FPGASystimeSyncLastRTT::FPGASystimeSyncLastRTT(Systime const value) : m_value(value) {}

FPGASystimeSyncLastRTT::Systime FPGASystimeSyncLastRTT::get_value() const
{
	return m_value;
}

void FPGASystimeSyncLastRTT::set_value(Systime const value)
{
	m_value = value;
}

bool FPGASystimeSyncLastRTT::operator==(FPGASystimeSyncLastRTT const& other) const
{
	return m_value == other.m_value;
}

bool FPGASystimeSyncLastRTT::operator!=(FPGASystimeSyncLastRTT const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, FPGASystimeSyncLastRTT const& config)
{
	os << "FPGASystimeSyncLastRTT(" << config.m_value << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, FPGASystimeSyncLastRTT::read_config_size_in_words>
FPGASystimeSyncLastRTT::read_addresses(coordinate_type const& /*coord*/)
{
	return {
	    halco::hicann_dls::vx::OmnibusAddress(systime_last_rtt_address),
	    halco::hicann_dls::vx::OmnibusAddress(systime_last_rtt_address + 1)};
}

std::
    array<halco::hicann_dls::vx::OmnibusAddress, FPGASystimeSyncLastRTT::write_config_size_in_words>
    FPGASystimeSyncLastRTT::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

namespace {

struct FPGASystimeSyncLastRTTBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct {
			uint64_t low          : 32;
			uint64_t high         : 11;
			uint64_t /* unused */ : 21;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FPGASystimeSyncLastRTTBitfield()
	{
		u.raw = 0ul;
	}

	FPGASystimeSyncLastRTTBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<fisch::vx::word_access_type::Omnibus, FPGASystimeSyncLastRTT::write_config_size_in_words>
FPGASystimeSyncLastRTT::encode() const
{
	return {};
}

void FPGASystimeSyncLastRTT::decode(std::array<
                                    fisch::vx::word_access_type::Omnibus,
                                    FPGASystimeSyncLastRTT::read_config_size_in_words> const& data)
{
	FPGASystimeSyncLastRTTBitfield bitfield;
	bitfield.u.m.low = data[1];
	bitfield.u.m.high = data[0];

	m_value = Systime(bitfield.u.raw);
}


FPGASystimeSyncLastAsicSystime::FPGASystimeSyncLastAsicSystime(Systime const value) : m_value(value)
{}

FPGASystimeSyncLastAsicSystime::Systime FPGASystimeSyncLastAsicSystime::get_value() const
{
	return m_value;
}

void FPGASystimeSyncLastAsicSystime::set_value(Systime const value)
{
	m_value = value;
}

bool FPGASystimeSyncLastAsicSystime::operator==(FPGASystimeSyncLastAsicSystime const& other) const
{
	return m_value == other.m_value;
}

bool FPGASystimeSyncLastAsicSystime::operator!=(FPGASystimeSyncLastAsicSystime const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, FPGASystimeSyncLastAsicSystime const& config)
{
	os << "FPGASystimeSyncLastAsicSystime(" << config.m_value << ")";
	return os;
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    FPGASystimeSyncLastAsicSystime::read_config_size_in_words>
FPGASystimeSyncLastAsicSystime::read_addresses(coordinate_type const& /*coord*/)
{
	return {
	    halco::hicann_dls::vx::OmnibusAddress(systime_last_asic_systime_address),
	    halco::hicann_dls::vx::OmnibusAddress(systime_last_asic_systime_address + 1)};
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    FPGASystimeSyncLastAsicSystime::write_config_size_in_words>
FPGASystimeSyncLastAsicSystime::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

namespace {

struct FPGASystimeSyncLastAsicSystimeBitfield
{
	union
	{
		uint64_t raw;
		// clang-format off
		struct {
			uint64_t low : 32;
			uint64_t high : 11;
			uint64_t /* unused */ : 21;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FPGASystimeSyncLastAsicSystimeBitfield()
	{
		u.raw = 0ul;
	}

	FPGASystimeSyncLastAsicSystimeBitfield(uint64_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<
    fisch::vx::word_access_type::Omnibus,
    FPGASystimeSyncLastAsicSystime::write_config_size_in_words>
FPGASystimeSyncLastAsicSystime::encode() const
{
	return {};
}

void FPGASystimeSyncLastAsicSystime::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        FPGASystimeSyncLastAsicSystime::read_config_size_in_words> const& data)
{
	FPGASystimeSyncLastAsicSystimeBitfield bitfield;
	bitfield.u.m.low = data[1];
	bitfield.u.m.high = data[0];

	m_value = Systime(bitfield.u.raw);
}


FPGASystimeSyncActiveState::FPGASystimeSyncActiveState(bool const active_state) :
    m_active_state(active_state)
{}

bool FPGASystimeSyncActiveState::get_active_state() const
{
	return m_active_state;
}

void FPGASystimeSyncActiveState::set_active_state(bool const active_state)
{
	m_active_state = active_state;
}

bool FPGASystimeSyncActiveState::operator==(FPGASystimeSyncActiveState const& other) const
{
	return m_active_state == other.m_active_state;
}

bool FPGASystimeSyncActiveState::operator!=(FPGASystimeSyncActiveState const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, FPGASystimeSyncActiveState const& config)
{
	os << "FPGASystimeSyncActiveState(" << std::boolalpha << config.m_active_state << ")";
	return os;
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    FPGASystimeSyncActiveState::read_config_size_in_words>
FPGASystimeSyncActiveState::read_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(systime_active_state_address)};
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    FPGASystimeSyncActiveState::write_config_size_in_words>
FPGASystimeSyncActiveState::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

namespace {

struct FPGASystimeSyncActiveStateBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct {
			uint32_t active : 1;
			uint32_t /* unused */ : 31;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	FPGASystimeSyncActiveStateBitfield()
	{
		u.raw = 0ul;
	}

	FPGASystimeSyncActiveStateBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<
    fisch::vx::word_access_type::Omnibus,
    FPGASystimeSyncActiveState::write_config_size_in_words>
FPGASystimeSyncActiveState::encode() const
{
	return {};
}

void FPGASystimeSyncActiveState::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        FPGASystimeSyncActiveState::read_config_size_in_words> const& data)
{
	FPGASystimeSyncActiveStateBitfield bitfield;
	bitfield.u.m.active = data[0];

	m_active_state = static_cast<bool>(bitfield.u.raw);
}


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


SystimeCorrectionBarrierConfig::SystimeCorrectionBarrierConfig() : m_enable_interrupt(false) {}

bool SystimeCorrectionBarrierConfig::get_enable_interrupt() const
{
	return m_enable_interrupt;
}

void SystimeCorrectionBarrierConfig::set_enable_interrupt(bool const enable_interrupt)
{
	m_enable_interrupt = enable_interrupt;
}

bool SystimeCorrectionBarrierConfig::operator==(SystimeCorrectionBarrierConfig const& other) const
{
	return m_enable_interrupt == other.m_enable_interrupt;
}

bool SystimeCorrectionBarrierConfig::operator!=(SystimeCorrectionBarrierConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SystimeCorrectionBarrierConfig const& config)
{
	os << "SystimeCorrectionBarrierConfig(" << std::boolalpha << config.m_enable_interrupt << ")";
	return os;
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    SystimeCorrectionBarrierConfig::read_config_size_in_words>
SystimeCorrectionBarrierConfig::read_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(systime_correction_barrier_config)};
}

std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    SystimeCorrectionBarrierConfig::write_config_size_in_words>
SystimeCorrectionBarrierConfig::write_addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusAddress(systime_correction_barrier_config)};
}

namespace {

struct SystimeCorrectionBarrierConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t enable_interrupt : 1;   // 0
			uint32_t /* unused */             : 31;  // 1 - 31
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	SystimeCorrectionBarrierConfigBitfield()
	{
		u.raw = 0ul;
	}

	SystimeCorrectionBarrierConfigBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<
    fisch::vx::word_access_type::Omnibus,
    SystimeCorrectionBarrierConfig::write_config_size_in_words>
SystimeCorrectionBarrierConfig::encode() const
{
	SystimeCorrectionBarrierConfigBitfield bitfield;
	bitfield.u.m.enable_interrupt = m_enable_interrupt;
	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw)};
}

void SystimeCorrectionBarrierConfig::decode(
    std::array<
        fisch::vx::word_access_type::Omnibus,
        SystimeCorrectionBarrierConfig::read_config_size_in_words> const& data)
{
	SystimeCorrectionBarrierConfigBitfield bitfield(data[0]);
	m_enable_interrupt = bitfield.u.m.enable_interrupt;
}


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
	fisch::vx::word_access_type::Omnibus::ByteEnables byte_enables{false, false, false, false};
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
	out << "\t quad: [" << hate::join(config.m_quad.begin(), config.m_quad.end(), ", ") << "]\n";
	out << "\t enables: [" << hate::join(config.m_enables.begin(), config.m_enables.end(), ", ")
	    << "]\n";
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


ExternalPPUDRAMMemoryByte::Value ExternalPPUDRAMMemoryByte::get_value() const
{
	return m_value;
}

void ExternalPPUDRAMMemoryByte::set_value(ExternalPPUDRAMMemoryByte::Value const& value)
{
	m_value = value;
}

bool ExternalPPUDRAMMemoryByte::operator==(ExternalPPUDRAMMemoryByte const& other) const
{
	return m_value == other.m_value;
}

bool ExternalPPUDRAMMemoryByte::operator!=(ExternalPPUDRAMMemoryByte const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExternalPPUDRAMMemoryByte const& config)
{
	using namespace hate::math;
	auto const w = static_cast<uintmax_t>(config.m_value);
	std::stringstream out;
	out << "ExternalPPUDRAMMemoryByte(";
	out << "0x" << std::setfill('0') << std::hex
	    << std::setw(round_up_integer_division(num_bits(ExternalPPUDRAMMemoryByte::Value::max), 4))
	    << w;
	os << out.str() << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, ExternalPPUDRAMMemoryByte::config_size_in_words>
ExternalPPUDRAMMemoryByte::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    external_ppu_dram_memory_base_address + (coord.toEnum() / sizeof(uint32_t)))};
}

std::array<fisch::vx::word_access_type::Omnibus, ExternalPPUDRAMMemoryByte::config_size_in_words>
ExternalPPUDRAMMemoryByte::encode(coordinate_type const& coord) const
{
	auto const byte_in_word = (sizeof(uint32_t) - 1) - (coord.toEnum() % sizeof(uint32_t));
	uint32_t const raw_value = static_cast<uint32_t>(m_value) << (byte_in_word * CHAR_BIT);
	fisch::vx::word_access_type::Omnibus::ByteEnables byte_enables{false, false, false, false};
	byte_enables[byte_in_word] = true;
	return {fisch::vx::word_access_type::Omnibus(raw_value, byte_enables)};
}

void ExternalPPUDRAMMemoryByte::decode(
    coordinate_type const& coord,
    std::array<
        fisch::vx::word_access_type::Omnibus,
        ExternalPPUDRAMMemoryByte::config_size_in_words> const& data)
{
	auto const byte_in_word = (sizeof(uint32_t) - 1) - (coord.toEnum() % sizeof(uint32_t));
	m_value = Value((data[0] >> (byte_in_word * CHAR_BIT)) & 0xff);
}


ExternalPPUDRAMMemoryQuad::ExternalPPUDRAMMemoryQuad() :
    m_quad(), m_enables({true, true, true, true})
{}

ExternalPPUDRAMMemoryQuad::Quad const& ExternalPPUDRAMMemoryQuad::get_quad() const
{
	return m_quad;
}

void ExternalPPUDRAMMemoryQuad::set_quad(ExternalPPUDRAMMemoryQuad::Quad const& quad)
{
	m_quad = quad;
}

ExternalPPUDRAMMemoryQuad::Enables const& ExternalPPUDRAMMemoryQuad::get_enables() const
{
	return m_enables;
}

void ExternalPPUDRAMMemoryQuad::set_enables(ExternalPPUDRAMMemoryQuad::Enables const& enables)
{
	m_enables = enables;
}

bool ExternalPPUDRAMMemoryQuad::operator==(ExternalPPUDRAMMemoryQuad const& other) const
{
	return (m_quad == other.m_quad) && (m_enables == other.m_enables);
}

bool ExternalPPUDRAMMemoryQuad::operator!=(ExternalPPUDRAMMemoryQuad const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ExternalPPUDRAMMemoryQuad const& config)
{
	using namespace hate::math;
	std::stringstream out;
	out << "ExternalPPUDRAMMemoryQuad(\n";
	out << "\t quad: [" << hate::join(config.m_quad.begin(), config.m_quad.end(), ", ") << "]\n";
	out << "\t enables: [" << hate::join(config.m_enables.begin(), config.m_enables.end(), ", ")
	    << "]\n";
	os << out.str() << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, ExternalPPUDRAMMemoryQuad::config_size_in_words>
ExternalPPUDRAMMemoryQuad::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusAddress(
	    external_ppu_dram_memory_base_address + coord.toEnum())};
}

namespace {

struct ExternalPPUDRAMMemoryQuadBitfield
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

	ExternalPPUDRAMMemoryQuadBitfield()
	{
		u.raw = 0ul;
	}

	ExternalPPUDRAMMemoryQuadBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::array<fisch::vx::word_access_type::Omnibus, ExternalPPUDRAMMemoryQuad::config_size_in_words>
ExternalPPUDRAMMemoryQuad::encode() const
{
	static_assert(
	    std::tuple_size<fisch::vx::word_access_type::Omnibus::ByteEnables>::value ==
	    std::tuple_size<Enables>::value);
	fisch::vx::word_access_type::Omnibus::ByteEnables byte_enables;
	std::copy(m_enables.begin(), m_enables.end(), byte_enables.rbegin());

	ExternalPPUDRAMMemoryQuadBitfield bitfield;
	bitfield.u.m.byte_0 = m_quad[halco::hicann_dls::vx::EntryOnQuad(0)];
	bitfield.u.m.byte_1 = m_quad[halco::hicann_dls::vx::EntryOnQuad(1)];
	bitfield.u.m.byte_2 = m_quad[halco::hicann_dls::vx::EntryOnQuad(2)];
	bitfield.u.m.byte_3 = m_quad[halco::hicann_dls::vx::EntryOnQuad(3)];
	return {fisch::vx::word_access_type::Omnibus(bitfield.u.raw, byte_enables)};
}

void ExternalPPUDRAMMemoryQuad::decode(std::array<
                                       fisch::vx::word_access_type::Omnibus,
                                       ExternalPPUDRAMMemoryQuad::config_size_in_words> const& data)
{
	static_assert(
	    std::tuple_size<fisch::vx::word_access_type::Omnibus::ByteEnables>::value ==
	    std::tuple_size<Enables>::value);
	std::copy(data[0].byte_enables.begin(), data[0].byte_enables.end(), m_enables.rbegin());
	ExternalPPUDRAMMemoryQuadBitfield bitfield(data[0]);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(0)] = Value(bitfield.u.m.byte_0);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(1)] = Value(bitfield.u.m.byte_1);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(2)] = Value(bitfield.u.m.byte_2);
	m_quad[halco::hicann_dls::vx::EntryOnQuad(3)] = Value(bitfield.u.m.byte_3);
}


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

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::FPGADeviceDNA)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::EventRecordingConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::InstructionTimeoutConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SystimeCorrectionBarrierConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ExternalPPUMemoryByte)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ExternalPPUMemoryQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ExternalPPUDRAMMemoryByte)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::ExternalPPUDRAMMemoryQuad)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikeIOConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikeIOInputRoute)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::SpikeIOOutputRoute)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::FPGASystimeSyncLastAsicSystime)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::FPGASystimeSyncLastRTT)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::FPGASystimeSyncActiveState)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::FPGASystimeSyncNumRetries)
