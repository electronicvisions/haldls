#include "cereal/types/haldls/vx/global_merger_matrix.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/global_merger_matrix.h"

namespace cereal {

template <class Archive>
void serialize(Archive& ar, haldls::vx::GlobalMergerMatrixOutputConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_slow));
	ar(CEREAL_NVP(value.m_enable_event_counter));
}

template <class Archive>
void serialize(
    Archive& ar, haldls::vx::GlobalMergerMatrixInputDropCounter& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(
    Archive& ar, haldls::vx::GlobalMergerMatrixOutputEventCounter& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::GlobalMergerMatrixNode& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_mask));
	ar(CEREAL_NVP(value.m_target));
	ar(CEREAL_NVP(value.m_enable_drop_counter));
	ar(CEREAL_NVP(value.m_accept_gl1_index));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::NeuronLabelToGL1EventLUTEntry& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_gl1_index));
	ar(CEREAL_NVP(value.m_global_merger_matrix_mutable));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixOutputConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixInputDropCounter)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixOutputEventCounter)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::GlobalMergerMatrixNode)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NeuronLabelToGL1EventLUTEntry)
CEREAL_REGISTER_TYPE(haldls::vx::GlobalMergerMatrixOutputConfig)
CEREAL_REGISTER_TYPE(haldls::vx::GlobalMergerMatrixInputDropCounter)
CEREAL_REGISTER_TYPE(haldls::vx::GlobalMergerMatrixOutputEventCounter)
CEREAL_REGISTER_TYPE(haldls::vx::GlobalMergerMatrixNode)
CEREAL_REGISTER_TYPE(haldls::vx::NeuronLabelToGL1EventLUTEntry)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::GlobalMergerMatrixOutputConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::GlobalMergerMatrixInputDropCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::GlobalMergerMatrixOutputEventCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::GlobalMergerMatrixNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::NeuronLabelToGL1EventLUTEntry)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_global_merger_matrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::GlobalMergerMatrixOutputConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::GlobalMergerMatrixInputDropCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::GlobalMergerMatrixOutputEventCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::GlobalMergerMatrixNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::NeuronLabelToGL1EventLUTEntry)
