#pragma once

#include <ostream>

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/genpybind.h"

#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {

class GENPYBIND(visible) CommonSynramConfig
{
public:
	typedef halco::hicann_dls::v2::CommonSynramConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Precharge configuration.
	 * A higher value results in a shorter time.
	 * See: Hock, Matthias. (2014). Modern Semiconductor Technologies for Neuromorphic Hardware,
	 * p.26ff, p.156f, http://doi.org/10.11588/heidok.00017129.
	 */
	struct GENPYBIND(inline_base("*")) PCConf : public halco::common::detail::RantWrapper<PCConf, uint_fast16_t, 15, 0>
	{
		constexpr explicit PCConf(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
	};
	/**
	 * Wordline activation delay.
	 * A higher value results in a shorter delay.
	 * See: Hock, Matthias. (2014). Modern Semiconductor Technologies for Neuromorphic Hardware,
	 * p.26ff, p.156f, http://doi.org/10.11588/heidok.00017129.
	 */
	struct GENPYBIND(inline_base("*")) WConf : public halco::common::detail::RantWrapper<WConf, uint_fast32_t, 255, 0>
	{
		constexpr explicit WConf(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
	};
	struct GENPYBIND(inline_base("*")) WaitCtrClear
		: public halco::common::detail::RantWrapper<WaitCtrClear, uint_fast16_t, 7, 0>
	{
		constexpr explicit WaitCtrClear(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
	};

	CommonSynramConfig() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(pc_conf))
	PCConf get_pc_conf() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(pc_conf))
	void set_pc_conf(PCConf const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(w_conf))
	WConf get_w_conf() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(w_conf))
	void set_w_conf(WConf const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(wait_ctr_clear))
	WaitCtrClear get_wait_ctr_clear() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wait_ctr_clear))
	void set_wait_ctr_clear(WaitCtrClear const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(use_internal_i_bias_correlation_output))
	bool get_use_internal_i_bias_correlation_output() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(use_internal_i_bias_correlation_output))
	void set_use_internal_i_bias_correlation_output(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(use_internal_i_bias_vstore))
	bool get_use_internal_i_bias_vstore() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(use_internal_i_bias_vstore))
	void set_use_internal_i_bias_vstore(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(use_internal_i_bias_vramp))
	bool get_use_internal_i_bias_vramp() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(use_internal_i_bias_vramp))
	void set_use_internal_i_bias_vramp(bool const value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(use_internal_i_bias_vdac))
	bool get_use_internal_i_bias_vdac() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(use_internal_i_bias_vdac))
	void set_use_internal_i_bias_vdac(bool const value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 4;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& unique) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

	bool operator==(CommonSynramConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonSynramConfig const& other) const SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	PCConf m_pc_conf;
	WConf m_w_conf;
	WaitCtrClear m_wait_ctr_clear;
	bool m_use_internal_i_bias_correlation_output;
	bool m_use_internal_i_bias_vstore;
	bool m_use_internal_i_bias_vramp;
	bool m_use_internal_i_bias_vdac;
};


class GENPYBIND(visible) SynapseBlock
{
public:
	typedef halco::hicann_dls::v2::SynapseBlockOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/// \brief Single synapse containing a 6 bit weight and address.
	/// For each synapse, individual calibration bits can control the STDP time constant
	/// and amplitude and may be used to compensate the synapse-to-synapse variations of
	/// the correlation measurements.
	/// \note The calibration bits change both the causal and anticausal branch at
	///       once. In other words: causal and anticausal branches cannot be controlled
	///       individually.
	/// \see <a href="http://www.kip.uni-heidelberg.de/Veroeffentlichungen/details.php?id=3380">
	///      Bachelor's Thesis by Timo Wunderlich</a> for measurements.
	struct Synapse
	{
		struct GENPYBIND(inline_base("*")) Weight : public halco::common::detail::RantWrapper<Weight, uint_fast16_t, 63, 0>
		{
			constexpr explicit Weight(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
		};

		struct GENPYBIND(inline_base("*")) Address : public halco::common::detail::RantWrapper<Address, uint_fast16_t, 63, 0>
		{
			constexpr explicit Address(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
		};

		// The more bits set, the shorter the time constant
		struct GENPYBIND(inline_base("*")) TimeCalib : public halco::common::detail::RantWrapper<TimeCalib, uint_fast16_t, 3, 0>
		{
			constexpr explicit TimeCalib(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
		};

		// The more bits set, the lower the amplitude
		struct GENPYBIND(inline_base("*")) AmpCalib : public halco::common::detail::RantWrapper<AmpCalib, uint_fast16_t, 3, 0>
		{
			constexpr explicit AmpCalib(uintmax_t const val = 0) GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val) {}
		};

		Synapse() SYMBOL_VISIBLE;

		GENPYBIND(getter_for(weight))
		Weight get_weight() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(weight))
		void set_weight(Weight const& value) SYMBOL_VISIBLE;

		GENPYBIND(getter_for(address))
		Address get_address() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(address))
		void set_address(Address const& value) SYMBOL_VISIBLE;

		GENPYBIND(getter_for(time_calib))
		TimeCalib get_time_calib() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(time_calib))
		void set_time_calib(TimeCalib const& value) SYMBOL_VISIBLE;

