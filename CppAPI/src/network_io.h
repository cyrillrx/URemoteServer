#pragma once

//#include "platform_config.h"
//# if defined(WINDOWS_PLATFORM)
//#	include "windows/network_io.h"
//# else
//#	include "unix/network_io.h"
//# endif

#include <string>
#include "network_io/server_exchange.pb.h"

namespace network_io
{

	class serialized_message 
	{
	public :
		serialized_message(const char* buffer, const int& buffer_size)
			: m_buffer(buffer), m_buffer_size(buffer_size) { }

		const int size() const {
			return m_buffer_size;
		}
		
		const char* buffer() const {
			return m_buffer;
		}

	private:
		int m_buffer_size;
		const char* m_buffer;
	};

	Request build_request(serialized_message message);
	//Response build_response(serialized_message message);
	//serialized_message serialize_request(Request request);
	serialized_message serialize_response(Response response);

	std::string hostname();
	std::string ip_address(const std::string& hostname);
}

