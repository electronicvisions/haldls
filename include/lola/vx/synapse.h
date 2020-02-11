#pragma once
#include <boost/hana/adapt_struct.hpp>
#include "halco/common/typed_heap_array.h"
#include "haldls/vx/common.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

#include <pybind11/numpy.h>

namespace cereal {
class access;
} // namespace cereal

namespace lola::vx GENPYBIND_TAG_LOLA_VX {

class GENPYBIND(visible) SynapseRow : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseRowOnDLS coordinate_type;

	typedef haldls::vx::SynapseQuad::Synapse::Weight Weight GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::Address Address GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::AmpCalib AmpCalib GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Weight, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _weights_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<Address, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _addresses_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<TimeCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _time_calibs_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<AmpCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _amp_calibs_type GENPYBIND(opaque);

	/** Default constructor. */
	SynapseRow() SYMBOL_VISIBLE;

	/** Synapse weight array. */
	_weights_type weights;

	/** Synapse address array. */
	_addresses_type addresses;

	/** Synapse correlation time calibration array. */
	_time_calibs_type time_calibs;

	/** Synapse correlation amplitude calibration array. */
	_amp_calibs_type amp_calibs;

	bool operator==(SynapseRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseRow const& row) SYMBOL_VISIBLE;

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
				    "Number of dimensions to assign to typed_heap_array must be one.");
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
			auto attr = parent.attr("_weights_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseRow::_weights_type _values_type;
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
		{
			auto attr = parent.attr("_addresses_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseRow::_addresses_type _values_type;
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
		{
			auto attr = parent.attr("_time_calibs_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseRow::_time_calibs_type _values_type;
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
		{
			auto attr = parent.attr("_amp_calibs_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseRow::_amp_calibs_type _values_type;
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

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseRow>;
};


class GENPYBIND(visible) SynapseMatrix : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynramOnDLS coordinate_type;

	template <typename T>
	using matrix_type = halco::common::typed_heap_array<
	    halco::common::typed_heap_array<T, halco::hicann_dls::vx::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::SynapseRowOnSynram>;

	typedef haldls::vx::SynapseQuad::Synapse::Weight Weight GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::Address Address GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::AmpCalib AmpCalib GENPYBIND(visible);

	typedef matrix_type<Weight> _weights_type GENPYBIND(opaque);
	typedef matrix_type<Address> _addresses_type GENPYBIND(opaque);
	typedef matrix_type<TimeCalib> _time_calibs_type GENPYBIND(opaque);
	typedef matrix_type<AmpCalib> _amp_calibs_type GENPYBIND(opaque);

	/** Default constructor. */
	SynapseMatrix() SYMBOL_VISIBLE;

	/** Synapse weight array. */
	_weights_type weights;

	/** Synapse address array. */
	_addresses_type addresses;

	/** Synapse correlation time calibration array. */
	_time_calibs_type time_calibs;

	/** Synapse correlation amplitude calibration array. */
	_amp_calibs_type amp_calibs;

	bool operator==(SynapseMatrix const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseMatrix const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseMatrix const& row) SYMBOL_VISIBLE;

	GENPYBIND_MANUAL({
		auto to_numpy_template = [parent](auto const& self) {
			typedef typename std::remove_reference<typename std::remove_cv<decltype(self)>::type>::
			    type::value_type::value_type::value_type value_type;
			auto ret =
			    pybind11::array_t<value_type>({::halco::hicann_dls::vx::SynapseOnSynapseRow::size,
			                                   ::halco::hicann_dls::vx::SynapseRowOnSynram::size});
			auto access = ret.template mutable_unchecked<2>();
			for (auto row :
			     ::halco::common::iter_all<::halco::hicann_dls::vx::SynapseRowOnSynram>()) {
				for (auto col :
				     ::halco::common::iter_all<::halco::hicann_dls::vx::SynapseOnSynapseRow>()) {
					access(static_cast<size_t>(row.toEnum()), static_cast<size_t>(col.toEnum())) =
					    self[row][col];
				}
			}
			return ret;
		};

		auto from_numpy_template = [parent](
		                               auto& self,
		                               pybind11::array_t<typename std::remove_reference<decltype(
		                                   self)>::type::value_type::value_type::value_type>
		                                   array) {
			if (array.ndim() != 2) {
				throw std::runtime_error("Number of dimensions to assign to matrix must be two.");
			}
			auto access = array.template mutable_unchecked<2>();
			if (access.shape(0) != ::halco::hicann_dls::vx::SynapseRowOnSynram::size ||
			    access.shape(1) != ::halco::hicann_dls::vx::SynapseOnSynapseRow::size) {
				throw std::runtime_error("Input shape does not match.");
			}
			for (auto row :
			     ::halco::common::iter_all<::halco::hicann_dls::vx::SynapseRowOnSynram>()) {
				for (auto col :
				     ::halco::common::iter_all<::halco::hicann_dls::vx::SynapseOnSynapseRow>()) {
					self[row][col] = typename std::remove_reference<decltype(self)>::type::
					    value_type::value_type(access(
					        static_cast<size_t>(row.toEnum()), static_cast<size_t>(col.toEnum())));
				}
			}
		};

		{
			auto attr = parent.attr("_weights_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseMatrix::_weights_type _values_type;
			attr.attr("to_numpy") = parent->py::cpp_function(
			    [to_numpy_template](_values_type const& self) { return to_numpy_template(self); },
			    ism);
			attr.attr("from_numpy") = parent->py::cpp_function(
			    [from_numpy_template](
			        _values_type& self,
			        pybind11::array_t<_values_type::value_type::value_type::value_type> array) {
				    from_numpy_template(self, array);
			    },
			    ism);
		}
		{
			auto attr = parent.attr("_addresses_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseMatrix::_addresses_type _values_type;
			attr.attr("to_numpy") = parent->py::cpp_function(
			    [to_numpy_template](_values_type const& self) { return to_numpy_template(self); },
			    ism);
			attr.attr("from_numpy") = parent->py::cpp_function(
			    [from_numpy_template](
			        _values_type& self,
			        pybind11::array_t<_values_type::value_type::value_type::value_type> array) {
				    from_numpy_template(self, array);
			    },
			    ism);
		}
		{
			auto attr = parent.attr("_time_calibs_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseMatrix::_time_calibs_type _values_type;
			attr.attr("to_numpy") = parent->py::cpp_function(
			    [to_numpy_template](_values_type const& self) { return to_numpy_template(self); },
			    ism);
			attr.attr("from_numpy") = parent->py::cpp_function(
			    [from_numpy_template](
			        _values_type& self,
			        pybind11::array_t<_values_type::value_type::value_type::value_type> array) {
				    from_numpy_template(self, array);
			    },
			    ism);
		}
		{
			auto attr = parent.attr("_amp_calibs_type");
			auto ism = parent->py::is_method(attr);

			typedef ::lola::vx::SynapseMatrix::_amp_calibs_type _values_type;
			attr.attr("to_numpy") = parent->py::cpp_function(
			    [to_numpy_template](_values_type const& self) { return to_numpy_template(self); },
			    ism);
			attr.attr("from_numpy") = parent->py::cpp_function(
			    [from_numpy_template](
			        _values_type& self,
			        pybind11::array_t<_values_type::value_type::value_type::value_type> array) {
				    from_numpy_template(self, array);
			    },
			    ism);
		}
	})

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseMatrix>;
};

/**
 * Reset correlation capacitors in all quads of a given row on synram.
 * Using this container is equivalent to writing haldls CorrelationReset containers
 * to all quads of the row.
 */
class GENPYBIND(visible) CorrelationResetRow
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::CorrelationResetRowOnDLS coordinate_type;

	/** Default constructor */
	CorrelationResetRow() SYMBOL_VISIBLE;

	bool operator==(CorrelationResetRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(CorrelationResetRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CorrelationResetRow const& row)
	    SYMBOL_VISIBLE;

private:
	friend class haldls::vx::detail::VisitPreorderImpl<CorrelationResetRow>;
	friend class cereal::access;

	template <typename Archive>
	void serialize(Archive& ar);
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::SynapseRow>
    : public BackendContainerBase<
          lola::vx::SynapseRow,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config, lola::vx::SynapseRow::coordinate_type const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				auto& syn_config = quad_config.m_synapses[syn];
				syn_config.m_weight = config.weights[syn_on_row];
				syn_config.m_address = config.addresses[syn_on_row];
				syn_config.m_time_calib = config.time_calibs[syn_on_row];
				syn_config.m_amp_calib = config.amp_calibs[syn_on_row];
			}
			visit_preorder(
			    quad_config,
			    SynapseQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, lola::vx::SynapseRow const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					auto const& syn_config = quad_config.m_synapses[syn];
					config.weights[syn_on_row] = syn_config.m_weight;
					config.addresses[syn_on_row] = syn_config.m_address;
					config.time_calibs[syn_on_row] = syn_config.m_time_calib;
					config.amp_calibs[syn_on_row] = syn_config.m_amp_calib;
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::SynapseMatrix>
    : public BackendContainerBase<
          lola::vx::SynapseMatrix,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseMatrix>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::SynapseMatrix::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::SynapseQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					auto& syn_config = quad_config.m_synapses[syn];
					syn_config.m_weight = config.weights[row][syn_on_row];
					syn_config.m_address = config.addresses[row][syn_on_row];
					syn_config.m_time_calib = config.time_calibs[row][syn_on_row];
					syn_config.m_amp_calib = config.amp_calibs[row][syn_on_row];
				}
				visit_preorder(
				    quad_config, SynapseQuadOnDLS(SynapseQuadOnSynram(quad, row), coord), visitor);
				// only on alteration
				if constexpr (!std::is_same<ContainerT, lola::vx::SynapseMatrix const>::value) {
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow const syn_on_row(syn, quad);
						auto const& syn_config = quad_config.m_synapses[syn];
						config.weights[row][syn_on_row] = syn_config.m_weight;
						config.addresses[row][syn_on_row] = syn_config.m_address;
						config.time_calibs[row][syn_on_row] = syn_config.m_time_calib;
						config.amp_calibs[row][syn_on_row] = syn_config.m_amp_calib;
					}
				}
			}
		}
	}
};

template <>
struct BackendContainerTrait<lola::vx::CorrelationResetRow>
    : public BackendContainerBase<
          lola::vx::CorrelationResetRow,
          fisch::vx::OmnibusChip,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::CorrelationResetRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::CorrelationResetRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);
		CorrelationReset reset_container;

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			SynapseQuadOnSynram const quad_on_synram(quad, coord.toSynapseRowOnSynram());
			CorrelationResetOnDLS const reset_coord(quad_on_synram, coord.toSynramOnDLS());

			visit_preorder(reset_container, reset_coord, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseRow, weights, addresses, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseMatrix, weights, addresses, time_calibs, amp_calibs);
