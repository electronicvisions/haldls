#pragma once
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/dac.h"
#include "haldls/vx/common.h"
#include "haldls/vx/spi.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

namespace cereal {
class access;
} // namespace cereal

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

	/**
	 * Get array of channel values.
	 * @return Array of channel values
	 */
	GENPYBIND(getter_for(value), return_value_policy(reference))
	_value_type const& get_value() const SYMBOL_VISIBLE;

	/**
	 * Set array of channel values.
	 * @param value Array of channel values
	 */
	GENPYBIND(setter_for(value))
	void set_value(_value_type const& value) SYMBOL_VISIBLE;

	/**
	 * Default value for LDO version 1.
	 */
	static const GENPYBIND(visible) DACChannelBlock default_ldo_1;

	/**
	 * Default value for LDO version 2.
	 */
	static const GENPYBIND(visible) DACChannelBlock default_ldo_2;

	bool operator==(DACChannelBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(DACChannelBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, DACChannelBlock const& block) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::DACChannelBlock>;
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar);

	_value_type m_value;
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

	/**
	 * Get array of channel enable values.
	 * @return Array of boolean values
	 */
	GENPYBIND(getter_for(enable), return_value_policy(reference))
	_enable_type const& get_enable() const SYMBOL_VISIBLE;

	/**
	 * Set array of channel enable values.
	 * @param value Array of boolean values
	 */
	GENPYBIND(setter_for(enable))
	void set_enable(_enable_type const& value) SYMBOL_VISIBLE;

	bool operator==(DACControlBlock const& other) const SYMBOL_VISIBLE;
	bool operator!=(DACControlBlock const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, DACControlBlock const& block) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<lola::vx::DACControlBlock>;
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar);

	_enable_type m_enable;
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
			DACChannel channel(config.m_value[ch]);
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
				control.set_enable_channel(ch, config.m_enable[DACChannelOnBoard(ch, dac)]);
			}
			visit_preorder(control, dac, visitor);
		}
	}
};

} // namespace haldls::vx::detail
