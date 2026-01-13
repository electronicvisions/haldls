#pragma once
#include "hate/visibility.h"
#include "hxcomm/common/hwdb_entry.h"
#include "stadls/vx/v3/playback_generator.h"
#include "stadls/vx/v3/playback_program_builder.h"

namespace stadls::vx {
namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

class SystemInit;


class GENPYBIND(expose_as(_ASICAdapterBoardInit)) SYMBOL_VISIBLE ASICAdapterBoardInit
{
public:
	/** Default constructor. */
	ASICAdapterBoardInit() = default;

	virtual ~ASICAdapterBoardInit();

	/** Builder typedef (e.g. for usage in generators). */
	typedef v3::PlaybackProgramBuilder Builder;

	typedef hate::Nil Result;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const;

	/**
	 * Copy derived type.
	 */
	virtual std::unique_ptr<ASICAdapterBoardInit> copy() const;

	friend class SystemInit;

private:
	friend auto stadls::vx::generate<ASICAdapterBoardInit>(ASICAdapterBoardInit const&);
};

class GENPYBIND(expose_as(_CubeASICAdapterBoardInit)) SYMBOL_VISIBLE CubeASICAdapterBoardInit
    : public ASICAdapterBoardInit
{
public:
	/** Default constructor. */
	CubeASICAdapterBoardInit();

	/** Shift register on xBoard config. */
	haldls::vx::ShiftRegister shift_register;

	/** DAC control config on xBoard. */
	lola::vx::DACControlBlock dac_control_block;

	/** DAC channel configs on xBoard. */
	lola::vx::DACChannelBlock dac_channel_block;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const override;

	/**
	 * Copy derived type.
	 */
	virtual std::unique_ptr<ASICAdapterBoardInit> copy() const override;

private:
	friend auto stadls::vx::generate<CubeASICAdapterBoardInit>(CubeASICAdapterBoardInit const&);
};


class GENPYBIND(expose_as(_JboaASICAdapterBoardInit)) SYMBOL_VISIBLE JboaASICAdapterBoardInit
    : public ASICAdapterBoardInit
{
public:
	/** Default constructor. */
	JboaASICAdapterBoardInit();

	/** Multiplexer config on jboa. */
	haldls::vx::TCA9546ChannelConfig multiplexer;

	/** General purpose input/output expander config on jboa. */
	haldls::vx::TCA9554Config io_expander;

	/** Digital potentiometers config on jboa. */
	typedef halco::common::typed_array<
	    haldls::vx::AD5252ChannelConfig,
	    halco::hicann_dls::vx::AD5252ChannelConfigOnBoard>
	    DigitalPotiConfigArray GENPYBIND(opaque(false));
	DigitalPotiConfigArray DigitalPotiArray;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const override;

	/**
	 * Copy derived type.
	 */
	virtual std::unique_ptr<ASICAdapterBoardInit> copy() const override;

private:
	friend auto stadls::vx::generate<JboaASICAdapterBoardInit>(JboaASICAdapterBoardInit const&);
};


class SYMBOL_VISIBLE GENPYBIND(expose_as("_ChipInit")) ChipInit
{
public:
	ChipInit(hxcomm::HwdbEntry const& hwdb_entry) SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef v3::PlaybackProgramBuilder Builder;

	/** JTAG clock scaler setting. */
	haldls::vx::JTAGClockScaler jtag_clock_scaler;

	/** PLL clock output block setting. */
	haldls::vx::v3::PLLClockOutputBlock pll_clock_output_block;

	/** ADPLL setting. */
	typedef halco::common::typed_array<haldls::vx::ADPLL, halco::hicann_dls::vx::v3::ADPLLOnDLS>
	    adplls_type GENPYBIND(opaque(false));
	adplls_type adplls;

	struct GENPYBIND(visible) HighspeedLink
	{
		/** Default constructor. */
		HighspeedLink() SYMBOL_VISIBLE;

		/** Highspeed-link PHY enable values for the FPGA side. */
		haldls::vx::CommonPhyConfigFPGA common_phy_config_fpga;

		/** Highspeed-link PHY enable values for the chip side. */
		haldls::vx::CommonPhyConfigChip common_phy_config_chip;

		/** Highspeed-link PHY settings for the FPGA side. */
		typedef halco::common::
		    typed_array<haldls::vx::PhyConfigFPGA, halco::hicann_dls::vx::v3::PhyConfigFPGAOnDLS>
		        phy_configs_fpga_type GENPYBIND(opaque(false));
		phy_configs_fpga_type phy_configs_fpga;

		/** Highspeed-link PHY settings for the chip side. */
		typedef halco::common::
		    typed_array<haldls::vx::PhyConfigChip, halco::hicann_dls::vx::v3::PhyConfigChipOnDLS>
		        phy_configs_chip_type GENPYBIND(opaque(false));
		phy_configs_chip_type phy_configs_chip;

