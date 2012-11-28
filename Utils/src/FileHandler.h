#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include "FileException.h"

class FileHandler
{
public:
	FileHandler(const std::string& path, const int openType);
	~FileHandler();
	
	std::fstream& GetFile();

	static const int OPEN_TYPE_READ		= 0;
	static const int OPEN_TYPE_WRITE	= 1;

private :
	std::fstream m_File;
};

