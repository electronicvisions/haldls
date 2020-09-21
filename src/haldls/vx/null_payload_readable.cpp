#include "haldls/vx/null_payload_readable.h"

#include "fisch/vx/null_payload_readable.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "haldls/cerealization.tcc"

namespace haldls {
namespace vx {

std::array<
    halco::hicann_dls::vx::NullPayloadReadableOnFPGA,
    NullPayloadReadable::read_config_size_in_words>
NullPayloadReadable::read_addresses(NullPayloadReadable::coordinate_type const& coord)
{
	return {coord};
}

std::array<
    halco::hicann_dls::vx::NullPayloadReadableOnFPGA,
    NullPayloadReadable::write_config_size_in_words>
NullPayloadReadable::write_addresses(NullPayloadReadable::coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::NullPayloadReadable, NullPayloadReadable::write_config_size_in_words>
NullPayloadReadable::encode() const
{
	return {};
}

void NullPayloadReadable::decode(std::array<
                                 fisch::vx::NullPayloadReadable,
                                 NullPayloadReadable::read_config_size_in_words> const&)
{}

std::ostream& operator<<(std::ostream& os, NullPayloadReadable const&)
{
	os << "NullPayloadReadable()";
	return os;
}

bool NullPayloadReadable::operator==(NullPayloadReadable const&) const
{
	return true;
}

bool NullPayloadReadable::operator!=(NullPayloadReadable const& other) const
{
	return !(*this == other);
}

template <class Archive>
void NullPayloadReadable::serialize(Archive&, std::uint32_t const)
{}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(NullPayloadReadable)

} // namespace vx
} // namespace haldls

CEREAL_CLASS_VERSION(haldls::vx::NullPayloadReadable, 0)
