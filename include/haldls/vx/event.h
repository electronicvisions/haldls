#pragma once
#include "fisch/vx/event.h"
#include "haldls/vx/genpybind.h"
#include "haldls/vx/traits.h"

namespace cereal {
class access;
} // namespace cereal

namespace haldls::vx GENPYBIND_TAG_HALDLS_VX {

typedef fisch::vx::SpikeLabel SpikeLabel GENPYBIND(opaque(false));
typedef fisch::vx::SpikeFromChip SpikeFromChip GENPYBIND(opaque(false));
typedef fisch::vx::MADCSampleFromChip MADCSampleFromChip GENPYBIND(opaque(false));
typedef fisch::vx::SpikeFromChipEvent SpikeFromChipEvent GENPYBIND(opaque(false));
typedef fisch::vx::MADCSampleFromChipEvent MADCSampleFromChipEvent GENPYBIND(opaque(false));

#define SpikePackToChip(Num)                                                                       \
	class GENPYBIND(visible) SpikePack##Num##ToChip                                                \
	{                                                                                              \
	public:                                                                                        \
		typedef halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS coordinate_type;                \
		typedef std::true_type is_leaf_node;                                                       \
                                                                                                   \
		typedef typename fisch::vx::SpikePack##Num##ToChip::labels_type labels_type;               \
                                                                                                   \
		/** Default constructor. */                                                                \
		SpikePack##Num##ToChip() SYMBOL_VISIBLE;                                                   \
                                                                                                   \
		/**                                                                                        \
		 * Construct spike pack with labels.                                                       \
		 * @param labels Array of SpikeLabel values to use                                         \
		 */                                                                                        \
		SpikePack##Num##ToChip(labels_type const& labels) SYMBOL_VISIBLE;                          \
                                                                                                   \
		/**                                                                                        \
		 * Get spike labels.                                                                       \
		 * @return Array of SpikeLabel                                                             \
		 */                                                                                        \
		GENPYBIND(getter_for(labels))                                                              \
		labels_type get_labels() const SYMBOL_VISIBLE;                                             \
                                                                                                   \
		/**                                                                                        \
		 * Set spike labels.                                                                       \
		 * @param value Array of SpikeLabel to set                                                 \
		 */                                                                                        \
		GENPYBIND(setter_for(labels))                                                              \
		void set_labels(labels_type const& value) SYMBOL_VISIBLE;                                  \
                                                                                                   \
		bool operator==(SpikePack##Num##ToChip const& other) const SYMBOL_VISIBLE;                 \
		bool operator!=(SpikePack##Num##ToChip const& other) const SYMBOL_VISIBLE;                 \
                                                                                                   \
		GENPYBIND(stringstream)                                                                    \
		friend std::ostream& operator<<(std::ostream& os, SpikePack##Num##ToChip const& config)    \
		    SYMBOL_VISIBLE;                                                                        \
                                                                                                   \
		constexpr static size_t GENPYBIND(hidden) config_size_in_words = 1;                        \
		std::array<halco::hicann_dls::vx::SpikePack##Num##ToChipOnDLS, config_size_in_words>       \
		addresses(coordinate_type const& word) const SYMBOL_VISIBLE GENPYBIND(hidden);             \
		std::array<fisch::vx::SpikePack##Num##ToChip, config_size_in_words> encode() const         \
		    SYMBOL_VISIBLE GENPYBIND(hidden);                                                      \
		void decode(std::array<fisch::vx::SpikePack##Num##ToChip, config_size_in_words> const&     \
		                data) SYMBOL_VISIBLE GENPYBIND(hidden);                                    \
                                                                                                   \
	private:                                                                                       \
		friend class cereal::access;                                                               \
		template <class Archive>                                                                   \
		void serialize(Archive& ar) SYMBOL_VISIBLE;                                                \
                                                                                                   \
		fisch::vx::SpikePack##Num##ToChip m_impl;                                                  \
	};                                                                                             \
                                                                                                   \
	namespace detail {                                                                             \
                                                                                                   \
	template <>                                                                                    \
	struct BackendContainerTrait<SpikePack##Num##ToChip>                                           \
	    : public BackendContainerBase<SpikePack##Num##ToChip, fisch::vx::SpikePack##Num##ToChip>   \
	{};                                                                                            \
                                                                                                   \
	} // namespace detail

// clang-format off
SpikePackToChip(1)
SpikePackToChip(2)
SpikePackToChip(3)
// clang-format on

#undef SpikePackToChip

} // namespace haldls::vx
