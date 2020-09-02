#pragma once
#include <array>
#include <cstdint>
#include <ostream>
#include <variant>

#include "halco/common/geometry.h"
#include "halco/common/iter_all.h"
#include "halco/common/typed_heap_array.h"
#include "halco/hicann-dls/vx/capmem.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

template <typename Coordinates>
class CapMemCell;

template <typename Coordinates>
std::ostream& operator<<(std::ostream& os, CapMemCell<Coordinates> const& cell);

template <typename Coordinates>
class SYMBOL_VISIBLE CapMemCell : public DifferentialWriteTrait
{
public:
	typedef typename Coordinates::CapMemCellOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint_fast16_t, 1022, 0>
	{
		typedef typename halco::common::detail::RantWrapper<Value, uint_fast16_t, 1022, 0>::rant_t
		    rant_t;
		// default constructor needed because std::variant checks std::is_default_constructible
		// which is false for constructors with default arguments
		constexpr explicit Value() : rant_t(0) {}
		constexpr explicit Value(uintmax_t const val) GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) DisableRefresh
	    : public halco::common::detail::RantWrapper<DisableRefresh, uint_fast16_t, 1023, 1023>
	{
		typedef typename halco::common::detail::
		    RantWrapper<DisableRefresh, uint_fast16_t, 1023, 1023>::rant_t rant_t;
		constexpr explicit DisableRefresh() : rant_t(1023) {}
		constexpr explicit DisableRefresh(uintmax_t const val) : rant_t(val) {}
	};

	typedef std::variant<Value, DisableRefresh> value_type;

	explicit CapMemCell(value_type const& value = Value()) : m_value(value) {}

	GENPYBIND(getter_for(value))
	value_type get_value() const;
	GENPYBIND(setter_for(value))
	void set_value(value_type const& value);

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& cell)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<<>(std::ostream& os, CapMemCell const& cell);

	bool operator==(CapMemCell const& other) const;
	bool operator!=(CapMemCell const& other) const;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	value_type m_value;
};

template <typename Coordinates>
class CapMemBlock;

template <typename Coordinates>
std::ostream& operator<<(std::ostream& os, CapMemBlock<Coordinates> const& cell);

template <typename Coordinates>
class SYMBOL_VISIBLE CapMemBlock : public DifferentialWriteTrait
{
public:
	typedef typename Coordinates::CapMemBlockOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	CapMemBlock();

	typename CapMemCell<Coordinates>::value_type get_cell(
	    typename Coordinates::CapMemCellOnCapMemBlock const& cell) const;
	void set_cell(
	    typename Coordinates::CapMemCellOnCapMemBlock const& cell,
	    typename CapMemCell<Coordinates>::value_type const& value);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<<>(std::ostream& os, CapMemBlock<Coordinates> const& block);

	bool operator==(CapMemBlock const& other) const;
	bool operator!=(CapMemBlock const& other) const;

private:
	friend detail::VisitPreorderImpl<CapMemBlock>;
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	halco::common::
	    typed_heap_array<CapMemCell<Coordinates>, typename Coordinates::CapMemCellOnCapMemBlock>
	        m_capmem_cells;
};


template <typename Coordinates>
class CapMemBlockConfig;

template <typename Coordinates>
std::ostream& operator<<(std::ostream& os, CapMemBlockConfig<Coordinates> const& cell);

/** Enum inside templated class not wrapped correctly by genpybind (Issue #3699). */
enum class GENPYBIND(expose_as(_CapMemBlockConfigVRefSelect))
    CapMemBlockConfigVRefSelect : uint_fast8_t
{
	disabled = 0,
	v_ref_v = 1,
	v_ref_i = 2
};

/** Enum inside templated class not wrapped correctly by genpybind (Issue #3699). */
enum class GENPYBIND(expose_as(_CapMemBlockConfigIOutSelect))
    CapMemBlockConfigIOutSelect : uint_fast8_t
{
	disabled = 0,
	i_out_mux = 1,
	i_out_ramp = 2
};

