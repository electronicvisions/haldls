#include "haldls/vx/phy.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/common/cerealization_typed_array.h"
#include "halco/common/iter_all.h"
#include "halco/hicann-dls/vx/jtag.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.h"
#include "haldls/vx/omnibus_constants.h"
#include "haldls/vx/print.h"

namespace haldls {
namespace vx {

namespace detail {

PhyConfigBase::PhyConfigBase() :
    m_enable_bit_slip(false),
    m_manual_delay(),
    m_enable_delay_cell_measurement(false),
    m_enable_initialization_master_mode(false),
    m_enable_manual_tx_data_valid_for_init(false),
    m_enable_force_lvds_power_up(false),
    m_enable_force_start(false),
    m_enable_manual_training_mode(false),
    m_enable_ber_loopback(false),
    m_vbias(),
    m_debug_outputs(),
    m_enable_transmission_without_idle_pattern(false),
    m_enable_clock_pre_alignment(false),
    m_enable_des_recal(false),
    m_enable_loopback_en(false),
    m_enable_auto_init(false)
{}

bool PhyConfigBase::get_enable_bit_slip() const
{
	return m_enable_bit_slip;
}

void PhyConfigBase::set_enable_bit_slip(bool const value)
{
	m_enable_bit_slip = value;
}

typename PhyConfigBase::ManualDelay PhyConfigBase::get_manual_delay() const
{
	return m_manual_delay;
}

void PhyConfigBase::set_manual_delay(ManualDelay const value)
{
	m_manual_delay = value;
}

bool PhyConfigBase::get_enable_delay_cell_measurement() const
{
	return m_enable_delay_cell_measurement;
}

void PhyConfigBase::set_enable_delay_cell_measurement(bool const value)
{
	m_enable_delay_cell_measurement = value;
}

bool PhyConfigBase::get_enable_initialization_master_mode() const
{
	return m_enable_initialization_master_mode;
}

void PhyConfigBase::set_enable_initialization_master_mode(bool const value)
{
	m_enable_initialization_master_mode = value;
}

bool PhyConfigBase::get_enable_manual_tx_data_valid_for_init() const
{
	return m_enable_manual_tx_data_valid_for_init;
}

void PhyConfigBase::set_enable_manual_tx_data_valid_for_init(bool const value)
{
	m_enable_manual_tx_data_valid_for_init = value;
}

bool PhyConfigBase::get_enable_force_lvds_power_up() const
{
	return m_enable_force_lvds_power_up;
}

void PhyConfigBase::set_enable_force_lvds_power_up(bool const value)
{
	m_enable_force_lvds_power_up = value;
}

bool PhyConfigBase::get_enable_force_start() const
{
	return m_enable_force_start;
}

void PhyConfigBase::set_enable_force_start(bool const value)
{
	m_enable_force_start = value;
}

bool PhyConfigBase::get_enable_manual_training_mode() const
{
	return m_enable_manual_training_mode;
}

void PhyConfigBase::set_enable_manual_training_mode(bool const value)
{
	m_enable_manual_training_mode = value;
}

bool PhyConfigBase::get_enable_ber_loopback() const
{
	return m_enable_ber_loopback;
}

void PhyConfigBase::set_enable_ber_loopback(bool const value)
{
	m_enable_ber_loopback = value;
}

typename PhyConfigBase::VBias PhyConfigBase::get_vbias() const
{
	return m_vbias;
}

void PhyConfigBase::set_vbias(VBias const value)
{
	m_vbias = value;
}

typename PhyConfigBase::DebugOutputs PhyConfigBase::get_debug_outputs() const
{
	return m_debug_outputs;
}

void PhyConfigBase::set_debug_outputs(DebugOutputs const value)
{
	m_debug_outputs = value;
}

bool PhyConfigBase::get_enable_transmission_without_idle_pattern() const
{
	return m_enable_transmission_without_idle_pattern;
}

void PhyConfigBase::set_enable_transmission_without_idle_pattern(bool const value)
{
	m_enable_transmission_without_idle_pattern = value;
}

bool PhyConfigBase::get_enable_clock_pre_alignment() const
{
	return m_enable_clock_pre_alignment;
}

void PhyConfigBase::set_enable_clock_pre_alignment(bool const value)
{
	m_enable_clock_pre_alignment = value;
}

bool PhyConfigBase::get_enable_des_recal() const
{
	return m_enable_des_recal;
}

void PhyConfigBase::set_enable_des_recal(bool const value)
{
	m_enable_des_recal = value;
}

bool PhyConfigBase::get_enable_loopback_en() const
{
	return m_enable_loopback_en;
}

void PhyConfigBase::set_enable_loopback_en(bool const value)
{
	m_enable_loopback_en = value;
}

bool PhyConfigBase::get_enable_auto_init() const
{
	return m_enable_auto_init;
}

void PhyConfigBase::set_enable_auto_init(bool const value)
{
	m_enable_auto_init = value;
}

bool PhyConfigBase::operator==(PhyConfigBase const& other) const
{
	return m_enable_bit_slip == other.m_enable_bit_slip && m_manual_delay == other.m_manual_delay &&
	       m_enable_delay_cell_measurement == other.m_enable_delay_cell_measurement &&
	       m_enable_initialization_master_mode == other.m_enable_initialization_master_mode &&
	       m_enable_manual_tx_data_valid_for_init == other.m_enable_manual_tx_data_valid_for_init &&
	       m_enable_force_lvds_power_up == other.m_enable_force_lvds_power_up &&
	       m_enable_force_start == other.m_enable_force_start &&
	       m_enable_manual_training_mode == other.m_enable_manual_training_mode &&
	       m_enable_ber_loopback == other.m_enable_ber_loopback && m_vbias == other.m_vbias &&
	       m_debug_outputs == other.m_debug_outputs &&
	       m_enable_transmission_without_idle_pattern ==
	           other.m_enable_transmission_without_idle_pattern &&
	       m_enable_clock_pre_alignment == other.m_enable_clock_pre_alignment &&
	       m_enable_des_recal == other.m_enable_des_recal &&
	       m_enable_loopback_en == other.m_enable_loopback_en &&
	       m_enable_auto_init == other.m_enable_auto_init;
}

bool PhyConfigBase::operator!=(PhyConfigBase const& other) const
{
	return !(*this == other);
}

template <typename Archive>
void PhyConfigBase::cerealize_impl(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_bit_slip));
	ar(CEREAL_NVP(m_manual_delay));
	ar(CEREAL_NVP(m_enable_delay_cell_measurement));
	ar(CEREAL_NVP(m_enable_initialization_master_mode));
	ar(CEREAL_NVP(m_enable_manual_tx_data_valid_for_init));
	ar(CEREAL_NVP(m_enable_force_lvds_power_up));
	ar(CEREAL_NVP(m_enable_force_start));
	ar(CEREAL_NVP(m_enable_manual_training_mode));
	ar(CEREAL_NVP(m_enable_ber_loopback));
	ar(CEREAL_NVP(m_vbias));
	ar(CEREAL_NVP(m_debug_outputs));
	ar(CEREAL_NVP(m_enable_transmission_without_idle_pattern));
	ar(CEREAL_NVP(m_enable_clock_pre_alignment));
	ar(CEREAL_NVP(m_enable_des_recal));
	ar(CEREAL_NVP(m_enable_loopback_en));
	ar(CEREAL_NVP(m_enable_auto_init));
}

namespace {

struct PhyConfigBaseBitfield
{
	union
	{
		uint32_t raw;
		// clang-format off
		struct __attribute__((packed)) {
			uint32_t enable_bit_slip                          :  1;
			uint32_t manual_delay                             :  4;
			uint32_t enable_delay_cell_measurement            :  1;
			uint32_t enable_initialization_master_mode        :  1;
			uint32_t enable_manual_tx_data_valid_for_init     :  1;
			uint32_t enable_force_lvds_power_up               :  1;
			uint32_t enable_force_start                       :  1;
			uint32_t enable_manual_training_mode              :  1;
			uint32_t enable_ber_loopback                      :  1;
			uint32_t vbias                                    :  3;
			uint32_t debug_outputs                            :  1;
			uint32_t /* unused */                             :  1;
			uint32_t enable_transmission_without_idle_pattern :  1;
			uint32_t enable_clock_pre_alignment               :  1;
			uint32_t enable_des_recal                         :  1;
			uint32_t enable_loopback_en                       :  1;
			uint32_t enable_auto_init                         :  1;
			uint32_t /* unused */                             : 10;
		} m;
		// clang-format on
		static_assert(sizeof(raw) == sizeof(m), "sizes of union types should match");
	} u;

