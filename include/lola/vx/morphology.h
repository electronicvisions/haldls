#pragma once

#ifndef CHIP_REVISION
#error "Requires CHIP_REVISION"
#endif

#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include <utility>
#include <vector>

#define ID(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
// clang-format off
#define CONCAT(w, x, y, z) STR(ID(w)ID(/)ID(x)ID(/)ID(v)ID(y)ID(/)ID(z))
#include CONCAT(lola,vx,CHIP_REVISION,neuron.h)
// clang-format on
#undef ID
#undef XSTR
#undef STR
#undef CONCAT

#if CHIP_REVISION == 3
#define GENPYBIND_TAG_LOLA_VX_VY GENPYBIND_TAG_LOLA_VX_V3
#define CHIP_REVISION_STR v3
#else
#error "Unknown CHIP_REVISION"
#endif

namespace lola::vx::CHIP_REVISION_STR GENPYBIND_TAG_LOLA_VX_VY {
#undef GENPYBIND_TAG_LOLA_VX_VY

class LogicalNeuron;

/**
 * Builder for the LogicalNeuron.
 * The Morphology represents the structure of a MC-neuron, more precise it saves which switches are
 * set but also the whole neuron config. Through the set switches the structure is fully described.
 * It is composed of single AtomicNeurons (denmem neurons).
 */
class GENPYBIND(visible) Morphology
{
public:
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::AtomicNeuronOnLogicalNeuron
	    AtomicNeuronOnLogicalNeuron;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::NeuronColumnOnLogicalNeuron
	    NeuronColumnOnLogicalNeuron;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::NeuronRowOnLogicalNeuron
	    NeuronRowOnLogicalNeuron;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments::Compartment
	    Compartment;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::CompartmentOnLogicalNeuron
	    CompartmentOnLogicalNeuron;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments::Compartments
	    Compartments;
	typedef std::map<AtomicNeuronOnLogicalNeuron, AtomicNeuron::Multicompartment> MorphologyMap;
	MorphologyMap morphology;

	/* Connects neuron to somatic line using a switch.
	 * Enables connect_soma and disables enable_conductance.
	 *
	 * @param coord Neuron which will be connected to somatic line via switch.
	 */
	void connect_to_soma(AtomicNeuronOnLogicalNeuron const& coord) SYMBOL_VISIBLE;

	/* Connects neuron to somatic line using a resistor.
	 * Enables enable_conductance and disables connect_soma.
	 *
	 * @param coord Neuron which will be connected to the somatic line via resistor.
	 */
	void connect_resistor_to_soma(AtomicNeuronOnLogicalNeuron const& coord) SYMBOL_VISIBLE;

	/* Connect all somatic line segments from start to end, including end.
	 *
	 * @param start Left neuron where somatic line will be continuous.
	 * @param end Last neuron to the right where somatic line is continuous.
	 * @param row Row in which the somatic line will be set.
	 */
	void connect_soma_line(
	    NeuronColumnOnLogicalNeuron const& start,
	    NeuronColumnOnLogicalNeuron const& end,
	    NeuronRowOnLogicalNeuron const& row) SYMBOL_VISIBLE;

	/* Connect neurons in compartment and return its index.
	 * This uses the direct membrane connection switches 'connect_right' and
	 * 'connect_vertical'. Therefore the provided compartment has to be made up of a contiguous
	 * block of neuron circuits.
	 *
	 * @param compartment Neurons which will be connected directly via their membrane.
	 * @return Index of the compartment created. */
	CompartmentOnLogicalNeuron create_compartment(Compartment const& compartment) SYMBOL_VISIBLE;

	/**
	 * Get neuron.
	 *
	 * @return AtomicNeurons's Multicompartment configuration.
	 */
	GENPYBIND(expose_as(__getitem__))
	AtomicNeuron::Multicompartment at(AtomicNeuronOnLogicalNeuron const& coord) const
	{
		return morphology.at(coord);
	}

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, Morphology const& morph) SYMBOL_VISIBLE;

