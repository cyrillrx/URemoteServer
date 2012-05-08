#pragma once

#include <vector>
#include <string>

using namespace std;

class FileManager
{
private:
	static vector<string> ListFiles(string _dirPath);
public:
	static string ListFilesStr(string _dirPath);
	static string OpenFile(string _filePath);
};

