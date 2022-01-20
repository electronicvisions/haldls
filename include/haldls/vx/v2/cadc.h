#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/v2/cadc.h"
#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "haldls/cerealization.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/genpybind.h"

namespace fisch::vx {
class Omnibus;
} // namespace fisch::vx

namespace halco::hicann_dls::vx {
class OmnibusAddress;
} // namespace halco::hicann_dls::vx

namespace haldls::vx {
CADC_EXTERN_TEMPLATE(halco::hicann_dls::vx::v2::Coordinates)
} // namespace haldls::vx

namespace haldls::vx::v2 GENPYBIND_TAG_HALDLS_VX_V2 {

using CADCConfig GENPYBIND(visible) = haldls::vx::CADCConfig;
using CADCChannelConfig GENPYBIND(opaque) =
    haldls::vx::CADCChannelConfig<halco::hicann_dls::vx::v2::Coordinates>;
using CADCOffsetSRAMTimingConfig GENPYBIND(visible) = haldls::vx::CADCOffsetSRAMTimingConfig;

/**
 * CADC container of four CADC samples.
 */
class GENPYBIND(visible) CADCSampleQuad
{
public:
	typedef std::true_type is_leaf_node;
	typedef halco::hicann_dls::vx::v2::CADCSampleQuadOnDLS coordinate_type;

	/**
	 * CADC measurement value (offset-corrected, see CADCChannelConfig).
	 */
	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast16_t, 255, 0>
	{
		constexpr explicit Value(intmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** Default constructor. */
	CADCSampleQuad() SYMBOL_VISIBLE;

	/**
	 * Get sample value.
	 * @param coord Sample on quad coordinate
	 * @return Value value
	 */
	Value get_sample(halco::hicann_dls::vx::EntryOnQuad const& coord) const SYMBOL_VISIBLE;

	/**
	 * Set sample value.
	 * @param coord Sample on quad coordinate
	 * @param value Value value
	 */
	void set_sample(halco::hicann_dls::vx::EntryOnQuad const& coord, Value value) SYMBOL_VISIBLE;

	bool operator==(CADCSampleQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCSampleQuad const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCSampleQuad const& config) SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::word_access_type::Omnibus, write_config_size_in_words> encode() const
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<fisch::vx::word_access_type::Omnibus, read_config_size_in_words> const&
	                data) SYMBOL_VISIBLE GENPYBIND(hidden);

protected:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	halco::common::typed_array<Value, halco::hicann_dls::vx::EntryOnQuad> m_samples;
};

} // namespace haldls::vx::v2

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<v2::CADCSampleQuad>
    : public BackendContainerBase<v2::CADCSampleQuad, fisch::vx::word_access_type::Omnibus>
{};

} // namespace haldls::vx::detail

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v2::CADCSampleQuad)
