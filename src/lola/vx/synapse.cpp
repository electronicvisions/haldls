#include "lola/vx/synapse.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"
#include "lola/vx/gray_scale.h"

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
		typedef typename std::remove_cv<
		    typename std::remove_reference<decltype(values)>::type>::type::value_type value_type;
		std::stringstream ss;
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			// average quads of synapse values for 80-character width of printout
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynapseOnSynapseRow(entry, quad)];
			}
			ss << detail::gray_scale(double(acc) / double(EntryOnQuad::size * value_type::size));
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


SynapseMatrix::SynapseMatrix() {}

SynapseMatrix::_weights_type const& SynapseMatrix::get_weights() const
{
	return m_weights;
}

void SynapseMatrix::set_weights(_weights_type const& value)
{
	m_weights = value;
}

SynapseMatrix::_addresses_type const& SynapseMatrix::get_addresses() const
{
	return m_addresses;
}

void SynapseMatrix::set_addresses(_addresses_type const& value)
{
	m_addresses = value;
}

SynapseMatrix::_time_calibs_type const& SynapseMatrix::get_time_calibs() const
{
	return m_time_calibs;
}

void SynapseMatrix::set_time_calibs(_time_calibs_type const& value)
{
	m_time_calibs = value;
}

SynapseMatrix::_amp_calibs_type const& SynapseMatrix::get_amp_calibs() const
{
	return m_amp_calibs;
}

void SynapseMatrix::set_amp_calibs(_amp_calibs_type const& value)
{
	m_amp_calibs = value;
}

bool SynapseMatrix::operator==(SynapseMatrix const& other) const
{
	return m_weights == other.m_weights && m_addresses == other.m_addresses &&
	       m_time_calibs == other.m_time_calibs && m_amp_calibs == other.m_amp_calibs;
}

bool SynapseMatrix::operator!=(SynapseMatrix const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, SynapseMatrix const& matrix)
{
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;

	auto print = [](auto const& values) -> std::string {
		typedef typename std::remove_cv<typename std::remove_reference<decltype(values)>::type>::
		    type::value_type::value_type value_type;
		std::stringstream ss;
		constexpr static size_t vertical_average_count = 8;
		// average 8 vertical and 4 horizontal for 80-character wide printout and approx. quadratic
		// shape
		for (size_t rows = 0; rows < SynapseRowOnSynram::size / vertical_average_count; ++rows) {
			for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
				size_t acc = 0;
				for (auto entry : iter_all<EntryOnQuad>()) {
					for (size_t row_in_row_quad = 0; row_in_row_quad < vertical_average_count;
					     ++row_in_row_quad) {
						acc += values[SynapseRowOnSynram(
						    rows * vertical_average_count + row_in_row_quad)]
						             [SynapseOnSynapseRow(entry, quad)];
					}
				}
				ss << detail::gray_scale(
				    double(acc) /
				    double(vertical_average_count * EntryOnQuad::size * value_type::size));
			}
			ss << std::endl << "\t\t";
		}
		return ss.str();
	};

	os << "SynapseMatrix(" << std::endl;
	os << "  weights:\t" << print(matrix.m_weights) << std::endl;
	os << "  addresses:\t" << print(matrix.m_addresses) << std::endl;
	os << "  time_calibs:\t" << print(matrix.m_time_calibs) << std::endl;
	os << "  amp_calibs:\t" << print(matrix.m_amp_calibs) << std::endl;
	os << ")";
	return os;
}

template <typename Archive>
void SynapseMatrix::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_weights));
	ar(CEREAL_NVP(m_addresses));
	ar(CEREAL_NVP(m_time_calibs));
	ar(CEREAL_NVP(m_amp_calibs));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SynapseMatrix)

} // namespace lola::vx
