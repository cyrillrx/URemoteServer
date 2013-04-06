#pragma once

#include <vector>
#include <string>

#include "network_io/server_exchange.pb.h"
#include "fs_utils.h"

class FileManager
{
public :
	static void HandleMessage(network_io::Response* reply, network_io::Request_Code code, const std::string& param);

private :
	static bool AddFile(network_io::DirContent* dirContent, fs_utils::file& file);
	static bool GetDirectoryContent(network_io::Response* reply, std::string dirPath);
	static void OpenFile(const std::string& filePath);
};