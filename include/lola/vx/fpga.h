#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/fpga_on_system.h"

#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/fpga.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include "lola/vx/routing_table.h"
#include <cereal/macros.hpp>

namespace fisch::vx {
class Omnibus;
} // namespace fisch::vx

namespace lola {
namespace vx GENPYBIND_TAG_LOLA_VX {

/**
 * Multichip-jboa-leaf FPGA connected to a single chip.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) MultichipJboaLeafFPGA
    : public haldls::vx::ContainerBase<MultichipJboaLeafFPGA>
{
public:
	typedef halco::hicann_dls::vx::FPGAOnChipAndMultichipJboaLeafFPGA coordinate_type;
	typedef std::false_type has_local_data;

	MultichipJboaLeafFPGA() SYMBOL_VISIBLE;

	bool operator==(MultichipJboaLeafFPGA const& other) const SYMBOL_VISIBLE;
	bool operator!=(MultichipJboaLeafFPGA const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MultichipJboaLeafFPGA const& config)
	    SYMBOL_VISIBLE;

	OutputRoutingTable output_routing_table;
	InputRoutingTable input_routing_table;

private:
	friend haldls::vx::detail::VisitPreorderImpl<MultichipJboaLeafFPGA>;
};


/**
 * FPGA connected to a single chip.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SinglechipFPGA
    : public haldls::vx::ContainerBase<SinglechipFPGA>
{
public:
	typedef halco::hicann_dls::vx::FPGAOnChipAndSinglechipFPGA coordinate_type;
	typedef std::false_type has_local_data;

	SinglechipFPGA() SYMBOL_VISIBLE;

	bool operator==(SinglechipFPGA const& other) const SYMBOL_VISIBLE;
	bool operator!=(SinglechipFPGA const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SinglechipFPGA const& config) SYMBOL_VISIBLE;

	using SpikeIOInputRoutes GENPYBIND(opaque(false)) = halco::common::typed_heap_array<
	    haldls::vx::SpikeIOInputRoute,
	    halco::hicann_dls::vx::SpikeIOInputRouteOnFPGA>;

	using SpikeIOOutputRoutes GENPYBIND(opaque(false)) = halco::common::typed_heap_array<
	    haldls::vx::SpikeIOOutputRoute,
	    halco::hicann_dls::vx::SpikeIOOutputRouteOnFPGA>;


	haldls::vx::SpikeIOConfig spikeio_config;
	SpikeIOInputRoutes spikeio_input_routes;
	SpikeIOOutputRoutes spikeio_output_routes;

private:
	friend haldls::vx::detail::VisitPreorderImpl<SinglechipFPGA>;
};
} // namespace vx
} // namespace lola

#include "lola/vx/fpga.tcc"
