#include "cereal/types/haldls/vx/v4/global_merger_matrix.h"

#include "cereal/types/halco/common/geometry.h"
#include "cereal/types/halco/common/typed_array.h"
#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/v4/global_merger_matrix.h"

namespace cereal {

template <class Archive>
void serialize(
    Archive& ar, haldls::vx::v4::GlobalMergerMatrixOutputConfig& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_enable_slow));
	ar(CEREAL_NVP(value.m_enable_event_counter));
}

template <class Archive>
void serialize(
    Archive& ar, haldls::vx::v4::GlobalMergerMatrixInputDropCounter& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(
    Archive& ar, haldls::vx::v4::GlobalMergerMatrixOutputEventCounter& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_value));
}

template <class Archive>
void serialize(Archive& ar, haldls::vx::v4::GlobalMergerMatrixNode& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_mask));
	ar(CEREAL_NVP(value.m_target));
	ar(CEREAL_NVP(value.m_enable_drop_counter));
	ar(CEREAL_NVP(value.m_accept_gl1_index));
}

template <class Archive>
void serialize(
    Archive& ar, haldls::vx::v4::NeuronLabelToGL1EventLUTEntry& value, std::uint32_t const)
{
	ar(CEREAL_NVP(value.m_gl1_index));
	ar(CEREAL_NVP(value.m_global_merger_matrix_mutable));
}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixOutputConfig)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixInputDropCounter)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixOutputEventCounter)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::GlobalMergerMatrixNode)
EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::v4::NeuronLabelToGL1EventLUTEntry)
CEREAL_REGISTER_TYPE(haldls::vx::v4::GlobalMergerMatrixOutputConfig)
CEREAL_REGISTER_TYPE(haldls::vx::v4::GlobalMergerMatrixInputDropCounter)
CEREAL_REGISTER_TYPE(haldls::vx::v4::GlobalMergerMatrixOutputEventCounter)
CEREAL_REGISTER_TYPE(haldls::vx::v4::GlobalMergerMatrixNode)
CEREAL_REGISTER_TYPE(haldls::vx::v4::NeuronLabelToGL1EventLUTEntry)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::v4::GlobalMergerMatrixOutputConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::v4::GlobalMergerMatrixInputDropCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::v4::GlobalMergerMatrixOutputEventCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::v4::GlobalMergerMatrixNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Container, haldls::vx::v4::NeuronLabelToGL1EventLUTEntry)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_v4_global_merger_matrix)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::v4::GlobalMergerMatrixOutputConfig)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::v4::GlobalMergerMatrixInputDropCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::v4::GlobalMergerMatrixOutputEventCounter)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::v4::GlobalMergerMatrixNode)
CEREAL_REGISTER_POLYMORPHIC_RELATION(
    haldls::vx::Encodable, haldls::vx::v4::NeuronLabelToGL1EventLUTEntry)