	PhyConfigBaseBitfield() { u.raw = 0u; }

	PhyConfigBaseBitfield(uint32_t data) { u.raw = data; }
};

} // namespace

uint32_t PhyConfigBase::pack() const
{
	PhyConfigBaseBitfield bitfield;

	bitfield.u.m.enable_bit_slip = m_enable_bit_slip;
	bitfield.u.m.manual_delay = m_manual_delay;
	bitfield.u.m.enable_delay_cell_measurement = m_enable_delay_cell_measurement;
	bitfield.u.m.enable_initialization_master_mode = m_enable_initialization_master_mode;
	bitfield.u.m.enable_manual_tx_data_valid_for_init = m_enable_manual_tx_data_valid_for_init;
	bitfield.u.m.enable_force_lvds_power_up = m_enable_force_lvds_power_up;
	bitfield.u.m.enable_force_start = m_enable_force_start;
	bitfield.u.m.enable_manual_training_mode = m_enable_manual_training_mode;
	bitfield.u.m.enable_ber_loopback = m_enable_ber_loopback;
	bitfield.u.m.vbias = m_vbias;
	bitfield.u.m.debug_outputs = m_debug_outputs;
	bitfield.u.m.enable_transmission_without_idle_pattern =
	    m_enable_transmission_without_idle_pattern;
	bitfield.u.m.enable_clock_pre_alignment = m_enable_clock_pre_alignment;
	bitfield.u.m.enable_des_recal = m_enable_des_recal;
	bitfield.u.m.enable_loopback_en = m_enable_loopback_en;
	bitfield.u.m.enable_auto_init = m_enable_auto_init;

	return bitfield.u.raw;
}

void PhyConfigBase::unpack(uint32_t const value)
{
	PhyConfigBaseBitfield bitfield(value);

	m_enable_bit_slip = bitfield.u.m.enable_bit_slip;
	m_manual_delay = ManualDelay(bitfield.u.m.manual_delay);
	m_enable_delay_cell_measurement = bitfield.u.m.enable_delay_cell_measurement;
	m_enable_initialization_master_mode = bitfield.u.m.enable_initialization_master_mode;
	m_enable_manual_tx_data_valid_for_init = bitfield.u.m.enable_manual_tx_data_valid_for_init;
	m_enable_force_lvds_power_up = bitfield.u.m.enable_force_lvds_power_up;
	m_enable_force_start = bitfield.u.m.enable_force_start;
	m_enable_manual_training_mode = bitfield.u.m.enable_manual_training_mode;
	m_enable_ber_loopback = bitfield.u.m.enable_ber_loopback;
	m_vbias = VBias(bitfield.u.m.vbias);
	m_debug_outputs = DebugOutputs(bitfield.u.m.debug_outputs);
	m_enable_transmission_without_idle_pattern =
	    bitfield.u.m.enable_transmission_without_idle_pattern;
	m_enable_clock_pre_alignment = bitfield.u.m.enable_clock_pre_alignment;
	m_enable_des_recal = bitfield.u.m.enable_des_recal;
	m_enable_loopback_en = bitfield.u.m.enable_loopback_en;
	m_enable_auto_init = bitfield.u.m.enable_auto_init;
}

} // namespace detail

