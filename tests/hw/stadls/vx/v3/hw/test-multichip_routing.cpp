#include "fisch/vx/constants.h"
#include "haldls/vx/v3/barrier.h"
#include "lola/vx/v3/routing_table.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/run.h"

#include "multichip_test_helper.h"

#include <variant>
#include <gtest/gtest.h>

// Loopback of a single spike
TEST(Multichip, RoutingLoopback)
{
	using namespace haldls::vx::v3;

	auto connection = hxcomm::vx::get_connection_from_env();
	if (!is_jboa_multichip_setup(connection)) {
		GTEST_SKIP();
	}

	size_t const connection_size = std::visit([](auto& conn) { return conn.size(); }, connection);
	auto const hwdb_entries =
	    std::visit([](auto const& connection) { return connection.get_hwdb_entry(); }, connection);

	// Increase instruction timeout so that sync barriers do not timeout
	auto timeout = InstructionTimeoutConfig();
	timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));

	std::vector<stadls::vx::v3::PlaybackProgramBuilder> builders;
	for (size_t i = 0; i < connection_size; i++) {
		auto [builder, _] =
		    stadls::vx::v3::generate(stadls::vx::v3::SystemInit(hwdb_entries.at(i)));
		builders.push_back(std::move(builder));
	}

	for (size_t i = 0; i < connection_size; i++) {
		// Larger instruction timout
		builders.at(i).write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);
	}

	// Set output routing table
	lola::vx::v3::OutputRoutingTable output_routing_table;
	haldls::vx::v3::OutputRoutingTableEntry output_entry;
	output_entry.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(347));
	output_entry.set_enable(true);

	output_routing_table.entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(17)] =
	    output_entry;
	builders.at(0).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table);

	// Set input routing table
	lola::vx::v3::InputRoutingTable input_routing_table;
	haldls::vx::v3::InputRoutingTableEntry input_entry;
	input_entry.set_label(haldls::vx::v3::InputRoutingTableEntry::Label(20));
	input_entry.set_enable(true);
	input_routing_table.entries[halco::hicann_dls::vx::InputRoutingTableEntryOnFPGA(347)] =
	    input_entry;
	builders.at(0).write(halco::hicann_dls::vx::InputRoutingTableOnFPGA(), input_routing_table);

	// Sync barrier
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
	}

	// Send spikes
	auto spike_label = halco::hicann_dls::vx::v3::SpikeLabel(17);
	haldls::vx::v3::SpikePack1ToChip::labels_type labels;
	labels.fill(spike_label);
	haldls::vx::v3::SpikePack1ToChip spike(labels);
	builders.at(0).write(halco::hicann_dls::vx::SpikePack1ToChipOnDLS(), spike);

	// Run finished programs
	std::vector<stadls::vx::v3::PlaybackProgram> programs;
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer());
		builders.at(i).block_until(
		    halco::hicann_dls::vx::v3::TimerOnDLS(),
		    haldls::vx::v3::Timer::Value(int(1 * fisch::vx::fpga_clock_cycles_per_s)));
		programs.push_back(builders.at(i).done());
	}

	std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;
	for (auto& playback_program : programs) {
		programs_wrapped.push_back(playback_program);
	}
	stadls::vx::v3::run(connection, programs_wrapped);

	auto spikes_0 = programs.at(0).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_0;
	for (auto const& spike : spikes_0) {
		spike_labels_0.insert(spike.label);
	}
	EXPECT_EQ(spikes_0.size(), 2);
	EXPECT_TRUE(spike_labels_0.contains(halco::hicann_dls::vx::v3::SpikeLabel(17)));
	EXPECT_TRUE(spike_labels_0.contains(halco::hicann_dls::vx::v3::SpikeLabel(20)));
	EXPECT_EQ(spike_labels_0.size(), 2);
}