	/* Finishing step for creating a MC-Neuron.
	 * Will check if morphology is valid (i.e if all its neurons are properly connected)
	 * and return a pair of the LogicalNeuronCompartments and the LogicalNeuron.
	 * Note that bridge neurons are as well returned as a LogicalNeuronCompartment on their own.
	 * Thereby, bridge neurons are neurons which only have connect_soma_right enabled, and therefore
	 * do not contribute to the functional part of a LogicalNeuron; however, they are relevant for
	 * the structural part of it.
	 *
	 * @throws RuntimeError if provided morphology is not contiguous.
	 * @return Pair of LogicalNeuronCompartments and LogicalNeuron.
	 */
	std::pair<halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronCompartments, LogicalNeuron>
	done() SYMBOL_VISIBLE;

private:
	Compartments m_compartments;

	/* Assign each atomic neuron in morphology to its respective compartment. This will fill
	 * 'm_compartments' with the appropriate compartments.
	 *
	 * This is done by calling extract_compartment() for every neuron in morphology.
	 */
	void define_compartments();

	/* Get all neurons from the set 'unvisited_neurons' which belong to the same compartment as
	 * 'start_neuron' and return the whole compartment.
	 *
	 * This is achieved by a breath first search starting from 'start_neuron' and considering all
	 * neurons of the set 'unvisited_neurons'. Thereby, a stack is used. First, it only contains
	 * the 'start_neuron' and while the stack is not empty, the top element of the stack is poped
	 * and used for 'get_neighbors()'. The neurons returned by 'get_neighbors()' which, were not
	 * already visited, are then stored in the stack. All types of connections are considered in
	 * 'get_neighbors()' except those containing intercompartment conductances.
	 *
	 * @param start_neuron Neuron where the breath first search will start from.
	 * @param unvisited_neurons Set of neurons to look for their belonging to the compartment. If
	 * they are part of the same compartment as 'start_neuron' they get erased from the set.
	 * @return All neurons belonging to the same compartment as 'start_neuron' and are part of
	 * 'unvisited_neurons'.
	 */
	Compartment extract_compartment(
	    AtomicNeuronOnLogicalNeuron const& start_neuron,
	    std::set<AtomicNeuronOnLogicalNeuron>& unvisited_neurons);

	/* Get all neighbors which are directly connected to 'start_neuron'.
	 *
	 * Only neurons within morphology are considered, especially 'start_neuron' must be in
	 * morphology. For that all spatial direct membrane switches of 'start_neuron' are considered
	 * and any somatic connections which don't include inter-compartment conductances. E.g. in the
	 * following configuration B is a neighbor of A, since it is a direct neighbor, while C is not a
	 * neighbor of A, since it uses two direct connections to reach C from A. Furthermore D and F
	 * are neighbors of A, due to the somatic connections, while C is not a neighbor, due to the
	 * inter-compartment conductance (R).
	 *
	 * +-+-+-+-+-+
	 * |     | R |
	 * |     | | |
	 * A-B-C D E F
	 *
	 * @param start_neuron Neuron which neighbors are searched.
	 * @return All neurons which are direct neighbors of start_neuron.
	 */
	Compartment get_neighbors(AtomicNeuronOnLogicalNeuron const& start_neuron);

	/* Check if provided compartments are connected.
	 *
	 * @param comp_set Compartments which are checked for connection.
	 * @return Boolean discribing state of the connection.
	 */
	bool check_compartment_connectivity(Compartments const& comp_sets);

	/* Returns a set of all compartments which are reachable from compartment start_comp.
	 *
	 * This is achieved by a breath first search analog to
	 * Morphology::extract_compartment(AtomicNeuronOnLogicalNeuron,
	 * std::set<AtomicNeuronOnLogicalNeuron>&) except that here connections using the
	 * intercompartment conductance are allowed and only somatic line connections in general are
	 * considered.
	 *
	 * @param start_comp Compartment where the breath first search will start from.
	 * @return All compartments reachable from compartment start_comp.
	 */
	std::set<CompartmentOnLogicalNeuron> walk_neighbors(
	    CompartmentOnLogicalNeuron const& start_comp);

