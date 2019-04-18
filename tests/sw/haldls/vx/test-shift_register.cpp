#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/jtag.h"
#include "haldls/vx/spi.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<omnibus_address_type> addresses_type;
typedef std::vector<fisch::vx::Omnibus> words_type;

TEST(ShiftRegister, General)
{
	// test getter/setter
	ShiftRegister reg;

	// test getter/setter
	{
		bool value = reg.get_enable_capmem_i_ref();
		reg.set_enable_capmem_i_ref(!value);
		EXPECT_EQ(reg.get_enable_capmem_i_ref(), !value);
	}

	{
		bool value = reg.get_enable_measure_capmem_i_ref();
		reg.set_enable_measure_capmem_i_ref(!value);
		EXPECT_EQ(reg.get_enable_measure_capmem_i_ref(), !value);
	}

	{
		bool value = reg.get_enable_dac_to_readout_0();
		reg.set_enable_dac_to_readout_0(!value);
		EXPECT_EQ(reg.get_enable_dac_to_readout_0(), !value);
	}

	{
		bool value = reg.get_enable_dac_to_readout_1();
		reg.set_enable_dac_to_readout_1(!value);
		EXPECT_EQ(reg.get_enable_dac_to_readout_1(), !value);
	}

	{
		bool value = reg.get_enable_adc_power_down();
		reg.set_enable_adc_power_down(!value);
		EXPECT_EQ(reg.get_enable_adc_power_down(), !value);
	}

	{
		bool value = reg.get_enable_adc_reset();
		reg.set_enable_adc_reset(!value);
		EXPECT_EQ(reg.get_enable_adc_reset(), !value);
	}

	{
		auto value = draw_ranged_non_default_value<ADCSourceOnBoard>(reg.get_adc_source());
		reg.set_adc_source(value);
		EXPECT_EQ(reg.get_adc_source(), value);
	}

	for (auto led : iter_all<LEDOnBoard>()) {
		bool value = reg.get_enable_led(led);
		reg.set_enable_led(led, !value);
		EXPECT_EQ(reg.get_enable_led(led), !value);
	}

	for (auto vdd : iter_all<VDDOnBoard>()) {
		bool value = reg.get_enable_vdd(vdd);
		reg.set_enable_vdd(vdd, !value);
		EXPECT_EQ(reg.get_enable_vdd(vdd), !value);
	}

	ShiftRegister reg_eq = reg;
	ShiftRegister reg_default;

	// test comparison
	ASSERT_EQ(reg, reg_eq);
	ASSERT_FALSE(reg == reg_default);

	ASSERT_NE(reg, reg_default);
	ASSERT_FALSE(reg != reg_eq);
}

TEST(ShiftRegister, EncodeDecode)
{
	ShiftRegister config;

	config.set_enable_capmem_i_ref(true);
	config.set_enable_dac_to_readout_0(true);
	config.set_enable_adc_power_down(true);
	config.set_enable_led(LEDOnBoard::LED1, true);
	config.set_enable_led(LEDOnBoard::LED2, true);
	config.set_enable_led(LEDOnBoard::LED3, true);
	config.set_enable_led(LEDOnBoard::LED4, true);
	config.set_enable_led(LEDOnBoard::LED7, false);
	config.set_enable_led(LEDOnBoard::LED8, false);
	for (auto vdd : iter_all<VDDOnBoard>()) {
		config.set_enable_vdd(vdd, false);
	}

	ShiftRegisterOnBoard coord;

	SPIShiftRegisterOnBoard ref_address;
	std::array<SPIShiftRegisterOnBoard, ShiftRegister::config_size_in_words> ref_addresses = {SPIShiftRegisterOnBoard()};
	std::array<fisch::vx::SPIShiftRegister, ShiftRegister::config_size_in_words> ref_data = {
	    fisch::vx::SPIShiftRegister(fisch::vx::SPIShiftRegister::Value(((3ul << 6) << 16) + ((1ul << 0 | 1ul << 5) << 8) + (1ul << 6)))};

	{ // write addresses
		std::vector<SPIShiftRegisterOnBoard> write_addresses;
		visit_preorder(config, coord, stadls::WriteAddressVisitor<std::vector<SPIShiftRegisterOnBoard>>{write_addresses});
		EXPECT_THAT(write_addresses, ::testing::ElementsAreArray(ref_addresses));
	}

	std::vector<fisch::vx::SPIShiftRegister> data;
	visit_preorder(config, coord, stadls::EncodeVisitor<std::vector<fisch::vx::SPIShiftRegister>>{data});
	EXPECT_THAT(data, ::testing::ElementsAreArray(ref_data));
}

TEST(ShiftRegister, CerealizeCoverage)
{
	ShiftRegister obj1, obj2;
	obj1.set_enable_capmem_i_ref(!obj1.get_enable_capmem_i_ref());
	obj1.set_enable_measure_capmem_i_ref(!obj1.get_enable_measure_capmem_i_ref());
	obj1.set_enable_dac_to_readout_0(!obj1.get_enable_dac_to_readout_0());
	obj1.set_enable_dac_to_readout_1(!obj1.get_enable_dac_to_readout_1());
	obj1.set_enable_adc_power_down(!obj1.get_enable_adc_power_down());
	obj1.set_enable_adc_reset(!obj1.get_enable_adc_reset());
	obj1.set_enable_led(LEDOnBoard::LED1, !obj1.get_enable_led(LEDOnBoard::LED1));
	obj1.set_enable_vdd(VDDOnBoard::VDD25Digital, !obj1.get_enable_vdd(VDDOnBoard::VDD25Digital));
	obj1.set_adc_source(draw_ranged_non_default_value<ADCSourceOnBoard>(obj1.get_adc_source()));

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
