#pragma once
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/common/typed_heap_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "haldls/vx/v2/cadc.h"
#include "haldls/vx/v2/capmem.h"
#include "haldls/vx/v2/correlation.h"
#include "haldls/vx/v2/synapse.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"
#include "lola/vx/v2/synapse.h"
#include <boost/hana/adapt_struct.hpp>

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include <pybind11/numpy.h>
#endif

namespace lola::vx::v2 GENPYBIND_TAG_LOLA_VX_V2 {

/**
 * Configuration container for CADC and correlation/neuron-readout related settings.
 */
class GENPYBIND(visible) CADCReadoutChain
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v2::CADCOnDLS coordinate_type;

	/**
	 * Configuration of each CADC channel. Contains individual calibration and connection settings.
	 */
	class GENPYBIND(visible) Channel
	{
	public:
		typedef haldls::vx::v2::CADCChannelConfig::Offset Offset GENPYBIND(visible);

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
	typedef halco::common::
	    typed_array<Channel, halco::hicann_dls::vx::v2::CADCChannelColumnOnSynram>
	        ChannelArray GENPYBIND(opaque);

	typedef halco::common::typed_array<
	    haldls::vx::v2::CapMemCell::value_type,
	    halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere>
	    AnalogValues;

	/**
	 * CADC ramp generation settings.
	 */
	class GENPYBIND(visible) Ramp
	{
	public:
		typedef haldls::vx::v2::CADCConfig::ResetWait ResetWait GENPYBIND(visible);
		typedef haldls::vx::v2::CADCConfig::DeadTime DeadTime GENPYBIND(visible);
		typedef haldls::vx::v2::CapMemCell::Value AnalogValue GENPYBIND(visible);
		typedef haldls::vx::v2::CapMemCell::DisableRefresh AnalogDisableRefresh GENPYBIND(visible);

		typedef haldls::vx::v2::CommonCorrelationConfig::RampOffsets OffsetAssignment
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
		    halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere(0),
		    halco::hicann_dls::vx::v2::CapMemBlockOnHemisphere(1)};
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
		typedef haldls::vx::v2::CommonCorrelationConfig::SenseDelay SenseDelay GENPYBIND(visible);
		typedef haldls::vx::v2::CommonCorrelationConfig::ResetDuration ResetDuration
		    GENPYBIND(visible);
		typedef haldls::vx::v2::CommonCorrelationConfig::ResetFallTime ResetFallTime
		    GENPYBIND(visible);
		typedef haldls::vx::v2::CommonCorrelationConfig::ResetMode ResetMode GENPYBIND(visible);
		typedef haldls::vx::v2::CapMemCell::Value AnalogValue GENPYBIND(visible);
		typedef haldls::vx::v2::CapMemCell::DisableRefresh AnalogDisableRefresh GENPYBIND(visible);

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
		auto av = parent->py::template class_<::lola::vx::v2::CADCReadoutChain::AnalogValues>(
		    parent, "AnalogValues");
		av.def(
		    "fill", &::lola::vx::v2::CADCReadoutChain::AnalogValues::fill, "",
		    parent->py::arg("val"));
		{
			typedef ::lola::vx::v2::CADCReadoutChain::AnalogValues::reference (
			    ::lola::vx::v2::CADCReadoutChain::AnalogValues::*genpybind_at_type)(
			    const ::lola::vx::v2::CADCReadoutChain::AnalogValues::key_type&);
			av.def(
			    "at", (genpybind_at_type) & ::lola::vx::v2::CADCReadoutChain::AnalogValues::at, "",
			    parent->py::arg("key"));
		}
		{
			typedef ::lola::vx::v2::CADCReadoutChain::AnalogValues::const_reference (
			    ::lola::vx::v2::CADCReadoutChain::AnalogValues::*genpybind_at_type)(
			    const ::lola::vx::v2::CADCReadoutChain::AnalogValues::key_type&) const;
			av.def(
			    "at", (genpybind_at_type) & ::lola::vx::v2::CADCReadoutChain::AnalogValues::at, "",
			    parent->py::arg("key"));
		}
		{
			typedef ::lola::vx::v2::CADCReadoutChain::AnalogValues::reference (
			    ::lola::vx::v2::CADCReadoutChain::AnalogValues::*genpybind_front_type)();
			av.def(
			    "front",
			    (genpybind_front_type) & ::lola::vx::v2::CADCReadoutChain::AnalogValues::front, "");
		}
		{
			typedef ::lola::vx::v2::CADCReadoutChain::AnalogValues::const_reference (
			    ::lola::vx::v2::CADCReadoutChain::AnalogValues::*genpybind_front_type)() const;
			av.def(
			    "front",
			    (genpybind_front_type) & ::lola::vx::v2::CADCReadoutChain::AnalogValues::front, "");
		}
		{
			typedef ::lola::vx::v2::CADCReadoutChain::AnalogValues::reference (
			    ::lola::vx::v2::CADCReadoutChain::AnalogValues::*genpybind_back_type)();
			av.def(
			    "back",
			    (genpybind_back_type) & ::lola::vx::v2::CADCReadoutChain::AnalogValues::back, "");
		}
		{
			typedef ::lola::vx::v2::CADCReadoutChain::AnalogValues::const_reference (
			    ::lola::vx::v2::CADCReadoutChain::AnalogValues::*genpybind_back_type)() const;
			av.def(
			    "back",
			    (genpybind_back_type) & ::lola::vx::v2::CADCReadoutChain::AnalogValues::back, "");
		}
		av.def(
		    "__getitem__", &::lola::vx::v2::CADCReadoutChain::AnalogValues::get, "",
		    parent->py::arg("key"), parent->py::return_value_policy::reference);
		av.def(
		    "__setitem__", &::lola::vx::v2::CADCReadoutChain::AnalogValues::set, "",
		    parent->py::arg("key"), parent->py::arg("value"));
		av.def(
		    "__iter__",
		    [av](::lola::vx::v2::CADCReadoutChain::AnalogValues& self) {
			    return pybind11::make_iterator(self);
		    },
		    parent->py::template keep_alive<0, 1>());

