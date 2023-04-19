#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "lola/vx/morphology.h"

#include <stack>
#include <string>

#include "haldls/vx/container.tcc"
#include "lola/vx/hana.h"

#if CHIP_REVISION == 3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR {

typedef halco::hicann_dls::vx::CHIP_REVISION_STR::NeuronColumnOnLogicalNeuron
    NeuronColumnOnLogicalNeuron;
typedef halco::hicann_dls::vx::CHIP_REVISION_STR::NeuronRowOnLogicalNeuron NeuronRowOnLogicalNeuron;
typedef halco::hicann_dls::vx::CHIP_REVISION_STR::AtomicNeuronOnLogicalNeuron
    AtomicNeuronOnLogicalNeuron;

typedef halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments::Compartment
    Compartment;
typedef halco::hicann_dls::vx::CHIP_REVISION_STR::CompartmentOnLogicalNeuron
    CompartmentOnLogicalNeuron;
typedef halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments::Compartments
    Compartments;

namespace { /** Helper functions. */

struct ColumnMajorOrder
{
	bool operator()(
	    AtomicNeuronOnLogicalNeuron const& C1, AtomicNeuronOnLogicalNeuron const& C2) const
	{
		if (C1.x() == C2.x()) {
			return (C1.y() < C2.y());
		}
		return (C1.x() < C2.x());
	}
};

/* Checks if set of coordinates is contiguous.
 *
 * A set of more than one coordinate is contiguous if:
 *     * Every coordinate is reachable from every coordinate only using paths which include
 *       coordinates of the set and the taxicab geometry.
 *
 * @param comp_set Set of compartments, whose connection should be determined.
 * @return Returns true if the set of coordinates is contiguous.
 */
bool is_contiguous(std::set<AtomicNeuronOnLogicalNeuron, ColumnMajorOrder> const& comp_set)
{
	// Uses the logic/ordering of a set to check if the given compartments are contiguous.
	std::set<AtomicNeuronOnLogicalNeuron, ColumnMajorOrder>::iterator it = comp_set.begin();

	// No neuron or only one in set.
	if (comp_set.empty() or comp_set.size() == 1) {
		return true;
	}

	// Check if the last and second to last compartment are properly connected.
	std::set<AtomicNeuronOnLogicalNeuron, ColumnMajorOrder>::iterator last_comp_it =
	    std::prev(comp_set.end(), 1);
	std::set<AtomicNeuronOnLogicalNeuron, ColumnMajorOrder>::iterator second_to_last_comp_it =
	    std::prev(comp_set.end(), 2);

	auto last_comp = *last_comp_it;
	auto second_to_last_comp = *second_to_last_comp_it;

	// Second to last comp is in the top row.
	if (second_to_last_comp.y().value() == 0) {
		// Check for next comp below or to the right.
		bool neuron_below = last_comp.y().value() == 1 && last_comp.x() == second_to_last_comp.x();
		bool neuron_right = last_comp.y().value() == 0 &&
		                    last_comp.x().value() == second_to_last_comp.x().value() + 1;
		if (not(neuron_below || neuron_right)) {
			// No compartment to the right or below current neuron.
			return false;
		}
	}
	// Second to last comp is in the bottom row.
	else {
		// Check next comp to the right.
		bool neuron_right = last_comp.y().value() == 1 &&
		                    last_comp.x().value() == second_to_last_comp.x().value() + 1;
		if (not(neuron_right)) {
			// Special case were last comp is in top row and second to last comp is in the bottom
			// row.
			// -> check if third to last comp is in top row before last comp.
			// Since chain might only exist out of two comp return early.
			if (comp_set.size() < 3) {
				return false;
			} else {
				auto third_to_last_comp = *(std::prev(comp_set.end(), 3));
				bool third_to_last_is_right_of_last =
				    last_comp.y() == third_to_last_comp.y() &&
				    last_comp.x().value() == third_to_last_comp.x().value() + 1;
				if (not(third_to_last_is_right_of_last)) {
					return false;
				}
			}
		}
	}

	// Iterate until the third last element.
	while (not(it == last_comp_it || it == second_to_last_comp_it)) {
		auto curr_comp = *(it);
		auto next_comp = *(std::next(it, 1));
		auto next2_comp = *(std::next(it, 2));

		bool neuron_right = next_comp.y().value() == curr_comp.y().value() &&
		                    next_comp.x().value() == curr_comp.x().value() + 1;
		bool next2_neuron_right = next2_comp.y().value() == curr_comp.y().value() &&
		                          next2_comp.x().value() == curr_comp.x().value() + 1;
		// Current compartment is in the top row.
		if (curr_comp.y().value() == 0) {
			// Check for next comp below OR to the right.
			bool neuron_below = next_comp.y().value() == 1 && next_comp.x() == curr_comp.x();
			if (not(neuron_below || neuron_right)) {
				// No compartment to the right or below current neuron.
				return false;
			}
			// Corner case: if comp below AND to the right are set, advance by 2.
			if (neuron_below && next2_neuron_right) {
				std::advance(it, 2);
				continue;
			}
		}
		// Current compartment is in the bottom row.
		else {
			// Check if next comp in set is to the right.
			if (not(neuron_right)) {
				// Check if next to next comp is to the right (implies the next compartment to be to
				// the diagonal right in the top row).
				if (not(next2_neuron_right)) {
					return false;
				}
			}
		}
		++it;
	}
	return true;
}

// Returns true if AtomicNeuron is a bridge segment.
// A bridge segment is a neuron, whos only modified multicompartment parameter is
// 'connect_soma_right'. So the only purpose of the bridge segment is to continue the somatic
// line.
bool is_bridge_segment(AtomicNeuron::Multicompartment neuron)
{
	if (neuron.connect_soma_right == false) {
		return false;
	}
	if (neuron.connect_soma == true) {
		return false;
	}
	if (neuron.connect_right == true) {
		return false;
	}
	if (neuron.connect_vertical == true) {
		return false;
	}
	if (neuron.enable_conductance == true) {
		return false;
	}
	return true;
}

// advance curr_coord_left to left neigbor if possible returns true and sets curr_coord_left to its
// neighbor.
bool advance_left(AtomicNeuronOnLogicalNeuron& curr_coord_left)
{
	if (curr_coord_left.x() == NeuronColumnOnLogicalNeuron::min) {
		return false;
	} else {
		curr_coord_left =
		    curr_coord_left.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::left);
		return true;
	}
}

} // namespace

