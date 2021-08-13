#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/highspeed_link.h"
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace halco::hicann_dls::vx {
class OmnibusAddress;
class OmnibusChipOverJTAGAddress;
} // namespace halco::hicann_dls::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

namespace detail GENPYBIND_MODULE {

/**
 * Common base class for PhyConfig of the FPGA- and chip-side PHYs.
 */
class GENPYBIND(visible) PhyConfigBase
{
public:
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) ManualDelay
	    : public halco::common::detail::RantWrapper<ManualDelay, uint_fast32_t, 15, 0>
	{
		constexpr explicit ManualDelay(uintmax_t const val = 0) : rant_t(val) {}
	};

	struct GENPYBIND(inline_base("*")) VBias
	    : public halco::common::detail::RantWrapper<VBias, uint_fast32_t, 7, 0>
	{
		constexpr explicit VBias(uintmax_t const val = 0) : rant_t(val) {}
	};

	struct GENPYBIND(inline_base("*")) DebugOutputs
	    : public halco::common::detail::RantWrapper<DebugOutputs, uint_fast32_t, 1, 0>
	{
		constexpr explicit DebugOutputs(uintmax_t const val = 0) : rant_t(val) {}
	};

	GENPYBIND(getter_for(enable_bit_slip))
	bool get_enable_bit_slip() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_bit_slip))
	void set_enable_bit_slip(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(manual_delay))
	ManualDelay get_manual_delay() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(manual_delay))
	void set_manual_delay(ManualDelay value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_delay_cell_measurement))
	bool get_enable_delay_cell_measurement() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_delay_cell_measurement))
	void set_enable_delay_cell_measurement(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_initialization_master_mode))
	bool get_enable_initialization_master_mode() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_initialization_master_mode))
	void set_enable_initialization_master_mode(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_manual_tx_data_valid_for_init))
	bool get_enable_manual_tx_data_valid_for_init() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_manual_tx_data_valid_for_init))
	void set_enable_manual_tx_data_valid_for_init(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_force_lvds_power_up))
	bool get_enable_force_lvds_power_up() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_force_lvds_power_up))
	void set_enable_force_lvds_power_up(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_force_start))
	bool get_enable_force_start() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_force_start))
	void set_enable_force_start(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_manual_training_mode))
	bool get_enable_manual_training_mode() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_manual_training_mode))
	void set_enable_manual_training_mode(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_ber_loopback))
	bool get_enable_ber_loopback() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_ber_loopback))
	void set_enable_ber_loopback(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(vbias))
	VBias get_vbias() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(vbias))
	void set_vbias(VBias value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(debug_outputs))
	DebugOutputs get_debug_outputs() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(debug_outputs))
	void set_debug_outputs(DebugOutputs value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_transmission_without_idls_pattern))
	bool get_enable_transmission_without_idle_pattern() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_transmission_without_idls_pattern))
	void set_enable_transmission_without_idle_pattern(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_clock_pre_alignment))
	bool get_enable_clock_pre_alignment() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_clock_pre_alignment))
	void set_enable_clock_pre_alignment(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_des_recal))
	bool get_enable_des_recal() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_des_recal))
	void set_enable_des_recal(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_loopback_en))
	bool get_enable_loopback_en() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_loopback_en))
	void set_enable_loopback_en(bool value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(enable_auto_init))
	bool get_enable_auto_init() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_auto_init))
	void set_enable_auto_init(bool value) SYMBOL_VISIBLE;

	bool operator==(PhyConfigBase const& other) const SYMBOL_VISIBLE;
	bool operator!=(PhyConfigBase const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PhyConfigBase const& config) SYMBOL_VISIBLE;

