#pragma once

#include <cstdint>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"
#include "haldls/container/v2/common.h"

namespace haldls {
namespace container {
namespace v2 {

class CapMemCell
{
public:
	typedef halco::hicann_dls::v2::CapMemCellOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct Value
		: public halco::common::detail::RantWrapper<Value, uint_fast16_t, 1023, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	CapMemCell() HALDLS_VISIBLE;
	explicit CapMemCell(Value const& value) HALDLS_VISIBLE;

	Value get_value() const HALDLS_VISIBLE;
	void set_value(Value const& value) HALDLS_VISIBLE;

	static size_t constexpr config_size_in_words = 1;
	std::array<hardware_address_type, config_size_in_words> addresses(coordinate_type const& cell) const HALDLS_VISIBLE;
	std::array<hardware_word_type, config_size_in_words> encode() const HALDLS_VISIBLE;
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) HALDLS_VISIBLE;

	bool operator==(CapMemCell const& other) const HALDLS_VISIBLE;
	bool operator!=(CapMemCell const& other) const HALDLS_VISIBLE;

private:
	Value m_value;
};

class CapMem
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::false_type has_local_data;

	/// \brief Default constructor, yielding safe default values.
	CapMem() HALDLS_VISIBLE;

	CapMemCell get(halco::hicann_dls::v2::CapMemCellOnDLS const& cell) const HALDLS_VISIBLE;
	void set(halco::hicann_dls::v2::CapMemCellOnDLS const& cell, CapMemCell const& value) HALDLS_VISIBLE;

	CapMemCell get(
		halco::hicann_dls::v2::NeuronOnDLS const& neuron,
		halco::hicann_dls::v2::NeuronParameter const& neuron_parameter) const HALDLS_VISIBLE;
	void set(
		halco::hicann_dls::v2::NeuronOnDLS const& neuron,
		halco::hicann_dls::v2::NeuronParameter const& neuron_parameter,
		CapMemCell const& value) HALDLS_VISIBLE;
	CapMemCell get(
		halco::hicann_dls::v2::CommonNeuronParameter const& common_parameter) const HALDLS_VISIBLE;
	void set(
		halco::hicann_dls::v2::CommonNeuronParameter const& common_parameter,
		CapMemCell const& value) HALDLS_VISIBLE;

	bool operator==(CapMem const& other) const HALDLS_VISIBLE;
	bool operator!=(CapMem const& other) const HALDLS_VISIBLE;