		GENPYBIND(getter_for(amp_calib))
		AmpCalib get_amp_calib() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(amp_calib))
		void set_amp_calib(AmpCalib const& value) SYMBOL_VISIBLE;

		bool operator==(Synapse const& other) const SYMBOL_VISIBLE;
		bool operator!=(Synapse const& other) const SYMBOL_VISIBLE;

	private:
		friend class cereal::access;
		template <class Archive>
		void cerealize(Archive& ar) SYMBOL_VISIBLE;

		Weight m_weight;
		Address m_address;
		TimeCalib m_time_calib;
		AmpCalib m_amp_calib;
	};

	SynapseBlock() SYMBOL_VISIBLE;

	Synapse get_synapse(halco::hicann_dls::v2::SynapseOnSynapseBlock const& synapse) const
		SYMBOL_VISIBLE;
	void set_synapse(
		halco::hicann_dls::v2::SynapseOnSynapseBlock const& synapse,
		Synapse const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& block) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

	bool operator==(SynapseBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseBlock const& other) const SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<Synapse, halco::hicann_dls::v2::SynapseOnSynapseBlock> m_synapses;
};

class GENPYBIND(visible) ColumnCorrelationBlock
{
public:
	typedef halco::hicann_dls::v2::ColumnCorrelationBlockOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct ColumnCorrelationSwitch
	{
		enum class Config : uint_fast8_t
		{
			disabled = 0,
			external = 1,
			internal = 2,
			readout = 3
		};

		ColumnCorrelationSwitch() SYMBOL_VISIBLE;

		GENPYBIND(getter_for(causal_config))
		Config get_causal_config() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(causal_config))
		void set_causal_config(Config const value) SYMBOL_VISIBLE;

		GENPYBIND(getter_for(acausal_config))
		Config get_acausal_config() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(acausal_config))
		void set_acausal_config(Config const value) SYMBOL_VISIBLE;

		bool operator==(ColumnCorrelationSwitch const& other) const SYMBOL_VISIBLE;
		bool operator!=(ColumnCorrelationSwitch const& other) const SYMBOL_VISIBLE;

	private:
		friend class cereal::access;
		template <class Archive>
		void cerealize(Archive& ar) SYMBOL_VISIBLE;

		Config m_causal;
		Config m_acausal;
	};

	ColumnCorrelationBlock() SYMBOL_VISIBLE;

	ColumnCorrelationSwitch get_switch(
	    halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnCorrelationBlock const&
	        correlation_switch) const SYMBOL_VISIBLE;
	void set_switch(
	    halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnCorrelationBlock const&
	        correlation_switch,
	    ColumnCorrelationSwitch const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& block) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

	bool operator==(ColumnCorrelationBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(ColumnCorrelationBlock const& other) const SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<
	    ColumnCorrelationSwitch,
	    halco::hicann_dls::v2::ColumnCorrelationSwitchOnColumnCorrelationBlock>
	    m_switches;
};

class GENPYBIND(visible) ColumnCurrentBlock
{
public:
	typedef halco::hicann_dls::v2::ColumnCurrentBlockOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct ColumnCurrentSwitch
	{
		enum class Config : uint_fast8_t
		{
			disabled = 0,
			external = 1,
			internal = 2,
			readout = 3
		};

		ColumnCurrentSwitch() SYMBOL_VISIBLE;

		GENPYBIND(getter_for(exc_config))
		Config get_exc_config() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(exc_config))
		void set_exc_config(Config const value) SYMBOL_VISIBLE;

		GENPYBIND(getter_for(inh_config))
		Config get_inh_config() const SYMBOL_VISIBLE;
		GENPYBIND(setter_for(inh_config))
		void set_inh_config(Config const value) SYMBOL_VISIBLE;

		bool operator==(ColumnCurrentSwitch const& other) const SYMBOL_VISIBLE;
		bool operator!=(ColumnCurrentSwitch const& other) const SYMBOL_VISIBLE;

	private:
		friend class cereal::access;
		template <class Archive>
		void cerealize(Archive& ar) SYMBOL_VISIBLE;

		Config m_exc;
		Config m_inh;
	};

	ColumnCurrentBlock() SYMBOL_VISIBLE;

	ColumnCurrentSwitch get_switch(
	    halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnCurrentBlock const& current_switch) const
	    SYMBOL_VISIBLE;
	void set_switch(
	    halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnCurrentBlock const& current_switch,
	    ColumnCurrentSwitch const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	std::array<hardware_address_type, config_size_in_words> addresses(
		coordinate_type const& block) const SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<hardware_word_type, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	void decode(std::array<hardware_word_type, config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);

	bool operator==(ColumnCurrentBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(ColumnCurrentBlock const& other) const SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<
	    ColumnCurrentSwitch,
	    halco::hicann_dls::v2::ColumnCurrentSwitchOnColumnCurrentBlock>
	    m_switches;
};

} // namespace v2
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::CommonSynramConfig::PCConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::CommonSynramConfig::WConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::CommonSynramConfig::WaitCtrClear)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SynapseBlock::Synapse::Weight)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SynapseBlock::Synapse::Address)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SynapseBlock::Synapse::TimeCalib)
HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SynapseBlock::Synapse::AmpCalib)

} // namespace std
