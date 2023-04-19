#include "haldls/vx/common.h"
#include <boost/hana/adapt_struct.hpp>

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::Morphology>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::Morphology,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::MCSafeAtomicNeuron>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::MCSafeAtomicNeuron,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct BackendContainerTrait<lola::vx::CHIP_REVISION_STR::LogicalNeuron>
    : public BackendContainerBase<
          lola::vx::CHIP_REVISION_STR::LogicalNeuron,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};


template <>
struct VisitPreorderImpl<lola::vx::CHIP_REVISION_STR::LogicalNeuron>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    lola::vx::CHIP_REVISION_STR::LogicalNeuron::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		visitor(coord, config);

		auto placed_compartments = coord.get_placed_compartments();
		for (auto const& [compartment_on_ln, atomic_neurons_on_dls] : placed_compartments) {
			for (size_t i = 0; i < atomic_neurons_on_dls.size(); ++i) {
				hate::Empty<lola::vx::CHIP_REVISION_STR::AtomicNeuron> atomic_neuron;
				visit_preorder(atomic_neuron, atomic_neurons_on_dls.at(i), visitor);
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::CHIP_REVISION_STR::LogicalNeuron::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using namespace halco::hicann_dls::vx::CHIP_REVISION_STR;
		typedef std::pair<CompartmentOnLogicalNeuron, size_t> NeuronOnLogicalNeuronCompartment;
		typedef std::pair<NeuronOnLogicalNeuronCompartment, NeuronOnLogicalNeuronCompartment>
		    Connection;

		visitor(coord, config);
		// std::map<CompIndx, vec<ANOnDLS>
		auto placed_compartments = coord.get_placed_compartments();

		// copy for write instruction
		auto morphology = config.morphology;
		// disable all "connect_right" and "connect_soma_right" connections
		for (auto& [comp_index, comp_neurons] : morphology) {
			for (auto& neuron : comp_neurons) {
				neuron.multicompartment.connect_right = false;
				neuron.multicompartment.connect_soma_right = false;
			}
		}

		// enable "connect_right" at proper neurons
		for (auto const& connection_pair : config.m_involved_connect_right_neurons) {
			auto coords = config.get_coordinates(connection_pair, placed_compartments);
			auto neurons = config.get_neurons(connection_pair, morphology);
			// get neuron further to the left
			if (coords.first.toEnum() <= coords.second.toEnum()) {
				neurons.first.multicompartment.connect_right = true;
			} else {
				neurons.second.multicompartment.connect_right = true;
			}
		}

		// enable "connect_soma_right" at proper neurons
		for (auto const& connection_pair : config.m_involved_connect_soma_right_neurons) {
			auto coords = config.get_coordinates(connection_pair, placed_compartments);
			auto neurons = config.get_neurons(connection_pair, morphology);
			// get neuron further to the left
			if (coords.first.toEnum() <= coords.second.toEnum()) {
				neurons.first.multicompartment.connect_right = true;
			} else {
				neurons.second.multicompartment.connect_right = true;
			}
		}

		constexpr bool write =
		    std::is_same<ContainerT, lola::vx::CHIP_REVISION_STR::LogicalNeuron const>::value;
		for (auto const& [compartment_on_ln, atomic_neurons_on_dls] : placed_compartments) {
			for (size_t i = 0; i < atomic_neurons_on_dls.size(); ++i) {
				// write
				if constexpr (write) {
					auto atomic_neuron = static_cast<lola::vx::CHIP_REVISION_STR::AtomicNeuron>(
					    (morphology.at(compartment_on_ln)).at(i));
					visit_preorder(atomic_neuron, atomic_neurons_on_dls.at(i), visitor);
				}

				// read
				else {
					auto atomic_neuron = lola::vx::CHIP_REVISION_STR::AtomicNeuron();
					visit_preorder(atomic_neuron, atomic_neurons_on_dls.at(i), visitor);
					// reconstruct morphology
					std::vector<lola::vx::CHIP_REVISION_STR::MCSafeAtomicNeuron>&
					    vector_mc_safe_neurons = config.morphology[compartment_on_ln];
					lola::vx::CHIP_REVISION_STR::MCSafeAtomicNeuron mc_safe_neuron(atomic_neuron);
					vector_mc_safe_neurons.push_back(mc_safe_neuron);
					// check for connect_{soma_}right
					if (mc_safe_neuron.multicompartment.connect_right == true) {
						auto coord = atomic_neurons_on_dls[i];
						// FIXME: neighbor_coord will throw if we are at the right borders of the
						// chip with coord
						AtomicNeuronOnDLS neighbor_coord(
						    NeuronColumnOnDLS(coord.x().value() + 1), coord.y());
						auto it = std::find(
						    atomic_neurons_on_dls.begin(), atomic_neurons_on_dls.end(),
						    neighbor_coord);
						size_t index_neighbor = std::distance(it, atomic_neurons_on_dls.begin());
						NeuronOnLogicalNeuronCompartment first_pair(compartment_on_ln, i);
						NeuronOnLogicalNeuronCompartment second_pair(
						    compartment_on_ln, index_neighbor);
						Connection pair(first_pair, second_pair);
						if (it != atomic_neurons_on_dls.end()) {
							config.m_involved_connect_right_neurons.push_back(pair);
						} else {
							throw std::logic_error("Neighbor of neuron does not exist even though "
							                       "its right membrane connection is enabled.");
						}
					}

					if (mc_safe_neuron.multicompartment.connect_soma_right == true) {
						auto coord = atomic_neurons_on_dls[i];
						// FIXME: neighbor_coord will throw if we are at the right borders of the
						// chip with coord
						AtomicNeuronOnDLS neighbor_coord(
						    NeuronColumnOnDLS(coord.x().value() + 1), coord.y());
						auto it = std::find(
						    atomic_neurons_on_dls.begin(), atomic_neurons_on_dls.end(),
						    neighbor_coord);
						size_t index_neighbor = std::distance(it, atomic_neurons_on_dls.begin());
						NeuronOnLogicalNeuronCompartment first_pair(compartment_on_ln, i);
						NeuronOnLogicalNeuronCompartment second_pair(
						    compartment_on_ln, index_neighbor);
						Connection pair(first_pair, second_pair);
						if (it != atomic_neurons_on_dls.end()) {
							config.m_involved_connect_soma_right_neurons.push_back(pair);
						} else {
							throw std::logic_error("Neighbor of neuron does not exist even though "
							                       "its right soma connection is enabled.");
						}
					}
				}
			}
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::CHIP_REVISION_STR::Morphology, morphology);

BOOST_HANA_ADAPT_STRUCT(
    lola::vx::CHIP_REVISION_STR::MCSafeAtomicNeuron,
    excitatory_input,
    inhibitory_input,
    leak,
    reset,
    threshold,
    multicompartment,
    constant_current,
    membrane_capacitance,
    adaptation,
    exponential,
    event_routing,
    readout,
    refractory_period,
    bayesian);

BOOST_HANA_ADAPT_STRUCT(lola::vx::CHIP_REVISION_STR::LogicalNeuron, morphology);