// Sending a single spike to another chip: 0 -> 1
TEST(Multichip, RoutingOneToOne)
{
	using namespace haldls::vx::v3;

	auto connection = hxcomm::vx::get_connection_from_env();
	if (!is_jboa_multichip_of_min_size(connection, 2)) {
		GTEST_SKIP();
	}

	size_t const connection_size = std::visit([](auto& conn) { return conn.size(); }, connection);
	auto const hwdb_entries =
	    std::visit([](auto const& connection) { return connection.get_hwdb_entry(); }, connection);

	// Increase instruction timeout so that sync barriers do not timeout
	auto timeout = InstructionTimeoutConfig();
	timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));

	std::vector<stadls::vx::v3::PlaybackProgramBuilder> builders;
	for (size_t i = 0; i < connection_size; i++) {
		auto [builder, _] =
		    stadls::vx::v3::generate(stadls::vx::v3::SystemInit(hwdb_entries.at(i)));
		builders.push_back(std::move(builder));
	}

	for (size_t i = 0; i < connection_size; i++) {
		// Larger instruction timout
		builders.at(i).write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);
	}

	// Define spike labels and routing labels
	std::vector<size_t> spike_labels{17, 2384, 24398, 5, 28};
	std::vector<size_t> routing_labels{347, 9823, 4587, 17};
	std::vector<size_t> target_spike_labels{1, 2, 3, 4};

	// Set output routing table
	lola::vx::v3::OutputRoutingTable output_routing_table;
	for (size_t i = 0; i < routing_labels.size(); i++) {
		haldls::vx::v3::OutputRoutingTableEntry output_entry;
		output_entry.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(routing_labels[i]));
		output_entry.set_enable(true);

		output_routing_table
		    .entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(spike_labels[i])] =
		    output_entry;
	}
	builders.at(0).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table);
	builders.at(0).write(
	    halco::hicann_dls::vx::InputRoutingTableOnFPGA(), lola::vx::v3::InputRoutingTable());

	// Set input routing table
	lola::vx::v3::InputRoutingTable input_routing_table;
	for (size_t i = 0; i < routing_labels.size(); i++) {
		haldls::vx::v3::InputRoutingTableEntry input_entry;
		input_entry.set_label(
		    haldls::vx::v3::InputRoutingTableEntry::Label(target_spike_labels[i]));
		input_entry.set_enable(true);
		input_routing_table
		    .entries[halco::hicann_dls::vx::InputRoutingTableEntryOnFPGA(routing_labels[i])] =
		    input_entry;
	}
	builders.at(1).write(halco::hicann_dls::vx::InputRoutingTableOnFPGA(), input_routing_table);
	builders.at(1).write(
	    halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), lola::vx::v3::OutputRoutingTable());

	// Sync barrier
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
	}

	// Send spikes
	for (auto const& spike_index : spike_labels) {
		auto spike_label = halco::hicann_dls::vx::v3::SpikeLabel(spike_index);
		haldls::vx::v3::SpikePack1ToChip::labels_type labels;
		labels.fill(spike_label);
		haldls::vx::v3::SpikePack1ToChip spike(labels);
		builders.at(0).write(halco::hicann_dls::vx::SpikePack1ToChipOnDLS(), spike);
	}

	// Run finished programs
	std::vector<stadls::vx::v3::PlaybackProgram> programs;
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer());
		builders.at(i).block_until(
		    halco::hicann_dls::vx::v3::TimerOnDLS(),
		    haldls::vx::v3::Timer::Value(int(1 * fisch::vx::fpga_clock_cycles_per_s)));
		programs.push_back(builders.at(i).done());
	}

	std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;
	for (auto& playback_program : programs) {
		programs_wrapped.push_back(playback_program);
	}
	stadls::vx::v3::run(connection, programs_wrapped);

	auto spikes_0 = programs.at(0).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_0;
	for (auto const& spike : spikes_0) {
		spike_labels_0.insert(spike.label);
	}
	for (auto const& spike_index : spike_labels) {
		EXPECT_TRUE(spike_labels_0.contains(halco::hicann_dls::vx::v3::SpikeLabel(spike_index)));
	}
	EXPECT_EQ(spike_labels_0.size(), 5);

	auto spikes_1 = programs.at(1).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_1;
	for (auto const& spike : spikes_1) {
		spike_labels_1.insert(spike.label);
	}
	for (auto const& spike_index : target_spike_labels) {
		EXPECT_TRUE(spike_labels_1.contains(halco::hicann_dls::vx::v3::SpikeLabel(spike_index)));
	}
	EXPECT_EQ(spike_labels_1.size(), 4);
}


