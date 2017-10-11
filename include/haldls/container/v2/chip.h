#pragma once

#include "halco/common/typed_array.h"
#include "halco/hicann-dls/v2/coordinates.h"

#include "haldls/common/optional.h"
#include "haldls/common/visibility.h"
#include "haldls/container/v2/capmem.h"
#include "haldls/container/v2/common.h"
#include "haldls/container/v2/correlation.h"
#include "haldls/container/v2/neuron.h"
#include "haldls/container/v2/ppu.h"
#include "haldls/container/v2/rate_counter.h"
#include "haldls/container/v2/synapse.h"


namespace haldls {
namespace container {
namespace v2 {

class Chip
{
public:
	typedef halco::common::Unique coordinate_type;
	typedef std::false_type has_local_data;

	Chip() HALDLS_VISIBLE;

	void enable_buffered_readout(halco::hicann_dls::v2::NeuronOnDLS const& neuron) HALDLS_VISIBLE;
	void disable_buffered_readout() HALDLS_VISIBLE;
	common::optional<halco::hicann_dls::v2::NeuronOnDLS>
	get_buffered_readout_neuron() const HALDLS_VISIBLE;

	/// \brief Returns the digital configuration of the specified neuron.
	/// \note To prevent misconfiguration, enabling the external current input and
	///       external voltage output switches is only possible on the Chip level.
	///       Consequently the corresponding bits of the returned configuration container
	///       are always disabled.
	NeuronDigitalConfig get_neuron_digital_config(
		halco::hicann_dls::v2::NeuronOnDLS const& neuron) const HALDLS_VISIBLE;

	/// \brief Changes the digital configuration of the specified neuron.
	/// \note The state of the external current input and external voltage output switches
	///       is solely controlled through enable_buffered_readout(). Corresponding
	///       settings of the passed-in configuration have no effect on the stored instance.
	void set_neuron_digital_config(
		halco::hicann_dls::v2::NeuronOnDLS const& neuron, NeuronDigitalConfig value) HALDLS_VISIBLE;

	SynapseBlock get_synapse_block(halco::hicann_dls::v2::SynapseBlockOnDLS const& synapse_block) const HALDLS_VISIBLE;
	void set_synapse_block(
		halco::hicann_dls::v2::SynapseBlockOnDLS const& synapse_block, SynapseBlock const& value) HALDLS_VISIBLE;

	SynapseBlock::Synapse get_synapse(halco::hicann_dls::v2::SynapseOnDLS const& synapse) const HALDLS_VISIBLE;
	void set_synapse(
		halco::hicann_dls::v2::SynapseOnDLS const& synapse, SynapseBlock::Synapse const& value) HALDLS_VISIBLE;

	ColumnCorrelationBlock get_column_correlation_block(
		halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block) const HALDLS_VISIBLE;
	void set_column_correlation_block(
		halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block,
		ColumnCorrelationBlock const& value) HALDLS_VISIBLE;

	ColumnCorrelationBlock::ColumnCorrelationSwitch get_column_correlation_switch(
		halco::hicann_dls::v2::ColumnCorrelationSwitchOnDLS const& correlation_switch) const HALDLS_VISIBLE;
	void set_column_correlation_switch(
		halco::hicann_dls::v2::ColumnCorrelationSwitchOnDLS const& correlation_switch,
		ColumnCorrelationBlock::ColumnCorrelationSwitch const& value) HALDLS_VISIBLE;

	ColumnCurrentBlock get_column_current_block(
		halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block) const HALDLS_VISIBLE;
	void set_column_current_block(
		halco::hicann_dls::v2::ColumnBlockOnDLS const& column_block,
		ColumnCurrentBlock const& value) HALDLS_VISIBLE;

	ColumnCurrentBlock::ColumnCurrentSwitch get_column_current_switch(
		halco::hicann_dls::v2::ColumnCurrentSwitchOnDLS const& current_switch) const HALDLS_VISIBLE;
	void set_column_current_switch(
		halco::hicann_dls::v2::ColumnCurrentSwitchOnDLS const& current_switch,
		ColumnCurrentBlock::ColumnCurrentSwitch const& value) HALDLS_VISIBLE;

	CapMem get_capmem() const HALDLS_VISIBLE;
	void set_capmem(CapMem const& value) HALDLS_VISIBLE;

	PPUMemory get_ppu_memory() const HALDLS_VISIBLE;
	void set_ppu_memory(PPUMemory const& value) HALDLS_VISIBLE;

	PPUControlRegister get_ppu_control_register() const HALDLS_VISIBLE;
	void set_ppu_control_register(PPUControlRegister const& value) HALDLS_VISIBLE;

