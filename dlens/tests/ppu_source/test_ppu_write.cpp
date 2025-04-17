#ifdef __ppu__
#include "halco/hicann-dls/vx/v3/neuron.h"
#include "haldls/vx/neuron.h"
#include "stadls/vx/v3/ppu/read.h"
#include "stadls/vx/v3/ppu/write.h"

using namespace haldls::vx::v3;
using namespace halco::hicann_dls::vx::v3;

int start(void)
{
	CommonNeuronBackendConfigOnDLS coord = CommonNeuronBackendConfigOnDLS();
	CommonNeuronBackendConfig config = stadls::vx::v3::ppu::read<CommonNeuronBackendConfig>(coord);

	// Just set a single value and check with host code that it was changed.
	config.set_enable_clocks(!config.get_enable_clocks());
	stadls::vx::v3::ppu::write(coord, config);
	return 0;
}
#endif
