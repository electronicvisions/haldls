#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "haldls/vx/pll.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

TEST(ADPLL, General)
{
	ADPLL config;

	// test getter/setter
	{
		auto value =
		    draw_ranged_non_default_value<ADPLL::LoopFilterInt>(config.get_loop_filter_int());
		config.set_loop_filter_int(value);
		EXPECT_EQ(config.get_loop_filter_int(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<ADPLL::LoopFilterProp>(config.get_loop_filter_prop());
		config.set_loop_filter_prop(value);
		EXPECT_EQ(config.get_loop_filter_prop(), value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::LoopDivN>(config.get_loop_div_n());
		config.set_loop_div_n(value);
		EXPECT_EQ(config.get_loop_div_n(), value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::CoreDivM0>(config.get_core_div_m0());
		config.set_core_div_m0(value);
		EXPECT_EQ(config.get_core_div_m0(), value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::CoreDivM1>(config.get_core_div_m1());
		config.set_core_div_m1(value);
		EXPECT_EQ(config.get_core_div_m1(), value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::PreDivP0>(config.get_pre_div_p0());
		config.set_pre_div_p0(value);
		EXPECT_EQ(config.get_pre_div_p0(), value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::PreDivP1>(config.get_pre_div_p1());
		config.set_pre_div_p1(value);
		EXPECT_EQ(config.get_pre_div_p1(), value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::PreDivP2>(config.get_pre_div_p2());
		config.set_pre_div_p2(value);
		EXPECT_EQ(config.get_pre_div_p2(), value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::Tune>(config.get_tune());
		config.set_tune(value);
		EXPECT_EQ(config.get_tune(), value);
	}

	{
		bool value = config.get_open_lock();
		config.set_open_lock(!value);
		EXPECT_EQ(config.get_open_lock(), !value);
	}

	{
		bool value = config.get_enforce_lock();
		config.set_enforce_lock(!value);
		EXPECT_EQ(config.get_enforce_lock(), !value);
	}

	{
		bool value = config.get_pfd_select();
		config.set_pfd_select(!value);
		EXPECT_EQ(config.get_pfd_select(), !value);
	}

	{
		bool value = config.get_lock_window();
		config.set_lock_window(!value);
		EXPECT_EQ(config.get_lock_window(), !value);
	}

	{
		auto value = draw_ranged_non_default_value<ADPLL::FilterShift>(config.get_filter_shift());
		config.set_filter_shift(value);
		EXPECT_EQ(config.get_filter_shift(), value);
	}

	{
		bool value = config.get_enable_output_clock();
		config.set_enable_output_clock(!value);
		EXPECT_EQ(config.get_enable_output_clock(), !value);
	}

	{
		bool value = config.get_enable();
		config.set_enable(!value);
		EXPECT_EQ(config.get_enable(), !value);
	}

	{
		bool value = config.get_use_external_config();
		config.set_use_external_config(!value);
		EXPECT_EQ(config.get_use_external_config(), !value);
	}

	ADPLL config_eq = config;
	ADPLL config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(ADPLL, EncodeDecode)
{
	ADPLL config;

	ADPLLOnDLS coord(0);

	// JTAG write access
	{
		std::array<JTAGPLLRegisterOnDLS, ADPLL::config_size_in_words> ref_addresses = {
		    JTAGPLLRegisterOnDLS(0), JTAGPLLRegisterOnDLS(1)};
		std::array<fisch::vx::JTAGPLLRegister, ADPLL::config_size_in_words> ref_data = {
		    fisch::vx::JTAGPLLRegister::Value(0x2a20a902),
		    fisch::vx::JTAGPLLRegister::Value(0xc0cbf200)};

		{ // write addresses
			std::vector<JTAGPLLRegisterOnDLS> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<std::vector<JTAGPLLRegisterOnDLS>>{write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::JTAGPLLRegister> data;
		visit_preorder(
		    config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::JTAGPLLRegister>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	// Omnibus over JTAG access
	{
		config.set_enable(!config.get_enable());

		std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, ADPLL::config_size_in_words> ref_addresses = {
		    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x80000), halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x80001)};
		std::array<fisch::vx::OmnibusChipOverJTAG, ADPLL::config_size_in_words> ref_data = {
		    fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x2a20a902)),
		    fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x80cbf200))};

		{ // write addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		{ // read addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> read_addresses;
			visit_preorder(
			    config, coord,
			    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{read_addresses});
			EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::OmnibusChipOverJTAG> data;
		visit_preorder(
		    config, coord,
		    stadls::EncodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

		ADPLL config_2;
		visit_preorder(
		    config_2, coord,
		    stadls::DecodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>(data));
		EXPECT_EQ(config, config_2);
	}
}

TEST(ADPLL, CerealizeCoverage)
{
	ADPLL obj1, obj2;
	obj1.set_loop_filter_int(
	    draw_ranged_non_default_value<ADPLL::LoopFilterInt>(obj1.get_loop_filter_int()));
	obj1.set_loop_filter_prop(
	    draw_ranged_non_default_value<ADPLL::LoopFilterProp>(obj1.get_loop_filter_prop()));
	obj1.set_loop_div_n(draw_ranged_non_default_value<ADPLL::LoopDivN>(obj1.get_loop_div_n()));
	obj1.set_core_div_m0(draw_ranged_non_default_value<ADPLL::CoreDivM0>(obj1.get_core_div_m0()));
	obj1.set_core_div_m1(draw_ranged_non_default_value<ADPLL::CoreDivM1>(obj1.get_core_div_m1()));
	obj1.set_pre_div_p0(draw_ranged_non_default_value<ADPLL::PreDivP0>(obj1.get_pre_div_p0()));
	obj1.set_pre_div_p1(draw_ranged_non_default_value<ADPLL::PreDivP1>(obj1.get_pre_div_p1()));
	obj1.set_pre_div_p2(draw_ranged_non_default_value<ADPLL::PreDivP2>(obj1.get_pre_div_p2()));
	obj1.set_tune(draw_ranged_non_default_value<ADPLL::Tune>(obj1.get_tune()));
	obj1.set_open_lock(!obj1.get_open_lock());
	obj1.set_enforce_lock(!obj1.get_enforce_lock());
	obj1.set_pfd_select(!obj1.get_pfd_select());
	obj1.set_lock_window(!obj1.get_lock_window());
	obj1.set_filter_shift(
	    draw_ranged_non_default_value<ADPLL::FilterShift>(obj1.get_filter_shift()));
	obj1.set_enable_output_clock(!obj1.get_enable_output_clock());
	obj1.set_enable(!obj1.get_enable());
	obj1.set_use_external_config(!obj1.get_use_external_config());

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}


TEST(PLLClockOutputBlock, General)
{
	PLLClockOutputBlock config;

	// test getter/setter
	for (auto output : iter_all<PLLClockOutputOnDLS>()) {
		auto value = config.get_clock_output(output);

		value.set_enable_output(!value.get_enable_output());
		value.set_enable_bypass(!value.get_enable_bypass());
		value.set_select_adpll(draw_ranged_non_default_value<ADPLLOnDLS>(value.get_select_adpll()));
		value.set_select_adpll_output(
		    PLLClockOutputBlock::ClockOutput::ADPLLOutput::dco); // != default

		config.set_clock_output(output, value);
		EXPECT_EQ(config.get_clock_output(output), value);
	}

	PLLClockOutputBlock config_eq = config;
	PLLClockOutputBlock config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(PLLClockOutputBlock, EncodeDecode)
{
	PLLClockOutputBlock config;

	PLLOnDLS coord;

	// JTAG write access
	{
		std::array<JTAGPLLRegisterOnDLS, PLLClockOutputBlock::config_size_in_words> ref_addresses = {
		    JTAGPLLRegisterOnDLS(4)};
		std::array<fisch::vx::JTAGPLLRegister, PLLClockOutputBlock::config_size_in_words> ref_data =
		    {fisch::vx::JTAGPLLRegister::Value(0x918d8181)};

		{ // write addresses
			std::vector<JTAGPLLRegisterOnDLS> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<std::vector<JTAGPLLRegisterOnDLS>>{write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::JTAGPLLRegister> data;
		visit_preorder(
		    config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::JTAGPLLRegister>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	// Omnibus over JTAG access
	{
		auto value = config.get_clock_output(PLLClockOutputOnDLS(0));
		value.set_enable_bypass(true);
		config.set_clock_output(PLLClockOutputOnDLS(0), value);

		std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PLLClockOutputBlock::config_size_in_words> ref_addresses =
		    {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x80004)};
		std::array<fisch::vx::OmnibusChipOverJTAG, PLLClockOutputBlock::config_size_in_words>
		    ref_data = {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x918d8183))};

		{ // write addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		{ // read addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> read_addresses;
			visit_preorder(
			    config, coord,
			    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{read_addresses});
			EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::OmnibusChipOverJTAG> data;
		visit_preorder(
		    config, coord,
		    stadls::EncodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

		PLLClockOutputBlock config_2;
		visit_preorder(
		    config_2, coord,
		    stadls::DecodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>(data));
		EXPECT_EQ(config, config_2);
	}
}

TEST(PLLClockOutputBlock, CerealizeCoverage)
{
	PLLClockOutputBlock obj1, obj2;
	for (auto output : iter_all<PLLClockOutputOnDLS>()) {
		auto value = obj1.get_clock_output(output);

		value.set_enable_output(!value.get_enable_output());
		value.set_enable_bypass(!value.get_enable_bypass());
		value.set_select_adpll(draw_ranged_non_default_value<ADPLLOnDLS>(value.get_select_adpll()));
		value.set_select_adpll_output(
		    PLLClockOutputBlock::ClockOutput::ADPLLOutput::dco); // != default

		obj1.set_clock_output(output, value);
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}


TEST(PLLSelfTest, General)
{
	PLLSelfTest config;

	// test getter/setter
	{
		bool value = config.get_clock_enable();
		config.set_clock_enable(!value);
		EXPECT_EQ(config.get_clock_enable(), !value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<PLLSelfTest::PreScalerP>(config.get_pre_scaler_p());
		config.set_pre_scaler_p(value);
		EXPECT_EQ(config.get_pre_scaler_p(), value);
	}

	{
		auto value = draw_ranged_non_default_value<PLLClockOutputOnDLS>(config.get_select_source());
		config.set_select_source(value);
		EXPECT_EQ(config.get_select_source(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<PLLSelfTest::CheckRange>(config.get_check_range());
		config.set_check_range(value);
		EXPECT_EQ(config.get_check_range(), value);
	}

	{
		auto value =
		    draw_ranged_non_default_value<PLLSelfTest::CheckValue>(config.get_check_value());
		config.set_check_value(value);
		EXPECT_EQ(config.get_check_value(), value);
	}

	PLLSelfTest config_eq = config;
	PLLSelfTest config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config == config_default);

	ASSERT_NE(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(PLLSelfTest, EncodeDecode)
{
	PLLSelfTest config;

	PLLOnDLS coord;

	// JTAG write access
	{
		std::array<JTAGPLLRegisterOnDLS, PLLSelfTest::config_size_in_words> ref_addresses = {
		    JTAGPLLRegisterOnDLS(5)};
		std::array<fisch::vx::JTAGPLLRegister, PLLSelfTest::config_size_in_words> ref_data = {
		    fisch::vx::JTAGPLLRegister::Value(0x0)};

		{ // write addresses
			std::vector<JTAGPLLRegisterOnDLS> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<std::vector<JTAGPLLRegisterOnDLS>>{write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::JTAGPLLRegister> data;
		visit_preorder(
		    config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::JTAGPLLRegister>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
	}

	// Omnibus over JTAG access
	{
		bool const value = config.get_clock_enable();
		config.set_clock_enable(!value);

		std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PLLSelfTest::config_size_in_words> ref_addresses = {
		    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x80005)};
		std::array<fisch::vx::OmnibusChipOverJTAG, PLLSelfTest::config_size_in_words> ref_data = {
		    fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x1))};

		{ // write addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> write_addresses;
			visit_preorder(
			    config, coord,
			    stadls::WriteAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{write_addresses});
			EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		{ // read addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> read_addresses;
			visit_preorder(
			    config, coord,
			    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{read_addresses});
			EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		std::vector<fisch::vx::OmnibusChipOverJTAG> data;
		visit_preorder(
		    config, coord,
		    stadls::EncodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>{data});
		EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));

		PLLSelfTest config_2;
		visit_preorder(
		    config_2, coord,
		    stadls::DecodeVisitor<std::vector<fisch::vx::OmnibusChipOverJTAG>>(data));
		EXPECT_EQ(config, config_2);
	}
}

TEST(PLLSelfTest, CerealizeCoverage)
{
	PLLSelfTest obj1, obj2;
	obj1.set_clock_enable(!obj1.get_clock_enable());
	obj1.set_pre_scaler_p(
	    draw_ranged_non_default_value<PLLSelfTest::PreScalerP>(obj1.get_pre_scaler_p()));
	obj1.set_select_source(
	    draw_ranged_non_default_value<PLLClockOutputOnDLS>(obj1.get_select_source()));
	obj1.set_check_range(
	    draw_ranged_non_default_value<PLLSelfTest::CheckRange>(obj1.get_check_range()));
	obj1.set_check_value(
	    draw_ranged_non_default_value<PLLSelfTest::CheckValue>(obj1.get_check_value()));

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}

TEST(PLLSelfTestStatus, General)
{
	PLLSelfTestStatus config;

	// test getter
	EXPECT_EQ(config.get_success(), false);
	EXPECT_EQ(config.get_finished(), false);
	EXPECT_EQ(config.get_counter_value(), PLLSelfTestStatus::CounterValue(0));

	PLLSelfTestStatus config_eq = config;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_FALSE(config != config_eq);
}

TEST(PLLSelfTestStatus, EncodeDecode)
{
	PLLOnDLS coord;

	// Omnibus over JTAG access
	{
		std::array<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress, PLLSelfTestStatus::config_size_in_words> ref_addresses = {
		    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(0x80007)};
		std::array<fisch::vx::OmnibusChipOverJTAG, PLLSelfTestStatus::config_size_in_words>
		    ref_data = {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x00312345))};

		{ // read addresses
			std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress> read_addresses;
			PLLSelfTestStatus config;
			visit_preorder(
			    config, coord,
			    stadls::ReadAddressVisitor<std::vector<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>>{read_addresses});
			EXPECT_THAT(read_addresses, ::testing::ElementsAreArray(ref_addresses));
		}

		PLLSelfTestStatus config;
		visit_preorder(
		    config, coord,
		    stadls::DecodeVisitor<std::array<fisch::vx::OmnibusChipOverJTAG, ref_data.size()>>(
		        ref_data));
		EXPECT_EQ(config.get_success(), true);
		EXPECT_EQ(config.get_finished(), true);
		EXPECT_EQ(config.get_counter_value(), PLLSelfTestStatus::CounterValue(0x12345));
	}
}

TEST(PLLSelfTestStatus, CerealizeCoverage)
{
	PLLSelfTestStatus obj1, obj2;

	{
		std::array<fisch::vx::OmnibusChipOverJTAG, PLLSelfTestStatus::config_size_in_words> data =
		    {fisch::vx::OmnibusChipOverJTAG(fisch::vx::OmnibusData(0x00312345))};
		visit_preorder(
		    obj1, PLLOnDLS(),
		    stadls::DecodeVisitor<std::array<fisch::vx::OmnibusChipOverJTAG, data.size()>>(data));
	}

	std::ostringstream ostream;
	{
		cereal::JSONOutputArchive oa(ostream);
		oa(obj1);
	}

	std::istringstream istream(ostream.str());
	{
		cereal::JSONInputArchive ia(istream);
		ia(obj2);
	}
	ASSERT_EQ(obj1, obj2);
}
