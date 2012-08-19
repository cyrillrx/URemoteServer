#pragma once

#include <vector>
#include <string>
#include "server_exchange.pb.h"

using namespace std;
using namespace network;

class FileManager
{
private :
	static bool AddFile(DirContent* _dirContent, string _filename, DirContent_File_FileType _type, int _size);
	static bool SetDirectoryContent(DirContent* _dirContent, string _dirPath);
	static string OpenFile(string _filePath);

public :
	static Response* HandleMessage(Request_Code _code, string _param);
};

