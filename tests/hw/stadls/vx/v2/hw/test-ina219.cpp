#include "halco/hicann-dls/vx/v2/coordinates.h"
#include "haldls/vx/v2/barrier.h"
#include "haldls/vx/v2/i2c.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v2/init_generator.h"
#include "stadls/vx/v2/playback_program.h"
#include "stadls/vx/v2/playback_program_builder.h"
#include "stadls/vx/v2/run.h"
#include <gtest/gtest.h>

using namespace halco::common;
using namespace halco::hicann_dls::vx::v2;
using namespace haldls::vx::v2;
using namespace stadls::vx::v2;

TEST(INA219, DACVoltages)
{
	auto sequence = DigitalInit();
	auto [builder, _] = generate(sequence);

	for (auto const ina : iter_all<INA219ConfigOnBoard>()) {
		builder.write(ina, INA219Config());
	}

	std::vector<PlaybackProgram::ContainerTicket<INA219Status>> tickets;
	for (auto const ina : iter_all<INA219StatusOnBoard>()) {
		tickets.push_back(builder.read(ina));
	}

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	for (auto const& ticket : tickets) {
		EXPECT_TRUE(ticket.valid());
	}

	auto const range12_min = 1100 / 4 /* linear with 4mV/LSB*/;
	auto const range12_max = 1300 / 4 /* linear with 4mV/LSB*/;
	auto const range25_min = 2400 / 4 /* linear with 4mV/LSB*/;
	auto const range25_max = 2600 / 4 /* linear with 4mV/LSB*/;
	EXPECT_LE(tickets.at(INA219StatusOnBoard::vdd12_digital).get().get_bus_voltage(), range12_max);
	EXPECT_LE(tickets.at(INA219StatusOnBoard::vdd25_digital).get().get_bus_voltage(), range25_max);
	EXPECT_LE(tickets.at(INA219StatusOnBoard::vdd12_analog).get().get_bus_voltage(), range12_max);
	EXPECT_LE(tickets.at(INA219StatusOnBoard::vdd25_analog).get().get_bus_voltage(), range25_max);
	EXPECT_LE(tickets.at(INA219StatusOnBoard::vdd12_madc).get().get_bus_voltage(), range12_max);
	EXPECT_LE(tickets.at(INA219StatusOnBoard::vdd12_pll).get().get_bus_voltage(), range12_max);

	EXPECT_GE(tickets.at(INA219StatusOnBoard::vdd12_digital).get().get_bus_voltage(), range12_min);
	EXPECT_GE(tickets.at(INA219StatusOnBoard::vdd25_digital).get().get_bus_voltage(), range25_min);
	EXPECT_GE(tickets.at(INA219StatusOnBoard::vdd12_analog).get().get_bus_voltage(), range12_min);
	EXPECT_GE(tickets.at(INA219StatusOnBoard::vdd25_analog).get().get_bus_voltage(), range25_min);
	EXPECT_GE(tickets.at(INA219StatusOnBoard::vdd12_madc).get().get_bus_voltage(), range12_min);
	EXPECT_GE(tickets.at(INA219StatusOnBoard::vdd12_pll).get().get_bus_voltage(), range12_min);
}