	friend detail::VisitPreorderImpl<CapMem>;

private:
	halco::common::typed_array<CapMemCell, halco::hicann_dls::v2::CapMemCellOnDLS> m_capmem_cells;
};


class CommonCapMemConfig
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	struct OutAmpBias
		: public halco::common::detail::RantWrapper<OutAmpBias, uint_fast16_t, 15, 0>
	{
		constexpr explicit OutAmpBias(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct SourceFollowerBias
		: public halco::common::detail::RantWrapper<SourceFollowerBias, uint_fast16_t, 15, 0>
	{
		constexpr explicit SourceFollowerBias(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct LevelShifterBias
		: public halco::common::detail::RantWrapper<LevelShifterBias, uint_fast16_t, 15, 0>
	{
		constexpr explicit LevelShifterBias(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	struct VGlobalBias
		: public halco::common::detail::RantWrapper<VGlobalBias, uint_fast16_t, 15, 0>
	{
		constexpr explicit VGlobalBias(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct CurrentCellRes
		: public halco::common::detail::RantWrapper<CurrentCellRes, uint_fast16_t, 63, 0>
	{
		constexpr explicit CurrentCellRes(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct BoostFactor
		: public halco::common::detail::RantWrapper<BoostFactor, uint_fast16_t, 15, 0>
	{
		constexpr explicit BoostFactor(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct PrescalePause
		: public halco::common::detail::RantWrapper<PrescalePause, uint_fast16_t, 6, 0>
	{
		constexpr explicit PrescalePause(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct PrescaleRamp
		: public halco::common::detail::RantWrapper<PrescaleRamp, uint_fast16_t, 6, 0>
	{
		constexpr explicit PrescaleRamp(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct SubCounter
		: public halco::common::detail::
			  RantWrapper<SubCounter, uint_fast16_t, 65535 /* 2^16*-1 */, 0>
	{
		constexpr explicit SubCounter(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct PauseCounter
		: public halco::common::detail::
			  RantWrapper<PauseCounter, uint_fast32_t, 4294967295 /* 2^32-1 */, 0>
	{
		constexpr explicit PauseCounter(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct PulseA
		: public halco::common::detail::RantWrapper<PulseA, uint_fast16_t, 65535, 0>
	{
		constexpr explicit PulseA(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct PulseB
		: public halco::common::detail::RantWrapper<PulseB, uint_fast16_t, 65535, 0>
	{
		constexpr explicit PulseB(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct BoostA
		: public halco::common::detail::RantWrapper<BoostA, uint_fast16_t, 65535, 0>
	{
		constexpr explicit BoostA(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};
	struct BoostB
		: public halco::common::detail::RantWrapper<BoostB, uint_fast16_t, 65535, 0>
	{
		constexpr explicit BoostB(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	enum class VRefSelect : uint_fast8_t
	{
		disabled = 0,
		v_ref_v = 1,
		v_ref_i = 2
	};

	enum class IOutSelect : uint_fast8_t
	{
		disabled = 0,
		i_out_mux = 1,
		i_out_ramp = 2
	};

	CommonCapMemConfig() HALDLS_VISIBLE;

	bool get_enable_capmem() const HALDLS_VISIBLE;
	void set_enable_capmem(bool const value) HALDLS_VISIBLE;

	bool get_debug_readout_enable() const HALDLS_VISIBLE;
	void set_debug_readout_enable(bool const value) HALDLS_VISIBLE;

	halco::hicann_dls::v2::CapMemCellOnDLS get_debug_capmem_coord() const HALDLS_VISIBLE;
	void set_debug_capmem_coord(halco::hicann_dls::v2::CapMemCellOnDLS const& value) HALDLS_VISIBLE;

	VRefSelect get_debug_v_ref_select() const HALDLS_VISIBLE;
	void set_debug_v_ref_select(VRefSelect const& value) HALDLS_VISIBLE;

	IOutSelect get_debug_i_out_select() const HALDLS_VISIBLE;
	void set_debug_i_out_select(IOutSelect const& value) HALDLS_VISIBLE;

	OutAmpBias get_debug_out_amp_bias() const HALDLS_VISIBLE;
	void set_debug_out_amp_bias(OutAmpBias const& value) HALDLS_VISIBLE;

	SourceFollowerBias get_debug_source_follower_bias() const HALDLS_VISIBLE;
	void set_debug_source_follower_bias(SourceFollowerBias const& value) HALDLS_VISIBLE;

	LevelShifterBias get_debug_level_shifter_bias() const HALDLS_VISIBLE;
	void set_debug_level_shifter_bias(LevelShifterBias const& value) HALDLS_VISIBLE;

	VGlobalBias get_v_global_bias() const HALDLS_VISIBLE;
	void set_v_global_bias(VGlobalBias const& value) HALDLS_VISIBLE;

	CurrentCellRes get_current_cell_res() const HALDLS_VISIBLE;
	void set_current_cell_res(CurrentCellRes const& value) HALDLS_VISIBLE;

	BoostFactor get_boost_factor() const HALDLS_VISIBLE;
	void set_boost_factor(BoostFactor const& value) HALDLS_VISIBLE;

	bool get_enable_boost() const HALDLS_VISIBLE;
	void set_enable_boost(bool const value) HALDLS_VISIBLE;

	bool get_enable_autoboost() const HALDLS_VISIBLE;
	void set_enable_autoboost(bool const value) HALDLS_VISIBLE;

	PrescalePause get_prescale_pause() const HALDLS_VISIBLE;
	void set_prescale_pause(PrescalePause const& value) HALDLS_VISIBLE;

	PrescaleRamp get_prescale_ramp() const HALDLS_VISIBLE;
	void set_prescale_ramp(PrescaleRamp const& value) HALDLS_VISIBLE;

	SubCounter get_sub_counter() const HALDLS_VISIBLE;
	void set_sub_counter(SubCounter const& value) HALDLS_VISIBLE;

	PauseCounter get_pause_counter() const HALDLS_VISIBLE;
	void set_pause_counter(PauseCounter const& value) HALDLS_VISIBLE;

	PulseA get_pulse_a() const HALDLS_VISIBLE;
	void set_pulse_a(PulseA const& value) HALDLS_VISIBLE;

	PulseB get_pulse_b() const HALDLS_VISIBLE;
	void set_pulse_b(PulseB const& value) HALDLS_VISIBLE;

	BoostA get_boost_a() const HALDLS_VISIBLE;
	void set_boost_a(BoostA const& value) HALDLS_VISIBLE;

	BoostB get_boost_b() const HALDLS_VISIBLE;
	void set_boost_b(BoostB const& value) HALDLS_VISIBLE;

	bool operator==(CommonCapMemConfig const& other) const HALDLS_VISIBLE;
	bool operator!=(CommonCapMemConfig const& other) const HALDLS_VISIBLE;

private:
	bool m_enable_capmem;
	bool m_debug_readout_enable;
	halco::hicann_dls::v2::CapMemCellOnDLS m_debug_capmem_coord;
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

namespace detail {

template <>
struct VisitPreorderImpl<CapMem> {
	template <typename ContainerT, typename VisitorT>
	static void call(ContainerT& config, halco::common::Unique const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::v2;

		visitor(coord, config);

		for (auto const cell : iter_all<CapMemCellOnDLS>()) {
			// No std::forward for visitor argument, as we want to pass a reference to the
			// nested visitor in any case, even if it was passed as an rvalue to this function.
			visit_preorder(config.m_capmem_cells[cell], cell, visitor);
		}
	}
};

} // namespace detail

} // namespace v2
} // namespace container
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CapMemCell::Value)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::OutAmpBias)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::SourceFollowerBias)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::LevelShifterBias)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::VGlobalBias)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::CurrentCellRes)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::BoostFactor)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::PrescalePause)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::PrescaleRamp)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::SubCounter)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::PauseCounter)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::PulseA)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::PulseB)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::BoostA)
HALCO_GEOMETRY_HASH_CLASS(haldls::container::v2::CommonCapMemConfig::BoostB)

} // namespace std