// Morphology member functions.

void Morphology::connect_to_soma(AtomicNeuronOnLogicalNeuron const& coord)
{
	morphology[coord].connect_soma = true;
	morphology[coord].enable_conductance = false;
}

void Morphology::connect_resistor_to_soma(AtomicNeuronOnLogicalNeuron const& coord)
{
	morphology[coord].connect_soma = false;
	morphology[coord].enable_conductance = true;
}

void Morphology::connect_vertical(NeuronColumnOnLogicalNeuron const& column)
{
	auto coord_top = AtomicNeuronOnLogicalNeuron(column, NeuronRowOnLogicalNeuron(0));
	auto coord_bot = AtomicNeuronOnLogicalNeuron(column, NeuronRowOnLogicalNeuron(1));
	morphology[coord_top].connect_vertical = true;
	morphology[coord_bot].connect_vertical = true;
}

void Morphology::connect_soma_line(
    NeuronColumnOnLogicalNeuron const& start,
    NeuronColumnOnLogicalNeuron const& end,
    NeuronRowOnLogicalNeuron const& row)
{
	for (auto column = start.value(); column < end.value(); ++column) {
		auto coord = AtomicNeuronOnLogicalNeuron(NeuronColumnOnLogicalNeuron(column), row);
		morphology[coord].connect_soma_right = true;
		std::pair<AtomicNeuronOnLogicalNeuron, AtomicNeuronOnLogicalNeuron> pair(
		    coord, coord.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::right));
		m_involved_connect_soma_right.push_back(pair);
	}
	// Track last neuron (end) as part of the morphology.
	morphology[AtomicNeuronOnLogicalNeuron(end, row)];
}

void Morphology::connect_right(
    AtomicNeuronOnLogicalNeuron const& neuron_left, AtomicNeuronOnLogicalNeuron const& neuron_right)
{
	morphology[neuron_left].connect_right = true;
	morphology[neuron_right];
	std::pair<AtomicNeuronOnLogicalNeuron, AtomicNeuronOnLogicalNeuron> pair(
	    neuron_left, neuron_right);
	m_involved_connect_right.push_back(pair);
}

CompartmentOnLogicalNeuron Morphology::create_compartment(Compartment const& compartment)
{
	// TODO: Set here as data structure okay? (allows easy checking below).
	std::set<AtomicNeuronOnLogicalNeuron, ColumnMajorOrder> neurons(
	    compartment.begin(), compartment.end());

	// No neuron was provided.
	if (neurons.empty()) {
		throw std::runtime_error("At least one neuron must be in a compartment.");
	}

	// Check if provided neurons of compartment were not used for previous created compartments
	for (auto const& [compartment_index, created_compartment] : m_compartments) {
		for (auto const& neuron : compartment) {
			for (auto const& used_neuron : created_compartment) {
				if (neuron == used_neuron) {
					std::stringstream ss;
					ss << "Neuron with coordinate " << neuron
					   << " was already used for constructing "
					   << "the compartment with index " << compartment_index
					   << " containing following neurons: \n";
					for (auto const& neuron_in_compartment : created_compartment) {
						ss << neuron_in_compartment << "\n";
					}
					throw std::runtime_error(ss.str());
				}
			}
		}
	}

	// One neuron is provided.
	if (neurons.size() == 1) {
		morphology[*neurons.begin()];
		CompartmentOnLogicalNeuron current_compartment_index(m_compartments.size());
		m_compartments[current_compartment_index] = compartment;
		return current_compartment_index;
	}

	if (not is_contiguous(neurons)) {
		throw std::runtime_error("Compartment is not made up of a contiguous block of neurons.");
	}

	// Connect all neurons until second to last since we always have to look two connections ahead
	// due to horizontal connections.
	std::set<AtomicNeuronOnLogicalNeuron, ColumnMajorOrder>::iterator it = neurons.begin();
	while (it != std::prev(neurons.end(), 2)) {
		auto neuron = *(it);
		auto next_neuron = *(std::next(it));
		auto next2_neuron = *(std::next(it, 2));
		// Connect to right and save connection in m_involved_connect_right.
		if (neuron.y() == next_neuron.y()) {
			Morphology::connect_right(neuron, next_neuron);
		}
		if (neuron.y() == next2_neuron.y()) {
			Morphology::connect_right(neuron, next2_neuron);
		}
		// Connect vertically.
		if (neuron.x() == next_neuron.x()) {
			Morphology::connect_vertical(neuron.x());
		}
		++it;
	}

	// Connect the last and second to last neurons.
	auto last_neuron = *(neurons.rbegin());
	auto second_to_last_neuron = *(std::next(neurons.rbegin(), 1));

	if (last_neuron.x() == second_to_last_neuron.x()) {
		Morphology::connect_vertical(last_neuron.x());
	}
	if (last_neuron.y() == second_to_last_neuron.y()) {
		Morphology::connect_right(second_to_last_neuron, last_neuron);
	}
	// Add compartment to compartments.
	CompartmentOnLogicalNeuron current_compartment_index(m_compartments.size());
	m_compartments[current_compartment_index] = compartment;
	return current_compartment_index;
}

