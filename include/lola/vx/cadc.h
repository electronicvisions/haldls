#pragma once

#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/common/typed_heap_array.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include "lola/vx/synapse.h"
#include <boost/hana/adapt_struct.hpp>

#define ID(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
// clang-format off
#define CONCAT(w, x, y, z) STR(ID(w)ID(/)ID(x)ID(/)ID(v)ID(y)ID(/)ID(z))
#include CONCAT(haldls,vx,CHIP_REVISION,capmem.h)
#include CONCAT(haldls,vx,CHIP_REVISION,correlation.h)
// clang-format on
#undef ID
#undef XSTR
#undef STR
#undef CONCAT

#if CHIP_REVISION == 3
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V3
#define CHIP_REVISION_STR v3
#elif CHIP_REVISION == 4
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V4
#define CHIP_REVISION_STR v4
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx {
namespace CHIP_REVISION_STR GENPYBIND_TAG_LOLA_VX_VY {
#undef GENPYBIND_TAG_LOLA_VX_VY

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include <pybind11/numpy.h>
#endif

/**
 * Configuration container for CADC and correlation/neuron-readout related settings.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) CADCReadoutChain
    : public haldls::vx::ContainerBase<CADCReadoutChain>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::CADCOnDLS coordinate_type;

	/**
	 * Configuration of each CADC channel. Contains individual calibration and connection settings.
	 */
	class GENPYBIND(visible) Channel
	{
	public:
		typedef haldls::vx::CADCChannelConfig::Offset Offset GENPYBIND(visible);

		/**
		 * Digital offset for the result obtained by the CADC channel.
		 * Typically obtained from calibration.
		 */
		Offset offset{};

		/**
		 * Decide whether the CADC channel is connected to the synapse array. This is required when
		 * reading either from the synapses' correlation sensors or from the neurons.
		 */
		bool enable_connect_correlation{false};
		/**
		 * Decide whether the CADC channel is connected to a common debug line. This must not be
		 * enabled if all channels are to be used independently.
		 */
		bool enable_connect_debug{false};
		/**
		 * Decide whether the line through the synapse array is connected to the neuron readout
		 * buffer. This must not be enabled if correlations are to be read.
		 */
		bool enable_connect_neuron{false};

		Channel() = default;

		bool operator==(Channel const& other) const SYMBOL_VISIBLE;
		bool operator!=(Channel const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Channel const& config) SYMBOL_VISIBLE;
	};

	/**
	 * Configuration of each CADC channel. Contains individual calibration and connection settings.
	 */
	typedef halco::common::typed_array<Channel, halco::hicann_dls::vx::CADCChannelColumnOnSynram>
	    ChannelArray GENPYBIND(opaque);

	typedef halco::common::typed_array<
	    haldls::vx::CHIP_REVISION_STR::CapMemCell::value_type,
	    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnHemisphere>
	    AnalogValues;

	/**
	 * CADC ramp generation settings.
	 */
	class GENPYBIND(visible) Ramp
	{
	public:
		typedef haldls::vx::CADCConfig::ResetWait ResetWait GENPYBIND(visible);
		typedef haldls::vx::CADCConfig::DeadTime DeadTime GENPYBIND(visible);
		typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::Value AnalogValue GENPYBIND(visible);
		typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::DisableRefresh AnalogDisableRefresh
		    GENPYBIND(visible);

		typedef haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig::RampOffsets OffsetAssignment
		    GENPYBIND(visible);

		/**
		 * Enable CADC for the current hemisphere.
		 */
		bool enable{false};

		/**
		 * Reset duration of the CADC ramp. At the beginning of a read (after the sense_delay has
		 * passed), the ramp is pulled down from 1.2 V to v_offset for this time. Given in PPU clock
		 * cycles.
		 */
		ResetWait reset_wait{};
		/**
		 * Dead time before a read returns, after the CADC ramp has increased to 1.2 V. Given in PPU
		 * clock cycles.
		 */
		DeadTime dead_time{};

		/**
		 * Select the assignment of v_offset between left and right CapMem instances.
		 * Each of the ramp generators (left and right CADC branch) can independently select a
		 * source for their v_offset voltage (left and right CapMem).
		 */
		OffsetAssignment v_offset_assignment{
		    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnHemisphere(0),
		    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnHemisphere(1)};
		/**
		 * Ramp start voltage.
		 */
		CADCReadoutChain::AnalogValues v_offset{};
		/**
		 * Ramp current, determines slope of the linear voltage range.
		 */
		CADCReadoutChain::AnalogValues i_slope{};
		/**
		 * Bias current for the comparator in each CADC channel.
		 */
		CADCReadoutChain::AnalogValues i_bias_comparator{};
		/**
		 * Bias current for the buffer for v_offset, relevant during reset of the ramp.
		 */
		CADCReadoutChain::AnalogValues i_bias_vreset_buffer{};
		/**
		 * Bias voltage for the buffer distributing the ramp to each channel.
		 */
		CADCReadoutChain::AnalogValues v_bias_buffer{};

		Ramp() = default;

		bool operator==(Ramp const& other) const SYMBOL_VISIBLE;
		bool operator!=(Ramp const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Ramp const& config) SYMBOL_VISIBLE;
	};

	/**
	 * Synapses' correlation settings, affecting correlation reset, measurement and readout, but
	 * also CADC behaviour and neuron resets and readout.
	 */
	struct GENPYBIND(visible) Correlation
	{
		typedef haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig::SenseDelay SenseDelay
		    GENPYBIND(visible);
		typedef haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig::ResetDuration ResetDuration
		    GENPYBIND(visible);
		typedef haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig::ResetFallTime ResetFallTime
		    GENPYBIND(visible);
		typedef haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig::ResetMode ResetMode
		    GENPYBIND(visible);
		typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::Value AnalogValue GENPYBIND(visible);
		typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::DisableRefresh AnalogDisableRefresh
		    GENPYBIND(visible);

		/**
		 * Delay between issuing a read command, i.e. connecting correlation output of a given
		 * synapse row to the CADC channels, and triggering the CADC read. Given in PPU clock
		 * cycles. This setting also adds a delay to reading potentials from the neurons and the
		 * start of the auto-calibrating reset.
		 */
		SenseDelay sense_delay{};
		/**
		 * Duration of a correlation reset. Has to be sufficient to charge the correlation
		 * accumulation capacitor. Given in PPU clock cycles. This setting also affects the parallel
		 * neuron reset.
		 */
		ResetDuration reset_duration{};
		/**
		 * When using auto-calibrating correlation reset, this setting controls the maximum
		 * discharge time per synapse, after the initial reset to a higher voltage. The comparator
		 * needs to trigger within this time. Given in PPU clock cycles.
		 */
		ResetFallTime reset_fall_time{};

		/**
		 * Select between normal and auto-calibrating correlation reset.
		 *
		 * Normal reset:
		 * The correlation accumulation capacitors are connected to an external voltage (v_reset)
		 * for a given period of time (reset_duration).
		 *
		 * Auto-calibrating reset:
		 * The correlation readout is enabled for the synapses to be reset. The correlation
		 * accumulation capacitor is connected to an external voltage (v_reset) for reset_duration.
		 * V_reset must be higher than the target reset potential. After this initial reset, the
		 * capacitor is discharged by a constant current (controlled by i_bias_corout) until the
		 * column's CADC comparator triggers. The resulting potential on the correlation
		 * accumulation capacitor should therefore be close to v_offset of the CADC (given i_slope
		 * is zero). Note that while using auto-calibrating reset, the CADC cannot be used normally,
		 * a read will never return.
		 */
		ResetMode reset_mode{ResetMode::normal};

		/**
		 * Correlation ramp bias, controlling the time constant of the correlation measurements.
		 * A lower ramp current corresponds to a longer time constant.
		 */
		CADCReadoutChain::AnalogValues i_bias_ramp{};
		/**
		 * Correlation storage bias, controlling the amplitude of each correlated spike pair that is
		 * stored on the accumulation capacitor. Lower bias currents result in a higher amplitude
		 * per spike pair.
		 */
		CADCReadoutChain::AnalogValues i_bias_store{};
		/**
		 * Correlation readout bias current, applied to the source follower in the readout of the
		 * accumulation capacitor. When using auto-calibrating reset, this current also controls the
		 * discharge rate of the correlation capacitor after the initial reset.
		 */
		CADCReadoutChain::AnalogValues i_bias_corout{};

		Correlation() = default;

		bool operator==(Correlation const& other) const SYMBOL_VISIBLE;
		bool operator!=(Correlation const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Correlation const& config) SYMBOL_VISIBLE;
	};

	/**
	 * CADC ramp generation settings.
	 */
	Ramp ramp{};
	/**
	 * Individual channel settings, like their connections.
	 */
	ChannelArray channels_causal{};
	/**
	 * Individual channel settings, like their connections.
	 */
	ChannelArray channels_acausal{};
	/**
	 * Synapses' correlation settings, affecting correlation reset, measurement and readout, but
	 * also CADC behaviour and neuron resets and readout.
	 */
	Correlation correlation{};

	CADCReadoutChain() = default;

	bool operator==(CADCReadoutChain const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCReadoutChain const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCReadoutChain const& config)
	    SYMBOL_VISIBLE;

	// TODO (Issue 3622): Manual wrapping is necessary due to genpybind forgetting full
	// qualification
	GENPYBIND_MANUAL({
		auto av = parent->py::template class_<
		    ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues>(parent, "AnalogValues");
		av.def(
		    "fill", &::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::fill, "",
		    parent->py::arg("val"));
		{
			typedef ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::reference (
			    ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::*genpybind_at_type)(
			    const ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::key_type&);
			av.def(
			    "at",
			    (genpybind_at_type) &
			        ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::at,
			    "", parent->py::arg("key"));
		}
		{
			typedef ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::const_reference (
			    ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::*genpybind_at_type)(
			    const ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::key_type&)
			    const;
			av.def(
			    "at",
			    (genpybind_at_type) &
			        ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::at,
			    "", parent->py::arg("key"));
		}
		{
			typedef ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::reference (
			    ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::*
			        genpybind_front_type)();
			av.def(
			    "front",
			    (genpybind_front_type) &
			        ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::front,
			    "");
		}
		{
			typedef ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::const_reference (
			    ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::*
			        genpybind_front_type)() const;
			av.def(
			    "front",
			    (genpybind_front_type) &
			        ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::front,
			    "");
		}
		{
			typedef ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::reference (
			    ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::*
			        genpybind_back_type)();
			av.def(
			    "back",
			    (genpybind_back_type) &
			        ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::back,
			    "");
		}
		{
			typedef ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::const_reference (
			    ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::*
			        genpybind_back_type)() const;
			av.def(
			    "back",
			    (genpybind_back_type) &
			        ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::back,
			    "");
		}
		av.def(
		    "__getitem__", &::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::get, "",
		    parent->py::arg("key"), parent->py::return_value_policy::reference);
		av.def(
		    "__setitem__", &::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::set, "",
		    parent->py::arg("key"), parent->py::arg("value"));
		av.def(
		    "__iter__",
		    [av](::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues& self) {
			    return pybind11::make_iterator(self);
		    },
		    parent->py::template keep_alive<0, 1>());

		av.def(
		    "to_numpy",
		    [](::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues const& self) {
			    return ::halco::common::detail::to_numpy(self);
		    });
		av.def(
		    "from_numpy",
		    [](::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues& self,
		       pybind11::array const& array) { ::halco::common::detail::from_numpy(self, array); });
		av.def(
		    parent->py::template init<
		        const ::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues&>(),
		    "");
		av.def(parent->py::template init<>(), "");
		av.def_property_readonly(
		    "size", parent->py::cpp_function(
		                &::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::size));
		av.def_property_readonly(
		    "max_size",
		    parent->py::cpp_function(
		        &::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::max_size));
		av.def_property_readonly(
		    "empty", parent->py::cpp_function(
		                 &::lola::vx::CHIP_REVISION_STR::CADCReadoutChain::AnalogValues::empty));
	})
private:
	friend struct haldls::vx::detail::VisitPreorderImpl<CADCReadoutChain>;
};


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) CADCSampleRow
    : public haldls::vx::ContainerBase<CADCSampleRow>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::CADCSampleRowOnDLS coordinate_type;

	typedef haldls::vx::CADCSampleQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _samples_type GENPYBIND(opaque);

	/** Default constructor. */
	CADCSampleRow() SYMBOL_VISIBLE;

	/** Causal sample value array. */
	_samples_type causal;

	/** Acausal sample value array. */
	_samples_type acausal;

	bool operator==(CADCSampleRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCSampleRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCSampleRow const& row) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<CADCSampleRow>;
};


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) CADCSamples
    : public haldls::vx::ContainerBase<CADCSamples>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::CADCSamplesOnDLS coordinate_type;

	typedef haldls::vx::CADCSampleQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<
	    halco::common::typed_array<Value, halco::hicann_dls::vx::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::SynramOnDLS>
	    _samples_type GENPYBIND(opaque);

	/** Default constructor. */
	CADCSamples() SYMBOL_VISIBLE;

	_samples_type causal;
	_samples_type acausal;

	bool operator==(CADCSamples const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCSamples const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCSamples const& row) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<CADCSamples>;
};

} // namespace vx
} // namespace lola

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::CADCSampleRow>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::CADCSampleRow,
          fisch::vx::word_access_type::Omnibus>
{};

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::CADCSamples>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::CADCSamples,
          fisch::vx::word_access_type::Omnibus>
{};

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::CADCReadoutChain>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::CADCReadoutChain,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::CADCReadoutChain>
{
	template <typename VisitorT>
	static void call(
	    hate::Empty<lola::vx::CHIP_REVISION_STR::CADCReadoutChain> const& config,
	    lola::vx::CHIP_REVISION_STR::CADCReadoutChain::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		hate::Empty<haldls::vx::CADCConfig> cadc_config;
		auto const cadc_coord = coord.toCADCConfigOnDLS();
		visit_preorder(cadc_config, cadc_coord, visitor);

		for (auto const block : iter_all<CapMemBlockOnHemisphere>()) {
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_v_offset;
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_i_slope;
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_i_bias_comp;
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_i_bias_vreset_buf;
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_v_bias_buf;
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_i_bias_ramp;
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_i_bias_store;
			hate::Empty<haldls::vx::CHIP_REVISION_STR::CapMemCell> cell_i_bias_corout;

			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnDLS capmem_block(
			    block.toEnum() +
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnHemisphere::size *
			        coord.toEnum());
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_v_offset_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_v_ramp_offset,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_slope_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_i_ramp_slope,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_comp_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::cadc_i_bias_comp,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_vreset_buf_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_i_bias_vreset_buf,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_v_bias_buf_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_v_bias_ramp_buf,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_ramp_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::syn_i_bias_ramp,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_store_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::syn_i_bias_store,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_corout_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        syn_i_bias_corout,
			    capmem_block);
			visit_preorder(cell_v_offset, cell_v_offset_coord, visitor);
			visit_preorder(cell_i_slope, cell_i_slope_coord, visitor);
			visit_preorder(cell_i_bias_comp, cell_i_bias_comp_coord, visitor);
			visit_preorder(cell_i_bias_vreset_buf, cell_i_bias_vreset_buf_coord, visitor);
			visit_preorder(cell_v_bias_buf, cell_v_bias_buf_coord, visitor);
			visit_preorder(cell_i_bias_ramp, cell_i_bias_ramp_coord, visitor);
			visit_preorder(cell_i_bias_store, cell_i_bias_store_coord, visitor);
			visit_preorder(cell_i_bias_corout, cell_i_bias_corout_coord, visitor);
		}

		for (auto const column : iter_all<CADCChannelColumnOnSynram>()) {
			hate::Empty<haldls::vx::CADCChannelConfig> channel_config;

			CADCChannelConfigOnDLS channel_coord(
			    CADCChannelConfigOnSynram(column, CADCChannelType::causal), coord.toSynramOnDLS());
			visit_preorder(channel_config, channel_coord, visitor);
		}

		for (auto const column : iter_all<CADCChannelColumnOnSynram>()) {
			hate::Empty<haldls::vx::CADCChannelConfig> channel_config;

			CADCChannelConfigOnDLS channel_coord(
			    CADCChannelConfigOnSynram(column, CADCChannelType::acausal), coord.toSynramOnDLS());
			visit_preorder(channel_config, channel_coord, visitor);
		}

		hate::Empty<lola::vx::ColumnCorrelationRow> column_correlation_row;
		auto const column_correlation_row_coord = coord.toColumnCorrelationRowOnDLS();
		visit_preorder(column_correlation_row, column_correlation_row_coord, visitor);

		hate::Empty<haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig> correlation_config;
		auto const correlation_coord = coord.toCommonCorrelationConfigOnDLS();
		visit_preorder(correlation_config, correlation_coord, visitor);
	}


	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::CHIP_REVISION_STR::CADCReadoutChain::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		haldls::vx::CADCConfig cadc_config;
		cadc_config.set_enable(config.ramp.enable);
		cadc_config.set_reset_wait(config.ramp.reset_wait);
		cadc_config.set_dead_time(config.ramp.dead_time);
		hate::Empty<halco::hicann_dls::vx::CADCConfigOnDLS> const cadc_coord;
		visit_preorder(cadc_config, cadc_coord, visitor);

		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
			config.ramp.enable = cadc_config.get_enable();
			config.ramp.reset_wait = cadc_config.get_reset_wait();
			config.ramp.dead_time = cadc_config.get_dead_time();
		}

		for (auto const block : iter_all<CapMemBlockOnHemisphere>()) {
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_v_offset(config.ramp.v_offset[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_slope(config.ramp.i_slope[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_comp(
			    config.ramp.i_bias_comparator[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_vreset_buf(
			    config.ramp.i_bias_vreset_buffer[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_v_bias_buf(
			    config.ramp.v_bias_buffer[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_ramp(
			    config.correlation.i_bias_ramp[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_store(
			    config.correlation.i_bias_store[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_corout(
			    config.correlation.i_bias_corout[block]);

			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_v_offset_coord;
			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_i_slope_coord;
			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_i_bias_comp_coord;
			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_i_bias_vreset_buf_coord;
			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_v_bias_buf_coord;
			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_i_bias_ramp_coord;
			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_i_bias_store_coord;
			hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS>
			    cell_i_bias_corout_coord;
			visit_preorder(cell_v_offset, cell_v_offset_coord, visitor);
			visit_preorder(cell_i_slope, cell_i_slope_coord, visitor);
			visit_preorder(cell_i_bias_comp, cell_i_bias_comp_coord, visitor);
			visit_preorder(cell_i_bias_vreset_buf, cell_i_bias_vreset_buf_coord, visitor);
			visit_preorder(cell_v_bias_buf, cell_v_bias_buf_coord, visitor);
			visit_preorder(cell_i_bias_ramp, cell_i_bias_ramp_coord, visitor);
			visit_preorder(cell_i_bias_store, cell_i_bias_store_coord, visitor);
			visit_preorder(cell_i_bias_corout, cell_i_bias_corout_coord, visitor);

			if constexpr (!std::is_same<
			                  ContainerT,
			                  lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
				config.ramp.v_offset[block] = cell_v_offset.get_value();
				config.ramp.i_slope[block] = cell_i_slope.get_value();
				config.ramp.i_bias_comparator[block] = cell_i_bias_comp.get_value();
				config.ramp.i_bias_vreset_buffer[block] = cell_i_bias_vreset_buf.get_value();
				config.ramp.v_bias_buffer[block] = cell_v_bias_buf.get_value();
				config.correlation.i_bias_ramp[block] = cell_i_bias_ramp.get_value();
				config.correlation.i_bias_store[block] = cell_i_bias_store.get_value();
				config.correlation.i_bias_corout[block] = cell_i_bias_corout.get_value();
			}
		}

		for (auto const column : iter_all<CADCChannelColumnOnSynram>()) {
			haldls::vx::CADCChannelConfig channel_config;
			channel_config.set_offset(config.channels_causal[column].offset);

			hate::Empty<CADCChannelConfigOnDLS> channel_coord;
			visit_preorder(channel_config, channel_coord, visitor);

			if constexpr (!std::is_same<
			                  ContainerT,
			                  lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
				config.channels_causal[column].offset = channel_config.get_offset();
			}
		}

		for (auto const column : iter_all<CADCChannelColumnOnSynram>()) {
			haldls::vx::CADCChannelConfig channel_config;
			channel_config.set_offset(config.channels_acausal[column].offset);

			hate::Empty<CADCChannelConfigOnDLS> channel_coord;
			visit_preorder(channel_config, channel_coord, visitor);

			if constexpr (!std::is_same<
			                  ContainerT,
			                  lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
				config.channels_acausal[column].offset = channel_config.get_offset();
			}
		}

		lola::vx::ColumnCorrelationRow column_correlation_row;
		for (auto const synapse : iter_all<SynapseOnSynapseRow>()) {
			auto const column = synapse.toCADCChannelColumnOnSynram();

			auto& sw = column_correlation_row.values[synapse];
			sw.set_enable_internal_causal(
			    config.channels_causal[column].enable_connect_correlation);
			sw.set_enable_internal_acausal(
			    config.channels_acausal[column].enable_connect_correlation);
			sw.set_enable_debug_causal(config.channels_causal[column].enable_connect_debug);
			sw.set_enable_debug_acausal(config.channels_acausal[column].enable_connect_debug);
			sw.set_enable_cadc_neuron_readout_causal(
			    config.channels_causal[column].enable_connect_neuron);
			sw.set_enable_cadc_neuron_readout_acausal(
			    config.channels_acausal[column].enable_connect_neuron);
		}

		hate::Empty<halco::hicann_dls::vx::CHIP_REVISION_STR::ColumnCorrelationRowOnDLS> const
		    column_correlation_row_coord;
		visit_preorder(column_correlation_row, column_correlation_row_coord, visitor);

		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
			for (auto const synapse : iter_all<SynapseOnSynapseRow>()) {
				auto const column = synapse.toCADCChannelColumnOnSynram();

				auto const& sw = column_correlation_row.values[synapse];
				config.channels_causal[column].enable_connect_correlation =
				    sw.get_enable_internal_causal();
				config.channels_acausal[column].enable_connect_correlation =
				    sw.get_enable_internal_acausal();
				config.channels_causal[column].enable_connect_debug = sw.get_enable_debug_causal();
				config.channels_acausal[column].enable_connect_debug =
				    sw.get_enable_debug_acausal();
				config.channels_causal[column].enable_connect_neuron =
				    sw.get_enable_cadc_neuron_readout_causal();
				config.channels_acausal[column].enable_connect_neuron =
				    sw.get_enable_cadc_neuron_readout_acausal();
			}
		}

		haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig correlation_config;
		correlation_config.set_sense_delay(config.correlation.sense_delay);
		correlation_config.set_reset_duration(config.correlation.reset_duration);
		correlation_config.set_reset_fall_time(config.correlation.reset_fall_time);
		correlation_config.set_reset_mode(config.correlation.reset_mode);
		correlation_config.set_cadc_v_offset_assignment(config.ramp.v_offset_assignment);
		hate::Empty<halco::hicann_dls::vx::CommonCorrelationConfigOnDLS> const correlation_coord;
		visit_preorder(correlation_config, correlation_coord, visitor);

		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
			config.correlation.sense_delay = correlation_config.get_sense_delay();
			config.correlation.reset_duration = correlation_config.get_reset_duration();
			config.correlation.reset_fall_time = correlation_config.get_reset_fall_time();
			config.correlation.reset_mode = correlation_config.get_reset_mode();
			config.ramp.v_offset_assignment = correlation_config.get_cadc_v_offset_assignment();
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::CHIP_REVISION_STR::CADCReadoutChain::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		haldls::vx::CADCConfig cadc_config;
		cadc_config.set_enable(config.ramp.enable);
		cadc_config.set_reset_wait(config.ramp.reset_wait);
		cadc_config.set_dead_time(config.ramp.dead_time);
		auto const cadc_coord = coord.toCADCConfigOnDLS();
		visit_preorder(cadc_config, cadc_coord, visitor);

		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
			config.ramp.enable = cadc_config.get_enable();
			config.ramp.reset_wait = cadc_config.get_reset_wait();
			config.ramp.dead_time = cadc_config.get_dead_time();
		}

		for (auto const block : iter_all<CapMemBlockOnHemisphere>()) {
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_v_offset(config.ramp.v_offset[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_slope(config.ramp.i_slope[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_comp(
			    config.ramp.i_bias_comparator[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_vreset_buf(
			    config.ramp.i_bias_vreset_buffer[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_v_bias_buf(
			    config.ramp.v_bias_buffer[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_ramp(
			    config.correlation.i_bias_ramp[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_store(
			    config.correlation.i_bias_store[block]);
			haldls::vx::CHIP_REVISION_STR::CapMemCell cell_i_bias_corout(
			    config.correlation.i_bias_corout[block]);

			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnDLS capmem_block(
			    block.toEnum() +
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockOnHemisphere::size *
			        coord.toEnum());
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_v_offset_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_v_ramp_offset,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_slope_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_i_ramp_slope,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_comp_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::cadc_i_bias_comp,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_vreset_buf_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_i_bias_vreset_buf,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_v_bias_buf_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        cadc_v_bias_ramp_buf,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_ramp_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::syn_i_bias_ramp,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_store_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::syn_i_bias_store,
			    capmem_block);
			halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnDLS cell_i_bias_corout_coord(
			    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemCellOnCapMemBlock::
			        syn_i_bias_corout,
			    capmem_block);
			visit_preorder(cell_v_offset, cell_v_offset_coord, visitor);
			visit_preorder(cell_i_slope, cell_i_slope_coord, visitor);
			visit_preorder(cell_i_bias_comp, cell_i_bias_comp_coord, visitor);
			visit_preorder(cell_i_bias_vreset_buf, cell_i_bias_vreset_buf_coord, visitor);
			visit_preorder(cell_v_bias_buf, cell_v_bias_buf_coord, visitor);
			visit_preorder(cell_i_bias_ramp, cell_i_bias_ramp_coord, visitor);
			visit_preorder(cell_i_bias_store, cell_i_bias_store_coord, visitor);
			visit_preorder(cell_i_bias_corout, cell_i_bias_corout_coord, visitor);

			if constexpr (!std::is_same<
			                  ContainerT,
			                  lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
				config.ramp.v_offset[block] = cell_v_offset.get_value();
				config.ramp.i_slope[block] = cell_i_slope.get_value();
				config.ramp.i_bias_comparator[block] = cell_i_bias_comp.get_value();
				config.ramp.i_bias_vreset_buffer[block] = cell_i_bias_vreset_buf.get_value();
				config.ramp.v_bias_buffer[block] = cell_v_bias_buf.get_value();
				config.correlation.i_bias_ramp[block] = cell_i_bias_ramp.get_value();
				config.correlation.i_bias_store[block] = cell_i_bias_store.get_value();
				config.correlation.i_bias_corout[block] = cell_i_bias_corout.get_value();
			}
		}

		for (auto const column : iter_all<CADCChannelColumnOnSynram>()) {
			haldls::vx::CADCChannelConfig channel_config;
			channel_config.set_offset(config.channels_causal[column].offset);

			CADCChannelConfigOnDLS channel_coord(
			    CADCChannelConfigOnSynram(column, CADCChannelType::causal), coord.toSynramOnDLS());
			visit_preorder(channel_config, channel_coord, visitor);

			if constexpr (!std::is_same<
			                  ContainerT,
			                  lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
				config.channels_causal[column].offset = channel_config.get_offset();
			}
		}

		for (auto const column : iter_all<CADCChannelColumnOnSynram>()) {
			haldls::vx::CADCChannelConfig channel_config;
			channel_config.set_offset(config.channels_acausal[column].offset);

			CADCChannelConfigOnDLS channel_coord(
			    CADCChannelConfigOnSynram(column, CADCChannelType::acausal), coord.toSynramOnDLS());
			visit_preorder(channel_config, channel_coord, visitor);

			if constexpr (!std::is_same<
			                  ContainerT,
			                  lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
				config.channels_acausal[column].offset = channel_config.get_offset();
			}
		}

		lola::vx::ColumnCorrelationRow column_correlation_row;
		for (auto const synapse : iter_all<SynapseOnSynapseRow>()) {
			auto const column = synapse.toCADCChannelColumnOnSynram();

			auto& sw = column_correlation_row.values[synapse];
			sw.set_enable_internal_causal(
			    config.channels_causal[column].enable_connect_correlation);
			sw.set_enable_internal_acausal(
			    config.channels_acausal[column].enable_connect_correlation);
			sw.set_enable_debug_causal(config.channels_causal[column].enable_connect_debug);
			sw.set_enable_debug_acausal(config.channels_acausal[column].enable_connect_debug);
			sw.set_enable_cadc_neuron_readout_causal(
			    config.channels_causal[column].enable_connect_neuron);
			sw.set_enable_cadc_neuron_readout_acausal(
			    config.channels_acausal[column].enable_connect_neuron);
		}

		auto const column_correlation_row_coord = coord.toColumnCorrelationRowOnDLS();
		visit_preorder(column_correlation_row, column_correlation_row_coord, visitor);

		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
			for (auto const synapse : iter_all<SynapseOnSynapseRow>()) {
				auto const column = synapse.toCADCChannelColumnOnSynram();

				auto const& sw = column_correlation_row.values[synapse];
				config.channels_causal[column].enable_connect_correlation =
				    sw.get_enable_internal_causal();
				config.channels_acausal[column].enable_connect_correlation =
				    sw.get_enable_internal_acausal();
				config.channels_causal[column].enable_connect_debug = sw.get_enable_debug_causal();
				config.channels_acausal[column].enable_connect_debug =
				    sw.get_enable_debug_acausal();
				config.channels_causal[column].enable_connect_neuron =
				    sw.get_enable_cadc_neuron_readout_causal();
				config.channels_acausal[column].enable_connect_neuron =
				    sw.get_enable_cadc_neuron_readout_acausal();
			}
		}

		haldls::vx::CHIP_REVISION_STR::CommonCorrelationConfig correlation_config;
		correlation_config.set_sense_delay(config.correlation.sense_delay);
		correlation_config.set_reset_duration(config.correlation.reset_duration);
		correlation_config.set_reset_fall_time(config.correlation.reset_fall_time);
		correlation_config.set_reset_mode(config.correlation.reset_mode);
		correlation_config.set_cadc_v_offset_assignment(config.ramp.v_offset_assignment);
		auto const correlation_coord = coord.toCommonCorrelationConfigOnDLS();
		visit_preorder(correlation_config, correlation_coord, visitor);

		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCReadoutChain const>::value) {
			config.correlation.sense_delay = correlation_config.get_sense_delay();
			config.correlation.reset_duration = correlation_config.get_reset_duration();
			config.correlation.reset_fall_time = correlation_config.get_reset_fall_time();
			config.correlation.reset_mode = correlation_config.get_reset_mode();
			config.ramp.v_offset_assignment = correlation_config.get_cadc_v_offset_assignment();
		}
	}
};

template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::CADCSampleRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    lola::vx::CHIP_REVISION_STR::CADCSampleRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCSampleRow const>::value) {
			// trigger ADC sampling by reading one quad of causal channels
			// The values are broken and discarded, see Issue #3637
			hate::Empty<CADCSampleQuad> quad_config_trigger;
			CADCSampleQuadOnDLS quad_coord_trigger(
			    CADCSampleQuadOnSynram(
			        SynapseQuadOnSynram(
			            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min),
			            coord.toSynapseRowOnSynram()),
			        CADCChannelType::causal, CADCReadoutType::trigger_read),
			    coord.toSynramOnDLS());
			visit_preorder(quad_config_trigger, quad_coord_trigger, visitor);

			// buffered read of remaining causal channel quads:
			// This reads back the results that were acquired during the last
			// triggered measurement, which was just done above.
			for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max;
			     ++i) {
				auto quad_column = SynapseQuadColumnOnDLS(i);
				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::causal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				hate::Empty<CADCSampleQuad> quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
			}
			// buffered read of acausal channel quads
			for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::acausal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				hate::Empty<CADCSampleQuad> quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::CHIP_REVISION_STR::CADCSampleRow::coordinate_type> coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCSampleRow const>::value) {
			// trigger ADC sampling by reading one quad of causal channels
			// The values are broken and discarded, see Issue #3637
			CADCSampleQuad quad_config_trigger;
			hate::Empty<CADCSampleQuadOnDLS> quad_coord;
			visit_preorder(quad_config_trigger, quad_coord, visitor);

			// Use results of triggered read: write into result array
			for (auto const entry : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow syn_on_row(entry, SynapseQuadColumnOnDLS(0));
				config.causal[syn_on_row] = quad_config_trigger.get_sample(entry);
			}
			// buffered read of remaining causal channel quads:
			// This reads back the results that were acquired during the last
			// triggered measurement, which was just done above.
			for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max;
			     ++i) {
				auto quad_column = SynapseQuadColumnOnDLS(i);
				hate::Empty<CADCSampleQuadOnDLS> quad_coord;
				CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.causal[syn_on_row] = quad_config.get_sample(syn);
				}
			}
			// buffered read of acausal channel quads
			for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
				hate::Empty<CADCSampleQuadOnDLS> quad_coord;
				CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.acausal[syn_on_row] = quad_config.get_sample(syn);
				}
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::CHIP_REVISION_STR::CADCSampleRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCSampleRow const>::value) {
			// trigger ADC sampling by reading one quad of causal channels
			// The values are broken and discarded, see Issue #3637
			CADCSampleQuad quad_config_trigger;
			CADCSampleQuadOnDLS quad_coord_trigger(
			    CADCSampleQuadOnSynram(
			        SynapseQuadOnSynram(
			            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min),
			            coord.toSynapseRowOnSynram()),
			        CADCChannelType::causal, CADCReadoutType::trigger_read),
			    coord.toSynramOnDLS());
			visit_preorder(quad_config_trigger, quad_coord_trigger, visitor);

			// Use results of triggered read: write into result array
			for (auto const entry : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow syn_on_row(entry, SynapseQuadColumnOnDLS(0));
				config.causal[syn_on_row] = quad_config_trigger.get_sample(entry);
			}
			// buffered read of remaining causal channel quads:
			// This reads back the results that were acquired during the last
			// triggered measurement, which was just done above.
			for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max;
			     ++i) {
				auto quad_column = SynapseQuadColumnOnDLS(i);
				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::causal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.causal[syn_on_row] = quad_config.get_sample(syn);
				}
			}
			// buffered read of acausal channel quads
			for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::acausal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.acausal[syn_on_row] = quad_config.get_sample(syn);
				}
			}
		}
	}
};

template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::CADCSamples>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    lola::vx::CHIP_REVISION_STR::CADCSamples::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		auto const trigger = [&](auto const& loc) {
			hate::Empty<CADCSampleQuad> quad_config_trigger;
			CADCSampleQuadOnDLS quad_coord_trigger_top(
			    CADCSampleQuadOnSynram(
			        SynapseQuadOnSynram(
			            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min), SynapseRowOnSynram()),
			        CADCChannelType::causal, CADCReadoutType::trigger_read),
			    loc);
			visit_preorder(quad_config_trigger, quad_coord_trigger_top, visitor);
		};

		// trigger ADC sampling in top synram by reading one quad of causal channels
		// immediately trigger ADC sampling in bottom synram as well
		trigger(SynramOnDLS::top);
		trigger(SynramOnDLS::bottom);

		auto const buffered = [&](auto const& type, auto const& loc) {
			for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
				// Skip causal quad 0 (handled above as triggered read)
				if (type == CADCChannelType::causal && quad_column == SynapseQuadColumnOnDLS::min)
					continue;

				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, SynapseRowOnSynram()), type,
				        CADCReadoutType::buffered),
				    loc);
				hate::Empty<CADCSampleQuad> quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
			}
		};

		// buffered reads of remaining quads
		buffered(CADCChannelType::causal, SynramOnDLS::top);
		buffered(CADCChannelType::acausal, SynramOnDLS::top);
		buffered(CADCChannelType::causal, SynramOnDLS::bottom);
		buffered(CADCChannelType::acausal, SynramOnDLS::bottom);
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::CHIP_REVISION_STR::CADCSamples::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		auto const trigger = [&](auto& values, auto const& loc) {
			CADCSampleQuad quad_config_trigger;
			hate::Empty<CADCSampleQuadOnDLS> quad_coord_trigger_top;
			visit_preorder(quad_config_trigger, quad_coord_trigger_top, visitor);

			if constexpr (!std::is_same_v<
			                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCSamples const>) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(
					    syn, SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min));
					values[loc][syn_on_row] = quad_config_trigger.get_sample(syn);
				}
			}
		};

		// trigger ADC sampling in top synram by reading one quad of causal channels
		// immediately trigger ADC sampling in bottom synram as well
		trigger(config.causal, SynramOnDLS::top);
		trigger(config.causal, SynramOnDLS::bottom);

		auto const buffered = [&](auto& values, auto const& type, auto const& loc) {
			for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
				// Skip causal quad 0 (handled above as triggered read)
				if (type == CADCChannelType::causal && quad_column == SynapseQuadColumnOnDLS::min)
					continue;

				hate::Empty<CADCSampleQuadOnDLS> quad_coord;
				CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				if constexpr (!std::is_same_v<
				                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCSamples const>) {
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow syn_on_row(syn, quad_column);
						values[loc][syn_on_row] = quad_config.get_sample(syn);
					}
				}
			}
		};

		// buffered reads of remaining quads
		buffered(config.causal, CADCChannelType::causal, SynramOnDLS::top);
		buffered(config.acausal, CADCChannelType::acausal, SynramOnDLS::top);
		buffered(config.causal, CADCChannelType::causal, SynramOnDLS::bottom);
		buffered(config.acausal, CADCChannelType::acausal, SynramOnDLS::bottom);
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::CHIP_REVISION_STR::CADCSamples::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<
		                  ContainerT, lola::vx::CHIP_REVISION_STR::CADCSamples const>::value) {
			auto const trigger = [&](auto& values, auto const& loc) {
				CADCSampleQuad quad_config_trigger;
				CADCSampleQuadOnDLS quad_coord_trigger_top(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(
				            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min),
				            SynapseRowOnSynram()),
				        CADCChannelType::causal, CADCReadoutType::trigger_read),
				    loc);
				visit_preorder(quad_config_trigger, quad_coord_trigger_top, visitor);

				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(
					    syn, SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min));
					values[loc][syn_on_row] = quad_config_trigger.get_sample(syn);
				}
			};

			// trigger ADC sampling in top synram by reading one quad of causal channels
			// immediately trigger ADC sampling in bottom synram as well
			trigger(config.causal, SynramOnDLS::top);
			trigger(config.causal, SynramOnDLS::bottom);

			auto const buffered = [&](auto& values, auto const& type, auto const& loc) {
				for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
					// Skip causal quad 0 (handled above as triggered read)
					if (type == CADCChannelType::causal &&
					    quad_column == SynapseQuadColumnOnDLS::min)
						continue;

					CADCSampleQuadOnDLS quad_coord(
					    CADCSampleQuadOnSynram(
					        SynapseQuadOnSynram(quad_column, SynapseRowOnSynram()), type,
					        CADCReadoutType::buffered),
					    loc);
					CADCSampleQuad quad_config;
					visit_preorder(quad_config, quad_coord, visitor);
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow syn_on_row(syn, quad_column);
						values[loc][syn_on_row] = quad_config.get_sample(syn);
					}
				}
			};

			// buffered reads of remaining quads
			buffered(config.causal, CADCChannelType::causal, SynramOnDLS::top);
			buffered(config.acausal, CADCChannelType::acausal, SynramOnDLS::top);
			buffered(config.causal, CADCChannelType::causal, SynramOnDLS::bottom);
			buffered(config.acausal, CADCChannelType::acausal, SynramOnDLS::bottom);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::CADCReadoutChain,
    ramp,
    channels_causal,
    channels_acausal,
    correlation);
BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::CADCReadoutChain::Channel,
    offset,
    enable_connect_correlation,
    enable_connect_debug,
    enable_connect_neuron);
BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::CADCReadoutChain::Ramp,
    enable,
    reset_wait,
    dead_time,
    v_offset_assignment,
    v_offset,
    i_slope,
    i_bias_comparator,
    i_bias_vreset_buffer,
    v_bias_buffer);
BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::CADCReadoutChain::Correlation,
    sense_delay,
    reset_duration,
    reset_fall_time,
    reset_mode,
    i_bias_ramp,
    i_bias_store,
    i_bias_corout);
BOOST_HANA_ADAPT_STRUCT(lola::vx::CHIP_REVISION_STR::CADCSampleRow, causal, acausal);
BOOST_HANA_ADAPT_STRUCT(lola::vx::CHIP_REVISION_STR::CADCSamples, causal, acausal);
