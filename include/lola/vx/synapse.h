#pragma once
#include "halco/common/iter_all.h"
#include "halco/common/typed_array.h"
#include "halco/common/typed_heap_array.h"
#include "haldls/vx/common.h"
#include "haldls/vx/container.h"
#include "haldls/vx/synapse.h"
#include "haldls/vx/traits.h"
#include "hate/visibility.h"
#include "lola/vx/genpybind.h"
#include <boost/hana/adapt_struct.hpp>

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include <pybind11/numpy.h>
#endif

namespace lola::vx GENPYBIND_TAG_LOLA_VX {

class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ColumnCorrelationRow
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<ColumnCorrelationRow>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::ColumnCorrelationRowOnDLS coordinate_type;

	typedef haldls::vx::ColumnCorrelationQuad::ColumnCorrelationSwitch Value
	    GENPYBIND(inline_base("*ContainerBase*"));

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    Values GENPYBIND(opaque(false));

	ColumnCorrelationRow() SYMBOL_VISIBLE;

	Values values;

	bool operator==(ColumnCorrelationRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(ColumnCorrelationRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ColumnCorrelationRow const& row)
	    SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<ColumnCorrelationRow>;
};


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) ColumnCurrentRow
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<ColumnCurrentRow>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::ColumnCurrentRowOnDLS coordinate_type;

	typedef haldls::vx::ColumnCurrentQuad::ColumnCurrentSwitch Value
	    GENPYBIND(inline_base("*ContainerBase*"));

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    Values GENPYBIND(opaque(false));

	ColumnCurrentRow() SYMBOL_VISIBLE;

	Values values;

	bool operator==(ColumnCurrentRow const& other) const SYMBOL_VISIBLE;
	bool operator!=(ColumnCurrentRow const& other) const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, ColumnCurrentRow const& row) SYMBOL_VISIBLE;

private:
	friend struct haldls::vx::detail::VisitPreorderImpl<ColumnCurrentRow>;
};


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseWeightRow
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseWeightRow>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseWeightRowOnDLS coordinate_type;

	typedef haldls::vx::SynapseWeightQuad::Value Value GENPYBIND(inline_base("*ContainerBase*"));

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::SynapseOnSynapseRow>
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


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseLabelRow
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseLabelRow>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseLabelRowOnDLS coordinate_type;

	typedef haldls::vx::SynapseLabelQuad::Value Value GENPYBIND(inline_base("*ContainerBase*"));

	typedef halco::common::typed_heap_array<Value, halco::hicann_dls::vx::SynapseOnSynapseRow>
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


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseCorrelationCalibRow
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseCorrelationCalibRow>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseCorrelationCalibRowOnDLS coordinate_type;

	typedef haldls::vx::SynapseCorrelationCalibQuad::TimeCalib TimeCalib
	    GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseCorrelationCalibQuad::AmpCalib AmpCalib
	    GENPYBIND(inline_base("*ContainerBase*"));

	typedef halco::common::typed_heap_array<TimeCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _time_calibs_type GENPYBIND(opaque(false));
	typedef halco::common::typed_heap_array<AmpCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
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


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseRow
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseRow>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseRowOnDLS coordinate_type;

	typedef haldls::vx::SynapseQuad::Weight Weight GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseQuad::Label Label GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseQuad::TimeCalib TimeCalib GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseQuad::AmpCalib AmpCalib GENPYBIND(inline_base("*ContainerBase*"));

	typedef halco::common::typed_heap_array<Weight, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _weights_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<Label, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _labels_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<TimeCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
	    _time_calibs_type GENPYBIND(opaque);
	typedef halco::common::typed_heap_array<AmpCalib, halco::hicann_dls::vx::SynapseOnSynapseRow>
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


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseWeightMatrix
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseWeightMatrix>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseWeightMatrixOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::SynapseRowOnSynram>;

	typedef haldls::vx::SynapseWeightQuad::Value Value GENPYBIND(inline_base("*ContainerBase*"));

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


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseLabelMatrix
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseLabelMatrix>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseLabelMatrixOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::SynapseRowOnSynram>;

	typedef haldls::vx::SynapseLabelQuad::Value Value GENPYBIND(inline_base("*ContainerBase*"));

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


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseCorrelationCalibMatrix
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseCorrelationCalibMatrix>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynapseCorrelationCalibMatrixOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::SynapseRowOnSynram>;

	typedef haldls::vx::SynapseCorrelationCalibQuad::TimeCalib TimeCalib
	    GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseCorrelationCalibQuad::AmpCalib AmpCalib
	    GENPYBIND(inline_base("*ContainerBase*"));

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


