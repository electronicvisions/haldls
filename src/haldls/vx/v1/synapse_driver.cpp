#include "haldls/vx/v1/synapse_driver.h"

#include "fisch/vx/jtag.h"
#include "fisch/vx/omnibus.h"
#include "halco/common/cerealization_geometry.h"
#include "halco/hicann-dls/vx/omnibus.h"
#include "haldls/cerealization.tcc"
#include "haldls/vx/omnibus_constants.h"

namespace haldls::vx::v1 {

SynapseDriverConfig::SynapseDriverConfig() :
    m_en_receiver(false),
    m_row_address_compare_mask(),
    m_en_address_out(true),
    m_utilization(),
    m_offset(),
    m_recovery(),
    m_row_mode_bottom(RowMode::disabled),
    m_row_mode_top(RowMode::disabled),
    m_select_target_voltages(),
    m_en_readout(false),
    m_en_renewing(false),
    m_en_hagen_modulation(false),
    m_en_stp(false),
    m_en_charge_sharing(false),
    m_en_recovery(false)
{}

struct SynapseDriverConfig::SynapseDriverConfigBitfield
{
	union
	{
		std::array<uint32_t, SynapseDriverConfig::config_size_in_words> words;
		// clang-format off
        struct __attribute__((packed)) {
			uint32_t utilization                                          :  4;  // 3-0   ; 0
			uint32_t offset                                               :  4;  // 7-4   ; 0
			uint32_t en_receiver                                          :  1;  // 8     ; 0
			uint32_t row_address_compare_mask                             :  5;  // 9     ; 0
			uint32_t en_address_out                                       :  1;  // 14    ; 0
			uint32_t                                                      :  1;  // 15    ; 0
			uint32_t                                                      : 16;  // 31-16 ; 0
			uint32_t recovery                                             :  4;  // 3-0   ; 1
			uint32_t en_exc_bottom                                        :  1;  // 4     ; 1
			uint32_t en_exc_top                                           :  1;  // 5     ; 1
			uint32_t en_inh_bottom                                        :  1;  // 6     ; 1
			uint32_t en_inh_top                                           :  1;  // 7     ; 1
			uint32_t                                                      : 24;  // 31-8  ; 1
			uint32_t select_target_voltages                               :  1;  // 0     ; 2
			uint32_t en_readout                                           :  1;  // 1     ; 2
			uint32_t en_renewing                                          :  1;  // 2     ; 2
			uint32_t en_hagen_modulation                                  :  1;  // 3     ; 2
			uint32_t en_stp                                               :  1;  // 4     ; 2
			uint32_t                                                      :  1;  // 5     ; 2
			uint32_t en_charge_sharing                                    :  1;  // 6     ; 2
			uint32_t en_recovery                                          :  1;  // 7     ; 2
			uint32_t                                                      : 24;  // 31-8  ; 2
        } m;
		// clang-format on
		static_assert(sizeof(words) == sizeof(m), "Sizes of union types should match.");
	} u;

	SynapseDriverConfigBitfield() { u.words = {{0, 0, 0}}; }