template <typename Coordinates>
class SYMBOL_VISIBLE CapMemBlockConfig : public DifferentialWriteTrait
{
public:
	typedef typename Coordinates::CapMemBlockConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware};

	struct GENPYBIND(inline_base("*")) OutAmpBias
	    : public halco::common::detail::RantWrapper<OutAmpBias, uint_fast16_t, 15, 0>
	{
		typedef
		    typename halco::common::detail::RantWrapper<OutAmpBias, uint_fast16_t, 15, 0>::rant_t
		        rant_t;
		constexpr explicit OutAmpBias() : rant_t(0) {}
		constexpr explicit OutAmpBias(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) SourceFollowerBias
	    : public halco::common::detail::RantWrapper<SourceFollowerBias, uint_fast16_t, 15, 0>
	{
		typedef typename halco::common::detail::
		    RantWrapper<SourceFollowerBias, uint_fast16_t, 15, 0>::rant_t rant_t;
		constexpr explicit SourceFollowerBias() : rant_t(0) {}
		constexpr explicit SourceFollowerBias(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) LevelShifterBias
	    : public halco::common::detail::RantWrapper<LevelShifterBias, uint_fast16_t, 15, 0>
	{
		typedef typename halco::common::detail::
		    RantWrapper<LevelShifterBias, uint_fast16_t, 15, 0>::rant_t rant_t;
		constexpr explicit LevelShifterBias() : rant_t(0) {}
		constexpr explicit LevelShifterBias(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) VGlobalBias
	    : public halco::common::detail::RantWrapper<VGlobalBias, uint_fast16_t, 15, 0>
	{
		typedef
		    typename halco::common::detail::RantWrapper<VGlobalBias, uint_fast16_t, 15, 0>::rant_t
		        rant_t;
		constexpr explicit VGlobalBias() : rant_t(0) {}
		constexpr explicit VGlobalBias(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) CurrentCellRes
	    : public halco::common::detail::RantWrapper<CurrentCellRes, uint_fast16_t, 63, 0>
	{
		typedef typename halco::common::detail::RantWrapper<CurrentCellRes, uint_fast16_t, 63, 0>::
		    rant_t rant_t;
		constexpr explicit CurrentCellRes() : rant_t(0) {}
		constexpr explicit CurrentCellRes(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) BoostFactor
	    : public halco::common::detail::RantWrapper<BoostFactor, uint_fast16_t, 15, 0>
	{
		typedef
		    typename halco::common::detail::RantWrapper<BoostFactor, uint_fast16_t, 15, 0>::rant_t
		        rant_t;
		constexpr explicit BoostFactor() : rant_t(0) {}
		constexpr explicit BoostFactor(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) PrescalePause
	    : public halco::common::detail::RantWrapper<PrescalePause, uint_fast16_t, 6, 0>
	{
		typedef
		    typename halco::common::detail::RantWrapper<PrescalePause, uint_fast16_t, 6, 0>::rant_t
		        rant_t;
		constexpr explicit PrescalePause() : rant_t(0) {}
		constexpr explicit PrescalePause(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) PrescaleRamp
	    : public halco::common::detail::RantWrapper<PrescaleRamp, uint_fast16_t, 6, 0>
	{
		typedef
		    typename halco::common::detail::RantWrapper<PrescaleRamp, uint_fast16_t, 6, 0>::rant_t
		        rant_t;
		constexpr explicit PrescaleRamp() : rant_t(0) {}
		constexpr explicit PrescaleRamp(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) SubCounter
	    : public halco::common::detail::
	          RantWrapper<SubCounter, uint_fast16_t, 65535 /* 2^16*-1 */, 0>
	{
		typedef typename halco::common::detail::
		    RantWrapper<SubCounter, uint_fast16_t, 65535 /* 2^16*-1 */, 0>::rant_t rant_t;
		constexpr explicit SubCounter() : rant_t(0) {}
		constexpr explicit SubCounter(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) PauseCounter
	    : public halco::common::detail::
	          RantWrapper<PauseCounter, uint_fast32_t, 4294967295 /* 2^32-1 */, 0>
	{
		typedef typename halco::common::detail::
		    RantWrapper<PauseCounter, uint_fast32_t, 4294967295 /* 2^32-1 */, 0>::rant_t rant_t;
		constexpr explicit PauseCounter() : rant_t(0) {}
		constexpr explicit PauseCounter(uintmax_t const val) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) PulseA
	    : public halco::common::detail::RantWrapper<PulseA, uint_fast16_t, 65535, 0>
	{
		typedef typename halco::common::detail::RantWrapper<PulseA, uint_fast16_t, 65535, 0>::rant_t
		    rant_t;
		constexpr explicit PulseA() : rant_t(0) {}
		constexpr explicit PulseA(uintmax_t const val) GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) PulseB
	    : public halco::common::detail::RantWrapper<PulseB, uint_fast16_t, 65535, 0>
	{
		typedef typename halco::common::detail::RantWrapper<PulseB, uint_fast16_t, 65535, 0>::rant_t
		    rant_t;
		constexpr explicit PulseB() : rant_t(0) {}
		constexpr explicit PulseB(uintmax_t const val) GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) BoostA
	    : public halco::common::detail::RantWrapper<BoostA, uint_fast16_t, 65535, 0>
	{
		typedef typename halco::common::detail::RantWrapper<BoostA, uint_fast16_t, 65535, 0>::rant_t
		    rant_t;
		constexpr explicit BoostA() : rant_t(0) {}
		constexpr explicit BoostA(uintmax_t const val) GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};
	struct GENPYBIND(inline_base("*")) BoostB
	    : public halco::common::detail::RantWrapper<BoostB, uint_fast16_t, 65535, 0>
	{
		typedef typename halco::common::detail::RantWrapper<BoostB, uint_fast16_t, 65535, 0>::rant_t
		    rant_t;
		constexpr explicit BoostB() : rant_t(0) {}
		constexpr explicit BoostB(uintmax_t const val) GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};

	typedef CapMemBlockConfigVRefSelect VRefSelect GENPYBIND(visible);
	typedef CapMemBlockConfigIOutSelect IOutSelect GENPYBIND(visible);

	CapMemBlockConfig();

	GENPYBIND(getter_for(enable_capmem))
	bool get_enable_capmem() const;
	GENPYBIND(setter_for(enable_capmem))
	void set_enable_capmem(bool const value);

	GENPYBIND(getter_for(debug_readout_enable))
	bool get_debug_readout_enable() const;
	GENPYBIND(setter_for(debug_readout_enable))
	void set_debug_readout_enable(bool const value);

	GENPYBIND(getter_for(debug_capmem_coord))
	typename Coordinates::CapMemCellOnCapMemBlock get_debug_capmem_coord() const;
	GENPYBIND(setter_for(debug_capmem_coord))
	void set_debug_capmem_coord(typename Coordinates::CapMemCellOnCapMemBlock const& value);

	GENPYBIND(getter_for(debug_v_ref_select))
	VRefSelect get_debug_v_ref_select() const;
	GENPYBIND(setter_for(debug_v_ref_select))
	void set_debug_v_ref_select(VRefSelect const& value);

	GENPYBIND(getter_for(debug_i_out_select))
	IOutSelect get_debug_i_out_select() const;
	GENPYBIND(setter_for(debug_i_out_select))
	void set_debug_i_out_select(IOutSelect const& value);

	GENPYBIND(getter_for(debug_out_amp_bias))
	OutAmpBias get_debug_out_amp_bias() const;
	GENPYBIND(setter_for(debug_out_amp_bias))
	void set_debug_out_amp_bias(OutAmpBias const& value);

	GENPYBIND(getter_for(debug_source_follower_bias))
	SourceFollowerBias get_debug_source_follower_bias() const;
	GENPYBIND(setter_for(debug_source_follower_bias))
	void set_debug_source_follower_bias(SourceFollowerBias const& value);

	GENPYBIND(getter_for(debug_level_shifter_bias))
	LevelShifterBias get_debug_level_shifter_bias() const;
	GENPYBIND(setter_for(debug_level_shifter_bias))
	void set_debug_level_shifter_bias(LevelShifterBias const& value);

	GENPYBIND(getter_for(v_global_bias))
	VGlobalBias get_v_global_bias() const;
	GENPYBIND(setter_for(v_global_bias))
	void set_v_global_bias(VGlobalBias const& value);

	GENPYBIND(getter_for(current_cell_res))
	CurrentCellRes get_current_cell_res() const;
	GENPYBIND(setter_for(current_cell_res))
	void set_current_cell_res(CurrentCellRes const& value);

	GENPYBIND(getter_for(boost_factor))
	BoostFactor get_boost_factor() const;
	GENPYBIND(setter_for(boost_factor))
	void set_boost_factor(BoostFactor const& value);

	GENPYBIND(getter_for(enable_boost))
	bool get_enable_boost() const;
	GENPYBIND(setter_for(enable_boost))
	void set_enable_boost(bool const value);

	GENPYBIND(getter_for(enable_autoboost))
	bool get_enable_autoboost() const;
	GENPYBIND(setter_for(enable_autoboost))
	void set_enable_autoboost(bool const value);

	GENPYBIND(getter_for(prescale_pause))
	PrescalePause get_prescale_pause() const;
	GENPYBIND(setter_for(prescale_pause))
	void set_prescale_pause(PrescalePause const& value);

	GENPYBIND(getter_for(prescale_ramp))
	PrescaleRamp get_prescale_ramp() const;
	GENPYBIND(setter_for(prescale_ramp))
	void set_prescale_ramp(PrescaleRamp const& value);

	GENPYBIND(getter_for(sub_counter))
	SubCounter get_sub_counter() const;
	GENPYBIND(setter_for(sub_counter))
	void set_sub_counter(SubCounter const& value);

	GENPYBIND(getter_for(pause_counter))
	PauseCounter get_pause_counter() const;
	GENPYBIND(setter_for(pause_counter))
	void set_pause_counter(PauseCounter const& value);

	GENPYBIND(getter_for(pulse_a))
	PulseA get_pulse_a() const;
	GENPYBIND(setter_for(pulse_a))
	void set_pulse_a(PulseA const& value);

	GENPYBIND(getter_for(pulse_b))
	PulseB get_pulse_b() const;
	GENPYBIND(setter_for(pulse_b))
	void set_pulse_b(PulseB const& value);

	GENPYBIND(getter_for(boost_a))
	BoostA get_boost_a() const;
	GENPYBIND(setter_for(boost_a))
	void set_boost_a(BoostA const& value);

	GENPYBIND(getter_for(boost_b))
	BoostB get_boost_b() const;
	GENPYBIND(setter_for(boost_b))
	void set_boost_b(BoostB const& value);

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 10;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	bool operator==(CapMemBlockConfig const& other) const;
	bool operator!=(CapMemBlockConfig const& other) const;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<<>(std::ostream&, CapMemBlockConfig<Coordinates> const&);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	bool m_enable_capmem;
	bool m_debug_readout_enable;
	typename Coordinates::CapMemCellOnCapMemBlock m_debug_capmem_coord;
	VRefSelect m_debug_v_ref_select;
	IOutSelect m_debug_i_out_select;
	OutAmpBias m_debug_out_amp_bias;
	SourceFollowerBias m_debug_source_follower_bias;
	LevelShifterBias m_debug_level_shifter_bias;
	VGlobalBias m_v_global_bias;
	CurrentCellRes m_current_cell_res;
	bool m_enable_boost;
	BoostFactor m_boost_factor;
	bool m_enable_autoboost;
	PrescalePause m_prescale_pause;
	PrescaleRamp m_prescale_ramp;
	SubCounter m_sub_counter;
	PauseCounter m_pause_counter;
	PulseA m_pulse_a;
	PulseB m_pulse_b;
	BoostA m_boost_a;
	BoostB m_boost_b;
};

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
		constexpr explicit CapMemAmplifier(uintmax_t const val = 60)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) CapMemOffset
	    : public halco::common::detail::RantWrapper<CapMemOffset, uint_fast8_t, 63, 0>
	{
		constexpr explicit CapMemOffset(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) CapMemSlope
	    : public halco::common::detail::RantWrapper<CapMemSlope, uint_fast8_t, 63, 0>
	{
		constexpr explicit CapMemSlope(uintmax_t const val = 5) GENPYBIND(implicit_conversion) :
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

	/** Set CapMem amplifier bias current. */
	GENPYBIND(getter_for(capmem_amplifier))
	CapMemAmplifier get_capmem_amplifier() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_amplifier))
	void set_capmem_amplifier(CapMemAmplifier value) SYMBOL_VISIBLE;

	/** Set CapMem offset current. */
	GENPYBIND(getter_for(capmem_offset))
	CapMemOffset get_capmem_offset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_offset))
	void set_capmem_offset(CapMemOffset value) SYMBOL_VISIBLE;

	/** Set CapMem slope current. */
	GENPYBIND(getter_for(capmem_slope))
	CapMemSlope get_capmem_slope() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(capmem_slope))
	void set_capmem_slope(CapMemSlope value) SYMBOL_VISIBLE;

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

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;

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
	CapMemAmplifier m_capmem_amplifier;
	CapMemOffset m_capmem_offset;
	CapMemSlope m_capmem_slope;
	ReferenceControl m_reference_control;
	ResistorControl m_resistor_control;
	bool m_enable_reset;
};

namespace detail {

template <typename Coordinates>
struct BackendContainerTrait<CapMemBlockConfig<Coordinates>>
    : public BackendContainerBase<
          CapMemBlockConfig<Coordinates>,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <typename Coordinates>
struct BackendContainerTrait<CapMemBlock<Coordinates>>
    : public BackendContainerBase<
          CapMemBlock<Coordinates>,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <typename Coordinates>
struct BackendContainerTrait<CapMemCell<Coordinates>>
    : public BackendContainerBase<
          CapMemCell<Coordinates>,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <typename Coordinates>
struct VisitPreorderImpl<CapMemBlock<Coordinates>>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename CapMemBlock<Coordinates>::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;

		visitor(coord, config);

		for (auto const cell : iter_all<typename Coordinates::CapMemCellOnCapMemBlock>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			visit_preorder(
			    config.m_capmem_cells[cell], typename Coordinates::CapMemCellOnDLS(cell, coord),
			    visitor);
		}
	}
};

template <>
struct BackendContainerTrait<ReferenceGeneratorConfig>
    : public BackendContainerBase<
          ReferenceGeneratorConfig,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

#define CAPMEM_EXTERN_TEMPLATE(Coordinates)                                                        \
	extern template class SYMBOL_VISIBLE CapMemCell<Coordinates>;                                  \
	extern template class SYMBOL_VISIBLE CapMemBlock<Coordinates>;                                 \
	extern template class SYMBOL_VISIBLE CapMemBlockConfig<Coordinates>;                           \
	std::ostream& operator<<(                                                                      \
	    std::ostream& os, typename CapMemCell<Coordinates>::value_type const& value)               \
	    SYMBOL_VISIBLE GENPYBIND(hidden);                                                          \
                                                                                                   \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusAddress, CapMemCell<Coordinates>::config_size_in_words>      \
	CapMemCell<Coordinates>::addresses<halco::hicann_dls::vx::OmnibusAddress>(                     \
	    coordinate_type const& cell);                                                              \
	extern template SYMBOL_VISIBLE                                                                 \
	    std::array<fisch::vx::OmnibusChipOverJTAG, CapMemCell<Coordinates>::config_size_in_words>  \
	    CapMemCell<Coordinates>::encode<fisch::vx::OmnibusChipOverJTAG>() const;                   \
	extern template SYMBOL_VISIBLE                                                                 \
	    std::array<fisch::vx::Omnibus, CapMemCell<Coordinates>::config_size_in_words>              \
	    CapMemCell<Coordinates>::encode<fisch::vx::Omnibus>() const;                               \
	extern template SYMBOL_VISIBLE void                                                            \
	CapMemCell<Coordinates>::decode<fisch::vx::OmnibusChipOverJTAG>(                               \
	    std::array<                                                                                \
	        fisch::vx::OmnibusChipOverJTAG, CapMemCell<Coordinates>::config_size_in_words> const&  \
	        data);                                                                                 \
	extern template SYMBOL_VISIBLE void CapMemCell<Coordinates>::decode<fisch::vx::Omnibus>(       \
	    std::array<fisch::vx::Omnibus, CapMemCell<Coordinates>::config_size_in_words> const&       \
	        data);                                                                                 \
                                                                                                   \
	extern template SYMBOL_VISIBLE GENPYBIND(hidden) std::ostream& operator<<<Coordinates>(        \
	    std::ostream& os, CapMemCell<Coordinates> const& value);                                   \
	extern template SYMBOL_VISIBLE GENPYBIND(hidden) std::ostream& operator<<<Coordinates>(        \
	    std::ostream& os, CapMemBlock<Coordinates> const& value);                                  \
	extern template SYMBOL_VISIBLE GENPYBIND(hidden) std::ostream& operator<<<Coordinates>(        \
	    std::ostream& os, CapMemBlockConfig<Coordinates> const& value);                            \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    CapMemCell<Coordinates>::config_size_in_words>                                             \
	CapMemCell<Coordinates>::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(         \
	    coordinate_type const& cell);                                                              \
                                                                                                   \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,                                         \
	    CapMemBlockConfig<Coordinates>::config_size_in_words>                                      \
	CapMemBlockConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    halco::hicann_dls::vx::OmnibusAddress,                                                     \
	    CapMemBlockConfig<Coordinates>::config_size_in_words>                                      \
	CapMemBlockConfig<Coordinates>::addresses(coordinate_type const& coord);                       \
	extern template SYMBOL_VISIBLE std::array<                                                     \
	    fisch::vx::OmnibusChipOverJTAG, CapMemBlockConfig<Coordinates>::config_size_in_words>      \
	CapMemBlockConfig<Coordinates>::encode() const;                                                \
	extern template SYMBOL_VISIBLE                                                                 \
	    std::array<fisch::vx::Omnibus, CapMemBlockConfig<Coordinates>::config_size_in_words>       \
	    CapMemBlockConfig<Coordinates>::encode() const;                                            \
	extern template SYMBOL_VISIBLE void CapMemBlockConfig<Coordinates>::decode(                    \
	    std::array<                                                                                \
	        fisch::vx::OmnibusChipOverJTAG,                                                        \
	        CapMemBlockConfig<Coordinates>::config_size_in_words> const& data);                    \
	extern template SYMBOL_VISIBLE void CapMemBlockConfig<Coordinates>::decode(                    \
	    std::array<                                                                                \
	        fisch::vx::Omnibus, CapMemBlockConfig<Coordinates>::config_size_in_words> const&       \
	        data);

} // namespace vx
} // namespace haldls

