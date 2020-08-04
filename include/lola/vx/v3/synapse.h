#pragma once
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/common/typed_heap_array.h"
#include "haldls/cerealization.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "haldls/vx/v3/synapse.h"
#include "hate/visibility.h"
#include "lola/vx/cerealization.h"
#include "lola/vx/genpybind.h"
#include <boost/hana/adapt_struct.hpp>

#include <pybind11/numpy.h>

namespace lola::vx::v3 GENPYBIND_TAG_LOLA_VX_V3 {

class GENPYBIND(visible) SynapseWeightRow : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynapseWeightRowOnDLS coordinate_type;

	typedef haldls::vx::v3::SynapseWeightQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	    _values_type GENPYBIND(opaque(false));

	/** Default constructor. */
	SynapseWeightRow() SYMBOL_VISIBLE;

	/** Synapse weight array. */
	_values_type values;

	bool operator==(SynapseWeightRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseWeightRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseWeightRow const& row) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseWeightRow>;
};


class GENPYBIND(visible) SynapseLabelRow : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynapseLabelRowOnDLS coordinate_type;

	typedef haldls::vx::v3::SynapseLabelQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	    _values_type GENPYBIND(opaque(false));

	/** Default constructor. */
	SynapseLabelRow() SYMBOL_VISIBLE;

	/** Synapse label array. */
	_values_type values;

	bool operator==(SynapseLabelRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseLabelRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseLabelRow const& row) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseLabelRow>;
};


class GENPYBIND(visible) SynapseCorrelationCalibRow : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynapseCorrelationCalibRowOnDLS coordinate_type;

	typedef haldls::vx::v3::SynapseCorrelationCalibQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseCorrelationCalibQuad::AmpCalib AmpCalib GENPYBIND(visible);

	typedef halco::common::
	    typed_heap_array<TimeCalib, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	        _time_calibs_type GENPYBIND(opaque(false));
	typedef halco::common::
	    typed_heap_array<AmpCalib, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	        _amp_calibs_type GENPYBIND(opaque(false));


	/** Default constructor. */
	SynapseCorrelationCalibRow() SYMBOL_VISIBLE;

	/** Synapse correlation time calibration array. */
	_time_calibs_type time_calibs;

	/** Synapse correlation amplitude calibration array. */
	_amp_calibs_type amp_calibs;


	bool operator==(SynapseCorrelationCalibRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseCorrelationCalibRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseCorrelationCalibRow const& row)
	    SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseCorrelationCalibRow>;
};


class GENPYBIND(visible) SynapseRow : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynapseRowOnDLS coordinate_type;

	typedef haldls::vx::v3::SynapseQuad::Weight Weight GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseQuad::Label Label GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseQuad::AmpCalib AmpCalib GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Weight, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	    _weights_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<Label, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	    _labels_type GENPYBIND(opaque);
	typedef halco::common::
	    typed_heap_array<TimeCalib, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	        _time_calibs_type GENPYBIND(opaque);
	typedef halco::common::
	    typed_heap_array<AmpCalib, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>
	        _amp_calibs_type GENPYBIND(opaque);

	/** Default constructor. */
	SynapseRow() SYMBOL_VISIBLE;

	/** Synapse weight array. */
	_weights_type weights;

	/** Synapse label array. */
	_labels_type labels;

	/** Synapse correlation time calibration array. */
	_time_calibs_type time_calibs;

	/** Synapse correlation amplitude calibration array. */
	_amp_calibs_type amp_calibs;

	bool operator==(SynapseRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseRow const& row) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseRow>;
};


class GENPYBIND(visible) SynapseWeightMatrix : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynapseWeightMatrixOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::v3::SynapseRowOnSynram>;

	typedef haldls::vx::v3::SynapseWeightQuad::Value Value GENPYBIND(visible);

	typedef row_type<Value> _row_type GENPYBIND(opaque(false));
	typedef matrix_type<Value> _values_type GENPYBIND(opaque(false));

	/** Default constructor. */
	SynapseWeightMatrix() SYMBOL_VISIBLE;

	/** Synapse weight array. */
	_values_type values;

	bool operator==(SynapseWeightMatrix const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseWeightMatrix const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseWeightMatrix const& row)
	    SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseWeightMatrix>;
};