protected:
	PhyConfigBase() SYMBOL_VISIBLE;

	friend class cereal::access;
	template <typename Archive>
	void cerealize_impl(Archive& ar) SYMBOL_VISIBLE;

	uint32_t pack() const SYMBOL_VISIBLE;
	void unpack(uint32_t value) SYMBOL_VISIBLE;

	bool m_enable_bit_slip;
	ManualDelay m_manual_delay;
	bool m_enable_delay_cell_measurement;
	bool m_enable_initialization_master_mode;
	bool m_enable_manual_tx_data_valid_for_init;
	bool m_enable_force_lvds_power_up;
	bool m_enable_force_start;
	bool m_enable_manual_training_mode;
	bool m_enable_ber_loopback;
	VBias m_vbias;
	DebugOutputs m_debug_outputs;
	bool m_enable_transmission_without_idle_pattern;
	bool m_enable_clock_pre_alignment;
	bool m_enable_des_recal;
	bool m_enable_loopback_en;
	bool m_enable_auto_init;
};

} // namespace detail


/**
 * Container for individual configuration of FPGA-side PHYs.
 */
// FIXME: Instead of normal public inheritance we want CRTP here, which is not possible due to
// python wrapping issues, the properties don't show up in the derived class.
class SYMBOL_VISIBLE GENPYBIND(visible) PhyConfigFPGA : public detail::PhyConfigBase
{
public:
	typedef halco::hicann_dls::vx::PhyConfigFPGAOnDLS coordinate_type;

	/** Default constructor. */
	PhyConfigFPGA() SYMBOL_VISIBLE;

	bool operator==(PhyConfigFPGA const& other) const SYMBOL_VISIBLE;
	bool operator!=(PhyConfigFPGA const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PhyConfigFPGA const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::Omnibus, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Omnibus, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PhyConfigFPGA)

namespace detail {

template <>
struct BackendContainerTrait<PhyConfigFPGA>
    : public BackendContainerBase<PhyConfigFPGA, fisch::vx::Omnibus>
{};

} // namespace detail


/**
 * Container for individual configuration of chip-side PHYs.
 */
// FIXME: Instead of normal public inheritance we want CRTP here, which is not possible due to
// python wrapping issues, the properties don't show up in the derived class.
class GENPYBIND(visible) PhyConfigChip : public detail::PhyConfigBase
{
public:
	typedef halco::hicann_dls::vx::PhyConfigChipOnDLS coordinate_type;

	/** Default constructor. */
	PhyConfigChip() SYMBOL_VISIBLE;

	bool operator==(PhyConfigChip const& other) const SYMBOL_VISIBLE;
	bool operator!=(PhyConfigChip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PhyConfigChip const& config) SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::JTAGPhyRegisterOnDLS, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::JTAGPhyRegister, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::JTAGPhyRegister, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PhyConfigChip)

namespace detail {

template <>
struct BackendContainerTrait<PhyConfigChip>
    : public BackendContainerBase<PhyConfigChip, fisch::vx::JTAGPhyRegister>
{};

} // namespace detail


/**
 * Container for configuration of enable values for the FPGA-side PHYs.
 */
class GENPYBIND(visible) CommonPhyConfigFPGA
{
public:
	typedef halco::hicann_dls::vx::CommonPhyConfigFPGAOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor. */
	CommonPhyConfigFPGA() SYMBOL_VISIBLE;

	bool get_enable_phy(halco::hicann_dls::vx::PhyConfigFPGAOnDLS const& coord) const
	    SYMBOL_VISIBLE;
	void set_enable_phy(halco::hicann_dls::vx::PhyConfigFPGAOnDLS const& coord, bool value)
	    SYMBOL_VISIBLE;