	/* Get all directly neighboring compartments and possibly involved bridge neurons of the
	 * compartment curr_comp_idx.
	 *
	 * Thereby, all somatic line connections going out from the compartment curr_comp_idx are
	 * checked for adjecent compartments.
	 * Bridge neurons, which are neurons that only have connect_soma_right enabled are as well
	 * considered as neighboring compartments, eventhough they do not contribute functionally to the
	 * LogicalNeuron.
	 *
	 * @param curr_comp_idx Compartment which neighbors are searched.
	 * @return All compartments and bridge neurons which are direct neighbors of curr_comp_idx.
	 */
	std::set<CompartmentOnLogicalNeuron> get_neighbors(
	    CompartmentOnLogicalNeuron const& curr_comp_idx);

	// FIXME: The connection information is both stored in the morphology and in this container.
	// However, it would be cleaner to only store this information here and construct the neuron
	// from this infromation.
	/** Remember "connect_right" connections since they change by x-flipping. */
	std::vector<std::pair<AtomicNeuronOnLogicalNeuron, AtomicNeuronOnLogicalNeuron>>
	    m_involved_connect_right;
	/** Remember "connect_soma_right" connections since they change by x-flipping. */
	std::vector<std::pair<AtomicNeuronOnLogicalNeuron, AtomicNeuronOnLogicalNeuron>>
	    m_involved_connect_soma_right;

	/* Connect neurons on opposite rows given their column.
	 *
	 * @param coord Neurons in 'column' will be connected via switch.
	 */
	void connect_vertical(NeuronColumnOnLogicalNeuron const& column);

	/* Connect pair of horizontally neighboring neurons and add them to the morphology and
	 * m_involved_connect_right.
	 *
	 * @param neuron_left Coord of left neuron.
	 * @param neuron_right Coord of right neuron.
	 */
	void connect_right(
	    AtomicNeuronOnLogicalNeuron const& neuron_left,
	    AtomicNeuronOnLogicalNeuron const& neuron_right);

	/** Create string stream which sketches the current structure of the morphology. */
	std::stringstream create_current_config() const;

	friend class LogicalNeuron;
};

/**
 * Helper class for the LogicalNeuron (Multi-Compartment Safe AtomicNeuron).
 *
 * This helper class is completly equivalent to the AtomicNeuron with the exception that it sets
 * members of the Multicompartment struct private, which influence the morphology/structure of a
 * neuron ultimately preserving the morphology/structure of a LogicalNeuron.
 */
class GENPYBIND(visible) MCSafeAtomicNeuron
{
public:
	using AnalogValueVariant = AtomicNeuron::AnalogValueVariant;

	/** Constructors. */
	MCSafeAtomicNeuron() SYMBOL_VISIBLE;
	explicit MCSafeAtomicNeuron(AtomicNeuron atomic_neuron) SYMBOL_VISIBLE;
	explicit MCSafeAtomicNeuron(AtomicNeuron::Multicompartment an_multicompartment) SYMBOL_VISIBLE;
	/** Copy constructor. */
	MCSafeAtomicNeuron(MCSafeAtomicNeuron const& mc_safe_atomic_neuron) SYMBOL_VISIBLE;

	/** Destructor. */
	~MCSafeAtomicNeuron() = default;

	/** Move constructor. */
	MCSafeAtomicNeuron(MCSafeAtomicNeuron&& other) SYMBOL_VISIBLE;

	/** Move assignment operator. */
	MCSafeAtomicNeuron& operator=(MCSafeAtomicNeuron&& other) SYMBOL_VISIBLE;

	/** Copy assignment operator. */
	MCSafeAtomicNeuron& operator=(MCSafeAtomicNeuron const& other) SYMBOL_VISIBLE;

	explicit operator AtomicNeuron() const SYMBOL_VISIBLE;

	using SynapticInput GENPYBIND(visibile) = AtomicNeuron::SynapticInput;
	using Leak GENPYBIND(visible) = AtomicNeuron::Leak;
	using Reset GENPYBIND(visible) = AtomicNeuron::Reset;
	using Threshold GENPYBIND(visible) = AtomicNeuron::Threshold;
	using ConstantCurrent GENPYBIND(visible) = AtomicNeuron::ConstantCurrent;
	using MembraneCapacitance GENPYBIND(visible) = AtomicNeuron::MembraneCapacitance;
	using Adaptation GENPYBIND(visible) = AtomicNeuron::Adaptation;
	using Exponential GENPYBIND(visible) = AtomicNeuron::Exponential;
	using EventRouting GENPYBIND(visible) = AtomicNeuron::EventRouting;
	using Readout GENPYBIND(visible) = AtomicNeuron::Readout;
	using RefractoryPeriod GENPYBIND(visible) = AtomicNeuron::RefractoryPeriod;
	using Bayesian GENPYBIND(visible) = AtomicNeuron::Bayesian;

