#pragma once

#include <string>

/**
 * Mother class for file exceptions.
 */
class FileException
{
public:
	FileException();
protected:
	std::string m_Message;
};

class OpenFileException  : public FileException
{
public:
	OpenFileException();
};

class CloseFileException : public FileException 
{
public:
	CloseFileException();
};

class ReadFileException  : public FileException 
{
public:
	ReadFileException();
};

class WriteFileException : public FileException 
{
public:
	WriteFileException();
};