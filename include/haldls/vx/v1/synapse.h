#pragma once
#include "haldls/vx/genpybind.h"
#include "haldls/vx/synapse.h"

#include <array>
#include <ostream>

#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/v1/switch_rows.h"
#include "halco/hicann-dls/vx/v1/synapse.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class Omnibus;
class OmnibusChipOverJTAG;
} // namespace fisch::vx

namespace lola::vx::v1 {
class SynapseMatrix;
class SynapseWeightMatrix;
class SynapseLabelMatrix;
class SynapseCorrelationCalibMatrix;
class SynapseRow;
class SynapseWeightRow;
class SynapseLabelRow;
class SynapseCorrelationCalibRow;
} // namespace lola::vx::v1

namespace haldls::vx::v1 GENPYBIND_TAG_HALDLS_VX_V1 {

using CommonSynramConfig GENPYBIND(visible) = haldls::vx::CommonSynramConfig;
using SynapseBiasSelection GENPYBIND(visible) = haldls::vx::SynapseBiasSelection;

class GENPYBIND(visible) SynapseWeightQuad : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::v1::SynapseWeightQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uint8_t, 63, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	SynapseWeightQuad() SYMBOL_VISIBLE;

	typedef halco::common::typed_array<Value, halco::hicann_dls::vx::EntryOnQuad> values_type
	    GENPYBIND(opaque);

	GENPYBIND(getter_for(values), return_value_policy(reference))
	values_type const& get_values() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(values))
	void set_values(values_type const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	bool operator==(SynapseWeightQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseWeightQuad const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseWeightQuad const& config)
	    SYMBOL_VISIBLE;

private:
	// used for direct member access without function calls
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::v1::SynapseWeightRow>;
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::v1::SynapseWeightMatrix>;
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t version);

	values_type m_values;
};


class GENPYBIND(visible) SynapseLabelQuad : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::v1::SynapseLabelQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	typedef SynapseLabelValue Value GENPYBIND(visible);

	SynapseLabelQuad() SYMBOL_VISIBLE;

	typedef halco::common::typed_array<Value, halco::hicann_dls::vx::EntryOnQuad> values_type
	    GENPYBIND(opaque);

	GENPYBIND(getter_for(values), return_value_policy(reference))
	values_type const& get_values() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(values))
	void set_values(values_type const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	bool operator==(SynapseLabelQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseLabelQuad const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseLabelQuad const& config)
	    SYMBOL_VISIBLE;

private:
	// used for direct member access without function calls
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::v1::SynapseLabelRow>;
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::v1::SynapseLabelMatrix>;
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t version);

	values_type m_values;
};


