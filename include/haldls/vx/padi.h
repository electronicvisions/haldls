#pragma once
#include <array>
#include <ostream>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/padi.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/synapse.h"
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

/**
 * A container for the PADI event trigger register.
 * There exists one trigger register per block of PADI buses.
 * Events carry the actual event address, a row select address to
 * determine the target synapse driver row, and a mask to specify
 * the set of PADI buses to fire the event on.
 */
class GENPYBIND(visible) PADIEvent
{
public:
	typedef halco::hicann_dls::vx::PADIEventOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	using EventAddress = haldls::vx::SynapseQuad::Label;

	/**
	 * Address in Hagen-mode.
	 * The highest bit of the six-bit EventAddress payload is used as label.
	 */
	struct GENPYBIND(inline_base("*")) HagenAddress
	    : public halco::common::detail::RantWrapper<HagenAddress, uint_fast16_t, 1, 0>
	{
		constexpr explicit HagenAddress(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/**
	 * Activation payload in Hagen-mode.
	 * The lower five bits of the six-bit EventAddress payload are used to modulate the pulselength.
	 */
	struct GENPYBIND(inline_base("*")) HagenActivation
	    : public halco::common::detail::RantWrapper<HagenActivation, uint_fast16_t, 31, 0>
	{
		constexpr explicit HagenActivation(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	struct GENPYBIND(inline_base("*")) RowSelectAddress
	    : public halco::common::detail::RantWrapper<RowSelectAddress, uint_fast16_t, 31, 0>
	{
		constexpr explicit RowSelectAddress(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::PADIBusOnPADIBusBlock>
	    fire_bus_type GENPYBIND(opaque, expose_as(_fire_bus_type));

	PADIEvent() SYMBOL_VISIBLE;

	// accessors

	/** Set mask determining the padi bus(es) to fire the event on. */
	GENPYBIND(getter_for(fire_bus), return_value_policy(reference))
	fire_bus_type const& get_fire_bus() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(fire_bus))
	void set_fire_bus(fire_bus_type const& value) SYMBOL_VISIBLE;

	/** Set event address, which is eventually forwarded to synapses. */
	GENPYBIND(getter_for(event_address))
	EventAddress get_event_address() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(event_address))
	void set_event_address(EventAddress const value) SYMBOL_VISIBLE;

	/**
	 * Set Hagen-mode address, which is eventually forwarded to synapses.
	 * This setting accesses the highest bit of the EventAddress.
	 */
	GENPYBIND(getter_for(hagen_address))
	HagenAddress get_hagen_address() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(hagen_address))
	void set_hagen_address(HagenAddress const value) SYMBOL_VISIBLE;

	/**
	 * Set Hagen-mode activation payload, which is eventually forwarded to synapses.
	 * This setting accesses the lower five bits of the EventAddress.
	 */
	GENPYBIND(getter_for(hagen_activation))
	HagenActivation get_hagen_activation() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(hagen_activation))
	void set_hagen_activation(HagenActivation const value) SYMBOL_VISIBLE;

	/**
	 * Set row select address determining the synapse driver which is supposed to
	 * fire.
	 * Note that each synapse driver can be configured to match only parts of the
	 * row select address.
	 */
	GENPYBIND(getter_for(row_select_address))
	RowSelectAddress get_row_select_address() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(row_select_address))
	void set_row_select_address(RowSelectAddress const value) SYMBOL_VISIBLE;

	bool operator==(PADIEvent const& other) const SYMBOL_VISIBLE;
	bool operator!=(PADIEvent const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PADIEvent const& event) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	struct PADIEventBitfield;

	fire_bus_type m_fire_bus;
	EventAddress m_event_address;
	RowSelectAddress m_row_select_address;
};

/**
 * Configuration for a PADI bus block.
 * Each of the four buses can be configured to accept events from the PADI
 * event trigger register or from the SPL1 merger matrix.
 * Additionally the timing of PADI pulses can be modified.
 */
class GENPYBIND(visible) CommonPADIBusConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::CommonPADIBusConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::PADIBusOnPADIBusBlock>
	    enable_spl1_type GENPYBIND(opaque(false), expose_as(_enable_spl1_type));

	typedef halco::common::typed_array<bool, halco::hicann_dls::vx::PADIBusOnPADIBusBlock>
	    enable_extended_timing_type
	        GENPYBIND(opaque(false), expose_as(_enable_extended_timing_type));