std::ostream& operator<<(std::ostream& os, Morphology const& morph)
{
	std::stringstream ss;
	for (auto& neuron : morph.morphology) {
		ss << "neuron at: " << neuron.first << std::endl;
		ss << neuron.second << std::endl;
	}
	ss << std::endl;
	ss << morph.create_current_config().str();
	os << ss.str();
	return os;
}

std::stringstream Morphology::create_current_config() const
{
	// Plot the morphology.
	// Make greedy guess for needed length.
	std::stringstream ss;
	int const len = std::min(morphology.size() * 3, NeuronColumnOnLogicalNeuron::max);
	std::string s(len, ' ');
	std::vector<std::string> vs{9, s};
	for (auto const& [coord, config] : morphology) {
		if (coord.y().value() == 0) {
			auto& n = vs[3];
			n[coord.x().value() * 2] = 'N';
			if (config.connect_soma_right == true) {
				auto& c = vs[0];
				c[coord.x().value() * 2 + 1] = '-';
				c[coord.x().value() * 2] = '+';
			}
			if (config.connect_soma == true) {
				auto& b1 = vs[0];
				b1[coord.x().value() * 2] = '+';
				auto& c = vs[1];
				c[coord.x().value() * 2] = '|';
				auto& b2 = vs[2];
				b2[coord.x().value() * 2] = '|';
			}
			if (config.enable_conductance == true) {
				auto& b1 = vs[0];
				b1[coord.x().value() * 2] = '+';
				auto& c = vs[1];
				c[coord.x().value() * 2] = 'R';
				auto& b2 = vs[2];
				b2[coord.x().value() * 2] = '|';
			}
			if (config.connect_right == true) {
				auto& c = vs[3];
				c[coord.x().value() * 2 + 1] = '-';
			}
			if (config.connect_vertical == true) {
				auto& c = vs[4];
				c[coord.x().value() * 2] = '|';
			}
		} else {
			auto& n = vs[5];
			n[coord.x().value() * 2] = 'N';
			if (config.connect_soma_right == true) {
				auto& c = vs[8];
				c[coord.x().value() * 2 + 1] = '-';
				c[coord.x().value() * 2] = '+';
			}
			if (config.connect_soma == true) {
				auto& b1 = vs[8];
				b1[coord.x().value() * 2] = '+';
				auto& c = vs[7];
				c[coord.x().value() * 2] = '|';
				auto& b2 = vs[6];
				b2[coord.x().value() * 2] = '|';
			}
			if (config.enable_conductance == true) {
				auto& b1 = vs[8];
				b1[coord.x().value() * 2] = '+';
				auto& c = vs[7];
				c[coord.x().value() * 2] = 'R';
				auto& b2 = vs[6];
				b2[coord.x().value() * 2] = '|';
			}
			if (config.connect_right == true) {
				auto& c = vs[5];
				c[coord.x().value() * 2 + 1] = '-';
			}
		}
	}
	for (auto const& line : vs) {
		// Don't print lines without information.
		std::string default_str(len, ' ');
		if (line != default_str) {
			ss << line << std::endl;
		}
	}
	return ss;
}

void Morphology::define_compartments()
{
	// All coordinates in morphology.
	std::set<AtomicNeuronOnLogicalNeuron> all_neurons;
	std::transform(
	    morphology.begin(), morphology.end(), std::inserter(all_neurons, all_neurons.end()),
	    [](auto pair) { return pair.first; });
	// Check if already created compartments were enlarged by somatic connections and delete neurons
	// from all_neurons which belong to an already defined compartment.
	for (auto const& [compartment_index, compartment] : m_compartments) {
		Compartment control_compartment = extract_compartment(*compartment.begin(), all_neurons);
		// if size is equal, compartments are equal
		if (control_compartment.size() != compartment.size()) {
			std::stringstream ss;
			// TODO: Print more useful error message (i.e give index or coordinate of neurons
			// which should not be reachable from the compartment.
			ss << "Compartment with index " << compartment_index
			   << " is larger than on construction."
			   << " Make sure that it is not connected via the somatic line to another "
			   << "compartment while not using a conductance.\n"
			   << "Current config:\n"
			   << create_current_config().str();
			throw std::runtime_error(ss.str());
		}
	}
	// Now all_neurons contains only neurons which were not added to the morphology via
	// create_compartment. For each of those neurons create an own compartment and check if they are
	// connected to other neurons.
	while (not all_neurons.empty()) {
		AtomicNeuronOnLogicalNeuron current_neuron = *all_neurons.begin();
		CompartmentOnLogicalNeuron current_compartment(m_compartments.size());
		// Add all reachable neurons to the same compartment as current_neuron and declare them as
		// visited.
		Compartment connectivity_group = extract_compartment(current_neuron, all_neurons);
		m_compartments[current_compartment] = connectivity_group;
		// All neurons not reached belong to another compartment.
	}
}

Compartment Morphology::extract_compartment(
    AtomicNeuronOnLogicalNeuron const& start_neuron,
    std::set<AtomicNeuronOnLogicalNeuron>& unvisited_neurons)
{
	// Unvisited neurons.
	std::stack<AtomicNeuronOnLogicalNeuron> unvisited;
	unvisited.push(start_neuron);
	// Register for neighboring neurons.
	std::set<AtomicNeuronOnLogicalNeuron> for_visit_registered_neighbors;
	// All elements in visited belong to one connectivity group.
	std::set<AtomicNeuronOnLogicalNeuron> visited;
	while (not unvisited.empty()) {
		auto curr_comp = unvisited.top();
		unvisited.pop();
		visited.insert(curr_comp);
		unvisited_neurons.erase(curr_comp);
		auto neighbors = get_neighbors(curr_comp);
		for (auto neighbor : neighbors)
			if (not visited.contains(neighbor) &&
			    not for_visit_registered_neighbors.contains(neighbor)) {
				for_visit_registered_neighbors.insert(neighbor);
				unvisited.push(neighbor);
			}
	}
	return Compartment(visited.begin(), visited.end());
}