// Sending a single spike through a chain of chips: 0 -> 1 -> 2 -> 3
TEST(Multichip, RoutingChain)
{
	using namespace haldls::vx::v3;

	auto connection = hxcomm::vx::get_connection_from_env();
	if (!is_jboa_multichip_of_min_size(connection, 4)) {
		GTEST_SKIP();
	}

	size_t const connection_size = std::visit([](auto& conn) { return conn.size(); }, connection);
	auto const hwdb_entries =
	    std::visit([](auto const& connection) { return connection.get_hwdb_entry(); }, connection);

	// Increase instruction timeout so that sync barriers do not timeout
	auto timeout = InstructionTimeoutConfig();
	timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));

	std::vector<stadls::vx::v3::PlaybackProgramBuilder> builders;
	for (size_t i = 0; i < connection_size; i++) {
		auto [builder, _] =
		    stadls::vx::v3::generate(stadls::vx::v3::SystemInit(hwdb_entries.at(i)));
		builders.push_back(std::move(builder));
	}

	for (size_t i = 0; i < connection_size; i++) {
		// Larger instruction timout
		builders.at(i).write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);
	}

	// Set output routing table on chip 0
	lola::vx::v3::OutputRoutingTable output_routing_table_0;
	haldls::vx::v3::OutputRoutingTableEntry output_entry_0;
	output_entry_0.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(347));
	output_entry_0.set_enable(true);

	output_routing_table_0.entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(0)] =
	    output_entry_0;
	builders.at(0).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table_0);
	builders.at(0).write(
	    halco::hicann_dls::vx::InputRoutingTableOnFPGA(), lola::vx::v3::InputRoutingTable());

	// Set input routing table on chip 1
	lola::vx::v3::InputRoutingTable input_routing_table_1;
	haldls::vx::v3::InputRoutingTableEntry input_entry_1;
	input_entry_1.set_label(haldls::vx::v3::InputRoutingTableEntry::Label(1));
	input_entry_1.set_enable(true);
	input_routing_table_1.entries[halco::hicann_dls::vx::InputRoutingTableEntryOnFPGA(347)] =
	    input_entry_1;
	builders.at(1).write(halco::hicann_dls::vx::InputRoutingTableOnFPGA(), input_routing_table_1);

	// Set output routing table on chip 1
	lola::vx::v3::OutputRoutingTable output_routing_table_1;
	haldls::vx::v3::OutputRoutingTableEntry output_entry_1;
	output_entry_1.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(6923));
	output_entry_1.set_enable(true);

	output_routing_table_1.entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(1)] =
	    output_entry_1;
	builders.at(1).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table_1);

	// Set input routing table on chip 2
	lola::vx::v3::InputRoutingTable input_routing_table_2;
	haldls::vx::v3::InputRoutingTableEntry input_entry_2;
	input_entry_2.set_label(haldls::vx::v3::InputRoutingTableEntry::Label(2));
	input_entry_2.set_enable(true);
	input_routing_table_2.entries[halco::hicann_dls::vx::InputRoutingTableEntryOnFPGA(6923)] =
	    input_entry_2;
	builders.at(2).write(halco::hicann_dls::vx::InputRoutingTableOnFPGA(), input_routing_table_2);

	// Set output routing table on chip 2
	lola::vx::v3::OutputRoutingTable output_routing_table_2;
	haldls::vx::v3::OutputRoutingTableEntry output_entry_2;
	output_entry_2.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(13));
	output_entry_2.set_enable(true);

	output_routing_table_2.entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(2)] =
	    output_entry_2;
	builders.at(2).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table_2);

	// Set input routing table on chip 3
	lola::vx::v3::InputRoutingTable input_routing_table_3;
	haldls::vx::v3::InputRoutingTableEntry input_entry_3;
	input_entry_3.set_label(haldls::vx::v3::InputRoutingTableEntry::Label(3));
	input_entry_3.set_enable(true);
	input_routing_table_3.entries[halco::hicann_dls::vx::InputRoutingTableEntryOnFPGA(13)] =
	    input_entry_3;
	builders.at(3).write(halco::hicann_dls::vx::InputRoutingTableOnFPGA(), input_routing_table_3);
	builders.at(3).write(
	    halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), lola::vx::v3::OutputRoutingTable());

	// Sync barrier
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
	}

	// Send spikes
	auto spike_label = halco::hicann_dls::vx::v3::SpikeLabel(0);
	haldls::vx::v3::SpikePack1ToChip::labels_type labels;
	labels.fill(spike_label);
	haldls::vx::v3::SpikePack1ToChip spike(labels);
	builders.at(0).write(halco::hicann_dls::vx::SpikePack1ToChipOnDLS(), spike);

	// Run finished programs
	std::vector<stadls::vx::v3::PlaybackProgram> programs;
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer());
		builders.at(i).block_until(
		    halco::hicann_dls::vx::v3::TimerOnDLS(),
		    haldls::vx::v3::Timer::Value(int(1 * fisch::vx::fpga_clock_cycles_per_s)));
		programs.push_back(builders.at(i).done());
	}
	std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;
	for (auto& playback_program : programs) {
		programs_wrapped.push_back(playback_program);
	}
	stadls::vx::v3::run(connection, programs_wrapped);

	auto spikes_0 = programs.at(0).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_0;
	for (auto const& spike : spikes_0) {
		spike_labels_0.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_0.contains(halco::hicann_dls::vx::v3::SpikeLabel(0)));
	EXPECT_EQ(spike_labels_0.size(), 1);

	auto spikes_1 = programs.at(1).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_1;
	for (auto const& spike : spikes_1) {
		spike_labels_1.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_1.contains(halco::hicann_dls::vx::v3::SpikeLabel(1)));
	EXPECT_EQ(spike_labels_1.size(), 1);

	auto spikes_2 = programs.at(2).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_2;
	for (auto const& spike : spikes_2) {
		spike_labels_2.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_2.contains(halco::hicann_dls::vx::v3::SpikeLabel(2)));
	EXPECT_EQ(spike_labels_2.size(), 1);

	auto spikes_3 = programs.at(3).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_3;
	for (auto const& spike : spikes_3) {
		spike_labels_3.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_3.contains(halco::hicann_dls::vx::v3::SpikeLabel(3)));
	EXPECT_EQ(spike_labels_3.size(), 1);
}