	SynapseDriverConfigBitfield(std::array<uint32_t, SynapseDriverConfig::config_size_in_words> data)
	{
		u.words[0] = data[0] & 0b0111'1111'0011'1111u;
		u.words[1] = data[1] & 0b0000'0000'1111'1111u;
		u.words[2] = data[2] & 0b0000'0000'1101'1111u;
	}
};


bool SynapseDriverConfig::get_enable_receiver() const
{
	return m_en_receiver;
}

void SynapseDriverConfig::set_enable_receiver(bool const value)
{
	m_en_receiver = value;
}

SynapseDriverConfig::RowAddressCompareMask SynapseDriverConfig::get_row_address_compare_mask() const
{
	return m_row_address_compare_mask;
}

void SynapseDriverConfig::set_row_address_compare_mask(SynapseDriverConfig::RowAddressCompareMask const value)
{
	m_row_address_compare_mask = value;
}

bool SynapseDriverConfig::get_enable_address_out() const
{
	return m_en_address_out;
}

void SynapseDriverConfig::set_enable_address_out(bool const value)
{
	m_en_address_out = value;
}

SynapseDriverConfig::Utilization SynapseDriverConfig::get_utilization() const
{
	return m_utilization;
}

void SynapseDriverConfig::set_utilization(SynapseDriverConfig::Utilization const value)
{
	m_utilization = value;
}

SynapseDriverConfig::Offset SynapseDriverConfig::get_offset() const
{
	return m_offset;
}

void SynapseDriverConfig::set_offset(SynapseDriverConfig::Offset const value)
{
	m_offset = value;
}

SynapseDriverConfig::Recovery SynapseDriverConfig::get_recovery() const
{
	return m_recovery;
}

void SynapseDriverConfig::set_recovery(SynapseDriverConfig::Recovery const value)
{
	m_recovery = value;
}

SynapseDriverConfig::RowMode SynapseDriverConfig::get_row_mode_bottom() const
{
	return m_row_mode_bottom;
}

void SynapseDriverConfig::set_row_mode_bottom(RowMode const value)
{
	m_row_mode_bottom = value;
}

SynapseDriverConfig::RowMode SynapseDriverConfig::get_row_mode_top() const
{
	return m_row_mode_top;
}

void SynapseDriverConfig::set_row_mode_top(RowMode const value)
{
	m_row_mode_top = value;
}

SynapseDriverConfig::TargetVoltages SynapseDriverConfig::get_select_target_voltages() const
{
	return m_select_target_voltages;
}

void SynapseDriverConfig::set_select_target_voltages(SynapseDriverConfig::TargetVoltages const value)
{
	m_select_target_voltages = value;
}

bool SynapseDriverConfig::get_enable_readout() const
{
	return m_en_readout;
}

void SynapseDriverConfig::set_enable_readout(bool const value)
{
	m_en_readout = value;
}

bool SynapseDriverConfig::get_enable_renewing() const
{
	return m_en_renewing;
}

void SynapseDriverConfig::set_enable_renewing(bool const value)
{
	m_en_renewing = value;
}

bool SynapseDriverConfig::get_enable_hagen_modulation() const
{
	return m_en_hagen_modulation;
}

void SynapseDriverConfig::set_enable_hagen_modulation(bool const value)
{
	m_en_hagen_modulation = value;
}

bool SynapseDriverConfig::get_enable_stp() const
{
	return m_en_stp;
}

void SynapseDriverConfig::set_enable_stp(bool const value)
{
	m_en_stp = value;
}

bool SynapseDriverConfig::get_enable_charge_sharing() const
{
	return m_en_charge_sharing;
}

void SynapseDriverConfig::set_enable_charge_sharing(bool const value)
{
	m_en_charge_sharing = value;
}

bool SynapseDriverConfig::get_enable_recovery() const
{
	return m_en_recovery;
}

void SynapseDriverConfig::set_enable_recovery(bool const value)
{
	m_en_recovery = value;
}

template <typename AddressT>
std::array<AddressT, SynapseDriverConfig::config_size_in_words> SynapseDriverConfig::addresses(
    SynapseDriverConfig::coordinate_type const& synapse_driver)
{
	assert(synapse_driver.toSynapseDriverBlockOnDLS() < synapse_driver_sram_base_addresses.size());
	auto const base_address = synapse_driver_sram_base_addresses.at(synapse_driver.toSynapseDriverBlockOnDLS());
	auto const synapse_driver_coord = synapse_driver.toSynapseDriverOnSynapseDriverBlock();
	std::array<AddressT, SynapseDriverConfig::config_size_in_words> data;
	size_t i = 0;
	std::generate(data.begin(), data.end(), [&]() {
		return static_cast<AddressT>(
		    base_address + synapse_driver_coord * 4 + i++);
	});
	return data;
}

template SYMBOL_VISIBLE std::array<
    halco::hicann_dls::vx::OmnibusChipOverJTAGAddress,
    SynapseDriverConfig::config_size_in_words>
SynapseDriverConfig::addresses<halco::hicann_dls::vx::OmnibusChipOverJTAGAddress>(
    coordinate_type const& coord);

template SYMBOL_VISIBLE
    std::array<halco::hicann_dls::vx::OmnibusAddress, SynapseDriverConfig::config_size_in_words>
    SynapseDriverConfig::addresses<halco::hicann_dls::vx::OmnibusAddress>(
        coordinate_type const& coord);


namespace {

SynapseDriverConfig::Recovery::value_type reverse_4_bit_value(
    SynapseDriverConfig::Recovery::value_type b)
{
	b = ((b & 0xC) >> 2) | ((b & 0x3) << 2);
	b = ((b & 0xA) >> 1) | ((b & 0x5) << 1);
	return b;
}

} // anonymous namespace

SynapseDriverConfig::SynapseDriverConfigBitfield SynapseDriverConfig::to_bitfield() const
{
	SynapseDriverConfigBitfield bitfield;
	bitfield.u.m.en_receiver = m_en_receiver;
	bitfield.u.m.row_address_compare_mask = m_row_address_compare_mask;
	bitfield.u.m.en_address_out = m_en_address_out;
	bitfield.u.m.utilization = m_utilization;
	bitfield.u.m.offset = m_offset;
	bitfield.u.m.recovery = reverse_4_bit_value(m_recovery);
	bitfield.u.m.en_exc_bottom = (m_row_mode_bottom == RowMode::excitatory) ||
	                             (m_row_mode_bottom == RowMode::excitatory_and_inhibitory);
	bitfield.u.m.en_exc_top = (m_row_mode_top == RowMode::excitatory) ||
	                          (m_row_mode_top == RowMode::excitatory_and_inhibitory);
	bitfield.u.m.en_inh_bottom = (m_row_mode_bottom == RowMode::inhibitory) ||
	                             (m_row_mode_bottom == RowMode::excitatory_and_inhibitory);
	bitfield.u.m.en_inh_top = (m_row_mode_top == RowMode::inhibitory) ||
	                          (m_row_mode_top == RowMode::excitatory_and_inhibitory);
	bitfield.u.m.select_target_voltages = m_select_target_voltages;
	bitfield.u.m.en_readout = m_en_readout;
	bitfield.u.m.en_renewing = m_en_renewing;
	bitfield.u.m.en_hagen_modulation = m_en_hagen_modulation;
	bitfield.u.m.en_stp = m_en_stp;
	bitfield.u.m.en_charge_sharing = m_en_charge_sharing;
	bitfield.u.m.en_recovery = m_en_recovery;
	return bitfield;
}

void SynapseDriverConfig::from_bitfield(SynapseDriverConfig::SynapseDriverConfigBitfield const& bitfield)
{
	m_en_receiver = bitfield.u.m.en_receiver;
	m_row_address_compare_mask = RowAddressCompareMask(bitfield.u.m.row_address_compare_mask);
	m_en_address_out = bitfield.u.m.en_address_out;
	m_utilization = Utilization(bitfield.u.m.utilization);
	m_offset = Offset(bitfield.u.m.offset);
	m_recovery = Recovery(reverse_4_bit_value(bitfield.u.m.recovery));
	m_row_mode_bottom =
	    bitfield.u.m.en_exc_bottom
	        ? (bitfield.u.m.en_inh_bottom ? RowMode::excitatory_and_inhibitory
	                                      : RowMode::excitatory)
	        : (bitfield.u.m.en_inh_bottom ? RowMode::inhibitory : RowMode::disabled);
	m_row_mode_top =
	    bitfield.u.m.en_exc_top
	        ? (bitfield.u.m.en_inh_top ? RowMode::excitatory_and_inhibitory : RowMode::excitatory)
	        : (bitfield.u.m.en_inh_top ? RowMode::inhibitory : RowMode::disabled);
	m_select_target_voltages = TargetVoltages(bitfield.u.m.select_target_voltages);
	m_en_readout = bitfield.u.m.en_readout;
	m_en_renewing = bitfield.u.m.en_renewing;
	m_en_hagen_modulation = bitfield.u.m.en_hagen_modulation;
	m_en_stp = bitfield.u.m.en_stp;
	m_en_charge_sharing = bitfield.u.m.en_charge_sharing;
	m_en_recovery = bitfield.u.m.en_recovery;
}

template <typename WordT>
std::array<WordT, SynapseDriverConfig::config_size_in_words> SynapseDriverConfig::encode() const
{
	auto bitfield = to_bitfield();
	std::array<WordT, SynapseDriverConfig::config_size_in_words> data;
	std::transform(
	    bitfield.u.words.begin(), bitfield.u.words.end(), data.begin(),
	    [](uint32_t const& w) { return static_cast<WordT>(fisch::vx::OmnibusData(w)); });
	return data;
}

template SYMBOL_VISIBLE
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseDriverConfig::config_size_in_words>
    SynapseDriverConfig::encode<fisch::vx::OmnibusChipOverJTAG>() const;

template SYMBOL_VISIBLE std::array<fisch::vx::Omnibus, SynapseDriverConfig::config_size_in_words>
SynapseDriverConfig::encode<fisch::vx::Omnibus>() const;

template <typename WordT>
void SynapseDriverConfig::decode(std::array<WordT, SynapseDriverConfig::config_size_in_words> const& data)
{
	std::array<uint32_t, SynapseDriverConfig::config_size_in_words> raw_data;
	std::transform(
	    data.begin(), data.end(), raw_data.begin(), [](WordT const& w) { return w.get(); });
	SynapseDriverConfigBitfield bitfield(raw_data);
	from_bitfield(bitfield);
}

template SYMBOL_VISIBLE void SynapseDriverConfig::decode<fisch::vx::OmnibusChipOverJTAG>(
    std::array<fisch::vx::OmnibusChipOverJTAG, SynapseDriverConfig::config_size_in_words> const& data);

template SYMBOL_VISIBLE void SynapseDriverConfig::decode<fisch::vx::Omnibus>(
    std::array<fisch::vx::Omnibus, SynapseDriverConfig::config_size_in_words> const& data);

std::ostream& operator<<(std::ostream& os, SynapseDriverConfig::RowMode const& mode)
{
	switch (mode) {
		case SynapseDriverConfig::RowMode::disabled:
			return os << "disabled";
		case SynapseDriverConfig::RowMode::excitatory:
			return os << "excitatory";
		case SynapseDriverConfig::RowMode::inhibitory:
			return os << "inhibitory";
		case SynapseDriverConfig::RowMode::excitatory_and_inhibitory:
			return os << "excitatory_and_inhibitory";
		default:
			throw std::logic_error("Ostream operator of specified RowMode not implemented");
	}
}

std::ostream& operator<<(std::ostream& os, SynapseDriverConfig const& config)
{
	std::stringstream ss;
	ss << "SynapseDriverConfig(\n" << std::boolalpha;
	// clang-format off
	ss << "\tenable_receiver:          \t" << config.m_en_receiver << "\n"
	   << "\trow_address_compare_mask: \t" << config.m_row_address_compare_mask << "\n"
	   << "\tenable_address_out:       \t" << config.m_en_address_out << "\n"
	   << "\tutilization:              \t" << config.m_utilization << "\n"
	   << "\toffset:                   \t" << config.m_offset << "\n"
	   << "\trecovery:                 \t" << config.m_recovery << "\n"
	   << "\trow_mode_bottom:          \t" << config.m_row_mode_bottom << "\n"
	   << "\trow_mode_top:             \t" << config.m_row_mode_top << "\n"
	   << "\tselect_target_voltages:   \t" << config.m_select_target_voltages << "\n"
	   << "\tenable_readout:           \t" << config.m_en_readout << "\n"
	   << "\tenable_renewing:          \t" << config.m_en_renewing << "\n"
	   << "\tenable_hagen_modulation:  \t" << config.m_en_hagen_modulation << "\n"
	   << "\tenable_stp:               \t" << config.m_en_stp << "\n"
	   << "\tenable_charge_sharing:    \t" << config.m_en_charge_sharing << "\n"
	   << "\tenable_recovery:          \t" << config.m_en_recovery << "\n)";
	// clang-format on
	return (os << ss.str());
}

bool SynapseDriverConfig::operator==(SynapseDriverConfig const& other) const
{
	// clang-format off
    return (
	m_en_receiver == other.get_enable_receiver() &&
	m_row_address_compare_mask == other.get_row_address_compare_mask() &&
	m_en_address_out == other.get_enable_address_out() &&
	m_utilization == other.get_utilization() &&
	m_offset == other.get_offset() &&
	m_recovery == other.get_recovery() &&
	m_row_mode_bottom == other.m_row_mode_bottom &&
	m_row_mode_top == other.m_row_mode_top &&
	m_select_target_voltages == other.get_select_target_voltages() &&
	m_en_readout == other.get_enable_readout() &&
	m_en_renewing == other.get_enable_renewing() &&
	m_en_hagen_modulation == other.get_enable_hagen_modulation() &&
	m_en_stp == other.get_enable_stp() &&
	m_en_charge_sharing == other.get_enable_charge_sharing() &&
	m_en_recovery == other.get_enable_recovery()
    );
	// clang-format on
}

bool SynapseDriverConfig::operator!=(SynapseDriverConfig const& other) const
{
	return !(*this == other);
}

template <class Archive>
void SynapseDriverConfig::serialize(Archive& ar, std::uint32_t const)
{
	ar(CEREAL_NVP(m_en_receiver));
	ar(CEREAL_NVP(m_row_address_compare_mask));
	ar(CEREAL_NVP(m_en_address_out));
	ar(CEREAL_NVP(m_utilization));
	ar(CEREAL_NVP(m_offset));
	ar(CEREAL_NVP(m_recovery));
	ar(CEREAL_NVP(m_row_mode_bottom));
	ar(CEREAL_NVP(m_row_mode_top));
	ar(CEREAL_NVP(m_select_target_voltages));
	ar(CEREAL_NVP(m_en_readout));
	ar(CEREAL_NVP(m_en_renewing));
	ar(CEREAL_NVP(m_en_hagen_modulation));
	ar(CEREAL_NVP(m_en_stp));
	ar(CEREAL_NVP(m_en_charge_sharing));
	ar(CEREAL_NVP(m_en_recovery));
}

} // namespace haldls::vx::v1

EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(haldls::vx::v1::SynapseDriverConfig)
CEREAL_CLASS_VERSION(haldls::vx::v1::SynapseDriverConfig, 1)
