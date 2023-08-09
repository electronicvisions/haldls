#ifdef __ppu__
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/v4/neuron.h"
#include "haldls/vx/neuron.h"
#include "libnux/vx/mailbox.h"
#include "libnux/vx/testing.h"
#include "libnux/vx/time.h"
#include "stadls/vx/v4/ppu/read.h"
#include "stadls/vx/v4/ppu/write.h"

using namespace haldls::vx::v4;
using namespace halco::hicann_dls::vx::v4;
using namespace halco::common;
using namespace libnux::vx;

TEST(SpikeCounter, ReadPerformance)
{
	auto const begin = now();
	for (auto const& column : iter_all<NeuronColumnOnDLS>()) {
		SpikeCounterReadOnDLS coord =
		    AtomicNeuronOnDLS(column, NeuronRowOnDLS()).toSpikeCounterReadOnDLS();
		[[maybe_unused]] SpikeCounterRead counter =
		    stadls::vx::v4::ppu::read<SpikeCounterRead>(coord);
	}
	auto const end = now();

	mailbox_write_int(end - begin);
	EXPECT_LE(end - begin, 320);
}

TEST(SpikeCounter, Reset)
{
	// Tests that `at()` array access is possible
	for (auto const& column : iter_all<NeuronColumnOnDLS>()) {
		SpikeCounterResetOnDLS coord =
		    AtomicNeuronOnDLS(column, NeuronRowOnDLS()).toSpikeCounterResetOnDLS();
		stadls::vx::v4::ppu::write(coord, haldls::vx::v4::SpikeCounterReset());
	}
}
#endif
