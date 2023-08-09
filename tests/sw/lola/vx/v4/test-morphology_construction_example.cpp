#include <iostream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lola/vx/v4/morphology.h"

using namespace lola::vx::v4;
using namespace halco::hicann_dls::vx::v4;

TEST(Morphology, Example)
{
	typedef halco::hicann_dls::vx::v4::AtomicNeuronOnLogicalNeuron coord;
	typedef halco::hicann_dls::vx::v4::NeuronColumnOnLogicalNeuron column;
	typedef halco::hicann_dls::vx::v4::NeuronRowOnLogicalNeuron row;

	/*
	HICANN X: A tree structure with proximal and distal dendrites,
	inspired by [Fig. 7, Schemmel et al. 2017]

	        [ distal  ] ["Ca"] [ more distal ]

	    0     1     2     3     4     5     6     7

	0 --+-   -+-----+-----+-----+-----+-----+-   -+--
	    |     |     |     |     |     |     |     |
	         [R]   [R]   [R]   [R]   [R]   [R]
	    |     |     |     |     |     |     |     |
	0 --N-   -N-   -N-   -N-   -N-   -N-   -N-   -N--
	    |     |     |     |     |     |     |     |
	                     [|]
	                      |   <----  (yes, there are two switches here)
	                     [|]
	    |     |     |     |     |     |     |     |
	1 --N-----N-----N-   -N-   -N-   -N-   -N-   -N--
	    |     |     |     |     |     |     |     |
	               [|]   [R]   [R]   [R]   [R]
	    |     |     |     |     |     |     |     |
	1 --+-   -+-   -+-----+-----+-----+-----+-   -+--

	    0     1     2     3     4     5     6     7

	   [ --- "Na" --- ] ["Ca"] [ proximal dend ]
	*/

	auto morphology = Morphology();

	// "Ca"-compartment
	morphology.connect_resistor_to_soma(coord(column(3), row(0)));
	morphology.connect_resistor_to_soma(coord(column(3), row(1)));

	EXPECT_TRUE(morphology.at(coord(column(3), row(0))).enable_conductance);
	EXPECT_TRUE(morphology.at(coord(column(3), row(1))).enable_conductance);

	std::vector<coord> vertical_connection{coord(column(3), row(0)), coord(column(3), row(1))};
	morphology.create_compartment(vertical_connection);
	EXPECT_TRUE(morphology.at(coord(column(3), row(0))).connect_vertical);
	EXPECT_TRUE(morphology.at(coord(column(3), row(1))).connect_vertical);

	// "Na"-compartment
	std::vector<coord> na_comp{
	    coord(column(0), row(1)), coord(column(1), row(1)), coord(column(2), row(1))};
	morphology.create_compartment(na_comp);
	EXPECT_TRUE(morphology.at(na_comp[0]).connect_right);
	EXPECT_TRUE(morphology.at(na_comp[1]).connect_right);

	morphology.connect_to_soma(coord(column(2), row(1)));
	EXPECT_TRUE(morphology.at(coord(column(2), row(1))).connect_soma);
	EXPECT_FALSE(morphology.at(coord(column(2), row(1))).enable_conductance);

	// distal dendrites
	morphology.connect_soma_line(column(1), column(6), row(0));

	// we expect 5 connect rights to be true from 1 to 5
	for (int i = 1; i < 6; ++i)
		EXPECT_TRUE(morphology.at(coord(column(i), row(0))).connect_soma_right);

	int columns_distal_dendrites[] = {1, 2, 4, 5, 6};
	for (auto col : columns_distal_dendrites) {
		morphology.connect_resistor_to_soma(coord(column(col), row(0)));
		EXPECT_FALSE(morphology.at(coord(column(col), row(0))).connect_soma);
		EXPECT_TRUE(morphology.at(coord(column(col), row(0))).enable_conductance);
	}

	// proximal dendrites
	morphology.connect_soma_line(column(2), column(6), row(1));

	int columns_proximal_dendrites[] = {4, 5, 6};
	for (auto col : columns_proximal_dendrites) {
		morphology.connect_resistor_to_soma(coord(column(col), row(1)));
	}
}

