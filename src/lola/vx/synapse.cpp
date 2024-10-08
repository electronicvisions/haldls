#include "lola/vx/synapse.h"

#include "haldls/vx/container.tcc"
#include "hate/indent.h"
#include "hate/join.h"
#include "lola/vx/gray_scale.h"
#include "lola/vx/hana.h"
#include <boost/hana/adapt_struct.hpp>

namespace lola::vx {

namespace {

template <typename ValuesT>
std::string print_row(ValuesT const& values)
{
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;
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
}

} // namespace


ColumnCorrelationRow::ColumnCorrelationRow() {}

bool ColumnCorrelationRow::operator==(ColumnCorrelationRow const& other) const
{
	return equal(*this, other);
}

bool ColumnCorrelationRow::operator!=(ColumnCorrelationRow const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ColumnCorrelationRow const& row)
{
	return print(os, row);
}


ColumnCurrentRow::ColumnCurrentRow() {}

bool ColumnCurrentRow::operator==(ColumnCurrentRow const& other) const
{
	return equal(*this, other);
}

bool ColumnCurrentRow::operator!=(ColumnCurrentRow const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, ColumnCurrentRow const& row)
{
	return print(os, row);
}


SynapseWeightRow::SynapseWeightRow() {}

bool SynapseWeightRow::operator==(SynapseWeightRow const& other) const
{
	return equal(*this, other);
}

bool SynapseWeightRow::operator!=(SynapseWeightRow const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseWeightRow const& row)
{
	return print(os, row);
}


SynapseLabelRow::SynapseLabelRow() {}

bool SynapseLabelRow::operator==(SynapseLabelRow const& other) const
{
	return equal(*this, other);
}

bool SynapseLabelRow::operator!=(SynapseLabelRow const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseLabelRow const& row)
{
	return print(os, row);
}


SynapseCorrelationCalibRow::SynapseCorrelationCalibRow() {}

bool SynapseCorrelationCalibRow::operator==(SynapseCorrelationCalibRow const& other) const
{
	return equal(*this, other);
}

bool SynapseCorrelationCalibRow::operator!=(SynapseCorrelationCalibRow const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseCorrelationCalibRow const& row)
{
	return print(os, row);
}


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
	return print(os, row);
}


namespace {

template <typename ValuesT>
std::string print_matrix(ValuesT const& values)
{
	using namespace halco::hicann_dls::vx;
	using namespace halco::common;
	typedef typename std::remove_cv<typename std::remove_reference<decltype(values)>::type>::type::
	    value_type::value_type value_type;
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
					acc +=
					    values[SynapseRowOnSynram(rows * vertical_average_count + row_in_row_quad)]
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
}

} // namespace


SynapseWeightMatrix::SynapseWeightMatrix() {}

bool SynapseWeightMatrix::operator==(SynapseWeightMatrix const& other) const
{
	return equal(*this, other);
}

bool SynapseWeightMatrix::operator!=(SynapseWeightMatrix const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseWeightMatrix const& matrix)
{
	os << "SynapseWeightMatrix(" << std::endl;
	boost::hana::for_each(boost::hana::keys(matrix), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print_matrix(boost::hana::at_key(matrix, key))
		   << std::endl;
	});
	os << ")";
	return os;
}


SynapseLabelMatrix::SynapseLabelMatrix() {}

bool SynapseLabelMatrix::operator==(SynapseLabelMatrix const& other) const
{
	return equal(*this, other);
}

bool SynapseLabelMatrix::operator!=(SynapseLabelMatrix const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseLabelMatrix const& matrix)
{
	os << "SynapseLabelMatrix(" << std::endl;
	boost::hana::for_each(boost::hana::keys(matrix), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print_matrix(boost::hana::at_key(matrix, key))
		   << std::endl;
	});
	os << ")";
	return os;
}


SynapseCorrelationCalibMatrix::SynapseCorrelationCalibMatrix() {}

bool SynapseCorrelationCalibMatrix::operator==(SynapseCorrelationCalibMatrix const& other) const
{
	return equal(*this, other);
}

bool SynapseCorrelationCalibMatrix::operator!=(SynapseCorrelationCalibMatrix const& other) const
{
	return unequal(*this, other);
}

std::ostream& operator<<(std::ostream& os, SynapseCorrelationCalibMatrix const& matrix)
{
	os << "SynapseCorrelationCalibMatrix(" << std::endl;
	boost::hana::for_each(boost::hana::keys(matrix), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print_matrix(boost::hana::at_key(matrix, key))
		   << std::endl;
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
	os << "SynapseMatrix(" << std::endl;
	boost::hana::for_each(boost::hana::keys(matrix), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print_matrix(boost::hana::at_key(matrix, key))
		   << std::endl;
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

EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::ColumnCorrelationRow)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::ColumnCurrentRow)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseRow)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseWeightRow)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseLabelRow)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseCorrelationCalibRow)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseWeightMatrix)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseLabelMatrix)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseCorrelationCalibMatrix)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::SynapseMatrix)
EXPLICIT_INSTANTIATE_HALDLS_CONTAINER_BASE(lola::vx::CorrelationResetRow)
