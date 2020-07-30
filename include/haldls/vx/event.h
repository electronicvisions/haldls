#pragma once
#include "fisch/vx/event.h"
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/event.h"
#include "halco/hicann-dls/vx/highspeed_link.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/neuron.h"
#include "haldls/vx/padi.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace cereal {
class access;
} // namespace cereal

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

struct GENPYBIND(inline_base("*")) SpikeLabel
    : public halco::common::detail::BaseType<SpikeLabel, uint16_t>
{
	constexpr explicit SpikeLabel(value_type const value = value_type()) : base_t(value) {}

	/** SPL1-label, bits 14-15. */
	GENPYBIND(getter_for(spl1_address))
	halco::hicann_dls::vx::SPL1Address get_spl1_address() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(spl1_address))
	void set_spl1_address(halco::hicann_dls::vx::SPL1Address value) SYMBOL_VISIBLE;

	/** Label type processed by Crossbar, bits 0-13. */
	GENPYBIND(getter_for(neuron_label))
	halco::hicann_dls::vx::NeuronLabel get_neuron_label() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(neuron_label))
	void set_neuron_label(halco::hicann_dls::vx::NeuronLabel value) SYMBOL_VISIBLE;

	/** Label type processed by PADI-bus, bits 0-10. */
	struct GENPYBIND(inline_base("*")) PADILabel
	    : public halco::common::detail::RantWrapper<PADILabel, uint_fast16_t, 0x3ff, 0>
	{
		constexpr explicit PADILabel(uintmax_t const val = 0) GENPYBIND(implicit_conversion) :
		    rant_t(val)
		{}
	};

	/** PADI-bus label, bits 0-10. */
	GENPYBIND(getter_for(padi_label))
	PADILabel get_padi_label() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(padi_label))
	void set_padi_label(PADILabel value) SYMBOL_VISIBLE;

	/** Configurable neuron backend output label, bits 0-7. */
	GENPYBIND(getter_for(neuron_backend_address_out))
	NeuronBackendConfig::AddressOut get_neuron_backend_address_out() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(neuron_backend_address_out))
	void set_neuron_backend_address_out(NeuronBackendConfig::AddressOut value) SYMBOL_VISIBLE;

	/** Configurable row select address on PADIBus, bits 6-10. */
	GENPYBIND(getter_for(row_select_address))
	PADIEvent::RowSelectAddress get_row_select_address() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(row_select_address))
	void set_row_select_address(PADIEvent::RowSelectAddress) SYMBOL_VISIBLE;

	/** Configurable synapse address, bits 0-5. */
	GENPYBIND(getter_for(synapse_label))
	SynapseQuad::Label get_synapse_label() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(synapse_label))
	void set_synapse_label(SynapseQuad::Label value) SYMBOL_VISIBLE;
};

