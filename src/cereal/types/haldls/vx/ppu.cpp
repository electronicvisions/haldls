#include "cereal/types/haldls/vx/ppu.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_heap_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/ppu.h"
#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>

namespace cereal {

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemoryWord& value, std::uint32_t const)
{
	ar(CEREAL_NVP_("m_value", value.m_value));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(
    Archive& ar, haldls::vx::PPUMemoryBlock& value, std::uint32_t const)
{
	ar(CEREAL_NVP_("m_words", value.m_words));
}

template <typename Archive>
void CEREAL_SERIALIZE_FUNCTION_NAME(Archive& ar, haldls::vx::PPUMemory& value, std::uint32_t const)
{
	ar(CEREAL_NVP_("m_words", value.m_words));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::PPUControlRegister& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_cache_controller_enable));
	ar(CEREAL_NVP(value.m_inhibit_reset));
	ar(CEREAL_NVP(value.m_force_clock_on));
	ar(CEREAL_NVP(value.m_force_clock_off));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::PPUStatusRegister& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_sleep));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUMemoryWord)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUMemoryBlock)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUMemory)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUControlRegister)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::PPUStatusRegister)
CEREAL_REGISTER_TYPE(haldls::vx::PPUMemoryWord)
CEREAL_REGISTER_TYPE(haldls::vx::PPUMemoryBlock)
CEREAL_REGISTER_TYPE(haldls::vx::PPUMemory)
CEREAL_REGISTER_TYPE(haldls::vx::PPUControlRegister)
CEREAL_REGISTER_TYPE(haldls::vx::PPUStatusRegister)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PPUMemoryWord)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PPUMemoryBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PPUMemory)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PPUControlRegister)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::PPUStatusRegister)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_ppu)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PPUMemoryWord)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PPUMemoryBlock)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PPUMemory)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PPUControlRegister)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::PPUStatusRegister)