PhyConfigFPGA::PhyConfigFPGA() : PhyConfigBase()
{
	set_enable_initialization_master_mode(true);
	set_vbias(VBias(4));
	set_enable_auto_init(true);
}

bool PhyConfigFPGA::operator==(PhyConfigFPGA const& other) const
{
	return (static_cast<detail::PhyConfigBase>(*this) == static_cast<detail::PhyConfigBase>(other));
}

bool PhyConfigFPGA::operator!=(PhyConfigFPGA const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(PhyConfigFPGA)

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, PhyConfigFPGA::config_size_in_words>
PhyConfigFPGA::addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusFPGAAddress(phy_omnibus_mask + coord.toEnum())};
}

std::array<fisch::vx::OmnibusFPGA, PhyConfigFPGA::config_size_in_words> PhyConfigFPGA::encode()
    const
{
	return {fisch::vx::OmnibusFPGA(fisch::vx::OmnibusData(pack()))};
}

void PhyConfigFPGA::decode(
    std::array<fisch::vx::OmnibusFPGA, PhyConfigFPGA::config_size_in_words> const& data)
{
	unpack(data[0].get().value());
}

template <typename Archive>
void PhyConfigFPGA::serialize(Archive& ar)
{
	cerealize_impl(ar);
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PhyConfigFPGA)


