#include "stadls/v2/quick_queue.h"

#include <SF/vector.hpp>

#include <chrono>
#include <cstdlib>
#include <sstream>
#include <thread>
#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <sys/wait.h>

#include "flyspi-rw_api/flyspi_com.h"
#include "halco/common/iter_all.h"
#include "log4cxx/logger.h"
#include "uni/bytewise_output_iterator.h"
#include "uni/decoder.h"
#include "uni/program_builder.h"

#include "haldls/exception/exceptions.h"
#include "haldls/v2/board.h"
#include "haldls/v2/chip.h"
#include "haldls/v2/common.h"
#include "haldls/v2/playback.h"
#include "haldls/v2/spike.h"
#include "stadls/v2/local_board_control.h"
#include "stadls/v2/ocp.h"
#include "stadls/visitors.h"

namespace SF {

void serialize(SF::Archive& ar, haldls::v2::ocp_address_type& addr)
{
	ar& addr.value;
}

void serialize(SF::Archive& ar, haldls::v2::ocp_word_type& word)
{
	ar& word.value;
}
} // SF


namespace stadls {
namespace v2 {

template <class Archive>
void QuickQueueRequest::serialize_detail(Archive& archive, std::false_type)
{
	archive(CEREAL_NVP(board_addresses));
	archive(CEREAL_NVP(board_words));
	archive(CEREAL_NVP(chip_program_bytes));
	archive(CEREAL_NVP(playback_program_bytes));
}

template <class Archive>
void QuickQueueRequest::serialize_detail(Archive& ar, std::true_type)
{
	ar& board_addresses& board_words& chip_program_bytes& playback_program_bytes;
}

template <class Archive>
void QuickQueueResponse::serialize_detail(Archive& archive, std::false_type)
{
	archive(CEREAL_NVP(result_bytes));
}

template <class Archive>
void QuickQueueResponse::serialize_detail(Archive& ar, std::true_type)
{
	ar& result_bytes;
}

// excplicit instantiation to fix build problems with jenkins (builds locally but fails with missing
// symbols during linker step on jenkins without these two lines)
// TODO: investigate
template void QuickQueueRequest::serialize_detail<SF::Archive>(SF::Archive& ar, std::true_type);
template void QuickQueueResponse::serialize_detail<SF::Archive>(SF::Archive& ar, std::true_type);


QuickQueueRequest create_request(
	haldls::v2::Board const& board,
	haldls::v2::Chip const& chip,
	haldls::v2::PlaybackProgram& playback_program)
{
	QuickQueueRequest req;

	halco::common::Unique const coord;
	visit_preorder(board, coord, WriteAddressVisitor<ocp_addresses_type>{req.board_addresses});
	visit_preorder(board, coord, EncodeVisitor<ocp_words_type>{req.board_words});

	req.chip_program_bytes = get_configure_program(chip).instruction_byte_blocks();
	req.playback_program_bytes = playback_program.instruction_byte_blocks();
	return req;
}


QuickQueueWorker::QuickQueueWorker(std::string const& usb_serial)
	: m_usb_serial(usb_serial), m_has_slurm_allocation(false)
{
	char const* env_partition = std::getenv(m_env_name_partition);
	if (env_partition == nullptr) {
		m_slurm_partition = "dls"; // TODO put default somewhere else
	} else {
		m_slurm_partition = env_partition;
	}
}

QuickQueueWorker::~QuickQueueWorker() = default;

void QuickQueueWorker::get_slurm_allocation()
{
	// prevent error if we already have slurm allocation
	if (m_has_slurm_allocation) {
		return;
	}
	auto log = log4cxx::Logger::getLogger("QuickQueueWorker");
	if (log->isEnabledFor(log4cxx::Level::getDebug())) {
		std::stringstream ss;
		ss << "Getting slurm allocation for " << get_slurm_jobname().c_str() << ".";
		LOG4CXX_DEBUG(log, ss.str());
	}
	if (log->isEnabledFor(log4cxx::Level::getDebug())) {
		std::stringstream ss;
		ss << "Running: /usr/local/bin/salloc salloc -p " << m_slurm_partition.c_str()
		   << " --no-shell --mem 0 --gres " << get_slurm_gres().c_str() << " -J "
		   << get_slurm_jobname().c_str();

		LOG4CXX_DEBUG(log, ss.str());
	}
	int pid, status;

	pid = fork();
	if (pid) {
		waitpid(pid, &status, 0); // wait for the child to exit
	} else {
		execl(
			"/usr/local/bin/salloc", "salloc", "-p", m_slurm_partition.c_str(), "--no-shell",
			"--gres", get_slurm_gres().c_str(), "--mem", "0M", "-J", get_slurm_jobname().c_str(),
			NULL);
		exit(0);
	}

	if (status) {
		throw std::logic_error("slurm allocation failed");
	}
	m_has_slurm_allocation = true;
	if (log->isEnabledFor(log4cxx::Level::getDebug())) {
		std::stringstream ss;
		ss << "Slurm allocation for " << get_slurm_jobname().c_str() << " successfully acquired.";
		LOG4CXX_DEBUG(log, ss.str());
	}
}

void QuickQueueWorker::setup()
{
	get_slurm_allocation();
	auto log = log4cxx::Logger::getLogger("QuickQueueWorker");
	LOG4CXX_DEBUG(log, "Setting up LocalBoardControl.");
	// TODO have the experiment control timeout (e.g. when the board is unresponsive)
	m_local_board_ctrl.reset(new LocalBoardControl(m_usb_serial));
	LOG4CXX_DEBUG(log, "SetUp completed!");
}

void QuickQueueWorker::teardown()
{
	m_local_board_ctrl.reset();
	free_slurm_allocation();

	auto log = log4cxx::Logger::getLogger("QuickQueueWorker");
	LOG4CXX_DEBUG(log, "TearDown completed!");
}

void QuickQueueWorker::free_slurm_allocation()
{
	// prevent error if we do not have any slurm allocation
	if (!m_has_slurm_allocation) {
		return;
	}
	auto log = log4cxx::Logger::getLogger("QuickQueueWorker");
	if (log->isEnabledFor(log4cxx::Level::getDebug())) {
		std::stringstream ss;
		ss << "Freeing slurm allocation for " << get_slurm_jobname().c_str() << ".";
		LOG4CXX_DEBUG(log, ss.str());
	}
	int pid, status;
	pid = fork();

	if (pid) {
		waitpid(pid, &status, 0); // wait for the child to exit
	} else {
		execl("/usr/local/bin/scancel", "scancel", "-n", get_slurm_jobname().c_str(), NULL);
		exit(0);
	}
	if (status) {
		throw std::logic_error("free slurm allocation failed");
	}
	m_has_slurm_allocation = false;
	if (log->isEnabledFor(log4cxx::Level::getDebug())) {
		std::stringstream ss;
		ss << "Slurm allocation for " << get_slurm_jobname().c_str() << " successfully freed.";
		LOG4CXX_DEBUG(log, ss.str());
	}
}

QuickQueueResponse QuickQueueWorker::work(QuickQueueRequest const& req)
{
	auto log = log4cxx::Logger::getLogger("QuickQueueWorker");
	LOG4CXX_DEBUG(log, "Running experiment!");

	m_local_board_ctrl->configure_static(
		req.board_addresses, req.board_words, req.chip_program_bytes);
	QuickQueueResponse response;
	try {
		response.result_bytes = m_local_board_ctrl->run(req.playback_program_bytes);
	} catch (const rw_api::LogicError& e) {
		// TODO: Power cycle board
		teardown();
		LOG4CXX_ERROR(log, "FPGA seems to be hung.");
		throw;
	}
	return response;
}

bool QuickQueueWorker::verify_user(std::string const& user_data)
{
	// TODO: implement REAL user verification --obreitwi, 20-03-18 16:57:43
	std::stringstream ss;
	ss << "Verifying " << user_data;
	auto log = log4cxx::Logger::getLogger("QuickQueueWorker");
	LOG4CXX_DEBUG(log, ss.str());
	return true;
}


struct QuickQueueClient::Impl
{
	Impl();
	Impl(const std::string& std, uint16_t port);
	~Impl();

