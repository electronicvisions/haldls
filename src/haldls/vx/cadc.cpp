#include "haldls/vx/cadc.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/expand_word.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"


namespace haldls {
namespace vx {

CADCChannelConfig::CADCChannelConfig() : m_offset() {}

typename CADCChannelConfig::Offset CADCChannelConfig::get_offset() const
{
	return m_offset;
}

void CADCChannelConfig::set_offset(Offset const value)
{
	m_offset = value;
}

bool CADCChannelConfig::operator==(CADCChannelConfig const& other) const
{
	return m_offset == other.m_offset;
}

bool CADCChannelConfig::operator!=(CADCChannelConfig const& other) const
{
	return !(*this == other);
}

#ifndef __ppu__
template std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CADCChannelConfig::config_size_in_words>
CADCChannelConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CADCChannelConfig::config_size_in_words>
    CADCChannelConfig::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::OmnibusChipOverJTAG, CADCChannelConfig::config_size_in_words>
    CADCChannelConfig::encode() const;
template std::array<fisch::vx::word_access_type::Omnibus, CADCChannelConfig::config_size_in_words>
CADCChannelConfig::encode() const;

template void CADCChannelConfig::decode(std::array<
                                        fisch::vx::word_access_type::OmnibusChipOverJTAG,
                                        CADCChannelConfig::config_size_in_words> const& data);
template void CADCChannelConfig::decode(
    std::array<fisch::vx::word_access_type::Omnibus, CADCChannelConfig::config_size_in_words> const&
        data);
#endif

namespace {

struct CADCChannelConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t offset       :  8;
			uint32_t /* unused */ : 24;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CADCChannelConfigBitfield()
	{
		u.raw = 0u;
	}

	CADCChannelConfigBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

template <typename AddressT>
std::array<AddressT, CADCChannelConfig::config_size_in_words> CADCChannelConfig::addresses(
    coordinate_type const& coord)
{
	bool const is_east = coord.toCADCChannelColumnOnSynram() >=
	                     (halco::hicann_dls::vx::CADCChannelColumnOnSynram::size / 2);
	uint32_t const column_offset =
	    coord.toCADCChannelColumnOnSynram() -
	    (is_east ? (halco::hicann_dls::vx::CADCChannelColumnOnSynram::size / 2) : 0);
	uint32_t const base = cadc_sram_base_addresses.at(coord.toSynramOnDLS().toEnum() * 2 + is_east);
	return {AddressT(
	    base + (2 * column_offset) + coord.toCADCChannelConfigOnSynram().toCADCChannelType())};
}

template <typename WordT>
std::array<WordT, CADCChannelConfig::config_size_in_words> CADCChannelConfig::encode() const
{
	CADCChannelConfigBitfield bitfield;
	bitfield.u.m.offset = static_cast<int32_t>(m_offset) + 128;

	return {WordT(bitfield.u.raw)};
}

template <typename WordT>
void CADCChannelConfig::decode(
    std::array<WordT, CADCChannelConfig::config_size_in_words> const& data)
{
	CADCChannelConfigBitfield bitfield(data[0]);
	m_offset = Offset(static_cast<int32_t>(bitfield.u.m.offset) - 128);
}

std::ostream& operator<<(std::ostream& os, CADCChannelConfig const& config)
{
	os << "CADCChannelConfig(" << config.m_offset << ")";
	return os;
}

CADCConfig::CADCConfig() : m_enable(false), m_reset_wait(), m_dead_time() {}

bool CADCConfig::get_enable() const
{
	return m_enable;
}

void CADCConfig::set_enable(bool const value)
{
	m_enable = value;
}

typename CADCConfig::ResetWait CADCConfig::get_reset_wait() const
{
	return m_reset_wait;
}

void CADCConfig::set_reset_wait(ResetWait const value)
{
	m_reset_wait = value;
}

typename CADCConfig::DeadTime CADCConfig::get_dead_time() const
{
	return m_dead_time;
}

void CADCConfig::set_dead_time(DeadTime const value)
{
	m_dead_time = value;
}

bool CADCConfig::operator==(CADCConfig const& other) const
{
	return m_enable == other.m_enable && m_reset_wait == other.m_reset_wait &&
	       m_dead_time == other.m_dead_time;
}

bool CADCConfig::operator!=(CADCConfig const& other) const
{
	return !(*this == other);
}

namespace {

struct CADCConfigBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t enable       :  1;) \
			(uint32_t /* unused */ :  3;) \
			(uint32_t reset_wait   :  8;) \
			(uint32_t dead_time    :  8;) \
			(uint32_t /* unused */ : 12;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CADCConfigBitfield() { u.raw = 0u; }

	CADCConfigBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::ostream& operator<<(std::ostream& os, CADCConfig const& config)
{
	std::stringstream ss;
	ss << "CADCConfig(enable: " << std::boolalpha << config.m_enable << ", " << config.m_reset_wait
	   << ", " << config.m_dead_time << ")";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, CADCConfig::config_size_in_words> CADCConfig::addresses(
    coordinate_type const& coord)
{
	return {AddressT(cadc_busreg_addresses.at(coord.toEnum()))};
}

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, CADCConfig::config_size_in_words>
    CADCConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, CADCConfig::config_size_in_words>
    CADCConfig::addresses(coordinate_type const& coord);

template <typename WordT>
std::array<WordT, CADCConfig::config_size_in_words> CADCConfig::encode() const
{
	CADCConfigBitfield bitfield;
	bitfield.u.m.enable = m_enable;
	bitfield.u.m.reset_wait = m_reset_wait;
	bitfield.u.m.dead_time = m_dead_time;

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::OmnibusChipOverJTAG, CADCConfig::config_size_in_words>
    CADCConfig::encode() const;
template SYMBOL_VISIBLE
    std::array<fisch::vx::word_access_type::Omnibus, CADCConfig::config_size_in_words>
    CADCConfig::encode() const;

template <typename WordT>
void CADCConfig::decode(std::array<WordT, CADCConfig::config_size_in_words> const& data)
{
	CADCConfigBitfield bitfield(data[0]);
	m_enable = bitfield.u.m.enable;
	m_reset_wait = ResetWait(bitfield.u.m.reset_wait);
	m_dead_time = DeadTime(bitfield.u.m.dead_time);
}

template SYMBOL_VISIBLE void CADCConfig::decode(std::array<
                                                fisch::vx::word_access_type::OmnibusChipOverJTAG,
                                                CADCConfig::config_size_in_words> const& data);
template SYMBOL_VISIBLE void CADCConfig::decode(
    std::array<fisch::vx::word_access_type::Omnibus, CADCConfig::config_size_in_words> const& data);


bool CADCOffsetSRAMTimingConfig::operator==(CADCOffsetSRAMTimingConfig const& other) const
{
	return static_cast<SRAMTimingConfig>(*this) == static_cast<SRAMTimingConfig>(other);
}

bool CADCOffsetSRAMTimingConfig::operator!=(CADCOffsetSRAMTimingConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CADCOffsetSRAMTimingConfig const& config)
{
	os << "CADCOffsetSRAMTimingConfig(" << config.get_read_delay() << ", "
	   << config.get_address_setup_time() << ", " << config.get_write_width() << ")";
	return os;
}

template <typename AddressT>
std::array<AddressT, CADCOffsetSRAMTimingConfig::config_size_in_words>
CADCOffsetSRAMTimingConfig::addresses(coordinate_type const& coord)
{
	return {AddressT(cadc_offset_sram_timing_config_base_addresses.at(coord.toEnum())),
	        AddressT(cadc_offset_sram_timing_config_base_addresses.at(coord.toEnum()) + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CADCOffsetSRAMTimingConfig::config_size_in_words>
CADCOffsetSRAMTimingConfig::addresses(coordinate_type const& coord);
template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusAddress, CADCOffsetSRAMTimingConfig::config_size_in_words>
    CADCOffsetSRAMTimingConfig::addresses(coordinate_type const& coord);


CADCSampleQuad::CADCSampleQuad() : m_samples() {}

typename CADCSampleQuad::Value CADCSampleQuad::get_sample(
    halco::hicann_dls::vx::EntryOnQuad const& coord) const
{
	return m_samples.at(coord);
}

void CADCSampleQuad::set_sample(halco::hicann_dls::vx::EntryOnQuad const& coord, Value const sample)
{
	m_samples.at(coord) = sample;
}

bool CADCSampleQuad::operator==(CADCSampleQuad const& other) const
{
	return m_samples == other.m_samples;
}

bool CADCSampleQuad::operator!=(CADCSampleQuad const& other) const
{
	return !(*this == other);
}

namespace {

struct CADCSampleQuadBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define WORD \
			(uint32_t sample_3 : 8;) \
			(uint32_t sample_2 : 8;) \
			(uint32_t sample_1 : 8;) \
			(uint32_t sample_0 : 8;)
			EXPAND_WORD(WORD)
#undef WORD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CADCSampleQuadBitfield()
	{
		u.raw = 0u;
	}

	CADCSampleQuadBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

std::ostream& operator<<(std::ostream& os, CADCSampleQuad const& config)
{
	os << "CADCSampleQuad(" << hate::join_string(config.m_samples, ", ") << ")";
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusAddress, CADCSampleQuad::read_config_size_in_words>
CADCSampleQuad::read_addresses(coordinate_type const& coord)
{
	uint32_t const base = synram_cadc_base_addresses.at(coord.toSynramOnDLS().toEnum())
	                          .at(coord.toCADCChannelType().toEnum()) |
	                      (coord.toCADCReadoutType() ? cadc_buffer_enable_mask : 0ul);

	auto const quad = coord.toSynapseQuadOnSynram();
	uint32_t const address_offset =
	    quad.y() * halco::hicann_dls::vx::SynapseQuadColumnOnDLS::size + quad.x();
	return {halco::hicann_dls::vx::OmnibusAddress(base + address_offset)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, CADCSampleQuad::write_config_size_in_words>
CADCSampleQuad::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

std::array<fisch::vx::word_access_type::Omnibus, CADCSampleQuad::write_config_size_in_words>
CADCSampleQuad::encode() const
{
	return {};
}

void CADCSampleQuad::decode(std::array<
                            fisch::vx::word_access_type::Omnibus,
                            CADCSampleQuad::read_config_size_in_words> const& data)
{
	CADCSampleQuadBitfield bitfield(data[0]);
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(0)) = Value(bitfield.u.m.sample_0);
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(1)) = Value(bitfield.u.m.sample_1);
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(2)) = Value(bitfield.u.m.sample_2);
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(3)) = Value(bitfield.u.m.sample_3);
}

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CADCConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CADCOffsetSRAMTimingConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CADCChannelConfig)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::CADCSampleQuad)