class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) SynapseMatrix
    : public haldls::vx::DifferentialWriteTrait
    , public haldls::vx::ContainerBase<SynapseMatrix>
{
public:
	typedef std::false_type has_local_data;
	typedef halco::hicann_dls::vx::SynramOnDLS coordinate_type;

	template <typename T>
	using row_type = halco::common::typed_array<T, halco::hicann_dls::vx::SynapseOnSynapseRow>;

	template <typename T>
	using matrix_type =
	    halco::common::typed_heap_array<row_type<T>, halco::hicann_dls::vx::SynapseRowOnSynram>;

	typedef haldls::vx::SynapseQuad::Weight Weight GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseQuad::Label Label GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseQuad::TimeCalib TimeCalib GENPYBIND(inline_base("*ContainerBase*"));
	typedef haldls::vx::SynapseQuad::AmpCalib AmpCalib GENPYBIND(inline_base("*ContainerBase*"));

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
class SYMBOL_VISIBLE GENPYBIND(inline_base("*ContainerBase*")) CorrelationResetRow
    : public haldls::vx::ContainerBase<CorrelationResetRow>
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
	friend struct haldls::vx::detail::VisitPreorderImpl<CorrelationResetRow>;
};

} // namespace lola::vx

namespace haldls::vx::detail {

template <>
struct BackendContainerTrait<lola::vx::ColumnCorrelationRow>
    : public BackendContainerBase<
          lola::vx::ColumnCorrelationRow,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::ColumnCorrelationRow>
{
	typedef lola::vx::ColumnCorrelationRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			hate::Empty<haldls::vx::ColumnCorrelationQuad> quad_config;
			visit_preorder(
			    quad_config,
			    ColumnCorrelationQuadOnDLS(
			        quad.toColumnCorrelationQuadOnSynram(), coord.toSynramOnDLS()),
			    visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::ColumnCorrelationQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_switches[syn] = config.values[syn_on_row];
			}
			hate::Empty<ColumnCorrelationQuadOnDLS> quad_coord;
			visit_preorder(quad_config, quad_coord, visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_switches[syn];
				}
			}
		}
	}
	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::ColumnCorrelationQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_switches[syn] = config.values[syn_on_row];
			}
			visit_preorder(
			    quad_config,
			    ColumnCorrelationQuadOnDLS(
			        quad.toColumnCorrelationQuadOnSynram(), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_switches[syn];
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::ColumnCurrentRow>
    : public BackendContainerBase<
          lola::vx::ColumnCurrentRow,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::ColumnCurrentRow>
{
	typedef lola::vx::ColumnCurrentRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			hate::Empty<haldls::vx::ColumnCurrentQuad> quad_config;
			visit_preorder(
			    quad_config,
			    ColumnCurrentQuadOnDLS(quad.toColumnCurrentQuadOnSynram(), coord.toSynramOnDLS()),
			    visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::ColumnCurrentQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_switches[syn] = config.values[syn_on_row];
			}
			hate::Empty<ColumnCurrentQuadOnDLS> quad_coord;
			visit_preorder(quad_config, quad_coord, visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_switches[syn];
				}
			}
		}
	}
	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::ColumnCurrentQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_switches[syn] = config.values[syn_on_row];
			}
			visit_preorder(
			    quad_config,
			    ColumnCurrentQuadOnDLS(quad.toColumnCurrentQuadOnSynram(), coord.toSynramOnDLS()),
			    visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_switches[syn];
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::SynapseWeightRow>
    : public BackendContainerBase<
          lola::vx::SynapseWeightRow,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseWeightRow>
{
	typedef lola::vx::SynapseWeightRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			hate::Empty<haldls::vx::SynapseWeightQuad> quad_config;
			visit_preorder(
			    quad_config,
			    SynapseWeightQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseWeightQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_values[syn] = config.values[syn_on_row];
			}
			hate::Empty<SynapseWeightQuadOnDLS> quad_coord;
			visit_preorder(quad_config, quad_coord, visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_values[syn];
				}
			}
		}
	}
	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseWeightQuad quad_config;
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
struct BackendContainerTrait<lola::vx::SynapseLabelRow>
    : public BackendContainerBase<
          lola::vx::SynapseLabelRow,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseLabelRow>
{
	typedef lola::vx::SynapseLabelRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			hate::Empty<haldls::vx::SynapseLabelQuad> quad_config;
			visit_preorder(
			    quad_config,
			    SynapseLabelQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseLabelQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_values[syn] = config.values[syn_on_row];
			}
			hate::Empty<SynapseLabelQuadOnDLS> quad_coord;
			visit_preorder(quad_config, quad_coord, visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, container_type const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.values[syn_on_row] = quad_config.m_values[syn];
				}
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseLabelQuad quad_config;
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
struct BackendContainerTrait<lola::vx::SynapseCorrelationCalibRow>
    : public BackendContainerBase<
          lola::vx::SynapseCorrelationCalibRow,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseCorrelationCalibRow>
{
	typedef lola::vx::SynapseCorrelationCalibRow container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			hate::Empty<haldls::vx::SynapseCorrelationCalibQuad> quad_config;
			visit_preorder(
			    quad_config,
			    SynapseCorrelationCalibQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseCorrelationCalibQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_time_calibs[syn] = config.time_calibs[syn_on_row];
				quad_config.m_amp_calibs[syn] = config.amp_calibs[syn_on_row];
			}
			hate::Empty<SynapseCorrelationCalibQuadOnDLS> quad_coord;
			visit_preorder(quad_config, quad_coord, visitor);
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

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseCorrelationCalibQuad quad_config;
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
struct BackendContainerTrait<lola::vx::SynapseRow>
    : public BackendContainerBase<
          lola::vx::SynapseRow,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseRow>
{
	typedef lola::vx::SynapseRow container_type;

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseQuad quad_config;
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

	template <typename VisitorT>
	static void call(
	    hate::Empty<lola::vx::SynapseRow> config,
	    lola::vx::SynapseRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			hate::Empty<haldls::vx::SynapseQuad> quad_config;
			visit_preorder(
			    quad_config,
			    SynapseQuadOnDLS(
			        SynapseQuadOnSynram(quad, coord.toSynapseRowOnSynram()), coord.toSynramOnDLS()),
			    visitor);
		}
	}
	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::SynapseRow::coordinate_type> coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			haldls::vx::SynapseQuad quad_config;
			for (auto const syn : iter_all<EntryOnQuad>()) {
				SynapseOnSynapseRow const syn_on_row(syn, quad);
				quad_config.m_weights[syn] = config.weights[syn_on_row];
				quad_config.m_labels[syn] = config.labels[syn_on_row];
				quad_config.m_time_calibs[syn] = config.time_calibs[syn_on_row];
				quad_config.m_amp_calibs[syn] = config.amp_calibs[syn_on_row];
			}
			hate::Empty<SynapseQuadOnDLS> quad_coord;
			visit_preorder(quad_config, quad_coord, visitor);
			// only on alteration
			if constexpr (!std::is_same<ContainerT, lola::vx::SynapseRow const>::value) {
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					config.weights[syn_on_row] = quad_config.m_weights[syn];
					config.labels[syn_on_row] = quad_config.m_labels[syn];
					config.time_calibs[syn_on_row] = quad_config.m_time_calibs[syn];
					config.amp_calibs[syn_on_row] = quad_config.m_amp_calibs[syn];
				}
			}
		}
	}
};


