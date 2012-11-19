#include "stdafx.h"
#include "FileException.h"

FileException::FileException() { }

OpenFileException::OpenFileException()
{
	m_Message = "OpenFileException : Error while opening file."; 
}

CloseFileException::CloseFileException()
{
	m_Message = "CloseFileException : Error while closing file."; 
}

ReadFileException::ReadFileException()
{
	m_Message = "ReadFileException : Error while reading file."; 
}

WriteFileException::WriteFileException()
{
	m_Message = "WriteFileException : Error while writting file."; 
}