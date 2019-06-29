#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/coordinates.h"

#include "haldls/vx/common.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChip;
class OmnibusChipOverJTAG;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) CommonSynramConfig
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
		constexpr explicit PCConf(uintmax_t const val = 8)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
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
		constexpr explicit WConf(uintmax_t const val = 3)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) WaitCtrClear
	    : public halco::common::detail::RantWrapper<WaitCtrClear, uint_fast16_t, 7, 0>
	{
		constexpr explicit WaitCtrClear(uintmax_t const val = 4)
		    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
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
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord) const
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
	void serialize(Archive& ar);

	PCConf m_pc_conf_west;
	PCConf m_pc_conf_east;
	WConf m_w_conf_west;
	WConf m_w_conf_east;
	WaitCtrClear m_wait_ctr_clear;
};


class GENPYBIND(visible) SynapseQuad
{
public:
	typedef halco::hicann_dls::vx::SynapseQuadOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/**
	 * Single synapse containing a 6 bit weight and address.
	 * For each synapse, individual calibration bits can control the STDP time constant
	 * and amplitude and may be used to compensate the synapse-to-synapse variations of
	 * the correlation measurements.
	 * @note The calibration bits change both the causal and anticausal branch at
	 *       once. In other words: causal and anticausal branches cannot be controlled
	 *       individually.
	 * @see <a href="http://www.kip.uni-heidelberg.de/Veroeffentlichungen/details.php?id=3380">
	 *      Bachelor's Thesis by Timo Wunderlich</a> for measurements.
	 * TODO: Still valid since CADC changed?
	 */
	struct Synapse
	{
		struct GENPYBIND(inline_base("*")) Weight
		    : public halco::common::detail::RantWrapper<Weight, uint_fast16_t, 63, 0>
		{
			constexpr explicit Weight(uintmax_t const val = 0)
			    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
			{}
		};

		struct GENPYBIND(inline_base("*")) Address
		    : public halco::common::detail::RantWrapper<Address, uint_fast16_t, 63, 0>
		{
			constexpr explicit Address(uintmax_t const val = 0)
			    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
			{}
		};

		// The more bits set, the shorter the time constant
		struct GENPYBIND(inline_base("*")) TimeCalib
		    : public halco::common::detail::RantWrapper<TimeCalib, uint_fast16_t, 3, 0>
		{
			constexpr explicit TimeCalib(uintmax_t const val = 0)
			    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
			{}
		};

		// The more bits set, the lower the amplitude
		struct GENPYBIND(inline_base("*")) AmpCalib
		    : public halco::common::detail::RantWrapper<AmpCalib, uint_fast16_t, 3, 0>
		{
			constexpr explicit AmpCalib(uintmax_t const val = 0)
			    GENPYBIND(implicit_conversion) SYMBOL_VISIBLE : rant_t(val)
			{}
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
		void serialize(Archive& ar);

		Weight m_weight;
		Address m_address;
		TimeCalib m_time_calib;
		AmpCalib m_amp_calib;
	};

	SynapseQuad() SYMBOL_VISIBLE;

	Synapse get_synapse(halco::hicann_dls::vx::SynapseOnSynapseQuad const& synapse) const
	    SYMBOL_VISIBLE;
	void set_synapse(
	    halco::hicann_dls::vx::SynapseOnSynapseQuad const& synapse,
	    Synapse const& value) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 2;
	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& block) const
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
	void serialize(Archive& ar) SYMBOL_VISIBLE;

	halco::common::typed_array<Synapse, halco::hicann_dls::vx::SynapseOnSynapseQuad> m_synapses;
};

namespace detail {

template <>
struct BackendContainerTrait<CommonSynramConfig>
    : public BackendContainerBase<
          CommonSynramConfig,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

template <>
struct BackendContainerTrait<SynapseQuad>
    : public BackendContainerBase<
          SynapseQuad,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
{};

} // namespace detail

} // namespace vx
} // namespace haldls

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::vx::CommonSynramConfig::PCConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::CommonSynramConfig::WConf)
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::CommonSynramConfig::WaitCtrClear)

} // namespace std
