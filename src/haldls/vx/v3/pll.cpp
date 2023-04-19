#include "haldls/vx/v3/pll.h"

#include "fisch/vx/word_access/type/jtag.h"
#include "halco/hicann-dls/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/bitfield.h"
#include "haldls/vx/container.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/traits.h"
#include "hate/indent.h"
#include "hate/join.h"


namespace haldls {
namespace vx {
namespace v3 {

PLLClockOutputBlock::ClockOutput::ClockOutput() :
    m_enable_output(true), m_enable_bypass(false), m_adpll(), m_adpll_output()
{}

bool PLLClockOutputBlock::ClockOutput::get_enable_output() const
{
	return m_enable_output;
}

void PLLClockOutputBlock::ClockOutput::set_enable_output(bool const value)
{
	m_enable_output = value;
}

bool PLLClockOutputBlock::ClockOutput::get_enable_bypass() const
{
	return m_enable_bypass;
}

void PLLClockOutputBlock::ClockOutput::set_enable_bypass(bool const value)
{
	m_enable_bypass = value;
}

halco::hicann_dls::vx::ADPLLOnDLS PLLClockOutputBlock::ClockOutput::get_select_adpll() const
{
	return m_adpll;
}

void PLLClockOutputBlock::ClockOutput::set_select_adpll(
    halco::hicann_dls::vx::ADPLLOnDLS const& value)
{
	m_adpll = value;
}

ADPLL::Output PLLClockOutputBlock::ClockOutput::get_select_adpll_output() const
{
	return m_adpll_output;
}

void PLLClockOutputBlock::ClockOutput::set_select_adpll_output(ADPLL::Output const value)
{
	m_adpll_output = value;
}

bool PLLClockOutputBlock::ClockOutput::operator==(ClockOutput const& other) const
{
	return (
	    (m_enable_output == other.m_enable_output) && (m_enable_bypass == other.m_enable_bypass) &&
	    (m_adpll == other.m_adpll) && (m_adpll_output == other.m_adpll_output));
}

bool PLLClockOutputBlock::ClockOutput::operator!=(ClockOutput const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PLLClockOutputBlock::ClockOutput const& config)
{
	std::stringstream ss;
	ss << "ClockOutput(enable_output: " << std::boolalpha << config.m_enable_output
	   << ", enable_bypass: " << config.m_enable_bypass << ", " << config.m_adpll << ", "
	   << config.m_adpll_output << ")";
	return (os << ss.str());
}

std::ostream& operator<<(std::ostream& os, PLLClockOutputBlock const& config)
{
	os << "PLLClockOutputBlock(\n"
	   << hate::indent(hate::join_string(config.m_output, "\n"), "\t") << "\n"
	   << "\tSPL1 clock source: " << config.get_spl1_source() << "\n)";
	return os;
}

PLLClockOutputBlock::PLLClockOutputBlock() : m_output(), m_switch_spl1_to_madc(false)
{
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::serdes_ser_send_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(0));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::phy_ref_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(0));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::ppu_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(1));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::madc_clk].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(0));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::serdes_ser_send_clk]
	    .set_select_adpll_output(ADPLL::Output::core_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::phy_ref_clk].set_select_adpll_output(
	    ADPLL::Output::core_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::ppu_clk].set_select_adpll_output(
	    ADPLL::Output::core_1);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS::madc_clk].set_select_adpll_output(
	    ADPLL::Output::dco);
}

PLLClockOutputBlock::ClockOutput const& PLLClockOutputBlock::get_clock_output(
    halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord) const
{
	return m_output[coord];
}

void PLLClockOutputBlock::set_clock_output(
    halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord, ClockOutput const& config)
{
	m_output[coord] = config;
}


halco::hicann_dls::vx::PLLClockOutputOnDLS const& PLLClockOutputBlock::get_spl1_source() const
{
	if (m_switch_spl1_to_madc)
		return halco::hicann_dls::vx::PLLClockOutputOnDLS::madc_clk;
	else
		return halco::hicann_dls::vx::PLLClockOutputOnDLS::phy_ref_clk;
}

void PLLClockOutputBlock::set_spl1_source(halco::hicann_dls::vx::PLLClockOutputOnDLS const& coord)
{
	if (coord == halco::hicann_dls::vx::PLLClockOutputOnDLS::phy_ref_clk)
		m_switch_spl1_to_madc = false;
	else if (coord == halco::hicann_dls::vx::PLLClockOutputOnDLS::madc_clk)
		m_switch_spl1_to_madc = true;
	else {
#ifndef __ppu__
		throw std::invalid_argument("Only phy_ref_clk and madc_clk are possible sources.");
#else
		exit(1);
#endif
	}
}

bool PLLClockOutputBlock::operator==(PLLClockOutputBlock const& other) const
{
	return m_output == other.m_output && m_switch_spl1_to_madc == other.m_switch_spl1_to_madc;
}

bool PLLClockOutputBlock::operator!=(PLLClockOutputBlock const& other) const
{
	return !(*this == other);
}

