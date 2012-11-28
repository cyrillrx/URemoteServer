#include "stdafx.h"
#include "FileException.h"

FileException::FileException(std::string message) : m_Message(message) { }

OpenFileException::OpenFileException()
	: FileException("OpenFileException : Error while opening file.")	{ }

CloseFileException::CloseFileException()
	: FileException("CloseFileException : Error while closing file.")	{ }

ReadFileException::ReadFileException()
	: FileException("ReadFileException : Error while reading file.")	{ }

WriteFileException::WriteFileException()
	: FileException("WriteFileException : Error while writting file.")	{ }