#include "lola/vx/cadc.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"
#include "lola/vx/gray_scale.h"

namespace lola::vx {

CADCSampleRow::CADCSampleRow() {}

CADCSampleRow::_samples_type const& CADCSampleRow::get_causal() const
{
	return m_causal;
}

void CADCSampleRow::set_causal(_samples_type const& value)
{
	m_causal = value;
}

CADCSampleRow::_samples_type const& CADCSampleRow::get_acausal() const
{
	return m_acausal;
}

void CADCSampleRow::set_acausal(_samples_type const& value)
{
	m_acausal = value;
}

bool CADCSampleRow::operator==(CADCSampleRow const& other) const
{
	return m_causal == other.m_causal && m_acausal == other.m_acausal;
}

bool CADCSampleRow::operator!=(CADCSampleRow const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CADCSampleRow const& row)
{
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;

	auto print = [](auto const& values) -> std::string {
		typedef typename std::remove_cv<
		    typename std::remove_reference<decltype(values)>::type>::type::value_type value_type;
		std::stringstream ss;
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynapseOnSynapseRow(entry, quad)];
			}
			ss << detail::gray_scale(double(acc) / double(EntryOnQuad::size * value_type::size));
		}
		return ss.str();
	};

	os << "CADCSampleRow(" << std::endl;
	os << "  causal:\t" << print(row.m_causal) << std::endl;
	os << "  acausal:\t" << print(row.m_acausal) << std::endl;
	os << ")";

	return os;
}

template <typename Archive>
void CADCSampleRow::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_causal));
	ar(CEREAL_NVP(m_acausal));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CADCSampleRow)

} // namespace lola::vx
