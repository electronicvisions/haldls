#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "haldls/vx/arq.h"
#include "haldls/vx/systime.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"

#include "executor.h"
#include "test-helper.h"
#include "test-init_helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable Highspeed omnibus connection, read ARQ status, expect counts == 0.
 */
TEST(HicannARQStatus, ReadCount0)
{
	PlaybackProgramBuilder builder;

	insert_highspeed_init(builder);

	auto ticket = builder.read(HicannARQStatusOnFPGA());

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(1000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	EXPECT_TRUE(ticket.valid());
	auto status = ticket.get();
	EXPECT_EQ(status.get_read_count(), 0);
	EXPECT_EQ(status.get_write_count(), 0);
	EXPECT_EQ(status.get_rx_count(), 0);
	EXPECT_EQ(status.get_tx_count(), 0);
}

/**
 * Enable Highspeed omnibus connection, read a container of known length and compare to ARQ status
 * counts.
 */
TEST(HicannARQStatus, OmnibusReadCount)
{
	PlaybackProgramBuilder builder;

	insert_highspeed_init(builder);

	builder.read(SystimeSyncBaseOnDLS(), Backend::OmnibusChip);

	// wait to make sure, whole Omnibus access over Highspeed completed
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(1000));

	auto ticket = builder.read(HicannARQStatusOnFPGA());

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(1000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	EXPECT_TRUE(ticket.valid());
	auto status = ticket.get();
	EXPECT_EQ(
	    status.get_read_count(),
	    SystimeSyncBase::config_size_in_words * fisch::vx::OmnibusChip::decode_ut_message_count);
	EXPECT_EQ(
	    status.get_write_count(), SystimeSyncBase::config_size_in_words *
	                                  fisch::vx::OmnibusChip::encode_read_ut_message_count);

	EXPECT_GE(status.get_rx_count(), status.get_read_count());
	EXPECT_GE(status.get_tx_count(), status.get_write_count());
}

/**
 * Enable Highspeed omnibus connection, write a container of known length and compare to ARQ status
 * counts.
 */
TEST(HicannARQStatus, OmnibusWriteCount)
{
	PlaybackProgramBuilder builder;

	insert_highspeed_init(builder);

	builder.write(SystimeSyncBaseOnDLS(), SystimeSyncBase(), Backend::OmnibusChip);

	// wait to make sure, whole Omnibus access over Highspeed completed
	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(1000));

	auto ticket = builder.read(HicannARQStatusOnFPGA());

	builder.write(TimerOnDLS(), Timer());
	builder.wait_until(TimerOnDLS(), Timer::Value(1000));
	auto program = builder.done();

	auto executor = generate_playback_program_test_executor();
	executor.run(program);

	EXPECT_TRUE(ticket.valid());
	auto status = ticket.get();
	// write response filtering above measuring point
	EXPECT_EQ(
	    status.get_read_count(),
	    SystimeSyncBase::config_size_in_words * fisch::vx::OmnibusChip::decode_ut_message_count);
	EXPECT_EQ(
	    status.get_write_count(), SystimeSyncBase::config_size_in_words *
	                                  fisch::vx::OmnibusChip::encode_write_ut_message_count);

	EXPECT_GE(status.get_rx_count(), status.get_read_count());
	EXPECT_GE(status.get_tx_count(), status.get_write_count());
}
