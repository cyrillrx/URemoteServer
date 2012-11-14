#pragma once

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// Exceptions
class FileError { } ;
class OpenFileError  : public FileError { } ;
class CloseFileError : public FileError { } ;
class ReadFileError  : public FileError { } ;
class WriteFileError : public FileError { } ;

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

