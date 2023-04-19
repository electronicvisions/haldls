#pragma once
#include <iosfwd>

#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/vx/block_until.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

namespace haldls::vx {

struct Barrier;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::Barrier& value, std::uint32_t const version);

} // namespace cereal

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class SYMBOL_VISIBLE GENPYBIND(inline_base("*BlockUntilBase*")) Barrier
    : public BlockUntilBase<Barrier>
{
public:
	typedef halco::hicann_dls::vx::BarrierOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	Barrier() SYMBOL_VISIBLE;

	static SYMBOL_VISIBLE const Barrier omnibus GENPYBIND(visible);
	static SYMBOL_VISIBLE const Barrier jtag GENPYBIND(visible);
	static SYMBOL_VISIBLE const Barrier systime GENPYBIND(visible);
	static SYMBOL_VISIBLE const Barrier multi_fpga GENPYBIND(visible);
	static SYMBOL_VISIBLE const Barrier systime_correction GENPYBIND(visible);

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

	GENPYBIND(getter_for(enable_multi_fpga))
	bool get_enable_multi_fpga() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_multi_fpga))
	void set_enable_multi_fpga(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_systime_correction))
	bool get_enable_systime_correction() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_systime_correction))
	void set_enable_systime_correction(bool value) SYMBOL_VISIBLE;

	bool operator==(Barrier const& other) const SYMBOL_VISIBLE;
	bool operator!=(Barrier const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::BarrierOnFPGA, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Barrier, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Barrier const& config) SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(Archive& ar, Barrier& value, std::uint32_t const version)
	    SYMBOL_VISIBLE;

	bool m_enable_omnibus;
	bool m_enable_jtag;
	bool m_enable_systime;
	bool m_enable_multi_fpga;
	bool m_enable_systime_correction;
};

namespace detail {

template <>
struct BackendContainerTrait<Barrier>
    : public BackendContainerBase<Barrier, fisch::vx::word_access_type::Barrier>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
