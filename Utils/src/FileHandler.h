#pragma once

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// exceptions
class file_error { } ;
class open_error : public file_error { } ;
class close_error : public file_error { } ;
class write_error : public file_error { } ;

using namespace std;

class FileHandler : public std::fstream
{
public:
	FileHandler(const string& path);
	~FileHandler();
};