TEST(Morphology, Chain)
{
	typedef halco::hicann_dls::vx::v4::AtomicNeuronOnLogicalNeuron coord;
	typedef halco::hicann_dls::vx::v4::NeuronColumnOnLogicalNeuron column;
	typedef halco::hicann_dls::vx::v4::NeuronRowOnLogicalNeuron row;

	auto morphology = Morphology();

	// test vertical connector
	std::vector<coord> vertical_connection{coord(column(0), row(0)), coord(column(0), row(1))};
	morphology.create_compartment(vertical_connection);
	EXPECT_TRUE(morphology.at(coord(column(0), row(0))).connect_vertical);
	EXPECT_TRUE(morphology.at(coord(column(0), row(1))).connect_vertical);

	std::vector<coord> vertical_connection2{coord(column(127), row(0)), coord(column(127), row(1))};
	morphology.create_compartment(vertical_connection2);
	EXPECT_TRUE(morphology.at(coord(column(127), row(0))).connect_vertical);
	EXPECT_TRUE(morphology.at(coord(column(127), row(1))).connect_vertical);

	// test morphologies connecting the membrane
	auto morph_chain = Morphology();
	std::vector<coord> chain{coord(column(0), row(0)), coord(column(1), row(0)),
	                         coord(column(2), row(0)), coord(column(2), row(1)),
	                         coord(column(3), row(1)), coord(column(4), row(1)),
	                         coord(column(4), row(0)), coord(column(5), row(0))};

	// morph_chain: ▀▀█▄█▀
	morph_chain.create_compartment(chain);

	EXPECT_TRUE(morph_chain.at(chain[0]).connect_right);
	EXPECT_FALSE(morph_chain.at(chain[0]).connect_vertical);
	EXPECT_FALSE(morph_chain.at(chain[1]).connect_vertical);
	EXPECT_TRUE(morph_chain.at(chain[1]).connect_right);
	EXPECT_TRUE(morph_chain.at(chain[2]).connect_vertical);
	EXPECT_FALSE(morph_chain.at(chain[2]).connect_right);
	EXPECT_TRUE(morph_chain.at(chain[3]).connect_vertical);
	EXPECT_TRUE(morph_chain.at(chain[3]).connect_right);
	EXPECT_FALSE(morph_chain.at(chain[4]).connect_vertical);
	EXPECT_TRUE(morph_chain.at(chain[4]).connect_right);
	EXPECT_TRUE(morph_chain.at(chain[5]).connect_vertical);
	EXPECT_FALSE(morph_chain.at(chain[5]).connect_right);
	EXPECT_TRUE(morph_chain.at(chain[6]).connect_vertical);
	EXPECT_TRUE(morph_chain.at(chain[6]).connect_right);
	EXPECT_FALSE(morph_chain.at(chain[7]).connect_vertical);
	EXPECT_FALSE(morph_chain.at(chain[7]).connect_right);

	// test: violating contiguousness (by deleting elements within the chain created above)
	for (auto it = std::next(chain.begin(), 1); it < std::prev(chain.end(), 2); ++it) {
		auto morph_chain_interrupted = Morphology();
		std::vector<coord> interrupted_chain;
		std::copy_if(
		    chain.begin(), chain.end(), std::back_inserter(interrupted_chain),
		    [it](auto t) { return t != *it; });
		EXPECT_THROW(
		    morph_chain_interrupted.create_compartment(interrupted_chain), std::runtime_error);
	}
}

TEST(Morphology, NeuronDoubleConnection2Soma)
{
	typedef halco::hicann_dls::vx::v4::AtomicNeuronOnLogicalNeuron coord;
	typedef halco::hicann_dls::vx::v4::NeuronColumnOnLogicalNeuron column;
	typedef halco::hicann_dls::vx::v4::NeuronRowOnLogicalNeuron row;

	auto morphology = Morphology();

	morphology.connect_to_soma(coord(column(0), row(0)));
	EXPECT_THROW(morphology.connect_resistor_to_soma(coord(column(0), row(0))), std::runtime_error);

	morphology.connect_resistor_to_soma(coord(column(1), row(0)));
	EXPECT_THROW(morphology.connect_to_soma(coord(column(1), row(0))), std::runtime_error);
}

TEST(MCSafeAtomicNeuron, Assignment)
{
	auto atomic_neuron = AtomicNeuron();

	atomic_neuron.multicompartment.connect_soma_right = true;

	auto mc_safe_atomic_neuron = MCSafeAtomicNeuron();
	auto mc_safe_atomic_neuron_2 = MCSafeAtomicNeuron(atomic_neuron);
	mc_safe_atomic_neuron = mc_safe_atomic_neuron_2;

	EXPECT_FALSE(mc_safe_atomic_neuron == mc_safe_atomic_neuron_2);
}

