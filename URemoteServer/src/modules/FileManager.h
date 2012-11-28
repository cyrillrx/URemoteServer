#pragma once

#include <vector>
#include <string>
#include "..\server_exchange.pb.h"

class FileManager
{
public :
	static void HandleMessage(network::Response* reply, network::Request_Code code, std::string param);

private :
	static bool AddFile(network::DirContent* dirContent, std::string filename, network::DirContent_File_FileType type, int size);
	static bool GetDirectoryContent(network::Response* reply, std::string dirPath);
	static std::string OpenFile(std::string filePath);
};