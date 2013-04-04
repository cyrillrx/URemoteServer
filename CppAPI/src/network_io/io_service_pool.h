#pragma once

#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace network_io
{

	class io_service_pool : private boost::noncopyable
	{
	public:
		explicit io_service_pool(std::size_t pool_size);

		/// Run all io_service objects in the pool.
		void run();

		/// Stop all io_service objects in the pool.
		void stop();

		/// Get an io_service to use.
		boost::asio::io_service& get_io_service();

	private:
		typedef std::shared_ptr<boost::asio::io_service> io_service_ptr;
		typedef std::shared_ptr<boost::asio::io_service::work> work_ptr;

		/// The pool of io_services.
		std::vector<io_service_ptr> io_services_;

		/// The work that keeps the io_services running.
		std::vector<work_ptr> work_;

		/// The next io_service to use for a connection.
		std::size_t next_io_service_;
	};

}