#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/i2c.h"
#include "hxcomm/vx/connection_from_env.h"
#include "stadls/vx/v3/container_ticket.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"
#include <gtest/gtest.h>

using namespace halco::common;
using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

TEST(INA219, DACVoltages)
{
	auto sequence = DigitalInit();
	auto [builder, _] = generate(sequence);

	for (auto const ina : iter_all<INA219ConfigOnBoard>()) {
		builder.write(ina, INA219Config());
	}

	std::vector<ContainerTicket> tickets;
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

	float const expect12 = 1.2 /* V */;
	float const error12 = 0.05 * expect12 /* V */;
	float const expect12d = 1.25 /* V */;
	float const error12d = 0.05 * expect12d /* V */;
	float const expect25 = 2.5 /* V */;
	float const error25 = 0.05 * expect25 /* V */;
	EXPECT_NEAR(
	    dynamic_cast<INA219Status const&>(tickets.at(INA219StatusOnBoard::vdd12_digital).get())
	        .get_bus_voltage()
	        .toUncalibratedVoltage(),
	    expect12d, error12d);
	EXPECT_NEAR(
	    dynamic_cast<INA219Status const&>(tickets.at(INA219StatusOnBoard::vdd25_digital).get())
	        .get_bus_voltage()
	        .toUncalibratedVoltage(),
	    expect25, error25);
	EXPECT_NEAR(
	    dynamic_cast<INA219Status const&>(tickets.at(INA219StatusOnBoard::vdd12_analog).get())
	        .get_bus_voltage()
	        .toUncalibratedVoltage(),
	    expect12, error12);
	EXPECT_NEAR(
	    dynamic_cast<INA219Status const&>(tickets.at(INA219StatusOnBoard::vdd25_analog).get())
	        .get_bus_voltage()
	        .toUncalibratedVoltage(),
	    expect25, error25);
	EXPECT_NEAR(
	    dynamic_cast<INA219Status const&>(tickets.at(INA219StatusOnBoard::vdd12_madc).get())
	        .get_bus_voltage()
	        .toUncalibratedVoltage(),
	    expect12, error12);
	EXPECT_NEAR(
	    dynamic_cast<INA219Status const&>(tickets.at(INA219StatusOnBoard::vdd12_pll).get())
	        .get_bus_voltage()
	        .toUncalibratedVoltage(),
	    expect12, error12);
}
