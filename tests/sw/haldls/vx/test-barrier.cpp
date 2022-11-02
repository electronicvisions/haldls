#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/word_access/type/barrier.h"
#include "halco/hicann-dls/vx/barrier.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/barrier.h"
#include "test-helper.h"

using namespace haldls::vx;

HALDLS_TEST(
    Barrier,
    (enable_omnibus)(enable_jtag)(enable_systime)(enable_multi_fpga)(enable_systime_correction))

TEST(Barrier, Encode)
{
	Barrier config;
	config.set_enable_omnibus(false);
	config.set_enable_jtag(true);
	config.set_enable_systime(true);
	config.set_enable_multi_fpga(true);
	config.set_enable_systime_correction(false);

	EXPECT_EQ(config.encode(), 0xe);
}
