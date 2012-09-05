#ifndef FILE_MANAGER_INCLUDED
#define FILE_MANAGER_INCLUDED

#include <vector>
#include <string>
#include "server_exchange.pb.h"

using namespace std;
using namespace network;

class FileManager
{
private :
	static bool AddFile(DirContent* _dirContent, string _filename, DirContent_File_FileType _type, int _size);
	static bool GetDirectoryContent(Response* _reply, string _dirPath);
	static string OpenFile(string _filePath);

public :
	static void HandleMessage(Response* _reply, Request_Code _code, string _param);
};

#endif // FILE_MANAGER_INCLUDED