	void setup_client(const std::string& std, uint16_t port);

	typedef typename QuickQueueServer::rcf_interface_t rcf_interface_t;

	std::unique_ptr<RcfClient<rcf_interface_t> > m_client;

	constexpr static char const* const env_name_ip = "QUIGGELDY_IP";
	constexpr static char const* const env_name_port = "QUIGGELDY_PORT";
};

QuickQueueClient::Impl::Impl()
{
	char const* env_ip = std::getenv(env_name_ip);
	if (env_ip == nullptr) {
		throw std::logic_error(std::string(env_name_ip) + " not set.");
	}
	char const* env_port = std::getenv(env_name_port);
	if (env_port == nullptr) {
		throw std::logic_error(std::string(env_name_port) + " not set.");
	}

	std::string ip(env_ip);
	uint16_t port((uint16_t) atoi(env_port));

	setup_client(ip, port);
}

QuickQueueClient::Impl::Impl(const std::string& ip, uint16_t port)
{
	setup_client(ip, port);
}

void QuickQueueClient::Impl::setup_client(const std::string& ip, uint16_t port)
{
	auto log = log4cxx::Logger::getLogger("QuickQueueClient");
	if (log->isEnabledFor(log4cxx::Level::getDebug())) {
		std::stringstream ss;
		ss << "Connecting to " << ip << ":" << port;

		LOG4CXX_DEBUG(log, ss.str());
	}

	RCF::init();
	m_client.reset(new RcfClient<rcf_interface_t>(RCF::TcpEndpoint(ip, port)));

	m_client->getClientStub().getTransport().setMaxIncomingMessageLength(max_message_length);
	// TODO: How long should we wait for an experiment to finish?
	m_client->getClientStub().setRemoteCallTimeoutMs(remote_call_timeout);

	// TODO: retrieve token from slurm that can be verified on the server side
	// for now it is just enough that this is a random string identifying each
	// user
	m_client->getClientStub().setRequestUserData(
		std::string(std::getenv("USER")) + "-without-authentication");
}

QuickQueueClient::Impl::~Impl()
{
	// client has to be destructed before we deinitialize!
	m_client.reset();
	RCF::deinit();
}

QuickQueueClient::QuickQueueClient() : m_impl(new Impl()) {}

QuickQueueClient::QuickQueueClient(const std::string& address, uint16_t port)
	: m_impl(new Impl(address, port))
{}

QuickQueueClient::QuickQueueClient(QuickQueueClient&&) noexcept = default;

QuickQueueClient& QuickQueueClient::operator=(QuickQueueClient&&) noexcept = default;

QuickQueueClient::~QuickQueueClient() {}

void QuickQueueClient::run_experiment(
	haldls::v2::Board const& board,
	haldls::v2::Chip const& chip,
	haldls::v2::PlaybackProgram& playback_program)
{
	auto log = log4cxx::Logger::getLogger("QuickQueueClient");

	QuickQueueRequest req = create_request(board, chip, playback_program);
	QuickQueueResponse response;

	// TODO make adjustable
	size_t max_connection_attempts = 10;
	size_t wait_after_connection_attempt_secs = 1;

	for (size_t num_connection_attempts = 0; num_connection_attempts < max_connection_attempts;
		 ++num_connection_attempts) {
		// build request and send it to server
		try {
			response = m_impl->m_client->submit_work(req);
			break;
		} catch (const RCF::Exception& e) {
			if (e.getErrorId() != RCF::RcfError_ClientConnectFail ||
				num_connection_attempts == max_connection_attempts - 1) {
				// reraise if something unexpected happened or we reached the
				// maximum number of tries
				throw;
			}
		}

		std::stringstream ss;
		ss << "Server not ready yet, waiting.. [Attempt: " << num_connection_attempts + 1 << "/"
		   << max_connection_attempts << "]";

		LOG4CXX_INFO(log, ss.str());
		std::this_thread::sleep_for(std::chrono::seconds(wait_after_connection_attempt_secs));
	}

	// decode received bytes
	LocalBoardControl::decode_result_bytes(response.result_bytes, playback_program);
}

} // namespace v2
} // namespace stadls
