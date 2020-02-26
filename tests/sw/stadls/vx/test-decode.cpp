#include <gtest/gtest.h>

#include "haldls/vx/container.h"
#include "haldls/vx/is_readable.h"
#include "lola/vx/container.h"
#include "stadls/vx/decode.h"

typedef hate::type_list<haldls::vx::CommonPhyConfigChip, haldls::vx::CommonPhyConfigFPGA>
    DefaultOnedTypes;

#define PLAYBACK_CONTAINER(Name, Type)                                                             \
	TEST(Name, IsRandomizable)                                                                     \
	{                                                                                              \
		std::mt19937 rng(1234);                                                                    \
		Type config;                                                                               \
		stadls::vx::decode_random<Type>(rng, config);                                              \
		typedef                                                                                    \
		    typename haldls::vx::detail::BackendContainerTrait<Type>::default_container word_type; \
		if (haldls::vx::detail::is_simulation_readable<Type, word_type>() ||                       \
		    haldls::vx::detail::is_hardware_readable<Type, word_type>()) {                         \
			EXPECT_NE(config, Type());                                                             \
		}                                                                                          \
	}                                                                                              \
                                                                                                   \
	TEST(Name, IsOneable)                                                                          \
	{                                                                                              \
		Type config;                                                                               \
		if constexpr (!std::is_same<Type, haldls::vx::CapMemBlockConfig>::value) {                 \
			stadls::vx::decode_ones<Type>(config);                                                 \
			typedef typename haldls::vx::detail::BackendContainerTrait<Type>::default_container    \
			    word_type;                                                                         \
			if (haldls::vx::detail::is_simulation_readable<Type, word_type>() ||                   \
			    haldls::vx::detail::is_hardware_readable<Type, word_type>()) {                     \
				if constexpr (!hate::is_in_type_list<Type, DefaultOnedTypes>::value) {             \
					EXPECT_NE(config, Type());                                                     \
				}                                                                                  \
			}                                                                                      \
		}                                                                                          \
	}
#pragma push_macro("PLAYBACK_CONTAINER")
#include "haldls/vx/container.def"
#pragma pop_macro("PLAYBACK_CONTAINER")
#include "lola/vx/container.def"
