#ifdef __ppu__
#include "haldls/vx/systime.h"
#include "libnux/vx/testing.h"
#include "stadls/vx/v4/ppu/read.h"
#include "stadls/vx/v4/ppu/write.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace libnux::vx;

TEST(SystimeSyncBase, WR)
{
	SystimeSyncBase config;
	config.set_value(SystimeSyncBase::Value(0x12345));

	SystimeSyncBaseOnDLS coord;

	stadls::vx::v4::ppu::write(coord, config);

	SystimeSyncBase const read_config = stadls::vx::v4::ppu::read<SystimeSyncBase>(coord);

	EXPECT_EQ(read_config, config);
}
#endif
