#pragma once

#include <string>
#include "network_io.h"
#include "network_io/message.pb.h"
#include "network_io/serialized_message.h"
#include "network_io/request_handler.h"
#include "AI.h"

class executor : public network_io::request_handler
{
public :
	explicit executor(std::shared_ptr<AI> ai);
	network_io::serialized_message handle_request(network_io::serialized_message serializedRequest);

private :
	std::shared_ptr<AI> ai_;

	void classic_command(network_io::Response& reply,	const network_io::Request_Code& code, const std::string& securityToken);
	void volume_command(network_io::Response& reply,	const network_io::Request_Code& code, const int& intParam);
	void ai_command(network_io::Response& reply,		const network_io::Request_Code& code);
	void app_command(network_io::Response& reply,		const network_io::Request_Code& code);
	void shutdown_pc(network_io::Response& reply,		const int& delay);
};