class GENPYBIND(visible) SynapseCorrelationCalibQuad : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::v1::SynapseCorrelationCalibQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	// The more bits set, the shorter the time constant
	struct GENPYBIND(inline_base("*")) TimeCalib
	    : public halco::common::detail::RantWrapper<TimeCalib, uint8_t, 3, 0>
	{
		constexpr explicit TimeCalib(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	// The more bits set, the lower the amplitude
	struct GENPYBIND(inline_base("*")) AmpCalib
	    : public halco::common::detail::RantWrapper<AmpCalib, uint8_t, 3, 0>
	{
		constexpr explicit AmpCalib(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	SynapseCorrelationCalibQuad() SYMBOL_VISIBLE;

	typedef halco::common::typed_array<TimeCalib, halco::hicann_dls::vx::EntryOnQuad>
	    time_calibs_type GENPYBIND(opaque);
	typedef halco::common::typed_array<AmpCalib, halco::hicann_dls::vx::EntryOnQuad> amp_calibs_type
	    GENPYBIND(opaque);

	GENPYBIND(getter_for(time_calibs), return_value_policy(reference))
	time_calibs_type const& get_time_calibs() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(time_calibs))
	void set_time_calibs(time_calibs_type const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(amp_calibs), return_value_policy(reference))
	amp_calibs_type const& get_amp_calibs() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(amp_calibs))
	void set_amp_calibs(amp_calibs_type const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	bool operator==(SynapseCorrelationCalibQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseCorrelationCalibQuad const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseCorrelationCalibQuad const& config)
	    SYMBOL_VISIBLE;

private:
	// used for direct member access without function calls
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::v1::SynapseCorrelationCalibRow>;
	friend struct haldls::vx::detail::VisitPreorderImpl<
	    lola::vx::v1::SynapseCorrelationCalibMatrix>;
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t version);

	time_calibs_type m_time_calibs;
	amp_calibs_type m_amp_calibs;
};


class GENPYBIND(visible) SynapseQuad : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::v1::SynapseQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	typedef SynapseWeightQuad::Value Weight GENPYBIND(visible);
	typedef SynapseLabelQuad::Value Label GENPYBIND(visible);
	typedef SynapseCorrelationCalibQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef SynapseCorrelationCalibQuad::AmpCalib AmpCalib GENPYBIND(visible);

	typedef SynapseWeightQuad::values_type weights_type GENPYBIND(visible);
	typedef SynapseLabelQuad::values_type labels_type GENPYBIND(visible);
	typedef SynapseCorrelationCalibQuad::time_calibs_type time_calibs_type GENPYBIND(visible);
	typedef SynapseCorrelationCalibQuad::amp_calibs_type amp_calibs_type GENPYBIND(visible);

	SynapseQuad() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(weights), return_value_policy(reference))
	weights_type const& get_weights() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(weights))
	void set_weights(weights_type const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(labels), return_value_policy(reference))
	labels_type const& get_labels() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(labels))
	void set_labels(labels_type const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(time_calibs), return_value_policy(reference))
	time_calibs_type const& get_time_calibs() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(time_calibs))
	void set_time_calibs(time_calibs_type const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(amp_calibs), return_value_policy(reference))
	amp_calibs_type const& get_amp_calibs() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(amp_calibs))
	void set_amp_calibs(amp_calibs_type const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	bool operator==(SynapseQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseQuad const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseQuad const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
	// used for direct member access without function calls
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::v1::SynapseMatrix>;
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::v1::SynapseRow>;

	weights_type m_weights;
	labels_type m_labels;
	time_calibs_type m_time_calibs;
	amp_calibs_type m_amp_calibs;
};


class GENPYBIND(visible) ColumnCorrelationQuad : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::v1::ColumnCorrelationQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct ColumnCorrelationSwitch
	{
		/** Default constructor. */
		ColumnCorrelationSwitch() SYMBOL_VISIBLE;

		/**
		 * Get enable value for internal causal line to CADC channel.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_internal_causal))
		bool get_enable_internal_causal() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for internal causal line to CADC channel.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_internal_causal))
		void set_enable_internal_causal(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for connecting acausal correlation to shared debug line.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_internal_acausal))
		bool get_enable_internal_acausal() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for connecting acausal correlation to shared debug line.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_internal_acausal))
		void set_enable_internal_acausal(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for connecting causal correlation to shared debug line.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_debug_causal))
		bool get_enable_debug_causal() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for connecting causal correlation to shared debug line.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_debug_causal))
		void set_enable_debug_causal(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for connecting acausal correlation to shared debug line.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_debug_acausal))
		bool get_enable_debug_acausal() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for connecting acausal correlation to shared debug line.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_debug_acausal))
		void set_enable_debug_acausal(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for connecting  the neuron's readout circuit to the respective causal
		 * CADC channel. Keep in mind that the neuron's readout amplifier must be enabled and
		 * biased, but the neuron must not be connected to the readout chain via the collective
		 * lines. Otherwise the neurons will be shorted.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_cadc_neuron_readout_causal))
		bool get_enable_cadc_neuron_readout_causal() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for connecting  the neuron's readout circuit to the respective causal
		 * CADC channel. Keep in mind that the neuron's readout amplifier must be enabled and
		 * biased, but the neuron must not be connected to the readout chain via the collective
		 * lines. Otherwise the neurons will be shorted.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_cadc_neuron_readout_causal))
		void set_enable_cadc_neuron_readout_causal(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for connecting  the neuron's readout circuit to the respective acausal
		 * CADC channel. Keep in mind that the neuron's readout amplifier must be enabled and
		 * biased, but the neuron must not be connected to the readout chain via the collective
		 * lines. Otherwise the neurons will be shorted.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_cadc_neuron_readout_acausal))
		bool get_enable_cadc_neuron_readout_acausal() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for connecting  the neuron's readout circuit to the respective acausal
		 * CADC channel. Keep in mind that the neuron's readout amplifier must be enabled and
		 * biased, but the neuron must not be connected to the readout chain via the collective
		 * lines. Otherwise the neurons will be shorted.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_cadc_neuron_readout_acausal))
		void set_enable_cadc_neuron_readout_acausal(bool value) SYMBOL_VISIBLE;

		bool operator==(ColumnCorrelationSwitch const& other) const SYMBOL_VISIBLE;
		bool operator!=(ColumnCorrelationSwitch const& other) const SYMBOL_VISIBLE;

	private:
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

		bool m_enable_internal_causal;
		bool m_enable_internal_acausal;
		bool m_enable_debug_causal;
		bool m_enable_debug_acausal;
		bool m_enable_cadc_neuron_readout_causal;
		bool m_enable_cadc_neuron_readout_acausal;
	};

	/** Default constructor. */
	ColumnCorrelationQuad() SYMBOL_VISIBLE;

	ColumnCorrelationSwitch get_switch(
	    halco::hicann_dls::vx::EntryOnQuad const& correlation_switch) const SYMBOL_VISIBLE;
	void set_switch(
	    halco::hicann_dls::vx::EntryOnQuad const& correlation_switch,
	    ColumnCorrelationSwitch const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 4;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	bool operator==(ColumnCorrelationQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(ColumnCorrelationQuad const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream&, ColumnCorrelationQuad const&) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	halco::common::typed_array<ColumnCorrelationSwitch, halco::hicann_dls::vx::EntryOnQuad>
	    m_switches;
};

class GENPYBIND(visible) ColumnCurrentQuad : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::v1::ColumnCurrentQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct ColumnCurrentSwitch
	{
		/** Default constructor. */
		ColumnCurrentSwitch() SYMBOL_VISIBLE;

		/**
		 * Get enable value for forwarding the excitatory synaptic currents of a whole column to the
		 * respective neuron's synaptic input.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_synaptic_current_excitatory))
		bool get_enable_synaptic_current_excitatory() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for forwarding the excitatory synaptic currents of a whole column to the
		 * respective neuron's synaptic input.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_synaptic_current_excitatory))
		void set_enable_synaptic_current_excitatory(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for forwarding the inhibitory synaptic currents of a whole column to the
		 * respective neuron's synaptic input.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_synaptic_current_inhibitory))
		bool get_enable_synaptic_current_inhibitory() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for forwarding the inhibitory synaptic currents of a whole column to the
		 * respective neuron's synaptic input.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_synaptic_current_inhibitory))
		void set_enable_synaptic_current_inhibitory(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for column-global excitatory synaptic current debug-line readout.
		 * In case the neuron's synaptic input circuit is disconnected,
		 * the current pulses originating from the synapse circuits can be read out.
		 * Note that current measuring equipment (i. e. source meter) is needed
		 * for this measurement.
		 * Also note that the readout line is shared amongst multiple synapses.
		 * Enabling the readout switch will short the synaptic columns, which is however
		 * non-destructive.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_debug_excitatory))
		bool get_enable_debug_excitatory() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for column-global excitatory synaptic current debug-line readout.
		 * In case the neuron's synaptic input circuit is disconnected,
		 * the current pulses originating from the synapse circuits can be read out.
		 * Note that current measuring equipment (i. e. source meter) is needed
		 * for this measurement.
		 * Also note that the readout line is shared amongst multiple synapses.
		 * Enabling the readout switch will short the synaptic columns, which is however
		 * non-destructive.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_debug_excitatory))
		void set_enable_debug_excitatory(bool value) SYMBOL_VISIBLE;

		/**
		 * Get enable value for column-global inhibitory synaptic current debug-line readout.
		 * In case the neuron's synaptic input circuit is disconnected,
		 * the current pulses originating from the synapse circuits can be read out.
		 * Note that current measuring equipment (i. e. source meter) is needed
		 * for this measurement.
		 * Also note that the readout line is shared amongst multiple synapses.
		 * Enabling the readout switch will short the synaptic columns, which is however
		 * non-destructive.
		 * @return Boolean enable value
		 */
		GENPYBIND(getter_for(enable_debug_inhibitory))
		bool get_enable_debug_inhibitory() const SYMBOL_VISIBLE;

		/**
		 * Set enable value for column-global inhibitory synaptic current debug-line readout.
		 * In case the neuron's synaptic input circuit is disconnected,
		 * the current pulses originating from the synapse circuits can be read out.
		 * Note that current measuring equipment (i. e. source meter) is needed
		 * for this measurement.
		 * Also note that the readout line is shared amongst multiple synapses.
		 * Enabling the readout switch will short the synaptic columns, which is however
		 * non-destructive.
		 * @param value Boolean enable value
		 */
		GENPYBIND(setter_for(enable_debug_inhibitory))
		void set_enable_debug_inhibitory(bool value) SYMBOL_VISIBLE;

		bool operator==(ColumnCurrentSwitch const& other) const SYMBOL_VISIBLE;
		bool operator!=(ColumnCurrentSwitch const& other) const SYMBOL_VISIBLE;

	private:
		friend class cereal::access;
		template <class Archive>
		void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

		bool m_enable_synaptic_current_excitatory;
		bool m_enable_synaptic_current_inhibitory;
		bool m_enable_debug_excitatory;
		bool m_enable_debug_inhibitory;
	};

	/** Default constructor. */
	ColumnCurrentQuad() SYMBOL_VISIBLE;

	ColumnCurrentSwitch get_switch(halco::hicann_dls::vx::EntryOnQuad const& current_switch) const
	    SYMBOL_VISIBLE;
	void set_switch(
	    halco::hicann_dls::vx::EntryOnQuad const& current_switch,
	    ColumnCurrentSwitch const& value) SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ColumnCurrentQuad const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block)
	    GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) GENPYBIND(hidden);

	bool operator==(ColumnCurrentQuad const& other) const SYMBOL_VISIBLE;
	bool operator!=(ColumnCurrentQuad const& other) const SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	halco::common::typed_array<ColumnCurrentSwitch, halco::hicann_dls::vx::EntryOnQuad> m_switches;
};