#define SpikePackToChip(Num)                                                                       \
	class GENPYBIND(visible) SpikePack##Num##ToChip                                                \
	{                                                                                              \
	public:                                                                                        \
		typedef halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS coordinate_type;                \
		typedef std::true_type is_leaf_node;                                                       \
                                                                                                   \
		typedef std::array<SpikeLabel, Num> labels_type;                                           \
                                                                                                   \
		/** Default constructor. */                                                                \
		SpikePack##Num##ToChip() SYMBOL_VISIBLE;                                                   \
                                                                                                   \
		/**                                                                                        \
		 * Construct spike pack with labels.                                                       \
		 * @param labels Array of SpikeLabel values to use                                         \
		 */                                                                                        \
		explicit SpikePack##Num##ToChip(labels_type const& labels) SYMBOL_VISIBLE;                 \
                                                                                                   \
		/**                                                                                        \
		 * Get spike labels.                                                                       \
		 * @return Array of SpikeLabel                                                             \
		 */                                                                                        \
		GENPYBIND(getter_for(labels))                                                              \
		labels_type get_labels() const SYMBOL_VISIBLE;                                             \
                                                                                                   \
		/**                                                                                        \
		 * Set spike labels.                                                                       \
		 * @param value Array of SpikeLabel to set                                                 \
		 */                                                                                        \
		GENPYBIND(setter_for(labels))                                                              \
		void set_labels(labels_type const& value) SYMBOL_VISIBLE;                                  \
                                                                                                   \
		bool operator==(SpikePack##Num##ToChip const& other) const SYMBOL_VISIBLE;                 \
		bool operator!=(SpikePack##Num##ToChip const& other) const SYMBOL_VISIBLE;                 \
                                                                                                   \
		GENPYBIND(stringstream)                                                                    \
		friend std::ostream& operator<<(std::ostream& os, SpikePack##Num##ToChip const& config)    \
		    SYMBOL_VISIBLE;                                                                        \
                                                                                                   \
		constexpr static size_t GENPYBIND(hidden) config_size_in_words = 1;                        \
		static std::array<                                                                         \
			halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS, config_size_in_words>              \
		addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);                   \
		std::array<fisch::vx::SpikePack##Num##ToChip, config_size_in_words> encode() const         \
		    SYMBOL_VISIBLE GENPYBIND(hidden);                                                      \
		void decode(std::array<fisch::vx::SpikePack##Num##ToChip, config_size_in_words> const&     \
		                data) SYMBOL_VISIBLE GENPYBIND(hidden);                                    \
                                                                                                   \
	private:                                                                                       \
		friend class cereal::access;                                                               \
		template <class Archive>                                                                   \
		void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;                   \
                                                                                                   \
		labels_type m_impl;                                                                        \
	};                                                                                             \
                                                                                                   \
	namespace detail {                                                                             \
                                                                                                   \
	template <>                                                                                    \
	struct BackendContainerTrait<SpikePack##Num##ToChip>                                           \
	    : public BackendContainerBase<SpikePack##Num##ToChip, fisch::vx::SpikePack##Num##ToChip>   \
	{};                                                                                            \
                                                                                                   \
	} // namespace detail

// clang-format off
SpikePackToChip(1)
SpikePackToChip(2)
SpikePackToChip(3)
// clang-format on

#undef SpikePackToChip


typedef fisch::vx::FPGATime FPGATime GENPYBIND(visible);
typedef fisch::vx::ChipTime ChipTime GENPYBIND(visible);

namespace detail {
	struct SpikeFromChipChecker;
}

/**
 * Spike from chip.
 * It is comprised of its label, FPGA and chip time annotation.
 */
class GENPYBIND(visible) SpikeFromChip
{
public:
	/** Default constructor. */
	SpikeFromChip() SYMBOL_VISIBLE;

	/**
	 * Construct spike from chip via a label, FPGA and chip time information.
	 * @param label SpikeLabel to use
	 * @param fpga_time FPGATime to use
	 * @param chip_time ChipTime to use
	 */
	SpikeFromChip(SpikeLabel const& label, FPGATime const& fpga_time, ChipTime const& chip_time)
	    SYMBOL_VISIBLE;

	/**
	 * Construct a spike from chip from the data respresentation.
	 * @param data Data to use
	 */
	SpikeFromChip(fisch::vx::SpikeFromChipEvent const& data) SYMBOL_VISIBLE;

	/**
	 * Get spike label.
	 * @return SpikeLabel value
	 */
	GENPYBIND(getter_for(label))
	SpikeLabel get_label() const SYMBOL_VISIBLE;

	/**
	 * Set spike label.
	 * @param value SpikeLabel value to set
	 */
	GENPYBIND(setter_for(label))
	void set_label(SpikeLabel value) SYMBOL_VISIBLE;

	/**
	 * Get FPGA time.
	 * @return FPGATime value
	 */
	GENPYBIND(getter_for(fpga_time))
	FPGATime get_fpga_time() const SYMBOL_VISIBLE;

	/**
	 * Set FPGA time.
	 * @param value FPGATime value to set
	 */
	GENPYBIND(setter_for(fpga_time))
	void set_fpga_time(FPGATime value) SYMBOL_VISIBLE;

	/**
	 * Get chip time.
	 * @return ChipTime value
	 */
	GENPYBIND(getter_for(chip_time))
	ChipTime get_chip_time() const SYMBOL_VISIBLE;

	/**
	 * Set chip time.
	 * @param value ChipTime value to set
	 */
	GENPYBIND(setter_for(chip_time))
	void set_chip_time(ChipTime value) SYMBOL_VISIBLE;

	bool operator==(SpikeFromChip const& other) const SYMBOL_VISIBLE;
	bool operator!=(SpikeFromChip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SpikeFromChip const& spike) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	SpikeLabel m_label;
	FPGATime m_fpga_time;
	ChipTime m_chip_time;

public:
	friend struct detail::SpikeFromChipChecker;
	// packing of structs and pods seems different
	struct __attribute__((packed)) GENPYBIND(hidden) SpikeFromChipDType
	{
		uint16_t label;
		uint64_t fpga_time;
		uint64_t chip_time;
	};
};

namespace detail {

struct SpikeFromChipChecker {
	static_assert(sizeof(SpikeFromChip::SpikeFromChipDType::label) == sizeof(SpikeFromChip::m_label));
	static_assert(sizeof(SpikeFromChip::SpikeFromChipDType::fpga_time) == sizeof(SpikeFromChip::m_fpga_time));
	static_assert(sizeof(SpikeFromChip::SpikeFromChipDType::chip_time) == sizeof(SpikeFromChip::m_chip_time));
	static_assert(sizeof(SpikeFromChip::SpikeFromChipDType) == sizeof(SpikeFromChip));
	static_assert(offsetof(SpikeFromChip::SpikeFromChipDType, label)     == offsetof(SpikeFromChip, m_label));
	static_assert(offsetof(SpikeFromChip::SpikeFromChipDType, fpga_time) == offsetof(SpikeFromChip, m_fpga_time));
	static_assert(offsetof(SpikeFromChip::SpikeFromChipDType, chip_time) == offsetof(SpikeFromChip, m_chip_time));
};

} // namespace detail


namespace detail {
struct MADCSampleFromChipChecker;
}

/**
 * MADCSample from chip.
 * It is comprised of the sample value, FPGA and chip time annotation.
 */
class GENPYBIND(visible) MADCSampleFromChip
{
public:
	/** Sample value. */
	struct GENPYBIND(inline_base("*")) Value
	    : public halco::common::detail::RantWrapper<Value, uintmax_t, 0x3fff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) : rant_t(val) {}
	};

	/** Default constructor. */
	MADCSampleFromChip() SYMBOL_VISIBLE;

	/**
	 * Construct MADC sample from chip via a value, FPGA and chip time information.
	 * @param value sample value to use
	 * @param fpga_time FPGATime to use
	 * @param chip_time ChipTime to use
	 */
	MADCSampleFromChip(Value const& value, FPGATime const& fpga_time, ChipTime const& chip_time)
	    SYMBOL_VISIBLE;

	/**
	 * Construct an MADC sample from the data representation.
	 * @param data Data to use
	 */
	MADCSampleFromChip(fisch::vx::MADCSampleFromChipEvent const& data) SYMBOL_VISIBLE;

	/**
	 * Get sample value.
	 * @return Value value
	 */
	GENPYBIND(getter_for(value))
	Value get_value() const SYMBOL_VISIBLE;

	/**
	 * Set sample value.
	 * @param value Value sample value to set
	 */
	GENPYBIND(setter_for(value))
	void set_value(Value value) SYMBOL_VISIBLE;

	/**
	 * Get FPGA time.
	 * @return FPGATime value
	 */
	GENPYBIND(getter_for(fpga_time))
	FPGATime get_fpga_time() const SYMBOL_VISIBLE;

	/**
	 * Set FPGA time.
	 * @param value FPGATime value to set
	 */
	GENPYBIND(setter_for(fpga_time))
	void set_fpga_time(FPGATime value) SYMBOL_VISIBLE;

	/**
	 * Get chip time.
	 * @return ChipTime value
	 */
	GENPYBIND(getter_for(chip_time))
	ChipTime get_chip_time() const SYMBOL_VISIBLE;

	/**
	 * Set chip time.
	 * @param value ChipTime value to set
	 */
	GENPYBIND(setter_for(chip_time))
	void set_chip_time(ChipTime value) SYMBOL_VISIBLE;

	bool operator==(MADCSampleFromChip const& other) const SYMBOL_VISIBLE;
	bool operator!=(MADCSampleFromChip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MADCSampleFromChip const& sample)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	Value m_value;
	FPGATime m_fpga_time;
	ChipTime m_chip_time;

public:
	friend struct detail::MADCSampleFromChipChecker;
	// packing of structs and pods seems different
	struct __attribute__((packed)) GENPYBIND(hidden) MADCSampleFromChipDType
	{
		uintmax_t value;
		uint64_t fpga_time;
		uint64_t chip_time;
	};
};

