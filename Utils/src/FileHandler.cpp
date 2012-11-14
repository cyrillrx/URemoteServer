#include "StdAfx.h"
#include "FileHandler.h"

FileHandler::FileHandler(const string& path, const int openType)
{
	if (openType == OPEN_TYPE_READ) {
		m_File.open(path.c_str(), ios::in);
	} else if (openType == OPEN_TYPE_WRITE) {
		m_File.open(path.c_str(), ios::out);
	} else {
		throw OpenFileError();
	}
	
	if (!m_File.is_open()) {
		cout << "Error while opening " << path << endl;
		throw OpenFileError();
	} else {
		cout << "File " << path << " is open !" << endl;
	}
}

FileHandler::~FileHandler()
{
	if (m_File.is_open()) {
		m_File.close();
		cout << "File has been closed !" << endl;
	}
}

fstream& FileHandler::GetFile()
{
	return m_File;
}