#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/cadc.h"

#include "haldls/vx/container.tcc"
#include "hate/join.h"
#include "lola/vx/gray_scale.h"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#elif CHIP_REVISION == 4
#define CHIP_REVISION_STR v4
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

bool CADCReadoutChain::Channel::operator==(CADCReadoutChain::Channel const& other) const
{
	return equal(*this, other);
}

bool CADCReadoutChain::Channel::operator!=(CADCReadoutChain::Channel const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, CADCReadoutChain::Channel const& config)
{
	os << "Channel(\n";
	boost::hana::for_each(boost::hana::keys(config), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << boost::hana::at_key(config, key) << "\n";
	});
	os << ")";

	return os;
}

bool CADCReadoutChain::Ramp::operator==(CADCReadoutChain::Ramp const& other) const
{
	return equal(*this, other);
}

bool CADCReadoutChain::Ramp::operator!=(CADCReadoutChain::Ramp const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, CADCReadoutChain::Ramp const& config)
{
	os << "Ramp(\n";
	os << "\tenable: " << config.enable << "\n";
	os << "\treset_wait: " << config.reset_wait << "\n";
	os << "\tdead_time: " << config.dead_time << "\n";
	os << "\tv_offset_assignment: "
	   << "[left: "
	   << config.v_offset_assignment[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left] << "\n";
	os << ", right: "
	   << config.v_offset_assignment[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right] << "]"
	   << "\n";
	os << "\tv_offset: "
	   << "[left: " << config.v_offset[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << "\n";
	os << ", right: " << config.v_offset[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n";
	os << "\ti_slope: "
	   << "[left: " << config.i_slope[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left] << "\n";
	os << ", right: " << config.i_slope[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n";
	os << ")";

	return os;
}

bool CADCReadoutChain::Correlation::operator==(CADCReadoutChain::Correlation const& other) const
{
	return equal(*this, other);
}

bool CADCReadoutChain::Correlation::operator!=(CADCReadoutChain::Correlation const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, CADCReadoutChain::Correlation const& config)
{
	os << "Correlation(\n";
	os << "\tsense_delay: " << config.sense_delay << "\n";
	os << "\treset_duration: " << config.reset_duration << "\n";
	os << "\treset_fall_time: " << config.reset_fall_time << "\n";
	os << "\treset_mode: " << config.reset_mode << "\n";
	os << "\ti_bias_ramp: "
	   << "[left: " << config.i_bias_ramp[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << "\n";
	os << ", right: " << config.i_bias_ramp[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n";
	os << "\ti_bias_store: "
	   << "[left: " << config.i_bias_store[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << "\n";
	os << ", right: " << config.i_bias_store[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n";
	os << "\ti_bias_corout: "
	   << "[left: " << config.i_bias_corout[halco::hicann_dls::vx::CapMemBlockOnHemisphere::left]
	   << "\n";
	os << ", right: " << config.i_bias_corout[halco::hicann_dls::vx::CapMemBlockOnHemisphere::right]
	   << "]\n";
	os << ")";
	return os;
}

bool CADCReadoutChain::operator==(CADCReadoutChain const& other) const
{
	return equal(*this, other);
}

bool CADCReadoutChain::operator!=(CADCReadoutChain const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, CADCReadoutChain const& config)
{
	os << "CADCReadoutChain(" << std::endl;
	os << "  ramp:\t" << config.ramp << std::endl;
	os << "  channels_causal:\t[";
	os << hate::join(config.channels_causal, ", ");
	os << "]" << std::endl;
	os << "  channels_acausal:\t[";
	os << hate::join(config.channels_acausal, ", ");
	os << "]" << std::endl;
	os << "  correlation:\t" << config.correlation << std::endl;
	os << ")";

	return os;
}


CADCSampleRow::CADCSampleRow() {}

bool CADCSampleRow::operator==(CADCSampleRow const& other) const
{
	return equal(*this, other);
}

bool CADCSampleRow::operator!=(CADCSampleRow const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, CADCSampleRow const& row)
{
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;

	auto print = [](auto const& values) -> std::string {
		typedef typename std::remove_cv<
		    typename std::remove_reference<decltype(values)>::type>::type::value_type value_type;
		std::stringstream ss;
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynapseOnSynapseRow(entry, quad)];
			}
			ss << detail::gray_scale(double(acc) / double(EntryOnQuad::size * value_type::size));
		}
		return ss.str();
	};

	os << "CADCSampleRow(" << std::endl;
	boost::hana::for_each(boost::hana::keys(row), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print(boost::hana::at_key(row, key)) << std::endl;
	});
	os << ")";

	return os;
}


CADCSamples::CADCSamples() : causal(), acausal() {}

bool CADCSamples::operator==(CADCSamples const& other) const
{
	return causal == other.causal && acausal == other.acausal;
}

bool CADCSamples::operator!=(CADCSamples const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CADCSamples const& samples)
{
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;

	auto print = [](auto const& values) -> std::string {
		typedef typename std::remove_cv<typename std::remove_reference<decltype(values)>::type>::
		    type::value_type::value_type value_type;
		std::stringstream ss;
		ss << "top:\t";
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynramOnDLS(0)][SynapseOnSynapseRow(entry, quad)];
			}
			ss << detail::gray_scale(double(acc) / double(EntryOnQuad::size * value_type::size));
		}
		ss << std::endl;
		ss << "\t\tbottom:\t";
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynramOnDLS(1)][SynapseOnSynapseRow(entry, quad)];
			}
			ss << detail::gray_scale(double(acc) / double(EntryOnQuad::size * value_type::size));
		}
		ss << std::endl;
		return ss.str();
	};

	os << "CADCSamples(" << std::endl;
	os << "  causal:\t" << print(samples.causal) << std::endl;
	os << "  acausal:\t" << print(samples.acausal) << std::endl;
	os << ")";

	return os;
}

} // namespace lola::vx::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::CADCReadoutChain)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::CADCSampleRow)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::CADCSamples)