// Sending a single spike from the first three chips to the last one
TEST(Multichip, RoutingManyToOne)
{
	using namespace haldls::vx::v3;

	auto connection = hxcomm::vx::get_connection_from_env();
	if (!is_jboa_multichip_of_min_size(connection, 4)) {
		GTEST_SKIP();
	}

	size_t const connection_size = std::visit([](auto& conn) { return conn.size(); }, connection);
	auto const hwdb_entries =
	    std::visit([](auto const& connection) { return connection.get_hwdb_entry(); }, connection);

	// Increase instruction timeout so that sync barriers do not timeout
	auto timeout = InstructionTimeoutConfig();
	timeout.set_value(Timer::Value(int(15 * fisch::vx::fpga_clock_cycles_per_s)));

	std::vector<stadls::vx::v3::PlaybackProgramBuilder> builders;
	for (size_t i = 0; i < connection_size; i++) {
		auto [builder, _] =
		    stadls::vx::v3::generate(stadls::vx::v3::SystemInit(hwdb_entries.at(i)));
		builders.push_back(std::move(builder));
	}

	for (size_t i = 0; i < connection_size; i++) {
		// Larger instruction timout
		builders.at(i).write(halco::hicann_dls::vx::InstructionTimeoutConfigOnFPGA(), timeout);
	}

	// Set output routing table on chip 0
	lola::vx::v3::OutputRoutingTable output_routing_table_0;
	haldls::vx::v3::OutputRoutingTableEntry output_entry_0;
	output_entry_0.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(10));
	output_entry_0.set_enable(true);

	output_routing_table_0.entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(10)] =
	    output_entry_0;
	builders.at(0).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table_0);
	builders.at(0).write(
	    halco::hicann_dls::vx::InputRoutingTableOnFPGA(), lola::vx::v3::InputRoutingTable());

	// Set output routing table on chip 1
	lola::vx::v3::OutputRoutingTable output_routing_table_1;
	haldls::vx::v3::OutputRoutingTableEntry output_entry_1;
	output_entry_1.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(20));
	output_entry_1.set_enable(true);

	output_routing_table_1.entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(11)] =
	    output_entry_1;
	builders.at(1).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table_1);
	builders.at(1).write(
	    halco::hicann_dls::vx::InputRoutingTableOnFPGA(), lola::vx::v3::InputRoutingTable());

	// Set output routing table on chip 2
	lola::vx::v3::OutputRoutingTable output_routing_table_2;
	haldls::vx::v3::OutputRoutingTableEntry output_entry_2;
	output_entry_2.set_label(haldls::vx::v3::OutputRoutingTableEntry::Label(30));
	output_entry_2.set_enable(true);

	output_routing_table_2.entries[halco::hicann_dls::vx::OutputRoutingTableEntryOnFPGA(12)] =
	    output_entry_2;
	builders.at(2).write(halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), output_routing_table_2);
	builders.at(2).write(
	    halco::hicann_dls::vx::InputRoutingTableOnFPGA(), lola::vx::v3::InputRoutingTable());

	// Set input routing table on chip 3
	lola::vx::v3::InputRoutingTable input_routing_table_3;
	for (size_t i = 0; i <= 2; i++) {
		haldls::vx::v3::InputRoutingTableEntry input_entry_3;
		input_entry_3.set_label(haldls::vx::v3::InputRoutingTableEntry::Label(100 * (i + 1)));
		input_entry_3.set_enable(true);
		input_routing_table_3
		    .entries[halco::hicann_dls::vx::InputRoutingTableEntryOnFPGA((i + 1) * 10)] =
		    input_entry_3;
	}
	builders.at(3).write(halco::hicann_dls::vx::InputRoutingTableOnFPGA(), input_routing_table_3);
	builders.at(3).write(
	    halco::hicann_dls::vx::OutputRoutingTableOnFPGA(), lola::vx::v3::OutputRoutingTable());

	// Sync barrier
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).block_until(halco::hicann_dls::vx::BarrierOnFPGA(), Barrier::multi_fpga);
	}

	// Send spikes
	for (size_t i = 0; i <= 2; i++) {
		auto spike_label = halco::hicann_dls::vx::v3::SpikeLabel(i + 10);
		haldls::vx::v3::SpikePack1ToChip::labels_type labels;
		labels.fill(spike_label);
		haldls::vx::v3::SpikePack1ToChip spike(labels);
		builders.at(i).write(halco::hicann_dls::vx::SpikePack1ToChipOnDLS(), spike);
	}

	// Run finished programs
	std::vector<stadls::vx::v3::PlaybackProgram> programs;
	for (size_t i = 0; i < connection_size; i++) {
		builders.at(i).write(halco::hicann_dls::vx::v3::TimerOnDLS(), haldls::vx::v3::Timer());
		builders.at(i).block_until(
		    halco::hicann_dls::vx::v3::TimerOnDLS(),
		    haldls::vx::v3::Timer::Value(int(1 * fisch::vx::fpga_clock_cycles_per_s)));
		programs.push_back(builders.at(i).done());
	}
	std::vector<std::reference_wrapper<stadls::vx::v3::PlaybackProgram>> programs_wrapped;
	for (auto& playback_program : programs) {
		programs_wrapped.push_back(playback_program);
	}
	stadls::vx::v3::run(connection, programs_wrapped);

	auto spikes_0 = programs.at(0).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_0;
	for (auto const& spike : spikes_0) {
		spike_labels_0.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_0.contains(halco::hicann_dls::vx::v3::SpikeLabel(10)));
	EXPECT_EQ(spike_labels_0.size(), 1);

	auto spikes_1 = programs.at(1).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_1;
	for (auto const& spike : spikes_1) {
		spike_labels_1.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_1.contains(halco::hicann_dls::vx::v3::SpikeLabel(11)));
	EXPECT_EQ(spike_labels_1.size(), 1);

	auto spikes_2 = programs.at(2).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_2;
	for (auto const& spike : spikes_2) {
		spike_labels_2.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_2.contains(halco::hicann_dls::vx::v3::SpikeLabel(12)));
	EXPECT_EQ(spike_labels_2.size(), 1);

	auto spikes_3 = programs.at(3).get_spikes();
	std::set<halco::hicann_dls::vx::v3::SpikeLabel> spike_labels_3;
	for (auto const& spike : spikes_3) {
		spike_labels_3.insert(spike.label);
	}
	EXPECT_TRUE(spike_labels_3.contains(halco::hicann_dls::vx::v3::SpikeLabel(100)));
	EXPECT_TRUE(spike_labels_3.contains(halco::hicann_dls::vx::v3::SpikeLabel(200)));
	EXPECT_TRUE(spike_labels_3.contains(halco::hicann_dls::vx::v3::SpikeLabel(300)));
	EXPECT_EQ(spike_labels_3.size(), 3);
}