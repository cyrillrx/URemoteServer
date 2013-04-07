#pragma once

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "serialized_message.h"
#include "request_handler.h"

#define BUFFER_SIZE 4096

namespace network_io
{

	class tcp_connection
		: public std::enable_shared_from_this<tcp_connection>, private boost::noncopyable
	{
	public:
		/// Construct a connection with the given io_service.

		explicit tcp_connection(boost::asio::io_service& io_service, request_handler& handler);

		/// Get the socket associated with the connection.
		boost::asio::ip::tcp::socket& socket();

		/// Start the first asynchronous operation for the connection.
		void start();

	private:
		/// Socket for the connection.
		boost::asio::ip::tcp::socket socket_;
		
		/// The handler for all incoming requests.
		request_handler& request_handler_;

		/// Buffer for incoming data.
		char read_buffer_[BUFFER_SIZE];

		/// Handle completion of a read operation.
		void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);

		/// Handle completion of a write operation.
		void handle_write(const boost::system::error_code& e);
	};

}