class GENPYBIND(visible) SynapseLabelMatrix : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynapseLabelMatrixOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::v3::SynapseRowOnSynram>;

	typedef haldls::vx::v3::SynapseLabelQuad::Value Value GENPYBIND(visible);

	typedef row_type<Value> _row_type GENPYBIND(opaque(false));
	typedef matrix_type<Value> _values_type GENPYBIND(opaque(false));

	/** Default constructor. */
	SynapseLabelMatrix() SYMBOL_VISIBLE;

	/** Synapse label array. */
	_values_type values;

	bool operator==(SynapseLabelMatrix const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseLabelMatrix const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseLabelMatrix const& row) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseLabelMatrix>;
};


class GENPYBIND(visible) SynapseCorrelationCalibMatrix : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynapseCorrelationCalibMatrixOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::v3::SynapseRowOnSynram>;

	typedef haldls::vx::v3::SynapseCorrelationCalibQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseCorrelationCalibQuad::AmpCalib AmpCalib GENPYBIND(visible);

	typedef row_type<TimeCalib> _time_calibs_row_type GENPYBIND(opaque(false));
	typedef row_type<AmpCalib> _amp_calibs_row_type GENPYBIND(opaque(false));
	typedef matrix_type<TimeCalib> _time_calibs_type GENPYBIND(opaque(false));
	typedef matrix_type<AmpCalib> _amp_calibs_type GENPYBIND(opaque(false));

	/** Default constructor. */
	SynapseCorrelationCalibMatrix() SYMBOL_VISIBLE;

	/** Synapse correlation time calibration array. */
	_time_calibs_type time_calibs;

	/** Synapse correlation amplitude calibration array. */
	_amp_calibs_type amp_calibs;


	bool operator==(SynapseCorrelationCalibMatrix const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseCorrelationCalibMatrix const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseCorrelationCalibMatrix const& row)
	    SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<SynapseCorrelationCalibMatrix>;
};


