#include "haldls/vx/v3/capmem.h"

#include "haldls/vx/capmem.tcc"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/cerealization_typed_heap_array.h"
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/capmem.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "hate/join.h"
#include "hate/math.h"

namespace haldls::vx {

CAPMEM_CELL_UNROLL(halco::hicann_dls::vx::v3::Coordinates)
CAPMEM_BLOCK_UNROLL(halco::hicann_dls::vx::v3::Coordinates)
CAPMEM_BLOCK_CONFIG_UNROLL(halco::hicann_dls::vx::v3::Coordinates)

} // namespace haldls::vx

CAPMEM_CEREAL_VERSION(halco::hicann_dls::vx::v3::Coordinates)

namespace haldls::vx::v3 {

using namespace halco::hicann_dls::vx;

ReferenceGeneratorConfig::ReferenceGeneratorConfig() :
    m_enable_internal_reference(),
    m_enable_reference_output(),
    m_enable_reference_input(),
    m_capmem_amplifier(),
    m_capmem_offset(),
    m_capmem_slope(),
    m_reference_control(),
    m_resistor_control(),
    m_enable_reset()
{
	m_enable_internal_reference = true;
}

bool const& ReferenceGeneratorConfig::get_enable_internal_reference() const
{
	return m_enable_internal_reference;
}

void ReferenceGeneratorConfig::set_enable_internal_reference(bool const value)
{
	m_enable_internal_reference = value;
}

bool const& ReferenceGeneratorConfig::get_enable_reference_output() const
{
	return m_enable_reference_output;
}

void ReferenceGeneratorConfig::set_enable_reference_output(bool const value)
{
	m_enable_reference_output = value;
}

bool const& ReferenceGeneratorConfig::get_enable_reference_input() const
{
	return m_enable_reference_input;
}

void ReferenceGeneratorConfig::set_enable_reference_input(bool const value)
{
	m_enable_reference_input = value;
}

ReferenceGeneratorConfig::capmem_amplifier_type const&
ReferenceGeneratorConfig::get_capmem_amplifier() const
{
	return m_capmem_amplifier;
}

void ReferenceGeneratorConfig::set_capmem_amplifier(
    ReferenceGeneratorConfig::capmem_amplifier_type const& value)
{
	m_capmem_amplifier = value;
}

ReferenceGeneratorConfig::capmem_slope_type const& ReferenceGeneratorConfig::get_capmem_slope()
    const
{
	return m_capmem_slope;
}

void ReferenceGeneratorConfig::set_capmem_slope(
    ReferenceGeneratorConfig::capmem_slope_type const& value)
{
	m_capmem_slope = value;
}

ReferenceGeneratorConfig::capmem_offset_type const& ReferenceGeneratorConfig::get_capmem_offset()
    const
{
	return m_capmem_offset;
}

void ReferenceGeneratorConfig::set_capmem_offset(
    ReferenceGeneratorConfig::capmem_offset_type const& value)
{
	m_capmem_offset = value;
}

ReferenceGeneratorConfig::ReferenceControl ReferenceGeneratorConfig::get_reference_control() const
{
	return m_reference_control;
}

void ReferenceGeneratorConfig::set_reference_control(
    ReferenceGeneratorConfig::ReferenceControl const value)
{
	m_reference_control = value;
}

ReferenceGeneratorConfig::ResistorControl ReferenceGeneratorConfig::get_resistor_control() const
{
	return m_resistor_control;
}

void ReferenceGeneratorConfig::set_resistor_control(
    ReferenceGeneratorConfig::ResistorControl const value)
{
	m_resistor_control = value;
}

bool const& ReferenceGeneratorConfig::get_enable_reset() const
{
	return m_enable_reset;
}

void ReferenceGeneratorConfig::set_enable_reset(bool const value)
{
	m_enable_reset = value;
}

bool ReferenceGeneratorConfig::operator==(ReferenceGeneratorConfig const& other) const
{
	return (
	    m_enable_internal_reference == other.m_enable_internal_reference &&
	    m_enable_reference_output == other.m_enable_reference_output &&
	    m_enable_reference_input == other.m_enable_reference_input &&
	    m_capmem_amplifier == other.m_capmem_amplifier &&
	    m_capmem_offset == other.m_capmem_offset && m_capmem_slope == other.m_capmem_slope &&
	    m_reference_control == other.m_reference_control &&
	    m_resistor_control == other.m_resistor_control && m_enable_reset == other.m_enable_reset);
}

bool ReferenceGeneratorConfig::operator!=(ReferenceGeneratorConfig const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, ReferenceGeneratorConfig const& config)
{
	std::stringstream ss;
	ss << "ReferenceGeneratorConfig(\n" << std::boolalpha;
	ss << "\tenable_internal_reference: \t" << config.m_enable_internal_reference << "\n";
	ss << "\tenable_reference_output:   \t" << config.m_enable_reference_output << "\n";
	ss << "\tenable_reference_input:    \t" << config.m_enable_reference_input << "\n";
	ss << "\treference_control:         \t" << config.m_reference_control << "\n";
	ss << "\tresistor_control:          \t" << config.m_resistor_control << "\n";
	ss << "\tenable_reset:              \t" << config.m_enable_reset << "\n";
	ss << "\tcapmem_amplifier:          \t[" << hate::join_string(config.m_capmem_amplifier, ", ")
	   << "]\n";
	ss << "\tcapmem_offset:             \t[" << hate::join_string(config.m_capmem_offset, ", ")
	   << "]\n";
	ss << "\tcapmem_slope:              \t[" << hate::join_string(config.m_capmem_slope, ", ")
	   << "]\n)";
	return (os << ss.str());
}

template <typename AddressT>
std::array<AddressT, ReferenceGeneratorConfig::config_size_in_words>
ReferenceGeneratorConfig::addresses(coordinate_type const& /*coord*/)
{
	return {AddressT(madc_base_address + 18), AddressT(madc_base_address + 19),
	        AddressT(madc_base_address + 20), AddressT(madc_base_address + 21),
	        AddressT(madc_base_address + 22), AddressT(madc_base_address + 23),
	        AddressT(madc_base_address + 24), AddressT(madc_base_address + 25),
	        AddressT(madc_base_address + 26)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::OmnibusAddress, ReferenceGeneratorConfig::config_size_in_words>
    ReferenceGeneratorConfig::addresses(coordinate_type const& coord);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    ReferenceGeneratorConfig::config_size_in_words>
ReferenceGeneratorConfig::addresses(coordinate_type const& coord);

namespace {
struct ReferenceGeneratorConfigBitfield
{
	union
	{
		std::array<uint32_t, ReferenceGeneratorConfig::config_size_in_words> words;

		struct __attribute__((packed))
		{
			// clang-format off
			uint32_t                     : 12; // 0...11
			uint32_t en_internal_ref     : 1;  // 12  in madc_base + 18
			uint32_t en_out_internal_ref : 1;  // 13
			uint32_t en_pad_ref          : 1;  // 14
			uint32_t en_reset            : 1;  // 15
			uint32_t                     : 16; // 16...31
			uint32_t                     : 6;  // 0...5  in madc_base + 19
			uint32_t reference_control   : 6;  // 6...11
			uint32_t                     : 20; // 12...31
			uint32_t resistor_control    : 6;  // 0...5  in madc_base + 20
			uint32_t                     : 26; // 6...31
			uint32_t cm_i_amplifier_0    : 6;  // 0...5  in madc_base + 21
			uint32_t cm_i_amplifier_1    : 6;  // 6...11
			uint32_t                     : 20; // 12...31
			uint32_t cm_i_amplifier_2    : 6;  // 0...5  in madc_base + 22
			uint32_t cm_i_amplifier_3    : 6;  // 6...11
			uint32_t                     : 20; // 12...31
			uint32_t cm_i_offset_0       : 6;  // 0...5  in madc_base + 23
			uint32_t cm_i_offset_1       : 6;  // 6...11
			uint32_t                     : 20; // 12...31
			uint32_t cm_i_offset_2       : 6;  // 0...5  in madc_base + 24
			uint32_t cm_i_offset_3       : 6;  // 6...11
			uint32_t                     : 20; // 12...31
			uint32_t cm_i_slope_0        : 6;  // 0...5  in madc_base + 25
			uint32_t cm_i_slope_1        : 6;  // 6...11
			uint32_t                     : 20; // 12...31
			uint32_t cm_i_slope_2        : 6;  // 0...5  in madc_base + 26
			uint32_t cm_i_slope_3        : 6;  // 6...11
			uint32_t                     : 20; // 12...31
			// clang-format on
		} m;

		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	ReferenceGeneratorConfigBitfield()
	{
		u.words = {{0, 0, 0, 0, 0, 0, 0, 0, 0}};
	}

	ReferenceGeneratorConfigBitfield(
	    std::array<uint32_t, ReferenceGeneratorConfig::config_size_in_words> data)
	{
		u.words = data;
	}
};

} // anonymous namespace

template <typename WordT>
std::array<WordT, ReferenceGeneratorConfig::config_size_in_words> ReferenceGeneratorConfig::encode()
    const
{
	ReferenceGeneratorConfigBitfield bitfield;

	bitfield.u.m.cm_i_amplifier_0 = m_capmem_amplifier[CapMemBlockOnDLS(0)];
	bitfield.u.m.cm_i_amplifier_1 = m_capmem_amplifier[CapMemBlockOnDLS(1)];
	bitfield.u.m.cm_i_amplifier_2 = m_capmem_amplifier[CapMemBlockOnDLS(2)];
	bitfield.u.m.cm_i_amplifier_3 = m_capmem_amplifier[CapMemBlockOnDLS(3)];
	bitfield.u.m.cm_i_offset_0 = m_capmem_offset[CapMemBlockOnDLS(0)];
	bitfield.u.m.cm_i_offset_1 = m_capmem_offset[CapMemBlockOnDLS(1)];
	bitfield.u.m.cm_i_offset_2 = m_capmem_offset[CapMemBlockOnDLS(2)];
	bitfield.u.m.cm_i_offset_3 = m_capmem_offset[CapMemBlockOnDLS(3)];
	bitfield.u.m.cm_i_slope_0 = m_capmem_slope[CapMemBlockOnDLS(0)];
	bitfield.u.m.cm_i_slope_1 = m_capmem_slope[CapMemBlockOnDLS(1)];
	bitfield.u.m.cm_i_slope_2 = m_capmem_slope[CapMemBlockOnDLS(2)];
	bitfield.u.m.cm_i_slope_3 = m_capmem_slope[CapMemBlockOnDLS(3)];
	bitfield.u.m.en_internal_ref = m_enable_internal_reference;
	bitfield.u.m.en_out_internal_ref = m_enable_reference_output;
	bitfield.u.m.en_pad_ref = m_enable_reference_input;
	bitfield.u.m.en_reset = m_enable_reset;
	bitfield.u.m.reference_control = m_reference_control;
	bitfield.u.m.resistor_control = m_resistor_control;

	std::array<WordT, ReferenceGeneratorConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(typename WordT::Value(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, ReferenceGeneratorConfig::config_size_in_words>
    ReferenceGeneratorConfig::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE
    std::array<fisch::vx::Omnibus, ReferenceGeneratorConfig::config_size_in_words>
    ReferenceGeneratorConfig::encode<fisch::vx::Omnibus>() const;

template <typename WordT>
void ReferenceGeneratorConfig::decode(
    std::array<WordT, ReferenceGeneratorConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, ReferenceGeneratorConfig::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	ReferenceGeneratorConfigBitfield bitfield(raw_data);

	m_capmem_amplifier[CapMemBlockOnDLS(0)] =
	    ReferenceGeneratorConfig::CapMemAmplifier(bitfield.u.m.cm_i_amplifier_0);
	m_capmem_amplifier[CapMemBlockOnDLS(1)] =
	    ReferenceGeneratorConfig::CapMemAmplifier(bitfield.u.m.cm_i_amplifier_1);
	m_capmem_amplifier[CapMemBlockOnDLS(2)] =
	    ReferenceGeneratorConfig::CapMemAmplifier(bitfield.u.m.cm_i_amplifier_2);
	m_capmem_amplifier[CapMemBlockOnDLS(3)] =
	    ReferenceGeneratorConfig::CapMemAmplifier(bitfield.u.m.cm_i_amplifier_3);
	m_capmem_offset[CapMemBlockOnDLS(0)] =
	    ReferenceGeneratorConfig::CapMemOffset(bitfield.u.m.cm_i_offset_0);
	m_capmem_offset[CapMemBlockOnDLS(1)] =
	    ReferenceGeneratorConfig::CapMemOffset(bitfield.u.m.cm_i_offset_1);
	m_capmem_offset[CapMemBlockOnDLS(2)] =
	    ReferenceGeneratorConfig::CapMemOffset(bitfield.u.m.cm_i_offset_2);
	m_capmem_offset[CapMemBlockOnDLS(3)] =
	    ReferenceGeneratorConfig::CapMemOffset(bitfield.u.m.cm_i_offset_3);
	m_capmem_slope[CapMemBlockOnDLS(0)] =
	    ReferenceGeneratorConfig::CapMemSlope(bitfield.u.m.cm_i_slope_0);
	m_capmem_slope[CapMemBlockOnDLS(1)] =
	    ReferenceGeneratorConfig::CapMemSlope(bitfield.u.m.cm_i_slope_1);
	m_capmem_slope[CapMemBlockOnDLS(2)] =
	    ReferenceGeneratorConfig::CapMemSlope(bitfield.u.m.cm_i_slope_2);
	m_capmem_slope[CapMemBlockOnDLS(3)] =
	    ReferenceGeneratorConfig::CapMemSlope(bitfield.u.m.cm_i_slope_3);
	m_enable_internal_reference = bitfield.u.m.en_internal_ref;
	m_enable_reference_output = bitfield.u.m.en_out_internal_ref;
	m_enable_reference_input = bitfield.u.m.en_pad_ref;
	m_enable_reset = bitfield.u.m.en_reset;
	m_reference_control =
	    ReferenceGeneratorConfig::ReferenceControl(bitfield.u.m.reference_control);
	m_resistor_control = ReferenceGeneratorConfig::ResistorControl(bitfield.u.m.resistor_control);
}

template SYMBOL_VISIBLE void ReferenceGeneratorConfig::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<
        fisch::vx::OmnibusChipOverJTAG,
        ReferenceGeneratorConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void ReferenceGeneratorConfig::decode<fisch::vx::Omnibus>(
    std::array<fisch::vx::Omnibus, ReferenceGeneratorConfig::config_size_in_words> const& data);

template <class Archive>
void ReferenceGeneratorConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_capmem_amplifier));
	ar(CEREAL_NVP(m_enable_internal_reference));
	ar(CEREAL_NVP(m_enable_reference_output));
	ar(CEREAL_NVP(m_enable_reference_input));
	ar(CEREAL_NVP(m_capmem_amplifier));
	ar(CEREAL_NVP(m_capmem_offset));
	ar(CEREAL_NVP(m_capmem_slope));
	ar(CEREAL_NVP(m_reference_control));
	ar(CEREAL_NVP(m_resistor_control));
	ar(CEREAL_NVP(m_enable_reset));
}

} // namespace haldls::vx::v3

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v3::ReferenceGeneratorConfig)
CEREAL_CLASS_VERSION(haldls::vx::v3::ReferenceGeneratorConfig, 0)
