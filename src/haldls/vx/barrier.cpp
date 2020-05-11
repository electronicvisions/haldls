#include "haldls/vx/barrier.h"

#include <string>

#include "fisch/vx/barrier.h"
#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace vx {

Barrier::Barrier() : m_enable_omnibus(true), m_enable_jtag(true), m_enable_systime(true) {}

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

fisch::vx::Barrier Barrier::encode() const
{
	return fisch::vx::Barrier(fisch::vx::Barrier::Value(
	    static_cast<uintmax_t>(m_enable_omnibus) | (static_cast<uintmax_t>(m_enable_jtag) << 1) |
	    (static_cast<uintmax_t>(m_enable_systime) << 2)));
}

std::ostream& operator<<(std::ostream& os, Barrier const& config)
{
	return (
	    os << "Barrier(omnibus: " << std::boolalpha << config.m_enable_omnibus << ", jtag: "
	       << config.m_enable_jtag << ", systime: " << config.m_enable_systime << ")");
}

bool Barrier::operator==(Barrier const& other) const
{
	return (
	    (m_enable_omnibus == other.m_enable_omnibus) && (m_enable_jtag == other.m_enable_jtag) &&
	    (m_enable_systime == other.m_enable_systime));
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
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(Barrier)

Barrier const Barrier::omnibus = []() {
	Barrier s;
	s.set_enable_omnibus(true);
	return s;
}();

Barrier const Barrier::jtag = []() {
	Barrier s;
	s.set_enable_jtag(true);
	return s;
}();

Barrier const Barrier::systime = []() {
	Barrier s;
	s.set_enable_systime(true);
	return s;
}();

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::Barrier, 0)
