#include "stadls/vx/v1/dumperdone.tcc"

#include "haldls/cerealization.tcc"

template void stadls::vx::v1::DumperDone::CEREAL_SERIALIZE_FUNCTION_NAME(
    cereal::PortableBinaryOutputArchive&, std::uint32_t const);
template void stadls::vx::v1::DumperDone::CEREAL_SERIALIZE_FUNCTION_NAME(
    cereal::PortableBinaryInputArchive&, std::uint32_t const);
