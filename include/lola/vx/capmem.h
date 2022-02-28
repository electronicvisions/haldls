#pragma once

#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#define ID(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
// clang-format off
#define CONCAT(w, x, y, z) STR(ID(w)ID(/)ID(x)ID(/)ID(v)ID(y)ID(/)ID(z))
#include CONCAT(haldls,vx,CHIP_REVISION,capmem.h)
#include CONCAT(haldls,vx,CHIP_REVISION,synapse.h)
// clang-format on
#undef ID
#undef XSTR
#undef STR
#undef CONCAT

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/vx/capmem.h"
#include "haldls/cerealization.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"

#if CHIP_REVISION == 1
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V1
#define CHIP_REVISION_STR v1
#elif CHIP_REVISION == 2
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V2
#define CHIP_REVISION_STR v2
#elif CHIP_REVISION == 3
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR GENPYBIND_TAG_LOLA_VX_VY {
#undef GENPYBIND_TAG_LOLA_VX_VY

/**
 * Configuration of the CapMem and associated selections as well as the reference generator.
 */
class GENPYBIND(visible) CapMem
{
public:
	typedef halco::hicann_dls::vx::CapMemOnDLS coordinate_type;
	typedef std::false_type has_local_data;

	CapMem() = default;

	typedef halco::common::typed_array<
	    haldls::vx::CHIP_REVISION_STR::CapMemBlockConfig,
	    halco::hicann_dls::vx::CHIP_REVISION_STR::CapMemBlockConfigOnDLS>
	    Blocks GENPYBIND(opaque(false));

	/**
	 * CapMem configuration per quadrant.
	 */
	Blocks blocks;

	/**
	 * Synapse bias selection.
	 */
	haldls::vx::CHIP_REVISION_STR::SynapseBiasSelection synapse_bias_selection;

	/**
	 * Reference generator.
	 */
	haldls::vx::CHIP_REVISION_STR::ReferenceGeneratorConfig reference_generator;

	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::Value AnalogValue GENPYBIND(visible);
	typedef haldls::vx::CHIP_REVISION_STR::CapMemCell::DisableRefresh AnalogDisableRefresh
	    GENPYBIND(visible);
	// TODO (Issue 3622): Expose in Python
	typedef halco::common::typed_array<
	    haldls::vx::CHIP_REVISION_STR::CapMemCell::value_type,
	    halco::hicann_dls::vx::CHIP_REVISION_STR::UnusedCapMemCellOnDLS>
	    UnusedCells GENPYBIND(hidden);

	/**
	 * Unused CapMem cells.
	 */
	UnusedCells unused_cells;

	bool operator==(CapMem const& other) const SYMBOL_VISIBLE;
	bool operator!=(CapMem const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CapMem const& config) SYMBOL_VISIBLE;

private:
	friend haldls::vx::detail::VisitPreorderImpl<CapMem>;
};

}

#include "lola/vx/capmem.tcc"