/**
 * Container to trigger reset of correlation measurements on a synapse quad.
 */
class GENPYBIND(visible) CorrelationReset
{
public:
	typedef halco::hicann_dls::vx::v1::CorrelationResetOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor */
	CorrelationReset() SYMBOL_VISIBLE;

	bool operator==(CorrelationReset const& other) const SYMBOL_VISIBLE;
	bool operator!=(CorrelationReset const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& neuron) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& neuron) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CorrelationReset const& config)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

} // namespace haldls::vx::v1

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<v1::SynapseWeightQuad>
    : public BackendContainerBase<
          v1::SynapseWeightQuad,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v1::SynapseLabelQuad>
    : public BackendContainerBase<
          v1::SynapseLabelQuad,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v1::SynapseCorrelationCalibQuad>
    : public BackendContainerBase<
          v1::SynapseCorrelationCalibQuad,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v1::SynapseQuad>
    : public BackendContainerBase<
          v1::SynapseQuad,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v1::ColumnCorrelationQuad>
    : public BackendContainerBase<
          v1::ColumnCorrelationQuad,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v1::ColumnCurrentQuad>
    : public BackendContainerBase<
          v1::ColumnCurrentQuad,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<v1::CorrelationReset>
    : public BackendContainerBase<
          v1::CorrelationReset,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace haldls::vx::detail

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v1::SynapseWeightQuad::Value)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v1::SynapseCorrelationCalibQuad::TimeCalib)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::v1::SynapseCorrelationCalibQuad::AmpCalib)

} // namespace std
