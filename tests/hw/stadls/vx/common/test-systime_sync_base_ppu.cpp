#ifdef __ppu__
#include "libnux/vx/testing.h"

#include "haldls/vx/systime.h"
#include "libnux/vx/omnibus.h"

#include "fisch/vx/word_access/type/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;
using namespace libnux::vx;

TEST(SystimeSyncBase, WR)
{
	SystimeSyncBase config;
	config.set_value(SystimeSyncBase::Value(0x12345));

	SystimeSyncBaseOnDLS coord;

	auto const addresses = config.addresses<halco::hicann_dls::vx::OmnibusAddress>(coord);
	auto const words = config.encode<fisch::vx::word_access_type::Omnibus>();

	for (size_t i = 0; i < SystimeSyncBase::config_size_in_words; ++i) {
		omnibus_write(addresses[i].value(), words[i]);
	}

	std::array<fisch::vx::word_access_type::Omnibus, SystimeSyncBase::config_size_in_words>
	    read_words;
	for (size_t i = 0; i < read_words.size(); ++i) {
		read_words[i] = fisch::vx::word_access_type::Omnibus(omnibus_read(addresses[i].value()));
	}

	SystimeSyncBase read_config;
	read_config.decode(read_words);

	EXPECT_EQ(read_config, config);
	for (size_t i = 0; i < read_words.size(); i++) {
		EXPECT_EQ(read_words[i], words[i]);
	}
}

int start()
{
	return RUN_ALL_TESTS();
}
#endif
