#pragma once
#include <boost/hana/adapt_struct.hpp>
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "haldls/vx/cadc.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

#include <pybind11/numpy.h>

namespace cereal {
class access;
} // namespace cereal

namespace lola::vx GENPYBIND_TAG_LOLA_VX {

class GENPYBIND(visible) CADCSampleRow
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::CADCSampleRowOnDLS coordinate_type;

	typedef haldls::vx::CADCSampleQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_array<Value, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _samples_type GENPYBIND(opaque);

	/** Default constructor. */
	CADCSampleRow() SYMBOL_VISIBLE;

	/** Causal sample value array. */
	_samples_type causal;

	/** Acausal sample value array. */
	_samples_type acausal;

	bool operator==(CADCSampleRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(CADCSampleRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND_MANUAL({
		auto to_numpy_template = [parent](auto const& self) {
			auto ret = pybind11::array_t<typename std::remove_reference<
			    typename std::remove_cv<decltype(self)>::type>::type::value_type::value_type>(
			    ::halco::hicann_dls::vx::SynapseOnSynapseRow::size);
			auto access = ret.mutable_unchecked();
			size_t index = 0;
			for (auto coord :
			     ::halco::common::iter_all<::halco::hicann_dls::vx::SynapseOnSynapseRow>()) {
				access[index] = self[coord];
				index++;
			}
			return ret;
		};

		auto from_numpy_template = [parent](
		                               auto& self,
		                               pybind11::array_t<typename std::remove_reference<decltype(
		                                   self)>::type::value_type::value_type> array) {
			if (array.ndim() != 1) {
				throw std::runtime_error(
				    "Number of dimensions to assign to typed_array must be one.");
			}
			if (array.size() != ::halco::hicann_dls::vx::SynapseOnSynapseRow::size) {
				throw std::runtime_error("Input shape does not match.");
			}
			auto access = array.mutable_unchecked();
			size_t index = 0;
			for (auto coord :
			     ::halco::common::iter_all<::halco::hicann_dls::vx::SynapseOnSynapseRow>()) {
				self[coord] =
				    typename std::remove_reference<decltype(self)>::type::value_type(access[index]);
				index++;
			}
		};

		{
			auto attr = parent.attr("_samples_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::CADCSampleRow::_samples_type _values_type;
			attr.attr("to_numpy") = parent->py::cpp_function(
			    [to_numpy_template](_values_type const& self) { return to_numpy_template(self); },
			    ism);
			attr.attr("from_numpy") = parent->py::cpp_function(
			    [from_numpy_template](
			        _values_type& self,
			        pybind11::array_t<_values_type::value_type::value_type> array) {
				    from_numpy_template(self, array);
			    },
			    ism);
		}
	})

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CADCSampleRow const& row) SYMBOL_VISIBLE;

private:
	friend class haldls::vx::detail::VisitPreorderImpl<CADCSampleRow>;
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::CADCSampleRow>
    : public BackendContainerBase<lola::vx::CADCSampleRow, fisch::vx::OmnibusChip>
{};

template <>
struct VisitPreorderImpl<lola::vx::CADCSampleRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::CADCSampleRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		// only do something on read
		if constexpr (!std::is_same<ContainerT, lola::vx::CADCSampleRow const>::value) {
			// trigger ADC sampling by reading one quad of causal channels
			CADCSampleQuad quad_config_trigger;
			CADCSampleQuadOnDLS quad_coord_trigger(
			    CADCSampleQuadOnSynram(
			        SynapseQuadOnSynram(
			            SynapseQuadColumnOnDLS(SynapseQuadColumnOnDLS::min),
			            coord.toSynapseRowOnSynram()),
			        CADCChannelType::causal, CADCReadoutType::trigger_read),
			    coord.toSynramOnDLS());
			visit_preorder(quad_config_trigger, quad_coord_trigger, visitor);

			for (auto const entry : iter_all<EntryOnQuad>()) {
				config.causal[SynapseOnSynapseRow(entry)] = quad_config_trigger.get_sample(entry);
			}

			// buffered read of remaining causal channel quads
			for (size_t i = SynapseQuadColumnOnDLS::min + 1; i <= SynapseQuadColumnOnDLS::max;
			     ++i) {
				auto quad_column = SynapseQuadColumnOnDLS(i);
				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::causal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.causal[syn_on_row] = quad_config.get_sample(syn);
				}
			}

			// buffered read of acausal channel quads
			for (auto quad_column : iter_all<SynapseQuadColumnOnDLS>()) {
				CADCSampleQuadOnDLS quad_coord(
				    CADCSampleQuadOnSynram(
				        SynapseQuadOnSynram(quad_column, coord.toSynapseRowOnSynram()),
				        CADCChannelType::acausal, CADCReadoutType::buffered),
				    coord.toSynramOnDLS());
				CADCSampleQuad quad_config;
				visit_preorder(quad_config, quad_coord, visitor);
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad_column);
					config.acausal[syn_on_row] = quad_config.get_sample(syn);
				}
			}
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::CADCSampleRow, causal, acausal);
