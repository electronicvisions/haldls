#pragma once
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/dac.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"
#include <boost/hana/adapt_struct.hpp>

namespace lola::vx GENPYBIND_TAG_LOLA_VX {

/**
 * Container for configuration of all DAC channels on the xBoard.
 */
class GENPYBIND(visible) DACChannelBlock
{
public:
	typedef halco::hicann_dls::vx::DACChannelBlockOnBoard coordinate_type;
	typedef std::false_type has_local_data;

	typedef haldls::vx::DACChannel::Value Value GENPYBIND(visible);

	/** Default constructor. */
	DACChannelBlock() SYMBOL_VISIBLE;

	typedef halco::common::typed_array<Value, halco::hicann_dls::vx::DACChannelOnBoard> _value_type
	    GENPYBIND(opaque);

	/** Array of channel values. */
	_value_type value;

	/**
	 * Default value for LDO version 1.
	 */
	static const SYMBOL_VISIBLE GENPYBIND(visible) DACChannelBlock default_ldo_1;

	/**
	 * Default value for LDO version 2.
	 */
	static const SYMBOL_VISIBLE GENPYBIND(visible) DACChannelBlock default_ldo_2;

	/**
	 * Setter for DAC values in volts.
	 *
	 * @param coord: Coordinate of the DAC channel to configure.
	 * @param voltage: Voltage in Volt (V) to apply at that channel.
	 */
	void set_voltage(halco::hicann_dls::vx::DACChannelOnBoard coord, double voltage);

	/**
	 * Getter for DAC values in volts.
	 *
	 * @param coord: Coordinate of the DAC channel to read.
	 * @return Voltage in Volt (V) applied at that channel.
	 */
	double get_voltage(halco::hicann_dls::vx::DACChannelOnBoard coord);

	bool operator==(DACChannelBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(DACChannelBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, DACChannelBlock const& block) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::DACChannelBlock>;
};


/**
 * Container for configuration of all DAC channel enable values on the xBoard.
 */
class GENPYBIND(visible) DACControlBlock
{
public:
	typedef halco::hicann_dls::vx::DACControlBlockOnBoard coordinate_type;
	typedef std::false_type has_local_data;

	/** Default constructor. */
	DACControlBlock() SYMBOL_VISIBLE;

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::DACChannelOnBoard> _enable_type
	    GENPYBIND(opaque);

	/** Array of channel enable values. */
	_enable_type enable;

	bool operator==(DACControlBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(DACControlBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, DACControlBlock const& block) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::DACControlBlock>;
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::DACChannelBlock>
    : public BackendContainerBase<lola::vx::DACChannelBlock, fisch::vx::SPIDACDataRegister>
{};

template <>
struct BackendContainerTrait<lola::vx::DACControlBlock>
    : public BackendContainerBase<lola::vx::DACControlBlock, fisch::vx::SPIDACControlRegister>
{};

template <>
struct VisitPreorderImpl<lola::vx::DACChannelBlock>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::DACChannelBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto ch : iter_all<DACChannelOnBoard>()) {
			DACChannel channel(config.value[ch]);
			visit_preorder(channel, ch, visitor);
		}
	}
};

template <>
struct VisitPreorderImpl<lola::vx::DACControlBlock>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::DACControlBlock::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto dac : iter_all<DACOnBoard>()) {
			DACControl control;
			for (auto ch : iter_all<DACChannelOnDAC>()) {
				control.set_enable_channel(ch, config.enable[DACChannelOnBoard(ch, dac)]);
			}
			visit_preorder(control, dac, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::DACChannelBlock, value);
BOOST_HANA_ADAPT_STRUCT(lola::vx::DACControlBlock, enable);

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::DACChannelBlock)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::DACControlBlock)