Compartment Morphology::get_neighbors(AtomicNeuronOnLogicalNeuron const& start_neuron)
{
	if (!morphology.contains(start_neuron)) {
		std::stringstream ss;
		ss << "start_neuron at " << start_neuron
		   << " is not part of morphology\nMorphology contains:\n";
		for (auto const& [coord, _] : morphology) {
			ss << coord << "\n";
		}
		throw std::runtime_error(ss.str());
	}
	// Check for direct connections to direct spatial neighbors.
	std::vector<AtomicNeuronOnLogicalNeuron> neighbors;
	// Check top/bottom row.
	if (morphology[start_neuron].connect_vertical == true) {
		auto coord =
		    start_neuron.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::opposite_row);
		if (morphology.contains(coord)) {
			if (morphology[coord].connect_vertical == true) {
				neighbors.push_back(coord);
			}
		}
	}
	// Check right.
	if (start_neuron.x() < NeuronColumnOnLogicalNeuron::max) {
		if (morphology[start_neuron].connect_right == true) {
			auto coord = start_neuron.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::right);
			if (morphology.contains(coord)) {
				neighbors.push_back(coord);
			}
		}
	}
	// Check left.
	if (start_neuron.x() > NeuronColumnOnLogicalNeuron::min) {
		auto coord = start_neuron.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::left);
		if (morphology.contains(coord)) {
			if (morphology[coord].connect_right == true) {
				neighbors.push_back(coord);
			}
		}
	}

	// Check for switch connections via the somatic line not using any conductances
	// Sketch:
	//  +-+-+-+-+   <- somatic line
	//  | | R   |   <- connection to somatic line
	//  | | |   |
	//  N N N N N   <- neuron
	//  0 0 1 x 0   <- Compartment membership
	// Legend: R for resistor, | for direct connection, N for neuron, - for connect_soma_right.
	//
	if (morphology[start_neuron].connect_soma == true) {
		// Follow somatic line towards the right
		if (start_neuron.x() < NeuronColumnOnLogicalNeuron::max) {
			auto temp_neuron = start_neuron;
			while (morphology[temp_neuron].connect_soma_right == true &&
			       temp_neuron.x() < NeuronColumnOnLogicalNeuron::max) {
				temp_neuron =
				    temp_neuron.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::right);
				if (morphology.contains(temp_neuron)) {
					if (morphology[temp_neuron].connect_soma == true) {
						neighbors.push_back(temp_neuron);
					}
				} else {
					break;
				}
			}
		}

		// Follow somatic line towards the left.
		if (start_neuron.x() > NeuronColumnOnLogicalNeuron::min) {
			auto temp_neuron =
			    start_neuron.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::left);
			if (morphology.contains(temp_neuron)) {
				while (morphology[temp_neuron].connect_soma_right == true) {
					if (morphology[temp_neuron].connect_soma == true) {
						neighbors.push_back(temp_neuron);
					}
					if (temp_neuron.x() == NeuronColumnOnLogicalNeuron::min) {
						break;
					} else {
						temp_neuron =
						    temp_neuron.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::left);
						if (!morphology.contains(temp_neuron)) {
							break;
						}
					}
				}
			}
		}
	}
	return neighbors;
}

std::set<CompartmentOnLogicalNeuron> Morphology::walk_neighbors(
    CompartmentOnLogicalNeuron const& start_comp)
{
	// Unvisited neighbors.
	std::stack<CompartmentOnLogicalNeuron> unvisited;
	unvisited.push(start_comp);
	// Reachable neighbors.
	std::set<CompartmentOnLogicalNeuron> reachable;
	// All elements in visited belong to one connectivity group.
	std::set<CompartmentOnLogicalNeuron> visited;
	while (not unvisited.empty()) {
		auto curr_comp = unvisited.top();
		unvisited.pop();
		visited.insert(curr_comp);
		auto neighbors = get_neighbors(curr_comp);
		for (auto neighbor : neighbors)
			if (not visited.contains(neighbor) && not reachable.contains(neighbor)) {
				reachable.insert(neighbor);
				unvisited.push(neighbor);
			}
	}
	return visited;
}

