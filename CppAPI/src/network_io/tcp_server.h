#pragma once

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "io_service_pool.h"
#include "tcp_connection.h"

namespace network_io
{

	class tcp_server : private boost::noncopyable
	{
	public:
		/// Construct the server to listen on the specified TCP port.
		explicit tcp_server(const unsigned short& port, std::size_t pool_size, request_handler handle_request);

		/// Run the server's io_service loop.
		void run();

	private:
		/// The pool of io_service objects used to perform asynchronous operations.
		io_service_pool io_service_pool_;

		/// The signal_set is used to register for process termination notifications.
		boost::asio::signal_set signals_;

		/// Acceptor used to listen for incoming connections.
		boost::asio::ip::tcp::acceptor acceptor_;

		/// The next connection to be accepted.
		std::shared_ptr<tcp_connection> new_connection_;

		/// The handler for all incoming requests.
		request_handler& handle_request_;

		/// Initiate an asynchronous accept operation.
		void start_accept();

		/// Handle completion of an asynchronous accept operation.
		void handle_accept(const boost::system::error_code& e);

		/// Handle a request to stop the server.
		void handle_stop();
	};

}