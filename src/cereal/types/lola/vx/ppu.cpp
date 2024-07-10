#include "cereal/types/lola/vx/ppu.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "cereal/types/lola/detail/hana.tcc"
#include "lola/vx/ppu.h"
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>


namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, lola::vx::ExternalPPUMemoryBlock& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP_("m_bytes", value.m_bytes));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, lola::vx::ExternalPPUDRAMMemoryBlock& value, std::uint32_t const /* version */)
{
	ar(CEREAL_NVP_("m_bytes", value.m_bytes));
}

} // namespace cereal

CEREAL_REGISTER_TYPE(lola::vx::ExternalPPUMemoryBlock)
CEREAL_REGISTER_TYPE(lola::vx::ExternalPPUMemory)
CEREAL_REGISTER_TYPE(lola::vx::ExternalPPUDRAMMemoryBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::ExternalPPUMemoryBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::ExternalPPUMemory)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, lola::vx::ExternalPPUDRAMMemoryBlock)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::PPUProgram::Symbol)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::ExternalPPUMemoryBlock)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::ExternalPPUMemory)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::ExternalPPUDRAMMemoryBlock)
CEREAL_CLASS_VERSION(lola::vx::ExternalPPUMemoryBlock, 0)
CEREAL_CLASS_VERSION(lola::vx::ExternalPPUMemory, 0)
CEREAL_CLASS_VERSION(lola::vx::ExternalPPUDRAMMemoryBlock, 0)
CEREAL_REGISTER_DYNAMIC_INIT(lola_vx_ppu)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::ExternalPPUMemoryBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::ExternalPPUMemory)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, lola::vx::ExternalPPUDRAMMemoryBlock)
