#pragma once
#include "hate/visibility.h"
#include "hxcomm/common/hwdb_entry.h"
#include "stadls/vx/v3/playback_generator.h"
#include "stadls/vx/v3/playback_program_builder.h"

namespace stadls::vx {
namespace v3 GENPYBIND_TAG_STADLS_VX_V3 {

namespace detail {

class InitGenerator;

} // namespace detail

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

	friend class detail::InitGenerator;

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
	/** Default constructor. */
	ChipInit() SYMBOL_VISIBLE;

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

	/** Select internal bias currents for synapses. */
	haldls::vx::SynapseBiasSelection synapse_bias_selection;

	/**
	 * Select whether the CapMem should be enabled.
	 * @note The CapMem initialization requires highspeed access.
	 */
	bool enable_capmem;

	/** Generate usable reference current for the CapMem. */
	haldls::vx::v3::ReferenceGeneratorConfig reference_generator_config;

	/** Initialize the CapMem with usable default values. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::CapMemBlockConfig,
	    halco::hicann_dls::vx::v3::CapMemBlockConfigOnDLS>
	    capmem_block_config_type GENPYBIND(visible);
	capmem_block_config_type capmem_block_config;

	/** Set initial CapMem config.
	 * By default, a value of zero is written to all cells. */
	typedef halco::common::
	    typed_array<haldls::vx::v3::CapMemBlock, halco::hicann_dls::vx::v3::CapMemBlockOnDLS>
	        capmem_block_type GENPYBIND(opaque(false));
	capmem_block_type capmem_config;

	typedef hate::Nil Result;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<ChipInit>(ChipInit const&);
};


namespace detail {

class SYMBOL_VISIBLE GENPYBIND(expose_as(_InitGenerator)) InitGenerator
{
public:
	InitGenerator(hxcomm::HwdbEntry const& hwdb_entry) SYMBOL_VISIBLE;

	// needed because of unique_ptr, for which contained type can be copied
	InitGenerator(InitGenerator const& other) SYMBOL_VISIBLE;
	InitGenerator(InitGenerator&& other) = default;
	InitGenerator& operator=(InitGenerator const& other) SYMBOL_VISIBLE;
	InitGenerator& operator=(InitGenerator&& other) = default;

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
	friend std::ostream& operator<<(std::ostream& os, InitGenerator const& sequence) SYMBOL_VISIBLE;

protected:
	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<InitGenerator>(InitGenerator const&);

	std::unique_ptr<ASICAdapterBoardInit> m_asic_adapter_board;
};

} // namespace detail

/**
 * Generator for initialization of the chip up to digital communication.
 *
 * Uses the default InitGenerator() to establish digital communication to the chip.
 */
class DigitalInit : public detail::InitGenerator
{
public:
	DigitalInit(hxcomm::HwdbEntry const& hwdb_entry) SYMBOL_VISIBLE;

private:
	friend auto stadls::vx::generate<DigitalInit>(DigitalInit const&);
};

/**
 * Generator for initialization required for typical experiments.
 * Uses the InitGenerator() to establish digital communication to the chip, and
 * further initializes the CapMem in a working state and selects internal bias currents for
 * synapses.
 */
class SYMBOL_VISIBLE ExperimentInit : public detail::InitGenerator
{
public:
	ExperimentInit(hxcomm::HwdbEntry const& hwdb_entry) SYMBOL_VISIBLE;

	/** Builder typedef (e.g. for usage in generators). */
	typedef PlaybackProgramBuilder Builder;

	/** Set common neuron backend with clocks enabled.
	 * If clocks are disabled, it may behave strangely. */
	typedef halco::common::typed_array<
	    haldls::vx::CommonNeuronBackendConfig,
	    halco::hicann_dls::vx::v3::CommonNeuronBackendConfigOnDLS>
	    common_neuron_backend_config_type GENPYBIND(visible);
	common_neuron_backend_config_type common_neuron_backend_config;

	/** Set ColumnCorrelationQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::ColumnCorrelationQuad,
	    halco::hicann_dls::vx::v3::ColumnCorrelationQuadOnDLS>
	    column_correlation_quad_type GENPYBIND(opaque(false));
	column_correlation_quad_type column_correlation_quad_config;

	/** Set ColumnCurrentQuad/Switch connections. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::ColumnCurrentQuad,
	    halco::hicann_dls::vx::v3::ColumnCurrentQuadOnDLS>
	    column_current_quad_type GENPYBIND(opaque(false));
	column_current_quad_type column_current_quad_config;

	/** Set common correlation config. */
	typedef halco::common::typed_array<
	    haldls::vx::v3::CommonCorrelationConfig,
	    halco::hicann_dls::vx::v3::CommonCorrelationConfigOnDLS>
	    common_correlation_config_type GENPYBIND(opaque(false));
	common_correlation_config_type common_correlation_config;

	typedef detail::InitGenerator::Result Result;

private:
	friend auto stadls::vx::generate<ExperimentInit>(ExperimentInit const&);

	/**
	 * Generate PlaybackProgramBuilder.
	 * @return PlaybackGeneratorReturn instance with sequence embodied and specified Result value
	 */
	virtual PlaybackGeneratorReturn<Result> generate() const override SYMBOL_VISIBLE;
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
	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<ChipInit const&>(*this));
	}
};

struct GENPYBIND(expose_as(ExperimentInit), inline_base("*ExperimentInit*")) PyExperimentInit
    : public ExperimentInit
    , public PlaybackGenerator
{
	PyExperimentInit(hxcomm::HwdbEntry const& hwdb_entry) : ExperimentInit(hwdb_entry) {}

	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<ExperimentInit const&>(*this));
	}
};

// TODO: we can't use the alias above, cf. https://github.com/kljohann/genpybind/issues/32
struct GENPYBIND(expose_as(DigitalInit), inline_base("*DigitalInit*")) PyDigitalInit
    : public DigitalInit
    , public PlaybackGenerator
{
	PyDigitalInit(hxcomm::HwdbEntry const& hwdb_entry) : DigitalInit(hwdb_entry) {}

	virtual pybind11::tuple GENPYBIND(expose_as("generate")) pygenerate() const override
	{
		return stadls::vx::detail::py_generate_impl(static_cast<DigitalInit const&>(*this));
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
	    [](::stadls::vx::v3::ExperimentInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

GENPYBIND_MANUAL({
	parent.def(
	    "generate",
	    [](::stadls::vx::v3::DigitalInit const& seq) {
		    return ::stadls::vx::detail::py_generate_impl(seq);
	    },
	    pybind11::return_value_policy::move);
})

} // namespace v3
} // namespace stadls::vx
