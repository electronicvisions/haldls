#pragma once
#include <array>
#include <ostream>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/synapse_driver.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class OmnibusChip;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

class GENPYBIND(visible) SynapseDriverConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::SynapseDriverOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) Utilization
	    : public halco::common::detail::RantWrapper<Utilization, uint_fast8_t, 15, 0>
	{
		constexpr explicit Utilization(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) Offset
	    : public halco::common::detail::RantWrapper<Offset, uint_fast8_t, 15, 0>
	{
		constexpr explicit Offset(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) Recovery
	    : public halco::common::detail::RantWrapper<Recovery, uint_fast8_t, 15, 0>
	{
		constexpr explicit Recovery(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) TargetVoltages
	    : public halco::common::detail::RantWrapper<TargetVoltages, uint_fast8_t, 1, 0>
	{
		constexpr explicit TargetVoltages(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) RowAddressCompareMask
	    : public halco::common::detail::RantWrapper<RowAddressCompareMask, uint_fast8_t, 31, 0>
	{
		constexpr explicit RowAddressCompareMask(uintmax_t const val = 0b11111)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	enum class GENPYBIND(visible) RowMode
	{
		disabled,
		excitatory,
		inhibitory,
		excitatory_and_inhibitory
	};

	SynapseDriverConfig() SYMBOL_VISIBLE;

	// accessors

	/** Enable padi bus receiver. */
	GENPYBIND(getter_for(enable_receiver))
	bool get_enable_receiver() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_receiver))
	void set_enable_receiver(bool const value) SYMBOL_VISIBLE;

	/**
	 * Set row select address compare mask.
	 * The incoming row select address allows to individually address a single
	 * synapse driver. For this purpose, the events row select address (5 bit)
	 * is compared to the synapse driver index (static).
	 * The padi bus receiver can be configured to ignore (parts of) the row
	 * select address by zeroing (parts of) the `row_address_compare_mask`.
	 */
	GENPYBIND(getter_for(row_address_compare_mask))
	RowAddressCompareMask get_row_address_compare_mask() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(row_address_compare_mask))
	void set_row_address_compare_mask(RowAddressCompareMask const value) SYMBOL_VISIBLE;

	/**
	 * This enables sending the five lower bits of an address to the synapses.
	 * The MSB is always sent. This bit also enables Hagen mode.
	 * These two options should have been mutually exclusive but overlap due to a
	 * faulty padi bus receiver circuit.
	 */
	GENPYBIND(getter_for(enable_address_out))
	bool get_enable_address_out() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_address_out))
	void set_enable_address_out(bool const value) SYMBOL_VISIBLE;

	/** Set strength of STP utilization. */
	GENPYBIND(getter_for(utilization))
	Utilization get_utilization() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(utilization))
	void set_utilization(Utilization const value) SYMBOL_VISIBLE;

	/** Calibration value for dacen pulse width. */
	GENPYBIND(getter_for(offset))
	Offset get_offset() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(offset))
	void set_offset(Offset const value) SYMBOL_VISIBLE;

	/** Set STP recovery time constant. */
	GENPYBIND(getter_for(recovery))
	Recovery get_recovery() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(recovery))
	void set_recovery(Recovery const value) SYMBOL_VISIBLE;

	/** Set bottom row mode. */
	GENPYBIND(getter_for(row_mode_bottom))
	RowMode get_row_mode_bottom() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(row_mode_bottom))
	void set_row_mode_bottom(RowMode value) SYMBOL_VISIBLE;

	/** Set top row mode. */
	GENPYBIND(getter_for(row_mode_top))
	RowMode get_row_mode_top() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(row_mode_top))
	void set_row_mode_top(RowMode value) SYMBOL_VISIBLE;

	/**
	 * Select a pair of STP target voltages (vrecover, vcharge).
	 * Each synapse driver is provided with two sets of the STP target
	 * voltages, to allow for two different parameterizations within a single
	 * array.
	 * Note that although there are only two logical arrays of synapse drivers
	 * per chip, even and odd synapse drivers are fed with disjunct analog
	 * parameters provided by the east and west capmem instances, respectively.
	 */
	GENPYBIND(getter_for(select_target_voltages))
	TargetVoltages get_select_target_voltages() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(select_target_voltages))
	void set_select_target_voltages(TargetVoltages const value) SYMBOL_VISIBLE;

	/**
	 * Enable readout source follower and connect to readout line.
	 * The readout is shared between all instances of a vertical half.
	 * Connecting multiple instances is non-destructive but will result
	 * in faulty readings.
	 * Also note that an additional buffer per half must be biased to drive the
	 * signals further down to the readout chain.
	 */
	GENPYBIND(getter_for(enable_readout))
	bool get_enable_readout() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_readout))
	void set_enable_readout(bool const value) SYMBOL_VISIBLE;

	/** Enable renewing synapses for LIF sampling (broken due to faulty circuit). */
	GENPYBIND(getter_for(enable_renewing))
	bool get_enable_renewing() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_renewing))
	void set_enable_renewing(bool const value) SYMBOL_VISIBLE;

	/**
	 * Enable modulation of pulse width proportional to event address.
	 * Note:
	 *   - Hagen DAC must also be enabled through `enable_address_out`
	 *   - STP circuit must also be enabled
	 */
	GENPYBIND(getter_for(enable_hagen_modulation))
	bool get_enable_hagen_modulation() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_hagen_modulation))
	void set_enable_hagen_modulation(bool const value) SYMBOL_VISIBLE;

	/** Enable STP circuit. */
	GENPYBIND(getter_for(enable_stp))
	bool get_enable_stp() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_stp))
	void set_enable_stp(bool const value) SYMBOL_VISIBLE;

	/** Enable charge sharing for larger STP utilization values (broken). */
	GENPYBIND(getter_for(enable_charge_sharing))
	bool get_enable_charge_sharing() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_charge_sharing))
	void set_enable_charge_sharing(bool const value) SYMBOL_VISIBLE;

	/** Enable STP recovery circuit. */
	GENPYBIND(getter_for(enable_recovery))
	bool get_enable_recovery() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_recovery))
	void set_enable_recovery(bool const value) SYMBOL_VISIBLE;

	bool operator==(SynapseDriverConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseDriverConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 3;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(
	    coordinate_type const& synapse_driver) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseDriverConfig const& config) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	struct SynapseDriverConfigBitfield;

	SynapseDriverConfigBitfield to_bitfield() const;
	void from_bitfield(SynapseDriverConfigBitfield const& bitfield);

	bool m_en_receiver;
	RowAddressCompareMask m_row_address_compare_mask;
	bool m_en_address_out;
	Utilization m_utilization;
	Offset m_offset;
	Recovery m_recovery;
	RowMode m_row_mode_bottom;
	RowMode m_row_mode_top;
	TargetVoltages m_select_target_voltages;
	bool m_en_readout;
	bool m_en_renewing;
	bool m_en_hagen_modulation;
	bool m_en_stp;
	bool m_en_charge_sharing;
	bool m_en_recovery;
};

std::ostream& operator<<(std::ostream& os, SynapseDriverConfig::RowMode const& mode) SYMBOL_VISIBLE;

namespace detail {

template <>
struct BackendContainerTrait<SynapseDriverConfig>
    : public BackendContainerBase<
          SynapseDriverConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