std::set<CompartmentOnLogicalNeuron> Morphology::get_neighbors(
    CompartmentOnLogicalNeuron const& curr_comp_idx)
{
	std::set<CompartmentOnLogicalNeuron> neighbors;
	auto comp_set = m_compartments[curr_comp_idx];
	for (auto coord : comp_set) {
		if (morphology[coord].connect_soma == true ||
		    morphology[coord].enable_conductance == true) {
			// Follow towards the right.
			auto curr_coord = coord;
			while (morphology[curr_coord].connect_soma_right == true &&
			       curr_coord.x() < NeuronColumnOnLogicalNeuron::max) {
				auto right_neighbor =
				    curr_coord.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::right);
				if (!morphology.contains(right_neighbor)) {
					break;
				}
				curr_coord = right_neighbor;
				// Current atomic neuron belongs to same compartment.
				if (std::find(begin(comp_set), end(comp_set), curr_coord) != std::end(comp_set)) {
					continue;
				}
				// Current atomic neuron does not belong to the compartment.
				// Check if it is connected to the somatic line or is a bridge neuron.
				if (morphology[curr_coord].connect_soma == true ||
				    morphology[curr_coord].enable_conductance == true ||
				    is_bridge_segment(morphology[curr_coord])) {
					// Atomic neuron belongs to neighboring compartment.
					for (auto map : m_compartments) {
						auto v = map.second;
						if (std::find(begin(v), end(v), curr_coord) != std::end(v)) {
							// Add neighboring compartment or bridge neuron to neighbors.
							neighbors.insert(map.first);
							break;
						}
					}
				}
			}
			// Follow towards the left.
			if (coord.x() == NeuronColumnOnLogicalNeuron::min) {
				continue;
			}

			auto curr_coord_left = coord.get_neighbor(AtomicNeuronOnLogicalNeuron::Direction::left);
			if (!morphology.contains(curr_coord_left)) {
				continue;
			}
			while (morphology[curr_coord_left].connect_soma_right == true) {
				auto v = m_compartments[curr_comp_idx];
				// Current atomic neuron belongs to connectivity group.
				if (std::find(begin(v), end(v), curr_coord_left) != std::end(v)) {
					if (advance_left(curr_coord_left)) {
						if (!morphology.contains(curr_coord_left)) {
							break;
						} else {
							continue;
						}
					} else {
						break;
					}
				}
				// Current atomic neuron does not belong to connectivity group.
				// Check if it is connected to the somatic line or is a bridge neuron.
				if (morphology[curr_coord_left].connect_soma == true ||
				    morphology[curr_coord_left].enable_conductance == true ||
				    is_bridge_segment(morphology[curr_coord_left])) {
					// Atomic neuron is neighbor.
					for (auto map : m_compartments) {
						auto v = map.second;
						if (std::find(begin(v), end(v), curr_coord_left) != std::end(v)) {
							neighbors.insert(map.first);
							break;
						}
					}
				}
				if (advance_left(curr_coord_left)) {
					if (!morphology.contains(curr_coord_left)) {
						break;
					} else {
						continue;
					}
				} else {
					break;
				}
			}
		}
	}
	return neighbors;
}

bool Morphology::check_compartment_connectivity(Compartments const& compartments)
{
	CompartmentOnLogicalNeuron const first_comp_set_idx = compartments.begin()->first;
	auto visited_compartments = walk_neighbors(first_comp_set_idx);

	if (visited_compartments.size() == compartments.size()) {
		return true;
	}
	return false;
}

// Check validity of given morphology. If everything is correct, return LogicalNeuron with immutable
// morhpology.
//
// Validity check:
// 1. Get all neurons, which are connected via their membrane or somtic line not using a resistive
// connection and create piecewise connected compartment/connectivity groups.
// 2. Check if compartments are connected to each other via the somatic line using a conductance.
// If 1. & 2. is true we have a valid configuration.
std::pair<halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments, LogicalNeuron>
Morphology::done()
{
	typedef std::pair<CompartmentOnLogicalNeuron, size_t> NeuronOnLogicalNeuronCompartment;
	typedef std::pair<NeuronOnLogicalNeuronCompartment, NeuronOnLogicalNeuronCompartment>
	    Connection;
	typedef std::vector<Connection> Connections;
	// Iterate through all neurons and assign them to their appropriate compartment. The result is
	// stored in m_compartments.
	define_compartments();
	// Check if each compartment is connected via the somatic line so one contiguous LogicalNeuron
	// has been generated.
	if (not(check_compartment_connectivity(m_compartments))) {
		auto config = create_current_config().str();
		throw std::runtime_error(
		    "LogicalNeuron can't be constructed since it is not contiguous!\n" + config);
	}
	// save all neurons which have set connect_right and their neighbors.
	Connections connected_neighbors;
	for (auto const& [comp_index, neuron_coords_in_comp] : m_compartments) {
		for (std::size_t i = 0; i < neuron_coords_in_comp.size(); ++i) {
			// Check if current neuron_coord is connected to another neuron_coord.
			for (auto const& pair : m_involved_connect_right) {
				if (pair.first == neuron_coords_in_comp[i]) {
					// Find index in vector of second compartment.
					auto it = std::find(
					    begin(neuron_coords_in_comp), end(neuron_coords_in_comp), pair.second);
					size_t j = std::distance(neuron_coords_in_comp.begin(), it);
					NeuronOnLogicalNeuronCompartment first_pair(comp_index, i);
					NeuronOnLogicalNeuronCompartment second_pair(comp_index, j);
					connected_neighbors.push_back(std::make_pair(first_pair, second_pair));
				}
			}
		}
	}

	// Save all neurons which have set connect_soma_right and their neighbors.
	Connections connect_soma_right_neighbors;
	for (auto pair : m_involved_connect_soma_right) {
		auto neuron_coord_1 = pair.first;
		auto neuron_coord_2 = pair.second;
		NeuronOnLogicalNeuronCompartment first_pair;
		NeuronOnLogicalNeuronCompartment second_pair;
		for (auto const& [comp_indx, neuron_coords_in_comp] : m_compartments) {
			for (std::size_t i = 0; i < neuron_coords_in_comp.size(); ++i) {
				if (neuron_coords_in_comp[i] == neuron_coord_1) {
					first_pair = {comp_indx, i};
					continue;
				}
				if (neuron_coords_in_comp[i] == neuron_coord_2) {
					second_pair = {comp_indx, i};
					continue;
				}
			}
		}
		connect_soma_right_neighbors.push_back(std::make_pair(first_pair, second_pair));
	}

	return std::pair<
	    halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments, LogicalNeuron>(
	    halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments(m_compartments),
	    LogicalNeuron(*this, connected_neighbors, connect_soma_right_neighbors));
}

// MCSafeAtomicNeuron::Multicompartment implementation.
MCSafeAtomicNeuron::Multicompartment::Multicompartment() :
    enable_conductance_division(false),
    enable_conductance_multiplication(false),
    i_bias_nmda(),
    connect_soma(false),
    connect_soma_right(false),
    connect_right(false),
    connect_vertical(false),
    enable_conductance(false)
{}

