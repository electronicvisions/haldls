#include "haldls/vx/event.h"
#include "haldls/cerealization.h"

namespace haldls::vx {

#define SpikePackToChip(Num)                                                                       \
	SpikePack##Num##ToChip::SpikePack##Num##ToChip() : m_impl() {}                                 \
                                                                                                   \
	SpikePack##Num##ToChip::SpikePack##Num##ToChip(labels_type const& labels) : m_impl(labels) {}  \
                                                                                                   \
	typename SpikePack##Num##ToChip::labels_type SpikePack##Num##ToChip::get_labels() const        \
	{                                                                                              \
		return m_impl.get_labels();                                                                \
	}                                                                                              \
                                                                                                   \
	void SpikePack##Num##ToChip::set_labels(labels_type const& value)                              \
	{                                                                                              \
		m_impl.set_labels(value);                                                                  \
	}                                                                                              \
                                                                                                   \
	bool SpikePack##Num##ToChip::operator==(SpikePack##Num##ToChip const& other) const             \
	{                                                                                              \
		return (m_impl == other.m_impl);                                                           \
	}                                                                                              \
                                                                                                   \
	bool SpikePack##Num##ToChip::operator!=(SpikePack##Num##ToChip const& other) const             \
	{                                                                                              \
		return !(*this == other);                                                                  \
	}                                                                                              \
                                                                                                   \
	std::ostream& operator<<(std::ostream& os, SpikePack##Num##ToChip const& spike_pack)           \
	{                                                                                              \
		os << spike_pack.m_impl;                                                                   \
		return os;                                                                                 \
	}                                                                                              \
                                                                                                   \
	std::array<                                                                                    \
	    halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS,                                        \
	    SpikePack##Num##ToChip::config_size_in_words>                                              \
	    SpikePack##Num##ToChip::addresses(coordinate_type const& coord) const                      \
	{                                                                                              \
		return {coord};                                                                            \
	}                                                                                              \
                                                                                                   \
	std::array<fisch::vx::SpikePack##Num##ToChip, SpikePack##Num##ToChip::config_size_in_words>    \
	    SpikePack##Num##ToChip::encode() const                                                     \
	{                                                                                              \
		return {m_impl};                                                                           \
	}                                                                                              \
                                                                                                   \
	void SpikePack##Num##ToChip::decode(                                                           \
	    std::array<fisch::vx::SpikePack##Num##ToChip, config_size_in_words> const& data)           \
	{                                                                                              \
		m_impl = data[0];                                                                          \
	}                                                                                              \
                                                                                                   \
	template <typename Archive>                                                                    \
	void SpikePack##Num##ToChip::serialize(Archive& ar)                                            \
	{                                                                                              \
		ar(CEREAL_NVP(m_impl));                                                                    \
	}                                                                                              \
                                                                                                   \
	EXPLICIT_INSTANTIATE_CEREAL_SERIALIZE(SpikePack##Num##ToChip)

// clang-format off
SpikePackToChip(1)
SpikePackToChip(2)
SpikePackToChip(3)
// clang-format on

#undef SpikePackToChip

} // namespace haldls::vx
