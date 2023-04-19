#include "cereal/types/lola/vx/memory_timing.h"

#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "halco/common/cerealization_typed_array.h"
#include "lola/vx/memory_timing.h"


EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::MemoryTiming)
CEREAL_REGISTER_TYPE(lola::vx::MemoryTiming)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::MemoryTiming)
CEREAL_CLASS_VERSION(lola::vx::MemoryTiming, 0)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_memory_timing)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::MemoryTiming)
