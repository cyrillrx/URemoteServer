#pragma once

#include <boost/noncopyable.hpp>

namespace network_io
{

	/// The common handler for all incoming requests.
	class request_handler : private boost::noncopyable
	{
	public:
		/// Handle a request and produce a reply.
		virtual serialized_message handle_request(serialized_message request) = 0;
	};

}