#include "lola/vx/synapse.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"

namespace lola::vx {

SynapseRow::SynapseRow() {}

SynapseRow::_weights_type const& SynapseRow::get_weights() const
{
	return m_weights;
}

void SynapseRow::set_weights(_weights_type const& value)
{
	m_weights = value;
}

SynapseRow::_addresses_type const& SynapseRow::get_addresses() const
{
	return m_addresses;
}

void SynapseRow::set_addresses(_addresses_type const& value)
{
	m_addresses = value;
}

SynapseRow::_time_calibs_type const& SynapseRow::get_time_calibs() const
{
	return m_time_calibs;
}

void SynapseRow::set_time_calibs(_time_calibs_type const& value)
{
	m_time_calibs = value;
}

SynapseRow::_amp_calibs_type const& SynapseRow::get_amp_calibs() const
{
	return m_amp_calibs;
}

void SynapseRow::set_amp_calibs(_amp_calibs_type const& value)
{
	m_amp_calibs = value;
}

bool SynapseRow::operator==(SynapseRow const& other) const
{
	return m_weights == other.m_weights && m_addresses == other.m_addresses &&
	       m_time_calibs == other.m_time_calibs && m_amp_calibs == other.m_amp_calibs;
}

bool SynapseRow::operator!=(SynapseRow const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SynapseRow const& row)
{
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;

	auto print = [](auto const& values) -> std::string {
		// value depicted by height of bar, dot == 0
		static const std::array<std::string, 8> grayscale_map = {"·", "▁", "▂", "▃",
		                                                         "▄", "▅", "▆", "▇"};
		typedef typename std::remove_cv<
		    typename std::remove_reference<decltype(values)>::type>::type::value_type value_type;
		std::stringstream ss;
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			// average quads of synapse values for 80-character width of printout
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynapseOnSynapseRow(entry, quad)];
			}
			ss << grayscale_map[int(double(acc / EntryOnQuad::size) / value_type::size * grayscale_map.size())];
		}
		return ss.str();
	};

	os << "SynapseRow(" << std::endl;
	os << "  weights:\t" << print(row.m_weights) << std::endl;
	os << "  addresses:\t" << print(row.m_addresses) << std::endl;
	os << "  time_calibs:\t" << print(row.m_time_calibs) << std::endl;
	os << "  amp_calibs:\t" << print(row.m_amp_calibs) << std::endl;
	os << ")";

	return os;
}

template <typename Archive>
void SynapseRow::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_weights));
	ar(CEREAL_NVP(m_addresses));
	ar(CEREAL_NVP(m_time_calibs));
	ar(CEREAL_NVP(m_amp_calibs));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseRow)

} // namespace lola::vx
