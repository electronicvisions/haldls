#include "haldls/vx/cadc.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/omnibus_constants.h"


namespace haldls::vx {

template <typename Coordinates>
CADCChannelConfig<Coordinates>::CADCChannelConfig() : m_offset()
{}

template <typename Coordinates>
typename CADCChannelConfig<Coordinates>::Offset CADCChannelConfig<Coordinates>::get_offset() const
{
	return m_offset;
}

template <typename Coordinates>
void CADCChannelConfig<Coordinates>::set_offset(Offset const value)
{
	m_offset = value;
}

template <typename Coordinates>
bool CADCChannelConfig<Coordinates>::operator==(CADCChannelConfig const& other) const
{
	return m_offset == other.m_offset;
}

template <typename Coordinates>
bool CADCChannelConfig<Coordinates>::operator!=(CADCChannelConfig const& other) const
{
	return !(*this == other);
}

template <typename Coordinates>
template <typename Archive>
void CADCChannelConfig<Coordinates>::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_offset));
}

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

template <typename Coordinates>
template <typename AddressT>
std::array<AddressT, CADCChannelConfig<Coordinates>::config_size_in_words>
CADCChannelConfig<Coordinates>::addresses(coordinate_type const& coord)
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

template <typename Coordinates>
template <typename WordT>
std::array<WordT, CADCChannelConfig<Coordinates>::config_size_in_words>
CADCChannelConfig<Coordinates>::encode() const
{
	CADCChannelConfigBitfield bitfield;
	bitfield.u.m.offset = static_cast<int32_t>(m_offset) + 128;

	return {WordT(typename WordT::Value(bitfield.u.raw))};
}

template <typename Coordinates>
template <typename WordT>
void CADCChannelConfig<Coordinates>::decode(
    std::array<WordT, CADCChannelConfig<Coordinates>::config_size_in_words> const& data)
{
	CADCChannelConfigBitfield bitfield(data[0].get());
	m_offset = Offset(static_cast<int32_t>(bitfield.u.m.offset) - 128);
}

#define CADC_CHANNEL_CONFIG_UNROLL(Coordinates)                                                    \
	template class CADCChannelConfig<Coordinates>;                                                 \
	EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CADCChannelConfig<Coordinates>)                          \
                                                                                                   \
	template std::array<                                                                           \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    CADCChannelConfig<Coordinates>::config_size_in_words>                                      \
	CADCChannelConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
	template SYMBOL_VISIBLE std::array<                                                            \
	    halco::hicann_dls::vx::OmnibusAddress,                                                     \
	    CADCChannelConfig<Coordinates>::config_size_in_words>                                      \
	CADCChannelConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
                                                                                                   \
	template SYMBOL_VISIBLE std::array<                                                            \
	    fisch::vx::OmnibusChipOverJTAG, CADCChannelConfig<Coordinates>::config_size_in_words>      \
	CADCChannelConfig<Coordinates>::encode() const;                                                \
	template std::array<fisch::vx::Omnibus, CADCChannelConfig<Coordinates>::config_size_in_words>  \
	CADCChannelConfig<Coordinates>::encode() const;                                                \
                                                                                                   \
	template void CADCChannelConfig<Coordinates>::decode(                                          \
	    std::array<                                                                                \
	        fisch::vx::OmnibusChipOverJTAG,                                                        \
	        CADCChannelConfig<Coordinates>::config_size_in_words> const& data);                    \
	template void CADCChannelConfig<Coordinates>::decode(                                          \
	    std::array<                                                                                \
	        fisch::vx::Omnibus, CADCChannelConfig<Coordinates>::config_size_in_words> const&       \
	        data);

} // namespace haldls::vx