namespace detail {

struct MADCSampleFromChipChecker
{
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType::value) ==
	    sizeof(MADCSampleFromChip::m_value));
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType::fpga_time) ==
	    sizeof(MADCSampleFromChip::m_fpga_time));
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType::chip_time) ==
	    sizeof(MADCSampleFromChip::m_chip_time));
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType) == sizeof(MADCSampleFromChip));
	static_assert(
	    offsetof(MADCSampleFromChip::MADCSampleFromChipDType, value) ==
	    offsetof(MADCSampleFromChip, m_value));
	static_assert(
	    offsetof(MADCSampleFromChip::MADCSampleFromChipDType, fpga_time) ==
	    offsetof(MADCSampleFromChip, m_fpga_time));
	static_assert(
	    offsetof(MADCSampleFromChip::MADCSampleFromChipDType, chip_time) ==
	    offsetof(MADCSampleFromChip, m_chip_time));
};

} // namespace detail


/**
 * Highspeed-Link notification from chip.
 */
class GENPYBIND(visible) HighspeedLinkNotification
{
public:
	/** Default constructor. */
	HighspeedLinkNotification() = default;

	/**
	 * Construct a HighspeedLinkNotification from the data representation.
	 * @param data Data to use
	 */
	HighspeedLinkNotification(fisch::vx::HighspeedLinkNotification const& data) SYMBOL_VISIBLE;

