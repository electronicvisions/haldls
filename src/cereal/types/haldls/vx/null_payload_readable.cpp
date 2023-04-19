#include "cereal/types/haldls/vx/null_payload_readable.h"

#include "cereal/types/haldls/cereal.tcc"
#include "haldls/vx/null_payload_readable.h"

namespace cereal {

template <class Archive>
void serialize(Archive&, haldls::vx::NullPayloadReadable&, std::uint32_t const)
{}

} // namespace cereal

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE_FREE(haldls::vx::NullPayloadReadable)
CEREAL_REGISTER_TYPE(haldls::vx::NullPayloadReadable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Container, haldls::vx::NullPayloadReadable)

CEREAL_REGISTER_DYNAMIC_INIT(haldls_vx_null_payload_readable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(haldls::vx::Encodable, haldls::vx::NullPayloadReadable)