		/** Systime synchronization base value. */
		haldls::vx::SystimeSyncBase systime_sync_base;

		/**
		 * Enable systime synchronization.
		 * @note Disabling this discards all systime related configuration, i.e. setting the
		 * SystimeSyncBase and initiating the time hand-shake between the chip and the FPGA.
		 * This leads to no time annotation being present in the response stream.
		 */
		bool enable_systime;

	} highspeed_link;

	/**
	 * Enable Highspeed connection.
	 * @note Disabling this discards all highspeed related configuration.
	 */
	bool enable_highspeed_link;

	/**
	 * Event recording config, defaults to active recording.
	 */
	haldls::vx::EventRecordingConfig event_recording;

	/** Memory timing settings. */
	lola::vx::v3::MemoryTiming memory_timing;

	/**
	 * Select whether the CapMem should be enabled.
	 * @note The CapMem initialization requires highspeed access.
	 */
	bool enable_capmem;

	/** Generate usable reference current for the CapMem. */
	haldls::vx::v3::ReferenceGeneratorConfig reference_generator_config;

	typedef hate::Nil Result;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<ChipInit>(ChipInit const&);

	/**
	 * Extract the synram timing from a hwdb entry and fill @ref ChipInit::memory_timing.
	 */
	void fill_synram_timing(hxcomm::HwdbEntry const& hwdb_entry);
};


class SYMBOL_VISIBLE GENPYBIND(expose_as("_SystemInit")) SystemInit
{
public:
	SystemInit(hxcomm::HwdbEntry const& hwdb_entry) SYMBOL_VISIBLE;

	// needed because of unique_ptr, for which contained type can be copied
	SystemInit(SystemInit const& other) SYMBOL_VISIBLE;
	SystemInit(SystemInit&& other) = default;
	SystemInit& operator=(SystemInit const& other) SYMBOL_VISIBLE;
	SystemInit& operator=(SystemInit&& other) = default;

	/** Builder typedef (e.g. for usage in generators). */
	typedef v3::PlaybackProgramBuilder Builder;

	/** Instruction timeout duration on FPGA config. */
	haldls::vx::InstructionTimeoutConfig instruction_timeout;

	GENPYBIND(getter_for(asic_adapter_board), return_value_policy(reference_internal))
	ASICAdapterBoardInit const& get_asic_adapter_board() const SYMBOL_VISIBLE;

	GENPYBIND(setter_for(asic_adapter_board))
	void set_asic_adapter_board(ASICAdapterBoardInit const& value) SYMBOL_VISIBLE;

	bool enable_asic_adapter_board;

	ChipInit chip;
	bool enable_chip;

	typedef hate::Nil Result;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SystemInit const& sequence) SYMBOL_VISIBLE;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<SystemInit>(SystemInit const&);

	std::unique_ptr<ASICAdapterBoardInit> m_asic_adapter_board;
};


#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
namespace detail {

struct GENPYBIND(expose_as(ASICAdapterBoardInit)) PyASICAdapterBoardInit
    : public ASICAdapterBoardInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const
	{
		return stadls::vx::detail::py_generate_impl(
		    static_cast<ASICAdapterBoardInit const&>(*this));
	}
};

struct GENPYBIND(expose_as(CubeASICAdapterBoardInit)) PyCubeASICAdapterBoardInit
    : public CubeASICAdapterBoardInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const
	{
		return stadls::vx::detail::py_generate_impl(
		    static_cast<CubeASICAdapterBoardInit const&>(*this));
	}
};

struct GENPYBIND(expose_as(JboaASICAdapterBoardInit)) PyJboaASICAdapterBoardInit
    : public JboaASICAdapterBoardInit
    , public PlaybackGenerator
{
	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const
	{
		return stadls::vx::detail::py_generate_impl(
		    static_cast<JboaASICAdapterBoardInit const&>(*this));
	}
};

struct GENPYBIND(expose_as(ChipInit)) PyChipInit
    : public ChipInit
    , public PlaybackGenerator
{
	PyChipInit(hxcomm::HwdbEntry const& hwdb_entry) : ChipInit(hwdb_entry) {}

	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<ChipInit const&>(*this));
	}
};

// TODO: we can't use the alias above, cf. https://github.com/kljohann/genpybind/issues/32
struct GENPYBIND(expose_as(SystemInit)) PySystemInit
    : public SystemInit
    , public PlaybackGenerator
{
	PySystemInit(hxcomm::HwdbEntry const& hwdb_entry) : SystemInit(hwdb_entry) {}

	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<SystemInit const&>(*this));
	}
};

} // namespace detail
#endif // defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::ASICAdapterBoardInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::CubeASICAdapterBoardInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::JboaASICAdapterBoardInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::ChipInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::SystemInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

} // namespace v3
} // namespace stadls::vx
