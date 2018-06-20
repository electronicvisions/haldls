#pragma once

#include <ostream>

#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/synapse.h"
#include "haldls/v2/genpybind.h"

#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {

class GENPYBIND(visible) PlaybackSpike
{
public:
	PlaybackSpike() SYMBOL_VISIBLE;

	PlaybackSpike(
		hardware_time_type const time,
		SynapseBlock::Synapse::Address const& source_address,
		halco::hicann_dls::v2::SynapseDriverOnDLS const& synapse_driver) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(time))
	hardware_time_type get_time() const SYMBOL_VISIBLE;
	GENPYBIND(getter_for(source_address))
	SynapseBlock::Synapse::Address get_source_address() const SYMBOL_VISIBLE;
	GENPYBIND(getter_for(synapse_driver))
	halco::hicann_dls::v2::SynapseDriverOnDLS get_synapse_driver() const SYMBOL_VISIBLE;

	bool operator==(PlaybackSpike const& other) const SYMBOL_VISIBLE;
	bool operator!=(PlaybackSpike const& other) const SYMBOL_VISIBLE;
	bool operator<(PlaybackSpike const& other) const SYMBOL_VISIBLE;
	bool operator>(PlaybackSpike const& other) const SYMBOL_VISIBLE;
	bool operator<=(PlaybackSpike const& other) const SYMBOL_VISIBLE;
	bool operator>=(PlaybackSpike const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PlaybackSpike const& spike) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	hardware_time_type m_time;
	SynapseBlock::Synapse::Address m_source_address;
	halco::hicann_dls::v2::SynapseDriverOnDLS m_synapse_driver;
}; // PlaybackSpike

class GENPYBIND(visible) RecordedSpike
{
public:
	RecordedSpike() SYMBOL_VISIBLE;

	RecordedSpike(
		hardware_time_type const time,
		halco::hicann_dls::v2::NeuronOnDLS const& neuron) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(time))
	hardware_time_type get_time() const SYMBOL_VISIBLE;
	GENPYBIND(getter_for(neuron))
	halco::hicann_dls::v2::NeuronOnDLS get_neuron() const SYMBOL_VISIBLE;

	bool operator==(RecordedSpike const& other) const SYMBOL_VISIBLE;
	bool operator!=(RecordedSpike const& other) const SYMBOL_VISIBLE;
	bool operator<(RecordedSpike const& other) const SYMBOL_VISIBLE;
	bool operator>(RecordedSpike const& other) const SYMBOL_VISIBLE;
	bool operator<=(RecordedSpike const& other) const SYMBOL_VISIBLE;
	bool operator>=(RecordedSpike const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, RecordedSpike const& spike) SYMBOL_VISIBLE;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	hardware_time_type m_time;
	halco::hicann_dls::v2::NeuronOnDLS m_neuron;
}; // RecordedSpike

} // namespace v2
} // namespace haldls
