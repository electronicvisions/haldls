#include <boost/program_options.hpp>
#include "pthread.h"
#include "signal.h"

#include "logger.h"
#include "logging_ctrl.h"
#include "stadls/v2/local_board_control.h"
#include "stadls/v2/quick_queue.h"

// needed to have lambdas as signal handlers
namespace quiggeldy {
std::function<int(int)> signal_handler;

void* handle_signals_thread(void*);

pthread_t setup_signal_handler_thread()
{
	// have all threads not listen to signals
	sigset_t sigset;
	pthread_t sigthread;

	// set dummy function that ignores all signals to avoid crashing if a
	// signal is received in the short amount of time until the
	// QuickQueueServer is running
	quiggeldy::signal_handler = [](int sig) {
		auto log = log4cxx::Logger::getLogger(__func__);
		if (log->isEnabledFor(log4cxx::Level::getDebug())) {
			std::stringstream ss;
			ss << "Received signal " << sig << " in dummy signal handler, ignoring..";
			LOG4CXX_DEBUG(log, ss.str());
		}
		return 0;
	};

	sigfillset(&sigset);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);
	pthread_create(&sigthread, NULL, handle_signals_thread, NULL);

	return sigthread;
}

void* handle_signals_thread(void*)
{
	auto log = log4cxx::Logger::getLogger(__func__);

	LOG4CXX_INFO(log, "Seting up signal handler thread");

	int sig;
	sigset_t sigset;

	sigemptyset(&sigset);

	// termination request, sent to the program
	sigaddset(&sigset, SIGTERM);

	// invalid memory access (segmentation fault)
	sigaddset(&sigset, SIGSEGV);

	// external interrupt, usually initiated by the user
	sigaddset(&sigset, SIGINT);

	// invalid program image, such as invalid instruction
	sigaddset(&sigset, SIGILL);

	// abnormal termination condition, as is e.g. initiated by std::abort()
	sigaddset(&sigset, SIGABRT);

	// erroneous arithmetic operation such as divide by zero
	sigaddset(&sigset, SIGFPE);

	// continue signal with which we indicate a reset of the idle timeout
	sigaddset(&sigset, SIGCONT);

	while (true) {
		LOG4CXX_DEBUG(log, "Waiting for signal..");
		sigwait(&sigset, &sig);
		if (signal_handler(sig) != 0) {
			break;
		}
	}
	return (void*) NULL;
}

} // namespace

namespace po = boost::program_options;

int main(int argc, const char* argv[])
{
	std::string ip, usb_serial;
	uint16_t port;
	uint32_t release_seconds;
	uint32_t timeout_seconds;
	size_t log_level;
	size_t num_threads_input;
	size_t num_threads_output;
	bool mock_mode;

	po::options_description desc("Allowed options");
	desc.add_options()("help,h", "produce help message")(
		"ip,i", po::value<std::string>(&ip)->default_value("0.0.0.0"), "specify listening IP")(
		"port,p", po::value<uint16_t>(&port)->required(), "specify listening port")(
		"usb,u", po::value<std::string>(&usb_serial)->required(),
		"specify USB serial for HICANN board")(
		"release,r", po::value<uint32_t>(&release_seconds)->default_value(600),
		"Number of seconds between releases of slurm allocation")(
		"timeout,t", po::value<uint32_t>(&timeout_seconds)->default_value(0),
		"Number of seconds after which quiggeldy shuts down when idling (0=disable).")(
		"loglevel,l", po::value<size_t>(&log_level)->default_value(1),
		"specify loglevel [0-ERROR,1-WARNING,2-INFO,3-DEBUG,4-TRACE]")(
		"num-threads-input,n", po::value<size_t>(&num_threads_input)->default_value(8),
		"Number of threads handling incoming connections.")(
		"num-threads-outputs,m", po::value<size_t>(&num_threads_output)->default_value(8),
		"Number of threads handling distribution of results.")(
		"mock-mode", po::bool_switch(&mock_mode)->default_value(false),
		"Operate in mock-mode, i.e., accept connections but return empty results.");

	// populate vm variable
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return EXIT_FAILURE;
	}
	po::notify(vm);

	logger_default_config(Logger::log4cxx_level(log_level));
	auto log = log4cxx::Logger::getLogger("quiggeldy");

	LOG4CXX_INFO(log, "Starting up..");

	// has to be called prior to QuickQueueServer
	auto sig_thread = quiggeldy::setup_signal_handler_thread();

	std::unique_ptr<stadls::v2::QuickQueueServer> server;

	{
		auto worker = stadls::v2::QuickQueueWorker(usb_serial);
		if (mock_mode) {
			LOG4CXX_INFO(log, "Setting mock-mode.");
		}
		worker.set_mock_mode(mock_mode);
		server.reset(new stadls::v2::QuickQueueServer(
			RCF::TcpEndpoint(ip, port), std::move(worker), num_threads_input, num_threads_output));
	}

	// we want to release a possible slurm allocation if the program fails under any circumstances
	quiggeldy::signal_handler = [&server, &log](int sig) {
		if (log->isEnabledFor(log4cxx::Level::getDebug())) {
			std::stringstream ss;
			ss << "Received signal " << sig << " in signal handler..";
			LOG4CXX_DEBUG(log, ss.str());
		}
		switch (sig) {
			case SIGCONT:
				if (log->isEnabledFor(log4cxx::Level::getDebug())) {
					LOG4CXX_DEBUG(log, "Resetting idle timeout!");
				}
				server->reset_idle_timeout();
				return 0;
			default:
				if (log->isEnabledFor(log4cxx::Level::getDebug())) {
					LOG4CXX_DEBUG(log, "Shutting down server!");
				}
				server->shutdown(); // worker teardown handled in shutdown
				return -1;
		}
	};

	// Set max message length to the same amount as in client
	server->get_server().getServerTransport().setMaxIncomingMessageLength(
		stadls::v2::QuickQueueClient::max_message_length);
	server->set_release_interval(std::chrono::seconds(release_seconds));

	LOG4CXX_INFO(log, "Quiggeldy set up!");
	server->start_server(std::chrono::seconds(timeout_seconds));
	LOG4CXX_INFO(log, "Quiggeldy shutting down due to idle timeout.");

	// cancel sighandler thread if it is still runningj
	pthread_cancel(sig_thread);
	return 0;
}
