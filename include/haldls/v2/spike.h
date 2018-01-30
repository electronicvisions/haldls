#pragma once

#include <ostream>

#include "halco/common/genpybind.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/synapse.h"

namespace haldls {
namespace v2 GENPYBIND(tag(haldls_v2)) {

class GENPYBIND(visible) PlaybackSpike
{
public:
	PlaybackSpike() HALDLS_VISIBLE;

	PlaybackSpike(
		hardware_time_type const time,
		SynapseBlock::Synapse::Address const& source_address,
		halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver) HALDLS_VISIBLE;

	hardware_time_type get_time() const HALDLS_VISIBLE;
	SynapseBlock::Synapse::Address get_source_address() const HALDLS_VISIBLE;
	halco::hicann_dls::v2::SynapseDriverOnDLS get_synapse_driver() const HALDLS_VISIBLE;

	bool operator==(PlaybackSpike const& other) const HALDLS_VISIBLE;
	bool operator!=(PlaybackSpike const& other) const HALDLS_VISIBLE;
	bool operator<(PlaybackSpike const& other) const HALDLS_VISIBLE;
	bool operator>(PlaybackSpike const& other) const HALDLS_VISIBLE;
	bool operator<=(PlaybackSpike const& other) const HALDLS_VISIBLE;
	bool operator>=(PlaybackSpike const& other) const HALDLS_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PlaybackSpike const& spike) HALDLS_VISIBLE;

private:
	hardware_time_type m_time;
	SynapseBlock::Synapse::Address m_source_address;
	halco::hicann_dls::v2::SynapseDriverOnDLS m_synapse_driver;
}; // PlaybackSpike

class GENPYBIND(visible) RecordedSpike
{
public:
	RecordedSpike() HALDLS_VISIBLE;

	RecordedSpike(
		hardware_time_type const time,
		halco::hicann_dls::v2::NeuronOnDLS const& neuron) HALDLS_VISIBLE;

	hardware_time_type get_time() const HALDLS_VISIBLE;
	halco::hicann_dls::v2::NeuronOnDLS get_neuron() const HALDLS_VISIBLE;

	bool operator==(RecordedSpike const& other) const HALDLS_VISIBLE;
	bool operator!=(RecordedSpike const& other) const HALDLS_VISIBLE;
	bool operator<(RecordedSpike const& other) const HALDLS_VISIBLE;
	bool operator>(RecordedSpike const& other) const HALDLS_VISIBLE;
	bool operator<=(RecordedSpike const& other) const HALDLS_VISIBLE;
	bool operator>=(RecordedSpike const& other) const HALDLS_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, RecordedSpike const& spike) HALDLS_VISIBLE;

private:
	hardware_time_type m_time;
	halco::hicann_dls::v2::NeuronOnDLS m_neuron;
}; // RecordedSpike

} // namespace v2
} // namespace haldls
