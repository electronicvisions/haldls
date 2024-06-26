#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/synapse_driver.h"

#include "haldls/vx/container.tcc"
#include "hate/indent.h"
#include "hate/join.h"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#elif CHIP_REVISION == 4
#define CHIP_REVISION_STR v4
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

bool SynapseDriverBlock::operator==(SynapseDriverBlock const& other) const
{
	return equal(*this, other);
}

bool SynapseDriverBlock::operator!=(SynapseDriverBlock const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseDriverBlock const& config)
{
	os << "SynapseDriverBlock(\n";
	os << "\tsynapse_drivers:\n";
	{
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t\t");
		ios << hate::join(config.synapse_drivers, "\n");
	}
	os << "\n";
	os << "\tpadi_bus:\n";
	{
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t\t");
		ios << config.padi_bus << "\n";
	}
	os << "\n";
	os << "\tstp:\n";
	{
		hate::IndentingOstream ios(os);
		ios << hate::Indentation("\t\t");
		ios << config.stp << "\n";
	}
	os << "\n";
	os << "\tstp_v_charge_0: "
	   << "[left: " << config.stp_v_charge_0[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << ", right: "
	   << config.stp_v_charge_0[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right] << "]\n";
	os << "\tstp_v_charge_1: "
	   << "[left: " << config.stp_v_charge_1[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << ", right: "
	   << config.stp_v_charge_1[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right] << "]\n";
	os << "\tstp_v_recover_0: "
	   << "[left: " << config.stp_v_recover_0[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << ", right: "
	   << config.stp_v_recover_0[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right] << "]\n";
	os << "\tstp_v_recover_1: "
	   << "[left: " << config.stp_v_recover_1[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << ", right: "
	   << config.stp_v_recover_1[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right] << "]\n";
	os << "\tstp_i_ramp: "
	   << "[left: " << config.stp_i_ramp[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << ", right: " << config.stp_i_ramp[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n";
	os << "\tstp_i_calib: "
	   << "[left: " << config.stp_i_calib[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << ", right: " << config.stp_i_calib[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n";
	os << "\tstp_i_bias_readout: " << config.stp_i_bias_readout << "\n";
	os << "\thagen_i_bias_dac: " << config.hagen_i_bias_dac << "\n";
	os << ")";

	return os;
}
} // namespace lola::vx::CHIP_REVISION_STR

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::SynapseDriverBlock)
