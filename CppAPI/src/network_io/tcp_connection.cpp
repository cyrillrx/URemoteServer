#include "tcp_connection.h"

#include <boost/bind.hpp>

namespace network_io
{
	
	tcp_connection::tcp_connection(boost::asio::io_service& io_service, request_handler handle_request)
		: socket_(io_service), handle_request_(handle_request) { }

	boost::asio::ip::tcp::socket& tcp_connection::socket()
	{
		return socket_;
	}

	void tcp_connection::start()
	{
		socket_.async_read_some(boost::asio::buffer(read_buffer_), boost::bind(&tcp_connection::handle_read, shared_from_this(),
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	void tcp_connection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		if (!error)	{
			auto request	= serialized_message(read_buffer_, bytes_transferred);
			auto response	= handle_request_(request);
			auto write_buffer = boost::asio::buffer(response.buffer(), response.size());
			boost::asio::async_write(socket_, write_buffer, boost::bind(&tcp_connection::handle_write, shared_from_this(), boost::asio::placeholders::error));
		}

		// If an error occurs then no new asynchronous operations are started. This
		// means that all shared_ptr references to the connection object will
		// disappear and the object will be destroyed automatically after this
		// handler returns. The connection class's destructor closes the socket.
	}

	void tcp_connection::handle_write(const boost::system::error_code& error)
	{
		if (!error) {
			// Initiate graceful connection closure.
			boost::system::error_code ignored_ec;
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		}

		// No new asynchronous operations are started. This means that all shared_ptr
		// references to the connection object will disappear and the object will be
		// destroyed automatically after this handler returns. The connection class's
		// destructor closes the socket.
	}

}