	/**
	 * Get sample value.
	 * @return Value value
	 */
	GENPYBIND(getter_for(phy))
	halco::hicann_dls::vx::PhyStatusOnFPGA get_phy() const SYMBOL_VISIBLE;

	/**
	 * Set sample value.
	 * @param value Value sample value to set
	 */
	GENPYBIND(setter_for(phy))
	void set_phy(halco::hicann_dls::vx::PhyStatusOnFPGA value) SYMBOL_VISIBLE;

	/**
	 * Get if link is up.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(link_up))
	bool get_link_up() const SYMBOL_VISIBLE;

	/**
	 * Set if link is up.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(link_up))
	void set_link_up(bool value) SYMBOL_VISIBLE;

	/**
	 * Get if decode error occured.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(decode_error))
	bool get_decode_error() const SYMBOL_VISIBLE;

	/**
	 * Set if decode error occured.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(decode_error))
	void set_decode_error(bool value) SYMBOL_VISIBLE;

	/**
	 * Get if crc error occured.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(crc_error))
	bool get_crc_error() const SYMBOL_VISIBLE;

	/**
	 * Set if crc error occured.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(crc_error))
	void set_crc_error(bool value) SYMBOL_VISIBLE;

	/**
	 * Get if crc is recovered.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(crc_recover))
	bool get_crc_recover() const SYMBOL_VISIBLE;

	/**
	 * Set if crc is recovered.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(crc_recover))
	void set_crc_recover(bool value) SYMBOL_VISIBLE;

	/**
	 * Get if check error occured.
	 * @return Boolean value
	 */
	GENPYBIND(getter_for(check_error))
	bool get_check_error() const SYMBOL_VISIBLE;

	/**
	 * Set if check error occured.
	 * @param value Boolean value to set
	 */
	GENPYBIND(setter_for(check_error))
	void set_check_error(bool value) SYMBOL_VISIBLE;

	/**
	 * Get FPGA time.
	 * @return FPGATime value
	 */
	GENPYBIND(getter_for(fpga_time))
	FPGATime get_fpga_time() const SYMBOL_VISIBLE;

	/**
	 * Set FPGA time.
	 * @param value FPGATime value to set
	 */
	GENPYBIND(setter_for(fpga_time))
	void set_fpga_time(FPGATime value) SYMBOL_VISIBLE;

	bool operator==(HighspeedLinkNotification const& other) const SYMBOL_VISIBLE;
	bool operator!=(HighspeedLinkNotification const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, HighspeedLinkNotification const& sample)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

	halco::hicann_dls::vx::PhyStatusOnFPGA m_phy{};
	bool m_link_up{false};
	bool m_decode_error{false};
	bool m_crc_error{false};
	bool m_crc_recover{false};
	bool m_check_error{false};
	FPGATime m_fpga_time{};
};

} // namespace haldls::vx

namespace std {
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::SpikeLabel)
} // namespace std