MCSafeAtomicNeuron::Multicompartment::Multicompartment(
    AtomicNeuron::Multicompartment an_multicomp) :
    enable_conductance_division(an_multicomp.enable_conductance_division),
    enable_conductance_multiplication(an_multicomp.enable_conductance_multiplication),
    i_bias_nmda(an_multicomp.i_bias_nmda),
    connect_soma(an_multicomp.connect_soma),
    connect_soma_right(an_multicomp.connect_soma_right),
    connect_right(an_multicomp.connect_right),
    connect_vertical(an_multicomp.connect_vertical),
    enable_conductance(an_multicomp.enable_conductance)
{}

MCSafeAtomicNeuron::Multicompartment::Multicompartment(Multicompartment const& multicompartment) :
    enable_conductance_division(multicompartment.enable_conductance_division),
    enable_conductance_multiplication(multicompartment.enable_conductance_multiplication),
    i_bias_nmda(multicompartment.i_bias_nmda),
    connect_soma(multicompartment.connect_soma),
    connect_soma_right(multicompartment.connect_soma_right),
    connect_right(multicompartment.connect_right),
    connect_vertical(multicompartment.connect_vertical),
    enable_conductance(multicompartment.enable_conductance)
{}

MCSafeAtomicNeuron::Multicompartment::Multicompartment(Multicompartment&& other) :
    enable_conductance_division(other.enable_conductance_division),
    enable_conductance_multiplication(other.enable_conductance_multiplication),
    i_bias_nmda(std::move(other.i_bias_nmda)),
    connect_soma(other.connect_soma),
    connect_soma_right(other.connect_soma_right),
    connect_right(other.connect_right),
    connect_vertical(other.connect_vertical),
    enable_conductance(other.enable_conductance)
{}

MCSafeAtomicNeuron::Multicompartment& MCSafeAtomicNeuron::Multicompartment::operator=(
    Multicompartment&& other)
{
	if (this != &other) {
		enable_conductance_division = other.enable_conductance_division;
		enable_conductance_multiplication = other.enable_conductance_multiplication;
		i_bias_nmda = std::move(other.i_bias_nmda);
	}
	return *this;
}

bool MCSafeAtomicNeuron::Multicompartment::operator==(Multicompartment const& other) const
{
	return (connect_soma == other.connect_soma) &&
	       (connect_soma_right == other.connect_soma_right) &&
	       (connect_right == other.connect_right) && (connect_vertical == other.connect_vertical) &&
	       (enable_conductance == other.enable_conductance) &&
	       (enable_conductance == other.enable_conductance) &&
	       (enable_conductance_division == other.enable_conductance_division) &&
	       (enable_conductance_multiplication == other.enable_conductance_multiplication) &&
	       (i_bias_nmda == other.i_bias_nmda);
}

bool MCSafeAtomicNeuron::Multicompartment::operator!=(Multicompartment const& other) const
{
	return !(*this == other);
}

MCSafeAtomicNeuron::Multicompartment& MCSafeAtomicNeuron::Multicompartment::operator=(
    Multicompartment const& other)
{
	if (this != &other) {
		enable_conductance_division = other.enable_conductance_division;
		enable_conductance_multiplication = other.enable_conductance_multiplication;
		i_bias_nmda = other.i_bias_nmda;
	}
	return *this;
}

MCSafeAtomicNeuron::Multicompartment& MCSafeAtomicNeuron::Multicompartment::operator=(
    AtomicNeuron::Multicompartment const& other)
{
	enable_conductance_division = other.enable_conductance_division;
	enable_conductance_multiplication = other.enable_conductance_multiplication;
	i_bias_nmda = other.i_bias_nmda;
	connect_soma = other.connect_soma;
	connect_soma_right = other.connect_soma_right;
	connect_right = other.connect_right;
	connect_vertical = other.connect_vertical;
	enable_conductance = other.enable_conductance;
	return *this;
}

MCSafeAtomicNeuron::Multicompartment::operator AtomicNeuron::Multicompartment() const
{
	AtomicNeuron::Multicompartment multicompartment;
	multicompartment.enable_conductance_division = enable_conductance_division;
	multicompartment.enable_conductance_multiplication = enable_conductance_multiplication;
	multicompartment.i_bias_nmda = i_bias_nmda;
	multicompartment.connect_soma = connect_soma;
	multicompartment.connect_soma_right = connect_soma_right;
	multicompartment.connect_vertical = connect_vertical;
	multicompartment.enable_conductance = enable_conductance;
	return multicompartment;
}

std::ostream& operator<<(std::ostream& os, MCSafeAtomicNeuron::Multicompartment const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "Multicompartment(\n"
	   << std::boolalpha << "\tconnect_soma:                      " << config.connect_soma << "\n"
	   << "\tconnect_soma_right:                " << config.connect_soma_right << "\n"
	   << "\tconnect_right:                     " << config.connect_right << "\n"
	   << "\tconnect_vertical:                  " << config.connect_vertical << "\n"
	   << "\tenable_conductance:                " << config.enable_conductance << "\n"
	   << "\tenable_conductance_division:       " << config.enable_conductance_division << "\n"
	   << "\tenable_conductance_multiplication: " << config.enable_conductance_multiplication
	   << "\n"
	   << "\ti_bias_nmda:                       " << config.i_bias_nmda << "\n)";
	os << ss.str();
	return os;
}

// MCSafeAtomicNeuron implementation.
MCSafeAtomicNeuron::MCSafeAtomicNeuron() :
    excitatory_input(),
    inhibitory_input(),
    leak(),
    reset(),
    threshold(),
    multicompartment(),
    constant_current(),
    membrane_capacitance(),
    adaptation(),
    exponential(),
    event_routing(),
    readout(),
    refractory_period(),
    bayesian()
{}

