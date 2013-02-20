#pragma once

#include <string>

class ComHelper
{
public:
	ComHelper();
	~ComHelper();
	
	void checkResult(long result);

private :
	std::string getResultMessage(long result);
};