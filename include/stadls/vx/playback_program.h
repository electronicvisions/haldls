#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <boost/variant.hpp>

#include "fisch/vx/container_cast.h"
#include "fisch/vx/container_ticket.h"
#include "fisch/vx/playback_program.h"
#include "haldls/vx/common.h"
#include "haldls/vx/event.h"
#include "hate/type_list.h"
#include "hate/visibility.h"
#include "hxcomm/vx/target.h"
#include "stadls/vx/genpybind.h"
#include "stadls/vx/run_time_info.h"

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>
#endif

namespace stadls::vx GENPYBIND_TAG_STADLS_VX {

namespace detail {

template <typename, typename, template <typename> class>
class PlaybackProgramBuilderAdapter;
template <typename, typename, template <typename> class>
class PlaybackProgramBuilderAdapterImpl;

template <typename BackendContainerTypeList>
struct to_ticket_variant;

template <typename... BackendContainer>
struct to_ticket_variant<hate::type_list<BackendContainer...>>
{
	typedef boost::variant<fisch::vx::ContainerTicket<decltype(
	    fisch::vx::container_cast(std::declval<BackendContainer>()))>...>
	    type;
};

} // namespace detail

class PlaybackProgram;

class ReinitStackEntry;

template <typename Connection>
RunTimeInfo run(Connection&, PlaybackProgram&);

/**
 * Sequential stream of executable instructions for the executor and result-container for event
 * response data during execution.
 */
class GENPYBIND(visible) PlaybackProgram
{
public:
	typedef fisch::vx::FPGATime fpga_time_type;

	/**
	 * Ticket for to-be-available container data corresponding to a read instruction.
	 * @tparam T Container type
	 */
	template <typename T>
	class ContainerTicket
	{
	public:
		typedef typename T::coordinate_type coordinate_type;

		/**
		 * Get container data if available.
		 * @throws std::runtime_error On container data not available yet
		 * @return Container data
		 */
		T get() const SYMBOL_VISIBLE;

		/**
		 * Get whether container data is available.
		 * @return Boolean value
		 */
		bool valid() const SYMBOL_VISIBLE;

		/**
		 * Get coordinate corresponding to location of (to-be) read container data.
		 * @return Coordinate value
		 */
		coordinate_type get_coordinate() const SYMBOL_VISIBLE;

		/**
		 * Get FPGA executor timestamp of last container response if time annotation is enabled.
		 * If time annotation is not enabled, get message count since last time annotation or
		 * from the beginning of the response stream.
		 * @return FPGATime value
		 */
		GENPYBIND(getter_for(fpga_time))
		fpga_time_type get_fpga_time() const SYMBOL_VISIBLE;

	private:
		typedef typename detail::to_ticket_variant<
		    typename haldls::vx::detail::BackendContainerTrait<T>::container_list>::type
		    ticket_impl_type;

		template <typename, typename, template <typename> class>
		friend class detail::PlaybackProgramBuilderAdapterImpl;

		ContainerTicket(coordinate_type const& coord, ticket_impl_type ticket_impl) :
		    m_coord(coord), m_ticket_impl(std::move(ticket_impl))
		{}

		coordinate_type m_coord;
		ticket_impl_type m_ticket_impl;
	}; // ContainerTicket


	/** Default constructor. */
	PlaybackProgram() SYMBOL_VISIBLE;

	typedef std::vector<haldls::vx::SpikeFromChip> spikes_type;
	typedef std::vector<haldls::vx::MADCSampleFromChip> madc_samples_type;
	typedef std::vector<haldls::vx::HighspeedLinkNotification> highspeed_link_notifications_type;

	typedef fisch::vx::PlaybackProgram::spike_pack_counts_type spike_pack_counts_type
	    GENPYBIND(visible);
	typedef fisch::vx::PlaybackProgram::madc_sample_pack_counts_type madc_sample_pack_counts_type
	    GENPYBIND(visible);

	/**
	 * Get vector of time-annotated spike events.
	 * @return Vector of spike events
	 */
	GENPYBIND(getter_for(spikes))
	spikes_type get_spikes() const SYMBOL_VISIBLE;

	/**
	 * Get vector of time-annotated MADC sample events.
	 * @return Vector of sample events
	 */
	GENPYBIND(getter_for(madc_samples))
	madc_samples_type get_madc_samples() const SYMBOL_VISIBLE;

	/**
	 * Get vector of time-annotated highspeed-link notifications.
	 * @return Vector of notifications
	 */
	GENPYBIND(getter_for(highspeed_link_notifications))
	highspeed_link_notifications_type get_highspeed_link_notifications() const SYMBOL_VISIBLE;

	/**
	 * Get number of occurences of spike packing from chip.
	 * @return Array of packing occurences
	 */
	GENPYBIND(getter_for(spikes_pack_counts))
	spike_pack_counts_type const& get_spikes_pack_counts() const SYMBOL_VISIBLE;

	/**
	 * Get number of occurences of MADC sample packing from chip.
	 * @return Array of packing occurences
	 */
	GENPYBIND(getter_for(madc_samples_pack_counts))
	madc_sample_pack_counts_type const& get_madc_samples_pack_counts() const SYMBOL_VISIBLE;

