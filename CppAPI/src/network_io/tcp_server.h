#pragma once

namespace network_io
{

class tcp_server
{
public:
	tcp_server(const unsigned short& port);
	~tcp_server();
};

}