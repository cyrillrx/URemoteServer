#pragma once

#include <vector>
#include <string>

#include "network_io/message.pb.h"
#include "fs_utils.h"

class file_manager
{
public :
	static void handle_message(network_io::Response* reply, const network_io::Request_Code& code, const std::string& strExtra);

private :
	static bool add_file(network_io::FileInfo* directoryInfo, fs_utils::file& file);
	static bool query_children(network_io::Response* reply, std::string dirPath);
	static void open_file(const std::string& filePath);
};