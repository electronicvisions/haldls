#pragma once
#include "halco/hicann-dls/vx/v3/capmem.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/cerealization.h"
#include "haldls/vx/capmem.h"
#include "haldls/vx/genpybind.h"

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls::vx {
CAPMEM_EXTERN_TEMPLATE(halco::hicann_dls::vx::v3::Coordinates)
} // namespace haldls::vx

namespace std {
CAPMEM_HALCO_GEOMETRY_HASH(halco::hicann_dls::vx::v3::Coordinates)
} // namespace std

namespace haldls::vx::v3 GENPYBIND_TAG_HALDLS_VX_V3 {

using CapMemCell GENPYBIND(opaque) = haldls::vx::CapMemCell<halco::hicann_dls::vx::v3::Coordinates>;

GENPYBIND_MANUAL({
	auto cls = pybind11::class_<::haldls::vx::v3::CapMemCell::value_type>(
	    parent, "_CapMemCell_value_type");
	cls.def(
	       pybind11::init<::haldls::vx::v3::CapMemCell::Value>(),
	       pybind11::arg("value") = ::haldls::vx::v3::CapMemCell::Value(0))
	    .def(
	        pybind11::init<::haldls::vx::v3::CapMemCell::DisableRefresh>(), pybind11::arg("value"));
	parent.attr("CapMemCell").attr("value_type") = parent.attr("_CapMemCell_value_type");
})

using CapMemBlock GENPYBIND(opaque) =
    haldls::vx::CapMemBlock<halco::hicann_dls::vx::v3::Coordinates>;
using CapMemBlockConfig GENPYBIND(opaque) =
    haldls::vx::CapMemBlockConfig<halco::hicann_dls::vx::v3::Coordinates>;

class GENPYBIND(visible) ReferenceGeneratorConfig
{
public:
	typedef halco::hicann_dls::vx::ReferenceGeneratorConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default config of reference generator. */
	explicit ReferenceGeneratorConfig() SYMBOL_VISIBLE;

	struct GENPYBIND(inline_base("*")) CapMemAmplifier
	    : public halco::common::detail::RantWrapper<CapMemAmplifier, uint_fast8_t, 63, 0>
	{
		constexpr explicit CapMemAmplifier(uintmax_t const val = 40)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) CapMemOffset
	    : public halco::common::detail::RantWrapper<CapMemOffset, uint_fast8_t, 63, 0>
	{
		constexpr explicit CapMemOffset(uintmax_t const val = 1) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) CapMemSlope
	    : public halco::common::detail::RantWrapper<CapMemSlope, uint_fast8_t, 63, 0>
	{
		constexpr explicit CapMemSlope(uintmax_t const val = 11) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) ReferenceControl
	    : public halco::common::detail::RantWrapper<ReferenceControl, uint_fast8_t, 63, 0>
	{
		constexpr explicit ReferenceControl(uintmax_t const val = 10)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) ResistorControl
	    : public halco::common::detail::RantWrapper<ResistorControl, uint_fast8_t, 63, 0>
	{
		constexpr explicit ResistorControl(uintmax_t const val = 40)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	typedef halco::common::typed_array<CapMemAmplifier, halco::hicann_dls::vx::CapMemBlockOnDLS>
	    capmem_amplifier_type GENPYBIND(opaque, expose_as(_capmem_amplifier_type));
	typedef halco::common::typed_array<CapMemOffset, halco::hicann_dls::vx::CapMemBlockOnDLS>
	    capmem_offset_type GENPYBIND(opaque, expose_as(_capmem_offset_type));
	typedef halco::common::typed_array<CapMemSlope, halco::hicann_dls::vx::CapMemBlockOnDLS>
	    capmem_slope_type GENPYBIND(opaque, expose_as(_capmem_slope_type));

	/** Set enable for internal reference current generation. */
	GENPYBIND(getter_for(enable_internal_reference))
	bool const& get_enable_internal_reference() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_internal_reference))
	void set_enable_internal_reference(bool value) SYMBOL_VISIBLE;

	/** Set enable for output of reference current to pad. */
	GENPYBIND(getter_for(enable_reference_output))
	bool const& get_enable_reference_output() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_reference_output))
	void set_enable_reference_output(bool value) SYMBOL_VISIBLE;

	/** Set enable for input of reference current from pad. */
	GENPYBIND(getter_for(enable_reference_input))
	bool const& get_enable_reference_input() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_reference_input))
	void set_enable_reference_input(bool value) SYMBOL_VISIBLE;

	/** Set CapMem amplifier bias current (settings for each quadrant). */
	GENPYBIND(getter_for(capmem_amplifier), return_value_policy(reference_internal))
	capmem_amplifier_type const& get_capmem_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_amplifier))
	void set_capmem_amplifier(capmem_amplifier_type const& value) SYMBOL_VISIBLE;

	/** Set CapMem offset current (settings for each quadrant). */
	GENPYBIND(getter_for(capmem_offset), return_value_policy(reference_internal))
	capmem_offset_type const& get_capmem_offset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_offset))
	void set_capmem_offset(capmem_offset_type const& value) SYMBOL_VISIBLE;

	/** Set CapMem slope current (settings for each quadrant). */
	GENPYBIND(getter_for(capmem_slope), return_value_policy(reference_internal))
	capmem_slope_type const& get_capmem_slope() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_slope))
	void set_capmem_slope(capmem_slope_type const& value) SYMBOL_VISIBLE;

	/** Set reference current generation DAC value. */
	GENPYBIND(getter_for(reference_control))
	ReferenceControl get_reference_control() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(reference_control))
	void set_reference_control(ReferenceControl value) SYMBOL_VISIBLE;

	/** Set reference current generation resistor value. */
	GENPYBIND(getter_for(resistor_control))
	ResistorControl get_resistor_control() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(resistor_control))
	void set_resistor_control(ResistorControl value) SYMBOL_VISIBLE;

	/** Set reset signal for resistor control of reference current generation.
	 *  The reset can be used after powerup to reach a working state. */
	GENPYBIND(getter_for(enable_reset))
	bool const& get_enable_reset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_reset))
	void set_enable_reset(bool value) SYMBOL_VISIBLE;

	bool operator==(ReferenceGeneratorConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(ReferenceGeneratorConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 9;

	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ReferenceGeneratorConfig const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable_internal_reference;
	bool m_enable_reference_output;
	bool m_enable_reference_input;
	capmem_amplifier_type m_capmem_amplifier;
	capmem_offset_type m_capmem_offset;
	capmem_slope_type m_capmem_slope;
	ReferenceControl m_reference_control;
	ResistorControl m_resistor_control;
	bool m_enable_reset;
};

} // namespace haldls::vx::v3

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<v3::ReferenceGeneratorConfig>
    : public BackendContainerBase<
          v3::ReferenceGeneratorConfig,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace haldls::vx::detail

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v3::ReferenceGeneratorConfig::CapMemAmplifier)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v3::ReferenceGeneratorConfig::CapMemOffset)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v3::ReferenceGeneratorConfig::CapMemSlope)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v3::ReferenceGeneratorConfig::ReferenceControl)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v3::ReferenceGeneratorConfig::ResistorControl)

} // namespace std

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v3::ReferenceGeneratorConfig)
