#include "stadls/vx/v3/dumperdone.tcc"

#include "haldls/cerealization.tcc"

template void stadls::vx::v3::DumperDone::CEREAL_SERIALIZE_FUNCTION_NAME(
    cereal::JSONOutputArchive&, std::uint32_t const);
template void stadls::vx::v3::DumperDone::CEREAL_SERIALIZE_FUNCTION_NAME(
    cereal::JSONInputArchive&, std::uint32_t const);