namespace std {

#define CAPMEM_HALCO_GEOMETRY_HASH(Coordinates)                                                    \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemCell<Coordinates>::Value)                 \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemCell<Coordinates>::DisableRefresh)        \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::OutAmpBias)     \
	HALCO_GEOMETRY_HASH_CLASS(                                                                     \
	    typename haldls::vx::CapMemBlockConfig<Coordinates>::SourceFollowerBias)                   \
	HALCO_GEOMETRY_HASH_CLASS(                                                                     \
	    typename haldls::vx::CapMemBlockConfig<Coordinates>::LevelShifterBias)                     \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::VGlobalBias)    \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::CurrentCellRes) \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::BoostFactor)    \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::PrescalePause)  \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::PrescaleRamp)   \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::SubCounter)     \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::PauseCounter)   \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::PulseA)         \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::PulseB)         \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::BoostA)         \
	HALCO_GEOMETRY_HASH_CLASS(typename haldls::vx::CapMemBlockConfig<Coordinates>::BoostB)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ReferenceGeneratorConfig::CapMemAmplifier)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ReferenceGeneratorConfig::CapMemOffset)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ReferenceGeneratorConfig::CapMemSlope)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ReferenceGeneratorConfig::ReferenceControl)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::ReferenceGeneratorConfig::ResistorControl)

} // namespace std
