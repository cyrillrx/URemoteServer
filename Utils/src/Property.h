#pragma once

#include <string>

class Property
{
public:
	Property();
	~Property();

	std::string Key;
	std::string Value;
	int Type;
};