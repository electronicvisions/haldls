#include "haldls/vx/v2/cadc.h"

#include "haldls/vx/cadc.tcc"

#include "fisch/vx/word_access/type/jtag.h"
#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"

#ifndef __ppu__
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.tcc"
#endif

namespace haldls::vx {

#ifndef __ppu__
CADC_CHANNEL_CONFIG_UNROLL(halco::hicann_dls::vx::v2::Coordinates)
#endif

#ifdef __ppu__
CADC_CHANNEL_CONFIG_UNROLL_PPU(halco::hicann_dls::vx::v2::Coordinates)
#endif

} // namespace haldls::vx

namespace haldls::vx::v2 {


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

#ifndef __ppu__
template <typename Archive>
void CADCSampleQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_samples));
}
#endif

namespace {

struct CADCSampleQuadBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t sample_0 : 8;
			uint32_t sample_1 : 8;
			uint32_t sample_2 : 8;
			uint32_t sample_3 : 8;
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
	    quad.y() * halco::hicann_dls::vx::v2::SynapseQuadColumnOnDLS::size + quad.x();
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

} // namespace haldls::vx::v2

#ifndef __ppu__
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v2::CADCSampleQuad)
CEREAL_CLASS_VERSION(haldls::vx::v2::CADCChannelConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::v2::CADCSampleQuad, 0)
#endif
