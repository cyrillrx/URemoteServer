#pragma once

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include "serialized_message.h"

namespace network_io
{
	
	class tcp_connection
		: public std::enable_shared_from_this<tcp_connection>,
		private boost::noncopyable
	{
	public:
		/// Construct a connection with the given io_service.
		
		explicit tcp_connection(boost::asio::io_service& io_service, void (*request_handler)(const serialized_message& request, serialized_message& response));

		/// Get the socket associated with the connection.
		boost::asio::ip::tcp::socket& socket();

		/// Start the first asynchronous operation for the connection.
		void start();

	private:
		/// Socket for the connection.
		boost::asio::ip::tcp::socket socket_;

		/// The handler used to process the incoming request.
		std::function<void(const serialized_message& request, serialized_message& response)>& request_handler_;

		/// Buffer for incoming data.
		char buffer_[4096];

		/// The incoming request.
		serialized_message request_;

		/// The reply to be sent back to the client.
		serialized_message response_;

		/// Handle completion of a read operation.
		void handle_read(const boost::system::error_code& e, std::size_t bytes_transferred);
		/// Handle completion of a write operation.
		void handle_write(const boost::system::error_code& e);
	};

}
