#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "FileException.h"

using namespace std;

class FileHandler
{
public:
	FileHandler(const string& path, const int openType);
	~FileHandler();
	
	fstream& GetFile();

	static const int OPEN_TYPE_READ		= 0;
	static const int OPEN_TYPE_WRITE	= 1;

private :
	fstream m_File;
};