namespace {

struct PLLClockOutputBlockBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
#define BITFIELD \
			(uint32_t enable_output_0       : 1;) \
			(uint32_t enable_bypass_0       : 1;) \
			(uint32_t select_adpll_0        : 1;) \
			(uint32_t select_output_adpll_0 : 2;) \
			(uint32_t /* unused */          : 3;) \
			(uint32_t enable_output_1       : 1;) \
			(uint32_t enable_bypass_1       : 1;) \
			(uint32_t select_adpll_1        : 1;) \
			(uint32_t select_output_adpll_1 : 2;) \
			(uint32_t /* unused */          : 3;) \
			(uint32_t enable_output_2       : 1;) \
			(uint32_t enable_bypass_2       : 1;) \
			(uint32_t select_adpll_2        : 1;) \
			(uint32_t select_output_adpll_2 : 2;) \
			(uint32_t /* unused */          : 3;) \
			(uint32_t enable_output_3       : 1;) \
			(uint32_t enable_bypass_3       : 1;) \
			(uint32_t select_adpll_3        : 1;) \
			(uint32_t select_output_adpll_3 : 2;) \
			(uint32_t /* unused */          : 2;) \
			(uint32_t switch_spl1_to_madc   : 1;)
			EXPAND_BITFIELD_ELEMENTS(BITFIELD)
#undef BITFIELD
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PLLClockOutputBlockBitfield()
	{
		u.raw = 0u;
	}

	PLLClockOutputBlockBitfield(uint32_t data)
	{
		u.raw = data;
	}
};

} // namespace

template <>
std::array<halco::hicann_dls::vx::JTAGPLLRegisterOnDLS, PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::JTAGPLLRegisterOnDLS(4)};
}

template <>
std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(pll_base_address + 4)};
}

template SYMBOL_VISIBLE std::
    array<halco::hicann_dls::vx::JTAGPLLRegisterOnDLS, PLLClockOutputBlock::config_size_in_words>
    PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/);

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::addresses(coordinate_type const& /*coord*/);

template <typename WordT>
std::array<WordT, PLLClockOutputBlock::config_size_in_words> PLLClockOutputBlock::encode() const
{
	PLLClockOutputBlockBitfield bitfield;
	bitfield.u.m.enable_output_0 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_enable_output();
	bitfield.u.m.enable_bypass_0 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_enable_bypass();
	bitfield.u.m.select_adpll_0 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_select_adpll();
	bitfield.u.m.select_output_adpll_0 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].get_select_adpll_output());

	bitfield.u.m.enable_output_1 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_enable_output();
	bitfield.u.m.enable_bypass_1 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_enable_bypass();
	bitfield.u.m.select_adpll_1 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_select_adpll();
	bitfield.u.m.select_output_adpll_1 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].get_select_adpll_output());

	bitfield.u.m.enable_output_2 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_enable_output();
	bitfield.u.m.enable_bypass_2 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_enable_bypass();
	bitfield.u.m.select_adpll_2 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_select_adpll();
	bitfield.u.m.select_output_adpll_2 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].get_select_adpll_output());

	bitfield.u.m.enable_output_3 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_enable_output();
	bitfield.u.m.enable_bypass_3 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_enable_bypass();
	bitfield.u.m.select_adpll_3 =
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_select_adpll();
	bitfield.u.m.select_output_adpll_3 = static_cast<uint32_t>(
	    m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].get_select_adpll_output());

	bitfield.u.m.switch_spl1_to_madc = m_switch_spl1_to_madc;

	return {WordT(bitfield.u.raw)};
}

template SYMBOL_VISIBLE std::
    array<fisch::vx::word_access_type::JTAGPLLRegister, PLLClockOutputBlock::config_size_in_words>
    PLLClockOutputBlock::encode() const;

template SYMBOL_VISIBLE std::array<
    fisch::vx::word_access_type::OmnibusChipOverJTAG,
    PLLClockOutputBlock::config_size_in_words>
PLLClockOutputBlock::encode() const;

template <>
void PLLClockOutputBlock::decode(
    std::array<fisch::vx::word_access_type::JTAGPLLRegister, config_size_in_words> const& /*data*/)
{}

template <typename WordT>
void PLLClockOutputBlock::decode(std::array<WordT, config_size_in_words> const& data)
{
	PLLClockOutputBlockBitfield bitfield;

	bitfield.u.raw = data[0];

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_enable_output(
	    bitfield.u.m.enable_output_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_0);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_0));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(0)].set_select_adpll_output(
	    static_cast<ADPLL::Output>(bitfield.u.m.select_output_adpll_0));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_enable_output(
	    bitfield.u.m.enable_output_1);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_1);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_1));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(1)].set_select_adpll_output(
	    static_cast<ADPLL::Output>(bitfield.u.m.select_output_adpll_1));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_enable_output(
	    bitfield.u.m.enable_output_2);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_2);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_2));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(2)].set_select_adpll_output(
	    static_cast<ADPLL::Output>(bitfield.u.m.select_output_adpll_2));

	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_enable_output(
	    bitfield.u.m.enable_output_3);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_enable_bypass(
	    bitfield.u.m.enable_bypass_3);
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_select_adpll(
	    halco::hicann_dls::vx::ADPLLOnDLS(bitfield.u.m.select_adpll_3));
	m_output[halco::hicann_dls::vx::PLLClockOutputOnDLS(3)].set_select_adpll_output(
	    static_cast<ADPLL::Output>(bitfield.u.m.select_output_adpll_3));

	m_switch_spl1_to_madc = bitfield.u.m.switch_spl1_to_madc;
}

template SYMBOL_VISIBLE void PLLClockOutputBlock::decode(
    std::array<fisch::vx::word_access_type::JTAGPLLRegister, config_size_in_words> const& /*data*/);

template SYMBOL_VISIBLE void PLLClockOutputBlock::decode(
    std::array<
        fisch::vx::word_access_type::OmnibusChipOverJTAG,
        config_size_in_words> const& /*data*/);

} // namespace v3
} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::v3::PLLClockOutputBlock)
