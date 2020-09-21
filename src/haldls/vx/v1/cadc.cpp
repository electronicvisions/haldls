#include "haldls/vx/v1/cadc.h"

#include "haldls/vx/cadc.tcc"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/print.tcc"
#include "haldls/vx/v1/address_transformation.h"
#include "haldls/vx/v1/omnibus_constants.h"
#include "hate/join.h"


namespace haldls::vx {
CADC_CHANNEL_CONFIG_UNROLL(halco::hicann_dls::vx::v1::Coordinates)
} // namespace haldls::vx

namespace haldls::vx::v1 {

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

template <typename Archive>
void CADCSampleQuad::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_samples));
}

namespace {

struct CADCSampleQuadBitfield
{
	union
	{
		haldls::vx::detail::raw_omnibus_type raw;
		// clang-format off
		struct __attribute__((packed)) {
			haldls::vx::detail::raw_omnibus_type sample_3 : 8;
			haldls::vx::detail::raw_omnibus_type sample_2 : 8;
			haldls::vx::detail::raw_omnibus_type sample_1 : 8;
			haldls::vx::detail::raw_omnibus_type sample_0 : 8;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	CADCSampleQuadBitfield()
	{
		u.raw = 0u;
	}

	CADCSampleQuadBitfield(haldls::vx::detail::raw_omnibus_type data)
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
	    quad.y() * halco::hicann_dls::vx::v1::SynapseQuadColumnOnDLS::size +
	    halco::hicann_dls::vx::v1::SynapseQuadColumnOnDLS::max -
	    detail::to_synram_quad_address_offset(quad.x());
	return {halco::hicann_dls::vx::OmnibusAddress(base + address_offset)};
}

std::array<halco::hicann_dls::vx::OmnibusAddress, CADCSampleQuad::write_config_size_in_words>
CADCSampleQuad::write_addresses(coordinate_type const& /* coord */)
{
	return {};
}

std::array<fisch::vx::Omnibus, CADCSampleQuad::write_config_size_in_words> CADCSampleQuad::encode()
    const
{
	return {};
}

namespace {

uint8_t reverse_byte(uint8_t b)
{
	b = ((b & 0xF0) >> 4) | ((b & 0x0F) << 4);
	b = ((b & 0xCC) >> 2) | ((b & 0x33) << 2);
	b = ((b & 0xAA) >> 1) | ((b & 0x55) << 1);
	return b;
}

} // namespace

void CADCSampleQuad::decode(
    std::array<fisch::vx::Omnibus, CADCSampleQuad::read_config_size_in_words> const& data)
{
	CADCSampleQuadBitfield bitfield(data[0].get());
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(0)) =
	    Value(reverse_byte(bitfield.u.m.sample_0));
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(1)) =
	    Value(reverse_byte(bitfield.u.m.sample_1));
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(2)) =
	    Value(reverse_byte(bitfield.u.m.sample_2));
	m_samples.at(halco::hicann_dls::vx::EntryOnQuad(3)) =
	    Value(reverse_byte(bitfield.u.m.sample_3));
}

} // namespace haldls::vx::v1

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::CADCSampleQuad)
CEREAL_CLASS_VERSION(haldls::vx::v1::CADCChannelConfig, 0)
CEREAL_CLASS_VERSION(haldls::vx::v1::CADCSampleQuad, 0)