MCSafeAtomicNeuron::MCSafeAtomicNeuron(AtomicNeuron atomic_neuron) :
    excitatory_input(atomic_neuron.excitatory_input),
    inhibitory_input(atomic_neuron.inhibitory_input),
    leak(atomic_neuron.leak),
    reset(atomic_neuron.reset),
    threshold(atomic_neuron.threshold),
    multicompartment(atomic_neuron.multicompartment),
    constant_current(atomic_neuron.constant_current),
    membrane_capacitance(atomic_neuron.membrane_capacitance),
    adaptation(atomic_neuron.adaptation),
    exponential(atomic_neuron.exponential),
    event_routing(atomic_neuron.event_routing),
    readout(atomic_neuron.readout),
    refractory_period(atomic_neuron.refractory_period),
    bayesian(atomic_neuron.bayesian)
{}

MCSafeAtomicNeuron::MCSafeAtomicNeuron(AtomicNeuron::Multicompartment an_multicompartment) :
    excitatory_input(AtomicNeuron::SynapticInput()),
    inhibitory_input(AtomicNeuron::SynapticInput()),
    leak(AtomicNeuron::Leak()),
    reset(AtomicNeuron::Reset()),
    threshold(AtomicNeuron::Threshold()),
    multicompartment(an_multicompartment),
    constant_current(AtomicNeuron::ConstantCurrent()),
    membrane_capacitance(AtomicNeuron::MembraneCapacitance()),
    adaptation(AtomicNeuron::Adaptation()),
    exponential(AtomicNeuron::Exponential()),
    event_routing(AtomicNeuron::EventRouting()),
    readout(AtomicNeuron::Readout()),
    refractory_period(AtomicNeuron::RefractoryPeriod()),
    bayesian(AtomicNeuron::Bayesian())
{}

MCSafeAtomicNeuron::MCSafeAtomicNeuron(MCSafeAtomicNeuron const& mc_safe_atomic_neuron) :
    excitatory_input(mc_safe_atomic_neuron.excitatory_input),
    inhibitory_input(mc_safe_atomic_neuron.inhibitory_input),
    leak(mc_safe_atomic_neuron.leak),
    reset(mc_safe_atomic_neuron.reset),
    threshold(mc_safe_atomic_neuron.threshold),
    multicompartment(mc_safe_atomic_neuron.multicompartment),
    constant_current(mc_safe_atomic_neuron.constant_current),
    membrane_capacitance(mc_safe_atomic_neuron.membrane_capacitance),
    adaptation(mc_safe_atomic_neuron.adaptation),
    exponential(mc_safe_atomic_neuron.exponential),
    event_routing(mc_safe_atomic_neuron.event_routing),
    readout(mc_safe_atomic_neuron.readout),
    refractory_period(mc_safe_atomic_neuron.refractory_period),
    bayesian(mc_safe_atomic_neuron.bayesian)
{}

MCSafeAtomicNeuron::MCSafeAtomicNeuron(MCSafeAtomicNeuron&& other) :
    excitatory_input(std::move(other.excitatory_input)),
    inhibitory_input(std::move(other.inhibitory_input)),
    leak(std::move(other.leak)),
    reset(std::move(other.reset)),
    threshold(std::move(other.threshold)),
    multicompartment(std::move(other.multicompartment)),
    constant_current(std::move(other.constant_current)),
    membrane_capacitance(std::move(other.membrane_capacitance)),
    adaptation(std::move(other.adaptation)),
    exponential(std::move(other.exponential)),
    event_routing(std::move(other.event_routing)),
    readout(std::move(other.readout)),
    refractory_period(std::move(other.refractory_period)),
    bayesian(std::move(other.bayesian))
{}

MCSafeAtomicNeuron& MCSafeAtomicNeuron::operator=(MCSafeAtomicNeuron&& other)
{
	if (this != &other) {
		excitatory_input = std::move(other.excitatory_input);
		inhibitory_input = std::move(other.inhibitory_input);
		leak = std::move(other.leak);
		reset = std::move(other.reset);
		threshold = std::move(other.threshold);
		multicompartment = std::move(other.multicompartment);
		constant_current = std::move(other.constant_current);
		membrane_capacitance = std::move(other.membrane_capacitance);
		adaptation = std::move(other.adaptation);
		exponential = std::move(other.exponential);
		event_routing = std::move(other.event_routing);
		readout = std::move(other.readout);
		refractory_period = std::move(other.refractory_period);
		bayesian = std::move(other.bayesian);
	}
	return *this;
}

MCSafeAtomicNeuron::operator AtomicNeuron() const
{
	AtomicNeuron atomic_neuron;
	atomic_neuron.excitatory_input = excitatory_input;
	atomic_neuron.inhibitory_input = inhibitory_input;
	atomic_neuron.leak = leak;
	atomic_neuron.reset = reset;
	atomic_neuron.threshold = threshold;
	atomic_neuron.multicompartment = static_cast<AtomicNeuron::Multicompartment>(multicompartment);
	atomic_neuron.constant_current = constant_current;
	atomic_neuron.membrane_capacitance = membrane_capacitance;
	atomic_neuron.adaptation = adaptation;
	atomic_neuron.exponential = exponential;
	atomic_neuron.event_routing = event_routing;
	atomic_neuron.readout = readout;
	atomic_neuron.refractory_period = refractory_period;
	atomic_neuron.bayesian = bayesian;

	return atomic_neuron;
}

bool MCSafeAtomicNeuron::operator==(MCSafeAtomicNeuron const& other) const
{
	return equal(*this, other);
}

bool MCSafeAtomicNeuron::operator!=(MCSafeAtomicNeuron const& other) const
{
	return unequal(*this, other);
}