		av.def("to_numpy", [](::lola::vx::v2::CADCReadoutChain::AnalogValues const& self) {
			return ::halco::common::detail::to_numpy(self);
		});
		av.def(
		    "from_numpy",
		    [](::lola::vx::v2::CADCReadoutChain::AnalogValues& self, pybind11::array const& array) {
			    ::halco::common::detail::from_numpy(self, array);
		    });
		av.def(
		    parent->py::template init<const ::lola::vx::v2::CADCReadoutChain::AnalogValues&>(), "");
		av.def(parent->py::template init<>(), "");
		av.def_property_readonly(
		    "size",
		    parent->py::cpp_function(&::lola::vx::v2::CADCReadoutChain::AnalogValues::size));
		av.def_property_readonly(
		    "max_size",
		    parent->py::cpp_function(&::lola::vx::v2::CADCReadoutChain::AnalogValues::max_size));
		av.def_property_readonly(
		    "empty",
		    parent->py::cpp_function(&::lola::vx::v2::CADCReadoutChain::AnalogValues::empty));
	})
private:
	friend class haldls::vx::detail::VisitPreorderImpl<CADCReadoutChain>;
};


class GENPYBIND(visible) CADCSampleRow
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v2::CADCSampleRowOnDLS coordinate_type;

	typedef haldls::vx::v2::CADCSampleQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
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
	friend class haldls::vx::detail::VisitPreorderImpl<CADCSampleRow>;
};


class GENPYBIND(visible) CADCSamples
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v2::CADCSamplesOnDLS coordinate_type;

	typedef haldls::vx::v2::CADCSampleQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<
	    halco::common::typed_array<Value, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::v2::SynramOnDLS>
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
	friend class haldls::vx::detail::VisitPreorderImpl<CADCSamples>;
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v2::CADCSampleRow>
    : public BackendContainerBase<lola::vx::v2::CADCSampleRow, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct BackendContainerTrait<lola::vx::v2::CADCSamples>
    : public BackendContainerBase<lola::vx::v2::CADCSamples, fisch::vx::word_access_type::Omnibus>
{};

