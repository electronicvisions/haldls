#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "fisch/vx/omnibus.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/vector_generator.h"
#include "stadls/visitors.h"
#include "test-helper.h"

using namespace haldls::vx;
using namespace halco::hicann_dls::vx;
using namespace halco::common;

typedef std::vector<halco::hicann_dls::vx::OmnibusAddress> addresses_type;
typedef std::vector<fisch::vx::Omnibus> words_type;

template <>
struct random_value<VectorGeneratorControl::Source>
{
	VectorGeneratorControl::Source operator()(VectorGeneratorControl::Source const& exclude)
	{
		size_t rnd;
		do {
			rnd = rand() % 4;
		} while (rnd == static_cast<size_t>(exclude));
		return static_cast<VectorGeneratorControl::Source>(rnd);
	};
};

template <>
struct random_value<VectorGeneratorControl::Notification>
{
	VectorGeneratorControl::Notification operator()(
	    VectorGeneratorControl::Notification const& exclude)
	{
		size_t rnd;
		do {
			rnd = rand() % 2;
		} while (rnd == static_cast<size_t>(exclude));
		return static_cast<VectorGeneratorControl::Notification>(rnd);
	};
};

template <>
struct random_value<VectorGeneratorControl::Trigger>
{
	VectorGeneratorControl::Trigger operator()(VectorGeneratorControl::Trigger const& exclude)
	{
		size_t rnd;
		do {
			rnd = rand() % 2;
		} while (rnd == static_cast<size_t>(exclude));
		return static_cast<VectorGeneratorControl::Trigger>(rnd);
	};
};

template <>
struct random_value<VectorGeneratorControl::Signal>
{
	VectorGeneratorControl::Signal operator()(VectorGeneratorControl::Signal const& exclude)
	{
		size_t rnd;
		do {
			rnd = rand() % 3;
		} while (rnd == static_cast<size_t>(exclude));
		return static_cast<VectorGeneratorControl::Signal>(rnd);
	};
};

HALDLS_TEST(
    VectorGeneratorControl,
    (source_count)(sources)(source_waits)(event_pack)(enable_ignore_zeros)(notification)(trigger)(
        signal)(lookup_table_entry_start))

TEST(VectorGeneratorControl, EncodeDecode)
{
	VectorGeneratorControl config;
	config.set_enable_ignore_zeros(true);
	config.set_lookup_table_entry_start(
	    halco::hicann_dls::vx::VectorGeneratorLUTEntryOnVectorGeneratorLUT(123));
	config.set_trigger(VectorGeneratorControl::Trigger::gpio);
	config.set_signal(VectorGeneratorControl::Signal::notification);

	VectorGeneratorControlOnFPGA coord;

	std::array<
	    halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorControl::write_config_size_in_words>
	    ref_addresses = {OmnibusAddress(0x84000000)};

	std::array<fisch::vx::Omnibus, VectorGeneratorControl::write_config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(
	        fisch::vx::Omnibus::Value{(1ul << 8) | (123ul << 24) | (1ul << 0) | (1ul << 3)})};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}

HALDLS_TEST(VectorGeneratorLUTEntry, (value))

TEST(VectorGeneratorLUTEntry, EncodeDecode)
{
	VectorGeneratorLUTEntry config;
	config.set_value(VectorGeneratorLUTEntry::Value(0x123));

	VectorGeneratorLUTEntryOnFPGA coord(VectorGeneratorLUTEntryOnVectorGeneratorLUT(123));

	std::array<halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorLUTEntry::config_size_in_words>
	    ref_addresses = {OmnibusAddress(0x84000100 + 123)};

	std::array<fisch::vx::Omnibus, VectorGeneratorLUTEntry::config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x123})};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}

HALDLS_TEST(VectorGeneratorNotificationAddress, (value))

TEST(VectorGeneratorNotificationAddress, EncodeDecode)
{
	VectorGeneratorNotificationAddress config;
	config.set_value(VectorGeneratorNotificationAddress::Value(0x12345678));

	VectorGeneratorNotificationAddressOnFPGA coord;

	std::array<
	    halco::hicann_dls::vx::OmnibusAddress,
	    VectorGeneratorNotificationAddress::config_size_in_words>
	    ref_addresses = {OmnibusAddress(0x84000001)};

	std::array<fisch::vx::Omnibus, VectorGeneratorNotificationAddress::config_size_in_words>
	    ref_data = {fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x12345678})};

	HALDLS_TEST_ENCODE_DECODE(config, coord, ref_addresses, ref_data)
}

TEST(VectorGeneratorTrigger, General)
{
	VectorGeneratorTrigger config;

	VectorGeneratorTrigger config_eq = config;
	VectorGeneratorTrigger config_default;

	// test comparison
	ASSERT_EQ(config, config_eq);
	ASSERT_TRUE(config == config_default);

	ASSERT_EQ(config, config_default);
	ASSERT_FALSE(config != config_eq);
}

TEST(VectorGeneratorTrigger, CerealizeCoverage)
{
	VectorGeneratorTrigger obj1, obj2;

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

TEST(VectorGeneratorTrigger, EncodeDecode)
{
	VectorGeneratorTrigger config;

	VectorGeneratorTriggerOnFPGA coord;

	std::array<
	    halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorTrigger::write_config_size_in_words>
	    ref_addresses = {OmnibusAddress(0x84000002)};

	std::array<fisch::vx::Omnibus, VectorGeneratorTrigger::write_config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(fisch::vx::Omnibus::Value{0x0})};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}

HALDLS_TEST(VectorGeneratorFIFOWord, (values)(last)(enable))

TEST(VectorGeneratorFIFOWord, EncodeDecode)
{
	VectorGeneratorFIFOWord config;
	auto values = config.get_values();
	values[EntryOnQuad(0)] = VectorGeneratorFIFOWord::Value(12);
	config.set_values(values);

	auto last = config.get_last();
	last[EntryOnQuad(1)] = true;
	config.set_last(last);

	auto enable = config.get_enable();
	enable[EntryOnQuad(0)] = true;
	enable[EntryOnQuad(1)] = true;
	config.set_enable(enable);

	VectorGeneratorFIFOWordOnFPGA coord;

	std::array<
	    halco::hicann_dls::vx::OmnibusAddress, VectorGeneratorFIFOWord::write_config_size_in_words>
	    ref_addresses = {OmnibusAddress(0x84000200)};

	std::array<fisch::vx::Omnibus, VectorGeneratorFIFOWord::write_config_size_in_words> ref_data = {
	    fisch::vx::Omnibus(
	        fisch::vx::Omnibus::Value{(12ul << 24) + ((1ul << 7) << 16)},
	        {false, false, true, true})};

	HALDLS_TEST_ENCODE(config, coord, ref_addresses, ref_data)
}
