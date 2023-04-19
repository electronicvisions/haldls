#include "haldls/vx/null_payload_readable.h"

#include "fisch/vx/word_access/type/null_payload_readable.h"
#include "halco/hicann-dls/vx/fpga.h"
#include "haldls/vx/container.tcc"

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

std::array<
    fisch::vx::word_access_type::NullPayloadReadable,
    NullPayloadReadable::write_config_size_in_words>
NullPayloadReadable::encode() const
{
	return {};
}

void NullPayloadReadable::decode(std::array<
                                 fisch::vx::word_access_type::NullPayloadReadable,
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

} // namespace vx
} // namespace haldls

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(haldls::vx::NullPayloadReadable)
