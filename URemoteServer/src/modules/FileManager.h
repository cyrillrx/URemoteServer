#pragma once

#include <vector>
#include <string>

#include "..\server_exchange.pb.h"
#include "fs_utils.h"

class FileManager
{
public :
	static void HandleMessage(network::Response* reply, network::Request_Code code, std::string param);

private :
	static bool AddFile(network::DirContent* dirContent, fs_utils::File& file);
	static bool GetDirectoryContent(network::Response* reply, std::string dirPath);
	static void OpenFile(std::string filePath);
};