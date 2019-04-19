#include <gtest/gtest.h>

#include "test-helper.h"
#include "haldls/v2/chip.h"
#include "halco/common/iter_all.h"
#include "stadls/visitors.h"

using namespace haldls::v2;
using namespace halco::hicann_dls::v2;
using namespace halco::common;

TEST(Chip, General)
{
	Chip chip;

	// test getter/setter: neuron
	NeuronDigitalConfig neuron;
	neuron.set_enable_leak(true);
	chip.set_neuron_digital_config(NeuronOnDLS(5), neuron);
	ASSERT_EQ(chip.get_neuron_digital_config(NeuronOnDLS(5)), neuron);

	// test getter/setter: synapse
	SynapseBlock::Synapse synapse;
	synapse.set_weight(SynapseBlock::Synapse::Weight(4));
	synapse.set_address(SynapseBlock::Synapse::Address(5));
	chip.set_synapse(SynapseOnDLS(Enum(3)), synapse);
	ASSERT_EQ(chip.get_synapse(SynapseOnDLS(Enum(3))), synapse);
	ASSERT_NE(chip.get_synapse(SynapseOnDLS(Enum(0))), synapse);

	// test getter/setter: synapse_block
	SynapseBlock synapse_block;
	synapse_block.set_synapse(SynapseOnSynapseBlock(3), synapse);
	chip.set_synapse_block(SynapseBlockOnDLS(X(0), Y(1)), synapse_block);
	SynapseBlock synapse_block_eq_1(chip.get_synapse_block(SynapseBlockOnDLS(X(0), Y(1))));
	ASSERT_EQ(synapse_block_eq_1, synapse_block);
	//test if single synapse setter in line 26 modifies correct block
	SynapseBlock synapse_block_eq_2(chip.get_synapse_block(SynapseBlockOnDLS(X(0), Y(0))));
	ASSERT_EQ(synapse_block_eq_2, synapse_block);
	// test if first synapse block in the third row are not equal
	SynapseBlock synapse_block_ne(chip.get_synapse_block(SynapseBlockOnDLS(X(0), Y(2))));
	ASSERT_NE(synapse_block_ne, synapse_block);

	ColumnCorrelationBlock::ColumnCorrelationSwitch corr_switch;
	corr_switch.set_causal_config(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::internal);
	chip.set_column_correlation_switch(ColumnCorrelationSwitchOnDLS(3), corr_switch);
	ASSERT_EQ(chip.get_column_correlation_switch(ColumnCorrelationSwitchOnDLS(3)), corr_switch);

	ColumnCorrelationBlock corr_block;
	corr_block.set_switch(ColumnCorrelationSwitchOnColumnCorrelationBlock(3), corr_switch);
	chip.set_column_correlation_block(ColumnCorrelationBlockOnDLS(7), corr_block);
	ASSERT_EQ(chip.get_column_correlation_block(ColumnCorrelationBlockOnDLS(7)), corr_block);
	//test if single correlation switch setter modifies correct block
	ASSERT_EQ(chip.get_column_correlation_block(ColumnCorrelationBlockOnDLS(0)), corr_block);

	ColumnCurrentBlock::ColumnCurrentSwitch curr_switch;
	curr_switch.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::internal);
	chip.set_column_current_switch(ColumnCurrentSwitchOnDLS(2), curr_switch);
	ASSERT_EQ(chip.get_column_current_switch(ColumnCurrentSwitchOnDLS(2)), curr_switch);

	ColumnCurrentBlock curr_block;
	curr_block.set_switch(ColumnCurrentSwitchOnColumnBlock(2), curr_switch);
	chip.set_column_current_block(ColumnBlockOnDLS(6), curr_block);
	ASSERT_EQ(chip.get_column_current_block(ColumnBlockOnDLS(6)), curr_block);
	//test if single current switch setter modifies correct block
	ASSERT_EQ(chip.get_column_current_block(ColumnBlockOnDLS(0)), curr_block);

	CapMem capmem;
	capmem.set(CapMemCellOnDLS(Enum(4)), CapMemCell::Value(123));
	chip.set_capmem(capmem);
	ASSERT_EQ(chip.get_capmem(), capmem);

	PPUMemory ppu_memory;
	PPUMemory::words_type test_mem{{PPUMemoryWord(PPUMemoryWord::Value(0))}};
	std::for_each(
			test_mem.begin(),
			test_mem.end(),
			[](PPUMemoryWord& x) { x = PPUMemoryWord(PPUMemoryWord::Value(std::rand())); });
	ppu_memory.set_words(test_mem);
	chip.set_ppu_memory(ppu_memory);
	ASSERT_EQ(chip.get_ppu_memory(), ppu_memory);

	PPUControlRegister control_register;
	control_register.set_inhibit_reset(true);
	chip.set_ppu_control_register(control_register);
	ASSERT_EQ(chip.get_ppu_control_register(), control_register);

	RateCounterConfig rate;
	rate.set_enable_neuron(NeuronOnDLS(4), true);
	chip.set_rate_counter_config(rate);
	ASSERT_EQ(chip.get_rate_counter_config(), rate);

	SynapseDriverBlock syndriver_config;
	syndriver_config.set_pulse_length(SynapseDriverBlock::PulseLength(22));
	chip.set_synapse_drivers(syndriver_config);
	ASSERT_EQ(chip.get_synapse_drivers(), syndriver_config);

	CommonSynramConfig synram_config;
	synram_config.set_pc_conf(CommonSynramConfig::PCConf(14));
	chip.set_common_synram_config(synram_config);
	ASSERT_EQ(chip.get_common_synram_config(), synram_config);

	CapMemConfig capmem_config;
	capmem_config.set_sub_counter(CapMemConfig::SubCounter(1234));
	chip.set_capmem_config(capmem_config);
	ASSERT_EQ(chip.get_capmem_config(), capmem_config);

	CommonNeuronConfig neuron_config;
	neuron_config.set_post_correlation_signal_length(
	    CommonNeuronConfig::PostCorrelationSignalLength(12));
	chip.set_common_neuron_config(neuron_config);
	ASSERT_EQ(chip.get_common_neuron_config(), neuron_config);

	CorrelationConfig correlation_config;
	correlation_config.set_sense_delay(CorrelationConfig::Delay(12));
	chip.set_correlation_config(correlation_config);
	ASSERT_EQ(chip.get_correlation_config(), correlation_config);

	// test relation operator
	Chip chip2 = chip;
	Chip chip3;
	capmem_config.set_sub_counter(CapMemConfig::SubCounter(4321));
	chip3.set_capmem_config(capmem_config);

	ASSERT_EQ(chip, chip2);
	ASSERT_EQ(chip == chip3, false);

	ASSERT_NE(chip, chip3);
	ASSERT_EQ(chip != chip2, false);

	// check neuron current input and voltage output setter
	NeuronOnDLS test_neuron_coord(14);
	NeuronOnDLS test_neuron_coord2(10);

	ASSERT_FALSE(chip.get_buffered_readout_neuron());

	chip.enable_buffered_readout(test_neuron_coord);

	ASSERT_TRUE(bool(chip.get_buffered_readout_neuron()));
	ASSERT_EQ(chip.get_buffered_readout_neuron(), test_neuron_coord);

	for (auto neuron : iter_all<NeuronOnDLS>()) {
		auto config = chip.get_neuron_digital_config(neuron);
		ASSERT_FALSE(config.get_enable_buffered_readout());
	}

	chip.enable_buffered_readout(test_neuron_coord2);
	ASSERT_EQ(chip.get_buffered_readout_neuron(), test_neuron_coord2);

	for (auto neuron : iter_all<NeuronOnDLS>()) {
		auto config = chip.get_neuron_digital_config(neuron);
		ASSERT_FALSE(config.get_enable_buffered_readout());
	}

	ASSERT_EQ(chip.get_buffered_readout_neuron(), test_neuron_coord2);

	auto config2 = chip.get_neuron_digital_config(test_neuron_coord2);
	ASSERT_FALSE(config2.get_enable_buffered_readout());

	chip.set_neuron_digital_config(test_neuron_coord, config2);
	auto config = chip.get_neuron_digital_config(test_neuron_coord);
	ASSERT_FALSE(config.get_enable_buffered_readout());
	ASSERT_EQ(config2, config);

	// Unaffected by the call to set_neuron_digital_config().
	ASSERT_EQ(chip.get_buffered_readout_neuron(), test_neuron_coord2);

	chip.disable_buffered_readout();
	ASSERT_FALSE(chip.get_buffered_readout_neuron());

}