PhyConfigChip::PhyConfigChip() : PhyConfigBase()
{
	set_vbias(VBias(4));
	set_enable_auto_init(true);
}

bool PhyConfigChip::operator==(PhyConfigChip const& other) const
{
	return (static_cast<detail::PhyConfigBase>(*this) == static_cast<detail::PhyConfigBase>(other));
}

bool PhyConfigChip::operator!=(PhyConfigChip const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(PhyConfigChip)

std::array<halco::hicann_dls::vx::JTAGPhyRegisterOnDLS, PhyConfigChip::config_size_in_words>
PhyConfigChip::addresses(coordinate_type const& coord)
{
	return {coord.toJTAGPhyRegisterOnDLS()};
}

std::array<fisch::vx::JTAGPhyRegister, PhyConfigChip::config_size_in_words> PhyConfigChip::encode()
    const
{
	return {fisch::vx::JTAGPhyRegister(fisch::vx::JTAGPhyRegister::Value(pack()))};
}

void PhyConfigChip::decode(
    std::array<fisch::vx::JTAGPhyRegister, PhyConfigChip::config_size_in_words> const& /*data*/)
{}

template <typename Archive>
void PhyConfigChip::serialize(Archive& ar)
{
	cerealize_impl(ar);
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PhyConfigChip)


CommonPhyConfigFPGA::CommonPhyConfigFPGA() : m_enable_phy()
{
	m_enable_phy.fill(true);
}

bool CommonPhyConfigFPGA::get_enable_phy(
    halco::hicann_dls::vx::PhyConfigFPGAOnDLS const& coord) const
{
	return m_enable_phy[coord];
}

void CommonPhyConfigFPGA::set_enable_phy(
    halco::hicann_dls::vx::PhyConfigFPGAOnDLS const& coord, bool const value)
{
	m_enable_phy[coord] = value;
}

bool CommonPhyConfigFPGA::operator==(CommonPhyConfigFPGA const& other) const
{
	return m_enable_phy == other.m_enable_phy;
}

bool CommonPhyConfigFPGA::operator!=(CommonPhyConfigFPGA const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(CommonPhyConfigFPGA)

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, CommonPhyConfigFPGA::config_size_in_words>
CommonPhyConfigFPGA::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusFPGAAddress(ut_omnibus_mask)};
}