template <>
struct BackendContainerTrait<lola::vx::SynapseWeightMatrix>
    : public BackendContainerBase<
          lola::vx::SynapseWeightMatrix,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseWeightMatrix>
{
	typedef lola::vx::SynapseWeightMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				hate::Empty<haldls::vx::SynapseWeightQuad> quad_config;
				visit_preorder(
				    quad_config,
				    SynapseWeightQuadOnDLS(SynapseQuadOnSynram(quad, row), coord.toSynramOnDLS()),
				    visitor);
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::SynapseWeightQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					quad_config.m_values[syn] = config.values[row][syn_on_row];
				}
				hate::Empty<SynapseWeightQuadOnDLS> quad_coord;
				visit_preorder(quad_config, quad_coord, visitor);
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

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::SynapseWeightQuad quad_config;
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
struct BackendContainerTrait<lola::vx::SynapseLabelMatrix>
    : public BackendContainerBase<
          lola::vx::SynapseLabelMatrix,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseLabelMatrix>
{
	typedef lola::vx::SynapseLabelMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				hate::Empty<haldls::vx::SynapseLabelQuad> quad_config;
				visit_preorder(
				    quad_config,
				    SynapseLabelQuadOnDLS(SynapseQuadOnSynram(quad, row), coord.toSynramOnDLS()),
				    visitor);
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::SynapseLabelQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					quad_config.m_values[syn] = config.values[row][syn_on_row];
				}
				hate::Empty<SynapseLabelQuadOnDLS> quad_coord;
				visit_preorder(quad_config, quad_coord, visitor);
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

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::SynapseLabelQuad quad_config;
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
struct BackendContainerTrait<lola::vx::SynapseCorrelationCalibMatrix>
    : public BackendContainerBase<
          lola::vx::SynapseCorrelationCalibMatrix,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseCorrelationCalibMatrix>
{
	typedef lola::vx::SynapseCorrelationCalibMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				hate::Empty<haldls::vx::SynapseCorrelationCalibQuad> quad_config;
				visit_preorder(
				    quad_config,
				    SynapseCorrelationCalibQuadOnDLS(
				        SynapseQuadOnSynram(quad, row), coord.toSynramOnDLS()),
				    visitor);
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::SynapseCorrelationCalibQuad quad_config;
				for (auto const syn : iter_all<EntryOnQuad>()) {
					SynapseOnSynapseRow const syn_on_row(syn, quad);
					quad_config.m_time_calibs[syn] = config.time_calibs[row][syn_on_row];
					quad_config.m_amp_calibs[syn] = config.amp_calibs[row][syn_on_row];
				}
				hate::Empty<SynapseCorrelationCalibQuadOnDLS> quad_coord;
				visit_preorder(quad_config, quad_coord, visitor);
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

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				haldls::vx::SynapseCorrelationCalibQuad quad_config;
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
struct BackendContainerTrait<lola::vx::SynapseMatrix>
    : public BackendContainerBase<
          lola::vx::SynapseMatrix,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::SynapseMatrix>
{
	typedef lola::vx::SynapseMatrix container_type;

	template <typename ContainerT, typename VisitorT>
	static void call(
	    hate::Empty<ContainerT> const& config,
	    typename container_type::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);

		for (auto const row : iter_all<SynapseRowOnSynram>()) {
			for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
				hate::Empty<haldls::vx::SynapseQuad> quad_config;
				visit_preorder(
				    quad_config, SynapseQuadOnDLS(SynapseQuadOnSynram(quad, row), coord), visitor);
			}
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<typename container_type::coordinate_type> const& coord,
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
					quad_config.m_weights.at(syn) = config.weights[row][syn_on_row];
					quad_config.m_labels.at(syn) = config.labels[row][syn_on_row];
					quad_config.m_time_calibs.at(syn) = config.time_calibs[row][syn_on_row];
					quad_config.m_amp_calibs.at(syn) = config.amp_calibs[row][syn_on_row];
				}
				visit_preorder(quad_config, hate::Empty<SynapseQuadOnDLS>{}, visitor);
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

	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    typename container_type::coordinate_type const& coord,
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
struct BackendContainerTrait<lola::vx::CorrelationResetRow>
    : public BackendContainerBase<
          lola::vx::CorrelationResetRow,
          fisch::vx::word_access_type::Omnibus,
          fisch::vx::word_access_type::OmnibusChipOverJTAG>
{};

template <>
struct VisitPreorderImpl<lola::vx::CorrelationResetRow>
{
	template <typename ContainerT, typename VisitorT>
	static std::enable_if_t<!hate::is_empty_v<ContainerT>> call(
	    ContainerT& config,
	    lola::vx::CorrelationResetRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);
		haldls::vx::CorrelationReset reset_container;

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			SynapseQuadOnSynram const quad_on_synram(quad, coord.toSynapseRowOnSynram());
			CorrelationResetOnDLS const reset_coord(quad_on_synram, coord.toSynramOnDLS());

			visit_preorder(reset_container, reset_coord, visitor);
		}
	}

	template <typename VisitorT>
	static void call(
	    hate::Empty<lola::vx::CorrelationResetRow> config,
	    lola::vx::CorrelationResetRow::coordinate_type const& coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);
		hate::Empty<CorrelationReset> reset_container;

		for (auto const quad : iter_all<SynapseQuadColumnOnDLS>()) {
			SynapseQuadOnSynram const quad_on_synram(quad, coord.toSynapseRowOnSynram());
			CorrelationResetOnDLS const reset_coord(quad_on_synram, coord.toSynramOnDLS());

			visit_preorder(reset_container, reset_coord, visitor);
		}
	}

	template <typename ContainerT, typename VisitorT>
	static void call(
	    ContainerT& config,
	    hate::Empty<lola::vx::CorrelationResetRow::coordinate_type> coord,
	    VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::vx;

		visitor(coord, config);
		CorrelationReset reset_container;

		for ([[maybe_unused]] auto const& _ : iter_all<SynapseQuadColumnOnDLS>()) {
			hate::Empty<CorrelationResetOnDLS> const reset_coord;
			visit_preorder(reset_container, reset_coord, visitor);
		}
	}
};

} // namespace haldls::vx::detail

BOOST_HANA_ADAPT_STRUCT(lola::vx::ColumnCorrelationRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::ColumnCurrentRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseRow, weights, labels, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseWeightRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseLabelRow, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseCorrelationCalibRow, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseWeightMatrix, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseLabelMatrix, values);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseCorrelationCalibMatrix, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::SynapseMatrix, weights, labels, time_calibs, amp_calibs);
BOOST_HANA_ADAPT_STRUCT(lola::vx::CorrelationResetRow);
