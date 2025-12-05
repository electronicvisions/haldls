#include <gtest/gtest.h>

#include "fisch/vx/container_cast.h"
#include "fisch/vx/traits.h"
#include "haldls/vx/is_readable.h"
#include "haldls/vx/v3/container.h"
#include "lola/vx/v3/container.h"
#include "stadls/vx/v3/decode.h"
#include <random>

typedef hate::type_list<
    haldls::vx::INA219Status,
    haldls::vx::NullPayloadReadable,
    haldls::vx::TMP112Status,
    lola::vx::v3::AtomicNeuron,
    lola::vx::v3::Chip,
    lola::vx::v3::LogicalNeuron,
    lola::vx::v3::ChipAndMultichipJboaLeafFPGA,
    lola::vx::v3::ChipAndSinglechipFPGA>
    NotRandomizableTypes;

typedef hate::type_list<
    haldls::vx::NullPayloadReadable,
    haldls::vx::INA219Status,
    haldls::vx::v3::CapMemBlockConfig,
    lola::vx::v3::AtomicNeuron,
    lola::vx::v3::CapMem,
    lola::vx::v3::Chip,
    lola::vx::v3::LogicalNeuron,
    lola::vx::v3::ChipAndMultichipJboaLeafFPGA, // Not oneable as chip is not oneable
    lola::vx::v3::ChipAndSinglechipFPGA>        // Not oneable as chip is not oneable
    NotOneableTypes;

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	TEST(Name, IsRandomizable)                                                                     \
	{                                                                                              \
		if constexpr (!hate::is_in_type_list<Type, NotRandomizableTypes>::value) {                 \
			std::mt19937 rng(std::random_device{}());                                              \
			Type config;                                                                           \
			typedef typename haldls::vx::detail::BackendContainerTrait<Type>::default_container    \
			    word_type;                                                                         \
			if (haldls::vx::detail::IsReadable<Type>::value &&                                     \
			    fisch::vx::IsReadable<decltype(fisch::vx::container_cast(                          \
			        std::declval<word_type>()))>::value) {                                         \
				EXPECT_NO_THROW((stadls::vx::decode_random<Type>(rng, config)));                   \
			}                                                                                      \
		}                                                                                          \
	}                                                                                              \
                                                                                                   \
	TEST(Name, IsOneable)                                                                          \
	{                                                                                              \
		if constexpr (!hate::is_in_type_list<Type, NotOneableTypes>::value) {                      \
			Type config;                                                                           \
			typedef typename haldls::vx::detail::BackendContainerTrait<Type>::default_container    \
			    word_type;                                                                         \
			if (haldls::vx::detail::IsReadable<Type>::value &&                                     \
			    fisch::vx::IsReadable<decltype(fisch::vx::container_cast(                          \
			        std::declval<word_type>()))>::value) {                                         \
				EXPECT_NO_THROW((stadls::vx::decode_ones<Type>(config)));                          \
			}                                                                                      \
		}                                                                                          \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v3/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v3/container.def"
