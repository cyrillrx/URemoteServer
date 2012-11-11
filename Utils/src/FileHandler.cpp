#include "StdAfx.h"
#include "FileHandler.h"


FileHandler::FileHandler(const string& path)
{
	open(path.c_str(), ios::in);
	
}

FileHandler::~FileHandler()
{
	close();
}
