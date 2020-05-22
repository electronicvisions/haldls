#pragma once
#include <array>
#include <ostream>

#include "halco/common/geometry.h"
#include "halco/hicann-dls/vx/synapse_driver.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/sram_controller.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"

namespace cereal {
class access;
} // namespace cereal

namespace fisch::vx {
class OmnibusChipOverJTAG;
class Omnibus;
} // namespace fisch::vx

namespace haldls {
namespace vx GENPYBIND_TAG_HALDLS_VX {

// TODO: Switch to CRTP pattern when https://github.com/kljohann/genpybind/issues/28 is solved
class GENPYBIND(visible) SynapseDriverSRAMTimingConfig : public detail::SRAMTimingConfig
{
public:
	typedef halco::hicann_dls::vx::SynapseDriverSRAMTimingConfigOnDLS coordinate_type;
	constexpr static auto unsupported_read_targets GENPYBIND(hidden) = {
	    hxcomm::vx::Target::simulation, hxcomm::vx::Target::hardware};

	bool operator==(SynapseDriverSRAMTimingConfig const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseDriverSRAMTimingConfig const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseDriverSRAMTimingConfig const& config)
	    SYMBOL_VISIBLE;

	template <typename AddressT>
	std::array<AddressT, config_size_in_words> addresses(coordinate_type const& word) const
	    SYMBOL_VISIBLE GENPYBIND(hidden);

private:
	friend class cereal::access;
	template <typename Archive>
	void serialize(Archive& ar, std::uint32_t);
};

namespace detail {

template <>
struct BackendContainerTrait<SynapseDriverSRAMTimingConfig>
    : public BackendContainerBase<
          SynapseDriverSRAMTimingConfig,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

} // namespace detail

} // namespace vx
} // namespace haldls
