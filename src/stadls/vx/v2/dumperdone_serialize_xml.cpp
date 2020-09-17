#include "stadls/vx/v2/dumperdone.tcc"

#include "haldls/cerealization.tcc"

template void stadls::vx::v2::DumperDone::CEREAL_SERIALIZE_FUNCTION_NAME(
    cereal::XMLOutputArchive&, std::uint32_t const);
template void stadls::vx::v2::DumperDone::CEREAL_SERIALIZE_FUNCTION_NAME(
    cereal::XMLInputArchive&, std::uint32_t const);
