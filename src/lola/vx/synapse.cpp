#include "lola/vx/synapse.h"

#include <boost/hana/adapt_struct.hpp>
#include "lola/vx/gray_scale.h"
#include "lola/vx/hana.h"

namespace lola::vx {

SynapseRow::SynapseRow() {}

bool SynapseRow::operator==(SynapseRow const& other) const
{
	return equal(*this, other);
}

bool SynapseRow::operator!=(SynapseRow const& other) const
{
	return unequal(*this, other);
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
	boost::hana::for_each(boost::hana::keys(row), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print(boost::hana::at_key(row, key)) << std::endl;
	});
	os << ")";

	return os;
}


SynapseMatrix::SynapseMatrix() {}

bool SynapseMatrix::operator==(SynapseMatrix const& other) const
{
	return equal(*this, other);
}

bool SynapseMatrix::operator!=(SynapseMatrix const& other) const
{
	return unequal(*this, other);
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
	boost::hana::for_each(boost::hana::keys(matrix), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print(boost::hana::at_key(matrix, key)) << std::endl;
	});
	os << ")";
	return os;
}


CorrelationResetRow::CorrelationResetRow() {}

bool CorrelationResetRow::operator==(CorrelationResetRow const& /* other */) const
{
	return true;
}

bool CorrelationResetRow::operator!=(CorrelationResetRow const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CorrelationResetRow const& /* row */)
{
	os << "CorrelationResetRow()";
	return os;
}

} // namespace lola::vx
