#pragma once

#include "halco/common/genpybind.h"
#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/visibility.h"
#include "haldls/v2/common.h"
#include "haldls/v2/dac.h"
#include "haldls/v2/register.h"
#include "haldls/v2/synapse.h"

namespace haldls {
namespace v2 GENPYBIND(tag(haldls_v2)) {

class GENPYBIND(visible) SpikeRouter
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::true_type is_leaf_node;

	SpikeRouter() HALDLS_VISIBLE;

	struct GENPYBIND(inline_base("*")) Delay
		: public halco::common::detail::RantWrapper<Delay, uint_fast16_t, 0x4000 - 1, 0>
	{
		constexpr explicit Delay(uintmax_t const val = 0) HALDLS_VISIBLE : rant_t(val) {}
	};

	/// \brief Disable routing of spikes altogether (default).
	void reset() HALDLS_VISIBLE;

	/// \brief Enable "squeeze" mode, where every spike received in a specified interval
	///        is combined into a single packet with a fixed spike address.
	/// \note In the verilog implementation this mode is called "bypass" mode.  It is
	///       implemented by or-ing the spike vector (target synapse drivers) of all
	///       received spikes and assumes a fixed mapping from neurons to synapse drivers,
	///       e.g. spike events of neuron 0, 1, 2, … will be sent to synapse drivers 0,
	///       1, 2, ….
	/// After a specified delay, a single spike with a fixed address will be emitted (as a
	/// single packet) to all synapse drivers, whose corresponding neuron spiked at least
	/// once in that interval.
	void enable_squeeze_mode(
		SynapseBlock::Synapse::Address const& address, Delay const& delay) HALDLS_VISIBLE;

	typedef std::bitset<halco::hicann_dls::v2::SynapseDriverOnDLS::size> target_rows_type;

	void set_neuron_route(
		halco::hicann_dls::v2::NeuronOnDLS const& neuron,
		SynapseBlock::Synapse::Address const& address,
		target_rows_type const& target_rows) HALDLS_VISIBLE;

	bool operator==(SpikeRouter const& other) const HALDLS_VISIBLE;
	bool operator!=(SpikeRouter const& other) const HALDLS_VISIBLE;

	static size_t constexpr config_size_in_words GENPYBIND(hidden) =
		2 * halco::hicann_dls::v2::NeuronOnDLS::size + 1;
	std::array<ocp_address_type, config_size_in_words> write_addresses(
		coordinate_type const& unique) const HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<ocp_address_type, 0> read_addresses(coordinate_type const& unique) const
		HALDLS_VISIBLE GENPYBIND(hidden);
	std::array<ocp_word_type, config_size_in_words> encode() const HALDLS_VISIBLE GENPYBIND(hidden);
	void decode(std::array<ocp_word_type, 0> const& words) HALDLS_VISIBLE GENPYBIND(hidden);

private:
	bool m_squeeze_mode_enabled;
	SynapseBlock::Synapse::Address m_squeeze_mode_address;
	Delay m_squeeze_mode_delay;

	halco::common::typed_array<SynapseBlock::Synapse::Address, halco::hicann_dls::v2::NeuronOnDLS>
		m_address_by_neuron;
	halco::common::typed_array<target_rows_type, halco::hicann_dls::v2::NeuronOnDLS>
		m_target_rows_by_neuron;
}; // SpikeRouter

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


	Board() HALDLS_VISIBLE;

	// accessors
	void set_parameter(Parameter const& parameter, DAC::Value const& value) HALDLS_VISIBLE;
	DAC::Value get_parameter(Parameter const& parameter) const HALDLS_VISIBLE;

	FlyspiConfig get_flyspi_config() const HALDLS_VISIBLE;
	void set_flyspi_config(FlyspiConfig const& config) HALDLS_VISIBLE;

	SpikeRouter get_spike_router() const HALDLS_VISIBLE;
	void set_spike_router(SpikeRouter const& config) HALDLS_VISIBLE;

	// read-only accessor
	FlyspiException get_flyspi_exception() const HALDLS_VISIBLE;

	bool operator==(Board const& other) const HALDLS_VISIBLE;
	bool operator!=(Board const& other) const HALDLS_VISIBLE;

	friend detail::VisitPreorderImpl<Board>;

private:
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

namespace std {

HALCO_GEOMETRY_HASH_CLASS(haldls::v2::SpikeRouter::Delay)

} // namespace std