	/**
	 * Get set of unsupported targets.
	 * @return Set of target restictions
	 */
	GENPYBIND(getter_for(unsupported_targets))
	std::unordered_set<hxcomm::vx::Target> const& get_unsupported_targets() const SYMBOL_VISIBLE;

	GENPYBIND(stringstream)
	friend std::ostream& operator<<(std::ostream& os, PlaybackProgram const& program)
	    SYMBOL_VISIBLE;

	/**
	 * Get whether program is empty, i.e. does not feature any commands to be sent.
	 * @return Boolean value
	 */
	bool empty() const SYMBOL_VISIBLE;

	bool operator==(PlaybackProgram const& other) const SYMBOL_VISIBLE;
	bool operator!=(PlaybackProgram const& other) const SYMBOL_VISIBLE;

	/**
	 * Get spikes as 2D matrix, i.e. numpy array(s).
	 *
	 * @note We expose the data as a flat numpy DTYPE with the same data layout
	 *       as the underlying SpikeFromChipDType type.
	 */

	GENPYBIND_MANUAL({
		PYBIND11_NUMPY_DTYPE(
		    haldls::vx::SpikeFromChip::SpikeFromChipDType, label, fpga_time, chip_time);

		// expose spikes_type with pybinds11 STL vector thingy
		pybind11::bind_vector<stadls::vx::PlaybackProgram::spikes_type>(parent, "spikes_type");
		{
			auto attr = parent.attr("spikes_type");
			auto ism = parent->py::is_method(attr);

			/**
			 * This version exposes with 3 columns:
			 * chip time, neuron label, and spl1 address
			 */
			typedef ::stadls::vx::PlaybackProgram::spikes_type _values_type;
			attr.attr("to_numpy") = parent->py::cpp_function(
			    [](_values_type const& self) {
				    pybind11::array_t<haldls::vx::SpikeFromChip::SpikeFromChipDType> ret(
				        {static_cast<pybind11::ssize_t>(self.size())},
				        reinterpret_cast<haldls::vx::SpikeFromChip::SpikeFromChipDType const*>(
				            self.data()));
				    return ret;
			    },
			    ism);
		}
	})

	/**
	 * Get MADC samples as 2D matrix, i.e. numpy array(s).
	 *
	 * @note We expose the data as a flat numpy DTYPE with the same data layout
	 *       as the underlying MADCSampleFromChipDType type.
	 */

	GENPYBIND_MANUAL({
		PYBIND11_NUMPY_DTYPE(
		    haldls::vx::MADCSampleFromChip::MADCSampleFromChipDType, value, fpga_time, chip_time);

		// expose madc_samples_type with pybinds11 STL vector thingy
		pybind11::bind_vector<stadls::vx::PlaybackProgram::madc_samples_type>(
		    parent, "madc_samples_type");
		{
			typedef ::stadls::vx::PlaybackProgram::madc_samples_type _values_type;
			auto attr = parent.attr("madc_samples_type");
			auto ism = parent->py::is_method(attr);

			auto const to_numpy = [](_values_type const& self) {
				pybind11::array_t<haldls::vx::MADCSampleFromChip::MADCSampleFromChipDType> ret(
				    {static_cast<pybind11::ssize_t>(self.size())},
				    reinterpret_cast<
				        haldls::vx::MADCSampleFromChip::MADCSampleFromChipDType const*>(
				        self.data()));
				return ret;
			};

			attr.attr("to_numpy") = parent->py::cpp_function(to_numpy, ism);
		}
	})

private:
	template <typename, typename, template <typename> class>
	friend class stadls::vx::detail::PlaybackProgramBuilderAdapter;

	template <typename, typename, template <typename> class>
	friend class stadls::vx::detail::PlaybackProgramBuilderAdapterImpl;

	template <typename Connection>
	friend RunTimeInfo run(Connection&, PlaybackProgram&);

	friend ReinitStackEntry;

	/**
	 * Construct PlaybackProgram from implementation.
	 * Used in PlaybackProgramBuilderAdapter
	 * @param program_impl Implementation playback program
	 * @param unsupported_targets Build-imposed restrictions on targets
	 */
	PlaybackProgram(
	    std::shared_ptr<fisch::vx::PlaybackProgram> const& program_impl,
	    std::unordered_set<hxcomm::vx::Target> unsupported_targets) SYMBOL_VISIBLE;

	std::shared_ptr<fisch::vx::PlaybackProgram> m_program_impl;

	std::unordered_set<hxcomm::vx::Target> m_unsupported_targets;
};

} // namespace stadls::vx

#if defined(__GENPYBIND__) or defined(__GENPYBIND_GENERATED__)
// disable pybind11's automatic conversion to python types via its `list_caster`
PYBIND11_MAKE_OPAQUE(stadls::vx::PlaybackProgram::spikes_type)
PYBIND11_MAKE_OPAQUE(stadls::vx::PlaybackProgram::madc_samples_type)
#endif
