#pragma once

#include "server_exchange.pb.h"

namespace network_io
{

	class serialized_message 
	{
	public :
		serialized_message(const char* buffer, const int& buffer_size)
			: buffer_(buffer), buffer_size_(buffer_size) { }

		const int size() const {
			return buffer_size_;
		}

		const char* buffer() const {
			return buffer_;
		}

	private:
		int buffer_size_;
		const char* buffer_;
	};

	Request build_request(serialized_message message);
	//Response build_response(serialized_message message);
	//serialized_message serialize_request(Request request);
	serialized_message serialize_response(Response response);

}