class GENPYBIND(visible) SynapseMatrix : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v3::SynramOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::v3::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::v3::SynapseRowOnSynram>;

	typedef haldls::vx::v3::SynapseQuad::Weight Weight GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseQuad::Label Label GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v3::SynapseQuad::AmpCalib AmpCalib GENPYBIND(visible);

	typedef row_type<Weight> _weights_row_type GENPYBIND(opaque);
	typedef row_type<Label> _labels_row_type GENPYBIND(opaque);
	typedef row_type<TimeCalib> _time_calibs_row_type GENPYBIND(opaque);
	typedef row_type<AmpCalib> _amp_calibs_row_type GENPYBIND(opaque);

	typedef matrix_type<Weight> _weights_type GENPYBIND(opaque);
	typedef matrix_type<Label> _labels_type GENPYBIND(opaque);
	typedef matrix_type<TimeCalib> _time_calibs_type GENPYBIND(opaque);
	typedef matrix_type<AmpCalib> _amp_calibs_type GENPYBIND(opaque);

	/** Default constructor. */
	SynapseMatrix() SYMBOL_VISIBLE;

	/** Synapse weight array. */
	_weights_type weights;

	/** Synapse label array. */
	_labels_type labels;

	/** Synapse correlation time calibration array. */
	_time_calibs_type time_calibs;

	/** Synapse correlation amplitude calibration array. */
	_amp_calibs_type amp_calibs;

	bool operator==(SynapseMatrix const& other) const SYMBOL_VISIBLE;
	bool operator!=(SynapseMatrix const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, SynapseMatrix const& row) SYMBOL_VISIBLE;

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
	typedef halco::hicann_dls::vx::v3::CorrelationResetRowOnDLS coordinate_type;

	/** Default constructor */
	CorrelationResetRow() SYMBOL_VISIBLE;

	bool operator==(CorrelationResetRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(CorrelationResetRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, CorrelationResetRow const& row)
	    SYMBOL_VISIBLE;

private:
	friend class haldls::vx::detail::VisitPreorderImpl<CorrelationResetRow>;
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::v3::SynapseWeightRow>
    : public BackendContainerBase<
          lola::vx::v3::SynapseWeightRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseWeightRow>
{
	typedef lola::vx::v3::SynapseWeightRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v3::SynapseWeightQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_values[syn] = config.values[syn_on_row];
			}
			visit_preorder(
			    quad_config,
			    SynapseWeightQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_values[syn];
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::v3::SynapseLabelRow>
    : public BackendContainerBase<
          lola::vx::v3::SynapseLabelRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseLabelRow>
{
	typedef lola::vx::v3::SynapseLabelRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v3::SynapseLabelQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_values[syn] = config.values[syn_on_row];
			}
			visit_preorder(
			    quad_config,
			    SynapseLabelQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_values[syn];
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::v3::SynapseCorrelationCalibRow>
    : public BackendContainerBase<
          lola::vx::v3::SynapseCorrelationCalibRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseCorrelationCalibRow>
{
	typedef lola::vx::v3::SynapseCorrelationCalibRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v3::SynapseCorrelationCalibQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_time_calibs[syn] = config.time_calibs[syn_on_row];
				quad_config.m_amp_calibs[syn] = config.amp_calibs[syn_on_row];
			}
			visit_preorder(
			    quad_config,
			    SynapseCorrelationCalibQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.time_calibs[syn_on_row] = quad_config.m_time_calibs[syn];
					config.amp_calibs[syn_on_row] = quad_config.m_amp_calibs[syn];
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::v3::SynapseRow>
    : public BackendContainerBase<
          lola::vx::v3::SynapseRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseRow>
{
	typedef lola::vx::v3::SynapseRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v3::SynapseQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_weights.at(syn) = config.weights[syn_on_row];
				quad_config.m_labels.at(syn) = config.labels[syn_on_row];
				quad_config.m_time_calibs.at(syn) = config.time_calibs[syn_on_row];
				quad_config.m_amp_calibs.at(syn) = config.amp_calibs[syn_on_row];
			}
			visit_preorder(
			    quad_config,
			    SynapseQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.weights[syn_on_row] = quad_config.m_weights.at(syn);
					config.labels[syn_on_row] = quad_config.m_labels.at(syn);
					config.time_calibs[syn_on_row] = quad_config.m_time_calibs.at(syn);
					config.amp_calibs[syn_on_row] = quad_config.m_amp_calibs.at(syn);
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::v3::SynapseWeightMatrix>
    : public BackendContainerBase<
          lola::vx::v3::SynapseWeightMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseWeightMatrix>
{
	typedef lola::vx::v3::SynapseWeightMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v3::SynapseWeightQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					quad_config.m_values[syn] = config.values[row][syn_on_row];
				}
				visit_preorder(
				    quad_config,
				    SynapseWeightQuadOnDLS(SynapseQuadOnSynram(quad, row), coord.toSynramOnDLS()),
				    visitor);
				// only on alteration
				if constexpr (!std::is_same<ContainerT, container_type const>::value) {
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow const syn_on_row(syn, quad);
						config.values[row][syn_on_row] = quad_config.m_values[syn];
					}
				}
			}
		}
	}
};

template <>
struct BackendContainerTrait<lola::vx::v3::SynapseLabelMatrix>
    : public BackendContainerBase<
          lola::vx::v3::SynapseLabelMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseLabelMatrix>
{
	typedef lola::vx::v3::SynapseLabelMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v3::SynapseLabelQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					quad_config.m_values[syn] = config.values[row][syn_on_row];
				}
				visit_preorder(
				    quad_config,
				    SynapseLabelQuadOnDLS(SynapseQuadOnSynram(quad, row), coord.toSynramOnDLS()),
				    visitor);
				// only on alteration
				if constexpr (!std::is_same<ContainerT, container_type const>::value) {
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow const syn_on_row(syn, quad);
						config.values[row][syn_on_row] = quad_config.m_values[syn];
					}
				}
			}
		}
	}
};

