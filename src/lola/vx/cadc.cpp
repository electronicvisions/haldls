#include "lola/vx/cadc.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"
#include "lola/vx/gray_scale.h"
#include "lola/vx/hana.h"

namespace lola::vx {

CADCSampleRow::CADCSampleRow() {}

bool CADCSampleRow::operator==(CADCSampleRow const& other) const
{
	return equal(*this, other);
}

bool CADCSampleRow::operator!=(CADCSampleRow const& other) const
{
	return unequal(*this, other);
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
	boost::hana::for_each(boost::hana::keys(row), [&](auto const key) {
		os << "  " << key.c_str() << ":\t" << print(boost::hana::at_key(row, key)) << std::endl;
	});
	os << ")";

	return os;
}

} // namespace lola::vx
