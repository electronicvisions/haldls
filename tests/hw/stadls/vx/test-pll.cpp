#include <gtest/gtest.h>

#include "halco/hicann-dls/vx/coordinates.h"
#include "haldls/vx/jtag.h"
#include "haldls/vx/pll.h"
#include "haldls/vx/reset.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"

class PLLTest : public ::testing::Test
{
protected:
	PLLTest() {}

	void configure()
	{
		builder.write(halco::hicann_dls::vx::ResetChipOnDLS(), haldls::vx::ResetChip(true));
		builder.write(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
		builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10));
		builder.write(halco::hicann_dls::vx::ResetChipOnDLS(), haldls::vx::ResetChip(false));
		builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(100));

		builder.write(
		    halco::hicann_dls::vx::JTAGClockScalerOnDLS(),
		    haldls::vx::JTAGClockScaler(haldls::vx::JTAGClockScaler::Value(3)));
		builder.write(halco::hicann_dls::vx::ResetJTAGTapOnDLS(), haldls::vx::ResetJTAGTap());

		// wait until ASIC omnibus is up (22 us)
		builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(22 * 125));
		builder.write(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
	}

	void test_run_program(stadls::vx::PlaybackProgram& program)
	{
		auto executor = generate_playback_program_test_executor();
		executor.run(program);
	}

	stadls::vx::PlaybackProgramBuilder builder;
}; // PLLTest

class PLLTestWriteReadADPLL
    : public PLLTest
    , public ::testing::WithParamInterface<size_t>
{
protected:
	PLLTestWriteReadADPLL() : PLLTest() {}

	void configure()
	{
		PLLTest::configure();

		auto coord = halco::hicann_dls::vx::ADPLLOnDLS(GetParam());

		haldls::vx::ADPLL adpll_config;

		builder.write(coord, adpll_config, haldls::vx::Backend::OmnibusChipOverJTAG);
		builder.write(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
		// wait for omnibus clock to come up again
		builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(30 * 125));
		ticket = builder.read(coord, haldls::vx::Backend::OmnibusChipOverJTAG);

		builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10000));

		program = builder.done();
	}

	void test_run_program() { PLLTest::test_run_program(program); }

	void check_equality()
	{
		auto read_config = ticket->get();
		EXPECT_EQ(read_config, haldls::vx::ADPLL());
	}

	stadls::vx::PlaybackProgram program;
	std::optional<stadls::vx::PlaybackProgram::ContainerTicket<haldls::vx::ADPLL>> ticket;
}; // PLLTestWriteReadADPLL

INSTANTIATE_TEST_CASE_P(
    PLLTestWriteReadADPLLIteration,
    PLLTestWriteReadADPLL,
    ::testing::Range(
        halco::hicann_dls::vx::ADPLLOnDLS::begin, halco::hicann_dls::vx::ADPLLOnDLS::end));

TEST_P(PLLTestWriteReadADPLL, OmnibusEquality)
{
	configure();
	test_run_program();
	check_equality();
}

TEST_F(PLLTest, WriteReadPLLClockOutputBlockOmnibusEquality)
{
	configure();

	haldls::vx::PLLClockOutputBlock config;
	halco::hicann_dls::vx::PLLClockOutputBlockOnDLS coord;

	builder.write(coord, config, haldls::vx::Backend::OmnibusChipOverJTAG);
	auto ticket = builder.read(coord, haldls::vx::Backend::OmnibusChipOverJTAG);

	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10000));

	auto program = builder.done();
	test_run_program(program);

	EXPECT_EQ(ticket.get(), config);
}

TEST_F(PLLTest, WriteReadPLLSelfTestOmnibusEquality)
{
	configure();

	haldls::vx::PLLSelfTest config;
	config.set_check_value(haldls::vx::PLLSelfTest::CheckValue(0x1234));
	config.set_clock_enable(true);

	halco::hicann_dls::vx::PLLSelfTestOnDLS coord;

	builder.write(coord, config, haldls::vx::Backend::OmnibusChipOverJTAG);
	auto ticket = builder.read(coord, haldls::vx::Backend::OmnibusChipOverJTAG);

	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10000));

	auto program = builder.done();
	test_run_program(program);

	EXPECT_EQ(ticket.get(), config);
}

TEST_F(PLLTest, PLLSelfTest)
{
	configure();

	{
		haldls::vx::ADPLL adpll_config;
		for (auto adpll : halco::common::iter_all<halco::hicann_dls::vx::ADPLLOnDLS>()) {
			builder.write(adpll, adpll_config, haldls::vx::Backend::OmnibusChipOverJTAG);
		}
	}

	{
		haldls::vx::PLLClockOutputBlock config;
		halco::hicann_dls::vx::PLLClockOutputBlockOnDLS coord;
		builder.write(coord, config, haldls::vx::Backend::OmnibusChipOverJTAG);
	}

	haldls::vx::PLLSelfTest config;
	config.set_pre_scaler_p(haldls::vx::PLLSelfTest::PreScalerP(8));
	config.set_check_value(haldls::vx::PLLSelfTest::CheckValue(10240));
	config.set_check_range(haldls::vx::PLLSelfTest::CheckRange(2));
	config.set_clock_enable(true);

	halco::hicann_dls::vx::PLLSelfTestOnDLS coord;

	builder.write(coord, config, haldls::vx::Backend::OmnibusChipOverJTAG);

	builder.write(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10000));

	auto ticket = builder.read(
	    halco::hicann_dls::vx::PLLSelfTestStatusOnDLS(), haldls::vx::Backend::OmnibusChipOverJTAG);

	builder.write(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer());
	builder.wait_until(halco::hicann_dls::vx::TimerOnDLS(), haldls::vx::Timer::Value(10000));

	auto program = builder.done();
	test_run_program(program);

	EXPECT_TRUE(ticket.valid());

	auto status = ticket.get();

	EXPECT_TRUE(status.get_finished());
	EXPECT_TRUE(status.get_success());
	EXPECT_TRUE(
	    std::abs(
	        static_cast<long>(status.get_counter_value()) -
	        static_cast<long>(config.get_check_value())) <=
	    static_cast<long>(config.get_check_range()));
}