MCSafeAtomicNeuron& MCSafeAtomicNeuron::operator=(MCSafeAtomicNeuron const& other)
{
	excitatory_input = other.excitatory_input;
	inhibitory_input = other.inhibitory_input;
	leak = other.leak;
	reset = other.reset;
	threshold = other.threshold;
	multicompartment = other.multicompartment;
	constant_current = other.constant_current;
	membrane_capacitance = other.membrane_capacitance;
	adaptation = other.adaptation;
	exponential = other.exponential;
	event_routing = other.event_routing;
	readout = other.readout;
	refractory_period = other.refractory_period;
	bayesian = other.bayesian;
	return *this;
}

std::ostream& operator<<(std::ostream& os, MCSafeAtomicNeuron const& config)
{
	using namespace halco::hicann_dls::vx;
	std::stringstream ss;
	ss << "excitatory_input: " << config.excitatory_input << std::endl;
	ss << "inhibitory_input: " << config.inhibitory_input << std::endl;
	ss << config.leak << std::endl;
	ss << config.reset << std::endl;
	ss << config.threshold << std::endl;
	ss << config.multicompartment << std::endl;
	ss << config.constant_current << std::endl;
	ss << config.membrane_capacitance << std::endl;
	ss << config.adaptation << std::endl;
	ss << config.exponential << std::endl;
	ss << config.event_routing;
	ss << config.readout;
	ss << config.refractory_period;
	ss << config.bayesian;
	os << ss.str();
	return os;
}

// LogicalNeuron implementation.
LogicalNeuron::LogicalNeuron()
{
	MorphologyMap ln;
	morphology = ln;
}

LogicalNeuron::LogicalNeuron(
    Morphology morph,
    Connections connected_neighbors,
    Connections involved_connect_soma_right_neurons)
{
	MorphologyMap ln;
	for (auto const& [compartment_on_ln, compartment_coords] : morph.m_compartments) {
		std::vector<MCSafeAtomicNeuron> vec;
		// Get all AtomicNeurons which belong to current compartment.
		for (auto const& coord : compartment_coords) {
			MCSafeAtomicNeuron neuron(morph.morphology[coord]);
			vec.push_back(neuron);
		}
		ln[compartment_on_ln] = vec;
	}
	m_involved_connect_right_neurons = connected_neighbors;
	m_involved_connect_soma_right_neurons = involved_connect_soma_right_neurons;
	morphology = ln;
}

MCSafeAtomicNeuron& LogicalNeuron::at(
    CompartmentOnLogicalNeuron const& compartment_coord, size_t const& neuron_index)
{
	// Throws if coord is out of range.
	auto& compartment = morphology.at(compartment_coord);
	return compartment[neuron_index];
}

MCSafeAtomicNeuron const& LogicalNeuron::at(
    CompartmentOnLogicalNeuron const& compartment_coord, size_t const& neuron_index) const
{
	// Throws if coord is out of range.
	auto& compartment = morphology.at(compartment_coord);
	return compartment[neuron_index];
}

std::map<CompartmentOnLogicalNeuron, std::vector<AtomicNeuron>> LogicalNeuron::collapse_neuron()
    const
{
	std::map<CompartmentOnLogicalNeuron, std::vector<AtomicNeuron>> res;
	for (auto const& [compartment_on_logical_neuron, mc_safe_atomic_neurons] : morphology) {
		std::vector<AtomicNeuron> tmp;
		for (auto const& mc_safe_atomic_neuron : mc_safe_atomic_neurons) {
			auto atomic_neuron = static_cast<AtomicNeuron>(mc_safe_atomic_neuron);
			tmp.push_back(atomic_neuron);
		}
		res[compartment_on_logical_neuron] = tmp;
	}
	return res;
}

MCSafeAtomicNeuron const& LogicalNeuron::get_neuron(
    std::pair<CompartmentOnLogicalNeuron, size_t> const& index) const
{
	return at(index.first, index.second);
}

void LogicalNeuron::set_neuron(
    std::pair<CompartmentOnLogicalNeuron, size_t> const& index, MCSafeAtomicNeuron const& neuron)
{
	at(index.first, index.second) = neuron;
}

bool LogicalNeuron::operator==(LogicalNeuron const& other) const
{
	return (morphology == other.morphology);
}

bool LogicalNeuron::operator!=(LogicalNeuron const& other) const
{
	return (morphology != other.morphology);
}

std::ostream& operator<<(std::ostream& os, LogicalNeuron const& logical_neuron)
{
	std::stringstream ss;
	for (auto const& compartment : logical_neuron.morphology) {
		ss << "neurons in compartment " << compartment.first << std::endl;
		for (auto const& neuron : compartment.second)
			ss << neuron << std::endl;
	}
	os << ss.str();
	return os;
}

std::pair<LogicalNeuron::AtomicNeuronOnDLS const&, LogicalNeuron::AtomicNeuronOnDLS const&>
LogicalNeuron::get_coordinates(
    LogicalNeuron::Connection const& connection,
    LogicalNeuron::PlacedCompartments placed_compartments) const
{
	auto const& compartment_coords = placed_compartments.at(connection.first.first);
	auto const& first_coord = compartment_coords[connection.first.second];
	auto const& second_coord = compartment_coords[connection.second.second];
	std::pair<LogicalNeuron::AtomicNeuronOnDLS const&, LogicalNeuron::AtomicNeuronOnDLS const&> res(
	    first_coord, second_coord);
	return res;
}

std::pair<MCSafeAtomicNeuron&, MCSafeAtomicNeuron&> LogicalNeuron::get_neurons(
    LogicalNeuron::Connection const& connection, LogicalNeuron::MorphologyMap& morphology) const
{
	auto first_neuron = morphology.at(connection.first.first)[connection.first.second];
	auto second_neuron = morphology.at(connection.second.first)[connection.second.second];
	std::pair<MCSafeAtomicNeuron&, MCSafeAtomicNeuron&> res(first_neuron, second_neuron);
	return res;
}

} // namespace lola::vx

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CHIP_REVISION_STR::LogicalNeuron)
