#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/v3/coordinates.h"
#include "haldls/vx/v3/arq.h"
#include "haldls/vx/v3/barrier.h"
#include "haldls/vx/v3/systime.h"
#include "haldls/vx/v3/traits.h"
#include "stadls/vx/v3/init_generator.h"
#include "stadls/vx/v3/playback_program.h"
#include "stadls/vx/v3/playback_program_builder.h"
#include "stadls/vx/v3/run.h"

#include "hxcomm/vx/connection_from_env.h"
#include "test-helper.h"

using namespace halco::common;
using namespace halco::hicann_dls::vx::v3;
using namespace haldls::vx::v3;
using namespace stadls::vx::v3;

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

	builder.read(SystimeSyncBaseOnDLS(), Backend::Omnibus);

	// wait to make sure, whole Omnibus access over Highspeed completed
	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);

	auto ticket = builder.read(HicannARQStatusOnFPGA());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	EXPECT_TRUE(ticket_ref.valid());
	EXPECT_TRUE(ticket.valid());
	auto status_ref = ticket_ref.get();
	auto status = ticket.get();
	EXPECT_EQ(
	    status.get_read_count() - status_ref.get_read_count(),
	    SystimeSyncBase::config_size_in_words * fisch::vx::Omnibus::decode_ut_message_count);
	EXPECT_EQ(
	    status.get_write_count() - status_ref.get_write_count(),
	    SystimeSyncBase::config_size_in_words * fisch::vx::Omnibus::encode_read_ut_message_count);

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

	builder.write(SystimeSyncBaseOnDLS(), SystimeSyncBase(), Backend::Omnibus);

	// wait to make sure, whole Omnibus access over Highspeed completed
	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);

	auto ticket = builder.read(HicannARQStatusOnFPGA());

	builder.block_until(BarrierOnFPGA(), Barrier::omnibus);
	auto program = builder.done();

	auto connection = hxcomm::vx::get_connection_from_env();
	run(connection, program);

	EXPECT_TRUE(ticket.valid());
	auto status = ticket.get();
	// write response filtering above measuring point
	EXPECT_EQ(
	    status.get_read_count(),
	    SystimeSyncBase::config_size_in_words * fisch::vx::Omnibus::decode_ut_message_count);
	EXPECT_EQ(
	    status.get_write_count(),
	    SystimeSyncBase::config_size_in_words * fisch::vx::Omnibus::encode_write_ut_message_count);

	EXPECT_GE(status.get_rx_count(), status.get_read_count());
	EXPECT_GE(status.get_tx_count(), status.get_write_count());
}
