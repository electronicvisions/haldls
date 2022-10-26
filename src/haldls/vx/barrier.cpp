#include "haldls/vx/barrier.h"

#include <string>

#include "fisch/vx/word_access/type/barrier.h"
#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/cerealization.tcc"

namespace haldls {
namespace vx {

Barrier::Barrier() :
    m_enable_omnibus(true), m_enable_jtag(true), m_enable_systime(true), m_enable_multi_fpga(true)
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

fisch::vx::word_access_type::Barrier Barrier::encode() const
{
	return fisch::vx::word_access_type::Barrier(
	    static_cast<uintmax_t>(m_enable_omnibus) | (static_cast<uintmax_t>(m_enable_jtag) << 1) |
	    (static_cast<uintmax_t>(m_enable_systime) << 2) |
	    (static_cast<uintmax_t>(m_enable_multi_fpga) << 3));
}

std::ostream& operator<<(std::ostream& os, Barrier const& config)
{
	return (
	    os << "Barrier(omnibus: " << std::boolalpha << config.m_enable_omnibus
	       << ", jtag: " << config.m_enable_jtag << ", systime: " << config.m_enable_systime
	       << ", multi_fpga: " << config.m_enable_multi_fpga << ")");
}

bool Barrier::operator==(Barrier const& other) const
{
	return (
	    (m_enable_omnibus == other.m_enable_omnibus) && (m_enable_jtag == other.m_enable_jtag) &&
	    (m_enable_systime == other.m_enable_systime) &&
	    (m_enable_multi_fpga == other.m_enable_multi_fpga));
}

bool Barrier::operator!=(Barrier const& other) const
{
	return !(*this == other);
}

template <class Archive>
void Barrier::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_enable_omnibus));
	ar(CEREAL_NVP(m_enable_jtag));
	ar(CEREAL_NVP(m_enable_systime));
	ar(CEREAL_NVP(m_enable_multi_fpga));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(Barrier)

Barrier const Barrier::omnibus = []() {
	Barrier s;
	s.set_enable_jtag(false);
	s.set_enable_omnibus(true);
	s.set_enable_systime(false);
	s.set_enable_multi_fpga(false);
	return s;
}();

Barrier const Barrier::jtag = []() {
	Barrier s;
	s.set_enable_jtag(true);
	s.set_enable_omnibus(false);
	s.set_enable_systime(false);
	s.set_enable_multi_fpga(false);
	return s;
}();

Barrier const Barrier::systime = []() {
	Barrier s;
	s.set_enable_jtag(false);
	s.set_enable_omnibus(false);
	s.set_enable_systime(true);
	s.set_enable_multi_fpga(false);
	return s;
}();

Barrier const Barrier::multi_fpga = []() {
	Barrier s;
	s.set_enable_jtag(false);
	s.set_enable_omnibus(false);
	s.set_enable_systime(false);
	s.set_enable_multi_fpga(true);
	return s;
}();

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::Barrier, 1)
