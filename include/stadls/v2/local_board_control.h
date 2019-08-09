#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/common.h"
#include "haldls/v2/playback.h"
#include "hate/visibility.h"
#include "hate/optional.h"
#include "stadls/v2/genpybind.h"

namespace stadls {
namespace v2 GENPYBIND_TAG_STADLS_V2 {

namespace detail {

#define LAST_OCP_CONTAINER(Type) Type
#define OCP_CONTAINER(Type) LAST_OCP_CONTAINER(Type),
typedef hate::type_list<
#include "haldls/v2/ocp_container.def"
    >
    container_list;

#define LAST_OCP_CONTAINER(Type) typename Type::coordinate_type
#define OCP_CONTAINER(Type) LAST_OCP_CONTAINER(Type),
typedef hate::type_list<
#include "haldls/v2/ocp_container.def"
    >
    coordinate_list;

template <typename CoordinateT>
struct coordinate_type_to_container_type
{
	typedef typename hate::index_type_list_by_integer<
	    hate::index_type_list_by_type<CoordinateT, coordinate_list>::value,
	    container_list>::type type;
};

} // namespace detail

std::shared_ptr<haldls::v2::PlaybackProgram> get_configure_program(haldls::v2::Chip chip);

class GENPYBIND(visible) LocalBoardControl
{
public:
	/// \brief creates Flyspi communication object and calls soft_reset
	LocalBoardControl(std::string const& usb_serial_number) SYMBOL_VISIBLE;

	LocalBoardControl(LocalBoardControl&& other) noexcept SYMBOL_VISIBLE;
	LocalBoardControl& operator=(LocalBoardControl&& other) noexcept SYMBOL_VISIBLE;

	LocalBoardControl(LocalBoardControl const& other) = delete;
	LocalBoardControl& operator=(LocalBoardControl const& other) = delete;

	~LocalBoardControl() SYMBOL_VISIBLE;

	std::string usb_serial() const SYMBOL_VISIBLE;

	/// \brief toggle soft reset and chip reset and restore fpga to default config
	void soft_reset() SYMBOL_VISIBLE;

	/// \brief write ocp based asynchronous container to FPGA
	/// \param coord Coordinate of to be written container
	/// \param config To be written container
	template <class T>
	void write(typename T::coordinate_type const& coord, T const& config) SYMBOL_VISIBLE;

	/// \brief read ocp based asynchronous container from FPGA
	/// \param coord Coordinate of to be read container
	template <class CoordinateT>
	typename detail::coordinate_type_to_container_type<CoordinateT>::type read(
	    CoordinateT const& coord) SYMBOL_VISIBLE;

	void configure_static(
		std::vector<haldls::v2::ocp_address_type> const& board_addresses,
		std::vector<haldls::v2::ocp_word_type> const& board_words,
		std::vector<std::vector<haldls::v2::instruction_word_type> > const& chip_program_bytes)
		SYMBOL_VISIBLE;
	void configure_static(haldls::v2::Board const& board, haldls::v2::Chip const& chip)
		SYMBOL_VISIBLE;

	/// \brief transfers the program and sets the program size and address
	///        registers
	void transfer(std::vector<std::vector<haldls::v2::instruction_word_type> > const& program_bytes)
		SYMBOL_VISIBLE;
	void transfer(std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program)
	    SYMBOL_VISIBLE;

	/// \brief toggle the execute flag and wait until turned off again
	void execute() SYMBOL_VISIBLE;

	/// \brief toggle the execute flag and wait until turned off again
	///        given timing parameter
	/// \param min_wait_time Minimal wait time between checks of execute flag
	/// \param max_wait_time Maximal wait time between checks of execute flag
	/// \param max_wait Maximal wait time for execute flag to clear
	/// \param expected_runtime Time to wait until first check of execute flag
	///        and successive checks with exponentially increasing wait time
	void execute(
	    std::chrono::microseconds min_wait_period,
	    std::chrono::microseconds max_wait_period,
	    std::chrono::microseconds max_wait,
	    hate::optional<std::chrono::microseconds> expected_runtime = hate::nullopt) SYMBOL_VISIBLE;

	std::vector<haldls::v2::instruction_word_type> fetch() SYMBOL_VISIBLE;
	void fetch(std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program) SYMBOL_VISIBLE;

	static void decode_result_bytes(
	    std::vector<haldls::v2::instruction_word_type> const& result_bytes,
	    std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program) SYMBOL_VISIBLE;

	/// \brief this just wraps the sequence transfer-execute-fetch
	std::vector<haldls::v2::instruction_word_type> run(
		std::vector<std::vector<haldls::v2::instruction_word_type> > const& program_byte)
		SYMBOL_VISIBLE;
	void run(std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program) SYMBOL_VISIBLE;

	/// \brief Run experiment on given board and chip
	void run_experiment(
	    haldls::v2::Board const& board,
	    haldls::v2::Chip const& chip,
	    std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program) SYMBOL_VISIBLE;

	constexpr static char const* const env_name_board_id = "FLYSPI_ID";

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
}; // LocalBoardControl

std::vector<std::string> available_board_usb_serial_numbers() SYMBOL_VISIBLE GENPYBIND(visible);

#ifdef __GENPYBIND__
// Explicit instantiation of template member functions for all valid playback container types.
#define OCP_CONTAINER(Type)                                                                        \
	extern template void LocalBoardControl::write<Type>(                                           \
	    Type::coordinate_type const&, Type const&);                                                \
	extern template Type LocalBoardControl::read<typename Type::coordinate_type>(                  \
	    Type::coordinate_type const&);
#include "haldls/v2/ocp_container.def"
#endif // __GENPYBIND__

} // namespace v2
} // namespace stadls