template <>
struct BackendContainerTrait<lola::vx::v2::CADCReadoutChain>
    : public BackendContainerBase<
          lola::vx::v2::CADCReadoutChain,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::CADCReadoutChain>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v2::CADCReadoutChain::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		haldls::vx::v2::CADCConfig cadc_config;
		cadc_config.set_enable(config.ramp.enable);
		cadc_config.set_reset_wait(config.ramp.reset_wait);
		cadc_config.set_dead_time(config.ramp.dead_time);
		auto const cadc_coord = coord.toCADCConfigOnDLS();
		visit_preorder(cadc_config, cadc_coord, visitor);

		if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCReadoutChain const>::value) {
			config.ramp.enable = cadc_config.get_enable();
			config.ramp.reset_wait = cadc_config.get_reset_wait();
			config.ramp.dead_time = cadc_config.get_dead_time();
		}

		for (auto const block : iter_all<CapMemBlockOnHemisphere>()) {
			haldls::vx::v2::CapMemCell cell_v_offset(config.ramp.v_offset[block]);
			haldls::vx::v2::CapMemCell cell_i_slope(config.ramp.i_slope[block]);
			haldls::vx::v2::CapMemCell cell_i_bias_comp(config.ramp.i_bias_comparator[block]);
			haldls::vx::v2::CapMemCell cell_i_bias_vreset_buf(
			    config.ramp.i_bias_vreset_buffer[block]);
			haldls::vx::v2::CapMemCell cell_v_bias_buf(config.ramp.v_bias_buffer[block]);
			haldls::vx::v2::CapMemCell cell_i_bias_ramp(config.correlation.i_bias_ramp[block]);
			haldls::vx::v2::CapMemCell cell_i_bias_store(config.correlation.i_bias_store[block]);
			haldls::vx::v2::CapMemCell cell_i_bias_corout(config.correlation.i_bias_corout[block]);

			CapMemBlockOnDLS capmem_block(
			    block.toEnum() + CapMemBlockOnHemisphere::size * coord.toEnum());
			CapMemCellOnDLS cell_v_offset_coord(
			    CapMemCellOnCapMemBlock::cadc_v_ramp_offset, capmem_block);
			CapMemCellOnDLS cell_i_slope_coord(
			    CapMemCellOnCapMemBlock::cadc_i_ramp_slope, capmem_block);
			CapMemCellOnDLS cell_i_bias_comp_coord(
			    CapMemCellOnCapMemBlock::cadc_i_bias_comp, capmem_block);
			CapMemCellOnDLS cell_i_bias_vreset_buf_coord(
			    CapMemCellOnCapMemBlock::cadc_i_bias_vreset_buf, capmem_block);
			CapMemCellOnDLS cell_v_bias_buf_coord(
			    CapMemCellOnCapMemBlock::cadc_v_bias_ramp_buf, capmem_block);
			CapMemCellOnDLS cell_i_bias_ramp_coord(
			    CapMemCellOnCapMemBlock::syn_i_bias_ramp, capmem_block);
			CapMemCellOnDLS cell_i_bias_store_coord(
			    CapMemCellOnCapMemBlock::syn_i_bias_store, capmem_block);
			CapMemCellOnDLS cell_i_bias_corout_coord(
			    CapMemCellOnCapMemBlock::syn_i_bias_corout, capmem_block);
			visit_preorder(cell_v_offset, cell_v_offset_coord, visitor);
			visit_preorder(cell_i_slope, cell_i_slope_coord, visitor);
			visit_preorder(cell_i_bias_comp, cell_i_bias_comp_coord, visitor);
			visit_preorder(cell_i_bias_vreset_buf, cell_i_bias_vreset_buf_coord, visitor);
			visit_preorder(cell_v_bias_buf, cell_v_bias_buf_coord, visitor);
			visit_preorder(cell_i_bias_ramp, cell_i_bias_ramp_coord, visitor);
			visit_preorder(cell_i_bias_store, cell_i_bias_store_coord, visitor);
			visit_preorder(cell_i_bias_corout, cell_i_bias_corout_coord, visitor);

			if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCReadoutChain const>::value) {
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
			haldls::vx::v2::CADCChannelConfig channel_config;
			channel_config.set_offset(config.channels_causal[column].offset);

			CADCChannelConfigOnDLS channel_coord(
			    CADCChannelConfigOnSynram(column, CADCChannelType::causal), coord.toSynramOnDLS());
			visit_preorder(channel_config, channel_coord, visitor);

			if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCReadoutChain const>::value) {
				config.channels_causal[column].offset = channel_config.get_offset();
			}
		}

		for (auto const column : iter_all<CADCChannelColumnOnSynram>()) {
			haldls::vx::v2::CADCChannelConfig channel_config;
			channel_config.set_offset(config.channels_acausal[column].offset);

			CADCChannelConfigOnDLS channel_coord(
			    CADCChannelConfigOnSynram(column, CADCChannelType::acausal), coord.toSynramOnDLS());
			visit_preorder(channel_config, channel_coord, visitor);

			if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCReadoutChain const>::value) {
				config.channels_acausal[column].offset = channel_config.get_offset();
			}
		}

		lola::vx::v2::ColumnCorrelationRow column_correlation_row;
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

		if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCReadoutChain const>::value) {
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

		haldls::vx::v2::CommonCorrelationConfig correlation_config;
		correlation_config.set_sense_delay(config.correlation.sense_delay);
		correlation_config.set_reset_duration(config.correlation.reset_duration);
		correlation_config.set_reset_fall_time(config.correlation.reset_fall_time);
		correlation_config.set_reset_mode(config.correlation.reset_mode);
		correlation_config.set_cadc_v_offset_assignment(config.ramp.v_offset_assignment);
		auto const correlation_coord = coord.toCommonCorrelationConfigOnDLS();
		visit_preorder(correlation_config, correlation_coord, visitor);

		if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCReadoutChain const>::value) {
			config.correlation.sense_delay = correlation_config.get_sense_delay();
			config.correlation.reset_duration = correlation_config.get_reset_duration();
			config.correlation.reset_fall_time = correlation_config.get_reset_fall_time();
			config.correlation.reset_mode = correlation_config.get_reset_mode();
			config.ramp.v_offset_assignment = correlation_config.get_cadc_v_offset_assignment();
		}
	}
};

