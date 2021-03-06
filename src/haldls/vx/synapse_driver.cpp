#include "haldls/vx/synapse_driver.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.tcc"

namespace haldls {
namespace vx {

bool SynapseDriverSRAMTimingConfig::operator==(SynapseDriverSRAMTimingConfig const& other) const
{
	return static_cast<SRAMTimingConfig>(*this) == static_cast<SRAMTimingConfig>(other);
}

bool SynapseDriverSRAMTimingConfig::operator!=(SynapseDriverSRAMTimingConfig const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(SynapseDriverSRAMTimingConfig)

template <typename AddressT>
std::array<AddressT, SynapseDriverSRAMTimingConfig::config_size_in_words>
SynapseDriverSRAMTimingConfig::addresses(coordinate_type const& coord) const
{
	return {AddressT(synapse_driver_sram_timing_base_addresses.at(coord.toEnum())),
	        AddressT(synapse_driver_sram_timing_base_addresses.at(coord.toEnum()) + 1)};
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SynapseDriverSRAMTimingConfig::config_size_in_words>
SynapseDriverSRAMTimingConfig::addresses(coordinate_type const& coord) const;
template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusAddress,
    SynapseDriverSRAMTimingConfig::config_size_in_words>
SynapseDriverSRAMTimingConfig::addresses(coordinate_type const& coord) const;

template <typename Archive>
void SynapseDriverSRAMTimingConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(cereal::base_class<detail::SRAMTimingConfig>(this));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseDriverSRAMTimingConfig)

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::SynapseDriverSRAMTimingConfig, 0)