	struct GENPYBIND(visible) Multicompartment
	{
		Multicompartment() SYMBOL_VISIBLE;
		explicit Multicompartment(AtomicNeuron::Multicompartment an_multicomp) SYMBOL_VISIBLE;
		/** Copy constructor. */
		Multicompartment(Multicompartment const& multicompartment) SYMBOL_VISIBLE;

		/** Destructor. */
		~Multicompartment() = default;

		/** Move constructor. */
		Multicompartment(Multicompartment&& other) SYMBOL_VISIBLE;

		/** Move assignment operator. */
		Multicompartment& operator=(Multicompartment&& other) SYMBOL_VISIBLE;

		/* Copy assignment operator. */
		Multicompartment& operator=(Multicompartment const& other) SYMBOL_VISIBLE;

		/** Enable division (/4) of inter-compartment conductance. */
		bool enable_conductance_division;

		/** Enable multiplication (*4) of inter-compartment conductance. */
		bool enable_conductance_multiplication;

		/** Bias current of inter-compartmental conductance. */
		AnalogValueVariant i_bias_nmda;

		bool operator==(Multicompartment const& other) const SYMBOL_VISIBLE;
		bool operator!=(Multicompartment const& other) const SYMBOL_VISIBLE;

		GENPYBIND(stringstream)
		friend std::ostream& operator<<(std::ostream& os, Multicompartment const& config)
		    SYMBOL_VISIBLE;

		friend struct haldls::vx::detail::VisitPreorderImpl<LogicalNeuron>;
		friend struct haldls::vx::detail::VisitPreorderImpl<MCSafeAtomicNeuron>;

	private:
		/** Members that can change the morphology of a neuron are immutable. */
		/** Connect local membrane to soma. */
		bool connect_soma;

		/** Connect soma to soma on the right. */
		bool connect_soma_right;

		/** Connect local membrane to membrane on the right. */
		bool connect_right;

		/** Connect local membrane vertically to membrane on the opposite hemisphere. */
		bool connect_vertical;

		/** Enable inter-compartment conductance. */
		bool enable_conductance;

		friend struct cereal::access;
		friend class MCSafeAtomicNeuron;
		Multicompartment& operator=(AtomicNeuron::Multicompartment const& other);
		explicit operator AtomicNeuron::Multicompartment() const SYMBOL_VISIBLE;
		template <class Archive>
		void serialize(Archive& ar, std::uint32_t const) SYMBOL_VISIBLE;
	};

	SynapticInput excitatory_input;
	SynapticInput inhibitory_input;
	Leak leak;
	Reset reset;
	Threshold threshold;
	Multicompartment multicompartment;
	ConstantCurrent constant_current;
	MembraneCapacitance membrane_capacitance;
	Adaptation adaptation;
	Exponential exponential;
	EventRouting event_routing;
	Readout readout;
	RefractoryPeriod refractory_period;
	Bayesian bayesian;

	bool operator==(MCSafeAtomicNeuron const& other) const SYMBOL_VISIBLE;
	bool operator!=(MCSafeAtomicNeuron const& other) const SYMBOL_VISIBLE;
	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, MCSafeAtomicNeuron const& config)
	    SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<LogicalNeuron>;
	friend class LogicalNeuron;
};

/**
 * Configuration of a multicompartment/logical neuron.
 */
class GENPYBIND(visible) LogicalNeuron
{
public:
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::LogicalNeuronOnDLS coordinate_type;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::CompartmentOnLogicalNeuron
	    CompartmentOnLogicalNeuron;
	typedef halco::hicann_dls::vx::CHIP_REVISION_STR::AtomicNeuronOnDLS AtomicNeuronOnDLS;
	typedef std::pair<CompartmentOnLogicalNeuron, size_t> NeuronOnLogicalNeuronCompartment;
	typedef std::pair<NeuronOnLogicalNeuronCompartment, NeuronOnLogicalNeuronCompartment>
	    Connection;
	typedef std::map<CompartmentOnLogicalNeuron, std::vector<AtomicNeuronOnDLS>> PlacedCompartments;
	typedef std::false_type has_local_data;