template <>
struct BackendContainerTrait<lola::vx::v3::SynapseCorrelationCalibMatrix>
    : public BackendContainerBase<
          lola::vx::v3::SynapseCorrelationCalibMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseCorrelationCalibMatrix>
{
	typedef lola::vx::v3::SynapseCorrelationCalibMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v3::SynapseCorrelationCalibQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					quad_config.m_time_calibs[syn] = config.time_calibs[row][syn_on_row];
					quad_config.m_amp_calibs[syn] = config.amp_calibs[row][syn_on_row];
				}
				visit_preorder(
				    quad_config,
				    SynapseCorrelationCalibQuadOnDLS(
				        SynapseQuadOnSynram(quad, row), coord.toSynramOnDLS()),
				    visitor);
				// only on alteration
				if constexpr (!std::is_same<ContainerT, container_type const>::value) {
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow const syn_on_row(syn, quad);
						config.time_calibs[row][syn_on_row] = quad_config.m_time_calibs[syn];
						config.amp_calibs[row][syn_on_row] = quad_config.m_amp_calibs[syn];
					}
				}
			}
		}
	}
};

template <>
struct BackendContainerTrait<lola::vx::v3::SynapseMatrix>
    : public BackendContainerBase<
          lola::vx::v3::SynapseMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::SynapseMatrix>
{
	typedef lola::vx::v3::SynapseMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v3::SynapseQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					quad_config.m_weights.at(syn) = config.weights[row][syn_on_row];
					quad_config.m_labels.at(syn) = config.labels[row][syn_on_row];
					quad_config.m_time_calibs.at(syn) = config.time_calibs[row][syn_on_row];
					quad_config.m_amp_calibs.at(syn) = config.amp_calibs[row][syn_on_row];
				}
				visit_preorder(
				    quad_config, SynapseQuadOnDLS(SynapseQuadOnSynram(quad, row), coord), visitor);
				// only on alteration
				if constexpr (!std::is_same<ContainerT, container_type const>::value) {
					for (auto const syn : iter_all<EntryOnQuad>()) {
						SynapseOnSynapseRow const syn_on_row(syn, quad);
						config.weights[row][syn_on_row] = quad_config.m_weights.at(syn);
						config.labels[row][syn_on_row] = quad_config.m_labels.at(syn);
						config.time_calibs[row][syn_on_row] = quad_config.m_time_calibs.at(syn);
						config.amp_calibs[row][syn_on_row] = quad_config.m_amp_calibs.at(syn);
					}
				}
			}
		}
	}
};

template <>
struct BackendContainerTrait<lola::vx::v3::CorrelationResetRow>
    : public BackendContainerBase<
          lola::vx::v3::CorrelationResetRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v3::CorrelationResetRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v3::CorrelationResetRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v3;

		visitor(coord, config);
		haldls::vx::v3::CorrelationReset reset_container;

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			SynapseQuadOnSynram const quad_on_synram(quad, coord.toSynapseRowOnSynram());
			CorrelationResetOnDLS const reset_coord(quad_on_synram, coord.toSynramOnDLS());

			visit_preorder(reset_container, reset_coord, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseRow, weights, labels, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseWeightRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseLabelRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseCorrelationCalibRow, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseWeightMatrix, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseLabelMatrix, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseCorrelationCalibMatrix, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::SynapseMatrix, weights, labels, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v3::CorrelationResetRow);

EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseRow)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseWeightRow)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseLabelRow)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseCorrelationCalibRow)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseWeightMatrix)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseLabelMatrix)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseCorrelationCalibMatrix)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::SynapseMatrix)
EXTERN_INSTANTIATE_CEREAL_SERIALIZE_FREE(lola::vx::v3::CorrelationResetRow)