template <>
struct VisitPreorderImpl<lola::vx::v2::CADCSampleRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v2::CADCSampleRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCSampleRow const>::value) {
			// trigger ADC sampling by reading one quad of causal channels
			v2::CADCSampleQuad quad_config_trigger;
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
				v2::CADCSampleQuad quad_config;
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
				v2::CADCSampleQuad quad_config;
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
struct VisitPreorderImpl<lola::vx::v2::CADCSamples>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v2::CADCSamples::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<ContainerT, lola::vx::v2::CADCSamples const>::value) {
			auto const trigger = [&](auto& values, auto const& loc) {
				v2::CADCSampleQuad quad_config_trigger;
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
					v2::CADCSampleQuad quad_config;
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
    lola::vx::v2::CADCReadoutChain, ramp, channels_causal, channels_acausal, correlation);
BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::CADCReadoutChain::Channel,
    offset,
    enable_connect_correlation,
    enable_connect_debug,
    enable_connect_neuron);
BOOST_HANA_ADAPT_STRUCT(
    lola::vx::v2::CADCReadoutChain::Ramp,
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
    lola::vx::v2::CADCReadoutChain::Correlation,
    sense_delay,
    reset_duration,
    reset_fall_time,
    reset_mode,
    i_bias_ramp,
    i_bias_store,
    i_bias_corout);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::CADCSampleRow, causal, acausal);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::CADCSamples, causal, acausal);
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::CADCReadoutChain)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::CADCSampleRow)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v2::CADCSamples)
