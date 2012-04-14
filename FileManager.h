#pragma once

#include <vector>
#include <string>

using namespace std;

class FileManager
{
private:
	static vector<string> ListFiles(string path);
public:
	static string ListFilesStr(string path);
};

