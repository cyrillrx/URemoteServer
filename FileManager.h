#pragma once

#include <vector>
#include <string>
#include "directory_content.pb.h"

using namespace std;
using namespace network;

class FileManager
{
private:
public:
	static ProtoDirContent* GetDirectoryContent(string _dirPath);
	//static string ListFilesStr(string _dirPath);
	static string OpenFile(string _filePath);
};