	/** Holds the morphology and configuration of the logical/multicompartment neuron. */
	typedef std::map<CompartmentOnLogicalNeuron, std::vector<MCSafeAtomicNeuron>> MorphologyMap;
	MorphologyMap morphology;

	/** Returns neuron within specified compartment at given index. */
	MCSafeAtomicNeuron& at(
	    CompartmentOnLogicalNeuron const& compartment_coord,
	    size_t const& neuron_index) SYMBOL_VISIBLE;

	MCSafeAtomicNeuron const& at(
	    CompartmentOnLogicalNeuron const& compartment_coord,
	    size_t const& neuron_index) const SYMBOL_VISIBLE;

	/** Convert MCSafeAtomicNeuron to AtomicNeuron inside the morphology. */
	std::map<CompartmentOnLogicalNeuron, std::vector<AtomicNeuron>> collapse_neuron() const
	    SYMBOL_VISIBLE;

	/**
	 * Get neuron within specified compartment at given index.
	 * @param coord Compartment coordinate.
	 * @return Config of neurons in compartment.
	 */
	GENPYBIND(expose_as(__getitem__), return_value_policy(reference))
	MCSafeAtomicNeuron const& get_neuron(
	    std::pair<CompartmentOnLogicalNeuron, size_t> const& index) const SYMBOL_VISIBLE;

	/**
	 * Set neuron within specified compartment at given index.
	 * @param coord Compartment coordinate.
	 * @param neuron Set config for neurons in compartment.
	 */
	GENPYBIND(expose_as(__setitem__))
	void set_neuron(
	    std::pair<CompartmentOnLogicalNeuron, size_t> const& index,
	    MCSafeAtomicNeuron const& neuron) SYMBOL_VISIBLE;

	bool operator==(LogicalNeuron const& other) const SYMBOL_VISIBLE;
	bool operator!=(LogicalNeuron const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, LogicalNeuron const& logical_neuron)
	    SYMBOL_VISIBLE;

	LogicalNeuron() SYMBOL_VISIBLE;

private:
	typedef std::vector<Connection> Connections;

	/**
	 * Remember connected neighbors.
	 * Only store horizontal connections since vertical connections are symmetric wrt to flips.
	 */
	Connections m_involved_connect_right_neurons;
	Connections m_involved_connect_soma_right_neurons;

	/**
	 * Get pair of AtomicNeuronOnDLS from connection provided the placed compartments.
	 * @param connection Connection for which pair of AtomicNeuronOnDLS will be created.
	 * @param placed_compartments Placed compartments of the LogicalNeuron.
	 * @return Pair of coordinates involved in connection (pair of AtomicNeuronOnDLS).
	 */
	std::pair<AtomicNeuronOnDLS const&, AtomicNeuronOnDLS const&> get_coordinates(
	    Connection const& connection, PlacedCompartments placed_compartments) const SYMBOL_VISIBLE;

	/**
	 * Get pair of MCSafeAtomicNeuron from the LogicalNeuron's morphology for given connection.
	 * @param connection Connection between to neurons.
	 * @param morphology Morphology of the LogicalNeuron containing the connection.
	 * @return Reference to the in the connection involved neurons (pair of MCSafeAtomicNeuron).
	 */
	std::pair<MCSafeAtomicNeuron&, MCSafeAtomicNeuron&> get_neurons(
	    Connection const& connection, MorphologyMap& morphology) const SYMBOL_VISIBLE;

	friend class Morphology;
	LogicalNeuron(
	    Morphology morph,
	    Connections connected_neighbors,
	    Connections involved_connect_soma_right_neurons) SYMBOL_VISIBLE;

	friend struct haldls::vx::detail::VisitPreorderImpl<LogicalNeuron>;
};

} // namespace lola::vx

#include "lola/vx/morphology.tcc"
