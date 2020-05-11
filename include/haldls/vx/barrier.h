#pragma once
#include <ostream>

#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) Barrier
{
public:
	Barrier() SYMBOL_VISIBLE;

	static SYMBOL_VISIBLE const Barrier omnibus GENPYBIND(visible);
	static SYMBOL_VISIBLE const Barrier jtag GENPYBIND(visible);
	static SYMBOL_VISIBLE const Barrier systime GENPYBIND(visible);

	GENPYBIND(getter_for(enable_omnibus))
	bool get_enable_omnibus() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_omnibus))
	void set_enable_omnibus(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_jtag))
	bool get_enable_jtag() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_jtag))
	void set_enable_jtag(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_systime))
	bool get_enable_systime() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_systime))
	void set_enable_systime(bool value) SYMBOL_VISIBLE;

	bool operator==(Barrier const& other) const SYMBOL_VISIBLE;
	bool operator!=(Barrier const& other) const SYMBOL_VISIBLE;

	fisch::vx::Barrier encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Barrier const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable_omnibus;
	bool m_enable_jtag;
	bool m_enable_systime;
};

} // namespace vx
} // namespace haldls