	// Read-only property, available when reading the chip config
	PPUStatusRegister get_ppu_status_register() const HALDLS_VISIBLE;

	RateCounter get_rate_counter() const HALDLS_VISIBLE;
	void set_rate_counter(RateCounter const& value) HALDLS_VISIBLE;

	SynapseDrivers get_synapse_drivers() const HALDLS_VISIBLE;
	void set_synapse_drivers(SynapseDrivers const& value) HALDLS_VISIBLE;

	CommonSynramConfig get_common_synram_config() const HALDLS_VISIBLE;
	void set_common_synram_config(CommonSynramConfig const& value) HALDLS_VISIBLE;

	CommonCapMemConfig get_common_capmem_config() const HALDLS_VISIBLE;
	void set_common_capmem_config(CommonCapMemConfig const& value) HALDLS_VISIBLE;

	CommonNeuronConfig get_common_neuron_config() const HALDLS_VISIBLE;
	void set_common_neuron_config(CommonNeuronConfig const& value) HALDLS_VISIBLE;

	CorrelationConfig get_correlation_config() const HALDLS_VISIBLE;
	void set_correlation_config(CorrelationConfig const& value) HALDLS_VISIBLE;

	bool operator==(Chip const& other) const HALDLS_VISIBLE;
	bool operator!=(Chip const& other) const HALDLS_VISIBLE;

	friend detail::VisitPreorderImpl<Chip>;

private:
	halco::common::typed_array<NeuronDigitalConfig, halco::hicann_dls::v2::NeuronOnDLS>
		m_neuron_digital_configs;
	halco::common::typed_array<SynapseBlock, halco::hicann_dls::v2::SynapseBlockOnDLS>
		m_synapse_blocks;
	halco::common::
		typed_array<ColumnCorrelationBlock, halco::hicann_dls::v2::ColumnBlockOnDLS>
			m_correlation_blocks;
	halco::common::typed_array<ColumnCurrentBlock, halco::hicann_dls::v2::ColumnBlockOnDLS>
		m_current_blocks;
	CapMem m_capmem;
	PPUMemory m_ppu_memory;
	PPUControlRegister m_ppu_control_register;
	PPUStatusRegister m_ppu_status_register;
	RateCounter m_rate_counter;

	SynapseDrivers m_synapse_drivers;
	CommonSynramConfig m_synram_config;
	CommonCapMemConfig m_capmem_config;
	CommonNeuronConfig m_neuron_config;
	CorrelationConfig m_correlation_config;
};

namespace detail {

template <>
struct VisitPreorderImpl<Chip> {
	template <typename ContainerT, typename VisitorT>
	static void call(
		ContainerT& config, halco::common::Unique const& coord, VisitorT&& visitor)
	{
		using halco::common::iter_all;
		using namespace halco::hicann_dls::v2;

		visitor(coord, config);

		// No std::forward for visitor argument, as we want to pass a reference to the
		// nested visitor in any case, even if it was passed as an rvalue to this function.

		halco::common::Unique const unique;

		// CommonSynramConfig _has_ to be configured before attempting changes to the synram.
		visit_preorder(config.m_synram_config, unique, visitor);

		for (auto const neuron : iter_all<NeuronOnDLS>()) {
			visit_preorder(config.m_neuron_digital_configs[neuron], neuron, visitor);
		}

		for (auto const synapse_block : iter_all<SynapseBlockOnDLS>()) {
			visit_preorder(config.m_synapse_blocks[synapse_block], synapse_block, visitor);
		}

		for (auto const column_block : iter_all<ColumnBlockOnDLS>()) {
			visit_preorder(config.m_correlation_blocks[column_block], column_block, visitor);
		}

		for (auto const column_block : iter_all<ColumnBlockOnDLS>()) {
			visit_preorder(config.m_current_blocks[column_block], column_block, visitor);
		}

		visit_preorder(config.m_capmem, unique, visitor);
		visit_preorder(config.m_ppu_memory, unique, visitor);
		visit_preorder(config.m_ppu_control_register, unique, visitor);
		visit_preorder(config.m_ppu_status_register, unique, visitor);
		visit_preorder(config.m_rate_counter, unique, visitor);
		visit_preorder(config.m_synapse_drivers, unique, visitor);
		visit_preorder(config.m_capmem_config, unique, visitor);
		visit_preorder(config.m_neuron_config, unique, visitor);
		visit_preorder(config.m_correlation_config, unique, visitor);
	}
};

} // namespace detail

} // namespace v2
} // namespace container
} // namespace haldls