std::array<fisch::vx::OmnibusFPGA, CommonPhyConfigFPGA::config_size_in_words>
CommonPhyConfigFPGA::encode() const
{
	hate::bitset<halco::hicann_dls::vx::PhyConfigFPGAOnDLS::size> enable_mask;
	for (auto phy : halco::common::iter_all<halco::hicann_dls::vx::PhyConfigFPGAOnDLS>()) {
		enable_mask.set(phy.toEnum(), m_enable_phy[phy]);
	}

	return {fisch::vx::OmnibusFPGA(
	    fisch::vx::OmnibusData(static_cast<fisch::vx::OmnibusData::value_type>(enable_mask)))};
}

void CommonPhyConfigFPGA::decode(
    std::array<fisch::vx::OmnibusFPGA, CommonPhyConfigFPGA::config_size_in_words> const& data)
{
	hate::bitset<halco::hicann_dls::vx::PhyConfigFPGAOnDLS::size> enable_mask(data.at(0).get());
	for (auto phy : halco::common::iter_all<halco::hicann_dls::vx::PhyConfigFPGAOnDLS>()) {
		m_enable_phy[phy] = enable_mask.test(phy.toEnum());
	}
}

template <typename Archive>
void CommonPhyConfigFPGA::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_phy));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CommonPhyConfigFPGA)


CommonPhyConfigChip::CommonPhyConfigChip() : m_enable_phy()
{
	m_enable_phy.fill(true);
}

bool CommonPhyConfigChip::get_enable_phy(
    halco::hicann_dls::vx::PhyConfigChipOnDLS const& coord) const
{
	return m_enable_phy[coord];
}

void CommonPhyConfigChip::set_enable_phy(
    halco::hicann_dls::vx::PhyConfigChipOnDLS const& coord, bool const value)
{
	m_enable_phy[coord] = value;
}

bool CommonPhyConfigChip::operator==(CommonPhyConfigChip const& other) const
{
	return m_enable_phy == other.m_enable_phy;
}

bool CommonPhyConfigChip::operator!=(CommonPhyConfigChip const& other) const
{
	return !(*this == other);
}

HALDLS_VX_DEFAULT_OSTREAM_OP(CommonPhyConfigChip)

std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    CommonPhyConfigChip::config_size_in_words>
CommonPhyConfigChip::addresses(coordinate_type const& /*coord*/)
{
	return {halco::hicann_dls::vx::OmnibusChipOverJTAGAddress(phy_on_chip_base_address)};
}

std::array<fisch::vx::OmnibusChipOverJTAG, CommonPhyConfigChip::config_size_in_words>
CommonPhyConfigChip::encode() const
{
	hate::bitset<halco::hicann_dls::vx::PhyConfigChipOnDLS::size> enable_mask;
	for (auto phy : halco::common::iter_all<halco::hicann_dls::vx::PhyConfigChipOnDLS>()) {
		// phy enables on chip are reversed compared to on FPGA
		enable_mask.set(
		    halco::hicann_dls::vx::PhyConfigChipOnDLS::max - phy.toEnum(), m_enable_phy[phy]);
	}

	return {fisch::vx::OmnibusChipOverJTAG(
	    fisch::vx::OmnibusData(static_cast<fisch::vx::OmnibusData::value_type>(enable_mask)))};
}

void CommonPhyConfigChip::decode(std::array<
                                 fisch::vx::OmnibusChipOverJTAG,
                                 CommonPhyConfigChip::config_size_in_words> const& /*data*/)
{}

template <typename Archive>
void CommonPhyConfigChip::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_enable_phy));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(CommonPhyConfigChip)


PhyStatus::PhyStatus() :
    m_crc_error_count(),
    m_online_time(),
    m_rx_dropped_count(),
    m_rx_count(),
    m_tx_count()
{}

