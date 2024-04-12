#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/readout_chain.h"

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#elif CHIP_REVISION == 4
#define CHIP_REVISION_STR v4
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

ReadoutChain::BufferToPadT::BufferToPadT() : enable(false), amp_i_bias() {}

bool ReadoutChain::BufferToPadT::operator==(BufferToPadT const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::BufferToPadT::operator!=(BufferToPadT const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::BufferToPadT const& config)
{
	return print(os, config);
}

bool ReadoutChain::DynamicMux::operator==(DynamicMux const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::DynamicMux::operator!=(DynamicMux const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::DynamicMux const& config)
{
	return print(os, config);
}

bool ReadoutChain::PseudoDifferentialConverter::operator==(
    PseudoDifferentialConverter const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::PseudoDifferentialConverter::operator!=(
    PseudoDifferentialConverter const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::PseudoDifferentialConverter const& config)
{
	return print(os, config);
}

ReadoutChain::MADCInputCrossbar::MADCInputCrossbar() :
    connect_source_measure_unit(false),
    connect_dynamic_mux(true),
    connect_pad_mux(false),
    connect_madc_preamp(true)
{}

bool ReadoutChain::MADCInputCrossbar::operator==(MADCInputCrossbar const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADCInputCrossbar::operator!=(MADCInputCrossbar const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADCInputCrossbar const& config)
{
	return print(os, config);
}

bool ReadoutChain::MADCPreamp::operator==(MADCPreamp const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADCPreamp::operator!=(MADCPreamp const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADCPreamp const& config)
{
	return print(os, config);
}

ReadoutChain::MADCDebugCrossbar::MADCDebugCrossbar() :
    connect_preamp_to_madc(true), connect_pads_to_madc(false), connect_preamp_to_pads(false)
{}

bool ReadoutChain::MADCDebugCrossbar::operator==(MADCDebugCrossbar const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADCDebugCrossbar::operator!=(MADCDebugCrossbar const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADCDebugCrossbar const& config)
{
	return print(os, config);
}

bool ReadoutChain::MADC::operator==(MADC const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::MADC::operator!=(MADC const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::MADC const& config)
{
	return print(os, config);
}

bool ReadoutChain::SourceMeasureUnit::operator==(SourceMeasureUnit const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::SourceMeasureUnit::operator!=(SourceMeasureUnit const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::SourceMeasureUnit const& config)
{
	return print(os, config);
}

ReadoutChain::CurrentDAC::CurrentDAC() :
    enable_current(false),
    current(),
    sign(),
    connect_neuron_stimulus({false, false}),
    connect_synapse_debug({false, false}),
    i_bias(),
    i_bias_casc()
{}

bool ReadoutChain::CurrentDAC::operator==(CurrentDAC const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::CurrentDAC::operator!=(CurrentDAC const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain::CurrentDAC const& config)
{
	return print(os, config);
}

bool ReadoutChain::operator==(ReadoutChain const& other) const
{
	return equal(*this, other);
}

bool ReadoutChain::operator!=(ReadoutChain const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ReadoutChain const& config)
{
	return print(os, config);
}

} // lola::vx::CHIP_REVISION

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::ReadoutChain)
