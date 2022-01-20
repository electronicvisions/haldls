#include <gtest/gtest.h>

#include "fisch/vx/container_cast.h"
#include "fisch/vx/traits.h"
#include "haldls/vx/is_readable.h"
#include "haldls/vx/v3/container.h"
#include "lola/vx/v3/container.h"
#include "stadls/vx/v3/decode.h"
#include <random>

typedef hate::type_list<
    lola::vx::v3::AtomicNeuron,
    haldls::vx::NullPayloadReadable,
    haldls::vx::INA219Status,
    haldls::vx::VectorGeneratorControl>
    NotRandomizableTypes;

typedef hate::type_list<
    lola::vx::v3::AtomicNeuron,
    haldls::vx::NullPayloadReadable,
    haldls::vx::CommonPhyConfigChip,
    haldls::vx::CommonPhyConfigFPGA,
    haldls::vx::v3::CapMemBlockConfig,
    haldls::vx::INA219Status,
    haldls::vx::VectorGeneratorControl>
    NotOneableTypes;

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	TEST(Name, IsRandomizable)                                                                     \
	{                                                                                              \
		if constexpr (!hate::is_in_type_list<Type, NotRandomizableTypes>::value) {                 \
			std::mt19937 rng(1234);                                                                \
			Type config;                                                                           \
			stadls::vx::decode_random<Type>(rng, config);                                          \
			typedef typename haldls::vx::detail::BackendContainerTrait<Type>::default_container    \
			    word_type;                                                                         \
			if (haldls::vx::detail::IsReadable<Type>::value &&                                     \
			    fisch::vx::IsReadable<decltype(                                                    \
			        fisch::vx::container_cast(std::declval<word_type>()))>::value) {               \
				EXPECT_NE(config, Type());                                                         \
			}                                                                                      \
		}                                                                                          \
	}                                                                                              \
                                                                                                   \
	TEST(Name, IsOneable)                                                                          \
	{                                                                                              \
		if constexpr (!hate::is_in_type_list<Type, NotOneableTypes>::value) {                      \
			Type config;                                                                           \
			stadls::vx::decode_ones<Type>(config);                                                 \
			typedef typename haldls::vx::detail::BackendContainerTrait<Type>::default_container    \
			    word_type;                                                                         \
			if (haldls::vx::detail::IsReadable<Type>::value &&                                     \
			    fisch::vx::IsReadable<decltype(                                                    \
			        fisch::vx::container_cast(std::declval<word_type>()))>::value) {               \
				EXPECT_NE(config, Type());                                                         \
			}                                                                                      \
		}                                                                                          \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v3/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v3/container.def"
