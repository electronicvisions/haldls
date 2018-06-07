#pragma once
#include <vector>

#include <boost/asio.hpp>
#include <tbb/concurrent_queue.h>

#include "logging_ctrl.h"

#include "gdb_protocol.hpp"

using boost::asio::ip::tcp;

extern tbb::concurrent_queue<std::string> requests;
extern tbb::concurrent_queue<std::string> responses;

extern bool run;

/*
    TCP session accepting requests from gdb and sending responses to gdb.
*/
static void session(tcp::socket sock)
{
	auto logger = log4cxx::Logger::getLogger(__func__);

	std::vector<char> request;
	std::string response;
	char buffer[1];

	try {
		while (run) {
			boost::system::error_code error;
			if (responses.try_pop(response)) {
				// response == PPU-program stopped, now stop gdbserver
				if (response == "W0") {
					run = false;
				}
				boost::asio::write(sock, boost::asio::buffer(make_response(response)));
				LOG4CXX_INFO(logger, "Sending response to GDB: " << response);
			}
			if (sock.available()) {
				size_t const length = sock.read_some(boost::asio::buffer(buffer), error);
				if (length != 0) {
					if (buffer[0] == '$') {
						// new request started
						request.clear();
						request.push_back(buffer[0]);
					} else if (buffer[0] == '#') {
						// request ends
						request.push_back(buffer[0]);
						char checksum[2];
						sock.read_some(boost::asio::buffer(checksum), error);
						request.push_back(checksum[0]);
						request.push_back(checksum[1]);
						// we got the request, now ack
						boost::asio::write(sock, boost::asio::buffer("+"));
						// add to requests queue
						requests.push(strip_request(std::string(request.begin(), request.end())));
					} else {
						// build up request message
						request.push_back(buffer[0]);
					}
				}
			}
			if (error == boost::asio::error::eof)
				// connection closed cleanly by peer
				break;
			else if (error)
				// some other error
				throw boost::system::system_error(error);
		}
	} catch (std::exception& e) {
		LOG4CXX_ERROR(logger, "Exception in tcp server session: " << e.what());
	}
}

/*
    TCP server listening on port and creating a session, if connected.
*/
void server(unsigned short port)
{
	boost::asio::io_service io_service;
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	while (run) {
		tcp::socket sock(io_service);
		a.accept(sock);
		// only one session at a time allowed
		session(std::move(sock));
	}
}
