#pragma once
#include <boost/hana/adapt_struct.hpp>
#include "halco/common/iter_all.h"
#include "halco/common/typed_heap_array.h"
#include "haldls/vx/common.h"
#include "haldls/vx/traits.h"
#include "haldls/vx/v2/synapse.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"

#include <pybind11/numpy.h>

namespace cereal {
class access;
} // namespace cereal

namespace lola::vx::v2 GENPYBIND_TAG_LOLA_VX_V2 {

class GENPYBIND(visible) SynapseWeightRow : public haldls::vx::DifferentialWriteTrait
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::v2::SynapseWeightRowOnDLS coordinate_type;

	typedef haldls::vx::v2::SynapseWeightQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
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
	typedef halco::hicann_dls::vx::v2::SynapseLabelRowOnDLS coordinate_type;

	typedef haldls::vx::v2::SynapseLabelQuad::Value Value GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
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
	typedef halco::hicann_dls::vx::v2::SynapseCorrelationCalibRowOnDLS coordinate_type;

	typedef haldls::vx::v2::SynapseCorrelationCalibQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseCorrelationCalibQuad::AmpCalib AmpCalib GENPYBIND(visible);

	typedef halco::common::
	    typed_heap_array<TimeCalib, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
	        _time_calibs_type GENPYBIND(opaque(false));
	typedef halco::common::
	    typed_heap_array<AmpCalib, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
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
	typedef halco::hicann_dls::vx::v2::SynapseRowOnDLS coordinate_type;

	typedef haldls::vx::v2::SynapseQuad::Weight Weight GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseQuad::Label Label GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseQuad::AmpCalib AmpCalib GENPYBIND(visible);

	typedef halco::common::typed_heap_array<Weight, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
	    _weights_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<Label, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
	    _labels_type GENPYBIND(opaque);
	typedef halco::common::
	    typed_heap_array<TimeCalib, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
	        _time_calibs_type GENPYBIND(opaque);
	typedef halco::common::
	    typed_heap_array<AmpCalib, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>
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
	typedef halco::hicann_dls::vx::v2::SynapseWeightMatrixOnDLS coordinate_type;

	template <typename T>
	using matrix_type = halco::common::typed_heap_array<
	    halco::common::typed_heap_array<T, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::v2::SynapseRowOnSynram>;

	typedef haldls::vx::v2::SynapseWeightQuad::Value Value GENPYBIND(visible);

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
	typedef halco::hicann_dls::vx::v2::SynapseLabelMatrixOnDLS coordinate_type;

	template <typename T>
	using matrix_type = halco::common::typed_heap_array<
	    halco::common::typed_heap_array<T, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::v2::SynapseRowOnSynram>;

	typedef haldls::vx::v2::SynapseLabelQuad::Value Value GENPYBIND(visible);

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
	typedef halco::hicann_dls::vx::v2::SynapseCorrelationCalibMatrixOnDLS coordinate_type;

	template <typename T>
	using matrix_type = halco::common::typed_heap_array<
	    halco::common::typed_heap_array<T, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::v2::SynapseRowOnSynram>;

	typedef haldls::vx::v2::SynapseCorrelationCalibQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseCorrelationCalibQuad::AmpCalib AmpCalib GENPYBIND(visible);

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
	typedef halco::hicann_dls::vx::v2::SynramOnDLS coordinate_type;

	template <typename T>
	using matrix_type = halco::common::typed_heap_array<
	    halco::common::typed_heap_array<T, halco::hicann_dls::vx::v2::SynapseOnSynapseRow>,
	    halco::hicann_dls::vx::v2::SynapseRowOnSynram>;

	typedef haldls::vx::v2::SynapseQuad::Weight Weight GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseQuad::Label Label GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseQuad::TimeCalib TimeCalib GENPYBIND(visible);
	typedef haldls::vx::v2::SynapseQuad::AmpCalib AmpCalib GENPYBIND(visible);

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
	typedef halco::hicann_dls::vx::v2::CorrelationResetRowOnDLS coordinate_type;

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
struct BackendContainerTrait<lola::vx::v2::SynapseWeightRow>
    : public BackendContainerBase<
          lola::vx::v2::SynapseWeightRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseWeightRow>
{
	typedef lola::vx::v2::SynapseWeightRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v2::SynapseWeightQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::SynapseLabelRow>
    : public BackendContainerBase<
          lola::vx::v2::SynapseLabelRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseLabelRow>
{
	typedef lola::vx::v2::SynapseLabelRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v2::SynapseLabelQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::SynapseCorrelationCalibRow>
    : public BackendContainerBase<
          lola::vx::v2::SynapseCorrelationCalibRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseCorrelationCalibRow>
{
	typedef lola::vx::v2::SynapseCorrelationCalibRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v2::SynapseCorrelationCalibQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::SynapseRow>
    : public BackendContainerBase<
          lola::vx::v2::SynapseRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseRow>
{
	typedef lola::vx::v2::SynapseRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::v2::SynapseQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::SynapseWeightMatrix>
    : public BackendContainerBase<
          lola::vx::v2::SynapseWeightMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseWeightMatrix>
{
	typedef lola::vx::v2::SynapseWeightMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v2::SynapseWeightQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::SynapseLabelMatrix>
    : public BackendContainerBase<
          lola::vx::v2::SynapseLabelMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseLabelMatrix>
{
	typedef lola::vx::v2::SynapseLabelMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v2::SynapseLabelQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::SynapseCorrelationCalibMatrix>
    : public BackendContainerBase<
          lola::vx::v2::SynapseCorrelationCalibMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseCorrelationCalibMatrix>
{
	typedef lola::vx::v2::SynapseCorrelationCalibMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v2::SynapseCorrelationCalibQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::SynapseMatrix>
    : public BackendContainerBase<
          lola::vx::v2::SynapseMatrix,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::SynapseMatrix>
{
	typedef lola::vx::v2::SynapseMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::v2::SynapseQuad quad_config;
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
struct BackendContainerTrait<lola::vx::v2::CorrelationResetRow>
    : public BackendContainerBase<
          lola::vx::v2::CorrelationResetRow,
          fisch::vx::Omnibus,
          fisch::vx::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::v2::CorrelationResetRow>
{
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    lola::vx::v2::CorrelationResetRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx::v2;

		visitor(coord, config);
		haldls::vx::v2::CorrelationReset reset_container;

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			SynapseQuadOnSynram const quad_on_synram(quad, coord.toSynapseRowOnSynram());
			CorrelationResetOnDLS const reset_coord(quad_on_synram, coord.toSynramOnDLS());

			visit_preorder(reset_container, reset_coord, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseRow, weights, labels, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseWeightRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseLabelRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseCorrelationCalibRow, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseWeightMatrix, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseLabelMatrix, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseCorrelationCalibMatrix, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::SynapseMatrix, weights, labels, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::v2::CorrelationResetRow);
