#ifdef __ppu__
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/v2/neuron.h"
#include "haldls/vx/neuron.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/testing.h"
#include "libnux/vx/time.h"
#include "stadls/vx/v2/ppu/read.h"
#include "stadls/vx/v2/ppu/write.h"

using namespace haldls::vx::v2;
using namespace halco::hicann_dls::vx::v2;
using namespace halco::common;
using namespace libnux::vx;

TEST(SpikeCounterRead, ReadPerformance)
{
	auto const begin = now();
	for (auto const& column : iter_all<NeuronColumnOnDLS>()) {
		SpikeCounterReadOnDLS coord =
		    AtomicNeuronOnDLS(column, NeuronRowOnDLS()).toSpikeCounterReadOnDLS();
		[[maybe_unused]] SpikeCounterRead counter =
		    stadls::vx::v2::ppu::read<SpikeCounterRead>(coord);
	}
	auto const end = now();

	mailbox_write_int(end - begin);
	// value found by execution to ensure no undetected changes
	EXPECT_LE(end - begin, 320);
}
#endif
