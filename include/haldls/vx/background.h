#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/background.h"
#include "halco/hicann-dls/vx/event.h"

#include "haldls/vx/container.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/math.h"
#include "hate/visibility.h"
#include <cereal/macros.hpp>

#ifndef __ppu__
#include "hxcomm/vx/target.h"
#endif

namespace haldls::vx {

struct BackgroundSpikeSource;

} // namespace haldls::vx

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::BackgroundSpikeSource& value, std::uint32_t const version);

} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

/**
 * Background spike source on chip.
 *
 * Two modes are supported, either regular spiking or binned Poisson spike-train generation.
 * While with the former the emitted NeuronLabel is configurable and stays fixed,
 * with the latter, the lower eight bits of the emitted NeuronLabel can bit-wise be replaced by
 * randomly generated values.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) BackgroundSpikeSource
    : public ContainerBase<BackgroundSpikeSource>
{
public:
	typedef halco::hicann_dls::vx::BackgroundSpikeSourceOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;
#ifndef __ppu__
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::hardware, hxcomm::vx::Target::simulation};
#endif

	/**
	 * Inter-spike interval for regular and inter-bin interval for Poisson spike generation.
	 * The period is set in units of clock cycles.
	 */
	struct GENPYBIND(inline_base("*")) Period
	    : public halco::common::detail::
	          RantWrapper<Period, uint_fast32_t, hate::math::pow(2, 16) - 1, 0>
	{
		constexpr explicit Period(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};

	/**
	 * Rate of spike output in the Poisson spike generation mode, without effect in regular mode.
	 * Every period, a random 8-bit number is compared to the rate value and a spike is generated
	 * if the random value is smaller-equal than the configured rate.
	 * This leads to a mean ISI of (period / (rate / 255)) and disabled spike output on rate = 0.
	 */
	struct GENPYBIND(inline_base("*")) Rate
	    : public halco::common::detail::
	          RantWrapper<Rate, uint_fast16_t, hate::math::pow(2, 8) - 1, 0>
	{
		constexpr explicit Rate(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};

	/**
	 * Random seed for random number generation.
	 * Only the lower 16 bit of the generator are used on every draw.
	 */
	struct GENPYBIND(inline_base("*")) Seed
	    : public halco::common::detail::
	          RantWrapper<Seed, uint_fast64_t, hate::math::pow(2, 32) - 1, 0>
	{
		constexpr explicit Seed(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};

	/**
	 * NeuronLabel bit randomization mask.
	 * The lower eight bits of the set neuron label can be replaced by random values on every spike
	 * output.
	 * Enabled bits lead to replacement by random values.
	 */
	struct GENPYBIND(inline_base("*")) Mask
	    : public halco::common::detail::
	          RantWrapper<Mask, uint_fast16_t, hate::math::pow(2, 8) - 1, 0>
	{
		constexpr explicit Mask(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) : rant_t(val)
		{}
	};

	/** Default constructor. */
	BackgroundSpikeSource() SYMBOL_VISIBLE;

	/**
	 * Get enable value.
	 * On change of disabled -> enabled the random seed is copied to the random number generator and
	 * the period is initialized with the full period which is decremented on every clock cycle.
	 * The first spike or bin is therefore reached with a full period delay after the enable change.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable))
	bool get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set enable value.
	 * On change of disabled -> enabled the random seed is copied to the random number generator and
	 * the period is initialized with the full period which is decremented on every clock cycle.
	 * The first spike or bin is therefore reached with a full period delay after the enable change.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(bool value) SYMBOL_VISIBLE;

	/**
	 * Get enable random spike train generation.
	 * On true, every period a bin evaluation in a binned Poisson generator is performed. In
	 * addition the enabled mask bits are randomized on spike ocurrence.
	 * On false, every period a spike is emitted leading to a regular spike-train.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(enable_random))
	bool get_enable_random() const SYMBOL_VISIBLE;

	/**
	 * Set enable random spike train generation.
	 * On true, every period a bin evaluation in a binned Poisson generator is performed. In
	 * addition the enabled mask bits are randomized on spike ocurrence.
	 * On false, every period a spike is emitted leading to a regular spike-train.
	 * @param value Boolean value
	 */
	GENPYBIND(setter_for(enable_random))
	void set_enable_random(bool value) SYMBOL_VISIBLE;

	/**
	 * Get period value.
	 * It represents the ISI for regular spiking and the bin interval for Poisson spike generation.
	 * @return Period
	 */
	GENPYBIND(getter_for(period))
	Period get_period() const SYMBOL_VISIBLE;

	/**
	 * Set period value.
	 * It represents the ISI for regular spiking and the bin interval for Poisson spike generation.
	 * @param value Period
	 */
	GENPYBIND(setter_for(period))
	void set_period(Period const& value) SYMBOL_VISIBLE;

	/**
	 * Get Poisson spike generation rate factor.
	 * The mean ISI is divided by (rate / 255). The value has no effect on regular spike generation.
	 * @return Rate
	 */
	GENPYBIND(getter_for(rate))
	Rate get_rate() const SYMBOL_VISIBLE;

	/**
	 * Set Poisson spike generation rate factor.
	 * The mean ISI is divided by (rate / 255). The value has no effect on regular spike generation.
	 * @param value Rate
	 */
	GENPYBIND(setter_for(rate))
	void set_rate(Rate const& value) SYMBOL_VISIBLE;

	/**
	 * Get random seed value.
	 * The value has no effect on regular spike generation.
	 * @return Seed
	 */
	GENPYBIND(getter_for(seed))
	Seed get_seed() const SYMBOL_VISIBLE;

	/**
	 * Set random seed value.
	 * The value has no effect on regular spike generation.
	 * @param value Seed
	 */
	GENPYBIND(setter_for(seed))
	void set_seed(Seed const& value) SYMBOL_VISIBLE;

	/**
	 * Get neuron label randomization mask.
	 * The value has no effect on regular spike generation.
	 * @return Mask
	 */
	GENPYBIND(getter_for(mask))
	Mask get_mask() const SYMBOL_VISIBLE;

	/**
	 * Set neuron label randomization mask.
	 * The value has no effect on regular spike generation.
	 * @param value Mask
	 */
	GENPYBIND(setter_for(mask))
	void set_mask(Mask const& value) SYMBOL_VISIBLE;

	/**
	 * Get neuron label used for emitted spikes.
	 * On Poisson spike generation, it serves as a base of which the lower eight bits can bit-wise
	 * be replaced by random values using the mask.
	 * @return NeuronLabel
	 */
	GENPYBIND(getter_for(neuron_label))
	halco::hicann_dls::vx::NeuronLabel get_neuron_label() const SYMBOL_VISIBLE;

	/**
	 * Set neuron label used for emitted spikes.
	 * On Poisson spike generation, it serves as a base of which the lower eight bits can bit-wise
	 * be replaced by random values using the mask.
	 * @param value NeuronLabel
	 */
	GENPYBIND(setter_for(neuron_label))
	void set_neuron_label(halco::hicann_dls::vx::NeuronLabel const& value) SYMBOL_VISIBLE;

	bool operator==(BackgroundSpikeSource const& other) const SYMBOL_VISIBLE;
	bool operator!=(BackgroundSpikeSource const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& neuron)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, BackgroundSpikeSource const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct cereal::access;
	template <class Archive>
	friend void ::cereal::serialize(
	    Archive& ar, BackgroundSpikeSource& value, std::uint32_t const version) SYMBOL_VISIBLE;

	bool m_enable;
	bool m_enable_random;
	Period m_period;
	Rate m_rate;
	Seed m_seed;
	Mask m_mask;
	halco::hicann_dls::vx::NeuronLabel m_neuron_label;
};


namespace detail {

template <>
struct BackendContainerTrait<BackgroundSpikeSource>
    : public BackendContainerBase<
          BackgroundSpikeSource,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
