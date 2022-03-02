#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/capmem.h"
#include "halco/hicann-dls/vx/synapse.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

#ifndef __ppu__
#include "hxcomm/vx/target.h"
#endif

namespace fisch::vx {
class Omnibus;
class OmnibusChipOverJTAG;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Value type of a single synapse label configuration.
 */
struct GENPYBIND(inline_base("*")) SynapseLabelValue
    : public halco::common::detail::RantWrapper<SynapseLabelValue, uint8_t, 63, 0>
{
	constexpr explicit SynapseLabelValue(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
	    rant_t(val)
	{}
};

class GENPYBIND(visible) CommonSynramConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::CommonSynramConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Precharge configuration.
	 * A higher value results in a shorter time.
	 * See: Hock, Matthias. (2014). Modern Semiconductor Technologies for Neuromorphic Hardware,
	 * p.26ff, p.156f, http://doi.org/10.11588/heidok.00017129.
	 */
	struct GENPYBIND(inline_base("*")) PCConf
	    : public halco::common::detail::RantWrapper<PCConf, uint_fast16_t, 15, 0>
	{
		constexpr explicit PCConf(uintmax_t const val = 8) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Wordline activation delay.
	 * A higher value results in a shorter delay.
	 * See: Hock, Matthias. (2014). Modern Semiconductor Technologies for Neuromorphic Hardware,
	 * p.26ff, p.156f, http://doi.org/10.11588/heidok.00017129.
	 */
	struct GENPYBIND(inline_base("*")) WConf
	    : public halco::common::detail::RantWrapper<WConf, uint_fast32_t, 255, 0>
	{
		constexpr explicit WConf(uintmax_t const val = 3) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) WaitCtrClear
	    : public halco::common::detail::RantWrapper<WaitCtrClear, uint_fast16_t, 7, 0>
	{
		constexpr explicit WaitCtrClear(uintmax_t const val = 4) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	CommonSynramConfig() SYMBOL_VISIBLE;

	GENPYBIND(getter_for(pc_conf_west))
	PCConf get_pc_conf_west() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(pc_conf_west))
	void set_pc_conf_west(PCConf const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(pc_conf_east))
	PCConf get_pc_conf_east() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(pc_conf_east))
	void set_pc_conf_east(PCConf const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(w_conf_west))
	WConf get_w_conf_west() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(w_conf_west))
	void set_w_conf_west(WConf const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(w_conf_east))
	WConf get_w_conf_east() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(w_conf_east))
	void set_w_conf_east(WConf const& value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(wait_ctr_clear))
	WaitCtrClear get_wait_ctr_clear() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(wait_ctr_clear))
	void set_wait_ctr_clear(WaitCtrClear const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	bool operator==(CommonSynramConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonSynramConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CommonSynramConfig const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	PCConf m_pc_conf_west;
	PCConf m_pc_conf_east;
	WConf m_w_conf_west;
	WConf m_w_conf_east;
	WaitCtrClear m_wait_ctr_clear;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CommonSynramConfig)

class GENPYBIND(visible) SynapseBiasSelection
{
public:
	typedef halco::hicann_dls::vx::SynapseBiasSelectionOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware, hxcomm::vx::Target::simulation};
#endif

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::CapMemBlockOnDLS>
	    bias_selection_type GENPYBIND(opaque, expose_as(_bias_selection_type));

	/** Construct Synapse Bias Selection container with all internal biases enabled. */
	explicit SynapseBiasSelection() SYMBOL_VISIBLE;

	/** Set enable for internal synapse DAC bias current. */
	GENPYBIND(getter_for(enable_internal_dac_bias), return_value_policy(reference_internal))
	bias_selection_type const& get_enable_internal_dac_bias() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_internal_dac_bias))
	void set_enable_internal_dac_bias(bias_selection_type const& value) SYMBOL_VISIBLE;

	/** Set enable for internal synapse correlation ramp bias current. */
	GENPYBIND(getter_for(enable_internal_ramp_bias), return_value_policy(reference_internal))
	bias_selection_type const& get_enable_internal_ramp_bias() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_internal_ramp_bias))
	void set_enable_internal_ramp_bias(bias_selection_type const& value) SYMBOL_VISIBLE;

	/** Set enable for internal synapse correlation store bias current. */
	GENPYBIND(getter_for(enable_internal_store_bias), return_value_policy(reference_internal))
	bias_selection_type const& get_enable_internal_store_bias() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_internal_store_bias))
	void set_enable_internal_store_bias(bias_selection_type const& value) SYMBOL_VISIBLE;

	/** Set enable for internal synapse correlation output bias current. */
	GENPYBIND(getter_for(enable_internal_output_bias), return_value_policy(reference_internal))
	bias_selection_type const& get_enable_internal_output_bias() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_internal_output_bias))
	void set_enable_internal_output_bias(bias_selection_type const& value) SYMBOL_VISIBLE;

	bool operator==(SynapseBiasSelection const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseBiasSelection const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseBiasSelection const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;

	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);

	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& words) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	bias_selection_type m_int_dac_bias;
	bias_selection_type m_int_ramp_bias;
	bias_selection_type m_int_store_bias;
	bias_selection_type m_int_output_bias;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(SynapseBiasSelection)

namespace detail {

template <>
struct BackendContainerTrait<CommonSynramConfig>
    : public BackendContainerBase<
          CommonSynramConfig,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<SynapseBiasSelection>
    : public BackendContainerBase<
          SynapseBiasSelection,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::CommonSynramConfig::PCConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::SynapseLabelValue)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::CommonSynramConfig::WConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::CommonSynramConfig::WaitCtrClear)

} // namespace std