TEST(LogicalNeuron, SkipNeuronWithSoma)
{
	typedef halco::hicann_dls::vx::v4::AtomicNeuronOnLogicalNeuron coord;
	typedef halco::hicann_dls::vx::v4::NeuronColumnOnLogicalNeuron column;
	typedef halco::hicann_dls::vx::v4::NeuronRowOnLogicalNeuron row;

	// initialize the builder
	auto morphology = Morphology();

	// add some neurons and manipulate their values
	std::vector<coord> vertical_connection{coord(column(0), row(0)), coord(column(0), row(1))};
	morphology.create_compartment(vertical_connection);
	EXPECT_TRUE(morphology.at(coord(column(0), row(0))).connect_vertical);
	EXPECT_TRUE(morphology.at(coord(column(0), row(1))).connect_vertical);
	morphology.connect_resistor_to_soma(coord(column(2), row(0)));

	// generate the LogicalNeuron -> not contiguous
	EXPECT_THROW(auto mc_neuron = morphology.done(), std::runtime_error);

	// make morphology contiguous
	morphology.connect_soma_line(column(0), column(2), row(0)); // still not allowed
	EXPECT_THROW(auto mc_neuron = morphology.done(), std::runtime_error);
	// now it is congituous
	morphology.connect_to_soma(coord(column(0), row(0)));
	auto mc_neuron_done = morphology.done();
}

TEST(LogicalNeuron, CompartmentWithSomaticLine)
{
	typedef halco::hicann_dls::vx::v4::AtomicNeuronOnLogicalNeuron coord;
	typedef halco::hicann_dls::vx::v4::NeuronColumnOnLogicalNeuron column;
	typedef halco::hicann_dls::vx::v4::NeuronRowOnLogicalNeuron row;
	auto morph = Morphology();
	std::vector<coord> vertical_connection{coord(column(1), row(0)), coord(column(1), row(1))};
	std::vector<coord> vertical_connection2{coord(column(2), row(0)), coord(column(2), row(1))};
	morph.create_compartment(vertical_connection);
	morph.create_compartment(vertical_connection2);

	morph.connect_soma_line(column(0), column(3), row(0));
	morph.connect_soma_line(column(0), column(1), row(1));
	morph.connect_soma_line(column(2), column(3), row(1));
	morph.connect_to_soma(coord(column(0), row(0)));
	morph.connect_to_soma(coord(column(3), row(0)));
	morph.connect_to_soma(coord(column(0), row(1)));
	morph.connect_to_soma(coord(column(3), row(1)));
	morph.connect_resistor_to_soma(coord(column(1), row(0)));
	morph.connect_resistor_to_soma(coord(column(2), row(0)));
	morph.connect_resistor_to_soma(coord(column(1), row(1)));
	morph.connect_resistor_to_soma(coord(column(2), row(1)));

	auto rather_complex_neuron_done = morph.done();
	LogicalNeuron rather_complex_neuron = rather_complex_neuron_done.second;
}

TEST(LogicalNeuron, Multicompartment)
{
	/* Another more complex morphology structure
	 *
	 *   ┌---┬-┬-┐
	 *   |   R R R
	 * N-N n v c b
	 * |   |
	 * N n-n
	 * | R
	 * └-┴
	 * This compartment consists of 5 compartments (indicated by letters {N,n,v,c,b}) in the
	 * biological sense (compartments are seperated from each other via a resistor (R))
	 * */

	typedef halco::hicann_dls::vx::v4::AtomicNeuronOnLogicalNeuron coord;
	typedef halco::hicann_dls::vx::v4::NeuronColumnOnLogicalNeuron column;
	typedef halco::hicann_dls::vx::v4::NeuronRowOnLogicalNeuron row;

	typedef halco::hicann_dls::vx::v4::CompartmentOnLogicalNeuron comp_coord;

	auto morph = Morphology();
	std::vector<coord> N{
	    coord(column(0), row(0)), coord(column(1), row(0)), coord(column(0), row(1))};
	std::vector<coord> n{
	    coord(column(1), row(1)), coord(column(2), row(1)), coord(column(2), row(0))};
	morph.create_compartment(N);
	morph.create_compartment(n);
	morph.connect_resistor_to_soma(coord(column(4), row(0)));
	morph.connect_resistor_to_soma(coord(column(1), row(1)));
	morph.connect_resistor_to_soma(coord(column(3), row(0)));
	morph.connect_resistor_to_soma(coord(column(5), row(0)));
	morph.connect_to_soma(coord(column(1), row(0)));
	morph.connect_to_soma(coord(column(0), row(1)));
	morph.connect_soma_line(column(1), column(5), row(0));
	morph.connect_soma_line(column(0), column(1), row(1));

	auto mc_neuron_done = morph.done();
	LogicalNeuron mc_neuron = mc_neuron_done.second;
	MCSafeAtomicNeuron mc_safe_neuron = mc_neuron.at(comp_coord(0), 0);
	EXPECT_FALSE(mc_safe_neuron.multicompartment.enable_conductance_division);
	mc_safe_neuron.multicompartment.enable_conductance_division = true;
	EXPECT_TRUE(mc_safe_neuron.multicompartment.enable_conductance_division);
	EXPECT_FALSE(mc_safe_neuron.leak.enable_division);
	mc_safe_neuron.leak.enable_division = true;
	EXPECT_TRUE(mc_safe_neuron.leak.enable_division);
}
