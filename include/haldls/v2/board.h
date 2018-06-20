#pragma once

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "hate/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/dac.h"
#include "haldls/v2/fpga.h"
#include "haldls/v2/genpybind.h"
#include "haldls/v2/synapse.h"
#include "haldls/cerealization.h"

namespace haldls {
namespace v2 GENPYBIND_TAG_HALDLS_V2 {

class GENPYBIND(visible) Board {
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::false_type has_local_data;

	enum class Parameter : uint_fast16_t {
		cadc_ramp_01,
		cadc_ramp_bias,
		cadc_ramp_slope,
		cadc_v_bias,
		capmem_i_buf_bias,
		capmem_i_offset,
		capmem_i_ref,
		general_purpose_0,
		general_purpose_1,
		syn_corout_bias,
		syn_v_bias,
		syn_v_dd_res_meas,
		syn_v_ramp,
		syn_v_reset,
		syn_v_store
	};


	Board() SYMBOL_VISIBLE;

	// accessors
	void set_parameter(Parameter const& parameter, DAC::Value const& value) SYMBOL_VISIBLE;
	DAC::Value get_parameter(Parameter const& parameter) const SYMBOL_VISIBLE;

	GENPYBIND(getter_for(flypsi_config), return_value_policy(reference))
	FlyspiConfig const& get_flyspi_config() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(flypsi_config))
	void set_flyspi_config(FlyspiConfig const& config) SYMBOL_VISIBLE;

	GENPYBIND(getter_for(spike_router), return_value_policy(reference))
	SpikeRouter const& get_spike_router() const SYMBOL_VISIBLE;
	GENPYBIND(setter_for(spike_router))
	void set_spike_router(SpikeRouter const& config) SYMBOL_VISIBLE;

	// read-only accessor
	GENPYBIND(getter_for(flyspi_exception))
	FlyspiException get_flyspi_exception() const SYMBOL_VISIBLE;

	bool operator==(Board const& other) const SYMBOL_VISIBLE;
	bool operator!=(Board const& other) const SYMBOL_VISIBLE;

	friend detail::VisitPreorderImpl<Board>;

private:
	friend class cereal::access;
	template <class Archive>
	void cerealize(Archive& ar) SYMBOL_VISIBLE;

	FlyspiConfig m_flyspi_config;
	FlyspiException m_flyspi_exception;
	SpikeRouter m_spike_router;
	halco::common::typed_array<DAC, halco::hicann_dls::v2::DACOnBoard> m_dacs;
}; // Board

namespace detail {

template <>
struct VisitPreorderImpl<Board> {
	template <typename ContainerT, typename VisitorT>
	static void call(
		ContainerT& config, halco::common::Unique const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::v2;

		visitor(coord, config);

		// No std::forward for visitor argument, as we want to pass a reference to the
		// nested visitor in any case, even if it was passed as an rvalue to this function.

		for (auto const dac : iter_all<DACOnBoard>()) {
			visit_preorder(config.m_dacs[dac], dac, visitor);
		}

		halco::common::Unique const unique;
		visit_preorder(config.m_flyspi_config, unique, visitor);
		visit_preorder(config.m_flyspi_exception, unique, visitor);
		visit_preorder(config.m_spike_router, unique, visitor);
	}
};

} // namespace detail

} // namespace v2
} // namespace haldls
