#include "StdAfx.h"
#include "FileHandler.h"

using namespace std;

FileHandler::FileHandler(const string& path, const int openType)
{
	if (openType == OPEN_TYPE_READ) {
		m_File.open(path.c_str(), ios::in);
	} else if (openType == OPEN_TYPE_WRITE) {
		m_File.open(path.c_str(), ios::out);
	} else {
		throw OpenFileException();
	}
	
	if (!m_File.is_open()) {
		throw OpenFileException();
	}

	cout << "File " << path << " is open !" << endl;
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