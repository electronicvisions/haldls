#pragma once
#include <array>
#include <iosfwd>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/system.h"

#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "hate/visibility.h"
#include "lola/vx/fpga.h"
#include "lola/vx/genpybind.h"
#include "lola/vx/v3/chip.h"
#include <cereal/macros.hpp>


namespace fisch::vx {
class Omnibus;
} // namespace fisch::vx

namespace lola {
namespace vx GENPYBIND_TAG_LOLA_VX {

/**
 System container with a single chip and a multichip-jboa-leaf FPGA.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ChipAndMultichipJboaLeafFPGA
    : public haldls::vx::ContainerBase<ChipAndMultichipJboaLeafFPGA>
{
public:
	typedef halco::hicann_dls::vx::ChipAndMultichipJboaLeafFPGAOnSystem coordinate_type;
	typedef std::false_type has_local_data;

	ChipAndMultichipJboaLeafFPGA() SYMBOL_VISIBLE;

	bool operator==(ChipAndMultichipJboaLeafFPGA const& other) const SYMBOL_VISIBLE;
	bool operator!=(ChipAndMultichipJboaLeafFPGA const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ChipAndMultichipJboaLeafFPGA const& config)
	    SYMBOL_VISIBLE;

	using FPGA = MultichipJboaLeafFPGA;
	using Chip = v3::Chip;

	FPGA fpga;
	Chip chip;

private:
	friend haldls::vx::detail::VisitPreorderImpl<ChipAndMultichipJboaLeafFPGA>;
};


/**
 System container with a single chip and a Singlechip FPGA.
 */
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ChipAndSinglechipFPGA
    : public haldls::vx::ContainerBase<ChipAndSinglechipFPGA>
{
public:
	typedef halco::hicann_dls::vx::ChipAndSinglechipFPGAOnSystem coordinate_type;
	typedef std::false_type has_local_data;

	ChipAndSinglechipFPGA() SYMBOL_VISIBLE;

	bool operator==(ChipAndSinglechipFPGA const& other) const SYMBOL_VISIBLE;
	bool operator!=(ChipAndSinglechipFPGA const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ChipAndSinglechipFPGA const& config)
	    SYMBOL_VISIBLE;

	using FPGA = SinglechipFPGA;
	using Chip = v3::Chip;

	FPGA fpga;
	Chip chip;

private:
	friend haldls::vx::detail::VisitPreorderImpl<ChipAndSinglechipFPGA>;
};

} // namespace vx
} // namespace lola

#include "lola/vx/system.tcc"