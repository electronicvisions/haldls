#pragma once
#include "fisch/vx/event.h"
#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/event.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/neuron.h"
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

	/** Configurable synapse address, bits 0-5. */
	GENPYBIND(getter_for(synapse_label))
	SynapseQuad::Label get_synapse_label() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(synapse_label))
	void set_synapse_label(SynapseQuad::Label value) SYMBOL_VISIBLE;
};

typedef fisch::vx::MADCSampleFromChip MADCSampleFromChip GENPYBIND(opaque(false));
typedef fisch::vx::MADCSampleFromChipEvent MADCSampleFromChipEvent GENPYBIND(opaque(false));

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
	 * Construct spike from chip via a label, FPGa and chip time information.
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

} // namespace haldls::vx

namespace std {
HALCO_GEOMETRY_HASH_CLASS(haldls::vx::SpikeLabel)
} // namespace std
