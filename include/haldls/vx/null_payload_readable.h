#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/fpga.h"

#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace halco::hicann_dls::vx {
class NullPayloadReadableOnFPGA;
} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) NullPayloadReadable
{
public:
	typedef halco::hicann_dls::vx::NullPayloadReadableOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	NullPayloadReadable() = default;

	bool operator==(NullPayloadReadable const& other) const SYMBOL_VISIBLE;
	bool operator!=(NullPayloadReadable const& other) const SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::NullPayloadReadableOnFPGA, read_config_size_in_words>
	read_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::NullPayloadReadableOnFPGA, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::NullPayloadReadable, write_config_size_in_words>
	encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<
	            fisch::vx::word_access_type::NullPayloadReadable,
	            read_config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, NullPayloadReadable const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(NullPayloadReadable)

namespace detail {

template <>
struct BackendContainerTrait<NullPayloadReadable>
    : public BackendContainerBase<
          NullPayloadReadable,
          fisch::vx::word_access_type::NullPayloadReadable>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
