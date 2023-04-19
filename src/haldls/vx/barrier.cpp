#include "haldls/vx/barrier.h"

#include "fisch/vx/word_access/type/barrier.h"
#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/vx/block_until.tcc"
#include <string>


namespace haldls {
namespace vx {

Barrier::Barrier() :
    m_enable_omnibus(false),
    m_enable_jtag(false),
    m_enable_systime(false),
    m_enable_multi_fpga(false),
    m_enable_systime_correction(false)
{}

bool Barrier::get_enable_omnibus() const
{
	return m_enable_omnibus;
}
void Barrier::set_enable_omnibus(bool const value)
{
	m_enable_omnibus = value;
}

bool Barrier::get_enable_jtag() const
{
	return m_enable_jtag;
}
void Barrier::set_enable_jtag(bool const value)
{
	m_enable_jtag = value;
}

bool Barrier::get_enable_systime() const
{
	return m_enable_systime;
}
void Barrier::set_enable_systime(bool const value)
{
	m_enable_systime = value;
}

bool Barrier::get_enable_multi_fpga() const
{
	return m_enable_multi_fpga;
}
void Barrier::set_enable_multi_fpga(bool const value)
{
	m_enable_multi_fpga = value;
}

bool Barrier::get_enable_systime_correction() const
{
	return m_enable_systime_correction;
}
void Barrier::set_enable_systime_correction(bool const value)
{
	m_enable_systime_correction = value;
}

std::array<halco::hicann_dls::vx::BarrierOnFPGA, Barrier::write_config_size_in_words>
Barrier::write_addresses(coordinate_type const& coord)
{
	return {coord};
}

std::array<fisch::vx::word_access_type::Barrier, Barrier::write_config_size_in_words>
Barrier::encode() const
{
	return {fisch::vx::word_access_type::Barrier(
	    static_cast<uintmax_t>(m_enable_omnibus) | (static_cast<uintmax_t>(m_enable_jtag) << 1) |
	    (static_cast<uintmax_t>(m_enable_systime) << 2) |
	    (static_cast<uintmax_t>(m_enable_multi_fpga) << 3) |
	    (static_cast<uintmax_t>(m_enable_systime_correction) << 4))};
}

std::ostream& operator<<(std::ostream& os, Barrier const& config)
{
	return (
	    os << "Barrier(omnibus: " << std::boolalpha << config.m_enable_omnibus
	       << ", jtag: " << config.m_enable_jtag << ", systime: " << config.m_enable_systime
	       << ", multi_fpga: " << config.m_enable_multi_fpga
	       << ", systime_correction: " << config.m_enable_systime_correction << ")");
}

bool Barrier::operator==(Barrier const& other) const
{
	return (
	    (m_enable_omnibus == other.m_enable_omnibus) && (m_enable_jtag == other.m_enable_jtag) &&
	    (m_enable_systime == other.m_enable_systime) &&
	    (m_enable_multi_fpga == other.m_enable_multi_fpga) &&
	    (m_enable_systime_correction == other.m_enable_systime_correction));
}

bool Barrier::operator!=(Barrier const& other) const
{
	return !(*this == other);
}

Barrier const Barrier::omnibus = []() {
	Barrier s;
	s.set_enable_jtag(false);
	s.set_enable_omnibus(true);
	s.set_enable_systime(false);
	s.set_enable_multi_fpga(false);
	s.set_enable_systime_correction(false);
	return s;
}();

Barrier const Barrier::jtag = []() {
	Barrier s;
	s.set_enable_jtag(true);
	s.set_enable_omnibus(false);
	s.set_enable_systime(false);
	s.set_enable_multi_fpga(false);
	s.set_enable_systime_correction(false);
	return s;
}();

Barrier const Barrier::systime = []() {
	Barrier s;
	s.set_enable_jtag(false);
	s.set_enable_omnibus(false);
	s.set_enable_systime(true);
	s.set_enable_multi_fpga(false);
	s.set_enable_systime_correction(false);
	return s;
}();

Barrier const Barrier::multi_fpga = []() {
	Barrier s;
	s.set_enable_jtag(false);
	s.set_enable_omnibus(false);
	s.set_enable_systime(false);
	s.set_enable_multi_fpga(true);
	s.set_enable_systime_correction(false);
	return s;
}();

Barrier const Barrier::systime_correction = []() {
	Barrier s;
	s.set_enable_jtag(false);
	s.set_enable_omnibus(false);
	s.set_enable_systime(false);
	s.set_enable_multi_fpga(false);
	s.set_enable_systime_correction(true);
	return s;
}();

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_BLOCK_UNTIL_BASE(haldls::vx::Barrier)
