#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "haldls/vx/arq.h"
#include "haldls/vx/systime.h"
#include "haldls/vx/timer.h"
#include "stadls/vx/init_generator.h"
#include "stadls/vx/playback_program.h"
#include "stadls/vx/playback_program_builder.h"
#include "stadls/vx/run.h"

#include "connection.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx;
using namespace haldls::vx;
using namespace stadls::vx;

/**
 * Enable Highspeed omnibus connection, read a container of known length and compare to ARQ status
 * counts.
 */
TEST(HicannARQStatus, OmnibusReadCount)
{
	auto sequence = DigitalInit();
	sequence.highspeed_link.enable_systime = false;
	auto [builder, _] = generate(sequence);

	// wait to make sure, whole Omnibus access over Highspeed completed
	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);

	auto ticket_ref = builder.read(HicannARQStatusOnFPGA());

	builder.read(SystimeSyncBaseOnDLS(), Backend::OmnibusChip);

	// wait to make sure, whole Omnibus access over Highspeed completed
	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);

	auto ticket = builder.read(HicannARQStatusOnFPGA());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

	EXPECT_TRUE(ticket_ref.valid());
	EXPECT_TRUE(ticket.valid());
	auto status_ref = ticket_ref.get();
	auto status = ticket.get();
	EXPECT_EQ(
	    status.get_read_count() - status_ref.get_read_count(),
	    SystimeSyncBase::config_size_in_words * fisch::vx::OmnibusChip::decode_ut_message_count);
	EXPECT_EQ(
	    status.get_write_count() - status_ref.get_write_count(),
	    SystimeSyncBase::config_size_in_words *
	        fisch::vx::OmnibusChip::encode_read_ut_message_count);

	EXPECT_GE(status.get_rx_count(), status.get_read_count());
	EXPECT_GE(status.get_tx_count(), status.get_write_count());
}

/**
 * Enable Highspeed omnibus connection, write a container of known length and compare to ARQ status
 * counts.
 *
 * Disabled due to issue #3474.
 */
TEST(HicannARQStatus, DISABLED_OmnibusWriteCount)
{
	auto sequence = DigitalInit();
	sequence.highspeed_link.enable_systime = false;
	auto [builder, _] = generate(sequence);

	builder.write(SystimeSyncBaseOnDLS(), SystimeSyncBase(), Backend::OmnibusChip);

	// wait to make sure, whole Omnibus access over Highspeed completed
	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);

	auto ticket = builder.read(HicannARQStatusOnFPGA());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = generate_test_connection();
	run(connection, program);

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
