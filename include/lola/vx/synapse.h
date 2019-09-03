#pragma once
#include "halco/common/typed_array.h"
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

class GENPYBIND(visible) SynapseRow
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseRowOnDLS coordinate_type;

	typedef haldls::vx::SynapseQuad::Synapse::Weight Weight GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::Address Address GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::SynapseQuad::Synapse::AmpCalib AmpCalib GENPYBIND(visible);

	typedef halco::common::typed_array<Weight, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _weights_type GENPYBIND(opaque);
	typedef halco::common::typed_array<Address, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _addresses_type GENPYBIND(opaque);
	typedef halco::common::typed_array<TimeCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _time_calibs_type GENPYBIND(opaque);
	typedef halco::common::typed_array<AmpCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _amp_calibs_type GENPYBIND(opaque);

	/** Default constructor. */
	SynapseRow() SYMBOL_VISIBLE;

	/**
	 * Get synapse weight array.
	 * @return Array of haldls::vx::SynapseQuad::Synapse::Weight values
	 */
	GENPYBIND(getter_for(weights), return_value_policy(reference_internal))
	_weights_type const& get_weights() const SYMBOL_VISIBLE;

	/**
	 * Set synapse weight array.
	 * @param value Array of haldls::vx::SynapseQuad::Synapse::Weight values
	 */
	GENPYBIND(setter_for(weights))
	void set_weights(_weights_type const& value) SYMBOL_VISIBLE;

	/**
	 * Get synapse address array.
	 * @return Array of haldls::vx::SynapseQuad::Synapse::Address values
	 */
	GENPYBIND(getter_for(addresses), return_value_policy(reference_internal))
	_addresses_type const& get_addresses() const SYMBOL_VISIBLE;

	/**
	 * Set synapse address array.
	 * @param value Array of haldls::vx::SynapseQuad::Synapse::Address values
	 */
	GENPYBIND(setter_for(addresses))
	void set_addresses(_addresses_type const& value) SYMBOL_VISIBLE;

	/**
	 * Get synapse correlation time calibration array.
	 * @return Array of haldls::vx::SynapseQuad::Synapse::TimeCalib values
	 */
	GENPYBIND(getter_for(time_calibs), return_value_policy(reference_internal))
	_time_calibs_type const& get_time_calibs() const SYMBOL_VISIBLE;

	/**
	 * Set synapse correlation time calibration array.
	 * @param value Array of haldls::vx::SynapseQuad::Synapse::TimeCalib values
	 */
	GENPYBIND(setter_for(time_calibs))
	void set_time_calibs(_time_calibs_type const& value) SYMBOL_VISIBLE;

	/**
	 * Get synapse correlation amplitude calibration array.
	 * @return Array of haldls::vx::SynapseQuad::Synapse::AmpCalib values
	 */
	GENPYBIND(getter_for(amp_calibs), return_value_policy(reference_internal))
	_amp_calibs_type const& get_amp_calibs() const SYMBOL_VISIBLE;

	/**
	 * Set synapse correlation amplitude calibration array.
	 * @param value Array of haldls::vx::SynapseQuad::Synapse::AmpCalib values
	 */
	GENPYBIND(setter_for(amp_calibs))
	void set_amp_calibs(_amp_calibs_type const& value) SYMBOL_VISIBLE;

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
	friend class cereal::access;

	template <typename Archive>
	void serialize(Archive& ar);

	_weights_type m_weights;
	_addresses_type m_addresses;
	_time_calibs_type m_time_calibs;
	_amp_calibs_type m_amp_calibs;
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::SynapseRow>
    : public BackendContainerBase<
          lola::vx::SynapseRow,
          fisch::vx::OmnibusChipOverJTAG,
          fisch::vx::OmnibusChip>
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
				SynapseOnSynapseRow syn_on_row(syn, quad);
				haldls::vx::SynapseQuad::Synapse syn_config;
				syn_config.set_weight(config.m_weights[syn_on_row]);
				syn_config.set_address(config.m_addresses[syn_on_row]);
				syn_config.set_time_calib(config.m_time_calibs[syn_on_row]);
				syn_config.set_amp_calib(config.m_amp_calibs[syn_on_row]);
				quad_config.set_synapse(syn, syn_config);
			}
			visit_preorder(
			    quad_config,
			    SynapseQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, lola::vx::SynapseRow const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow syn_on_row(syn, quad);
					auto syn_config = quad_config.get_synapse(syn);
					config.m_weights[syn_on_row] = syn_config.get_weight();
					config.m_addresses[syn_on_row] = syn_config.get_address();
					config.m_time_calibs[syn_on_row] = syn_config.get_time_calib();
					config.m_amp_calibs[syn_on_row] = syn_config.get_amp_calib();
				}
			}
		}
	}
};

} // namespace haldls::vx::detail
