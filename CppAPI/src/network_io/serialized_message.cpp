#include "serialized_message.h"

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

}