	struct GENPYBIND(inline_base("*")) DacenPulseExtension
	    : public halco::common::detail::RantWrapper<DacenPulseExtension, uint_fast16_t, 15, 0>
	{
		constexpr explicit DacenPulseExtension(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	typedef halco::common::
	    typed_array<DacenPulseExtension, halco::hicann_dls::vx::PADIBusOnPADIBusBlock>
	        dacen_pulse_extension_type GENPYBIND(opaque, expose_as(_dacen_pulse_extension_type));

	CommonPADIBusConfig() SYMBOL_VISIBLE;

	// accessors

	/**
	 * Configure the individual buses to accept SPL1 events instead of PADI events.
	 * Note that these options are mutually exclusive!
	 */
	GENPYBIND(getter_for(enable_spl1), return_value_policy(reference))
	enable_spl1_type const& get_enable_spl1() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_spl1))
	void set_enable_spl1(enable_spl1_type const& value) SYMBOL_VISIBLE;

	/**
	 * Enable extended timing to allow the STP circuit to fully deplete the
	 * virtual neurotransmitter partition.
	 * Back-to-back events are not possible in this configuration. Handle
	 * with care!
	 */
	GENPYBIND(getter_for(enable_extended_timing), return_value_policy(reference))
	enable_extended_timing_type const& get_enable_extended_timing() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_extended_timing))
	void set_enable_extended_timing(enable_extended_timing_type const& value) SYMBOL_VISIBLE;

	/**
	 * Extend the timing used to generate the dacen pulses for the synapses.
	 * The longer the pulse, the larger the resulting synaptic weights.
	 * In particular useful for triggering the neuron's bypass mode.
	 * It is not recommended to use it for weight scaling!
	 */
	GENPYBIND(getter_for(dacen_pulse_extension), return_value_policy(reference))
	dacen_pulse_extension_type const& get_dacen_pulse_extension() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(dacen_pulse_extension))
	void set_dacen_pulse_extension(dacen_pulse_extension_type const& value) SYMBOL_VISIBLE;

	bool operator==(CommonPADIBusConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonPADIBusConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) = 1;
	template <typename AddressT>
	static std::array<AddressT, config_size_in_words> addresses(coordinate_type const& coord)
	    SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CommonPADIBusConfig const& event)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	struct CommonPADIBusConfigBitfield;

	enable_spl1_type m_enable_spl1;
	enable_extended_timing_type m_enable_extended_timing;
	dacen_pulse_extension_type m_dacen_pulse_extension;
};

/**
 * Common STP configuration shared by synapse drivers per vertical half.
 * It allows to enable/disable and set the speed for the recovery clock.
 */
class GENPYBIND(visible) CommonSTPConfig : public DifferentialWriteTrait
{
public:
	typedef halco::hicann_dls::vx::CommonSTPConfigOnDLS coordinate_type;
	typedef std::true_type is_leaf_node;

	struct GENPYBIND(inline_base("*")) RecoveryClockSpeed
	    : public halco::common::detail::RantWrapper<RecoveryClockSpeed, uint_fast16_t, 15, 0>
	{
		constexpr explicit RecoveryClockSpeed(uintmax_t const val = 0)
		    GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	CommonSTPConfig() SYMBOL_VISIBLE;

	// accessors

	/** Enable recovery clock for the STP circuit. */
	GENPYBIND(getter_for(enable_recovery_clock))
	bool get_enable_recovery_clock() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(enable_recovery_clock))
	void set_enable_recovery_clock(bool const value) SYMBOL_VISIBLE;

	/** Set the scaler for the recovery clock. */
	GENPYBIND(getter_for(recovery_clock_speed))
	RecoveryClockSpeed get_recovery_clock_speed() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(recovery_clock_speed))
	void set_recovery_clock_speed(RecoveryClockSpeed const value) SYMBOL_VISIBLE;

	bool operator==(CommonSTPConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(CommonSTPConfig const& other) const SYMBOL_VISIBLE;

	static size_t constexpr write_config_size_in_words GENPYBIND(hidden) = 1;
	static size_t constexpr read_config_size_in_words GENPYBIND(hidden) = 0;
	template <typename AddressT>
	static std::array<AddressT, read_config_size_in_words> read_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename AddressT>
	static std::array<AddressT, write_config_size_in_words> write_addresses(
	    coordinate_type const& coord) SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	std::array<WordT, write_config_size_in_words> encode() const SYMBOL_VISIBLE GENPYBIND(hidden);
	template <typename WordT>
	void decode(std::array<WordT, read_config_size_in_words> const& data) SYMBOL_VISIBLE
	    GENPYBIND(hidden);

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CommonSTPConfig const& event) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;

	struct CommonSTPConfigBitfield;

	bool m_enable_recovery_clock;
	RecoveryClockSpeed m_recovery_clock_speed;
};

namespace detail {

template <>
struct BackendContainerTrait<PADIEvent>
    : public BackendContainerBase<PADIEvent, fisch::vx::OmnibusChip, fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<CommonPADIBusConfig>
    : public BackendContainerBase<
          CommonPADIBusConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<CommonSTPConfig>
    : public BackendContainerBase<
          CommonSTPConfig,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