	bool operator==(CommonPhyConfigFPGA const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonPhyConfigFPGA const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CommonPhyConfigFPGA const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, config_size_in_words> addresses(
	    coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::Omnibus, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Omnibus, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	halco::common::typed_array<bool, halco::hicann_dls::vx::PhyConfigFPGAOnDLS> m_enable_phy;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CommonPhyConfigFPGA)

namespace detail {

template <>
struct BackendContainerTrait<CommonPhyConfigFPGA>
    : public BackendContainerBase<CommonPhyConfigFPGA, fisch::vx::Omnibus>
{};

} // namespace detail


/**
 * Container for configuration of enable values for the chip-side PHYs.
 */
class GENPYBIND(visible) CommonPhyConfigChip
{
public:
	typedef halco::hicann_dls::vx::CommonPhyConfigChipOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	/** Default constructor. */
	CommonPhyConfigChip() SYMBOL_VISIBLE;

	bool get_enable_phy(halco::hicann_dls::vx::PhyConfigChipOnDLS const& coord) const
	    SYMBOL_VISIBLE;
	void set_enable_phy(halco::hicann_dls::vx::PhyConfigChipOnDLS const& coord, bool value)
	    SYMBOL_VISIBLE;

	bool operator==(CommonPhyConfigChip const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonPhyConfigChip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CommonPhyConfigChip const& config)
	    SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	static std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, config_size_in_words>
	addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::OmnibusChipOverJTAG, config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::OmnibusChipOverJTAG, config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	halco::common::typed_array<bool, halco::hicann_dls::vx::PhyConfigChipOnDLS> m_enable_phy;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(CommonPhyConfigChip)

namespace detail {

template <>
struct BackendContainerTrait<CommonPhyConfigChip>
    : public BackendContainerBase<CommonPhyConfigChip, fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail


class GENPYBIND(visible) PhyStatus
{
public:
	typedef halco::hicann_dls::vx::PhyStatusOnFPGA coordinate_type;
	typedef std::true_type is_leaf_node;

	PhyStatus() SYMBOL_VISIBLE;

	/**
	 * Number of CRC errors since last reset.
	 */
	struct GENPYBIND(inline_base("*")) CRCErrorCount
	    : public halco::common::detail::BaseType<CRCErrorCount, uint32_t>
	{
		constexpr explicit CRCErrorCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Online time in cycles since last reset.
	 */
	struct GENPYBIND(inline_base("*")) OnlineTime
	    : public halco::common::detail::BaseType<OnlineTime, uint32_t>
	{
		constexpr explicit OnlineTime(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Number of dropped rx words.
	 */
	struct GENPYBIND(inline_base("*")) RxDroppedCount
	    : public halco::common::detail::BaseType<RxDroppedCount, uint32_t>
	{
		constexpr explicit RxDroppedCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Number of rx words from Chip since last reset.
	 */
	struct GENPYBIND(inline_base("*")) RxCount
	    : public halco::common::detail::BaseType<RxCount, uint32_t>
	{
		constexpr explicit RxCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	/**
	 * Number of tx words to Chip since last reset.
	 */
	struct GENPYBIND(inline_base("*")) TxCount
	    : public halco::common::detail::BaseType<TxCount, uint32_t>
	{
		constexpr explicit TxCount(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    base_t(val)
		{}
	};

	GENPYBIND(getter_for(crc_error_count))
	CRCErrorCount get_crc_error_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(crc_error_count))
	void set_crc_error_count(CRCErrorCount value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(online_time))
	OnlineTime get_online_time() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(online_time))
	void set_online_time(OnlineTime value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(rx_dropped_count))
	RxDroppedCount get_rx_dropped_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(rx_dropped_count))
	void set_rx_dropped_count(RxDroppedCount value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(rx_count))
	RxCount get_rx_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(rx_count))
	void set_rx_count(RxCount value) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(tx_count))
	TxCount get_tx_count() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(tx_count))
	void set_tx_count(TxCount value) SYMBOL_VISIBLE;

	bool operator==(PhyStatus const& other) const SYMBOL_VISIBLE;
	bool operator!=(PhyStatus const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PhyStatus const& config) SYMBOL_VISIBLE;

	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 5;
	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 0;
	static std::array<halco::hicann_dls::vx::OmnibusAddress, read_config_size_in_words>
	read_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	static std::array<halco::hicann_dls::vx::OmnibusAddress, write_config_size_in_words>
	write_addresses(coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	std::array<fisch::vx::Omnibus, write_config_size_in_words> encode() const SYMBOL_VISIBLE
	    GENPYBIND(hidden);
	void decode(std::array<fisch::vx::Omnibus, read_config_size_in_words> const& data)
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	CRCErrorCount m_crc_error_count;
	OnlineTime m_online_time;
	RxDroppedCount m_rx_dropped_count;
	RxCount m_rx_count;
	TxCount m_tx_count;
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(PhyStatus)

namespace detail {

template <>
struct BackendContainerTrait<PhyStatus> : public BackendContainerBase<PhyStatus, fisch::vx::Omnibus>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