TEST(Chip, CerealizeCoverage)
{
	Chip obj1,obj2;

	{
		auto config = obj1.get_common_synram_config();
		config.set_use_internal_i_bias_vdac(!config.get_use_internal_i_bias_vdac());
		obj1.set_common_synram_config(config);
	}
	{
		auto config = obj1.get_capmem_config();
		config.set_enable_capmem(!config.get_enable_capmem());
		obj1.set_capmem_config(config);
	}
	{
		auto config = obj1.get_common_neuron_config();
		config.set_enable_digital_out(!config.get_enable_digital_out());
		obj1.set_common_neuron_config(config);
	}
	{
		auto config = obj1.get_correlation_config();
		config.set_sense_delay(draw_ranged_non_default_value<CorrelationConfig::Delay>(config.get_sense_delay()));
		obj1.set_correlation_config(config);
	}
	{
		auto config = obj1.get_synapse_drivers();
		config.set_mode(SynapseDriverOnDLS(0), SynapseDriverBlock::Mode::inhibitory);
		obj1.set_synapse_drivers(config);
	}
	{
		auto config = obj1.get_rate_counter_config();
		config.set_enable_fire_interrupt(!config.get_enable_fire_interrupt());
		obj1.set_rate_counter_config(config);
	}
	// RateCounter values not settable
	// PPUStatusRegister not settable
	{
		auto config = obj1.get_ppu_control_register();
		config.set_inhibit_reset(!config.get_inhibit_reset());
		obj1.set_ppu_control_register(config);
	}
	{
		auto config = obj1.get_ppu_memory();
		auto word = draw_ranged_non_default_value<PPUMemoryWord::Value>(config.get_word(PPUMemoryWordOnDLS(0)).value());
		config.set_word(PPUMemoryWordOnDLS(0), word);
		obj1.set_ppu_memory(config);
	}
	{
		auto config = obj1.get_capmem();
		config.set(NeuronOnDLS(0), halco::hicann_dls::v2::NeuronParameter::v_leak, draw_ranged_non_default_value<CapMemCell::Value>(config.get(NeuronOnDLS(0), halco::hicann_dls::v2::NeuronParameter::v_leak)));
		obj1.set_capmem(config);
	}
	{
		auto config = obj1.get_column_current_switch(ColumnCurrentSwitchOnDLS(0));
		config.set_exc_config(ColumnCurrentBlock::ColumnCurrentSwitch::Config::readout);
		obj1.set_column_current_switch(ColumnCurrentSwitchOnDLS(0), config);
	}
	{
		auto config = obj1.get_column_correlation_switch(ColumnCorrelationSwitchOnDLS(0));
		config.set_causal_config(ColumnCorrelationBlock::ColumnCorrelationSwitch::Config::readout);
		obj1.set_column_correlation_switch(ColumnCorrelationSwitchOnDLS(0), config);
	}
	// {Causal,Acausal}CorrelationBlock not settable
	for (auto syn: iter_all<SynapseOnDLS>()) {
		auto config = obj1.get_synapse(syn);
		config.set_weight(draw_ranged_non_default_value<SynapseBlock::Synapse::Weight>(0));
		obj1.set_synapse(syn, config);
	}
	for (auto neuron: iter_all<NeuronOnDLS>()) {
		auto config = obj1.get_neuron_digital_config(neuron);
		config.set_enable_synapse_input_excitatory(!config.get_enable_synapse_input_excitatory());
		obj1.set_neuron_digital_config(neuron, config);
	}
	obj1.enable_buffered_readout(NeuronOnDLS(rand() % NeuronOnDLS::size));

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}
