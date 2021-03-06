#include <gtest/gtest.h>

#include "fisch/vx/traits.h"
#include "haldls/vx/is_readable.h"
#include "haldls/vx/v2/container.h"
#include "lola/vx/v2/container.h"
#include "stadls/vx/v2/decode.h"

typedef hate::
    type_list<lola::vx::v2::AtomicNeuron, haldls::vx::NullPayloadReadable, haldls::vx::INA219Status>
        NotRandomizableTypes;

typedef hate::type_list<
    lola::vx::v2::AtomicNeuron,
    haldls::vx::NullPayloadReadable,
    haldls::vx::CommonPhyConfigChip,
    haldls::vx::CommonPhyConfigFPGA,
    haldls::vx::v2::CapMemBlockConfig,
    haldls::vx::INA219Status>
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
			    fisch::vx::IsReadable<word_type>::value) {                                         \
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
			    fisch::vx::IsReadable<word_type>::value) {                                         \
				EXPECT_NE(config, Type());                                                         \
			}                                                                                      \
		}                                                                                          \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/v2/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/v2/container.def"
