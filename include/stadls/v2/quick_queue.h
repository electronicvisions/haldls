#pragma once

#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

#include "rcf-extensions/round-robin.h"

#include "hate/visibility.h"

#include "stadls/v2/local_board_control.h"

namespace SF {

void serialize(SF::Archive& ar, haldls::v2::ocp_address_type& addr);
void serialize(SF::Archive& ar, haldls::v2::ocp_word_type& word);

} // namespace SF

namespace stadls {
namespace v2 GENPYBIND(tag(stadls_v2)) {
typedef std::vector<haldls::v2::ocp_address_type> ocp_addresses_type;
typedef std::vector<haldls::v2::ocp_word_type> ocp_words_type;
typedef std::vector<std::vector<haldls::v2::instruction_word_type> > program_bytes_type;

struct QuickQueueRequest
{
	ocp_addresses_type board_addresses;
	ocp_words_type board_words;
	program_bytes_type chip_program_bytes;
	program_bytes_type playback_program_bytes;

	template <class Archive>
	void serialize(Archive& archive)
	{
		serialize_detail<Archive>(archive, typename std::is_same<Archive, SF::Archive>::type());
	}

private:
	// Archive is SF::Archive
	template <class Archive>
	void serialize_detail(Archive& archive, std::true_type) SYMBOL_VISIBLE;

	// Archive is from cereal
	template <class Archive>
	void serialize_detail(Archive& archive, std::false_type) SYMBOL_VISIBLE;
};

struct QuickQueueResponse
{
	std::vector<haldls::v2::instruction_word_type> result_bytes;

	template <class Archive>
	void serialize(Archive& archive)
	{
		serialize_detail<Archive>(archive, typename std::is_same<Archive, SF::Archive>::type());
	}

private:
	// Archive is SF::Archive
	template <class Archive>
	void serialize_detail(Archive& archive, std::true_type) SYMBOL_VISIBLE;

	// Archive is from cereal
	template <class Archive>
	void serialize_detail(Archive& archive, std::false_type) SYMBOL_VISIBLE;
};

QuickQueueRequest create_request(
	haldls::v2::Board const& board, haldls::v2::Chip const& chip,
	std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program);

class QuickQueueWorker
{
public:
	/// \brief sets up the Quick Queue Server
	QuickQueueWorker(std::string const& usb_serial) SYMBOL_VISIBLE;

	QuickQueueWorker(QuickQueueWorker&& other) = default;
	QuickQueueWorker& operator=(QuickQueueWorker&& other) = default;

	QuickQueueWorker(QuickQueueWorker const& other) = delete;
	QuickQueueWorker& operator=(QuickQueueWorker const& other) = delete;

	~QuickQueueWorker() SYMBOL_VISIBLE;

	// run whenever there are any jobs to complete
	void setup() SYMBOL_VISIBLE;

	std::optional<size_t> verify_user(std::string const& user_data) SYMBOL_VISIBLE;

	QuickQueueResponse work(QuickQueueRequest const&) SYMBOL_VISIBLE;

	// run whenever there are no jobs to anymore
	void teardown() SYMBOL_VISIBLE;

	// Set or unset the worker into mock-mode.
	// If in mock-mode, no communication with any hardware is attempted and
	// empty results are returned.
	void set_mock_mode(bool mode_enable) SYMBOL_VISIBLE { m_mock_mode = mode_enable; };

private:
	// methods
	std::string get_slurm_jobname() { return "board_alloc_" + get_slurm_gres(); }
	std::string get_slurm_gres() { return m_usb_serial; }
	void get_slurm_allocation();
	void free_slurm_allocation();

	// members
	std::unique_ptr<LocalBoardControl> m_local_board_ctrl;
	std::string m_usb_serial;
	bool m_has_slurm_allocation;
	constexpr static char const* const m_env_name_partition = "SLURM_JOB_PARTITION";
	std::string m_slurm_partition;

	bool m_mock_mode;

}; // QuickQueueWorker

// generate sechduling Quick Queue Server that operates on worker
RR_GENERATE(QuickQueueWorker, QuickQueueServer)

// TODO: Decide if pImpl is really needed here! --obreitwi, 06-03-18 14:12:45
class GENPYBIND(visible) QuickQueueClient
{
public:
	/// \brief connects to server (gets ip and port from environment)
	QuickQueueClient() SYMBOL_VISIBLE;
	/// \brief connects to server with explicit ip and port
	QuickQueueClient(std::string const& ip, uint16_t port) SYMBOL_VISIBLE;

	QuickQueueClient(QuickQueueClient&& other) noexcept SYMBOL_VISIBLE;
	QuickQueueClient& operator=(QuickQueueClient&& other) noexcept SYMBOL_VISIBLE;

	QuickQueueClient(QuickQueueClient const& other) = delete;
	QuickQueueClient& operator=(QuickQueueClient const& other) = delete;

	~QuickQueueClient() SYMBOL_VISIBLE;

	void run_experiment(
		haldls::v2::Board const& board,
		haldls::v2::Chip const& chip,
		std::shared_ptr<haldls::v2::PlaybackProgram> const& playback_program) SYMBOL_VISIBLE;

	static int const max_message_length = 1280 * 1024 * 1024;
	static int const remote_call_timeout = 3600 * 1000;

	constexpr static char const* const env_name_ip = "QUIGGELDY_IP";
	constexpr static char const* const env_name_port = "QUIGGELDY_PORT";

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
}; // QuickQueueClient

} // namespace v2
} // namespace stadls

#pragma GCC diagnostic pop
