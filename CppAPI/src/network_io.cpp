#include "network_io.h"

#include <boost/asio/ip/host_name.hpp>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

#include "exception/socket_exception.h"

namespace network_io {

	Request build_request(serialized_message message)
	{
		Request request;
		//auto* request = new Request();
		// Read varint delimited protobuf object in the buffer
		google::protobuf::io::ArrayInputStream arrayInputStream(message.buffer(), message.size());
		google::protobuf::io::CodedInputStream codedInputStream(&arrayInputStream);

		google::protobuf::uint32 size;
		codedInputStream.ReadVarint32(&size);
		google::protobuf::io::CodedInputStream::Limit msgLimit = codedInputStream.PushLimit(size);

		request.ParseFromCodedStream(&codedInputStream);
		codedInputStream.PopLimit(msgLimit);

		return request;
	}

	serialized_message serialize_response(Response response)
	{
		// Build a buffer that can hold message and room for a 32bit delimiter
		int bufSize	= response.ByteSize() + 4;
		char* buf	= new char [bufSize];

		// Write varint delimiter to the buffer
		google::protobuf::io::ArrayOutputStream arrayOutputStream(buf, bufSize);
		google::protobuf::io::CodedOutputStream codedOutputStream(&arrayOutputStream);
		codedOutputStream.WriteVarint32(response.ByteSize());

		// Write response to the buffer
		response.SerializeToCodedStream(&codedOutputStream);

		serialized_message message(buf, bufSize);
		return message;
	}

	std::string hostname()
	{
		try {
			return boost::asio::ip::host_name();

		} catch (const boost::system::error_code& e) {
			throw socket_exception("network_io::hostname()", e.message());
		}
	}

	std::string ip_address(const std::string& hostname)
	{
		std::string ipAddress = "";
		struct hostent *host = gethostbyname(hostname.c_str());
		if (host == 0) {
			throw socket_exception("network_io::ip_address()", "Bad host lookup.");
		}

		struct in_addr addr;
		memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
		ipAddress = inet_ntoa(addr);

		return ipAddress;
	}
}