#include <gtest/gtest.h>

#include "haldls/vx/i2c.h"

#include "fisch/vx/word_access/type/i2c.h"
#include "halco/hicann-dls/vx/i2c.h"
#include "haldls/vx/common.h"
#include "stadls/visitors.h"
#include "test-helper.h"


using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;


TEST(TMP112Status, General)
{
	TMP112Status status;

	// test getter/setter
	{
		auto value = draw_ranged_non_default_value<TMP112Status::Temperature>();
		status.set_temperature(value);
		EXPECT_EQ(status.get_temperature(), value);
	}

	TMP112Status status_eq = status;
	TMP112Status status_default;

	// test comparison
	ASSERT_EQ(status, status_eq);
	ASSERT_FALSE(status == status_default);

	ASSERT_NE(status, status_default);
	ASSERT_FALSE(status != status_eq);
}

TEST(TMP112Status, EncodeDecode)
{
	TMP112Status status;

	status.set_temperature(TMP112Status::Temperature(-768));

	TMP112StatusOnBoard coord;

	std::array<I2CTempRegisterOnBoard, TMP112Status::read_config_size_in_words> ref_addresses = {
	    I2CTempRegisterOnBoard()};


	std::array<
	    fisch::vx::word_access_type::I2CTempRegister, TMP112Status::read_config_size_in_words> const
	    ref_data = {fisch::vx::word_access_type::I2CTempRegister(64768)};

	HALDLS_TEST_DECODE(status, coord, ref_addresses, ref_data);
}

TEST(TMP112Status, CerealizeCoverage)
{
	TMP112Status status1, status2;

	// set test config values
	auto value = draw_ranged_non_default_value<TMP112Status::Temperature>();
	status1.set_temperature(value);

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(status1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(status2);
	}
	ASSERT_EQ(status1, status2);
}