PhyStatus::CRCErrorCount PhyStatus::get_crc_error_count() const
{
	return m_crc_error_count;
}

void PhyStatus::set_crc_error_count(CRCErrorCount const value)
{
	m_crc_error_count = value;
}

PhyStatus::OnlineTime PhyStatus::get_online_time() const
{
	return m_online_time;
}

void PhyStatus::set_online_time(OnlineTime const value)
{
	m_online_time = value;
}

PhyStatus::RxDroppedCount PhyStatus::get_rx_dropped_count() const
{
	return m_rx_dropped_count;
}

void PhyStatus::set_rx_dropped_count(RxDroppedCount const value)
{
	m_rx_dropped_count = value;
}

PhyStatus::RxCount PhyStatus::get_rx_count() const
{
	return m_rx_count;
}

void PhyStatus::set_rx_count(RxCount const value)
{
	m_rx_count = value;
}

PhyStatus::TxCount PhyStatus::get_tx_count() const
{
	return m_tx_count;
}

void PhyStatus::set_tx_count(TxCount const value)
{
	m_tx_count = value;
}

bool PhyStatus::operator==(PhyStatus const& other) const
{
	return (m_crc_error_count == other.m_crc_error_count) &&
	       (m_online_time == other.m_online_time) &&
	       (m_rx_dropped_count == other.m_rx_dropped_count) && (m_rx_count == other.m_rx_count) &&
	       (m_tx_count == other.m_tx_count);
}

bool PhyStatus::operator!=(PhyStatus const& other) const
{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, PhyStatus const& config)
{
	os << config.m_crc_error_count << std::endl;
	os << config.m_online_time << std::endl;
	os << config.m_rx_dropped_count << std::endl;
	os << config.m_rx_count << std::endl;
	os << config.m_tx_count;
	return os;
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, PhyStatus::read_config_size_in_words>
PhyStatus::read_addresses(coordinate_type const& coord)
{
	return {halco::hicann_dls::vx::OmnibusFPGAAddress(ut_omnibus_mask + 4 + coord.value()),
	        halco::hicann_dls::vx::OmnibusFPGAAddress(
	            ut_omnibus_mask + 4 + coordinate_type::size + coord.value()),
	        halco::hicann_dls::vx::OmnibusFPGAAddress(
	            ut_omnibus_mask + 4 + 2 * coordinate_type::size + coord.value()),
	        halco::hicann_dls::vx::OmnibusFPGAAddress(l2_omnibus_mask + 2 * coord.value()),
	        halco::hicann_dls::vx::OmnibusFPGAAddress(l2_omnibus_mask + 2 * coord.value() + 1)};
}

std::array<halco::hicann_dls::vx::OmnibusFPGAAddress, PhyStatus::write_config_size_in_words>
PhyStatus::write_addresses(coordinate_type const& /*coord*/)
{
	return {};
}

std::array<fisch::vx::OmnibusFPGA, PhyStatus::write_config_size_in_words> PhyStatus::encode() const
{
	return {};
}

void PhyStatus::decode(
    std::array<fisch::vx::OmnibusFPGA, PhyStatus::read_config_size_in_words> const& data)
{
	m_crc_error_count = CRCErrorCount(data.at(0).get());
	m_online_time = OnlineTime(data.at(1).get());
	m_rx_dropped_count = RxDroppedCount(data.at(2).get());
	m_tx_count = TxCount(data.at(3).get());
	m_rx_count = RxCount(data.at(4).get());
}

template <typename Archive>
void PhyStatus::serialize(Archive& ar)
{
	ar(CEREAL_NVP(m_crc_error_count));
	ar(CEREAL_NVP(m_online_time));
	ar(CEREAL_NVP(m_rx_dropped_count));
	ar(CEREAL_NVP(m_rx_count));
	ar(CEREAL_NVP(m_tx_count));
}

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(PhyStatus)

} // namespace vx
} // namespace haldls
