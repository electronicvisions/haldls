#include "lola/vx/v1/cadc.h"

#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "haldls/cerealization.h"
#include "lola/vx/gray_scale.h"
#include "lola/vx/hana.h"

namespace lola::vx::v1 {

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
	using namespace halco::hicann_dls::vx::v1;
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


CADCSamples::CADCSamples() : causal(), acausal() {}

bool CADCSamples::operator==(CADCSamples const& other) const
{
	return causal == other.causal && acausal == other.acausal;
}

bool CADCSamples::operator!=(CADCSamples const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, CADCSamples const& samples)
{
	using namespace halco::hicann_dls::vx::v1;
	using namespace halco::common;

	auto print = [](auto const& values) -> std::string {
		typedef typename std::remove_cv<typename std::remove_reference<decltype(values)>::type>::
		    type::value_type::value_type value_type;
		std::stringstream ss;
		ss << "top:\t";
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynramOnDLS(0)][SynapseOnSynapseRow(entry, quad)];
			}
			ss << detail::gray_scale(double(acc) / double(EntryOnQuad::size * value_type::size));
		}
		ss << std::endl;
		ss << "\t\tbottom:\t";
		for (auto quad : iter_all<SynapseQuadColumnOnDLS>()) {
			size_t acc = 0;
			for (auto entry : iter_all<EntryOnQuad>()) {
				acc += values[SynramOnDLS(1)][SynapseOnSynapseRow(entry, quad)];
			}
			ss << detail::gray_scale(double(acc) / double(EntryOnQuad::size * value_type::size));
		}
		ss << std::endl;
		return ss.str();
	};

	os << "CADCSamples(" << std::endl;
	os << "  causal:\t" << print(samples.causal) << std::endl;
	os << "  acausal:\t" << print(samples.acausal) << std::endl;
	os << ")";

	return os;
}

} // namespace lola::vx::v1
