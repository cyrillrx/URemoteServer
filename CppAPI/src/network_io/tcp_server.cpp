#include "tcp_server.h"

#include <boost/bind.hpp>

typedef boost::asio::ip::tcp tcp;

namespace network_io
{

	tcp_server::tcp_server(const unsigned short& port, std::size_t pool_size, request_handler& handler)
		: io_service_pool_(pool_size),
		signals_(io_service_pool_.get_io_service()),
		acceptor_(io_service_pool_.get_io_service()),
		request_handler_(handler),
		new_connection_()
	{
		// Register to handle the signals that indicate when the server should exit.
		// It is safe to register for the same signal multiple times in a program,
		// provided all registration for the specified signal is made through Asio.
		signals_.add(SIGINT);
		signals_.add(SIGTERM);
#if defined(SIGQUIT)
		signals_.add(SIGQUIT);
#endif
		signals_.async_wait(boost::bind(&tcp_server::handle_stop, this));

		// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		tcp::endpoint endpoint(tcp::v4(), port);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();

		start_accept();
	}

	void tcp_server::run()
	{
		io_service_pool_.run();
	}

	void tcp_server::handle_stop()
	{
		io_service_pool_.stop();
	}

	void tcp_server::start_accept()
	{
		new_connection_.reset(new tcp_connection(io_service_pool_.get_io_service(), request_handler_));
		acceptor_.async_accept(new_connection_->socket(), boost::bind(&tcp_server::handle_accept, this, boost::asio::placeholders::error));
	}

	void tcp_server::handle_accept(const boost::system::error_code& error)
	{
		if (!error) {
			new_connection_->start();
		}

		start_accept();
	}

}