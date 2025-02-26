#pragma once
#include "halco/hicann-dls/vx/memory_timing.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

#include "haldls/vx/cadc.h"
#include "haldls/vx/container.h"
#include "haldls/vx/neuron.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/synapse_driver.h"

namespace lola { namespace vx GENPYBIND_TAG_LOLA_VX {

/**
 * Timing configuration of memories.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) MemoryTiming
    : public haldls::vx::ContainerBase<MemoryTiming>
{
public:
	typedef halco::hicann_dls::vx::MemoryTimingOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	MemoryTiming() = default;

	typedef halco::common::typed_array<
	    haldls::vx::CADCOffsetSRAMTimingConfig,
	    haldls::vx::CADCOffsetSRAMTimingConfig::coordinate_type>
	    CADCOffset GENPYBIND(opaque);
	typedef halco::common::typed_array<
	    haldls::vx::SynapseDriverSRAMTimingConfig,
	    haldls::vx::SynapseDriverSRAMTimingConfig::coordinate_type>
	    SynapseDriver GENPYBIND(opaque);
	typedef halco::common::typed_array<
	    haldls::vx::NeuronSRAMTimingConfig,
	    haldls::vx::NeuronSRAMTimingConfig::coordinate_type>
	    Neuron GENPYBIND(opaque);
	typedef halco::common::typed_array<
	    haldls::vx::NeuronBackendSRAMTimingConfig,
	    haldls::vx::NeuronBackendSRAMTimingConfig::coordinate_type>
	    NeuronBackend GENPYBIND(opaque);
	typedef halco::common::
	    typed_array<haldls::vx::CommonSynramConfig, haldls::vx::CommonSynramConfig::coordinate_type>
	        Synram GENPYBIND(opaque);

	/**
	 * Memory timing of SRAM of CADC offsets.
	 */
	CADCOffset cadc_offset;
	/**
	 * Memory timing of SRAM of synapse drivers.
	 */
	SynapseDriver synapse_driver;
	/**
	 * Memory timing of SRAM of neurons.
	 */
	Neuron neuron;
	/**
	 * Memory timing of SRAM of neuron backends.
	 */
	NeuronBackend neuron_backend;
	/**
	 * Memory timing of SRAM of synram.
	 */
	Synram synram;

	bool operator==(MemoryTiming const& other) const SYMBOL_VISIBLE;
	bool operator!=(MemoryTiming const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MemoryTiming const& config) SYMBOL_VISIBLE;

private:
	friend haldls::vx::detail::VisitPreorderImpl<MemoryTiming>;
};

} // namespace vx
} // namespace lola

#include "lola/vx/memory_timing.tcc"
