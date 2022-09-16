#pragma once
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/event.h"
#ifndef __ppu__
#include "fisch/vx/event.h"
#include "halco/hicann-dls/vx/highspeed_link.h"
#include "halco/hicann-dls/vx/readout.h"
#endif
#include "haldls/cerealization.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/neuron.h"
#include "haldls/vx/padi.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

#define SpikePackToChip(Num)                                                                       \
	class GENPYBIND(visible) SpikePack##Num##ToChip                                                \
	{                                                                                              \
	public:                                                                                        \
		typedef halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS coordinate_type;                \
		typedef std::true_type is_leaf_node;                                                       \
                                                                                                   \
		typedef std::array<halco::hicann_dls::vx::SpikeLabel, Num> labels_type;                    \
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
		constexpr static size_t GENPYBIND(hidden) write_config_size_in_words = 1;                  \
		constexpr static size_t GENPYBIND(hidden) read_config_size_in_words = 0;                   \
		static std::                                                                               \
		    array<halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS, write_config_size_in_words>  \
		    write_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);         \
		static std::                                                                               \
		    array<halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS, read_config_size_in_words>   \
		    read_addresses(coordinate_type const& word) SYMBOL_VISIBLE GENPYBIND(hidden);          \
		std::                                                                                      \
		    array<fisch::vx::word_access_type::SpikePack##Num##ToChip, write_config_size_in_words> \
		    encode() const SYMBOL_VISIBLE GENPYBIND(hidden);                                       \
		void decode(std::array<                                                                    \
		            fisch::vx::word_access_type::SpikePack##Num##ToChip,                           \
		            read_config_size_in_words> const& data) SYMBOL_VISIBLE GENPYBIND(hidden);      \
                                                                                                   \
	private:                                                                                       \
		friend class cereal::access;                                                               \
		template <class Archive>                                                                   \
		void serialize(Archive& ar, std::uint32_t const version) SYMBOL_VISIBLE;                   \
                                                                                                   \
		labels_type m_impl;                                                                        \
	};                                                                                             \
                                                                                                   \
	EXTERN_INSTANTIATE_CEREAL_SERIALIZE(SpikePack##Num##ToChip)                                    \
                                                                                                   \
	namespace detail {                                                                             \
                                                                                                   \
	template <>                                                                                    \
	struct BackendContainerTrait<SpikePack##Num##ToChip>                                           \
	    : public BackendContainerBase<                                                             \
	          SpikePack##Num##ToChip,                                                              \
	          fisch::vx::word_access_type::SpikePack##Num##ToChip>                                 \
	{};                                                                                            \
                                                                                                   \
	} // namespace detail

// clang-format off
SpikePackToChip(1)
SpikePackToChip(2)
SpikePackToChip(3)
// clang-format on

#undef SpikePackToChip


#ifndef __ppu__
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
	SpikeFromChip() = default;

	/**
	 * Construct spike from chip via a label, FPGA and chip time information.
	 * @param label SpikeLabel to use
	 * @param fpga_time FPGATime to use
	 * @param chip_time ChipTime to use
	 */
	SpikeFromChip(
	    halco::hicann_dls::vx::SpikeLabel const& label,
	    FPGATime const& fpga_time,
	    ChipTime const& chip_time) :
	    label(label), fpga_time(fpga_time), chip_time(chip_time)
	{}

	/**
	 * Construct a spike from chip from the data respresentation.
	 * @param data Data to use
	 */
	SpikeFromChip(fisch::vx::SpikeFromChip const& data) :
	    label(data.label), fpga_time(data.fpga_time), chip_time(data.chip_time)
	{}

	/**
	 * Spike label.
	 */
	halco::hicann_dls::vx::SpikeLabel label;

	/**
	 * FPGA time.
	 */
	FPGATime fpga_time;

	/**
	 * Chip time.
	 */
	ChipTime chip_time;

	bool operator==(SpikeFromChip const& other) const SYMBOL_VISIBLE;
	bool operator!=(SpikeFromChip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SpikeFromChip const& spike) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

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

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(SpikeFromChip)

namespace detail {

struct SpikeFromChipChecker {
	static_assert(sizeof(SpikeFromChip::SpikeFromChipDType::label) == sizeof(SpikeFromChip::label));
	static_assert(
	    sizeof(SpikeFromChip::SpikeFromChipDType::fpga_time) == sizeof(SpikeFromChip::fpga_time));
	static_assert(
	    sizeof(SpikeFromChip::SpikeFromChipDType::chip_time) == sizeof(SpikeFromChip::chip_time));
	static_assert(sizeof(SpikeFromChip::SpikeFromChipDType) == sizeof(SpikeFromChip));
	static_assert(
	    offsetof(SpikeFromChip::SpikeFromChipDType, label) == offsetof(SpikeFromChip, label));
	static_assert(
	    offsetof(SpikeFromChip::SpikeFromChipDType, fpga_time) ==
	    offsetof(SpikeFromChip, fpga_time));
	static_assert(
	    offsetof(SpikeFromChip::SpikeFromChipDType, chip_time) ==
	    offsetof(SpikeFromChip, chip_time));
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
	    : public halco::common::detail::RantWrapper<Value, uint16_t, 0x3ff, 0>
	{
		constexpr explicit Value(uintmax_t const val = 0) : rant_t(val) {}
	};

	/** Channel selected on the active multiplexer for which this sample was acquired. */
	typedef halco::hicann_dls::vx::SourceMultiplexerOnReadoutSourceSelection Channel
	    GENPYBIND(opaque(false));

	/** Default constructor. */
	MADCSampleFromChip() = default;

	/**
	 * Construct MADC sample from chip via a value, FPGA and chip time information.
	 * @param value sample value to use
	 * @param channel channel from which the value was sampled
	 * @param fpga_time FPGATime to use
	 * @param chip_time ChipTime to use
	 */
	MADCSampleFromChip(
	    Value const& value,
	    Channel const& channel,
	    FPGATime const& fpga_time,
	    ChipTime const& chip_time) :
	    value(value), channel(channel), fpga_time(fpga_time), chip_time(chip_time)
	{}

	/**
	 * Construct an MADC sample from the data representation.
	 * @param data Data to use
	 */
	MADCSampleFromChip(fisch::vx::MADCSampleFromChip const& data) :
	    value(data.value),
	    channel(data.channel),
	    fpga_time(data.fpga_time),
	    chip_time(data.chip_time)
	{}

	/**
	 * Sample value.
	 */
	Value value;

	/**
	 * Channel from which this sample was obtained.
	 */
	Channel channel;

	/**
	 * FPGA time.
	 */
	FPGATime fpga_time;

	/**
	 * Chip time.
	 */
	ChipTime chip_time;

	bool operator==(MADCSampleFromChip const& other) const SYMBOL_VISIBLE;
	bool operator!=(MADCSampleFromChip const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MADCSampleFromChip const& sample)
	    SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t const version);

public:
	friend struct detail::MADCSampleFromChipChecker;
	// packing of structs and pods seems different
	struct __attribute__((packed)) GENPYBIND(hidden) MADCSampleFromChipDType
	{
		uint16_t value;
		uint8_t channel;
		uint64_t fpga_time;
		uint64_t chip_time;
	};
};

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(MADCSampleFromChip)

namespace detail {

struct MADCSampleFromChipChecker
{
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType::value) ==
	    sizeof(MADCSampleFromChip::value));
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType::channel) ==
	    sizeof(MADCSampleFromChip::channel));
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType::fpga_time) ==
	    sizeof(MADCSampleFromChip::fpga_time));
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType::chip_time) ==
	    sizeof(MADCSampleFromChip::chip_time));
	static_assert(
	    sizeof(MADCSampleFromChip::MADCSampleFromChipDType) == sizeof(MADCSampleFromChip));
	static_assert(
	    offsetof(MADCSampleFromChip::MADCSampleFromChipDType, value) ==
	    offsetof(MADCSampleFromChip, value));
	static_assert(
	    offsetof(MADCSampleFromChip::MADCSampleFromChipDType, channel) ==
	    offsetof(MADCSampleFromChip, channel));
	static_assert(
	    offsetof(MADCSampleFromChip::MADCSampleFromChipDType, fpga_time) ==
	    offsetof(MADCSampleFromChip, fpga_time));
	static_assert(
	    offsetof(MADCSampleFromChip::MADCSampleFromChipDType, chip_time) ==
	    offsetof(MADCSampleFromChip, chip_time));
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

EXTERN_INSTANTIATE_CEREAL_SERIALIZE(HighspeedLinkNotification)
#endif

} // namespace haldls::vx
